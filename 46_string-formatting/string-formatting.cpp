// C++23 offers excellent support for string formatting with
// `std::format` and `std::println`. Their format specs
// (`{:>6}`, `{:.2f}`, ...) play the role of Go's `Printf`
// "verbs". Here are some examples of common string
// formatting tasks.
#include <cstdint>
#include <format>
#include <print>
#include <string>
#include <string_view>

struct point {
    int x, y;
};

// Go's `%v`, `%+v` and `%#v` print structs in three styles.
// There is no reflection in C++, so we write them by hand.
std::string v(const point& p) { return std::format("{{{} {}}}", p.x, p.y); }
std::string plus_v(const point& p) { return std::format("{{x:{} y:{}}}", p.x, p.y); }
std::string sharp_v(const point& p) { return std::format("main.point{{x:{}, y:{}}}", p.x, p.y); }

// Go's `%q`: double-quote a string, escaping as in source.
std::string quote(std::string_view s) {
    std::string out = "\"";
    for (char c : s) {
        if (c == '"' || c == '\\') out += '\\';
        out += c;
    }
    return out + "\"";
}

// Go's `%x` on a string: two hex digits per byte.
std::string hex(std::string_view s) {
    std::string out;
    for (unsigned char c : s) out += std::format("{:02x}", c);
    return out;
}

int main() {
    // Go offers several printing "verbs" designed to
    // format general values. In C++ a struct has no
    // built-in textual form, so we print our `point`
    // with the helpers above.
    point p{1, 2};
    std::println("{}", v(p));

    // The `%+v` style includes the struct's field names.
    std::println("{}", plus_v(p));

    // The `%#v` style prints a source-like representation.
    std::println("{}", sharp_v(p));

    // To print the type of a value, Go uses `%T`. C++'s
    // `typeid(...).name()` is compiler-specific (mangled), so
    // we print the Go-style name directly.
    std::println("main.point");

    // Formatting booleans is straight-forward.
    std::println("{}", true);

    // There are many options for formatting integers.
    // Use `{}` (or `{:d}`) for standard, base-10 formatting.
    std::println("{:d}", 123);

    // This prints a binary representation.
    std::println("{:b}", 14);

    // This prints the character corresponding to the
    // given integer.
    std::println("{:c}", 33);

    // `{:x}` provides hex encoding.
    std::println("{:x}", 456);

    // There are also several formatting options for
    // floats. For basic decimal formatting use `{:f}`.
    std::println("{:f}", 78.9);

    // `{:e}` and `{:E}` format the float in (slightly
    // different versions of) scientific notation.
    std::println("{:e}", 123400000.0);
    std::println("{:E}", 123400000.0);

    // For basic string printing use `{}`.
    std::println("{}", "\"string\"");

    // To double-quote strings as in source code, use our
    // `quote` helper (C++23 also has `{:?}` for debug
    // strings, whose escapes are similar).
    std::println("{}", quote("\"string\""));

    // As with integers seen earlier, hex-encoding a string
    // renders it in base-16, with two output characters
    // per byte of input.
    std::println("{}", hex("hex this"));

    // To print a representation of a pointer, format it
    // as a `const void*`.
    std::println("{}", static_cast<const void*>(&p));

    // When formatting numbers you will often want to
    // control the width and precision of the resulting
    // figure. To specify the width of an integer, use a
    // number after the `:` in the spec. Numbers are
    // right-justified by default and padded with spaces.
    std::println("|{:6}|{:6}|", 12, 345);

    // You can also specify the width of printed floats,
    // though usually you'll also want to restrict the
    // decimal precision at the same time with the
    // width.precision syntax.
    std::println("|{:6.2f}|{:6.2f}|", 1.2, 3.45);

    // To left-justify, use the `<` alignment.
    std::println("|{:<6.2f}|{:<6.2f}|", 1.2, 3.45);

    // You may also want to control width when formatting
    // strings, especially to ensure that they align in
    // table-like output. Strings are left-justified by
    // default in C++, so for basic right-justified width
    // use `>`.
    std::println("|{:>6}|{:>6}|", "foo", "b");

    // To left-justify use `<`.
    std::println("|{:<6}|{:<6}|", "foo", "b");

    // So far we've seen `println`, which prints the
    // formatted string to standard output. `std::format`
    // formats and returns a string without printing it
    // anywhere.
    std::string s = std::format("a {}", "string");
    std::println("{}", s);

    // You can format+print to other streams, such as
    // standard error, by passing the `FILE*` first.
    std::println(stderr, "an {}", "error");
}
