// Writing a basic HTTP server is easy once you have a
// listening socket. The C++ standard library has no
// networking, so we use the operating system's socket API
// (WinSock2 on Windows, BSD sockets elsewhere) and
// implement a very small slice of HTTP/1.1 ourselves.
#include <cctype>
#include <functional>
#include <map>
#include <print>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
using socket_t = SOCKET;
constexpr socket_t invalid_socket = INVALID_SOCKET;
inline void close_socket(socket_t s) { closesocket(s); }
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
constexpr socket_t invalid_socket = -1;
inline void close_socket(socket_t s) { close(s); }
#endif

// RAII wrapper: starts WinSock on Windows (a no-op elsewhere).
struct NetInit {
    NetInit() {
#ifdef _WIN32
        WSADATA w;
        WSAStartup(MAKEWORD(2, 2), &w);
#endif
    }
    ~NetInit() {
#ifdef _WIN32
        WSACleanup();
#endif
    }
};

// A parsed request: the pieces a handler can look at.
struct Request {
    std::string method, path;
    // Header names in the order received, canonicalized
    // like Go does (`user-agent` -> `User-Agent`).
    std::vector<std::pair<std::string, std::string>> headers;
};

// The response a handler fills in (Go's `http.ResponseWriter`).
struct Response {
    int status = 200;
    std::string body;
};

// A fundamental concept in HTTP servers is *handlers*. A
// handler is any callable with this signature.
using Handler = std::function<void(Response&, const Request&)>;

void send_all(socket_t s, const std::string& data) {
    std::size_t sent = 0;
    while (sent < data.size()) {
        int n = send(s, data.data() + sent, static_cast<int>(data.size() - sent), 0);
        if (n <= 0) return;
        sent += n;
    }
}

// Read until the blank line that ends the request headers and
// parse them.
bool read_request(socket_t c, Request& req) {
    std::string raw;
    char buf[2048];
    while (raw.find("\r\n\r\n") == std::string::npos) {
        int n = recv(c, buf, sizeof buf, 0);
        if (n <= 0) return false;
        raw.append(buf, n);
        if (raw.size() > 65536) return false;
    }
    std::string_view v = raw;
    v = v.substr(0, v.find("\r\n\r\n"));
    bool first = true;
    while (!v.empty()) {
        auto eol = v.find("\r\n");
        std::string_view line = v.substr(0, eol);
        v = eol == std::string_view::npos ? std::string_view{} : v.substr(eol + 2);
        if (first) {
            first = false;
            auto sp1 = line.find(' ');
            auto sp2 = line.find(' ', sp1 + 1);
            if (sp1 == std::string_view::npos || sp2 == std::string_view::npos) return false;
            req.method = line.substr(0, sp1);
            req.path = line.substr(sp1 + 1, sp2 - sp1 - 1);
            continue;
        }
        auto colon = line.find(':');
        if (colon == std::string_view::npos) continue;
        std::string name(line.substr(0, colon));
        bool up = true;
        for (auto& ch : name) {
            ch = static_cast<char>(up ? std::toupper(ch) : std::tolower(ch));
            up = ch == '-';
        }
        std::string value(line.substr(colon + 1));
        value.erase(0, value.find_first_not_of(" \t"));
        // Go's server moves `Host` out of the header map; do the same.
        if (name != "Host") req.headers.emplace_back(name, value);
    }
    return !req.method.empty();
}

// Handle one connection: read, route, respond, close.
void serve(socket_t c, const std::map<std::string, Handler>& routes) {
    Request req;
    if (read_request(c, req)) {
        Response res;
        if (auto it = routes.find(req.path); it != routes.end()) {
            it->second(res, req);
        } else {
            res.status = 404;
            res.body = "404 page not found\n";
        }
        std::string head = std::format(
            "HTTP/1.1 {} {}\r\nContent-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: {}\r\nConnection: close\r\n\r\n",
            res.status, res.status == 200 ? "OK" : "Not Found", res.body.size());
        send_all(c, head + res.body);
    }
    close_socket(c);
}

// Functions serving as handlers take a `Response&` and a
// `Request&`. The response is used to fill in the HTTP
// response. Here our simple response is just "hello\n".
void hello(Response& w, const Request&) {
    w.body += "hello\n";
}

void headers(Response& w, const Request& req) {
    // This handler does something a little more
    // sophisticated by reading all the HTTP request
    // headers and echoing them into the response body.
    for (const auto& [name, value] : req.headers) {
        w.body += std::format("{}: {}\n", name, value);
    }
}

int main() {
    NetInit net;

    // We register our handlers on server routes in a map,
    // like Go's `http.HandleFunc` does for its default router.
    std::map<std::string, Handler> routes = {
        {"/hello", hello},
        {"/headers", headers},
    };

    // Finally, we create a listening socket on port 8090 and
    // accept connections forever (Go's `ListenAndServe`). Each
    // connection is served on its own detached thread.
    socket_t srv = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof opt);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8090);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(srv, reinterpret_cast<sockaddr*>(&addr), sizeof addr) != 0 || listen(srv, 16) != 0) {
        std::println(stderr, "listen tcp :8090: bind failed");
        return 1;
    }
    for (;;) {
        socket_t c = accept(srv, nullptr, nullptr);
        if (c == invalid_socket) continue;
        std::thread(serve, c, std::cref(routes)).detach();
    }
}
