// _Channels_ are the pipes that connect concurrent
// threads. You can send values into channels from one
// thread and receive those values in another thread.
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
};

int main() {
    // Create a new channel with `Channel<val-type>`.
    // Channels are typed by the values they convey.
    Channel<std::string> messages;

    // _Send_ a value into a channel using `send`. Here we
    // send `"ping"` to the `messages` channel we made above,
    // from a new thread.
    std::jthread sender([&] { messages.send("ping"); });

    // `recv` _receives_ a value from the channel. Here we'll
    // receive the `"ping"` message we sent above and print
    // it out.
    auto msg = messages.recv();
    std::println("{}", *msg);
}
