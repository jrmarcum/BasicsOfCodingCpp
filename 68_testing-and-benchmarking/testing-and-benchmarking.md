The C++ standard library has no test framework, so this lesson contains a miniature one: a `T` type with `error` and `run` (subtests), a table-driven test, a `<chrono>`-based benchmark loop (`B::loop`), and a `main` that plays the role of `go test -v`. The output mimics Go's. Timings vary from run to run, and `zig run` builds without optimization, so the benchmark figure is much slower than an optimized build would give.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- testing-and-benchmarking.cpp`

##### Results:

`=== RUN   TestIntMinBasic`
`--- PASS: TestIntMinBasic (0.00s)`
`=== RUN   TestIntMinTableDriven`
`=== RUN   TestIntMinTableDriven/0,1`
`=== RUN   TestIntMinTableDriven/1,0`
`=== RUN   TestIntMinTableDriven/2,-2`
`=== RUN   TestIntMinTableDriven/0,-1`
`=== RUN   TestIntMinTableDriven/-1,0`
`--- PASS: TestIntMinTableDriven (0.00s)`
`    --- PASS: TestIntMinTableDriven/0,1 (0.00s)`
`    --- PASS: TestIntMinTableDriven/1,0 (0.00s)`
`    --- PASS: TestIntMinTableDriven/2,-2 (0.00s)`
`    --- PASS: TestIntMinTableDriven/0,-1 (0.00s)`
`    --- PASS: TestIntMinTableDriven/-1,0 (0.00s)`
`BenchmarkIntMin	  40000000	      8.20 ns/op`
`PASS`
`ok  	main	0.583s`
___
#### For a more realistic benchmark number, build with optimization.
___
##### Run Command:

`$ zig c++ -std=c++23 -O2 testing-and-benchmarking.cpp -o testing-and-benchmarking`

`$ ./testing-and-benchmarking`

##### Results:

`...`
`BenchmarkIntMin	1600000000	      0.19 ns/op`
`PASS`
`ok  	main	0.627s`
