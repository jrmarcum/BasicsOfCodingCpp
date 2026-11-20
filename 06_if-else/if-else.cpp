// Branching with `if` and `else` in C++ is
// straight-forward.
#include <print>

int main() {
    // Here's a basic example.
    if (7 % 2 == 0) {
        std::println("7 is even");
    } else {
        std::println("7 is odd");
    }

    // You can have an `if` statement without an else.
    if (8 % 4 == 0) {
        std::println("8 is divisible by 4");
    }

    // A statement can precede conditionals; any variables
    // declared in this statement are available in all
    // branches.
    if (int num = 9; num < 0) {
        std::println("{} is negative", num);
    } else if (num < 10) {
        std::println("{} has 1 digit", num);
    } else {
        std::println("{} has multiple digits", num);
    }
}

// Note that C++ requires parentheses around conditions,
// while the braces are optional (but recommended). Unlike
// Go, C++ does have a ternary `?:` operator for simple
// conditions.
