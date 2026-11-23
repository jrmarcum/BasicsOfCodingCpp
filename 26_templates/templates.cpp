// C++ has supported _generics_ (templates) since the
// beginning; C++20 added _concepts_ to constrain them.
#include <concepts>
#include <format>
#include <memory>
#include <print>
#include <ranges>
#include <string>
#include <vector>

// Go prints slices as `[a b c]`; this small helper
// reproduces that layout.
template <typename T>
std::string cpp_fmt(const T& v) {
    if constexpr (std::ranges::range<T> && !std::is_convertible_v<T, std::string_view>) {
        std::string out = "[";
        for (bool first = true; const auto& e : v) {
            if (!first) out += ' ';
            out += cpp_fmt(e);
            first = false;
        }
        return out + "]";
    } else {
        return std::format("{}", v);
    }
}

// As an example of a generic function, `SlicesIndex` takes
// a range of any equality-comparable type and an element of that
// type and returns the index of the first occurrence of
// v in s, or -1 if not present. The `std::equality_comparable`
// concept means that we can compare values of this type with the
// `==` and `!=` operators.
template <std::ranges::random_access_range S, std::equality_comparable E>
    requires std::equality_comparable_with<std::ranges::range_value_t<S>, E>
int SlicesIndex(const S& s, const E& v) {
    for (size_t i = 0; i < std::ranges::size(s); i++) {
        if (v == s[i]) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// As an example of a generic type, `List` is a
// singly-linked list with values of any type.
template <typename T>
class List {
    struct element {
        std::unique_ptr<element> next;
        T val;
    };
    std::unique_ptr<element> head;
    element* tail = nullptr;

public:
    // We can define methods on generic types just like we
    // do on regular types. Inside the class the type
    // parameter `T` is in scope.
    void Push(const T& v) {
        auto e = std::make_unique<element>(nullptr, v);
        element* raw = e.get();
        if (tail == nullptr) {
            head = std::move(e);
        } else {
            tail->next = std::move(e);
        }
        tail = raw;
    }

    // AllElements returns all the List elements as a vector.
    std::vector<T> AllElements() const {
        std::vector<T> elems;
        for (const element* e = head.get(); e != nullptr; e = e->next.get()) {
            elems.push_back(e->val);
        }
        return elems;
    }
};

int main() {
    std::vector<std::string> s = {"foo", "bar", "zoo"};

    // When invoking generic functions, we can often rely
    // on _type inference_. Note that we don't have to
    // specify the types for `S` and `E` when
    // calling `SlicesIndex` - the compiler infers them
    // automatically.
    std::println("index of zoo: {}", SlicesIndex(s, std::string("zoo")));

    // ... though we could also specify them explicitly.
    [[maybe_unused]] int unused = SlicesIndex<std::vector<std::string>, std::string>(s, "zoo");

    List<int> lst;
    lst.Push(10);
    lst.Push(13);
    lst.Push(23);
    std::println("list: {}", cpp_fmt(lst.AllElements()));
}
