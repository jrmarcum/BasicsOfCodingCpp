# Basics of Coding C++

## Project Overview

This project is one language implementation within a **multi-language comparative study** of
programming syntax, lines of code required, and compile/run performance. The same set of 78
example programs is implemented in C++, C, Go, Rust, V, and other languages so that the
languages can be compared side-by-side.

Lessons are translated from **[Basics of Coding Go](https://github.com/jrmarcum/BasicsOfCodingGo)**
by Jon Marcum, which is itself adapted from **[Go by Example](https://github.com/mmcgrana/gobyexample)**
by Mark McGranaghan. The sibling **[Basics of Coding C](https://github.com/jrmarcum/BasicsOfCodingC)**
project served as a secondary reference.

## Project Structure

```text
BasicsOfCodingCpp/
├── CLAUDE.md            — canonical project context for AI-assisted development
├── LICENSE              — CC0 (Jon Marcum's original contributions)
├── NOTICE               — attribution for CC BY 3.0 derived content
├── README.md            — this file
├── upstream/            — reference copy of BasicsOfCodingGo lessons
│   └── ##_topic-name/   — Go source and .md for each lesson
└── ##_topic-name/       — 78 lessons, 01_hello-world through 78_sha256-hashes
    ├── topic-name.cpp   — runnable C++23 source
    └── topic-name.md    — lesson explanation with run command and expected output
```

> **AI-assisted development:** `CLAUDE.md` in the project root contains full context for
> Claude sessions — toolchain, lesson conventions, and C++ translation decisions.

## Preface

What this text is and what it is not: This text is intended to introduce the reader to the basics of the C++ programming language in the sense that they will be able to write minimal types of programs and run the code. It is not intended to go into advanced topics like template metaprogramming, advanced data structures, in-depth object-oriented design, testing and debugging techniques, and other software engineering principles.

The programs are intended to be run in the terminal as that is common to most operating systems. Linux and Mac come preinstalled with a terminal. Windows may or may not have it pre-installed. "Windows Terminal" can be installed from the Microsoft Store.

## Installation and Setup of Zig (to compile and run C++)

To set up Zig, navigate to the [ziglang](https://github.com/ziglang/zig/wiki/Install-Zig-from-a-Package-Manager) website and locate an appropriate package manager for your operating system (brew for Mac, scoop or chocolatey for Windows, and varies based on Linux distribution). If Zig has been installed properly you will be able to type the following command in the terminal and receive a response like the one shown:

```text
$ zig version
0.17.1
(Note: the version shown here varies with your installed version)
```

Zig ships with a built-in C++ compiler (`zig c++`, based on Clang) and the libc++ standard library, so no separate compiler install is needed. All examples in this project use **C++23** and run with a single command:

```sh
$ zig run -lc++ -cflags -std=c++23 -- filename.cpp
```

- `-lc++` links the C++ standard library.
- `-cflags -std=c++23 --` passes the language standard to the compiler (everything between `-cflags` and `--` is a compiler flag).
- The very first run builds libc++ into Zig's cache and takes about a minute; subsequent runs take a few seconds.

A few lessons need extra arguments:

- **Command-line args** (lessons 64–66): `zig run -lc++ -cflags -std=c++23 -- filename.cpp -- arg1 arg2 ...`
- **Windows networking** (lessons 69–72): `zig run -lc++ -lws2_32 -cflags -std=c++23 -- filename.cpp`

To build a standalone binary instead:

```sh
$ zig c++ -std=c++23 filename.cpp -o filename
```

Any other C++23 compiler (g++ 14+, clang++ 18+, MSVC 19.40+) should also work, e.g. `g++ -std=c++23 filename.cpp -o filename` (with GCC 14/15 add `-lstdc++exp` for `std::print`).

## Comments
Comments are used to document what your code does so that others can understand it when reviewing your code.
```cpp
// This is a single-line comment
/* This is a
   multi-line comment */
```

## Keywords
|              |              |              |               |              |
|:------------:|:------------:|:------------:|:-------------:|:------------:|
| auto         | bool         | break        | case          | catch        |
| char         | class        | concept      | const         | consteval    |
| constexpr    | continue     | co_await     | decltype      | default      |
| delete       | do           | double       | else          | enum         |
| explicit     | extern       | false        | float         | for          |
| friend       | if           | inline       | int           | long         |
| mutable      | namespace    | new          | noexcept      | nullptr      |
| operator     | private      | protected    | public        | requires     |
| return       | short        | signed       | sizeof        | static       |
| static_cast  | struct       | switch       | template      | this         |
| throw        | true         | try          | typedef       | typename     |
| union        | unsigned     | using        | virtual       | void         |
| volatile     | while        |              |               |              |

## Data Types
> ### 1. Fundamental Types
>> #### a. **bool**: `true` or `false`
>> #### b. **char**: single character or small integer, 1 byte (`char8_t`, `char16_t`, `char32_t` for Unicode)
>> #### c. **short / int / long / long long**: signed integers (fixed-width: `int8_t` … `int64_t`)
>> #### d. **float / double / long double**: floating point
>> #### e. **void**: absence of a type
> ### 2. Compound Types
>> #### a. **Pointers** (`T*`) and **references** (`T&`): refer to another object
>> #### b. **Arrays**: built-in `T[N]` or `std::array<T, N>`
>> #### c. **Classes / structs**: data members plus member functions
>> #### d. **Enums**: `enum class` for scoped, strongly typed constants
>> #### e. **Unions**: members share the same memory (prefer `std::variant`)
> ### 3. Standard Library Types
>> #### a. **std::string**, **std::string_view**: text
>> #### b. **std::vector**, **std::span**: dynamic arrays and views (Go slices)
>> #### c. **std::map**, **std::unordered_map**: key-value maps
>> #### d. **std::optional**, **std::expected**, **std::variant**: values that may be absent, errors, or one of several types

## Operators
> ### Arithmetic Operators
> |        |                                                    |
> |:------:|:---------------------------------------------------|
> | **+**  | add one number to another                          |
> | **-**  | subtract one number from another                   |
> | **\*** | multiply one number by another                     |
> | **/**  | divide one number by another                       |
> | **%**  | remainder of dividing one number by another        |
> | **++** | increase the value of a variable by 1              |
> | **--** | decrease the value of a variable by 1              |
> ### Comparison Operators
> |         |                                                         |
> |:-------:|:--------------------------------------------------------|
> | **==**  | check if a variable is equal to another                 |
> | **!=**  | check if a variable is not equal to another             |
> | **>**   | check if a variable is greater than another             |
> | **<**   | check if a variable is less than another                |
> | **>=**  | check if a variable is greater than or equal to another |
> | **<=**  | check if a variable is less than or equal to another    |
> | **<=>** | three-way comparison (less, equal, or greater)          |
> ### Logical Operators
> |        |                                                 |
> |:------:|:------------------------------------------------|
> | **&&** | returns true if both statements are true        |
> | **\|\|** | returns true if one of the statements is true |
> | **!**  | reverse the result of a true or false statement |
> ### Assignment Operators
> |         |                                                                        |
> |:-------:|:-----------------------------------------------------------------------|
> | **=**   | assign a value to a variable                                           |
> | **+=**  | add a number to the existing value and assign the result               |
> | **-=**  | subtract a number from the existing value and assign the result        |
> | **\*=** | multiply the existing value by a number and assign the result          |
> | **/=**  | divide the existing value by a number and assign the result            |
> | **%=**  | take the remainder of the existing value divided by a number           |

## Statements
A statement is an instruction that a program can execute.
> ### Conditional Statements
> | **if**        | performs a statement if a condition is met                                           |
> | **if-else**   | performs a statement if a condition is met, otherwise performs the else statement    |
> | **switch**    | selects one of many code blocks to execute based on an expression                   |
> ### Iterative Statements
> | **for**       | loop with initial, condition, and increment expressions                 |
> | **range-for** | `for (auto& x : container)` iterates over every element                 |
> | **while**     | loop that continues while a condition is true                          |
> | **do-while**  | executes the body at least once, then continues while condition is true |
> ### Transfer Statements
> | **break**     | terminates the enclosing loop or switch statement               |
> | **continue**  | skips the rest of the loop body and continues to next iteration |
> | **return**    | returns a value from a function                                 |
> | **throw**     | raises an exception, caught by an enclosing `try`/`catch`       |

## Functions
> ### 1. **main**: entry point of every C++ program
> ### 2. **std::print / std::println**: formatted output (C++23), using `std::format` syntax
> ### 3. **std::cin / std::getline**: input
> ### 4. **Lambdas**: anonymous functions `[capture](params) { body }`
> ### 5. **std::make_unique / std::make_shared**: dynamic memory with automatic cleanup (RAII)

## File Input and Output
> ### 1. File Input
>> #### a. `std::ifstream`, `std::getline`, `rdbuf()`
>> #### b. Error handling by checking stream state (`if (!file)`)
> ### 2. File Output
>> #### a. `std::ofstream`, `<<`, `write`
>> #### b. Files close automatically when the stream goes out of scope
> ### 3. Filesystem
>> #### a. `std::filesystem` for paths, directories, and temporary files

## Attribution

This project is adapted in part from **[Basics of Coding Go](https://github.com/jrmarcum/BasicsOfCodingGo)**
by [Jon Marcum](https://github.com/jrmarcum), licensed under the
[Creative Commons Attribution 3.0 Unported License](http://creativecommons.org/licenses/by/3.0/).

Basics of Coding Go was itself adapted in part from **[Go by Example](https://github.com/mmcgrana/gobyexample)**
by [Mark McGranaghan](https://github.com/mmcgrana), also licensed under the
[Creative Commons Attribution 3.0 Unported License](http://creativecommons.org/licenses/by/3.0/).

Attribution for all derived lesson content is centralized in this README and in the NOTICE file.
This project exists as a platform for multi-language comparative study of syntax, language
simplicity, lines of code required, and compile/run performance.

## License

This repository contains two tiers of content:

| Content | License |
| --- | --- |
| Lesson files and code examples adapted from *Basics of Coding Go* (derived from *Go by Example*) | [CC BY 3.0](http://creativecommons.org/licenses/by/3.0/) — see NOTICE |
| Original contributions by Jon Marcum (project structure, README, CLAUDE.md, comparative study additions) | [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/) — see LICENSE |

The root `LICENSE` file (CC0) applies to Jon Marcum's original contributions only.
The `NOTICE` file clarifies that CC BY 3.0 governs all content adapted from *Basics of Coding Go*
and *Go by Example*.
