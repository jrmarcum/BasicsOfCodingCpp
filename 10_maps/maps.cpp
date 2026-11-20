// Maps are C++'s associative data type (sometimes called
// _hashes_ or _dicts_ in other languages). `std::map` keeps
// its keys sorted, like Go's map printing.
#include <map>
#include <print>
#include <string>

// Go prints maps as `map[k1:v1 k2:v2]`; C++23's built-in
// formatting gives `{"k1": 7, "k2": 13}`, so this helper
// reproduces Go's layout.
template <typename K, typename V>
std::string go_fmt(const std::map<K, V>& m) {
    std::string out = "map[";
    for (bool first = true; const auto& [k, v] : m) {
        if (!first) out += ' ';
        out += std::format("{}:{}", k, v);
        first = false;
    }
    return out + "]";
}

int main() {
    // To create an empty map, declare a `std::map`:
    // `std::map<key-type, val-type>`.
    std::map<std::string, int> m;

    // Set key/value pairs using typical `name[key] = val`
    // syntax.
    m["k1"] = 7;
    m["k2"] = 13;

    // Printing a map shows all of its key/value pairs
    // (through our `go_fmt` helper).
    std::println("map: {}", go_fmt(m));

    // Get a value for a key with `name[key]`.
    int v1 = m["k1"];
    std::println("v1:  {}", v1);

    // `size()` returns the number of key/value pairs.
    std::println("len: {}", m.size());

    // `erase` removes key/value pairs from a map.
    m.erase("k2");
    std::println("map: {}", go_fmt(m));

    // To tell whether a key is present (and so disambiguate
    // between missing keys and keys with zero values like
    // `0` or `""`), use `contains` (or `find`). Note that
    // `m["k2"]` would insert the key, so we avoid it here.
    bool prs = m.contains("k2");
    std::println("prs: {}", prs);

    // You can also declare and initialize a new map in
    // the same line with this syntax.
    std::map<std::string, int> n = {{"foo", 1}, {"bar", 2}};
    std::println("map: {}", go_fmt(n));
}
