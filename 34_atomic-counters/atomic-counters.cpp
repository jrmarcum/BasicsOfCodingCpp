// Here we'll look at using `std::atomic` for
// _atomic counters_ accessed by multiple threads.
#include <atomic>
#include <cstdint>
#include <print>
#include <thread>
#include <vector>

int main() {
    // We'll use an atomic integer type to represent our
    // (always-positive) counter.
    std::atomic<std::uint64_t> ops{0};

    // A vector of threads will help us wait for all of them
    // to finish their work.
    std::vector<std::jthread> wg;

    // We'll start 50 threads that each increment the
    // counter exactly 1000 times.
    for (int i = 0; i < 50; i++) {
        wg.emplace_back([&ops] {
            for (int j = 0; j < 1000; j++) {
                // To atomically increment the counter we use `fetch_add`.
                ops.fetch_add(1);
            }
        });
    }

    // Wait until all the threads are done.
    for (auto& t : wg) {
        t.join();
    }

    // Here no threads are writing to `ops`, but using
    // `load` it's safe to atomically read a value even while
    // other threads are (atomically) updating it.
    std::println("ops: {}", ops.load());
}
