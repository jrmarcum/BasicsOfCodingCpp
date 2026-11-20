// C++ supports methods (member functions) defined on struct types.
#include <print>

struct rect {
    int width, height;

    // This `area` method is non-const, so it could mutate
    // the receiving struct (like a Go pointer receiver).
    int area() {
        return width * height;
    }

    // A `const` method promises not to modify the struct,
    // like a Go value receiver.
    int perim() const {
        return 2 * width + 2 * height;
    }
};

int main() {
    rect r{.width = 10, .height = 5};

    // Here we call the 2 methods defined for our struct.
    std::println("area:  {}", r.area());
    std::println("perim: {}", r.perim());

    // C++ uses `->` to call methods through a pointer. You
    // may want a non-const method to allow mutating the
    // receiving struct, or pass by reference to avoid copying.
    rect* rp = &r;
    std::println("area:  {}", rp->area());
    std::println("perim: {}", rp->perim());
}
