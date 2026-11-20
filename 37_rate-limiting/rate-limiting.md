Go's `time.Tick` is a `Ticker` class (a `std::jthread` plus a channel); the bursty limiter is a `Channel` buffered to 3. Timestamps are UTC and differ on each run; the pacing (about 200ms apart, with the first 3 bursty requests immediate) is what matters.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- rate-limiting.cpp`

##### Results:

`request 1 2026-11-20 19:12:44.512963`
`request 2 2026-11-20 19:12:44.718902`
`request 3 2026-11-20 19:12:44.910207`
`request 4 2026-11-20 19:12:45.107086`
`request 5 2026-11-20 19:12:45.314496`
`request 1 2026-11-20 19:12:45.314659`
`request 2 2026-11-20 19:12:45.314746`
`request 3 2026-11-20 19:12:45.314778`
`request 4 2026-11-20 19:12:45.512083`
`request 5 2026-11-20 19:12:45.713146`
