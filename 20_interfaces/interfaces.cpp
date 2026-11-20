// Interfaces are named collections of method
// signatures. In C++ they are abstract base classes.
#include <format>
#include <numbers>
#include <print>
#include <string>

// Here's a basic interface for geometric shapes.
struct geometry {
    virtual ~geometry() = default;
    virtual double area() const = 0;
    virtual double perim() const = 0;
    // Go's `fmt.Println(g)` prints the struct's fields,
    // so each shape supplies its own description.
    virtual std::string str() const = 0;
};

// For our example we'll implement this interface on
// `rect` and `circle` types.
struct rect : geometry {
    double width, height;
    rect(double w, double h) : width(w), height(h) {}

    // To implement an interface in C++, we derive from the
    // base class and override all its pure virtual methods.
    // Here we implement `geometry` on `rect`s.
    double area() const override { return width * height; }
    double perim() const override { return 2 * width + 2 * height; }
    std::string str() const override { return std::format("{{{} {}}}", width, height); }
};

// The implementation for `circle`s.
struct circle : geometry {
    double radius;
    explicit circle(double r) : radius(r) {}

    double area() const override { return std::numbers::pi * radius * radius; }
    double perim() const override { return 2 * std::numbers::pi * radius; }
    std::string str() const override { return std::format("{{{}}}", radius); }
};

// If a variable has an interface type (here a reference to
// the base class), then we can call methods that are in
// the named interface. Here's a generic `measure` function
// taking advantage of this to work on any `geometry`.
void measure(const geometry& g) {
    std::println("{}", g.str());
    std::println("{}", g.area());
    std::println("{}", g.perim());
}

int main() {
    rect r(3, 4);
    circle c(5);

    // The `circle` and `rect` types both
    // implement the `geometry` interface so we can use
    // instances of these types as arguments to `measure`.
    measure(r);
    measure(c);
}
