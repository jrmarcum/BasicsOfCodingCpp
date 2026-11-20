// The Go standard library provides straightforward
// tools for outputting logs from Go programs, with
// the `log` package for free-form output and the
// `log/slog` package for structured output. C++ has no
// standard logger, so here a small `Logger` class and a
// `slog_info` function are built on `std::format` and
// `<chrono>`.
#include <chrono>
#include <cstdio>
#include <ctime>
#include <format>
#include <initializer_list>
#include <print>
#include <source_location>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

// Flags that control the output format, like Go's `log.L*` flags.
enum Flags : unsigned {
    Ldate = 1,
    Ltime = 2,
    Lmicroseconds = 4,
    Lshortfile = 8,
    LstdFlags = Ldate | Ltime,
};

// Local wall-clock time, formatted like Go's `2006/01/02 15:04:05`
// with optional microseconds.
std::string clock_text(bool date, bool time, bool micro) {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t tt = system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&tt);
    std::string out;
    if (date) out += std::format("{:04}/{:02}/{:02} ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    if (time) {
        out += std::format("{:02}:{:02}:{:02}", tm.tm_hour, tm.tm_min, tm.tm_sec);
        if (micro) {
            auto us = duration_cast<microseconds>(now.time_since_epoch()).count() % 1000000;
            out += std::format(".{:06}", us);
        }
        out += ' ';
    }
    return out;
}

// A Logger writes lines of the form `prefix date time file:line: message`
// to any `FILE*` or string sink.
class Logger {
    std::FILE* out_ = nullptr;     // output target, if a stream
    std::string* buf_ = nullptr;   // output target, if a string buffer
    std::string prefix_;
    unsigned flags_;

public:
    Logger(std::FILE* out, std::string prefix, unsigned flags)
        : out_(out), prefix_(std::move(prefix)), flags_(flags) {}
    Logger(std::string* buf, std::string prefix, unsigned flags)
        : buf_(buf), prefix_(std::move(prefix)), flags_(flags) {}

    void set_flags(unsigned flags) { flags_ = flags; }
    void set_prefix(std::string prefix) { prefix_ = std::move(prefix); }

    // `std::source_location::current()` is the default argument, so
    // it captures the file and line of the *caller*.
    void println(std::string_view msg, std::source_location loc = std::source_location::current()) {
        std::string line = prefix_;
        line += clock_text(flags_ & Ldate, flags_ & (Ltime | Lmicroseconds), flags_ & Lmicroseconds);
        if (flags_ & Lshortfile) {
            std::string_view file = loc.file_name();
            file = file.substr(file.find_last_of("/\\") + 1);
            line += std::format("{}:{}: ", file, loc.line());
        }
        line += msg;
        line += '\n';
        if (buf_) *buf_ += line;
        else {
            std::fputs(line.c_str(), out_);
            std::fflush(out_);
        }
    }
};

// The structured logger: a JSON line with time, level, msg and
// any extra key/value pairs (Go's `slog.NewJSONHandler`).
using Value = std::variant<std::string, int>;

void slog_info(std::string_view msg, std::initializer_list<std::pair<std::string_view, Value>> attrs = {}) {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t tt = system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&tt);
    char zone[8];
    std::strftime(zone, sizeof zone, "%z", &tm);  // like -0700
    std::string z = zone;
    z.insert(3, ":");
    auto ns = duration_cast<nanoseconds>(now.time_since_epoch()).count() % 1000000000;
    std::string out = std::format(R"({{"time":"{:04}-{:02}-{:02}T{:02}:{:02}:{:02}.{:09}{}","level":"INFO","msg":"{}")",
                                  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                                  ns, z, msg);
    for (const auto& [key, val] : attrs) {
        if (auto s = std::get_if<std::string>(&val)) out += std::format(R"(,"{}":"{}")", key, *s);
        else out += std::format(R"(,"{}":{})", key, std::get<int>(val));
    }
    out += "}\n";
    std::fputs(out.c_str(), stderr);
}

int main() {
    // Simply invoking a function like `println` on the
    // _standard_ logger, which is pre-configured for reasonable
    // logging output to `stderr`.
    Logger std_log(stderr, "", LstdFlags);
    std_log.println("standard logger");

    // Loggers can be configured with _flags_ to set
    // their output format. By default, the standard
    // logger has the `Ldate` and `Ltime` flags
    // set, and these are collected in `LstdFlags`.
    // We can change its flags to emit time with
    // microsecond accuracy, for example.
    std_log.set_flags(LstdFlags | Lmicroseconds);
    std_log.println("with micro");

    // It also supports emitting the file name and
    // line from which the log function is called.
    std_log.set_flags(LstdFlags | Lshortfile);
    std_log.println("with file/line");

    // It may be useful to create a custom logger and
    // pass it around. When creating a new logger, we
    // can set a _prefix_ to distinguish its output
    // from other loggers.
    Logger mylog(stdout, "my:", LstdFlags);
    mylog.println("from mylog");

    // We can set the prefix on existing loggers (including
    // the standard one) with the `set_prefix` method.
    mylog.set_prefix("ohmy:");
    mylog.println("from mylog");

    // Loggers can have custom output targets; here we log
    // into a `std::string` buffer.
    std::string buf;
    Logger buflog(&buf, "buf:", LstdFlags);

    // This call writes the log output into `buf`.
    buflog.println("hello");

    // This will actually show it on standard output.
    std::print("from buflog:{}", buf);
    std::fflush(stdout);

    // Structured log output: for example, logging in JSON
    // format is straightforward.
    slog_info("hi there");

    // In addition to the message, the output can
    // contain an arbitrary number of key=value pairs.
    slog_info("hello again", {{"key", "val"}, {"age", 25}});
}
