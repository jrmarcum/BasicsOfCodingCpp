`<filesystem>` provides `create_directory`, `create_directories`, `directory_iterator`, `recursive_directory_iterator`, and `current_path`. Entries are sorted by name to match Go's `ReadDir`/`Walk` order, and a scope guard removes `subdir` at exit (Go's `defer`). Run from inside the lesson folder.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- directories.cpp`

##### Results:

`Listing subdir/parent`
`  child true`
`  file2 false`
`  file3 false`
`Listing subdir/parent/child`
`  file4 false`
`Visiting subdir`
`  subdir true`
`  subdir/file1 false`
`  subdir/parent true`
`  subdir/parent/child true`
`  subdir/parent/child/file4 false`
`  subdir/parent/file2 false`
`  subdir/parent/file3 false`
