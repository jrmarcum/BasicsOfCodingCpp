// Command-line flags are a common way to specify options
// for command-line programs. For example, in `wc -l` the
// `-l` is a command-line flag.
#include <cstdlib>
#include <format>
#include <functional>
#include <map>
#include <print>
#include <span>
#include <string>
#include <vector>

// C++ has no `flag` package, so we write a tiny one that
// mirrors the behaviour of Go's: `-name=value`,
// `-name value`, `--name`, bare `-bool`, parsing stops at the
// first non-flag argument or `--`, and help/error text is
// printed in Go's format.
class FlagSet {
    struct Flag {
        std::string usage, type, def;
        bool is_bool = false;
        std::function<bool(const std::string&)> set;
    };
    std::string name_;
    std::map<std::string, Flag> flags_;  // sorted, like Go's help
    std::vector<std::string> args_;

public:
    explicit FlagSet(std::string name) : name_(std::move(name)) {}

    // Each declaration binds a flag to an existing variable
    // (like Go's `flag.StringVar`) and records its default.
    void string_var(std::string& p, const std::string& n, std::string def, const std::string& usage) {
        p = def;
        flags_[n] = {usage, "string", std::format("{:?}", def), false,
                     [&p](const std::string& v) { p = v; return true; }};
        if (def.empty()) flags_[n].def = "";
    }
    void int_var(int& p, const std::string& n, int def, const std::string& usage) {
        p = def;
        flags_[n] = {usage, "int", def == 0 ? "" : std::to_string(def), false,
                     [&p](const std::string& v) {
                         try {
                             size_t pos = 0;
                             int x = std::stoi(v, &pos, 0);
                             if (pos != v.size()) return false;
                             p = x;
                             return true;
                         } catch (...) { return false; }
                     }};
    }
    void bool_var(bool& p, const std::string& n, bool def, const std::string& usage) {
        p = def;
        flags_[n] = {usage, "", def ? "true" : "", true,
                     [&p](const std::string& v) {
                         if (v == "1" || v == "t" || v == "T" || v == "true" || v == "TRUE" || v == "True") { p = true; return true; }
                         if (v == "0" || v == "f" || v == "F" || v == "false" || v == "FALSE" || v == "False") { p = false; return true; }
                         return false;
                     }};
    }

    void usage() const {
        std::print(stderr, "Usage of {}:\n", name_);
        for (const auto& [n, f] : flags_) {
            std::string line = "  -" + n;
            if (!f.type.empty()) line += " " + f.type;
            std::print(stderr, "{}\n        {}", line, f.usage);
            if (!f.def.empty()) std::print(stderr, " (default {})", f.def);
            std::print(stderr, "\n");
        }
    }

    // Like Go's `ExitOnError`: print the problem plus usage
    // and exit with status 2 (help requests exit with 0).
    [[noreturn]] void fail(const std::string& msg) const {
        std::print(stderr, "{}\n", msg);
        usage();
        std::exit(2);
    }

    void parse(std::span<const std::string> args) {
        size_t i = 0;
        for (; i < args.size(); i++) {
            const std::string& a = args[i];
            if (a.size() < 2 || a[0] != '-') break;  // first non-flag
            if (a == "--") { i++; break; }
            std::string n = a.substr(a[1] == '-' ? 2 : 1);
            if (n.empty() || n[0] == '-' || n[0] == '=')
                fail(std::format("bad flag syntax: {}", a));
            std::string value;
            bool has_value = false;
            if (auto eq = n.find('='); eq != std::string::npos) {
                value = n.substr(eq + 1);
                n = n.substr(0, eq);
                has_value = true;
            }
            auto it = flags_.find(n);
            if (it == flags_.end()) {
                if (n == "h" || n == "help") { usage(); std::exit(0); }
                fail(std::format("flag provided but not defined: -{}", n));
            }
            auto& f = it->second;
            if (f.is_bool) {
                if (!f.set(has_value ? value : "true"))
                    fail(std::format("invalid boolean value {:?} for -{}: parse error", value, n));
            } else {
                if (!has_value) {
                    if (i + 1 >= args.size()) fail(std::format("flag needs an argument: -{}", n));
                    value = args[++i];
                }
                if (!f.set(value))
                    fail(std::format("invalid value {:?} for flag -{}: parse error", value, n));
            }
        }
        args_.assign(args.begin() + i, args.end());
    }

    // `args()` returns the trailing positional arguments.
    const std::vector<std::string>& args() const { return args_; }
};

// Go prints slices as `[a b c]`; this helper reproduces that.
std::string go_fmt(const std::vector<std::string>& v) {
    std::string out = "[";
    for (bool first = true; const auto& e : v) {
        if (!first) out += ' ';
        out += e;
        first = false;
    }
    return out + "]";
}

int main(int argc, char* argv[]) {
    FlagSet flags(argv[0]);
    // Basic flag declarations are available for string,
    // integer, and boolean options. Here we declare a
    // string flag `word` with a default value `"foo"`
    // and a short description. Each declaration binds the
    // flag to a variable we own (Go's `flag.String` returns
    // a pointer instead; here we pass the variable in, as
    // with Go's `flag.StringVar`).
    std::string word;
    int numb;
    bool fork;
    flags.string_var(word, "word", "foo", "a string");

    // This declares `numb` and `fork` flags, using a
    // similar approach to the `word` flag.
    flags.int_var(numb, "numb", 42, "an int");
    flags.bool_var(fork, "fork", false, "a bool");

    // It's also possible to declare an option that uses an
    // existing variable declared elsewhere in the program.
    // (All the flags above work the same way in C++.)
    std::string svar;
    flags.string_var(svar, "svar", "bar", "a string var");

    // Once all flags are declared, call `parse()`
    // to execute the command-line parsing.
    std::vector<std::string> args(argv + 1, argv + argc);
    flags.parse(args);

    // Here we'll just dump out the parsed options and
    // any trailing positional arguments.
    std::println("word: {}", word);
    std::println("numb: {}", numb);
    std::println("fork: {}", fork);
    std::println("svar: {}", svar);
    std::println("tail: {}", go_fmt(flags.args()));
}
