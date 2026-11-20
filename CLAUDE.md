# Basics of Coding C++ — Project Context

## Purpose

Multi-language comparative study of programming syntax, language simplicity,
lines of code required, and compile/run performance. C++ is one of several
languages implemented against the same set of example programs (Go, C, Rust,
V, ...), enabling direct side-by-side comparison.

## Upstream Reference

Lessons are translated from **Basics of Coding Go** by Jon Marcum, which is
itself adapted from **Go by Example** by Mark McGranaghan. The upstream Go
content lives at `upstream/` in this repository and is the source of truth
for program logic and expected output. The sibling **Basics of Coding C**
project (`../BasicsOfCodingC`) was used as a secondary reference for how
Go-only features were mapped onto a C-family language.

- Upstream Go repo: https://github.com/jrmarcum/BasicsOfCodingGo
- Sibling C repo: https://github.com/jrmarcum/BasicsOfCodingC
- Original Go by Example: https://github.com/mmcgrana/gobyexample

## Licensing Summary

- **CC BY 3.0** — lesson files and code examples adapted from Basics of
  Coding Go (Jon Marcum) / Go by Example (Mark McGranaghan).
- **CC0 1.0** — original contributions by Jon Marcum (project structure,
  README, CLAUDE.md, comparative-study additions). See LICENSE.

Attribution is centralized in NOTICE and the README Attribution/License
sections. **Do NOT add per-file attribution footers to lesson `.md` files.**

## Toolchain

C++ is compiled with `zig c++` / `zig run` (Zig's bundled Clang + libc++),
targeting **C++23**. This is the same single cross-platform toolchain the C
project uses. Verified with Zig 0.17.1 (Clang 22).

Compile and run pattern (`-cflags ... --` is how `zig run` accepts `-std`):
```
$ zig run -lc++ -cflags -std=c++23 -- filename.cpp
```

Programs that take command-line arguments (lessons 64–66) put them after a
second `--`:
```
$ zig run -lc++ -cflags -std=c++23 -- command-line-arguments.cpp -- a b c d
```

Build a standalone binary:
```
$ zig c++ -std=c++23 filename.cpp -o filename
```

Extra link flags:
```
$ zig run -lc++ -lws2_32 -cflags -std=c++23 -- file.cpp   # Windows sockets (69–72)
```

The first `zig run -lc++` on a machine builds libc++ into Zig's cache (about a
minute); later runs take a few seconds.

## Project Structure

```
BasicsOfCodingCpp/
├── CLAUDE.md          — this file
├── LICENSE            — CC0 (Jon Marcum's original contributions)
├── NOTICE             — attribution for CC BY 3.0 derived content
├── README.md          — overview, C++ language primer, attribution, license
├── upstream/          — reference copy of the BasicsOfCodingGo lessons
│   └── ##_topic-name/ — Go source and .md for each lesson
└── ##_topic-name/     — 78 lessons, 01_hello-world through 78_sha256-hashes
    ├── topic-name.cpp — runnable C++23 source
    └── topic-name.md  — lesson explanation, run command, expected output
```

## Lesson Conventions

**Source (`topic-name.cpp`)**
- One self-contained file per lesson; standard library only (no third-party
  libraries). Each lesson must compile cleanly with the command above.
- Follow the upstream Go program's structure, ordering, and comments: keep
  the Go explanatory comments, rewritten so they describe the C++ code.
  Use `//` comments.
- Use modern idiomatic C++23: `std::println`/`std::format` for output (not
  `printf`/`iostream`, except where a lesson is about streams/files),
  `auto`, range-for, `std::string`, `std::vector`, `std::map`, lambdas,
  RAII, `std::optional`/`std::expected`, `<ranges>`, `<chrono>`,
  `<filesystem>`, `<thread>`, `std::jthread`, etc.
- 4-space indentation, `int main()` (or `int main(int argc, char* argv[])`).
- **Output must match the upstream Go output** wherever practical, so the
  Results sections line up across languages. Go prints slices as
  `[a b c]` and maps as `map[k1:v1 k2:v2]`; C++23's built-in range formatting
  differs (`["a", "b"]`), so lessons that print containers define a small
  local `go_fmt` helper (see `09_slices/slices.cpp`).
- Where output legitimately differs (PRNG values, timestamps, thread ordering,
  pointer addresses, platform paths), say so in the `.md`.

**Docs (`topic-name.md`)**
- Optional first paragraph (no heading) explaining the C++ mapping when it
  is not obvious (see `09_slices/slices.md`). Then the standard block:
  ```
  ___
  #### To run the program, make sure you are in the program's folder, then use the Run Command below.
  ___
  ##### Run Command:

  `$ zig run -lc++ -cflags -std=c++23 -- topic-name.cpp`

  ##### Results:

  `line 1 of actual output`
  `line 2 of actual output`
  ```
- Each output line is its own backtick-wrapped line. Results are the
  **actual** output of running the program, not copied from Go.
- Keep any extra multi-step sections from the upstream `.md` (build a
  binary, run with args, etc.), adapted to the zig commands.

## C++ Translation Notes

