C++ parses numbers with `std::from_chars` (no allocation, no exceptions, reports an error code) or `std::stoi`/`std::stod` (throw on bad input). `from_chars` does not infer the base, so a small `parse_int` helper picks it from a `0x`/`0b`/`0` prefix like Go's `ParseInt(s, 0, 64)`.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- number-parsing.cpp`

##### Results:

`1.234`
`123`
`456`
`789`
`135`
`stoi: parsing "wat": invalid syntax`
