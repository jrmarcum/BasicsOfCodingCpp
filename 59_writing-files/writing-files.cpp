// Writing files in C++ follows similar patterns to the
// ones we saw earlier for reading.
#include <cstdint>
#include <fstream>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// Throw if a stream is in a failed state (like Go's `panic`).
void check(const std::ios& s, const char* what) {
    if (s.fail()) {
        throw std::runtime_error(std::string("error: ") + what);
    }
}

int main() {
    try {
        // To start, here's how to dump a string (or just
        // bytes) into a file.
        std::string_view d1 = "hello\ngo\n";
        {
            std::ofstream out("./tmp/dat1.txt", std::ios::binary);
            check(out, "create ./tmp/dat1.txt");
            out << d1;
        }

        // For more granular writes, open a file for writing.
        // `std::ofstream` is RAII: the destructor closes the
        // file when `f` goes out of scope, so no `defer` is
        // needed.
        std::ofstream f("./tmp/dat2.txt", std::ios::binary);
        check(f, "create ./tmp/dat2.txt");

        // You can `write` byte buffers as you'd expect.
        std::vector<uint8_t> d2{115, 111, 109, 101, 10};
        f.write(reinterpret_cast<const char*>(d2.data()), static_cast<std::streamsize>(d2.size()));
        check(f, "write");
        std::println("wrote {} bytes", d2.size());

        // Streaming a string is also available.
        std::string_view s3 = "writes\n";
        f << s3;
        check(f, "write");
        std::println("wrote {} bytes", s3.size());

        // Issue a `flush` to push writes to the OS.
        f.flush();

        // `std::ofstream` is already buffered, so it plays
        // the part of Go's `bufio.Writer` too.
        std::string_view s4 = "buffered\n";
        f << s4;
        check(f, "write");
        std::println("wrote {} bytes", s4.size());

        // Use `flush` to ensure all buffered operations have
        // been applied to the underlying file.
        f.flush();
    } catch (const std::exception& e) {
        std::println(stderr, "{}", e.what());
        return 1;
    }
}
