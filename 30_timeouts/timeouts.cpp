// _Timeouts_ are important for programs that connect to
// external resources or that otherwise need to bound
// execution time. With a channel, `recv_for` makes a
// timeout easy.
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <print>
#include <string>
#include <thread>


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

    // Waits up to `d` for a value; returns std::nullopt on timeout or close.
    template <typename Rep, typename Period>
    std::optional<T> recv_for(std::chrono::duration<Rep, Period> d) {
        std::unique_lock lk(mu_);
        cv_.wait_for(lk, d, [&] { return closed_ || !q_.empty(); });
        return pop();
    }
};

int main() {
    using namespace std::chrono_literals;

    // For our example, suppose we're executing an external
    // call that returns its result on a channel `c1`
    // after 2s. Note that the channel is buffered, so the
    // send in the thread is nonblocking. This is a
    // common pattern to prevent threads from hanging in case
    // the channel is never read.
    Channel<std::string> c1(1);
    std::jthread t1([&] {
        std::this_thread::sleep_for(2s);
        c1.send("result 1");
    });

    // Here's the timeout. `recv_for(1s)` awaits the result
    // but gives up after 1s. If the operation takes more
    // than the allowed 1s we take the timeout branch.
    if (auto res = c1.recv_for(1s)) {
        std::println("{}", *res);
    } else {
        std::println("timeout 1");
    }

    // If we allow a longer timeout of 3s, then the receive
    // from `c2` will succeed and we'll print the result.
    Channel<std::string> c2(1);
    std::jthread t2([&] {
        std::this_thread::sleep_for(2s);
        c2.send("result 2");
    });
    if (auto res = c2.recv_for(3s)) {
        std::println("{}", *res);
    } else {
        std::println("timeout 2");
    }
}
