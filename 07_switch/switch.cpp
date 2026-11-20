// Switch statements express conditionals across many
// branches.
#include <ctime>
#include <print>
#include <string>
#include <type_traits>

int main() {
    // Here's a basic `switch`.
    int i = 2;
    std::print("Write {} as ", i);
    switch (i) {
        case 1:
            std::println("one");
            break;
        case 2:
            std::println("two");
            break;
        case 3:
            std::println("three");
            break;
    }

    // Get the current local time broken down into fields.
    std::time_t now = std::time(nullptr);
    std::tm lt = *std::localtime(&now);

    // C++ `case` labels can be stacked to share one body,
    // which plays the role of Go's comma-separated cases.
    // `tm_wday` is 0 for Sunday and 6 for Saturday. We use
    // the optional `default` case in this example as well.
    switch (lt.tm_wday) {
        case 6:
        case 0:
            std::println("It's the weekend");
            break;
        default:
            std::println("It's a weekday");
    }

    // A C++ `switch` needs a constant integral expression,
    // so the alternate way to express if/else logic is a
    // plain `if` chain. Here the conditions can be
    // non-constants.
    if (lt.tm_hour < 12) {
        std::println("It's before noon");
    } else {
        std::println("It's after noon");
    }

    // A type switch compares types instead of values. C++
    // does this at compile time with a generic lambda and
    // `if constexpr`; the parameter `v` has the type
    // corresponding to its branch.
    auto whatAmI = [](const auto& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, bool>) {
            std::println("I'm a bool");
        } else if constexpr (std::is_same_v<T, int>) {
            std::println("I'm an int");
        } else if constexpr (std::is_convertible_v<T, std::string>) {
            // Go prints the type with `%T`; C++ has no
            // portable type name, so we spell it out.
            std::println("Don't know type string");
        } else {
            std::println("Don't know type");
        }
    };
    whatAmI(true);
    whatAmI(1);
    whatAmI("hey");
}
