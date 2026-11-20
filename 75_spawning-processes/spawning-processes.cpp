// Sometimes our C++ programs need to spawn other
// processes. The portable tools are `std::system` and
// `popen` (`_popen` on Windows), which run a command line
// through the shell and give us a pipe to its output
// (or input).
#include <cstdio>
#include <cstdlib>
#include <print>
#include <string>
#include <utility>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define DEVNULL "2>nul"
#else
#include <sys/wait.h>
#define DEVNULL "2>/dev/null"
#endif

// Convert the value returned by `pclose` / `std::system`
// into the command's exit code. POSIX packs it into a wait
// status; Windows returns the exit code directly.
int exit_code(int status) {
#ifdef _WIN32
    return status;
#else
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
#endif
}

// Like Go's `Command.Output`: run the command, wait for it to
// finish and collect its standard output and exit code.
std::pair<std::string, int> output(const std::string& cmd) {
    std::string out;
    FILE* p = popen(cmd.c_str(), "r");
    if (!p) return {out, -1};
    char buf[256];
    while (size_t n = std::fread(buf, 1, sizeof buf, p)) out.append(buf, n);
    return {out, exit_code(pclose(p))};
}

int main() {
    // We'll start with a simple command that takes no
    // arguments or input and just prints something to
    // stdout. `date` on Unix; `date /t` run through `cmd` on Windows
    // (plain `date` would wait for input).
#ifdef _WIN32
    auto [dateOut, rc] = output("date /t");
#else
    auto [dateOut, rc] = output("date");
#endif
    // `output` collected the stdout of the finished command.
    std::println("> date");
    std::println("{}", dateOut);

    // A command that runs but exits with a non-zero return
    // code reports it through its exit status. (The shell
    // reports a command that could not be found as a non-zero
    // status as well: 127 on Unix, 1 on Windows.)
#ifdef _WIN32
    auto [ignored, rc2] = output("cmd /c exit 1");
#else
    auto [ignored, rc2] = output("date -x " DEVNULL);
#endif
    if (rc2 != 0) std::println("command exit rc = {}", rc2);

    // Next we'll look at a slightly more involved case
    // where we pipe data to the external process on its
    // `stdin`. `popen` with mode "w" gives us a pipe into
    // the child's stdin; the child's stdout is inherited, so
    // we flush ours first to keep the output in order.
    std::println("> grep hello");
    std::fflush(stdout);
#ifdef _WIN32
    FILE* grepIn = popen("findstr hello", "w");
#else
    FILE* grepIn = popen("grep hello", "w");
#endif
    if (grepIn) {
        std::fputs("hello grep\ngoodbye grep", grepIn);
        pclose(grepIn);  // closes stdin and waits for the process
    }
    std::println();

    // Note that `popen` takes one command-line string, which the
    // shell splits into a command and arguments. Unlike Go we
    // don't need `bash -c` to use a full command line, since the
    // shell is always involved. `std::system` is the simplest
    // way to run a command whose output we just want shown.
#ifdef _WIN32
    const char* ls = "dir /b";
    std::println("> dir /b");
#else
    const char* ls = "ls -a -l -h";
    std::println("> ls -a -l -h");
#endif
    std::fflush(stdout);
    std::system(ls);
}
