Go's `context.Context` is modelled with `std::stop_source` / `std::stop_token`. Each request gets its own stop source; a watcher `std::jthread` triggers it when the client disconnects, and the handler waits with `std::condition_variable_any::wait_for(lock, token, 10s, pred)`, which returns early as soon as the token is stopped. The server uses raw sockets (WinSock2 on Windows, BSD sockets elsewhere) and serves one thread per connection.
___
#### Run the server in the background, then simulate a client request to `/hello`. Cancel the request shortly after (Ctrl+C in the curl terminal, or `curl -m 2` to give up after 2 seconds) to signal cancellation.
___
##### Run Command:

`$ zig run -lc++ -lws2_32 -cflags -std=c++23 -- context.cpp &` (Windows)

`$ zig run -lc++ -cflags -std=c++23 -- context.cpp &` (Unix)

`$ curl -m 2 localhost:8090/hello`

##### Results:

`server: hello handler started`
`server: context canceled`
`server: hello handler ended`
___
#### If the client waits the full 10 seconds, the handler completes normally.
___
##### Run Command:

`$ curl localhost:8090/hello`

##### Results:

`hello`

(server log)

`server: hello handler started`
`server: hello handler ended`
___
#### The server runs until it is stopped. Press Ctrl+C in its terminal, or kill the background job.
