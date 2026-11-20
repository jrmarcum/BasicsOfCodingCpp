The C++ standard library has no hash functions, so SHA1 is implemented from scratch in a small `Sha1` class (`write` then `sum`, like Go). The result was checked against Go's output for the same input.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
Running the program computes the hash and prints it in a human-readable hex format.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- sha1-hashes.cpp`

##### Results:

`sha1 this string`
`cf23df2207d99a74fbe169e3eba035e633b65d94`
___
#### You can compute other hashes using a similar pattern to the one shown above (see lesson 78 for SHA256).
___
#### Note that if you need cryptographically secure hashes, you should carefully research [hash strength](http://en.wikipedia.org/wiki/Cryptographic_hash_function).
