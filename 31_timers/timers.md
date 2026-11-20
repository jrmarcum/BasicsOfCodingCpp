A Go timer is built here as a small `Timer` class: a `std::jthread` that waits with a `std::stop_token` and then sends on a channel `C`. `stop()` cancels it before it fires.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- timers.cpp`

##### Results:

`Timer 1 fired`
`Timer 2 stopped`
