// In Go it's idiomatic to communicate errors via an
// explicit, separate return value. In C++23 the closest
// equivalent is `std::expected<T, E>`, which holds either
// a value or an error. This makes it easy to see which
// functions return errors and to handle them using the
// same language constructs employed for any other,
// non-error tasks.
#include <expected>
#include <format>
#include <print>
#include <string>

// `std::expected<int, std::string>` holds either an `int`
// result or an error message.
std::expected<int, std::string> f1(int arg) {
    if (arg == 42) {
        // `std::unexpected` constructs the error side
        // with the given error message.
        return std::unexpected("can't work with 42");
    }

    // Returning a plain value indicates that there was
    // no error.
    return arg + 3;
}

// It's possible to use custom types as errors. Here's a
// variant on the example above that uses a custom type
// to explicitly represent an argument error.
struct argError {
    int arg;
    std::string prob;

    std::string Error() const {
        return std::format("{} - {}", arg, prob);
    }
};

std::expected<int, argError> f2(int arg) {
    if (arg == 42) {
        // In this case we build an `argError`, supplying
        // values for the two fields `arg` and `prob`.
        return std::unexpected(argError{arg, "can't work with it"});
    }
    return arg + 3;
}

int main() {
    // The two loops below test out each of our
    // error-returning functions. Note that the inline
    // declaration in the `if` line is a common idiom.
    for (int i : {7, 42}) {
        if (auto r = f1(i); !r) {
            std::println("f1 failed: {}", r.error());
        } else {
            std::println("f1 worked: {}", *r);
        }
    }
    for (int i : {7, 42}) {
        if (auto r = f2(i); !r) {
            std::println("f2 failed: {}", r.error().Error());
        } else {
            std::println("f2 worked: {}", *r);
        }
    }

    // If you want to programmatically use the data in
    // a custom error, you can read its fields directly,
    // since `error()` returns the concrete `argError` type.
    auto e = f2(42);
    if (!e) {
        const argError& ae = e.error();
        std::println("{}", ae.arg);
        std::println("{}", ae.prob);
    }
}
