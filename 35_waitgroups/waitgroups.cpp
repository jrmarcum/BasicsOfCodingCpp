// To wait for multiple threads to finish, we can
// join them all. (A `std::latch` is another option.)
#include <chrono>
#include <print>
#include <thread>
#include <vector>

// This is the function we'll run in every thread.
void worker(int id) {
    std::println("Worker {} starting", id);

    // Sleep to simulate an expensive task.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::println("Worker {} done", id);
}

int main() {
    // This vector holds the threads launched here so we can
    // wait for all of them to finish. Note: threads are
    // movable but not copyable.
    std::vector<std::jthread> wg;

    // Launch several threads.
    for (int i = 1; i <= 5; i++) {
        wg.emplace_back([i] { worker(i); });
    }

    // Block until all the threads started above are
    // done. A thread is done when the function it invokes
    // returns.
    for (auto& t : wg) {
        t.join();
    }
}
