// Range-based `for` iterates over elements in a variety of
// data structures. Let's see how to use it with some of the
// data structures we've already learned.
#include <cstddef>
#include <map>
#include <print>
#include <string>
#include <vector>

int main() {
    // Here we use a range-for to sum the numbers in a
    // vector. Arrays work like this too.
    std::vector<int> nums = {2, 3, 4};
    int sum = 0;
    for (int num : nums) {
        sum += num;
    }
    std::println("sum: {}", sum);

    // A range-for gives only the values. When we want the
    // indexes too, use a classic indexed loop (C++23 also
    // offers `std::views::enumerate`).
    for (std::size_t i = 0; i < nums.size(); i++) {
        if (nums[i] == 3) {
            std::println("index: {}", i);
        }
    }

    // A range-for over a map iterates over key/value pairs,
    // which structured bindings split apart. `std::map`
    // iterates in sorted key order.
    std::map<std::string, std::string> kvs = {{"a", "apple"}, {"b", "banana"}};
    for (const auto& [k, v] : kvs) {
        std::println("{} -> {}", k, v);
    }

    // To iterate over just the keys of a map, use only the
    // `first` member of each pair.
    for (const auto& kv : kvs) {
        std::println("key: {}", kv.first);
    }

    // Iterating over a string visits bytes (UTF-8 code units).
    // The first value is the byte index and the second the
    // character; for ASCII text this matches Go's runes.
    std::string go = "go";
    for (std::size_t i = 0; i < go.size(); i++) {
        std::println("{} {}", i, static_cast<int>(go[i]));
    }
}
