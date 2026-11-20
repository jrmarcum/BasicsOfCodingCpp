// Sometimes we just want to completely replace the current
// C++ process with another (perhaps non-C++) one. To do this
// we'll use the classic `exec` family of functions:
// `execvp` on POSIX and `_execvp` on Windows.
#include <cstdio>
#include <print>
#include <string>
#include <vector>

#ifdef _WIN32
#include <process.h>
#define exec_vp _execvp
#else
#include <unistd.h>
#define exec_vp execvp
#endif

int main() {
    // `execvp` searches `PATH` for the binary (like Go's
    // `exec.LookPath`), so we only need its name. We'll exec
    // `ls` on Unix; on Windows `ls` is not a program, so we
    // run the shell built-in `dir` through `cmd /c`.
#ifdef _WIN32
    std::vector<std::string> args = {"cmd", "/c", "dir"};
#else
    // `exec` requires arguments in array form (as opposed to
    // one big string). Note that the first argument should
    // be the program name.
    std::vector<std::string> args = {"ls", "-a", "-l", "-h"};
#endif

    // Build the NULL-terminated `char*` array that `exec` wants.
    std::vector<char*> argv;
    for (auto& a : args) argv.push_back(a.data());
    argv.push_back(nullptr);

    // Flush anything buffered: after a successful exec our
    // buffers are gone.
    std::fflush(stdout);

    // Here's the actual `exec` call. The new process inherits our
    // environment. If this call is successful, the execution of our
    // process will end here and be replaced by `ls -a -l -h`.
    // If there is an error we'll get -1 back.
    exec_vp(argv[0], argv.data());

    // Only reached on failure.
    std::perror("exec failed");
    return 1;
}
