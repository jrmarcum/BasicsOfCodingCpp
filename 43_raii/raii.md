Go's `defer` maps to RAII: a small `Defer` scope-guard class runs a callable from its destructor when `main` exits. This lesson writes `./tmp/defer.txt`, so create the `tmp` folder first.
___
#### To run the program, make sure you are in the program's folder, create the `tmp` folder (`mkdir tmp`), then use the Run Command below.
___
##### Run Command:

`$ mkdir tmp`

`$ zig run -lc++ -cflags -std=c++23 -- raii.cpp`

##### Results:

`creating`
`writing`
`closing`
