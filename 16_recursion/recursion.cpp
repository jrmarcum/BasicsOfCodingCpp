// C++ supports recursive functions.
// Here's a classic factorial example.
#include <print>

// This `fact` function calls itself until it reaches the
// base case of `fact(0)`.
int fact(int n) {
    if (n == 0) {
        return 1;
    }
    return n * fact(n - 1);
}

int main() {
    std::println("{}", fact(7));
}
