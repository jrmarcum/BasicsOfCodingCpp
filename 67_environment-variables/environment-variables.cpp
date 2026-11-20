// Environment variables are a universal mechanism for
// conveying configuration information to Unix programs.
// Let's look at how to set, get, and list environment variables.
#include <cstdlib>
#include <print>
#include <string>
#include <string_view>

// `environ` is the C runtime's array of `KEY=value` strings.
// POSIX declares it as a global; on Windows `<cstdlib>`
// already provides `_environ`.
#ifndef _WIN32
extern char** environ;
#endif

// `getenv` returns a null pointer when the key isn't present.
// Like Go's `os.Getenv`, this helper returns an empty string
// instead.
std::string get_env(const char* key) {
    const char* v = std::getenv(key);
    return v ? v : "";
}

// `setenv` is POSIX; Windows spells it `_putenv_s`.
void set_env(const char* key, const char* value) {
#ifdef _WIN32
    _putenv_s(key, value);
#else
    setenv(key, value, 1);
#endif
}

int main() {
    // To set a key/value pair, use `set_env`. To get a
    // value for a key, use `get_env`. This will return
    // an empty string if the key isn't present in the
    // environment.
    set_env("FOO", "1");
    std::println("FOO: {}", get_env("FOO"));
    std::println("BAR: {}", get_env("BAR"));

    // Walk the environment block to list all key/value
    // pairs. Each entry is in the form `KEY=value`. We
    // split at the first `=` to get the key. Here we print
    // all the keys.
    std::println();
#ifdef _WIN32
    char** env = _environ;
#else
    char** env = environ;
#endif
    for (; *env; ++env) {
        std::string_view e = *env;
        // (On Windows, entries like `=C:=C:\dir` start with `=`,
        // so search for the separator after the first character.)
        auto eq = e.find('=', 1);
        std::println("{}", e.substr(0, eq));
    }
}
