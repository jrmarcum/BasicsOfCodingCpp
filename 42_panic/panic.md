Go's `panic` maps to throwing an exception that is never caught. The C++ runtime then calls `std::terminate`, which prints the exception message and aborts, giving a non-zero exit status (abnormal termination; under `zig run` the reported status is 9). We print `panic: a problem` to stderr first so the output resembles Go's. There are no goroutine traces.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- panic.cpp`

##### Results:

`panic: a problem`
`libc++abi: terminating due to uncaught exception of type std::runtime_error: a problem`
___
#### Note that unlike Go, where error-indicating return values are idiomatic, C++ commonly uses exceptions, though `std::expected` is available for the return-value style.
