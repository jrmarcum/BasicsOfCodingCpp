Go's pointer receiver maps to a non-const member function and its value receiver to a `const` member function; `->` replaces Go's automatic pointer dereference.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- methods.cpp`

##### Results:

`area:  50`
`perim: 30`
`area:  50`
`perim: 30`
