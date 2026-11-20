// A `panic` typically means something went unexpectedly
// wrong. In C++ the closest equivalent is throwing an
// exception that nothing catches: the runtime calls
// `std::terminate`, which aborts the program. Mostly we use
// it to fail fast on errors that shouldn't occur during
// normal operation, or that we aren't prepared to handle
// gracefully.
#include <fstream>
#include <print>
#include <stdexcept>

int main() {
    // This is the only program on the site designed to
    // panic. We print the message ourselves (as Go does with
    // `panic: a problem`) and then let the exception escape
    // `main`, which terminates the program abnormally.
    std::println(stderr, "panic: a problem");
    throw std::runtime_error("a problem");

    // A common use of panic is to abort if a function
    // returns an error value that we don't know how to
    // (or want to) handle. Here's an example of throwing
    // if we get an unexpected error when creating a new file.
    std::ofstream f("/tmp/file");
    if (!f) {
        throw std::runtime_error("cannot create /tmp/file");
    }
}
