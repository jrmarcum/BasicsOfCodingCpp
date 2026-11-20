// For more complex state we can use a _mutex_ to safely
// access data across multiple threads.
#include <map>
#include <mutex>
#include <print>
#include <string>
#include <thread>
#include <vector>

// Container holds a map of counters; since we want to
// update it concurrently from multiple threads, we
// add a `std::mutex` to synchronize access.
// Note that mutexes must not be copied (or moved), so if this
// `struct` is passed around, it should be done by
// reference.
struct Container {
    std::mutex mu;
    std::map<std::string, int> counters;

    void inc(const std::string& name) {
        // Lock the mutex before accessing `counters`; the
        // `lock_guard` unlocks it automatically at the end
        // of the function (RAII, Go's `defer`).
        std::lock_guard lock(mu);
        counters[name]++;
    }
};

int main() {
    Container c;
    c.counters = {{"a", 0}, {"b", 0}};

    // This function increments a named counter
    // in a loop.
    auto doIncrement = [&c](const std::string& name, int n) {
        for (int i = 0; i < n; i++) {
            c.inc(name);
        }
    };

    // Run several threads concurrently; note
    // that they all access the same `Container`,
    // and two of them access the same counter.
    {
        std::vector<std::jthread> wg;
        wg.emplace_back(doIncrement, "a", 10000);
        wg.emplace_back(doIncrement, "a", 10000);
        wg.emplace_back(doIncrement, "b", 10000);
        // Leaving this scope joins the threads, which waits
        // for them to finish.
    }

    // Go prints maps as `map[k:v ...]`.
    std::print("map[");
    for (bool first = true; const auto& [name, count] : c.counters) {
        if (!first) std::print(" ");
        std::print("{}:{}", name, count);
        first = false;
    }
    std::println("]");
}
