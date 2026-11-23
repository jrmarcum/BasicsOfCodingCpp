`std::map` is an ordered map, so iteration and printing are sorted by key exactly like Go's printed maps. A small `cpp_fmt` helper reproduces Go's `map[k:v]` layout.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- maps.cpp`

##### Results:

`map: map[k1:7 k2:13]`
`v1:  7`
`len: 2`
`map: map[k1:7]`
`prs: false`
`map: map[bar:2 foo:1]`
