C++ structs map directly to Go structs. Go's `{Bob 20}` / `&{Ann 40}` printing is reproduced with a `std::formatter<person>` specialization; `newPerson` returns a `std::unique_ptr`.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- structs.cpp`

##### Results:

`{Bob 20}`
`{Alice 30}`
`{Fred 0}`
`&{Ann 40}`
`&{Jon 42}`
`Sean`
`50`
`51`
