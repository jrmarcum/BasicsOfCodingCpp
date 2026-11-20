// C++ has no JSON support in its standard library, so this
// lesson builds a minimal one: a `Json` value type, an encoder
// (`dump`) and a small recursive-descent decoder (`parse`).
// That is enough to show the same ideas as Go's `encoding/json`.
#include <cctype>
#include <format>
#include <map>
#include <print>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

// A JSON value is one of: null, bool, number, string, array
// or object. `std::variant` models that directly. Objects use
// `std::map`, so keys come out sorted, as in Go.
struct Json {
    using Array = std::vector<Json>;
    using Object = std::map<std::string, Json>;
    std::variant<std::nullptr_t, bool, double, std::string, Array, Object> v = nullptr;

    Json() = default;
    Json(bool b) : v(b) {}
    Json(int n) : v(static_cast<double>(n)) {}
    Json(double d) : v(d) {}
    Json(const char* s) : v(std::string(s)) {}
    Json(std::string s) : v(std::move(s)) {}
    Json(Array a) : v(std::move(a)) {}
    Json(Object o) : v(std::move(o)) {}
};

// Encoding: the equivalent of `json.Marshal`.
std::string dump(const Json& j) {
    struct Visitor {
        std::string operator()(std::nullptr_t) const { return "null"; }
        std::string operator()(bool b) const { return b ? "true" : "false"; }
        std::string operator()(double d) const { return std::format("{}", d); }
        std::string operator()(const std::string& s) const {
            std::string out = "\"";
            for (char c : s) {
                if (c == '"' || c == '\\') out += '\\';
                out += c;
            }
            return out + "\"";
        }
        std::string operator()(const Json::Array& a) const {
            std::string out = "[";
            for (std::size_t i = 0; i < a.size(); i++) {
                if (i) out += ',';
                out += dump(a[i]);
            }
            return out + "]";
        }
        std::string operator()(const Json::Object& o) const {
            std::string out = "{";
            for (bool first = true; const auto& [k, val] : o) {
                if (!first) out += ',';
                out += (*this)(k) + ":" + dump(val);
                first = false;
            }
            return out + "}";
        }
    };
    return std::visit(Visitor{}, j.v);
}

// Decoding: the equivalent of `json.Unmarshal`. Malformed
// input throws, which callers can treat like Go's returned error.
class Parser {
public:
    explicit Parser(const std::string& s) : s_(s) {}
    Json parse() {
        Json j = value();
        ws();
        if (i_ != s_.size()) fail("trailing data");
        return j;
    }

private:
    const std::string& s_;
    std::size_t i_ = 0;

    [[noreturn]] void fail(const char* msg) { throw std::runtime_error(std::format("json: {} at {}", msg, i_)); }
    void ws() { while (i_ < s_.size() && std::isspace(static_cast<unsigned char>(s_[i_]))) i_++; }
    void expect(char c) { ws(); if (i_ >= s_.size() || s_[i_] != c) fail("unexpected character"); i_++; }
    bool take(char c) { ws(); if (i_ < s_.size() && s_[i_] == c) { i_++; return true; } return false; }

    std::string str() {
        expect('"');
        std::string out;
        while (i_ < s_.size() && s_[i_] != '"') {
            if (s_[i_] == '\\' && i_ + 1 < s_.size()) i_++;
            out += s_[i_++];
        }
        expect('"');
        return out;
    }

    Json value() {
        ws();
        if (i_ >= s_.size()) fail("unexpected end");
        char c = s_[i_];
        if (c == '{') {
            i_++;
            Json::Object o;
            if (!take('}')) {
                do {
                    std::string k = str();
                    expect(':');
                    o[k] = value();
                } while (take(','));
                expect('}');
            }
            return o;
        }
        if (c == '[') {
            i_++;
            Json::Array a;
            if (!take(']')) {
                do { a.push_back(value()); } while (take(','));
                expect(']');
            }
            return a;
        }
        if (c == '"') return str();
        if (s_.compare(i_, 4, "true") == 0) { i_ += 4; return true; }
        if (s_.compare(i_, 5, "false") == 0) { i_ += 5; return false; }
        if (s_.compare(i_, 4, "null") == 0) { i_ += 4; return Json(); }
        std::size_t used = 0;
        double d = std::stod(s_.substr(i_), &used);
        i_ += used;
        return d;
    }
};

Json parse(const std::string& text) { return Parser(text).parse(); }

