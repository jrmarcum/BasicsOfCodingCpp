// C++ offers built-in support for regular expressions in
// the `<regex>` header. Here are some examples of common
// regexp-related tasks in C++ (using the default ECMAScript
// grammar).
#include <algorithm>
#include <cctype>
#include <print>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

// Go prints slices as `[a b c]`, recursing into nested ones.
template <typename T>
std::string cpp_fmt(const T& v) {
    if constexpr (std::ranges::range<T> && !std::is_convertible_v<T, std::string_view>) {
        std::string out = "[";
        for (bool first = true; const auto& e : v) {
            if (!first) out += ' ';
            out += cpp_fmt(e);
            first = false;
        }
        return out + "]";
    } else {
        return std::format("{}", v);
    }
}

// Go's `FindString` returns the first match text.
std::string FindString(const std::regex& r, const std::string& s) {
    std::smatch m;
    return std::regex_search(s, m, r) ? m.str() : "";
}

// Go's `FindStringIndex` returns [start end] of the first match.
std::vector<int> FindStringIndex(const std::regex& r, const std::string& s) {
    std::smatch m;
    if (!std::regex_search(s, m, r)) return {};
    return {static_cast<int>(m.position(0)), static_cast<int>(m.position(0) + m.length(0))};
}

// Go's `FindStringSubmatch` returns the match and its groups.
std::vector<std::string> FindStringSubmatch(const std::regex& r, const std::string& s) {
    std::smatch m;
    std::vector<std::string> out;
    if (std::regex_search(s, m, r)) {
        for (const auto& g : m) out.push_back(g.str());
    }
    return out;
}

// Indexes of a match and all of its groups: [s0 e0 s1 e1 ...].
std::vector<int> SubmatchIndex(const std::smatch& m) {
    std::vector<int> out;
    for (std::size_t i = 0; i < m.size(); i++) {
        out.push_back(static_cast<int>(m.position(i)));
        out.push_back(static_cast<int>(m.position(i) + m.length(i)));
    }
    return out;
}

std::vector<int> FindStringSubmatchIndex(const std::regex& r, const std::string& s) {
    std::smatch m;
    return std::regex_search(s, m, r) ? SubmatchIndex(m) : std::vector<int>{};
}

// The `All` variants apply to every match; `n` limits the number
// of matches when non-negative.
std::vector<std::string> FindAllString(const std::regex& r, const std::string& s, int n) {
    std::vector<std::string> out;
    for (std::sregex_iterator it(s.begin(), s.end(), r), end; it != end; ++it) {
        if (n >= 0 && static_cast<int>(out.size()) >= n) break;
        out.push_back(it->str());
    }
    return out;
}

std::vector<std::vector<int>> FindAllStringSubmatchIndex(const std::regex& r, const std::string& s, int n) {
    std::vector<std::vector<int>> out;
    for (std::sregex_iterator it(s.begin(), s.end(), r), end; it != end; ++it) {
        if (n >= 0 && static_cast<int>(out.size()) >= n) break;
        out.push_back(SubmatchIndex(*it));
    }
    return out;
}

// Go's `ReplaceAllFunc`: transform each match with a function.
template <typename F>
std::string ReplaceAllFunc(const std::regex& r, const std::string& s, F f) {
    std::string out;
    auto last = s.cbegin();
    for (std::sregex_iterator it(s.begin(), s.end(), r), end; it != end; ++it) {
        out.append(last, (*it)[0].first);
        out += f(it->str());
        last = (*it)[0].second;
    }
    out.append(last, s.cend());
    return out;
}

int main() {
    const std::string pattern = "p([a-z]+)ch";

    // This tests whether a pattern matches a string.
    // `regex_search` finds the pattern anywhere in the input.
    bool match = std::regex_search("peach", std::regex(pattern));
    std::println("{}", match);

    // Above we used a pattern directly, but for
    // other regexp tasks you'll want to build a `std::regex`
    // object once and reuse it.
    std::regex r(pattern);

    // Many functions accept these objects. Here's
    // a match test like we saw earlier.
    std::println("{}", std::regex_search("peach", r));

    // This finds the match for the regexp.
    std::println("{}", FindString(r, "peach punch"));

    // This also finds the first match but returns the
    // start and end indexes for the match instead of the
    // matching text.
    std::println("{}", cpp_fmt(FindStringIndex(r, "peach punch")));

    // The submatch variants include information about
    // both the whole-pattern matches and the submatches
    // within those matches. For example this will return
    // information for both `p([a-z]+)ch` and `([a-z]+)`.
    std::println("{}", cpp_fmt(FindStringSubmatch(r, "peach punch")));

    // Similarly this will return information about the
    // indexes of matches and submatches.
    std::println("{}", cpp_fmt(FindStringSubmatchIndex(r, "peach punch")));

    // The `All` variants of these functions apply to all
    // matches in the input, not just the first. For
    // example to find all matches for a regexp.
    std::println("{}", cpp_fmt(FindAllString(r, "peach punch pinch", -1)));

    // These `All` variants are available for the other
    // functions we saw above as well.
    std::println("{}", cpp_fmt(FindAllStringSubmatchIndex(r, "peach punch pinch", -1)));

    // Providing a non-negative integer as the last
    // argument to these functions will limit the number
    // of matches.
    std::println("{}", cpp_fmt(FindAllString(r, "peach punch pinch", 2)));

    // C++ matches directly on any character range (here a
    // `std::vector<char>`), so there's no separate byte-slice API.
    std::vector<char> bytes = {'p', 'e', 'a', 'c', 'h'};
    std::println("{}", std::regex_search(bytes.begin(), bytes.end(), r));

    // The pattern text we compiled from is still ours to print.
    // (An invalid pattern throws `std::regex_error`, which is the
    // C++ analogue of Go's `MustCompile` panicking.)
    std::println("{}", pattern);

    // `std::regex_replace` replaces matches with other values.
    std::println("{}", std::regex_replace("a peach", r, "<fruit>"));

    // Our `ReplaceAllFunc` helper lets you transform matched
    // text with a given function.
    std::string out = ReplaceAllFunc(r, "a peach", [](std::string m) {
        std::ranges::transform(m, m.begin(), [](unsigned char c) { return std::toupper(c); });
        return m;
    });
    std::println("{}", out);
}
