// _Rate limiting_ is an important mechanism for controlling
// resource utilization and maintaining quality of service.
// C++ supports rate limiting with threads, channels, and
// tickers.
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <format>
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

    // Never blocks: drops the value if the buffer is full or closed.
    bool try_send(T v) {
        std::lock_guard lk(mu_);
        if (closed_ || q_.size() >= cap_) return false;
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

// A Ticker sends the current time on channel `C` at a regular
// interval, from its own thread, until it is stopped. Ticks are
// dropped if the receiver is too slow, just like Go's tickers.
class Ticker {
    std::jthread t_;

public:
    Channel<std::chrono::system_clock::time_point> C{1};

    explicit Ticker(std::chrono::milliseconds d)
        : t_([this, d](std::stop_token st) {
              std::mutex m;
              std::condition_variable_any cv;
              std::unique_lock lk(m);
              auto next = std::chrono::system_clock::now() + d;
              while (!st.stop_requested()) {
                  cv.wait_until(lk, st, next, [] { return false; });
                  if (st.stop_requested()) break;
                  C.try_send(std::chrono::system_clock::now());
                  next += d;
              }
          }) {}

    void stop() {
        t_.request_stop();
        C.close();
    }
};

// Formats the current time as `2006-01-02 15:04:05.000000` (UTC).
std::string now() {
    return std::format("{:%F %T}", std::chrono::floor<std::chrono::microseconds>(std::chrono::system_clock::now()));
}

int main() {
    using namespace std::chrono_literals;

    // First we'll look at basic rate limiting. Suppose
    // we want to limit our handling of incoming requests.
    // We'll serve these requests off a channel of the
    // same name.
    Channel<int> requests(5);
    for (int i = 1; i <= 5; i++) {
        requests.send(i);
    }
    requests.close();

    // This `limiter` ticker will deliver a value
    // every 200 milliseconds. This is the regulator in
    // our rate limiting scheme.
    Ticker limiter(200ms);

    // By blocking on a receive from the `limiter` channel
    // before serving each request, we limit ourselves to
    // 1 request every 200 milliseconds.
    while (auto req = requests.recv()) {
        limiter.C.recv();
        std::println("request {} {}", *req, now());
    }

    // We may want to allow short bursts of requests in
    // our rate limiting scheme while preserving the
    // overall rate limit. We can accomplish this by
    // buffering our limiter channel. This `burstyLimiter`
    // channel will allow bursts of up to 3 events.
    Channel<std::chrono::system_clock::time_point> burstyLimiter(3);

    // Fill up the channel to represent allowed bursting.
    for (int i = 0; i < 3; i++) {
        burstyLimiter.send(std::chrono::system_clock::now());
    }

    // Every 200 milliseconds we'll try to add a new
    // value to `burstyLimiter`, up to its limit of 3.
    std::jthread feeder([&] {
        while (auto t = limiter.C.recv()) {
            if (!burstyLimiter.send(*t)) break;
        }
    });

    // Now simulate 5 more incoming requests. The first
    // 3 of these will benefit from the burst capability
    // of `burstyLimiter`.
    Channel<int> burstyRequests(5);
    for (int i = 1; i <= 5; i++) {
        burstyRequests.send(i);
    }
    burstyRequests.close();
    while (auto req = burstyRequests.recv()) {
        burstyLimiter.recv();
        std::println("request {} {}", *req, now());
    }

    // Shut the feeder down so the program can exit.
    limiter.stop();
    burstyLimiter.close();
}
