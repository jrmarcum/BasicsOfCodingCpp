C++ uses `<random>`: an engine (`std::mt19937`) generates bits and a distribution (`uniform_int_distribution`, `uniform_real_distribution`) shapes them into a range. Go's generator is different from `std::mt19937`, so seeded values (such as seed `42`) will not match Go's output.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- random-numbers.cpp`

##### Results:

`27,74`
`0.6514338553828746`
`6.7558296506286535,5.951530332424757`
`1,5`
`51,92`
`51,92`
___
#### Note: Numbers will vary per run, except the last two lines, which use the same fixed seed (`42`) and are always identical to each other.
___
#### See the [`<random>`](https://en.cppreference.com/w/cpp/numeric/random) docs for other random quantities that C++ can provide.
