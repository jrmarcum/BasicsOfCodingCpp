// In the previous example we looked at setting up a simple
// HTTP server. HTTP servers are useful for demonstrating
// cancellation. Go's `context.Context` carries deadlines,
// cancellation signals, and other request-scoped values
// across API boundaries and goroutines. In C++ the standard
// building blocks for cancellation are `std::stop_source`
// and `std::stop_token`: the "context" of a request is a
// stop token that gets triggered when the client goes away.
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <format>
#include <mutex>
#include <print>
#include <stop_token>
#include <string>
#include <string_view>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
using socket_t = SOCKET;
constexpr socket_t invalid_socket = INVALID_SOCKET;
inline void close_socket(socket_t s) { closesocket(s); }
#else
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
constexpr socket_t invalid_socket = -1;
inline void close_socket(socket_t s) { close(s); }
#endif

using namespace std::chrono_literals;

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

void send_all(socket_t s, const std::string& data) {
    std::size_t sent = 0;
    while (sent < data.size()) {
        int n = send(s, data.data() + sent, static_cast<int>(data.size() - sent), 0);
        if (n <= 0) return;
        sent += n;
    }
}

void respond(socket_t c, int status, const char* reason, const std::string& body) {
    send_all(c, std::format("HTTP/1.1 {} {}\r\nContent-Type: text/plain; charset=utf-8\r\n"
                            "Content-Length: {}\r\nConnection: close\r\n\r\n{}",
                            status, reason, body.size(), body));
}

// Read until the blank line that ends the request headers and
// return the request path (the request line is all this
// example needs from the client).
bool read_path(socket_t c, std::string& path) {
    std::string raw;
    char buf[2048];
    while (raw.find("\r\n\r\n") == std::string::npos) {
        int n = recv(c, buf, sizeof buf, 0);
        if (n <= 0 || raw.size() > 65536) return false;
        raw.append(buf, n);
    }
    std::string_view line = std::string_view(raw).substr(0, raw.find("\r\n"));
    auto sp1 = line.find(' ');
    auto sp2 = line.find(' ', sp1 + 1);
    if (sp1 == std::string_view::npos || sp2 == std::string_view::npos) return false;
    path = line.substr(sp1 + 1, sp2 - sp1 - 1);
    return true;
}

// Return true if the client has closed its end of the
// connection (the socket is readable and `recv` yields 0/error).
bool client_gone(socket_t c) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(c, &fds);
    timeval tv{0, 100 * 1000};  // wait up to 100ms
    if (select(static_cast<int>(c) + 1, &fds, nullptr, nullptr, &tv) <= 0) return false;
    char b;
    return recv(c, &b, 1, 0) <= 0;
}

void hello(socket_t c) {
    // A "context" is created for each request: a stop source
    // whose token the handler can watch. A watcher thread
    // triggers it as soon as the client disconnects (Go's
    // `net/http` machinery does the same for `req.Context()`).
    std::stop_source ctx;
    std::stop_token done = ctx.get_token();
    std::jthread watcher([&](std::stop_token finished) {
        while (!finished.stop_requested()) {
            if (client_gone(c)) {
                ctx.request_stop();
                return;
            }
        }
    });

    std::println("server: hello handler started");
    std::fflush(stdout);

    // Wait for a few seconds before sending a reply to the
    // client. This could simulate some work the server is
    // doing. While working, keep an eye on the context's
    // stop token for a signal that we should cancel the work
    // and return as soon as possible. `wait_for` with a stop
    // token returns early when stop is requested.
    std::mutex m;
    std::condition_variable_any cv;
    {
        std::unique_lock lock(m);
        cv.wait_for(lock, done, 10s, [] { return false; });
    }
    bool cancelled = done.stop_requested();
    watcher.request_stop();
    watcher.join();

    if (!cancelled) {
        respond(c, 200, "OK", "hello\n");
    } else {
        // Go reports `ctx.Err()`, which reads "context canceled".
        std::println("server: context canceled");
        respond(c, 500, "Internal Server Error", "context canceled\n");
    }
    std::println("server: hello handler ended");
    std::fflush(stdout);
}

void serve(socket_t c) {
    std::string path;
    if (read_path(c, path) && path == "/hello") {
        hello(c);
    } else {
        respond(c, 404, "Not Found", "404 page not found\n");
    }
    close_socket(c);
}

int main() {
    NetInit net;

    // As before, we serve the "/hello" route.
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
        std::thread(serve, c).detach();
    }
}
