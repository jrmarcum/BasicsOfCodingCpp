// C++ lets you _recover_ from a thrown exception by
// using `try`/`catch`. A `catch` can stop a `throw` from
// aborting the program and let it continue with execution
// instead (Go's `panic` and `recover`).
#include <print>
#include <stdexcept>

// This function "panics".
void mayPanic() {
    throw std::runtime_error("a problem");
}

int main() {
    // A `catch` clause is the counterpart of `recover`.
    // When the code in the `try` block throws, control
    // jumps to the matching `catch`.
    try {
        mayPanic();

        // This code will not run, because `mayPanic` throws.
        // The execution of the `try` block stops at the point
        // of the throw and resumes in the `catch` clause.
        std::println("After mayPanic()");
    } catch (const std::exception& r) {
        // `what()` is the error raised in the call to `throw`.
        std::println("Recovered. Error:\n {}", r.what());
    }
}
