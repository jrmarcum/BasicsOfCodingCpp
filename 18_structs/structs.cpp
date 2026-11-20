// C++ structs are typed collections of fields.
// They're useful for grouping data together to form
// records.
#include <format>
#include <memory>
#include <print>
#include <string>

// This `person` struct type has `name` and `age` fields.
struct person {
    std::string name;
    int age = 0;
};

// Go prints a struct as `{Bob 20}`. This formatter
// reproduces that layout for `person`.
template <>
struct std::formatter<person> : std::formatter<std::string> {
    auto format(const person& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{{{} {}}}", p.name, p.age);
    }
};

// `newPerson` constructs a new person struct with the given name.
std::unique_ptr<person> newPerson(const std::string& name) {
    // Returning a heap-allocated object through a smart
    // pointer is safe: it survives the scope of the function.
    auto p = std::make_unique<person>(person{.name = name});
    p->age = 42;
    return p;
}

int main() {
    // This syntax creates a new struct.
    std::println("{}", person{"Bob", 20});

    // You can name the fields when initializing a struct
    // (designated initializers).
    std::println("{}", person{.name = "Alice", .age = 30});

    // Omitted fields will be zero-valued.
    std::println("{}", person{.name = "Fred"});

    // Go prints a pointer to a struct as `&{...}`; here we
    // take the address of a struct and print it the same way.
    person ann{.name = "Ann", .age = 40};
    person* annp = &ann;
    std::println("&{}", *annp);

    // It's idiomatic to encapsulate new struct creation in constructor functions
    std::println("&{}", *newPerson("Jon"));

    // Access struct fields with a dot.
    person s{.name = "Sean", .age = 50};
    std::println("{}", s.name);

    // Use `->` with struct pointers to access their fields.
    person* sp = &s;
    std::println("{}", sp->age);

    // Structs are mutable.
    sp->age = 51;
    std::println("{}", sp->age);
}
