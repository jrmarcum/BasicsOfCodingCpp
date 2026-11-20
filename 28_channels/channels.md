C++ has no built-in channel type, so the lesson defines a small `Channel<T>` class (mutex + condition variable + deque) with `send`, `recv` and `close`; `recv` returns a `std::optional`.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- channels.cpp`

##### Results:

`ping`
