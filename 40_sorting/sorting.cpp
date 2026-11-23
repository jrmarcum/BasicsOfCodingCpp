// The C++ `<algorithm>` header (and its `std::ranges`
// counterparts) implements sorting for builtins and
// user-defined types. We'll look at sorting for builtins
// first.
#include <algorithm>
#include <print>
#include <string>
#include <vector>

// Go prints slices as `[a b c]`; this helper reproduces that.
template <typename T>
std::string cpp_fmt(const T& v) {
    if constexpr (std::ranges::range<T> && !std::is_convertible_v<T, std::string_view>) {
        std::string out = "[";
        for (bool first = true; const auto& e : v) {
            if (!first) out += ' ';
            out += cpp_fmt(e);
            first = false;
        }
        return out + "]";
    } else {
        return std::format("{}", v);
    }
}

int main() {
    // `std::ranges::sort` works on any random-access range, so
    // the same function handles strings and ints. Note that
    // sorting is in-place, so it changes the given vector and
    // doesn't return a new one.
    std::vector<std::string> strs = {"c", "a", "b"};
    std::ranges::sort(strs);
    std::println("Strings: {}", cpp_fmt(strs));

    // An example of sorting `int`s.
    std::vector<int> ints = {7, 2, 4};
    std::ranges::sort(ints);
    std::println("Ints:    {}", cpp_fmt(ints));

    // We can also check if a range is already in sorted order.
    bool s = std::ranges::is_sorted(ints);
    std::println("Sorted:  {}", s);
}
