// A common requirement in programs is getting the number
// of seconds, milliseconds, or nanoseconds since the
// Unix epoch. Here's how to do it in C++ with `<chrono>`.
#include <chrono>
#include <cstdint>
#include <format>
#include <print>
#include <string>

using namespace std::chrono;
using Ns = nanoseconds;
using Time = sys_time<Ns>;

// Go prints times as `2012-10-31 16:13:58.292387 +0000 UTC`,
// trimming trailing zeros of the fractional second.
std::string go_time(Time t) {
    auto secs = floor<seconds>(t);
    auto frac = (t - secs).count();
    std::string f;
    if (frac != 0) {
        f = std::format(".{:09}", frac);
        while (f.back() == '0') f.pop_back();
    }
    return std::format("{:%Y-%m-%d %H:%M:%S}{} +0000 UTC", secs, f);
}

int main() {
    // `system_clock::now()` gives the current time. Its
    // `time_since_epoch()` is a duration since the Unix epoch
    // (guaranteed to be 1970-01-01 since C++20), which we can
    // express in seconds or nanoseconds with `duration_cast`.
    auto now = time_point_cast<Ns>(system_clock::now());
    std::int64_t secs = duration_cast<seconds>(now.time_since_epoch()).count();
    std::int64_t nanos = now.time_since_epoch().count();
    std::println("{}", go_time(now));

    // Milliseconds are just another duration unit here, but to
    // mirror the Go lesson we can also divide the nanoseconds.
    std::int64_t millis = nanos / 1000000;
    std::println("{}", secs);
    std::println("{}", millis);
    std::println("{}", nanos);

    // You can also convert integer seconds or nanoseconds
    // since the epoch into the corresponding time point.
    std::println("{}", go_time(Time{seconds{secs}}));
    std::println("{}", go_time(Time{Ns{nanos}}));
}
