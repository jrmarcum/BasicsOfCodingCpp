Go's type parameters map to templates, `comparable` to the `std::equality_comparable` concept, and the generic `List[T]` to a `List<T>` class template. `go_fmt` prints the vector Go-style.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- generics.cpp`

##### Results:

`index of zoo: 2`
`list: [10 13 23]`
