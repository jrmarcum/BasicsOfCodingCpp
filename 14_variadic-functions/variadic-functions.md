C++ offers variadic templates (parameter packs) for individual arguments and `std::span` for passing an existing vector, standing in for Go's `nums...`. A small `cpp_fmt` helper reproduces Go's `[a b c]` layout.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- variadic-functions.cpp`

##### Results:

`[1 2] 3`
`[1 2 3] 6`
`[1 2 3 4] 10`
