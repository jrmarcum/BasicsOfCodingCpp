The same hand-written `FlagSet` from the previous lesson (modelled on Go's `flag.NewFlagSet`) is used here, one instance per subcommand.
___
##### Run Command:

`$ zig c++ -std=c++23 command-line-subcommands.cpp -o command-line-subcommands`

___
#### First invoke the foo subcommand.
___
##### Run Command:

`$ ./command-line-subcommands foo -enable -name=joe a1 a2`

##### Results:

`subcommand 'foo'`
`  enable: true`
`  name: joe`
`  tail: [a1 a2]`
___
#### Now try bar.
___
##### Run Command:

`$ ./command-line-subcommands bar -level 8 a1`

##### Results:

`subcommand 'bar'`
`  level: 8`
`  tail: [a1]`
___
#### But bar won't accept foo's flags.
___
##### Run Command:

`$ ./command-line-subcommands bar -enable a1`

##### Results:

`flag provided but not defined: -enable`
`Usage of bar:`
`  -level int`
`        level`
___
#### Without building a binary, use `zig run` and put the program's arguments after a second `--`.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- command-line-subcommands.cpp -- foo -enable -name=joe a1 a2`

##### Results:

`subcommand 'foo'`
`  enable: true`
`  name: joe`
`  tail: [a1 a2]`
