// Unit testing is an important part of writing
// principled programs. Go has a `testing` package and a
// `go test` command; C++ has no test runner in the standard
// library, so this lesson builds a tiny self-contained one
// that prints Go-test-like output (`-v` style), plus a
// `<chrono>` benchmark loop.
#include <chrono>
#include <cstdlib>
#include <format>
#include <functional>
#include <memory>
#include <print>
#include <string>
#include <vector>

using Clock = std::chrono::steady_clock;

// --- A miniature `testing` package -------------------------------

// `T` plays the role of Go's `*testing.T`.
struct T {
    std::string name;
    bool failed = false;
    double seconds = 0;
    std::vector<std::string> messages;
    std::vector<std::unique_ptr<T>> children;

    explicit T(std::string n) : name(std::move(n)) {}

    // `error` will report test failures but continue
    // executing the test (like `t.Errorf`). A test that
    // should stop immediately could `return` after calling it.
    void error(const std::string& msg) {
        failed = true;
        messages.push_back(msg);
    }

    // `run` enables running "subtests", one for each
    // table entry. These are shown separately like
    // `go test -v` does.
    void run(const std::string& subname, const std::function<void(T&)>& fn) {
        auto child = std::make_unique<T>(name + "/" + subname);
        std::println("=== RUN   {}", child->name);
        auto start = Clock::now();
        fn(*child);
        child->seconds = std::chrono::duration<double>(Clock::now() - start).count();
        if (child->failed) failed = true;
        children.push_back(std::move(child));
    }
};

// Print the `--- PASS/FAIL` lines for a test and, indented,
// its subtests.
void report(const T& t, int depth = 0) {
    std::string indent(depth * 4, ' ');
    std::println("{}--- {}: {} ({:.2f}s)", indent, t.failed ? "FAIL" : "PASS", t.name, t.seconds);
    for (const auto& m : t.messages) std::println("{}    {}", indent, m);
    for (const auto& c : t.children) report(*c, depth + 1);
}

// Keep the compiler from optimizing away a benchmarked result.
template <typename V>
void do_not_optimize(V&& v) {
    asm volatile("" : : "r,m"(v) : "memory");
}

// `B` plays the role of Go's `*testing.B`.
struct B {
    // `loop()` returns true until enough time has elapsed
    // for a reasonable estimate; the runner calls the
    // benchmark with increasing `n` like Go does.
    std::size_t n = 0, i = 0;
    bool loop() { return i++ < n; }
};

// Run a benchmark, growing the iteration count until the run
// takes at least ~0.2s, then print Go's benchmark line.
void benchmark(const std::string& name, const std::function<void(B&)>& fn) {
    std::size_t n = 1;
    double elapsed = 0;
    for (;;) {
        B b;
        b.n = n;
        auto start = Clock::now();
        fn(b);
        elapsed = std::chrono::duration<double>(Clock::now() - start).count();
        if (elapsed >= 0.2 || n >= (1ull << 40)) break;
        n *= elapsed < 0.01 ? 10 : 2;
    }
    std::println("{}\t{:>10}\t{:>10.2f} ns/op", name, n, elapsed * 1e9 / double(n));
}

// --- The code under test and its tests ---------------------------

// We'll be testing this simple implementation of an
// integer minimum. Typically, the code we're testing
// would be in its own source file such as `intutils.cpp`,
// and the test file for it would be separate.
int IntMin(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

// A test is just a function taking a `T&`.
void TestIntMinBasic(T& t) {
    int ans = IntMin(2, -2);
    if (ans != -2) {
        t.error(std::format("IntMin(2, -2) = {}; want -2", ans));
    }
}

// Writing tests can be repetitive, so it's idiomatic to
// use a *table-driven style*, where test inputs and
// expected outputs are listed in a table and a single loop
// walks over them and performs the test logic.
void TestIntMinTableDriven(T& t) {
    struct Case { int a, b, want; };
    const std::vector<Case> tests = {
        {0, 1, 0},
        {1, 0, 0},
        {2, -2, -2},
        {0, -1, -1},
        {-1, 0, -1},
    };

    for (const auto& tt : tests) {
        std::string testname = std::format("{},{}", tt.a, tt.b);
        t.run(testname, [&](T& t) {
            int ans = IntMin(tt.a, tt.b);
            if (ans != tt.want) {
                t.error(std::format("got {}, want {}", ans, tt.want));
            }
        });
    }
}

// Benchmarks are named beginning with `Benchmark`.
void BenchmarkIntMin(B& b) {
    while (b.loop()) {
        // The benchmark runner will automatically execute this
        // loop body many times to determine a reasonable
        // estimate of the run-time of a single iteration.
        do_not_optimize(IntMin(1, 2));
    }
}

// --- The test runner (the role of `go test -v`) ------------------

int main() {
    const std::vector<std::pair<std::string, void (*)(T&)>> tests = {
        {"TestIntMinBasic", TestIntMinBasic},
        {"TestIntMinTableDriven", TestIntMinTableDriven},
    };

    auto start = Clock::now();
    bool ok = true;
    for (const auto& [name, fn] : tests) {
        T t(name);
        std::println("=== RUN   {}", name);
        auto s = Clock::now();
        fn(t);
        t.seconds = std::chrono::duration<double>(Clock::now() - s).count();
        report(t);
        ok = ok && !t.failed;
    }

    benchmark("BenchmarkIntMin", BenchmarkIntMin);

    double total = std::chrono::duration<double>(Clock::now() - start).count();
    std::println("{}", ok ? "PASS" : "FAIL");
    std::println("{}\tmain\t{:.3f}s", ok ? "ok  " : "FAIL", total);
    return ok ? 0 : 1;
}
