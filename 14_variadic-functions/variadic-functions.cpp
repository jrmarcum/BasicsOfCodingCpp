// Variadic functions can be called with any number of
// trailing arguments. In C++ we can use a variadic template
// (parameter pack) or take a `std::span`.
#include <print>
#include <span>
#include <string>
#include <vector>

// Go prints slices as `[a b c]`; this helper reproduces it.
std::string cpp_fmt(std::span<const int> v) {
    std::string out = "[";
    for (bool first = true; int e : v) {
        if (!first) out += ' ';
        out += std::format("{}", e);
        first = false;
    }
    return out + "]";
}

// Here's a function that takes a span of `int`s: the shared
// implementation used by both calling styles below.
void sum(std::span<const int> nums) {
    std::print("{} ", cpp_fmt(nums));
    int total = 0;
    for (int num : nums) {
        total += num;
    }
    std::println("{}", total);
}

// A variadic template takes an arbitrary number of `int`s
// as individual arguments (`nums...` is the parameter pack).
template <typename... Ints>
void sum(Ints... nums) {
    std::vector<int> v{static_cast<int>(nums)...};
    sum(std::span<const int>(v));
}

int main() {
    // Variadic functions can be called in the usual way
    // with individual arguments.
    sum(1, 2);
    sum(1, 2, 3);

    // If you already have multiple args in a vector, pass it
    // as a span, like Go's `func(slice...)`.
    std::vector<int> nums = {1, 2, 3, 4};
    sum(std::span<const int>(nums));
}
