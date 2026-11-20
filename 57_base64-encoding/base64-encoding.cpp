// C++ has no built-in support for [base64
// encoding/decoding](http://en.wikipedia.org/wiki/Base64), so
// both the standard and URL-compatible variants are
// implemented here from scratch.
#include <cstdint>
#include <print>
#include <string>
#include <string_view>

// The two alphabets differ only in their last two characters:
// standard uses `+` and `/`, URL-safe uses `-` and `_`.
constexpr std::string_view kStdAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
constexpr std::string_view kUrlAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

// Encode bytes three at a time into four characters, padding
// with `=` at the end.
std::string encode(std::string_view data, std::string_view alphabet) {
    std::string out;
    size_t i = 0;
    for (; i + 2 < data.size(); i += 3) {
        uint32_t n = (uint8_t(data[i]) << 16) | (uint8_t(data[i + 1]) << 8) | uint8_t(data[i + 2]);
        out += alphabet[(n >> 18) & 63];
        out += alphabet[(n >> 12) & 63];
        out += alphabet[(n >> 6) & 63];
        out += alphabet[n & 63];
    }
    if (size_t rem = data.size() - i; rem == 1) {
        uint32_t n = uint8_t(data[i]) << 16;
        out += alphabet[(n >> 18) & 63];
        out += alphabet[(n >> 12) & 63];
        out += "==";
    } else if (rem == 2) {
        uint32_t n = (uint8_t(data[i]) << 16) | (uint8_t(data[i + 1]) << 8);
        out += alphabet[(n >> 18) & 63];
        out += alphabet[(n >> 12) & 63];
        out += alphabet[(n >> 6) & 63];
        out += '=';
    }
    return out;
}

// Decoding may fail, which you can check if you don't already
// know the input to be well-formed. Returns false on error.
bool decode(std::string_view text, std::string_view alphabet, std::string& out) {
    out.clear();
    uint32_t acc = 0;
    int bits = 0;
    size_t pad = 0;
    for (char c : text) {
        if (c == '=') {
            ++pad;
            continue;
        }
        auto pos = alphabet.find(c);
        if (pos == std::string_view::npos || pad > 0) return false;
        acc = (acc << 6) | static_cast<uint32_t>(pos);
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            out += static_cast<char>((acc >> bits) & 0xFF);
        }
    }
    return text.size() % 4 == 0;
}

int main() {
    // Here's the `std::string` we'll encode/decode.
    std::string data = "abc123!?$*&()'-=@~";

    // We support both standard and URL-compatible base64.
    // Here's how to encode using the standard encoder.
    std::string sEnc = encode(data, kStdAlphabet);
    std::println("{}", sEnc);

    // Decode and print the result.
    std::string sDec;
    decode(sEnc, kStdAlphabet, sDec);
    std::println("{}", sDec);
    std::println();

    // This encodes/decodes using a URL-compatible base64
    // format.
    std::string uEnc = encode(data, kUrlAlphabet);
    std::println("{}", uEnc);
    std::string uDec;
    decode(uEnc, kUrlAlphabet, uDec);
    std::println("{}", uDec);
}
