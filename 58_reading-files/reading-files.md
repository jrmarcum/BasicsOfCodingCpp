File reading uses `std::ifstream` with `read`, `seekg`, and `gcount`; `peek` is done by reading and seeking back. The stream closes itself (RAII).
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
#### Create a new file with some data to read.
___
##### Run Command:

`$ mkdir tmp`

`$ echo "hello" > ./tmp/dat.txt`

`$ echo "go" >> ./tmp/dat.txt`

___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- reading-files.cpp`

##### Results:

`hello`
`go`
`5 bytes: hello`
`2 bytes @ 6: go`
`2 bytes @ 6: go`
`5 bytes: hello`
