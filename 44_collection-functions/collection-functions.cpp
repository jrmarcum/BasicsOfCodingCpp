// We often need our programs to perform operations on
// collections of data, like selecting all items that
// satisfy a given predicate or mapping all items to a new
// collection with a custom function.

// C++ already ships generic algorithms (`std::ranges::find`,
// `any_of`, `all_of`, `std::views::filter`, `transform`), but
// to mirror the Go lesson we write our own small collection
// functions for vectors of strings. Note that in some cases
// it may be clearest to just inline the collection-manipulating
// code directly, instead of creating and calling a helper
// function.
#include <algorithm>
#include <cctype>
#include <functional>
#include <print>
#include <string>
#include <vector>

using Strings = std::vector<std::string>;

// Go prints slices as `[a b c]`; this helper reproduces that.
std::string cpp_fmt(const Strings& v) {
    std::string out = "[";
    for (bool first = true; const auto& e : v) {
        if (!first) out += ' ';
        out += e;
        first = false;
    }
    return out + "]";
}

// Index returns the first index of the target string `t`, or
// -1 if no match is found.
int Index(const Strings& vs, const std::string& t) {
    for (int i = 0; i < static_cast<int>(vs.size()); i++) {
        if (vs[i] == t) {
            return i;
        }
    }
    return -1;
}

// Include returns `true` if the target string t is in the
// vector.
bool Include(const Strings& vs, const std::string& t) {
    return Index(vs, t) >= 0;
}

// Any returns `true` if one of the strings in the vector
// satisfies the predicate `f`.
bool Any(const Strings& vs, const std::function<bool(const std::string&)>& f) {
    for (const auto& v : vs) {
        if (f(v)) {
            return true;
        }
    }
    return false;
}

// All returns `true` if all of the strings in the vector
// satisfy the predicate `f`.
bool All(const Strings& vs, const std::function<bool(const std::string&)>& f) {
    for (const auto& v : vs) {
        if (!f(v)) {
            return false;
        }
    }
    return true;
}

// Filter returns a new vector containing all strings in the
// vector that satisfy the predicate `f`.
Strings Filter(const Strings& vs, const std::function<bool(const std::string&)>& f) {
    Strings vsf;
    for (const auto& v : vs) {
        if (f(v)) {
            vsf.push_back(v);
        }
    }
    return vsf;
}

// Map returns a new vector containing the results of applying
// the function `f` to each string in the original vector.
Strings Map(const Strings& vs, const std::function<std::string(const std::string&)>& f) {
    Strings vsm(vs.size());
    for (std::size_t i = 0; i < vs.size(); i++) {
        vsm[i] = f(vs[i]);
    }
    return vsm;
}

// A named function of the correct type.
std::string ToUpper(const std::string& s) {
    std::string r = s;
    std::ranges::transform(r, r.begin(), [](unsigned char c) { return std::toupper(c); });
    return r;
}

int main() {
    // Here we try out our various collection functions.
    Strings strs = {"peach", "apple", "pear", "plum"};

    std::println("{}", Index(strs, "pear"));

    std::println("{}", Include(strs, "grape"));

    std::println("{}", Any(strs, [](const std::string& v) {
        return v.starts_with("p");
    }));

    std::println("{}", All(strs, [](const std::string& v) {
        return v.starts_with("p");
    }));

    std::println("{}", cpp_fmt(Filter(strs, [](const std::string& v) {
        return v.contains("e");
    })));

    // The above examples all used anonymous functions (lambdas),
    // but you can also use named functions of the correct
    // type.
    std::println("{}", cpp_fmt(Map(strs, ToUpper)));
}
