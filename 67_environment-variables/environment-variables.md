C++ reads a variable with `std::getenv` (null when absent, wrapped here to return an empty string like Go's `os.Getenv`), sets one with `_putenv_s` on Windows or `setenv` on POSIX, and lists the environment by walking the C runtime's `environ` array.
___
#### Running the program shows that we pick up the value for `FOO` that we set in the program, but that `BAR` is empty.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- environment-variables.cpp`

##### Results:

`FOO: 1`
`BAR: `
`...`
___
#### The total list of keys in the environment will depend on your particular machine.
___
#### If we set `BAR` in the environment first, the running program picks that value up.
___
##### Run Command:

`$ $Env:BAR=2; zig run -lc++ -cflags -std=c++23 -- environment-variables.cpp` (for Windows PowerShell)

`$ BAR=2 zig run -lc++ -cflags -std=c++23 -- environment-variables.cpp` (for Unix-like environments)

##### Results:

`FOO: 1`
`BAR: 2`
`...`
