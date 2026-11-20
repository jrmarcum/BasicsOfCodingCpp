Go's `defer` maps to an RAII guard whose destructor prints `!`. `std::exit` ends the process without unwinding `main`'s stack, so the destructor never runs, just like `defer` with `os.Exit`.
___
#### If you run `exit.cpp` using `zig run`, the program exits with status 3 and the `!` from our guard never gets printed. Unlike `go run`, `zig run` does not print an `exit status` line, so we build a binary and check the status ourselves.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- exit.cpp`

##### Results:

(no output; the `!` is never printed)
___
#### Build the program into a binary and run it, then check the exit status.
___
##### Run Command:

`$ zig c++ -std=c++23 exit.cpp -o exit`

`$ ./exit`

`$ echo $?` (Unix shells; in PowerShell use `echo $LASTEXITCODE`, in cmd `echo %ERRORLEVEL%`)

##### Results:

`3`
