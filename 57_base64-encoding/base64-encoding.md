C++ has no base64 support in its standard library, so both the standard and URL-safe encoders/decoders are written from scratch.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
The string encodes to slightly different values with the standard and URL base64 encoders (trailing `+` vs `-`) but they both decode to the original string as desired.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- base64-encoding.cpp`

##### Results:

`YWJjMTIzIT8kKiYoKSctPUB+`
`abc123!?$*&()'-=@~`

`YWJjMTIzIT8kKiYoKSctPUB-`
`abc123!?$*&()'-=@~`
