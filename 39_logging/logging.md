C++ has no standard logging library, so this lesson builds a small `Logger` class (prefix, flags, output target, `std::source_location` for file/line) and a JSON `slog_info` function from `std::format` and `<chrono>`. The date, time, line numbers and time zone offset depend on when and where the program runs.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- logging.cpp`

##### Results:

`2026/11/20 14:12:54 standard logger`
`2026/11/20 14:12:54.222892 with micro`
`2026/11/20 14:12:54 logging.cpp:131: with file/line`
`my:2026/11/20 14:12:54 from mylog`
`ohmy:2026/11/20 14:12:54 from mylog`
`from buflog:buf:2026/11/20 14:12:54 hello`
`{"time":"2026-11-20T14:12:54.222979000-05:00","level":"INFO","msg":"hi there"}`
`{"time":"2026-11-20T14:12:54.223000000-05:00","level":"INFO","msg":"hello again","key":"val","age":25}`
