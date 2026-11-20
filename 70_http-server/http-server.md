The C++ standard library has no networking, so this lesson opens a listening socket with the operating system's socket API (WinSock2 on Windows, BSD sockets elsewhere) and implements a small slice of HTTP/1.1 by hand: a `std::map` of routes to handler functions plays the role of Go's default router, and each connection is served on its own thread. Like Go's server, the `/headers` handler does not list `Host`, and the order of the headers follows the request (Go's map iteration order is random).
___
#### Run the server in the background, then access the `/hello` route with curl.
___
##### Run Command:

`$ zig run -lc++ -lws2_32 -cflags -std=c++23 -- http-server.cpp &` (Windows)

`$ zig run -lc++ -cflags -std=c++23 -- http-server.cpp &` (Unix)

`$ curl localhost:8090/hello`

##### Results:

`hello`
___
#### The `/headers` route echoes the request headers.
___
##### Run Command:

`$ curl localhost:8090/headers`

##### Results:

`User-Agent: curl/8.19.0`
`Accept: */*`
___
#### Any other path gets a 404.
___
##### Run Command:

`$ curl localhost:8090/nope`

##### Results:

`404 page not found`
___
#### The server runs until it is stopped. Press Ctrl+C in its terminal, or kill the background job.
