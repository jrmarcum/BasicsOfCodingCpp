// Sometimes we'll want to sort a collection by something
// other than its natural order. For example, suppose we
// wanted to sort strings by their length instead of
// alphabetically. Here's an example of custom sorts
// in C++.
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
    // Unlike Go, we don't need a wrapper type implementing
    // `Len`, `Swap` and `Less`. We hand the sort a comparison
    // function (here a lambda) that holds the custom logic.
    // We want increasing string length, so we compare
    // `a.size()` and `b.size()`.
    std::vector<std::string> fruits = {"peach", "banana", "kiwi"};
    std::ranges::sort(fruits, [](const std::string& a, const std::string& b) {
        return a.size() < b.size();
    });
    std::println("{}", cpp_fmt(fruits));

    // A projection does the same thing even more tersely:
    // sort by the result of `size()` using the default `<`.
    std::vector<std::string> again = {"peach", "banana", "kiwi"};
    std::ranges::sort(again, {}, &std::string::size);
    std::println("{}", cpp_fmt(again));
}
