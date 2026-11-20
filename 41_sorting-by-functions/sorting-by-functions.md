C++ has no need for Go's `sort.Interface` wrapper type: `std::ranges::sort` takes a comparison lambda (or a projection) directly.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- sorting-by-functions.cpp`

##### Results:

`[kiwi peach banana]`
`[kiwi peach banana]`
___
#### By passing a comparison function (or a projection) to `std::ranges::sort`, we can sort vectors by arbitrary criteria. The second line uses the projection form.
