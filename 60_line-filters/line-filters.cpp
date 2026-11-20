// A _line filter_ is a common type of program that reads
// input on stdin, processes it, and then prints some
// derived result to stdout. `grep` and `sed` are common
// line filters.

// Here's an example line filter in C++ that writes a
// capitalized version of all input text. You can use this
// pattern to write your own C++ line filters.
#include <algorithm>
#include <cctype>
#include <iostream>
#include <print>
#include <string>

// Uppercase a copy of `s` (ASCII), like Go's `strings.ToUpper`.
std::string to_upper(std::string s) {
    std::ranges::transform(s, s.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return s;
}

int main() {
    // `std::getline` on `std::cin` advances to the next
    // line of input each call and returns false at end of
    // input.
    std::string line;
    while (std::getline(std::cin, line)) {
        // Strip a trailing carriage return so Windows
        // (CRLF) input behaves like Unix input.
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // Uppercase the current line.
        std::string ucl = to_upper(line);

        // Write out the uppercased line.
        std::println("{}", ucl);
    }

    // Check for errors during reading. End of file is
    // expected and sets only `eofbit`, not `badbit`.
    if (std::cin.bad()) {
        std::println(stderr, "error: read failure");
        return 1;
    }
}
