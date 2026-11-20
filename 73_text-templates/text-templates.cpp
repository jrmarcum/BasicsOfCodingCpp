// Go offers built-in support for creating dynamic content
// or showing customized output to the user with the
// `text/template` package. The C++ standard library has no
// template engine, so this lesson contains a compact
// hand-written one that supports the actions used by the
// Go example: `{{.}}`, `{{.Field}}`, `{{if}}...{{else}}...{{end}}`
// and `{{range}}...{{end}}`, plus `-` whitespace trimming.
#include <algorithm>
#include <cctype>
#include <map>
#include <print>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

// The data passed to a template: a plain value (string),
// a list of values, or a map of fields. A Go struct or map
// is represented here by `Fields`.
using List = std::vector<std::string>;
using Fields = std::map<std::string, std::string>;
using Data = std::variant<std::string, List, Fields>;

// A value is considered false if it's the default value of
// a type, such as an empty string, empty list or empty map.
bool truthy(const Data& d) {
    return std::visit([](const auto& v) { return !v.empty(); }, d);
}

// Render a value the way Go prints it: lists as `[a b]`.
std::string show(const Data& d) {
    if (auto* s = std::get_if<std::string>(&d)) return *s;
    std::string out = "[";
    if (auto* l = std::get_if<List>(&d)) {
        for (bool first = true; const auto& e : *l) {
            if (!first) out += ' ';
            out += e;
            first = false;
        }
    }
    return out + "]";
}

// A parsed template is a list of tokens: literal text or
// an action (the text between `{{` and `}}`).
struct Token {
    bool action;
    std::string text;
};

class Template {
public:
    // Parse the template body from a string. Throws on error,
    // like `template.Must` panicking.
    Template(std::string name, const std::string& body) : name_(std::move(name)) {
        constexpr auto ws = " \t\r\n";
        size_t pos = 0;
        bool trim_next = false;  // previous action ended with ` -}}`
        while (pos < body.size()) {
            size_t open = body.find("{{", pos);
            std::string lit = body.substr(pos, open == std::string::npos ? open : open - pos);
            if (trim_next) lit.erase(0, std::min(lit.find_first_not_of(ws), lit.size()));
            if (open == std::string::npos) {
                if (!lit.empty()) tokens_.push_back({false, lit});
                break;
            }
            size_t close = body.find("}}", open);
            if (close == std::string::npos) throw std::runtime_error("template: " + name_ + ": unclosed action");
            std::string act = body.substr(open + 2, close - open - 2);
            // `{{- ` trims whitespace before the action.
            if (act.starts_with("- ")) {
                act.erase(0, 2);
                while (!lit.empty() && std::isspace((unsigned char)lit.back())) lit.pop_back();
            }
            // ` -}}` trims whitespace after the action.
            trim_next = act.ends_with(" -");
            if (trim_next) act.resize(act.size() - 2);
            if (!lit.empty()) tokens_.push_back({false, lit});
            tokens_.push_back({true, act});
            pos = close + 2;
        }
    }

    // "Executing" the template generates its text with
    // specific values for its actions.
    std::string execute(const Data& data) const {
        std::string out;
        run(0, tokens_.size(), data, out);
        return out;
    }

private:
    // Index of the `end` (or the `else` at depth 1, when
    // wanted) matching the block opened just before `from`.
    size_t find_block(size_t from, size_t to, bool want_else) const {
        int depth = 1;
        for (size_t i = from; i < to; i++) {
            if (!tokens_[i].action) continue;
            const auto& a = tokens_[i].text;
            if (a.starts_with("if ") || a.starts_with("range ")) depth++;
            else if (a == "end" && --depth == 0) return i;
            else if (want_else && a == "else" && depth == 1) return i;
        }
        throw std::runtime_error("template: " + name_ + ": missing {{end}}");
    }

    void run(size_t begin, size_t end, const Data& dot, std::string& out) const {
        for (size_t i = begin; i < end; i++) {
            const Token& t = tokens_[i];
            if (!t.action) { out += t.text; continue; }
            const std::string& a = t.text;
            if (a == ".") {
                out += show(dot);
            } else if (a.starts_with(".")) {
                // `{{.Name}}` accesses a field of a struct or map.
                auto* f = std::get_if<Fields>(&dot);
                if (!f) throw std::runtime_error("template: " + name_ + ": can't evaluate field " + a);
                auto it = f->find(a.substr(1));
                out += it == f->end() ? "<no value>" : it->second;
            } else if (a.starts_with("if ")) {
                // if/else provides conditional execution.
                size_t mid = find_block(i + 1, end, true);
                size_t fin = tokens_[mid].text == "else" ? find_block(mid + 1, end, false) : mid;
                if (truthy(dot)) run(i + 1, mid, dot, out);
                else if (mid != fin) run(mid + 1, fin, dot, out);
                i = fin;
            } else if (a.starts_with("range ")) {
                // range loops through a list; inside the block
                // `{{.}}` is the current item.
                size_t fin = find_block(i + 1, end, false);
                if (auto* l = std::get_if<List>(&dot))
                    for (const auto& item : *l) run(i + 1, fin, Data(item), out);
                i = fin;
            } else {
                throw std::runtime_error("template: " + name_ + ": unknown action " + a);
            }
        }
    }

    std::string name_;
    std::vector<Token> tokens_;
};

int main() {
    // We can create a new template and parse its body from
    // a string. Templates are a mix of static text and
    // "actions" enclosed in `{{...}}` that are used to
    // dynamically insert content.
    Template t1("t1", "Value: {{.}}\n");

    // By "executing" the template we generate its text with
    // specific values for its actions. The `{{.}}` action is
    // replaced by the value passed as a parameter to `execute`.
    std::print("{}", t1.execute(std::string("some text")));
    std::print("{}", t1.execute(std::to_string(5)));
    std::print("{}", t1.execute(List{"Go", "Rust", "C++", "C#"}));

    // If the data is a struct (here a `Fields` map) we can use the
    // `{{.FieldName}}` action to access its fields.
    Template t2("t2", "Name: {{.Name}}\n");
    std::print("{}", t2.execute(Fields{{"Name", "Jane Doe"}}));

    // The same applies to maps; with maps there is no restriction on the
    // case of key names.
    std::print("{}", t2.execute(Fields{{"Name", "Mickey Mouse"}}));

    // if/else provide conditional execution for templates. A value is considered
    // false if it's the default value of a type, such as 0, an empty string,
    // an empty list, etc.
    // This sample demonstrates another feature of templates: using `-` in
    // actions to trim whitespace.
    Template t3("t3", "{{if . -}} yes {{else -}} no {{end}}\n");
    std::print("{}", t3.execute(std::string("not empty")));
    std::print("{}", t3.execute(std::string("")));

    // range blocks let us loop through lists. Inside
    // the range block `{{.}}` is set to the current item of the iteration.
    Template t4("t4", "Range: {{range .}}{{.}} {{end}}\n");
    std::print("{}", t4.execute(List{"Go", "Rust", "C++", "C#"}));
}
