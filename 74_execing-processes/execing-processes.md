Go's `syscall.Exec` maps to `execvp` (POSIX) / `_execvp` (Windows), selected with `#ifdef _WIN32`. On Unix the process is truly replaced by `ls -a -l -h`. On Windows there is no `ls` and `_execvp` starts the new process and ends the current one (the shell may print its prompt before the new process's output appears), so the program execs `cmd /c dir` instead.
___
#### When we run our program it is replaced by `ls` (Unix) or `dir` (Windows).
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- execing-processes.cpp`

##### Results (Windows):

` Volume in drive D is USB322FD`
` Volume Serial Number is 86A8-F7F3`
` Directory of D:\Programs\_ProgramExamples\Example_Programs\GithubProjects\BasicsOfCodingCpp\74_execing-processes`
`11/20/2026  02:07 PM    <DIR>          .`
`11/20/2026  02:07 PM    <DIR>          ..`
`11/20/2026  02:12 PM             1,605 execing-processes.cpp`
`               1 File(s)          1,605 bytes`
`               2 Dir(s)  192,012,615,680 bytes free`

##### Results (Unix, as in the Go lesson):

`total 16`
`drwxr-xr-x  4 mark 136B Oct 3 16:29 .`
`drwxr-xr-x 91 mark 3.0K Oct 3 12:50 ..`
`-rw-r--r--  1 mark 1.3K Oct 3 16:28 execing-processes.cpp`
