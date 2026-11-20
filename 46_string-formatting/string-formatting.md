`std::format` / `std::println` specs mirror Go's verbs: `{:b}`, `{:c}`, `{:x}`, `{:f}`, `{:e}`, `{:E}`, `{:6.2f}`, `{:<6}`, `{:>6}`. Go's struct verbs (`%v`, `%+v`, `%#v`), `%T`, `%q` and hex-of-string have no built-in equivalent, so small helper functions produce the same text. One difference: strings are left-aligned by default in C++ (right-aligned in Go), so right-justification needs `>`. The `an error` line goes to stderr.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- string-formatting.cpp`

##### Results:

`{1 2}`
`{x:1 y:2}`
`main.point{x:1, y:2}`
`main.point`
`true`
`123`
`1110`
`!`
`1c8`
`78.900000`
`1.234000e+08`
`1.234000E+08`
`"string"`
`"\"string\""`
`6865782074686973`
`0xd4d6dff7b0`
`|    12|   345|`
`|  1.20|  3.45|`
`|1.20  |3.45  |`
`|   foo|     b|`
`|foo   |b     |`
`a string`
`an error`
___
#### Note: The pointer address (`{}` of a `const void*` line) changes each time the program is run.
