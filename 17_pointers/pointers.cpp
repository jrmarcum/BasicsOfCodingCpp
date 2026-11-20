// C++ supports pointers (and references),
// allowing you to pass references to values and records
// within your program.
#include <print>

// We'll show how pointers work in contrast to values with
// 2 functions: `zeroval` and `zeroptr`. `zeroval` has an
// `int` parameter, so arguments will be passed to it by
// value. `zeroval` will get a copy of `ival` distinct
// from the one in the calling function.
void zeroval(int ival) {
    ival = 0;
}

// `zeroptr` in contrast has an `int*` parameter, meaning
// that it takes an `int` pointer. The `*iptr` code in the
// function body then _dereferences_ the pointer from its
// memory address to the current value at that address.
// Assigning a value to a dereferenced pointer changes the
// value at the referenced address.
void zeroptr(int* iptr) {
    *iptr = 0;
}

int main() {
    int i = 1;
    std::println("initial: {}", i);

    zeroval(i);
    std::println("zeroval: {}", i);

    // The `&i` syntax gives the memory address of `i`,
    // i.e. a pointer to `i`.
    zeroptr(&i);
    std::println("zeroptr: {}", i);

    // Pointers can be printed too (cast to `const void*`
    // so they format as an address).
    std::println("pointer: {}", static_cast<const void*>(&i));
}
