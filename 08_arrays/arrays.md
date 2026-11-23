Go's fixed-size arrays map to `std::array`. A small `cpp_fmt` helper reproduces Go's `[a b c]` print layout.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- arrays.cpp`

##### Results:

`emp: [0 0 0 0 0]`
`set: [0 0 0 0 100]`
`get: 100`
`len: 5`
`dcl: [1 2 3 4 5]`
`2d:  [[0 1 2] [1 2 3]]`
