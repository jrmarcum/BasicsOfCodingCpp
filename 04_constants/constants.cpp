// C++ supports constants of character, string, boolean,
// and numeric values.
#include <cmath>
#include <cstdint>
#include <print>

// `constexpr` declares a constant value.
constexpr const char* s = "constant";

int main() {
    std::println("{}", s);

    // A `constexpr` declaration can appear anywhere a
    // variable declaration can.
    constexpr int n = 500000000;

    // Constant expressions are evaluated at compile time.
    // Unlike Go, C++ constants are typed, so the arithmetic
    // uses `double` rather than arbitrary precision.
    constexpr double d = 3e20 / n;
    // `{:g}` matches Go's `6e+11` style of float output.
    std::println("{:g}", d);

    // A numeric constant has a type; to change it, use an
    // explicit conversion such as `static_cast`.
    std::println("{}", static_cast<int64_t>(d));

    // Here `std::sin` expects a `double`; the `int` constant
    // `n` is converted implicitly.
    std::println("{}", std::sin(n));
}
