// The `<filesystem>` header provides functions to parse
// and construct *file paths* in a way that is portable
// between operating systems; `dir/file` on Linux vs.
// `dir\file` on Windows, for example.
#include <filesystem>
#include <print>
#include <string>

namespace fs = std::filesystem;

// Go's `filepath` functions return clean forward-slash paths
// on every OS we show here; `generic_string()` gives the same
// layout on Windows, where the native separator is `\`.
static std::string gen(const fs::path& p) { return p.generic_string(); }

// Mimic Go's `filepath.Join`: the `/` operator joins paths,
// and `lexically_normal` removes superfluous separators and
// directory changes (`..`).
static fs::path join(const fs::path& a, const fs::path& b) {
    return (a / b).lexically_normal();
}

int main() {
    // `/` (or `join`) should be used to construct paths in a
    // portable way. It builds a hierarchical path from its
    // arguments.
    fs::path p = join(join("dir1", "dir2"), "filename");
    std::println("p: {}", gen(p));

    // You should always use path joining instead of
    // concatenating `/`s or `\`s manually. In addition
    // to providing portability, `lexically_normal` will also
    // normalize paths by removing superfluous separators
    // and directory changes.
    std::println("{}", gen(join("dir1//", "filename")));
    std::println("{}", gen(join("dir1/../dir1", "filename")));

    // `parent_path` and `filename` can be used to split a path
    // to the directory and the file.
    std::println("Dir(p): {}", gen(p.parent_path()));
    std::println("Base(p): {}", gen(p.filename()));

    // We can check whether a path is absolute. On Windows a
    // path needs a drive letter to be absolute, so a rooted
    // path like `/dir/file` is only "relative" there; we test
    // `has_root_directory` to match Go's answer on every OS.
    std::println("{}", fs::path("dir/file").has_root_directory());
    std::println("{}", fs::path("/dir/file").has_root_directory());

    std::string filename = "config.json";

    // Some file names have extensions following a dot. We
    // can split the extension out of such names with `extension`.
    std::string ext = fs::path(filename).extension().string();
    std::println("{}", ext);

    // To find the file's name with the extension removed,
    // use `stem`.
    std::println("{}", fs::path(filename).stem().string());

    // `lexically_relative` finds a relative path between a
    // *base* and a *target*. It returns an empty path if the
    // target cannot be made relative to base.
    fs::path rel = fs::path("a/b/t/file").lexically_relative("a/b");
    if (rel.empty()) {
        std::println(stderr, "error: cannot make relative path");
        return 1;
    }
    std::println("{}", gen(rel));

    rel = fs::path("a/c/t/file").lexically_relative("a/b");
    if (rel.empty()) {
        std::println(stderr, "error: cannot make relative path");
        return 1;
    }
    std::println("{}", gen(rel));
}
