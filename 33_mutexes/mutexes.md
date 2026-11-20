Go's `sync.Mutex` is `std::mutex`, locked with `std::lock_guard` (RAII replaces `defer`). Go's `wg.Go` is replaced by a vector of `std::jthread` that joins when it goes out of scope.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- mutexes.cpp`

##### Results:

`map[a:20000 b:10000]`
