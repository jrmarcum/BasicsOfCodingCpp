The C++ standard library has no networking, so this lesson uses raw TCP sockets (WinSock2 on Windows, BSD sockets elsewhere) and writes the HTTP/1.1 `GET` request by hand. Go's example fetches `https://gobyexample.com`, but that site now redirects plain HTTP to HTTPS and HTTPS needs a TLS library (such as OpenSSL), which is outside the standard library. So the program defaults to `example.com`, which still answers on plain HTTP; the response status and the first 5 lines of the body are printed as in Go. A different host can be passed as an argument (after the second `--`); `gobyexample.com` prints `Response status: 301 Moved Permanently` and the redirect page.
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -lws2_32 -cflags -std=c++23 -- http-client.cpp` (Windows)

`$ zig run -lc++ -cflags -std=c++23 -- http-client.cpp` (Unix)

##### Results:

`Response status: 200 OK`
`<!doctype html><html lang=en><head><meta charset=utf-8><link rel=icon href=data:,><meta name=viewport content="width=device-width,initial-scale=1"><title>Example Domain</title><style>html{color-scheme:light dark;background:light-dark(#eee,#222)}body{font:16px/1.6 system-ui,sans-serif;max-width:26em;margin:auto;padding:25vh 2em 2em;text-align:center}</style></head><body><p>This domain is for use in documentation examples without needing permission. This is not a service; avoid relying on it for testing and monitoring purposes.</p><script src=/s.js></script></body></html>`

The example.com page is a single long line, so only one body line appears.
