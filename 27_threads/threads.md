A goroutine maps to a `std::jthread`. Thread output order is not deterministic: the `thread : N` and `going` lines may interleave differently from run to run (as in Go).
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- threads.cpp`

##### Results:

`direct : 0`
`direct : 1`
`direct : 2`
`thread : 0`
`going`
`thread : 1`
`thread : 2`
`done`
