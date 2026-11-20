// Slices are a key data type in Go. The C++ equivalent is
// `std::vector`, a growable array, together with
// `std::span`, a non-owning view over a contiguous range.
#include <print>
#include <span>
#include <string>
#include <vector>

// Go prints slices as `[a b c]`. C++23 can format ranges
// directly (as `["a", "b", "c"]`), so this small helper
// reproduces Go's layout, recursing into nested ranges.
template <typename T>
std::string go_fmt(const T& v) {
    if constexpr (std::ranges::range<T> && !std::is_convertible_v<T, std::string_view>) {
        std::string out = "[";
        for (bool first = true; const auto& e : v) {
            if (!first) out += ' ';
            out += go_fmt(e);
            first = false;
        }
        return out + "]";
    } else {
        return std::format("{}", v);
    }
}

int main() {
    // To create a vector with non-zero length, pass the
    // length to the constructor. Here we make a vector of
    // `std::string`s of length `3` (initially empty strings).
    std::vector<std::string> s(3);
    std::println("emp: {}", go_fmt(s));

    // We can set and get just like with arrays.
    s[0] = "a";
    s[1] = "b";
    s[2] = "c";
    std::println("set: {}", go_fmt(s));
    std::println("get: {}", s[2]);

    // `size()` returns the length of the vector.
    std::println("len: {}", s.size());

    // `push_back` appends one value; `insert` at `end()`
    // appends several. Unlike Go's `append`, the vector is
    // modified in place.
    s.push_back("d");
    s.insert(s.end(), {"e", "f"});
    std::println("apd: {}", go_fmt(s));

    // Copying a vector is a plain assignment: the copy owns
    // its own elements.
    std::vector<std::string> c = s;
    std::println("cpy: {}", go_fmt(c));

    // `std::span` gives a view of part of a vector, like
    // Go's `slice[low:high]`. `subspan(offset, count)`
    // gets `s[2]`, `s[3]`, and `s[4]`.
    std::span<std::string> all(s);
    auto l = all.subspan(2, 3);
    std::println("sl1: {}", go_fmt(l));

    // This slices up to (but excluding) `s[5]`.
    l = all.first(5);
    std::println("sl2: {}", go_fmt(l));

    // And this slices up from (and including) `s[2]`.
    l = all.subspan(2);
    std::println("sl3: {}", go_fmt(l));

    // We can declare and initialize a vector in a single
    // line as well.
    std::vector<std::string> t = {"g", "h", "i"};
    std::println("dcl: {}", go_fmt(t));

    // Vectors can be composed into multi-dimensional data
    // structures. The length of the inner vectors can vary.
    std::vector<std::vector<int>> twoD(3);
    for (int i = 0; i < 3; i++) {
        int innerLen = i + 1;
        twoD[i].resize(innerLen);
        for (int j = 0; j < innerLen; j++) {
            twoD[i][j] = i + j;
        }
    }
    std::println("2d:  {}", go_fmt(twoD));
}
