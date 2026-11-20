`main` receives `argc`/`argv`; the program copies them into a `std::vector<std::string>` (index 0 is the program path) and a small `go_fmt` helper prints them in Go's `[a b c]` layout.
___
#### To experiment with command-line arguments with `zig run`, put the program's arguments after a second `--`. Everything after it is passed to the program.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- command-line-arguments.cpp -- a b c d`

##### Results:

`[C:\Users\you\.cache\zig-local\wazmrt\o\e59eb54dd93589ac846d45e6c285158b\command-line-arguments.exe a b c d]`
`[a b c d]`
`c`

The first element is the path of the temporary binary zig built, so it will differ on your machine.
___
#### It is also possible to build a binary with `zig c++` first.
___
##### Run Command:

`$ zig c++ -std=c++23 command-line-arguments.cpp -o command-line-arguments`

`$ ./command-line-arguments a b c d`

##### Results:

`[./command-line-arguments a b c d]`
`[a b c d]`
`c`

On Windows the first element is the full path to `command-line-arguments.exe`.
