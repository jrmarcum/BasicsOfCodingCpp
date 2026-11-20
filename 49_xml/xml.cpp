// C++ has no XML support in its standard library, so this
// lesson hand-writes a tiny encoder and decoder that handle
// just the shapes used here. Go's struct tags (`xml:"id,attr"`,
// `xml:"parent>child>plant"`) become explicit code.
#include <format>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

// Plant will be mapped to XML. The element name is `plant`,
// `id` becomes an XML _attribute_ rather than a nested
// element, and each `origin` becomes its own child element.
struct Plant {
    int id = 0;
    std::string name;
    std::vector<std::string> origin;
};

// Equivalent of Go's `Plant.String()`.
std::string to_string(const Plant& p) {
    std::string o;
    for (std::size_t i = 0; i < p.origin.size(); i++) o += (i ? " " : "") + p.origin[i];
    return std::format("Plant id={}, name={}, origin=[{}]", p.id, p.name, o);
}

// Escape the characters that are special in XML text.
std::string escape(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '&': out += "&amp;"; break;
            default: out += c;
        }
    }
    return out;
}

// Emit XML for a plant. `pad` is the full indentation of the
// `<plant>` line: Go's `MarshalIndent(v, " ", "  ")` uses a
// one-space prefix plus two spaces per nesting level.
std::string marshal(const Plant& p, const std::string& pad, const std::string& step = "  ") {
    std::string out = std::format("{}<plant id=\"{}\">\n", pad, p.id);
    out += std::format("{}{}<name>{}</name>\n", pad, step, escape(p.name));
    for (const auto& o : p.origin) {
        out += std::format("{}{}<origin>{}</origin>\n", pad, step, escape(o));
    }
    return out + pad + "</plant>";
}

// Emit a `<nesting>` element that wraps all plants in
// `<parent><child>...` (Go: `xml:"parent>child>plant"`).
std::string marshal_nesting(const std::vector<Plant>& plants) {
    std::string out = " <nesting>\n   <parent>\n     <child>\n";
    for (const auto& p : plants) out += marshal(p, "       ") + "\n";
    return out + "     </child>\n   </parent>\n </nesting>";
}

// A very small decoder: find `<tag>` ... `</tag>` starting at
// `pos`, returning the text between them (and advancing `pos`).
bool next_element(const std::string& xml, const std::string& tag, std::size_t& pos, std::string& text) {
    auto open = xml.find("<" + tag + ">", pos);
    if (open == std::string::npos) return false;
    open += tag.size() + 2;
    auto close = xml.find("</" + tag + ">", open);
    if (close == std::string::npos) throw std::runtime_error("XML syntax error: unexpected EOF");
    text = xml.substr(open, close - open);
    pos = close + tag.size() + 3;
    return true;
}

// Equivalent of `xml.Unmarshal`: parse a `<plant>` document.
// Malformed input throws, like Go's returned error.
Plant unmarshal(const std::string& xml) {
    Plant p;
    auto start = xml.find("<plant id=\"");
    if (start == std::string::npos) throw std::runtime_error("expected element type <plant>");
    start += 11;
    p.id = std::stoi(xml.substr(start, xml.find('"', start) - start));
    std::size_t pos = 0;
    next_element(xml, "name", pos, p.name);
    for (std::string text; next_element(xml, "origin", pos, text);) p.origin.push_back(text);
    return p;
}

int main() {
    Plant coffee{27, "Coffee", {"Ethiopia", "Brazil"}};

    // Emit XML representing our plant, indented to be more
    // human-readable. (Go's `MarshalIndent` prefix is one space.)
    std::string out = marshal(coffee, " ");
    std::println("{}", out);

    // To add a generic XML header to the output, append
    // it explicitly.
    const std::string header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    std::println("{}", header + out);

    // Use our decoder to parse the XML text into a data
    // structure. If the XML is malformed, a descriptive
    // exception is thrown (Go returns an error).
    Plant p;
    try {
        p = unmarshal(out);
    } catch (const std::exception& e) {
        std::println(stderr, "panic: {}", e.what());
        return 2;
    }
    std::println("{}", to_string(p));

    Plant tomato{81, "Tomato", {"Mexico", "California"}};

    // The encoder nests all `plant`s under `<parent><child>...`
    std::println("{}", marshal_nesting({coffee, tomato}));
}
