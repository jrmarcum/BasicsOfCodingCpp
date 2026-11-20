The C++ standard library has no hash functions, so `sha256-hashes.cpp` implements FIPS 180-4 SHA-256 from scratch in a small `Sha256` class (`write` / `sum`, like Go's `h.Write` / `h.Sum`). It was also verified against the standard vectors for `"abc"` and the empty string. Go's example output is truncated; the full digest is shown here.
___
#### Running the program computes the hash and prints it in a human-readable hex format.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- sha256-hashes.cpp`

##### Results:

`sha256 this string`
`1af1dfa857bf1d8814fe1af8983c18080019922e557f15a8a0d3db739d77aacb`
