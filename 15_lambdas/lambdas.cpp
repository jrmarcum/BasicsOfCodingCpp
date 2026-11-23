// C++ supports anonymous functions, called lambdas.
// Lambdas are useful when you want to define a function
// inline without having to name it.
#include <functional>
#include <print>

// This function `intSeq` returns another function, which
// we define anonymously in the body of `intSeq`. The
// returned lambda _captures_ the variable `i` by value (and
// is `mutable` so it may modify its own copy) to form a
// closure.
std::function<int()> intSeq() {
    int i = 0;
    return [i]() mutable {
        i++;
        return i;
    };
}

int main() {
    // We call `intSeq`, assigning the result (a function)
    // to `nextInt`. This function value holds its own `i`
    // value, which will be updated each time we call
    // `nextInt`.
    auto nextInt = intSeq();

    // See the effect of the closure by calling `nextInt`
    // a few times.
    std::println("{}", nextInt());
    std::println("{}", nextInt());
    std::println("{}", nextInt());

    // To confirm that the state is unique to that
    // particular function, create and test a new one.
    auto newInts = intSeq();
    std::println("{}", newInts());
}
