// Use `std::exit` to immediately exit with a given
// status.
#include <cstdlib>
#include <print>

// C++'s equivalent of Go's `defer` is an RAII guard: its
// destructor runs when the scope ends.
struct Defer {
    ~Defer() { std::println("!"); }
};

int main() {
    // Destructors of local objects will _not_ be run when
    // using `std::exit`, so this `std::println` will never
    // be called.
    Defer defer;

    // Exit with status 3.
    std::exit(3);
}

// Note that unlike Go, C++ _does_ use the integer return
// value from `main` to indicate exit status (`return 3;`).
// But `return` unwinds the stack and runs destructors,
// whereas `std::exit` (like Go's `os.Exit`) does not.
