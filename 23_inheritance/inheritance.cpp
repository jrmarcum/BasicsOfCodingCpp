// C++ supports _embedding_ through inheritance (or
// composition) to express a more seamless _composition_ of types.
#include <format>
#include <print>
#include <string>

// The `describer` interface: an abstract base class.
struct describer {
    virtual ~describer() = default;
    virtual std::string describe() const = 0;
};

// Embedding structs with methods may be used to bestow
// interface implementations onto other structs. `base`
// implements `describer`.
struct base : describer {
    int num;
    explicit base(int n) : num(n) {}

    std::string describe() const override {
        return std::format("base with num={}", num);
    }
};

// A `container` _embeds_ a `base` by inheriting from it.
// The base's members become members of the container.
struct container : base {
    std::string str;
    container(base b, std::string s) : base(std::move(b)), str(std::move(s)) {}
};

int main() {
    // When creating structs, we have to initialize the
    // embedded base explicitly; here the base subobject
    // is passed to the constructor.
    container co(base(1), "some name");

    // We can access the base's fields directly on `co`,
    // e.g. `co.num`.
    std::println("co={{num: {}, str: {}}}", co.num, co.str);

    // Alternatively, we can spell out the full path using
    // the base class name.
    std::println("also num: {}", co.base::num);

    // Since `container` inherits from `base`, the methods of
    // `base` also become methods of a `container`. Here
    // we invoke a method that was inherited from `base`
    // directly on `co`.
    std::println("describe: {}", co.describe());

    // Because `container` derives from `base`, which
    // implements `describer`, a `container` is now a
    // `describer`.
    const describer& d = co;
    std::println("describer: {}", d.describe());
}
