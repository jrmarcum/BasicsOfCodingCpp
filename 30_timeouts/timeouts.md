Timeouts are expressed with `Channel::recv_for(duration)`, which returns `std::nullopt` when the time runs out (the equivalent of Go's `select` with `time.After`).
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- timeouts.cpp`

##### Results:

`timeout 1`
`result 2`
