There is no `select` statement in C++. The lesson uses the `Channel<T>` class and polls each channel with `try_recv`, sleeping briefly between passes.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- select.cpp`

##### Results:

`received one`
`received two`
