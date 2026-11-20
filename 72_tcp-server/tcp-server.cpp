// The C++ standard library has no networking, so to build a TCP
// socket server we use the operating system's sockets API:
// WinSock2 on Windows and BSD sockets on POSIX systems.
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <print>
#include <string>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
using sock_t = SOCKET;
constexpr sock_t bad_sock = INVALID_SOCKET;
inline void close_sock(sock_t s) { closesocket(s); }
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using sock_t = int;
constexpr sock_t bad_sock = -1;
inline void close_sock(sock_t s) { close(s); }
#endif

// RAII wrapper that closes a socket when it goes out of
// scope: C++'s equivalent of Go's `defer conn.Close()`.
struct Socket {
    sock_t fd;
    explicit Socket(sock_t s) : fd(s) {}
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    ~Socket() { if (fd != bad_sock) close_sock(fd); }
};

// `handle_connection` handles a single client connection,
// reading one line of text from the client and returning a response.
void handle_connection(sock_t fd) {
    // Closing the connection releases resources when
    // we are finished interacting with the client.
    Socket conn(fd);

    // Read one line of data from the client (terminated by a newline).
    std::string message;
    char c;
    while (true) {
        int n = recv(conn.fd, &c, 1, 0);
        if (n <= 0) {
            std::println(stderr, "Read error: connection closed before newline");
            return;
        }
        message += c;
        if (c == '\n') break;
    }

    // Create and send a response back to the client,
    // demonstrating two-way communication.
    auto first = message.find_first_not_of(" \t\r\n");
    auto last = message.find_last_not_of(" \t\r\n");
    std::string ack = first == std::string::npos ? "" : message.substr(first, last - first + 1);
    std::ranges::transform(ack, ack.begin(), [](unsigned char ch) { return std::toupper(ch); });
    std::string response = "ACK: " + ack + "\n";
    if (send(conn.fd, response.data(), (int)response.size(), 0) < 0) {
        std::println(stderr, "Server write error");
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::println(stderr, "WSAStartup failed");
        return 1;
    }
#endif

    // `socket`, `bind` and `listen` start the server on the TCP
    // network at port 8090 on all interfaces.
    Socket listener(socket(AF_INET, SOCK_STREAM, 0));
    int yes = 1;
    setsockopt(listener.fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof yes);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8090);
    if (bind(listener.fd, (sockaddr*)&addr, sizeof addr) < 0 || listen(listener.fd, 16) < 0) {
        std::println(stderr, "Error listening");
        return 1;
    }
    // The `listener` guard closes the socket to free the port
    // when the application exits.

    // Loop indefinitely to accept new client connections.
    while (true) {
        // Wait for a connection.
        sock_t conn = accept(listener.fd, nullptr, nullptr);
        if (conn == bad_sock) {
            std::println(stderr, "Error accepting conn");
            continue;
        }

        // We use a thread here to handle the connection
        // so that the main loop can continue accepting more
        // connections.
        std::jthread(handle_connection, conn).detach();
    }
}
