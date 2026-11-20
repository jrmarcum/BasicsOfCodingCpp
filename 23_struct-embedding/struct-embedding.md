Go struct embedding maps to inheritance: `container` derives from `base`, which implements the abstract `describer` interface.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- struct-embedding.cpp`

##### Results:

`co={num: 1, str: some name}`
`also num: 1`
`describe: base with num=1`
`describer: base with num=1`
