// A C++ `std::string` is a sequence of bytes. The language
// and the standard library do not decode it: the bytes are
// simply UTF-8 encoded text if the source says so.
// In other languages, strings are made of "characters".
// In Go, the concept of a character is called a `rune` - it's
// an integer that represents a Unicode code point. The C++
// equivalent is `char32_t`, and we decode UTF-8 into it by hand.
#include <cstdint>
#include <print>
#include <string>
#include <string_view>
#include <utility>

// Decodes one UTF-8 sequence at the start of `s`, returning the
// code point and the number of bytes it used (like Go's
// `utf8.DecodeRuneInString`). Invalid input yields U+FFFD, width 1.
std::pair<char32_t, int> decodeRune(std::string_view s) {
    auto b = [&](size_t i) { return static_cast<unsigned char>(s[i]); };
    if (s.empty()) return {U'�', 0};
    unsigned char c = b(0);
    int n = 1;
    char32_t cp = c;
    if (c >= 0xF0 && c < 0xF8) { n = 4; cp = c & 0x07; }
    else if (c >= 0xE0) { n = 3; cp = c & 0x0F; }
    else if (c >= 0xC0) { n = 2; cp = c & 0x1F; }
    else if (c >= 0x80) return {U'�', 1};
    if (n > 1 && s.size() < static_cast<size_t>(n)) return {U'�', 1};
    for (int k = 1; k < n; k++) {
        if ((b(k) & 0xC0) != 0x80) return {U'�', 1};
        cp = (cp << 6) | (b(k) & 0x3F);
    }
    return {cp, n};
}

// Counts the runes in a string (like `utf8.RuneCountInString`).
// It has to decode each UTF-8 sequence one after another.
int runeCount(std::string_view s) {
    int count = 0;
    for (size_t i = 0; i < s.size();) {
        i += decodeRune(s.substr(i)).second;
        count++;
    }
    return count;
}

// Encodes a code point back to UTF-8 so it can be printed.
std::string encodeRune(char32_t cp) {
    std::string out;
    if (cp < 0x80) {
        out += static_cast<char>(cp);
    } else if (cp < 0x800) {
        out += static_cast<char>(0xC0 | (cp >> 6));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        out += static_cast<char>(0xE0 | (cp >> 12));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | (cp >> 18));
        out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }
    return out;
}

// Reproduces Go's `%#U` verb, e.g. `U+0E2A 'ส'`.
std::string fmtU(char32_t r) {
    return std::format("U+{:04X} '{}'", static_cast<uint32_t>(r), encodeRune(r));
}

void examineRune(char32_t r) {
    // Values like `U't'` are _rune literals_ (`char32_t`). We
    // can compare a rune value to a rune literal directly.
    if (r == U't') {
        std::println("found tee");
    } else if (r == U'ส') {
        std::println("found so sua");
    }
}

int main() {
    // `s` is a string assigned a literal value
    // representing the word "hello" in the Thai
    // language. The literal is UTF-8 encoded text.
    const std::string s = "สวัสดี";

    // A string is a sequence of bytes, so this produces
    // the length of the raw bytes stored within.
    std::println("Len: {}", s.size());

    // Indexing into a string produces the raw byte values at
    // each index. This loop generates the hex values of all
    // the bytes that constitute the code points in `s`.
    for (size_t i = 0; i < s.size(); i++) {
        std::print("{:x} ", static_cast<unsigned char>(s[i]));
    }
    std::println("");

    // To count how many _runes_ are in a string, we use
    // our own `runeCount`. Note that its run-time depends
    // on the size of the string, because it has to decode
    // each UTF-8 rune sequentially. Some Thai characters are
    // represented by UTF-8 code points that can span
    // multiple bytes, so the result of this count may be
    // surprising.
    std::println("Rune count: {}", runeCount(s));

    // Iterate over the string decoding each rune along with
    // its byte offset in the string.
    for (size_t idx = 0; idx < s.size();) {
        auto [runeValue, width] = decodeRune(std::string_view(s).substr(idx));
        std::println("{} starts at {}", fmtU(runeValue), idx);
        idx += width;
    }

    // The same iteration again, this time handing each
    // decoded rune to another function.
    std::println("\nUsing DecodeRuneInString");
    for (size_t i = 0, w = 0; i < s.size(); i += w) {
        auto [runeValue, width] = decodeRune(std::string_view(s).substr(i));
        std::println("{} starts at {}", fmtU(runeValue), i);
        w = width;

        // This demonstrates passing a rune value to a function.
        examineRune(runeValue);
    }
}
