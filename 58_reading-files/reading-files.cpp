// Reading and writing files are basic tasks needed for
// many C++ programs. First we'll look at some examples of
// reading files.
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <stdexcept>
#include <string>

// Reading files requires checking most calls for errors.
// This helper will streamline our error checks below by
// throwing when a stream has failed (like Go's `panic`).
void check(const std::ios& s, const char* what) {
    if (s.fail()) {
        throw std::runtime_error(std::string("error: ") + what);
    }
}

int main() {
    try {
        // Perhaps the most basic file reading task is
        // loading a file's entire contents into memory.
        std::ifstream in("./tmp/dat.txt", std::ios::binary);
        check(in, "open ./tmp/dat.txt");
        std::string dat{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
        std::print("{}", dat);
        in.close();

        // You'll often want more control over how and what
        // parts of a file are read. For these tasks, open
        // an `std::ifstream` (RAII: it closes itself when it
        // goes out of scope).
        std::ifstream f("./tmp/dat.txt", std::ios::binary);
        check(f, "open ./tmp/dat.txt");

        // Read some bytes from the beginning of the file.
        // Allow up to 5 to be read but also note how many
        // actually were read.
        std::string b1(5, '\0');
        f.read(b1.data(), 5);
        auto n1 = f.gcount();
        check(f, "read");
        std::println("{} bytes: {}", n1, b1.substr(0, n1));

        // You can also `seekg` to a known location in the file
        // and `read` from there.
        f.seekg(6, std::ios::beg);
        auto o2 = f.tellg();
        std::string b2(3, '\0');
        f.read(b2.data(), 3);
        auto n2 = f.gcount();
        check(f, "read");
        std::println("{} bytes @ {}: {}", n2, static_cast<int64_t>(o2), b2.substr(0, n2));

        // A read loop like the one above can be made more
        // robust by insisting on a minimum count: here we
        // check that all 3 requested bytes arrived (the
        // equivalent of Go's `io.ReadAtLeast`).
        f.seekg(6, std::ios::beg);
        auto o3 = f.tellg();
        std::string b3(3, '\0');
        f.read(b3.data(), 3);
        if (f.gcount() < 3) throw std::runtime_error("unexpected EOF");
        std::println("{} bytes @ {}: {}", f.gcount(), static_cast<int64_t>(o3), b3);

        // There is no built-in rewind, but `clear()` followed
        // by `seekg(0)` accomplishes this.
        f.clear();
        f.seekg(0, std::ios::beg);
        check(f, "seek");

        // `std::ifstream` is already buffered, and `peek`
        // looks ahead without consuming. To peek 5 bytes, read
        // them and then seek back to where we were.
        auto pos = f.tellg();
        std::string b4(5, '\0');
        f.read(b4.data(), 5);
        check(f, "peek");
        f.seekg(pos);
        std::println("5 bytes: {}", b4);

        // The file is closed automatically when `f` goes out
        // of scope; no explicit `Close` call is needed.
    } catch (const std::exception& e) {
        std::println(stderr, "{}", e.what());
        return 1;
    }
}
