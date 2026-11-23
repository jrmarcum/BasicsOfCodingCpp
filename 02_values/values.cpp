// C++ has various built-in types. Here are the basic types
// available (the fixed-width integers live in `<cstdint>`).
//
// bool                          - Boolean
// int8_t, int16_t, int32_t,     - signed integers
// int64_t, int
// uint8_t, uint16_t, uint32_t,  - unsigned integers
// uint64_t, unsigned, size_t
// float, double                 - floating point
// std::complex<float>,          - complex numbers
// std::complex<double>
// std::string                   - strings
#include <print>
#include <string>

int main() {
    // Strings, which can be added together with `+`.
    std::println("{}", std::string("cpp") + "lang");

    // Integers and floats.
    std::println("1+1 = {}", 1 + 1);
    std::println("7.0/3.0 = {}", 7.0 / 3.0);

    // Booleans, with boolean operators as you'd expect.
    // `{}` prints a `bool` as `true` or `false`.
    std::println("{}", true && false);
    std::println("{}", true || false);
    std::println("{}", !true);
}
