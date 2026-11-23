File writing uses `std::ofstream`; it is buffered and closes itself (RAII), covering both Go's `os.File` and `bufio.Writer`.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
#### Try running the file-writing code. The `tmp` folder must exist first.
___
##### Run Command:

`$ mkdir tmp`

`$ zig run -lc++ -cflags -std=c++23 -- writing-files.cpp`

##### Results:

`wrote 5 bytes`
`wrote 7 bytes`
`wrote 9 bytes`
___
#### Then check the contents of the written files.
___
##### Run Command:

`$ cat ./tmp/dat1.txt`

##### Results:

`hello`
`cpp`
___
##### Run Command:

`$ cat ./tmp/dat2.txt`

##### Results:

`some`
`writes`
`buffered`
