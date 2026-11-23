Go's `time` maps to `<chrono>`: `system_clock::time_point` for instants and `duration` for intervals. `cpp_time`, `cpp_duration` and `cpp_float` helpers print values in Go's layout. Times are printed in UTC.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- time.cpp`

##### Results:

`2026-11-20 19:13:13.130045 +0000 UTC`
`2009-11-17 20:34:58.651387237 +0000 UTC`
`2009`
`November`
`17`
`20`
`34`
`58`
`651387237`
`UTC`
`Tuesday`
`true`
`false`
`false`
`149086h38m14.478657763s`
`149086.63735518273`
`8.945198241310963e+06`
`5.367118944786578e+08`
`536711894478657763`
`2026-11-20 19:13:13.130045 +0000 UTC`
`1992-11-14 21:56:44.172729474 +0000 UTC`
___
#### Note: The current time (first line) and all derived duration/add values will vary with each run.
