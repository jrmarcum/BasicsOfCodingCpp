C++ has no networking in the standard library, so this lesson uses raw sockets: WinSock2 on Windows (link with `-lws2_32`) and BSD sockets on POSIX, selected with `#ifdef _WIN32`. A small RAII `Socket` wrapper plays the role of Go's `defer conn.Close()`, and each connection is handled by a detached `std::jthread` (Go's goroutine).
___
#### Run the TCP server in the background, then send data and capture the response using netcat.
___
##### Run Command:

`$ zig run -lc++ -lws2_32 -cflags -std=c++23 -- tcp-server.cpp &` (Windows)

`$ zig run -lc++ -cflags -std=c++23 -- tcp-server.cpp &` (Unix)

`$ echo "Hello from netcat" | nc localhost 8090`

On Windows without netcat, use a PowerShell TCP client (the server must still be running; stop it afterwards with `Stop-Process` or Ctrl+C):

`PS> $c=New-Object Net.Sockets.TcpClient("localhost",8090); $s=$c.GetStream(); $b=[Text.Encoding]::ASCII.GetBytes("Hello from netcat`n"); $s.Write($b,0,$b.Length); (New-Object IO.StreamReader($s)).ReadLine(); $c.Close()`

##### Results:

`ACK: HELLO FROM NETCAT`