// Go prints decoded values as `map[k:v]` / `[a b]`; this helper
// reproduces that layout.
std::string go_fmt(const Json& j) {
    struct Visitor {
        std::string operator()(std::nullptr_t) const { return "<nil>"; }
        std::string operator()(bool b) const { return b ? "true" : "false"; }
        std::string operator()(double d) const { return std::format("{}", d); }
        std::string operator()(const std::string& s) const { return s; }
        std::string operator()(const Json::Array& a) const {
            std::string out = "[";
            for (std::size_t i = 0; i < a.size(); i++) out += (i ? " " : "") + go_fmt(a[i]);
            return out + "]";
        }
        std::string operator()(const Json::Object& o) const {
            std::string out = "map[";
            for (bool first = true; const auto& [k, val] : o) {
                out += (first ? "" : " ") + k + ":" + go_fmt(val);
                first = false;
            }
            return out + "]";
        }
    };
    return std::visit(Visitor{}, j.v);
}

// We'll use these two structs to demonstrate encoding and
// decoding of custom types below. C++ has no field tags or
// reflection, so each struct gets explicit conversion code.
// `response1` uses the field names as keys...
struct response1 {
    int Page;
    std::vector<std::string> Fruits;
};

// ...while `response2` customizes them (Go: `json:"page"`).
struct response2 {
    int page;
    std::vector<std::string> fruits;
};

Json::Array to_array(const std::vector<std::string>& v) {
    return Json::Array(v.begin(), v.end());
}

// Keep the struct fields in Go's declared order when printing
// (`std::map` would sort the keys alphabetically).
std::string marshal(const response1& r) {
    return std::format("{{\"Page\":{},\"Fruits\":{}}}", r.Page, dump(to_array(r.Fruits)));
}
std::string marshal(const response2& r) {
    return std::format("{{\"page\":{},\"fruits\":{}}}", r.page, dump(to_array(r.fruits)));
}

int main() {
    // First we'll look at encoding basic data types to
    // JSON strings. Here are some examples for atomic
    // values.
    std::println("{}", dump(true));
    std::println("{}", dump(1));
    std::println("{}", dump(2.34));
    std::println("{}", dump("vector"));

    // And here are some for vectors and maps, which encode
    // to JSON arrays and objects as you'd expect.
    std::println("{}", dump(Json::Array{"apple", "peach", "pear"}));
    std::println("{}", dump(Json::Object{{"apple", 5}, {"lettuce", 7}}));

    // Custom data types are converted by our own code. Go
    // includes only exported fields and uses their names as
    // the JSON keys.
    response1 res1D{1, {"apple", "peach", "pear"}};
    std::println("{}", marshal(res1D));

    // Go uses tags on struct fields to customize the key
    // names; here `response2` simply spells the keys in lower case.
    response2 res2D{1, {"apple", "peach", "pear"}};
    std::println("{}", marshal(res2D));

    // Now let's look at decoding JSON data into values.
    // Here's an example for a generic data structure.
    std::string byt = R"({"num":6.13,"strs":["a","b"]})";

    // `parse` returns a `Json` holding the decoded data. Errors
    // are reported by exception, so we check for them here.
    Json dat;
    try {
        dat = parse(byt);
    } catch (const std::exception& e) {
        std::println(stderr, "panic: {}", e.what());
        return 2;
    }
    std::println("{}", go_fmt(dat));

    // In order to use the values in the decoded object,
    // we'll need to extract them as their appropriate type.
    // For example here we get the value in `num` as the
    // expected `double`.
    const auto& obj = std::get<Json::Object>(dat.v);
    double num = std::get<double>(obj.at("num").v);
    std::println("{}", num);

    // Accessing nested data requires a series of
    // extractions.
    const auto& strs = std::get<Json::Array>(obj.at("strs").v);
    std::string str1 = std::get<std::string>(strs[0].v);
    std::println("{}", str1);

    // We can also decode JSON into custom data types.
    // This adds type-safety to our programs and removes the
    // need for variant access when using the decoded data.
    std::string str = R"({"page": 1, "fruits": ["apple", "peach"]})";
    response2 res{};
    {
        Json doc = parse(str);
        const auto& o = std::get<Json::Object>(doc.v);
        res.page = static_cast<int>(std::get<double>(o.at("page").v));
        for (const auto& f : std::get<Json::Array>(o.at("fruits").v)) {
            res.fruits.push_back(std::get<std::string>(f.v));
        }
    }
    std::println("{{{} {}}}", res.page, go_fmt(Json(to_array(res.fruits))));
    std::println("{}", res.fruits[0]);

    // In the examples above we always used strings as
    // intermediates between the data and JSON representation.
    // We can also stream JSON encodings directly to any
    // output, like standard out (Go's `json.NewEncoder`
    // appends a newline, which `println` gives us).
    std::println("{}", dump(Json::Object{{"apple", 5}, {"lettuce", 7}}));
}
