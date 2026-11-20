// `for` and `while` are C++'s looping constructs. Here are
// some basic types of loops.
#include <print>

int main() {
    // The most basic type, with a single condition
    // (a `while` loop).
    int i = 1;
    while (i <= 3) {
        std::println("{}", i);
        i = i + 1;
    }

    // A classic initial/condition/after `for` loop.
    for (int j = 7; j <= 9; j++) {
        std::println("{}", j);
    }

    // `for (;;)` without a condition will loop repeatedly
    // until you `break` out of the loop or `return` from
    // the enclosing function.
    for (;;) {
        std::println("loop");
        break;
    }

    // You can also `continue` to the next iteration of
    // the loop.
    for (int n = 0; n <= 5; n++) {
        if (n % 2 == 0) {
            continue;
        }
        std::println("{}", n);
    }
}
