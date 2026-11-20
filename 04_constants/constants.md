Go's untyped constants map to `constexpr`. C++ constants are typed, so `3e20 / n` is a `double`; `{:g}` is used to print it as `6e+11` like Go (the default `{}` would print `600000000000`).
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- constants.cpp`

##### Results:

`constant`
`6e+11`
`600000000000`
`-0.28470407323754404`
