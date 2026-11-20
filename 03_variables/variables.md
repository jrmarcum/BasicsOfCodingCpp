Go's `var` and `:=` map to explicit types and `auto`. C++ locals have no implicit zero value, so `{}` value-initialization is used where Go relies on zero values.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- variables.cpp`

##### Results:

`initial`
`1 2`
`true`
`0`
`apple`
