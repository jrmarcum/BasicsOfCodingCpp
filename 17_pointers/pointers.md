`zeroval` doesn't change the `i` in `main`, but `zeroptr` does because it has a pointer to the memory address of that variable.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- pointers.cpp`

##### Results:

`initial: 1`
`zeroval: 1`
`zeroptr: 0`
`pointer: 0xfbfb7ff9d4`
___
#### Note: The pointer address on the last line changes each time the program is run (and differs from Go's).