| Go construct | C++ equivalent used |
|---|---|
| `fmt.Println` / `Printf` | `std::println` / `std::print` with `std::format` specs |
| Slices | `std::vector<T>` (owning) + `std::span<T>` (views) |
| Maps | `std::map` (ordered — matches Go's sorted map printing) / `std::unordered_map` |
| Multiple return values | `std::pair` / `std::tuple` + structured bindings |
| Variadic functions | Variadic templates / `std::initializer_list` |
| Closures | Lambdas with captures (`[x]`, `[&x]`, `mutable`) |
| Structs / methods | `struct` with member functions |
| Interfaces | Abstract base class with `virtual` functions |
| Struct embedding | Inheritance / composition |
| Enums + `String()` | `enum class` + `to_string` switch / `std::formatter` |
| Generics | Templates and C++20 concepts |
| `error` / `errors.Is` / `As` | `std::expected<T, E>`; custom error types; exceptions where idiomatic |
| `panic` / `recover` | `throw` / `try`–`catch` |
| `defer` | RAII scope guard (destructor runs at scope exit) |
| Goroutines | `std::jthread` / `std::thread` |
| Channels | Small `Channel<T>` class: `std::mutex` + `std::condition_variable` + `std::deque` |
| `select` / timeouts | `Channel::try_recv` / `recv_for(timeout)` polling |
| Timers / tickers | `std::jthread` + `std::stop_token` + `sleep_for` |
| `sync.WaitGroup` | Join a vector of `std::jthread` (or `std::latch`) |
| `sync.Mutex` | `std::mutex` + `std::lock_guard` / `std::scoped_lock` |
| `sync/atomic` | `std::atomic<uint64_t>` |
| Context / cancellation | `std::stop_source` / `std::stop_token` |
| `sort` / `slices.Sort` | `std::ranges::sort` (with projections / comparators) |
| `strings` package | `std::string` members, `<algorithm>`, `<ranges>` |
| `regexp` | `<regex>` (ECMAScript grammar) |
| JSON / XML | Hand-written minimal encoder/decoder (no std library support) |
| `time` | `<chrono>` (`system_clock`, durations, `std::format` chrono specs) |
| `math/rand` | `<random>` (`std::mt19937`, distributions) |
| `strconv` | `std::from_chars` / `std::stoi` / `std::stod` |
| `net/url` | Hand-written parser over `std::string_view` |
| SHA1 / SHA256 / Base64 | Implemented from scratch (not in the standard library) |
| `os` files / `bufio` | `<fstream>`, `std::getline`, `<filesystem>` |
| `filepath` / dirs / temp files | `<filesystem>` (`path`, `directory_iterator`, `temp_directory_path`) |
| `flag` package | Hand-written flag parser over `argv` |
| `os.Getenv` / `Setenv` | `std::getenv` / `_putenv_s` (Win) / `setenv` (POSIX) |
| `testing` | Self-contained mini test runner + `<chrono>` benchmark loop |
| `net/http`, TCP | Raw sockets: WinSock2 (Windows) / BSD sockets (POSIX) |
| `text/template` | Hand-written `{{.Field}}` substitution helpers |
| `os/exec` | `std::system` / `_popen`·`popen`; `_execvp`·`execvp` |
| `os/signal` | `std::signal` + `std::atomic<bool>` flag |
| `os.Exit` | `std::exit` (destructors / RAII guards do not run) |

## Notes for Future Claude Sessions

- Do **not** add per-file attribution footers to lesson `.md` files.
- Lessons 58–60 read/write files under `./tmp/`; create `tmp/` (and for 58,
  `tmp/dat.txt`) as the `.md` describes before running.
- Lesson 62 (directories) creates and removes `subdir/`; run from inside the
  lesson folder.
- VS Code IntelliSense may flag valid C++23 code (e.g. `<print>`); trust the
  `zig c++` compile as ground truth.
- All 78 lessons compile with `-Wall -Wextra`; the one remaining warning
  (17_pointers, `zeroval` parameter set but unused) is the point of the lesson.
- Lessons that print containers carry a local `go_fmt` helper (ranges →
  `[a b c]`, `std::map` → `map[k:v]`). Lesson 26 copies the 09 version.
- Concurrency lessons (28–37) inline the same `Channel<T>` class, trimmed per
  lesson. Capacity 0/1 behaves as a one-slot buffer, not a true unbuffered
  rendezvous. Declare channels before the threads that use them so threads
  join first at scope exit.
- libc++ in Zig 0.17.1 lacks `std::chrono::parse`/`from_stream` and a usable
  `current_zone()`. Lessons 50–52 print UTC in Go's layout and use
  `localtime_s`/`localtime_r` + `sscanf` for local offsets and parsing.
  Lessons 32/37 print UTC timestamps.
- `std::from_chars` does not infer a base or accept `0x`; lesson 54 strips the
  prefix and passes the base explicitly.
- Windows `std::filesystem`: use `generic_string()` for `/` separators, and
  `has_root_directory()` instead of `is_absolute()` for Go's `IsAbs("/x")`.
  `directory_iterator` order is unspecified, so lesson 62 sorts entries.
- Windows `_environ` is a macro in `<cstdlib>`; only declare
  `extern char** environ;` on POSIX (lesson 67).
- Lesson 53: `std::mt19937` values differ from Go's generator.
- Lesson 69: gobyexample.com now redirects plain HTTP to HTTPS and the std lib
  has no TLS, so the client defaults to `http://example.com` (host is an
  optional argument).
- Lessons 74/75 use `cmd /c dir`, `date /t`, `findstr` on Windows and the Go
  commands on Unix. Windows `_execvp` spawns a new process and exits rather
  than truly replacing the current one.
- Lesson 76: a real Ctrl+C was not automatable on Windows; the handler was
  verified via `std::raise(SIGINT)`.
- Lesson 77: `zig run` does not print Go's `exit status 3`; the `.md` builds
  the binary and checks `$?` / `$LASTEXITCODE` / `%ERRORLEVEL%`.
- Lesson 42 exits via an uncaught exception (libc++abi terminate message).
- Unix code paths (`#else` branches for sockets, env, exec, signals) were
  written but only Windows was tested; Unix builds may need `-lpthread`.
- When testing servers (70–72) from parallel sessions, stop them by PID, never
  by image name (`zig.exe`).
