// In C++, variables are explicitly declared and used by
// the compiler to e.g. check type-correctness of function
// calls.
#include <print>
#include <string>

int main() {
    // `auto` declares a variable and infers its type from
    // the initializer.
    auto a = std::string("initial");
    std::println("{}", a);

    // You can declare multiple variables of one type at once.
    int b = 1, c = 2;
    std::println("{} {}", b, c);

    // C++ will infer the type of initialized variables
    // if not explicitly defined.
    auto d = true;
    std::println("{}", d);

    // Variables declared without a corresponding
    // initialization are _indeterminate_ in C++ (unlike Go's
    // zero values), so we value-initialize with `{}`, which
    // gives the zero value. For an `int` that is `0`.
    int e{};
    std::println("{}", e);

    // `auto` with `=` is the closest match to Go's `:=`
    // shorthand for declaring and initializing a variable,
    // e.g. for `std::string f = "apple"` in this case.
    auto f = std::string("apple");
    std::println("{}", f);
}
