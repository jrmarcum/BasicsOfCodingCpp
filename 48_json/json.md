C++ has no JSON in the standard library, so this lesson defines a small `Json` value type (a `std::variant`), a `dump` encoder, and a recursive-descent `parse` decoder. Struct tags and reflection don't exist in C++, so the struct conversions are written by hand.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- json.cpp`

##### Results:

`true`
`1`
`2.34`
`"vector"`
`["apple","peach","pear"]`
`{"apple":5,"lettuce":7}`
`{"Page":1,"Fruits":["apple","peach","pear"]}`
`{"page":1,"fruits":["apple","peach","pear"]}`
`map[num:6.13 strs:[a b]]`
`6.13`
`a`
`{1 [apple peach]}`
`apple`
`{"apple":5,"lettuce":7}`
