A `Ticker` class (a `std::jthread` plus a channel `C`) delivers the current time at a fixed interval. Timestamps are UTC and vary on every run; Go prints local time with a zone.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- tickers.cpp`

##### Results:

`Tick at 2026-11-20 19:12:36.514`
`Tick at 2026-11-20 19:12:37.012`
`Tick at 2026-11-20 19:12:37.507`
`Ticker stopped`
