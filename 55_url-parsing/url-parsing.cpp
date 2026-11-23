// URLs provide a [uniform way to locate resources](https://adam.herokuapp.com/past/2010/3/30/urls_are_the_uniform_way_to_locate_resources/).
// Here's how to parse URLs in C++. The standard library has no URL
// type, so we write a small parser over `std::string_view`.
#include <map>
#include <optional>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// The pieces of a URL, like Go's `url.URL`.
struct Url {
    std::string scheme;
    std::optional<std::string> username;
    std::optional<std::string> password;
    std::string host;  // hostname and port, if present
    std::string path;
    std::string raw_query;
    std::string fragment;
};

// Split `s` at the first occurrence of `sep`. If `sep` is not
// found, the second half is empty and `found` is false.
static std::pair<std::string_view, std::string_view> cut(std::string_view s, char sep, bool& found) {
    auto pos = s.find(sep);
    found = pos != std::string_view::npos;
    if (!found) return {s, {}};
    return {s.substr(0, pos), s.substr(pos + 1)};
}

// Parse a URL of the form
// `scheme://user:pass@host:port/path?query#fragment`.
// Throws `std::invalid_argument` if there is no scheme.
Url parse_url(std::string_view s) {
    Url u;
    bool found = false;

    // The fragment follows the first `#`.
    auto [rest, frag] = cut(s, '#', found);
    u.fragment = frag;

    // The query follows the first `?`.
    auto [rest2, query] = cut(rest, '?', found);
    u.raw_query = query;

    // The scheme precedes `://`.
    auto sep = rest2.find("://");
    if (sep == std::string_view::npos) {
        throw std::invalid_argument("missing protocol scheme");
    }
    u.scheme = rest2.substr(0, sep);
    std::string_view after = rest2.substr(sep + 3);

    // The authority runs up to the first `/`; the rest is the path.
    auto slash = after.find('/');
    std::string_view authority = after.substr(0, slash);
    if (slash != std::string_view::npos) u.path = after.substr(slash);

    // Authentication info precedes the last `@` in the authority.
    auto at = authority.rfind('@');
    if (at != std::string_view::npos) {
        std::string_view userinfo = authority.substr(0, at);
        authority = authority.substr(at + 1);
        auto [name, pass] = cut(userinfo, ':', found);
        u.username = std::string(name);
        if (found) u.password = std::string(pass);
    }
    u.host = authority;
    return u;
}

// `split_host_port` extracts the hostname and port from a
// `host:port` string (like Go's `net.SplitHostPort`).
std::pair<std::string, std::string> split_host_port(std::string_view hostport) {
    auto colon = hostport.rfind(':');
    if (colon == std::string_view::npos) {
        throw std::invalid_argument("missing port in address");
    }
    return {std::string(hostport.substr(0, colon)), std::string(hostport.substr(colon + 1))};
}

// Parse `k=v&k2=v2` into a map from strings to vectors of
// strings (a key may repeat).
std::map<std::string, std::vector<std::string>> parse_query(std::string_view q) {
    std::map<std::string, std::vector<std::string>> m;
    while (!q.empty()) {
        auto amp = q.find('&');
        std::string_view pair = q.substr(0, amp);
        q = amp == std::string_view::npos ? std::string_view{} : q.substr(amp + 1);
        if (pair.empty()) continue;
        bool found = false;
        auto [k, v] = cut(pair, '=', found);
        m[std::string(k)].emplace_back(v);
    }
    return m;
}

// Go prints a `url.Values` map as `map[k:[v]]`.
std::string cpp_fmt(const std::map<std::string, std::vector<std::string>>& m) {
    std::string out = "map[";
    for (bool firstKey = true; const auto& [k, vs] : m) {
        if (!firstKey) out += ' ';
        firstKey = false;
        out += k + ":[";
        for (bool first = true; const auto& v : vs) {
            if (!first) out += ' ';
            first = false;
            out += v;
        }
        out += ']';
    }
    return out + "]";
}

int main() {
    // We'll parse this example URL, which includes a
    // scheme, authentication info, host, port, path,
    // query params, and query fragment.
    std::string s = "postgres://user:pass@host.com:5432/path?k=v#f";

    // Parse the URL and ensure there are no errors.
    Url u;
    try {
        u = parse_url(s);
    } catch (const std::exception& e) {
        std::println("error: {}", e.what());
        return 1;
    }

    // Accessing the scheme is straightforward.
    std::println("{}", u.scheme);

    // The user info contains all authentication info; print
    // `user:pass`, then the username and password individually.
    std::println("{}:{}", u.username.value_or(""), u.password.value_or(""));
    std::println("{}", u.username.value_or(""));
    std::println("{}", u.password.value_or(""));

    // The `host` contains both the hostname and the port,
    // if present. Use `split_host_port` to extract them.
    std::println("{}", u.host);
    auto [host, port] = split_host_port(u.host);
    std::println("{}", host);
    std::println("{}", port);

    // Here we extract the `path` and the fragment after
    // the `#`.
    std::println("{}", u.path);
    std::println("{}", u.fragment);

    // To get query params in a string of `k=v` format,
    // use `raw_query`. You can also parse query params
    // into a map. The parsed query param maps are from
    // strings to vectors of strings, so index into `[0]`
    // if you only want the first value.
    std::println("{}", u.raw_query);
    auto m = parse_query(u.raw_query);
    std::println("{}", cpp_fmt(m));
    std::println("{}", m["k"][0]);
}
