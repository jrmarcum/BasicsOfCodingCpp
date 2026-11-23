// A _goroutine_ is a lightweight thread of execution. The C++
// equivalent is a `std::jthread`, an OS thread that joins
// automatically when it goes out of scope.
#include <chrono>
#include <print>
#include <string>
#include <thread>

void f(const std::string& from) {
    for (int i = 0; i < 3; i++) {
        std::println("{} : {}", from, i);
    }
}

int main() {
    // Suppose we have a function call `f(s)`. Here's how
    // we'd call that in the usual way, running it
    // synchronously.
    f("direct");

    // To invoke this function in a thread, construct a
    // `std::jthread` with the function and its arguments.
    // The new thread executes concurrently with the calling
    // one.
    std::jthread g1(f, std::string("thread"));

    // You can also start a thread for an anonymous
    // function call (a lambda).
    std::jthread g2([](const std::string& msg) {
        std::println("{}", msg);
    }, std::string("going"));

    // Our two function calls are running asynchronously in
    // separate threads now. Wait for them to finish
    // (for a more robust approach, join the threads or use a
    // latch; a `jthread` also joins in its destructor).
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::println("done");
}
