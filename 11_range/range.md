C++'s range-for gives values only, so index loops are written classically. Iterating a `std::string` visits bytes, not Unicode code points; for ASCII text the output equals Go's.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- range.cpp`

##### Results:

`sum: 9`
`index: 1`
`a -> apple`
`b -> banana`
`key: a`
`key: b`
`0 99`
`1 112`
`2 112`
___
#### Note: Go's map iteration order is non-deterministic; `std::map` always iterates in sorted key order, so the output here is deterministic.
