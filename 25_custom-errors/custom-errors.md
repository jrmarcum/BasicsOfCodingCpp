Go's `error` interface is an abstract `error` base class, errors travel in a `std::expected<int, std::shared_ptr<error>>`, and `errors.As` is a small `errors_as<T>` template built on `std::dynamic_pointer_cast`.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- custom-errors.cpp`

##### Results:

`42`
`can't work with it`
