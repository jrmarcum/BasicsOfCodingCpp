// [_SHA1 hashes_](http://en.wikipedia.org/wiki/SHA-1) are
// frequently used to compute short identities for binary
// or text blobs. For example, the [git revision control
// system](http://git-scm.com/) uses SHA1s extensively to
// identify versioned files and directories. The C++ standard
// library has no hash functions, so SHA1 is implemented here
// from scratch.
#include <array>
#include <bit>
#include <cstdint>
#include <format>
#include <print>
#include <string>
#include <string_view>

// A small SHA1 class following Go's pattern: create a hash,
// `write` bytes into it, then `sum` to get the digest.
class Sha1 {
public:
    // `write` expects bytes; a `std::string_view` of the text
    // is fine.
    void write(std::string_view data) {
        for (unsigned char c : data) {
            push(c);
            ++totalLen_;
        }
    }

    // `sum` finalizes and returns the 20-byte digest.
    std::array<uint8_t, 20> sum() {
        // Padding: a 1 bit, zeros, then the 64-bit big-endian
        // message length in bits.
        uint64_t bitLen = totalLen_ * 8;
        push(0x80);
        while (blockLen_ != 56) push(0x00);
        for (int i = 7; i >= 0; --i) push(static_cast<uint8_t>(bitLen >> (i * 8)));

        std::array<uint8_t, 20> out{};
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 4; ++j)
                out[i * 4 + j] = static_cast<uint8_t>(h_[i] >> (24 - 8 * j));
        return out;
    }

private:
    // Add one byte to the current block, hashing it when full.
    void push(uint8_t b) {
        block_[blockLen_++] = b;
        if (blockLen_ == 64) {
            process_block();
            blockLen_ = 0;
        }
    }

    void process_block() {
        std::array<uint32_t, 80> w{};
        for (int i = 0; i < 16; ++i) {
            w[i] = (uint32_t(block_[i * 4]) << 24) | (uint32_t(block_[i * 4 + 1]) << 16) |
                   (uint32_t(block_[i * 4 + 2]) << 8) | uint32_t(block_[i * 4 + 3]);
        }
        for (int i = 16; i < 80; ++i) {
            w[i] = std::rotl(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        }
        uint32_t a = h_[0], b = h_[1], c = h_[2], d = h_[3], e = h_[4];
        for (int i = 0; i < 80; ++i) {
            uint32_t f, k;
            if (i < 20) {
                f = (b & c) | (~b & d);
                k = 0x5A827999;
            } else if (i < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (i < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            uint32_t temp = std::rotl(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = std::rotl(b, 30);
            b = a;
            a = temp;
        }
        h_[0] += a;
        h_[1] += b;
        h_[2] += c;
        h_[3] += d;
        h_[4] += e;
    }

    std::array<uint32_t, 5> h_{0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
    std::array<uint8_t, 64> block_{};
    size_t blockLen_ = 0;
    uint64_t totalLen_ = 0;
};

// SHA1 values are often printed in hex, for example in git
// commits. `std::format` with `{:02x}` converts each byte.
std::string to_hex(const std::array<uint8_t, 20>& bs) {
    std::string out;
    for (uint8_t b : bs) out += std::format("{:02x}", b);
    return out;
}

int main() {
    std::string s = "sha1 this string";

    // The pattern for generating a hash is to create a
    // `Sha1`, `write(bytes)`, then call `sum()`.
    // Here we start with a new hash.
    Sha1 h;

    // `write` accepts the string directly.
    h.write(s);

    // This gets the finalized hash result as a 20-byte array.
    auto bs = h.sum();

    std::println("{}", s);
    std::println("{}", to_hex(bs));
}
