// The C++ `<random>` header provides
// [pseudorandom number](http://en.wikipedia.org/wiki/Pseudorandom_number_generator)
// generation: an engine (here `std::mt19937`) produces raw
// bits and a distribution shapes them into a range.
#include <chrono>
#include <print>
#include <random>

int main() {
    // `std::random_device` supplies a non-deterministic seed.
    // Unlike Go's default global generator, a seeded engine
    // is explicit in C++.
    std::mt19937 rng{std::random_device{}()};

    // For example, `uniform_int_distribution<int>(0, 99)`
    // returns a random `int` n, `0 <= n < 100`.
    std::uniform_int_distribution<int> intn(0, 99);
    std::print("{},", intn(rng));
    std::print("{}", intn(rng));
    std::println();

    // `uniform_real_distribution<double>` returns a `double`
    // `f`, `0.0 <= f < 1.0`.
    std::uniform_real_distribution<double> float64(0.0, 1.0);
    std::println("{}", float64(rng));

    // This can be used to generate random floats in
    // other ranges, for example `5.0 <= f' < 10.0`.
    std::print("{},", (float64(rng) * 5) + 5);
    std::print("{}", (float64(rng) * 5) + 5);
    std::println();

    // An engine seeded with the same number always produces
    // the same sequence. To produce varying sequences, give
    // it a seed that changes, such as the current time. Note
    // that this is not safe to use for random numbers you
    // intend to be secret; use a cryptographic source for those.
    auto seed = static_cast<std::mt19937::result_type>(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 r1{seed};

    // Call the distribution with the new engine just like
    // with the first one.
    std::print("{},", intn(r1));
    std::print("{}", intn(r1));
    std::println();

    // If you seed an engine with the same number, it
    // produces the same sequence of random numbers.
    std::mt19937 r2{42};
    std::print("{},", intn(r2));
    std::print("{}", intn(r2));
    std::println();
    std::mt19937 r3{42};
    std::print("{},", intn(r3));
    std::print("{}", intn(r3));
    std::println();
}
