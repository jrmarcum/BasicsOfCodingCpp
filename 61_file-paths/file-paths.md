`std::filesystem::path` joins with `/`, `lexically_normal` cleans paths, and `generic_string()` prints forward slashes so the output is the same on Windows and Linux. Go's `IsAbs("/dir/file")` is `true` on every OS, while on Windows `path::is_absolute` needs a drive letter, so the program tests `has_root_directory()` instead.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- file-paths.cpp`

##### Results:

`p: dir1/dir2/filename`
`dir1/filename`
`dir1/filename`
`Dir(p): dir1/dir2`
`Base(p): filename`
`false`
`true`
`.json`
`config`
`t/file`
`../c/t/file`
