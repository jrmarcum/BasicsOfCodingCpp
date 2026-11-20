C++ has no URL type in its standard library, so the program uses a small hand-written parser over `std::string_view`. The Go `map[k:[v]]` layout is reproduced by a `go_fmt` helper.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
Running our URL parsing program shows all the different pieces that we extracted.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- url-parsing.cpp`

##### Results:

`postgres`
`user:pass`
`user`
`pass`
`host.com:5432`
`host.com`
`5432`
`/path`
`f`
`k=v`
`map[k:[v]]`
`v`
