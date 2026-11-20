A goroutine maps to a `std::jthread`. Thread output order is not deterministic: the `goroutine : N` and `going` lines may interleave differently from run to run (as in Go).
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- goroutines.cpp`

##### Results:

`direct : 0`
`direct : 1`
`direct : 2`
`goroutine : 0`
`going`
`goroutine : 1`
`goroutine : 2`
`done`
