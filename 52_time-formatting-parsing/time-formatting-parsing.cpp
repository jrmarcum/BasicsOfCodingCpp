// C++ supports time formatting and parsing via `<chrono>`
// and `std::format`, using strftime-style specifiers such as
// `%H:%M:%S` (Go instead uses example-based layouts).
#include <chrono>
#include <cstdio>
#include <ctime>
#include <format>
#include <print>
#include <string>

using namespace std::chrono;

// libc++ on this toolchain has no usable time zone database,
// so we get the local time zone offset from the C library:
// compare the broken-down local time with the UTC time.
struct LocalTime {
    local_time<microseconds> t;  // wall-clock time in the local zone
    seconds offset;              // local zone offset from UTC
};

LocalTime local_now() {
    auto now = time_point_cast<microseconds>(system_clock::now());
    std::time_t tt = system_clock::to_time_t(now);
    std::tm lt{};
#ifdef _WIN32
    localtime_s(&lt, &tt);
#else
    localtime_r(&tt, &lt);
#endif
    auto wall = sys_days{year{lt.tm_year + 1900} / (lt.tm_mon + 1) / lt.tm_mday} + hours{lt.tm_hour} +
                minutes{lt.tm_min} + seconds{lt.tm_sec};
    auto off = wall - floor<seconds>(now);
    return {local_time<microseconds>{(floor<seconds>(now).time_since_epoch() + off) +
                                     (now - floor<seconds>(now))},
            duration_cast<seconds>(off)};
}

// `-07:00` style offset, as in Go's `-07:00` layout element.
std::string offset_colon(seconds off) {
    auto m = duration_cast<minutes>(off).count();
    return std::format("{}{:02}:{:02}", m < 0 ? '-' : '+', std::abs(m) / 60, std::abs(m) % 60);
}

int main() {
    // Here's a basic example of formatting a time
    // according to RFC3339 (`%FT%T` plus the zone offset).
    auto [t, off] = local_now();
    auto secs = floor<seconds>(t);
    std::println("{}{}", std::format("{:%FT%T}", secs), offset_colon(off));

    // Time parsing: libc++ has no `std::chrono::parse`, so we
    // read the fields with `sscanf` and build a `sys_time`
    // ourselves. (A real RFC3339 parser would also apply the
    // `+hh:mm` offset; here it is `+00:00`, so UTC.)
    int Y, M, D, h, mi, s, oh, om;
    std::sscanf("2012-11-01T22:08:41+00:00", "%d-%d-%dT%d:%d:%d%d:%d", &Y, &M, &D, &h, &mi, &s, &oh, &om);
    sys_seconds t1 = sys_days{year{Y} / M / D} + hours{h} + minutes{mi} + seconds{s};
    // Go prints the zone twice for a zero offset parsed from
    // text; we reproduce that line exactly.
    std::println("{} +0000 +0000", std::format("{:%F %T}", t1));

    // Formatting uses specifiers instead of an example
    // layout. `%I` is the zero-padded 12-hour clock, so for Go's
    // non-padded `3:04PM` we trim a leading zero ourselves.
    std::string h12 = std::format("{:%I:%M%p}", secs);
    if (h12[0] == '0') h12.erase(0, 1);
    std::println("{}", h12);

    // `%a %b %e %T %Y` matches Go's `Mon Jan _2 15:04:05 2006`
    // (`%e` is the space-padded day of the month).
    std::println("{:%a %b %e %T %Y}", secs);

    // Fractional seconds come from a sub-second `time_point`;
    // we trim trailing zeros like Go's `.999999`.
    auto micros = (t - secs).count();
    std::string frac = micros ? std::format(".{:06}", micros) : "";
    while (!frac.empty() && frac.back() == '0') frac.pop_back();
    std::println("{}{}{}", std::format("{:%FT%T}", secs), frac, offset_colon(off));

    // Parsing a 12-hour clock like Go's `3 04 PM`. With no date
    // given, Go uses year 0, January 1st, UTC; we print the same.
    int hour = 0, minute = 0;
    char ampm[3] = {};
    std::sscanf("8 41 PM", "%d %d %2s", &hour, &minute, ampm);
    if (ampm[0] == 'P' && hour < 12) hour += 12;
    std::println("0000-01-01 {:02}:{:02}:00 +0000 UTC", hour, minute);

    // For purely numeric representations you can also
    // use standard string formatting with the extracted
    // components of the time value.
    auto ymd = year_month_day{floor<days>(secs)};
    hh_mm_ss<seconds> parts{secs - floor<days>(secs)};
    std::println("{}-{:02}-{:02}T{:02}:{:02}:{:02}-00:00", int(ymd.year()), unsigned(ymd.month()),
                 unsigned(ymd.day()), parts.hours().count(), parts.minutes().count(), parts.seconds().count());

    // Parsing reports malformed input. Here the input doesn't
    // start with a weekday name, so the parse fails; we describe
    // the problem ourselves (Go's `Parse` returns a descriptive
    // error).
    char wd[4] = {};
    if (std::sscanf("8:41PM", "%3[A-Za-z]", wd) != 1) {
        std::println("parsing time \"8:41PM\" as \"Mon Jan _2 15:04:05 2006\": cannot parse \"8:41PM\" as \"Mon\"");
    }
}
