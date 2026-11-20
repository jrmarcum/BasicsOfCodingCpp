// C++'s standard library has no networking, so for HTTP we
// talk to the operating system's socket API directly
// (WinSock2 on Windows, BSD sockets elsewhere) and speak
// HTTP/1.1 by hand. In this example we'll use it to issue a
// simple HTTP request.
#include <cstdlib>
#include <print>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
using socket_t = SOCKET;
constexpr socket_t invalid_socket = INVALID_SOCKET;
inline void close_socket(socket_t s) { closesocket(s); }
#else
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
constexpr socket_t invalid_socket = -1;
inline void close_socket(socket_t s) { close(s); }
#endif

// RAII wrapper: starts WinSock on Windows (a no-op elsewhere)
// and shuts it down when `main` is done.
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

// Open a TCP connection to host:port.
socket_t tcp_connect(const std::string& host, const std::string& port) {
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0)
        throw std::runtime_error("lookup " + host + ": no such host");
    socket_t s = invalid_socket;
    for (auto* p = res; p; p = p->ai_next) {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s == invalid_socket) continue;
        if (connect(s, p->ai_addr, static_cast<int>(p->ai_addrlen)) == 0) break;
        close_socket(s);
        s = invalid_socket;
    }
    freeaddrinfo(res);
    if (s == invalid_socket) throw std::runtime_error("dial tcp " + host + ":" + port + ": connection failed");
    return s;
}

// Decode a `Transfer-Encoding: chunked` body.
std::string dechunk(std::string_view raw) {
    std::string out;
    while (!raw.empty()) {
        auto eol = raw.find("\r\n");
        if (eol == std::string_view::npos) break;
        std::size_t size = std::stoul(std::string(raw.substr(0, eol)), nullptr, 16);
        if (size == 0) break;
        out += raw.substr(eol + 2, size);
        raw.remove_prefix(std::min(raw.size(), eol + 2 + size + 2));
    }
    return out;
}

int main(int argc, char* argv[]) {
    try {
        NetInit net;

        // Our host. Go's example fetches https://gobyexample.com,
        // but that site now redirects every plain-HTTP request to
        // HTTPS, which would need a TLS library. So we default to
        // example.com, which still answers on plain HTTP. Pass another
        // host as the first argument to try a different one.
        std::string host = argc > 1 ? argv[1] : "example.com";

        // Issue an HTTP GET request to a server: connect, then
        // write the request line and headers.
        socket_t s = tcp_connect(host, "80");
        std::string request = "GET / HTTP/1.1\r\nHost: " + host +
                              "\r\nUser-Agent: cpp-example\r\nAccept: */*\r\nConnection: close\r\n\r\n";
        send(s, request.data(), static_cast<int>(request.size()), 0);

        // Read the whole response (the server closes the connection
        // when it is done).
        std::string response;
        char buf[4096];
        for (int n; (n = recv(s, buf, sizeof buf, 0)) > 0;) response.append(buf, n);
        close_socket(s);

        // Split the response into status line, headers and body.
        auto head_end = response.find("\r\n\r\n");
        if (head_end == std::string::npos) throw std::runtime_error("malformed response");
        std::string head = response.substr(0, head_end);
        std::string body = response.substr(head_end + 4);
        if (head.find("hunked") != std::string::npos) body = dechunk(body);

        // Print the HTTP response status (`HTTP/1.1 200 OK` ->
        // `200 OK`).
        auto status_line = head.substr(0, head.find("\r\n"));
        std::println("Response status: {}", status_line.substr(status_line.find(' ') + 1));

        // Print the first 5 lines of the response body.
        std::istringstream lines(body);
        std::string line;
        for (int i = 0; i < 5 && std::getline(lines, line); i++) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            std::println("{}", line);
        }
    } catch (const std::exception& e) {
        std::println(stderr, "panic: {}", e.what());
        return 1;
    }
}
