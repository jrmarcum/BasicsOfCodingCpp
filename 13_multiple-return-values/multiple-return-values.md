C++ returns multiple values as a `std::pair` (or `std::tuple`) and unpacks them with structured bindings. There is no blank identifier, so unwanted values are simply not used.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- multiple-return-values.cpp`

##### Results:

`3`
`7`
`7`
