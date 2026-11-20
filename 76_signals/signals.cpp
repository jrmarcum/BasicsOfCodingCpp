// Sometimes we'd like our C++ programs to intelligently
// handle signals. For example, we might want a server
// to gracefully shutdown when it receives a `SIGTERM`, or
// a command-line tool to stop processing input if it
// receives a `SIGINT`. The standard library offers
// `std::signal`; the handler just sets a lock-free
// `std::atomic` flag that the main code polls.
#include <atomic>
#include <chrono>
#include <csignal>
#include <print>
#include <thread>

// The signal number that arrived (0 means none yet). Handlers may
// only touch lock-free atomics, so this is all they do.
std::atomic<int> received{0};

void on_signal(int sig) { received = sig; }

int main() {
    // `std::signal` registers a handler for SIGINT and SIGTERM
    // (the closest equivalent of `signal.NotifyContext`).
    std::signal(SIGINT, on_signal);
    std::signal(SIGTERM, on_signal);

    // The program will wait here until one of the
    // configured signals is received.
    std::println("awaiting signal");
    std::fflush(stdout);
    while (received == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Report why we stopped. Go's `context.Cause` prints the
    // signal; here we map the number to a message.
    std::println();
    std::println("{}", received == SIGINT ? "interrupt signal received"
                                          : "terminated signal received");
    std::println("exiting");
}
