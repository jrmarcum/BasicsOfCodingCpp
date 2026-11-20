// In C++, an _array_ is a numbered sequence of elements of a
// specific length. `std::array` is the standard wrapper.
#include <array>
#include <print>
#include <ranges>
#include <string>

// Go prints arrays as `[a b c]`. C++23 can format ranges
// directly (as `[1, 2, 3]`), so this small helper reproduces
// Go's layout, recursing into nested arrays.
template <typename T>
std::string go_fmt(const T& v) {
    if constexpr (std::ranges::range<T>) {
        std::string out = "[";
        for (bool first = true; const auto& e : v) {
            if (!first) out += ' ';
            out += go_fmt(e);
            first = false;
        }
        return out + "]";
    } else {
        return std::format("{}", v);
    }
}

int main() {
    // Here we create an array `a` that will hold exactly
    // 5 `int`s. The type of elements and length are both
    // part of the array's type. `{}` zero-initializes the
    // array, which for `int`s means `0`s.
    std::array<int, 5> a{};
    std::println("emp: {}", go_fmt(a));

    // We can set a value at an index using the
    // `array[index] = value` syntax, and get a value with
    // `array[index]`.
    a[4] = 100;
    std::println("set: {}", go_fmt(a));
    std::println("get: {}", a[4]);

    // `size()` returns the length of an array.
    std::println("len: {}", a.size());

    // Use this syntax to declare and initialize an array
    // in one line.
    std::array<int, 5> b = {1, 2, 3, 4, 5};
    std::println("dcl: {}", go_fmt(b));

    // Array types are one-dimensional, but you can
    // compose types to build multi-dimensional data
    // structures.
    std::array<std::array<int, 3>, 2> twoD{};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            twoD[i][j] = i + j;
        }
    }
    std::println("2d:  {}", go_fmt(twoD));
}
