// Parsing numbers from strings is a basic but common task
// in many programs; here's how to do it in C++.
#include <charconv>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <stdexcept>
#include <print>
#include <string>
#include <string_view>
#include <system_error>

// `std::from_chars` (in `<charconv>`) does the number
// parsing. It does not infer the base or accept a `0x`
// prefix, so this helper mimics Go's `ParseInt(s, 0, 64)`:
// it looks for a `0x` / `0b` / `0` prefix to pick the base.
template <typename T>
std::optional<T> parse_int(std::string_view s) {
    int base = 10;
    if (s.starts_with("0x") || s.starts_with("0X")) {
        base = 16;
        s.remove_prefix(2);
    } else if (s.starts_with("0b") || s.starts_with("0B")) {
        base = 2;
        s.remove_prefix(2);
    } else if (s.size() > 1 && s[0] == '0') {
        base = 8;
        s.remove_prefix(1);
    }
    T value{};
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value, base);
    if (ec != std::errc{} || ptr != s.data() + s.size()) return std::nullopt;
    return value;
}

// Parse floating point text with `from_chars`.
std::optional<double> parse_float(std::string_view s) {
    double value{};
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
    if (ec != std::errc{} || ptr != s.data() + s.size()) return std::nullopt;
    return value;
}

int main() {
    // `parse_float` returns an optional: empty on bad input.
    auto f = parse_float("1.234");
    std::println("{}", f.value_or(0.0));

    // The helper infers the base from the string; `int64_t`
    // requires that the result fit in 64 bits.
    auto i = parse_int<int64_t>("123");
    std::println("{}", i.value_or(0));

    // It also recognizes hex-formatted numbers.
    auto d = parse_int<int64_t>("0x1c8");
    std::println("{}", d.value_or(0));

    // An unsigned variant is available by changing the type.
    auto u = parse_int<uint64_t>("789");
    std::println("{}", u.value_or(0));

    // `std::stoi` is a convenience function for basic
    // base-10 `int` parsing.
    int k = std::stoi("135");
    std::println("{}", k);

    // `std::stoi` throws an exception on bad input
    // (`std::from_chars` reports an error code instead).
    try {
        [[maybe_unused]] int n = std::stoi("wat");
    } catch (const std::invalid_argument&) {
        std::println("stoi: parsing \"wat\": invalid syntax");
    }
}
