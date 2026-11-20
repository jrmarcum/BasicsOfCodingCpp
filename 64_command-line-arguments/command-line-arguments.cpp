// Command-line arguments are a common way to parameterize
// execution of programs. For example, `zig run hello.cpp`
// uses `run` and `hello.cpp` as arguments to the `zig`
// program.
#include <print>
#include <span>
#include <string>
#include <vector>

// Go prints slices as `[a b c]`; this helper reproduces that.
std::string go_fmt(std::span<const std::string> v) {
    std::string out = "[";
    for (bool first = true; const auto& e : v) {
        if (!first) out += ' ';
        out += e;
        first = false;
    }
    return out + "]";
}

int main(int argc, char* argv[]) {
    // `argc`/`argv` give access to the raw command-line
    // arguments. Note that the first value is the path to
    // the program, and `args[1..]` holds the arguments to
    // the program. We copy them into a vector of strings.
    std::vector<std::string> args(argv, argv + argc);
    std::span<const std::string> argsWithProg = args;
    auto argsWithoutProg = argsWithProg.subspan(1);

    // You can get individual args with normal indexing.
    // (Guard it: unlike Go, C++ would not panic cleanly
    // on an out-of-range index.)
    std::string arg = args.size() > 3 ? args[3] : "";

    std::println("{}", go_fmt(argsWithProg));
    std::println("{}", go_fmt(argsWithoutProg));
    std::println("{}", arg);
}
