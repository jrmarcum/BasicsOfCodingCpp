C++ closures are lambdas. Capturing `i` by value with `mutable` gives each returned function its own private counter, returned through a `std::function`.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- closures.cpp`

##### Results:

`1`
`2`
`3`
`1`
