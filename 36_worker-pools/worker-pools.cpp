// In this example we'll look at how to implement
// a _worker pool_ using threads and channels.
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <functional>
#include <print>
#include <thread>
#include <vector>

// C++ has no built-in channel type, so we build a small Go-style
// `Channel<T>`: a bounded queue guarded by a mutex and a condition
// variable. `recv` returns `std::nullopt` once the channel is closed
// and drained. A capacity of 0 or 1 behaves like a one-slot buffer.
template <typename T>
class Channel {
    std::mutex mu_;
    std::condition_variable cv_;
    std::deque<T> q_;
    std::size_t cap_;
    bool closed_ = false;

    std::optional<T> pop() {
        if (q_.empty()) return std::nullopt;
        T v = std::move(q_.front());
        q_.pop_front();
        cv_.notify_all();
        return v;
    }

public:
    explicit Channel(std::size_t capacity = 1) : cap_(std::max<std::size_t>(capacity, 1)) {}

    // Blocks while the buffer is full; returns false if the channel is closed.
    bool send(T v) {
        std::unique_lock lk(mu_);
        cv_.wait(lk, [&] { return closed_ || q_.size() < cap_; });
        if (closed_) return false;
        q_.push_back(std::move(v));
        cv_.notify_all();
        return true;
    }

    void close() {
        { std::lock_guard lk(mu_); closed_ = true; }
        cv_.notify_all();
    }

    // Blocks until a value arrives or the channel is closed and empty.
    std::optional<T> recv() {
        std::unique_lock lk(mu_);
        cv_.wait(lk, [&] { return closed_ || !q_.empty(); });
        return pop();
    }
};

// Here's the worker, of which we'll run several
// concurrent instances. These workers will receive
// work on the `jobs` channel and send the corresponding
// results on `results`. We'll sleep a second per job to
// simulate an expensive task.
void worker(int id, Channel<int>& jobs, Channel<int>& results) {
    while (auto j = jobs.recv()) {
        std::println("worker {} started  job {}", id, *j);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::println("worker {} finished job {}", id, *j);
        results.send(*j * 2);
    }
}

int main() {
    // In order to use our pool of workers we need to send
    // them work and collect their results. We make 2
    // channels for this.
    constexpr int numJobs = 5;
    Channel<int> jobs(numJobs);
    Channel<int> results(numJobs);

    // This starts up 3 workers, initially blocked
    // because there are no jobs yet.
    std::vector<std::jthread> pool;
    for (int w = 1; w <= 3; w++) {
        pool.emplace_back(worker, w, std::ref(jobs), std::ref(results));
    }

    // Here we send 5 `jobs` and then `close` that
    // channel to indicate that's all the work we have.
    for (int j = 1; j <= numJobs; j++) {
        jobs.send(j);
    }
    jobs.close();

    // Finally we collect all the results of the work.
    // This also ensures that the worker threads have
    // finished. (The jthreads also join on scope exit.)
    for (int a = 1; a <= numJobs; a++) {
        results.recv();
    }
}
