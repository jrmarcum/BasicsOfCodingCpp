Go's `sync.WaitGroup` is replaced by a vector of `std::jthread` that we `join` (a `std::latch` would also work). Worker start and finish order is not deterministic.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- thread-joining.cpp`

##### Results:

`Worker 3 starting`
`Worker 2 starting`
`Worker 1 starting`
`Worker 5 starting`
`Worker 4 starting`
`Worker 1 done`
`Worker 4 done`
`Worker 5 done`
`Worker 2 done`
`Worker 3 done`
