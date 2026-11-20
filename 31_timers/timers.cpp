// We often want to execute code at some point in the
// future, or repeatedly at some interval. Timers and
// tickers make both of these tasks easy. We'll look first
// at timers and then at tickers. Here a timer is a small
// class built from a `std::jthread`, a `std::stop_token`
// and a channel.
#include <atomic>
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <print>
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

// A Timer represents a single event in the future. It sleeps
// on its own thread, then sends a value on channel `C`.
// `stop()` cancels it if it has not fired yet.
class Timer {
    std::atomic<int> state_{0};  // 0 pending, 1 fired, 2 stopped
    std::jthread t_;

public:
    Channel<bool> C{1};

    explicit Timer(std::chrono::milliseconds d)
        : t_([this, d](std::stop_token st) {
              std::mutex m;
              std::condition_variable_any cv;
              std::unique_lock lk(m);
              cv.wait_for(lk, st, d, [] { return false; });
              int expected = 0;
              if (!st.stop_requested() && state_.compare_exchange_strong(expected, 1)) {
                  C.send(true);
              }
          }) {}

    // Returns true if the call stopped the timer, false if it
    // had already fired or been stopped.
    bool stop() {
        int expected = 0;
        bool stopped = state_.compare_exchange_strong(expected, 2);
        t_.request_stop();
        C.close();  // also releases anyone still waiting on C
        return stopped;
    }
};

int main() {
    using namespace std::chrono_literals;

    // Timers represent a single event in the future. You
    // tell the timer how long you want to wait, and it
    // provides a channel that will be notified at that
    // time. This timer will wait 2 seconds.
    Timer timer1(2s);

    // `timer1.C.recv()` blocks on the timer's channel `C`
    // until it sends a value indicating that the timer
    // fired.
    timer1.C.recv();
    std::println("Timer 1 fired");

    // If you just wanted to wait, you could have used
    // `sleep_for`. One reason a timer may be useful is
    // that you can cancel the timer before it fires.
    // Here's an example of that.
    Timer timer2(1s);
    std::jthread waiter([&] {
        if (timer2.C.recv()) {
            std::println("Timer 2 fired");
        }
    });
    bool stop2 = timer2.stop();
    if (stop2) {
        std::println("Timer 2 stopped");
    }

    // Give the `timer2` enough time to fire, if it ever
    // was going to, to show it is in fact stopped.
    std::this_thread::sleep_for(2s);
}
