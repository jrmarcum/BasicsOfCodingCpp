Go's `(value, error)` return pair maps to `std::expected<T, E>`: `std::unexpected` carries the error, and the custom `argError` is used directly as the error type, so no type assertion is needed.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- errors.cpp`

##### Results:

`f1 worked: 10`
`f1 failed: can't work with 42`
`f2 worked: 10`
`f2 failed: 42 - can't work with it`
`42`
`can't work with it`
