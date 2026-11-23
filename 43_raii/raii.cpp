// _Defer_ is used to ensure that a function call is
// performed later in a program's execution, usually for
// purposes of cleanup. In C++ this job belongs to RAII:
// an object's destructor runs when it goes out of scope,
// much like `finally` or `defer` in other languages.
#include <cstdlib>
#include <fstream>
#include <functional>
#include <print>
#include <stdexcept>
#include <string>
#include <utility>

// A tiny scope guard: it stores a callable and runs it in
// its destructor. This is the direct equivalent of `defer`.
class Defer {
public:
    explicit Defer(std::function<void()> f) : f_(std::move(f)) {}
    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;
    ~Defer() { f_(); }

private:
    std::function<void()> f_;
};

std::ofstream createFile(const std::string& p) {
    std::println("creating");
    std::ofstream f(p);
    if (!f) {
        throw std::runtime_error("cannot create " + p);
    }
    return f;
}

void writeFile(std::ofstream& f) {
    std::println("writing");
    f << "data\n";
}

void closeFile(std::ofstream& f) {
    std::println("closing");
    f.close();
    // It's important to check for errors when closing a
    // file, even in a deferred function.
    if (f.fail()) {
        std::println(stderr, "error: close failed");
        std::exit(1);
    }
}

// Suppose we wanted to create a file, write to it,
// and then close when we're done. Here's how we could
// do that with a scope guard.
int main() {
    // Immediately after getting a file object with
    // `createFile`, we create a `Defer` guard that closes
    // the file with `closeFile`. This will be executed at
    // the end of the enclosing scope (`main`), after
    // `writeFile` has finished.
    auto f = createFile("./tmp/defer.txt");
    Defer guard([&f] { closeFile(f); });
    writeFile(f);
}
