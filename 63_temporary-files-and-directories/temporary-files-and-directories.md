C++ has no `TempFile`/`TempDir`; the program builds names under `std::filesystem::temp_directory_path()` with a random suffix, and a scope guard removes them at exit.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- temporary-files-and-directories.cpp`

##### Results:

`Temp file name: ...\Temp\sample549955371`
`Temp dir name: ...\Temp\sampledir79167811`
___
#### Note: Temp file and directory names will vary with each run and by platform (e.g. `/tmp/sample...` on Linux).
