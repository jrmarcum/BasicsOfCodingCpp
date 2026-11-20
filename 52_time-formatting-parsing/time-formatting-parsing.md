C++ formatting uses strftime-style specifiers (`%H:%M:%S`) rather than Go's example-based layouts. libc++ has no `std::chrono::parse` or time zone database here, so the local offset comes from the C library and parsing is done with `sscanf`. The final error message is written by hand to match Go's wording (Go's own text is truncated in the upstream lesson).
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- time-formatting-parsing.cpp`

##### Results:

`2026-11-20T14:13:28-05:00`
`2012-11-01 22:08:41 +0000 +0000`
`2:13PM`
`Fri Nov 20 14:13:28 2026`
`2026-11-20T14:13:28.153549-05:00`
`0000-01-01 20:41:00 +0000 UTC`
`2026-11-20T14:13:28-00:00`
`parsing time "8:41PM" as "Mon Jan _2 15:04:05 2006": cannot parse "8:41PM" as "Mon"`
___
#### Note: Dates and times will vary with each run (local time zone offset included).
