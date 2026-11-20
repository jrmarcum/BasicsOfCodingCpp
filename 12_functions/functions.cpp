// Functions are central in C++. We'll learn about
// functions with a few different examples.
#include <print>

// Here's a function that takes two `int`s and returns
// their sum as an `int`.
int plus(int a, int b) {
    // C++ requires explicit returns, i.e. it won't
    // automatically return the value of the last
    // expression.
    return a + b;
}

// C++ has no shorthand for consecutive parameters of the
// same type: each parameter declares its own type.
int plusPlus(int a, int b, int c) {
    return a + b + c;
}

int main() {
    // Call a function just as you'd expect, with
    // `name(args)`.
    int res = plus(1, 2);
    std::println("1+2 = {}", res);

    res = plusPlus(1, 2, 3);
    std::println("1+2+3 = {}", res);
}
