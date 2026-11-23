C++ has no built-in slice type. `std::vector` provides the growable, owning array and `std::span` provides the non-owning `slice[low:high]` view. A small `cpp_fmt` helper reproduces Go's `[a b c]` print layout.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- vectors.cpp`

##### Results:

`emp: [  ]`
`set: [a b c]`
`get: c`
`len: 3`
`apd: [a b c d e f]`
`cpy: [a b c d e f]`
`sl1: [c d e]`
`sl2: [a b c d e]`
`sl3: [c d e f]`
`dcl: [g h i]`
`2d:  [[0] [1 2] [2 3 4]]`
