The filter reads stdin with `std::getline` and uppercases each line.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
#### To try out our line filter, first make a file with a few lowercase lines. If "lines.txt" file exists delete it before doing the exercise.
___
##### Run Command:

`$ mkdir tmp`

`$ echo 'hello'   > ./tmp/lines.txt`

`$ echo 'filter' >> ./tmp/lines.txt`

___
#### Then use the line filter to get uppercase lines.
___
##### Run Command:

`$ cat ./tmp/lines.txt | zig run -lc++ -cflags -std=c++23 -- line-filters.cpp`

##### Results:

`HELLO`
`FILTER`
