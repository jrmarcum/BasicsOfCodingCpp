C++ has no `os/exec`. `popen` / `_popen` run a command line through the shell and give a pipe to its stdout (mode `"r"`, used by a small `output` helper standing in for `Command.Output`) or stdin (mode `"w"`, used for `grep`). `std::system` runs a command and shows its output directly. The commands are platform-specific: Unix uses `date`, `date -x`, `grep`, `ls -a -l -h`; Windows uses `date /t`, `cmd /c exit 1`, `findstr`, `dir /b`. Because `popen` is one-directional, grep's output goes straight to our stdout instead of being read back.
___
#### The spawned programs return output that is the same as if we had run them directly from the command-line.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- spawning-processes.cpp`

##### Results (Windows):

`> date`
`Fri 11/20/2026 `
`command exit rc = 1`
`> grep hello`
`hello grep`
`> dir /b`
`spawning-processes.cpp`

##### Results (Unix, as in the Go lesson):

`> date`
`Thu 05 May 2022 10:10:12 PM PDT`
`command exit rc = 1`
`> grep hello`
`hello grep`
`> ls -a -l -h`
`drwxr-xr-x  4 mark 136B Oct 3 16:29 .`
`drwxr-xr-x 91 mark 3.0K Oct 3 12:50 ..`
`-rw-r--r--  1 mark 1.3K Oct 3 16:28 spawning-processes.cpp`
