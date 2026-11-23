// C++ offers extensive support for times and durations in
// the `<chrono>` header; here are some examples. A time point
// is a `std::chrono::time_point` and an interval is a
// `std::chrono::duration`.
#include <charconv>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <format>
#include <print>
#include <string>

using namespace std::chrono;
using Ns = nanoseconds;
using Time = sys_time<Ns>;  // a UTC time point with nanosecond precision

// Go prints times as `2009-11-17 20:34:58.651387237 +0000 UTC`,
// trimming trailing zeros of the fractional second.
std::string cpp_time(Time t) {
    auto secs = floor<seconds>(t);
    auto frac = (t - secs).count();
    std::string f;
    if (frac != 0) {
        f = std::format(".{:09}", frac);
        while (f.back() == '0') f.pop_back();
    }
    return std::format("{:%Y-%m-%d %H:%M:%S}{} +0000 UTC", secs, f);
}

// Go prints durations like `25891h15m15.142266763s`.
std::string cpp_duration(Ns d) {
    auto n = d.count();
    std::string out = n < 0 ? "-" : "";
    n = n < 0 ? -n : n;
    auto h = n / 3'600'000'000'000;
    auto m = n / 60'000'000'000 % 60;
    auto s = n % 60'000'000'000;
    if (h) out += std::format("{}h", h);
    if (h || m) out += std::format("{}m", m);
    std::string sec = std::format("{}.{:09}", s / 1'000'000'000, s % 1'000'000'000);
    while (sec.back() == '0') sec.pop_back();
    if (sec.back() == '.') sec.pop_back();
    return out + sec + "s";
}

// Go prints a float64 with `%v`: the shortest representation that
// round-trips, switching to `e+NN` notation once the exponent is
// below -4 or at least 6 (so 1234567.0 prints as 1.234567e+06).
std::string cpp_float(double v) {
    char buf[64];
    auto r = std::to_chars(buf, buf + sizeof buf, v, std::chars_format::scientific);
    std::string sci(buf, r.ptr);
    int exp = std::stoi(sci.substr(sci.find('e') + 1));
    if (exp < -4 || exp >= 6) return sci;
    return std::format("{}", v);
}

int main() {
    // We'll start by getting the current time.
    Time now = time_point_cast<Ns>(system_clock::now());
    std::println("{}", cpp_time(now));

    // You can build a time point by providing the year,
    // month, day, etc. Times here are always UTC (`sys_time`).
    Time then = sys_days{2009y / November / 17} + hours{20} + minutes{34} + seconds{58} + Ns{651387237};
    std::println("{}", cpp_time(then));

    // You can extract the various components of the time
    // value as expected.
    auto days = floor<std::chrono::days>(then);
    year_month_day ymd{days};
    hh_mm_ss<Ns> hms{then - days};
    std::println("{}", int(ymd.year()));
    std::println("{:%B}", ymd.month());
    std::println("{}", unsigned(ymd.day()));
    std::println("{}", hms.hours().count());
    std::println("{}", hms.minutes().count());
    std::println("{}", hms.seconds().count());
    std::println("{}", hms.subseconds().count());
    std::println("UTC");

    // The Monday-Sunday `weekday` is also available.
    std::println("{:%A}", weekday{days});

    // Time points compare with the ordinary operators, testing
    // if the first occurs before, after, or at the same time
    // as the second, respectively.
    std::println("{}", then < now);
    std::println("{}", then > now);
    std::println("{}", then == now);

    // Subtracting two time points returns a duration
    // representing the interval between them.
    auto diff = now - then;
    std::println("{}", cpp_duration(diff));

    // We can compute the length of the duration in
    // various units. Floating-point durations avoid the
    // truncation that integer `duration_cast` does.
    std::println("{}", cpp_float(duration<double, std::ratio<3600>>(diff).count()));
    std::println("{}", cpp_float(duration<double, std::ratio<60>>(diff).count()));
    std::println("{}", cpp_float(duration<double>(diff).count()));
    std::println("{}", diff.count());

    // You can use `+` to advance a time by a given
    // duration, or `-` to move backwards by a
    // duration.
    std::println("{}", cpp_time(then + diff));
    std::println("{}", cpp_time(then - diff));
}
