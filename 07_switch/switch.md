C++ `switch` works only on integral values, so the weekday uses stacked `case` labels on `tm_wday`, the no-expression switch becomes an `if` chain, and Go's type switch becomes a generic lambda with `if constexpr`. Go's `%T` has no portable C++ equivalent, so the `string` name is spelled out.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- switch.cpp`

##### Results:

`Write 2 as two`
`It's a weekday`
`It's after noon`
`I'm a bool`
`I'm an int`
`Don't know type string`
___
#### Note: The weekday line (`It's a weekday` or `It's the weekend`) and the time-of-day line (`It's before noon` or `It's after noon`) will reflect the actual day and time when the program is run.
