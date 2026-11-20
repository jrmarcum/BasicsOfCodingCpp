// C++'s `<filesystem>` has several useful functions for
// working with *directories* in the file system.
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// A scope guard: its destructor runs at scope exit, like
// Go's `defer`. `std::filesystem::remove_all` will delete a
// whole directory tree (similarly to `rm -rf`).
struct RemoveAllGuard {
    fs::path path;
    ~RemoveAllGuard() {
        std::error_code ec;
        fs::remove_all(path, ec);
    }
};

// `visit` is called for every file or directory found
// recursively by the walk below.
void visit(const fs::path& p, bool isDir) {
    std::println("  {} {}", p.generic_string(), isDir);
}

// List a directory's contents sorted by name (Go's `ReadDir`
// returns entries sorted; `directory_iterator` does not).
void list_dir(const fs::path& dir) {
    std::vector<fs::directory_entry> entries;
    for (const auto& e : fs::directory_iterator(dir)) entries.push_back(e);
    std::ranges::sort(entries, {}, [](const fs::directory_entry& e) { return e.path().filename(); });
    for (const auto& e : entries) {
        std::println("  {} {}", e.path().filename().string(), e.is_directory());
    }
}

int main() {
    try {
        // Create a new sub-directory in the current working
        // directory.
        fs::create_directory("subdir");

        // When creating temporary directories, it's good
        // practice to remove them at scope exit. The guard
        // deletes the whole tree when `main` returns.
        RemoveAllGuard cleanup{"subdir"};

        // Helper function to create a new empty file.
        auto createEmptyFile = [](const fs::path& name) {
            std::ofstream out(name);
            if (!out) throw std::runtime_error("cannot create " + name.string());
        };

        createEmptyFile("subdir/file1");

        // We can create a hierarchy of directories, including
        // parents with `create_directories`. This is similar to
        // the command-line `mkdir -p`.
        fs::create_directories("subdir/parent/child");

        createEmptyFile("subdir/parent/file2");
        createEmptyFile("subdir/parent/file3");
        createEmptyFile("subdir/parent/child/file4");

        // `directory_iterator` lists directory contents,
        // yielding `directory_entry` objects.
        std::println("Listing subdir/parent");
        list_dir("subdir/parent");

        // `current_path` lets us change the current working
        // directory, similarly to `cd`.
        fs::current_path("subdir/parent/child");

        // Now we'll see the contents of `subdir/parent/child`
        // when listing the *current* directory.
        std::println("Listing subdir/parent/child");
        list_dir(".");

        // `cd` back to where we started.
        fs::current_path("../../..");

        // We can also visit a directory *recursively*,
        // including all its sub-directories.
        // `recursive_directory_iterator` visits every file or
        // directory; we collect and sort them to get Go's
        // lexical `Walk` order, starting with the root itself.
        std::println("Visiting subdir");
        visit("subdir", true);
        std::vector<fs::directory_entry> all;
        for (const auto& e : fs::recursive_directory_iterator("subdir")) all.push_back(e);
        std::ranges::sort(all, {}, [](const fs::directory_entry& e) { return e.path().generic_string(); });
        for (const auto& e : all) visit(e.path(), e.is_directory());
    } catch (const std::exception& e) {
        std::println(stderr, "error: {}", e.what());
        return 1;
    }
}
