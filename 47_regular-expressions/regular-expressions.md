C++'s `<regex>` provides `regex_search`, `regex_replace` and iterators over matches. Go's `Find...Index` and `All` variants are small helper functions built on them.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- regular-expressions.cpp`

##### Results:

`true`
`true`
`peach`
`[0 5]`
`[peach ea]`
`[0 5 1 3]`
`[peach punch pinch]`
`[[0 5 1 3] [6 11 7 9] [12 17 13 15]]`
`[peach punch]`
`true`
`p([a-z]+)ch`
`a <fruit>`
`a PEACH`
