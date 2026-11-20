Go's `signal.NotifyContext` maps to `std::signal` plus a lock-free `std::atomic<int>` that the handler sets and the main code polls. Go's `context.Cause` message is reproduced by mapping the signal number to text.
___
#### When we run this program it will block waiting for a signal. By typing `ctrl-C` (which the terminal shows as `^C`) we can send a `SIGINT` signal, causing the program to print the signal message and then exit. (On Unix, `kill -TERM <pid>` sends `SIGTERM`, which prints `terminated signal received`.)
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- signals.cpp`

##### Results:

`awaiting signal`
`^C`
`interrupt signal received`
`exiting`
