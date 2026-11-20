// Throughout program execution, we often want to create
// data that isn't needed after the program exits.
// *Temporary files and directories* are useful for this
// purpose since they don't pollute the file system over
// time.
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <print>
#include <random>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

// The C++ standard library has no `TempFile`; instead,
// `temp_directory_path` gives the default temp location for
// our OS, and we add a random suffix to the prefix so that
// concurrent calls will always create different names.
static std::string random_suffix() {
    static std::mt19937_64 rng{std::random_device{}()};
    return std::to_string(rng() % 1000000000ULL);
}

// Remove a path at scope exit, like Go's `defer os.Remove`
// / `defer os.RemoveAll`.
struct RemoveAllGuard {
    fs::path path;
    ~RemoveAllGuard() {
        std::error_code ec;
        fs::remove_all(path, ec);
    }
};

int main() {
    try {
        // Create a temporary file in the default location for
        // our OS (`temp_directory_path`) and open it for
        // writing. `std::ios::trunc | std::ios::out` creates
        // the file.
        fs::path fname0 = fs::temp_directory_path() / ("sample" + random_suffix());
        std::ofstream f(fname0, std::ios::binary);
        if (!f) throw std::runtime_error("cannot create " + fname0.string());

        // Display the name of the temporary file. On
        // Unix-based OSes the directory will likely be `/tmp`.
        // The file name starts with the prefix we chose and the
        // rest is random to avoid name clashes.
        std::println("Temp file name: {}", fname0.string());

        // Clean up the file after we're done. The OS is
        // likely to clean up temporary files by itself after
        // some time, but it's good practice to do this
        // explicitly. (The file is closed below, before this guard
        // runs at scope exit.)
        RemoveAllGuard cleanupFile{fname0};

        // We can write some data to the file.
        const char data[] = {1, 2, 3, 4};
        f.write(data, sizeof data);
        if (!f) throw std::runtime_error("write failed");
        f.close();

        // If we intend to write many temporary files, we may
        // prefer to create a temporary *directory*. It is made
        // the same way as the file name, then created with
        // `create_directory`.
        fs::path dname = fs::temp_directory_path() / ("sampledir" + random_suffix());
        fs::create_directory(dname);
        std::println("Temp dir name: {}", dname.string());

        RemoveAllGuard cleanupDir{dname};

        // Now we can synthesize temporary file names by
        // joining them onto our temporary directory.
        fs::path fname = dname / "file1";
        std::ofstream out(fname, std::ios::binary);
        if (!out) throw std::runtime_error("cannot create " + fname.string());
        const char data2[] = {1, 2};
        out.write(data2, sizeof data2);
    } catch (const std::exception& e) {
        std::println(stderr, "error: {}", e.what());
        return 1;
    }
}
