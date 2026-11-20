___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- hello-world.cpp`

##### Results:

`hello world`
___
#### Sometimes we'll want to build our programs into binaries. We can do this using `zig c++`.
___
##### Run Command:

`$ zig c++ -std=c++23 hello-world.cpp -o hello-world`

`$ ls`

##### Results:

`hello-world.exe (or hello-world)`
`hello-world.cpp`
`hello-world.md`
___
#### We can then execute the built binary directly.
___
##### Run Command:

`$ ./hello-world`

##### Results:

`hello world`
