Go's `atomic.Uint64` is `std::atomic<std::uint64_t>`; `Add` and `Load` become `fetch_add` and `load`.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- atomic-counters.cpp`

##### Results:

`ops: 50000`
