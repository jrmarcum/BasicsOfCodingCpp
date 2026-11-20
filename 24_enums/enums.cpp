// _Enumerated types_ (enums) are a special case of
// sum types. An enum is a type that has a fixed number of possible
// values, each with a distinct name. C++ has a real enum
// feature: `enum class`, a scoped, strongly typed enumeration.
#include <format>
#include <print>
#include <stdexcept>
#include <string>

// Our enum type `ServerState` has an underlying `int` type.
// Enumerators get successive values automatically (the
// equivalent of Go's `iota`): 0, 1, 2 and so on.
enum class ServerState : int {
    StateIdle,
    StateConnected,
    StateError,
    StateRetrying,
};

// The equivalent of implementing `fmt.Stringer`: a
// `to_string` function so values can be printed out or
// converted to strings.
std::string to_string(ServerState ss) {
    switch (ss) {
    case ServerState::StateIdle:      return "idle";
    case ServerState::StateConnected: return "connected";
    case ServerState::StateError:     return "error";
    case ServerState::StateRetrying:  return "retrying";
    }
    return "";
}

// transition emulates a state transition for a
// server; it takes the existing state and returns
// a new state.
ServerState transition(ServerState s) {
    switch (s) {
    case ServerState::StateIdle:
        return ServerState::StateConnected;
    case ServerState::StateConnected:
    case ServerState::StateRetrying:
        return ServerState::StateIdle;
    case ServerState::StateError:
        return ServerState::StateError;
    default:
        // Go's `panic` becomes a thrown exception.
        throw std::logic_error("unknown state: " + to_string(s));
    }
}

int main() {
    auto ns = transition(ServerState::StateIdle);
    std::println("{}", to_string(ns));

    auto ns2 = transition(ns);
    std::println("{}", to_string(ns2));
}
