// It's possible to define custom error types by
// deriving from a common `error` interface and implementing
// the `Error()` method on them. Here's a variant on the
// previous example that uses a custom type to explicitly
// represent an argument error.
#include <expected>
#include <format>
#include <memory>
#include <print>
#include <string>

// The base `error` interface: anything with an `Error()` method.
struct error {
    virtual ~error() = default;
    virtual std::string Error() const = 0;
};

// An error value is a shared pointer to the interface, so
// callers can later ask for the concrete type.
using error_ptr = std::shared_ptr<error>;

// A custom error type usually has the suffix "Error".
struct argError : error {
    int arg;
    std::string message;
    argError(int a, std::string m) : arg(a), message(std::move(m)) {}

    // Overriding this `Error` method makes `argError`
    // implement the `error` interface.
    std::string Error() const override {
        return std::format("{} - {}", arg, message);
    }
};

std::expected<int, error_ptr> f(int arg) {
    if (arg == 42) {
        // Return our custom error.
        return std::unexpected(std::make_shared<argError>(arg, "can't work with it"));
    }
    return arg + 3;
}

// The equivalent of `errors.As`: checks that a given error
// matches a specific error type and converts to a pointer of
// that type (null if there is no match).
template <typename T>
std::shared_ptr<T> errors_as(const error_ptr& err) {
    return std::dynamic_pointer_cast<T>(err);
}

int main() {
    // `errors_as` checks that a given error matches a
    // specific error type and converts to that type,
    // returning a non-null pointer. If there's no match, it
    // returns null.
    auto r = f(42);
    if (auto ae = r ? nullptr : errors_as<argError>(r.error())) {
        std::println("{}", ae->arg);
        std::println("{}", ae->message);
    } else {
        std::println("err doesn't match argError");
    }
}
