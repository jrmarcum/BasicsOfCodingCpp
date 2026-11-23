// `std::string` and `<algorithm>` / `<ranges>` provide many
// useful string-related functions. Here are some examples
// to give you a sense of what is available.
#include <algorithm>
#include <cctype>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

// Go prints slices as `[a b c]`; this helper reproduces that.
std::string cpp_fmt(const std::vector<std::string>& v) {
    std::string out = "[";
    for (bool first = true; const auto& e : v) {
        if (!first) out += ' ';
        out += e;
        first = false;
    }
    return out + "]";
}

// Small helpers for operations that have no one-call
// equivalent in the C++ standard library.
int Count(std::string_view s, std::string_view sub) {
    int n = 0;
    for (auto pos = s.find(sub); pos != std::string_view::npos; pos = s.find(sub, pos + sub.size())) {
        n++;
    }
    return n;
}

std::string Join(const std::vector<std::string>& v, std::string_view sep) {
    std::string out;
    for (std::size_t i = 0; i < v.size(); i++) {
        if (i > 0) out += sep;
        out += v[i];
    }
    return out;
}

std::string Repeat(std::string_view s, int n) {
    std::string out;
    for (int i = 0; i < n; i++) out += s;
    return out;
}

// Replace the first `n` occurrences of `from` (all if n < 0).
std::string Replace(std::string s, std::string_view from, std::string_view to, int n) {
    std::size_t pos = 0;
    while (n != 0 && (pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.size(), to);
        pos += to.size();
        n--;
    }
    return s;
}

std::vector<std::string> Split(std::string_view s, std::string_view sep) {
    std::vector<std::string> out;
    for (auto part : s | std::views::split(sep)) {
        out.emplace_back(part.begin(), part.end());
    }
    return out;
}

std::string ToLower(std::string s) {
    std::ranges::transform(s, s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string ToUpper(std::string s) {
    std::ranges::transform(s, s.begin(), [](unsigned char c) { return std::toupper(c); });
    return s;
}

int main() {
    // Here's a sample of the functions available. Many are
    // member functions of the string itself (`contains`,
    // `starts_with`, `ends_with`, `find`), while others we
    // wrap in the small helpers above.
    std::string test = "test";
    std::println("Contains:   {}", test.contains("es"));
    std::println("Count:      {}", Count("test", "t"));
    std::println("HasPrefix:  {}", test.starts_with("te"));
    std::println("HasSuffix:  {}", test.ends_with("st"));
    std::println("Index:      {}", test.find("e"));
    std::println("Join:       {}", Join({"a", "b"}, "-"));
    std::println("Repeat:     {}", Repeat("a", 5));
    std::println("Replace:    {}", Replace("foo", "o", "0", -1));
    std::println("Replace:    {}", Replace("foo", "o", "0", 1));
    std::println("Split:      {}", cpp_fmt(Split("a-b-c-d-e", "-")));
    std::println("ToLower:    {}", ToLower("TEST"));
    std::println("ToUpper:    {}", ToUpper("test"));

    // Getting the length of a string in bytes and getting a
    // byte by index. We cast the byte to `int` to print its
    // numeric value, as Go does.
    std::string hello = "hello";
    std::println("Len:  {}", hello.size());
    std::println("Char: {}", static_cast<int>(hello[1]));
}

// Note that `size` and indexing above work at the byte level.
// `std::string` holds raw bytes, which with UTF-8 text is
// often useful as-is. If you're working with potentially
// multi-byte characters you'll want to use encoding-aware
// operations.
