// C++ has no built-in _multiple return values_, but a
// function can return a `std::pair` or `std::tuple` and the
// caller unpacks it with structured bindings.
#include <print>
#include <utility>

// The `std::pair<int, int>` return type shows that the
// function returns 2 `int`s.
std::pair<int, int> vals() {
    return {3, 7};
}

int main() {
    // Here we use the 2 different return values from the
    // call with a structured binding.
    auto [a, b] = vals();
    std::println("{}", a);
    std::println("{}", b);

    // If you only want a subset of the returned values,
    // just use the member you need (C++ has no blank
    // identifier `_` for structured bindings).
    int c = vals().second;
    std::println("{}", c);
}
