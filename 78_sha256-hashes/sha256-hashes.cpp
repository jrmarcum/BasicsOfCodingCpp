// SHA256 hashes are frequently used to compute short
// identities for binary or text blobs. For example,
// TLS/SSL certificates use SHA256 to compute a
// certificate's signature. The C++ standard library has no
// hash functions, so here is a compact implementation of
// FIPS 180-4 SHA-256 written from scratch.
#include <array>
#include <bit>
#include <cstdint>
#include <format>
#include <print>
#include <string>
#include <string_view>

// A small hasher type, mirroring Go's `sha256.New()`:
// `write` feeds bytes in, `sum` returns the finalized hash.
class Sha256 {
public:
    // `write` expects bytes; a `std::string_view` converts
    // naturally from a string.
    void write(std::string_view data) {
        for (unsigned char c : data) {
            block_[used_++] = c;
            if (used_ == 64) {
                compress();
                used_ = 0;
            }
            total_ += 8;
        }
    }

    // `sum` pads the message (0x80, zeros, 64-bit bit length),
    // and returns the 32-byte digest.
    std::array<uint8_t, 32> sum() {
        uint64_t bits = total_;
        block_[used_++] = 0x80;
        if (used_ > 56) {
            while (used_ < 64) block_[used_++] = 0;
            compress();
            used_ = 0;
        }
        while (used_ < 56) block_[used_++] = 0;
        for (int i = 7; i >= 0; i--) block_[used_++] = uint8_t(bits >> (i * 8));
        compress();

        std::array<uint8_t, 32> out{};
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++)
                out[i * 4 + j] = uint8_t(h_[i] >> (24 - 8 * j));
        return out;
    }

private:
    static constexpr std::array<uint32_t, 64> K = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

    void compress() {
        uint32_t w[64];
        for (int i = 0; i < 16; i++)
            w[i] = uint32_t(block_[i * 4]) << 24 | uint32_t(block_[i * 4 + 1]) << 16 |
                   uint32_t(block_[i * 4 + 2]) << 8 | uint32_t(block_[i * 4 + 3]);
        for (int i = 16; i < 64; i++) {
            uint32_t s0 = std::rotr(w[i - 15], 7) ^ std::rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = std::rotr(w[i - 2], 17) ^ std::rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }
        auto [a, b, c, d, e, f, g, h] = h_;
        for (int i = 0; i < 64; i++) {
            uint32_t S1 = std::rotr(e, 6) ^ std::rotr(e, 11) ^ std::rotr(e, 25);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t t1 = h + S1 + ch + K[i] + w[i];
            uint32_t S0 = std::rotr(a, 2) ^ std::rotr(a, 13) ^ std::rotr(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t t2 = S0 + maj;
            h = g; g = f; f = e; e = d + t1;
            d = c; c = b; b = a; a = t1 + t2;
        }
        h_[0] += a; h_[1] += b; h_[2] += c; h_[3] += d;
        h_[4] += e; h_[5] += f; h_[6] += g; h_[7] += h;
    }

    std::array<uint32_t, 8> h_ = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                                  0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    std::array<uint8_t, 64> block_{};
    size_t used_ = 0;
    uint64_t total_ = 0;  // message length in bits
};

// Format a digest as lowercase hex, like Go's `%x`.
std::string hex(const std::array<uint8_t, 32>& bs) {
    std::string out;
    for (uint8_t b : bs) out += std::format("{:02x}", b);
    return out;
}

int main() {
    std::string s = "sha256 this string";

    // Here we start with a new hash.
    Sha256 h;

    // `write` expects bytes (here a string view of `s`).
    h.write(s);

    // This gets the finalized hash result.
    auto bs = h.sum();

    std::println("{}", s);
    std::println("{}", hex(bs));
}
