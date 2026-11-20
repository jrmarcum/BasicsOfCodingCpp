C++ has no `flag` package, so this lesson includes a small hand-written `FlagSet` that mirrors Go's: `-name=value`, `-name value`, `--name`, bare boolean flags, parsing that stops at the first non-flag argument, and Go-style help and error text.
___
#### To experiment with the command-line flags program it's best to first compile it and then run the resulting binary directly.
___
##### Run Command:

`$ zig c++ -std=c++23 command-line-flags.cpp -o command-line-flags`

___
#### Try out the built program by first giving it values for all flags.
___
##### Run Command:

`$ ./command-line-flags -word=opt -numb=7 -fork -svar=flag`

##### Results:

`word: opt`
`numb: 7`
`fork: true`
`svar: flag`
`tail: []`
___
#### Note that if you omit flags they automatically take their default values.
___
##### Run Command:

`$ ./command-line-flags -word=opt`

##### Results:

`word: opt`
`numb: 42`
`fork: false`
`svar: bar`
`tail: []`
___
#### Trailing positional arguments can be provided after any flags.
___
##### Run Command:

`$ ./command-line-flags -word=opt a1 a2 a3`

##### Results:

`word: opt`
`numb: 42`
`fork: false`
`svar: bar`
`tail: [a1 a2 a3]`
___
#### Note that, like Go's `flag` package, this parser requires all flags to appear before positional arguments (otherwise the flags will be interpreted as positional arguments).
___
##### Run Command:

`$ ./command-line-flags -word=opt a1 a2 a3 -numb=7`

##### Results:

`word: opt`
`numb: 42`
`fork: false`
`svar: bar`
`tail: [a1 a2 a3 -numb=7]`
___
#### Use `-h` or `--help` flags to get automatically generated help text for the command-line program.
___
##### Run Command:

`$ ./command-line-flags -h`

##### Results:

`Usage of ./command-line-flags:`
`  -fork`
`        a bool`
`  -numb int`
`        an int (default 42)`
`  -svar string`
`        a string var (default "bar")`
`  -word string`
`        a string (default "foo")`

(The help text goes to stderr. On Windows the program path in the first line is the full path of the `.exe`.)
___
#### If you provide a flag that wasn't declared, the program will print an error message and show the help text again (and exit with status 2).
___
##### Run Command:

`$ ./command-line-flags -wat`

##### Results:

`flag provided but not defined: -wat`
`Usage of ./command-line-flags:`
`  -fork`
`        a bool`
`  -numb int`
`        an int (default 42)`
`  -svar string`
`        a string var (default "bar")`
`  -word string`
`        a string (default "foo")`
___
#### The program can also be run directly with `zig run`; put the flags after a second `--`.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- command-line-flags.cpp -- -word=opt -numb=7 a1`

##### Results:

`word: opt`
`numb: 7`
`fork: false`
`svar: bar`
`tail: [a1]`
