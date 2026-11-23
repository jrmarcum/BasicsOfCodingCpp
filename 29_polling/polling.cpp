// A Go-style _select_ lets you wait on multiple channel
// operations. In C++ we poll each channel with `try_recv`
// inside a loop.
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

    // Never blocks: returns std::nullopt if nothing is ready.
    std::optional<T> try_recv() {
        std::lock_guard lk(mu_);
        return pop();
    }
};

int main() {
    using namespace std::chrono_literals;

    // For our example we'll select across two channels.
    Channel<std::string> c1;
    Channel<std::string> c2;

    // Each channel will receive a value after some amount
    // of time, to simulate e.g. blocking RPC operations
    // executing in concurrent threads.
    std::jthread t1([&] {
        std::this_thread::sleep_for(1s);
        c1.send("one");
    });
    std::jthread t2([&] {
        std::this_thread::sleep_for(2s);
        c2.send("two");
    });

    // We'll "select" to await both of these values
    // simultaneously, printing each one as it arrives. Each
    // pass tries every channel without blocking, then naps
    // briefly if neither was ready.
    for (int received = 0; received < 2;) {
        if (auto msg1 = c1.try_recv()) {
            std::println("received {}", *msg1);
            received++;
        } else if (auto msg2 = c2.try_recv()) {
            std::println("received {}", *msg2);
            received++;
        } else {
            std::this_thread::sleep_for(5ms);
        }
    }
}
