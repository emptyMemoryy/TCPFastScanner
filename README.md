# TCPFastScanner :(

A fast TCP port scanner written in C using **non-blocking sockets** + **select()**.

This tool performs a TCP connect scan and classifies ports as:
- OPEN
- CLOSED
- FILTERED (timeout)

It also supports optional **banner grabbing** and basic **service detection**.

---

## Features

- Non-blocking TCP connect scan
- `select()` based multiplexing (multiple ports simultaneously)
- Timeout-based filtered detection
- Port range scanning
- Batch/window scanning (rate-limiting)
- Optional banner grabbing (`-b`)
- Basic service detection (SSH/HTTP/FTP/HTTPS hint)
- Text output and JSON output mode

---

## How it Works (High Level)

The scanner uses this workflow:

1. Create multiple sockets (one per port)
2. Set sockets to non-blocking mode
3. Call `connect()` on all ports (does not block)
4. Use `select()` to monitor which sockets become writable
5. Use `getsockopt(SO_ERROR)` to confirm if connection succeeded
6. Classify the port:
   - `err == 0` → OPEN
   - `err != 0` → CLOSED
   - timeout reached → FILTERED
7. If OPEN and `-b` enabled, reconnect and attempt to grab banner

---

## Build

### Requirements
- Linux (recommended)
- GCC

### Compile
gcc -Wall -Wextra -O2 main.c scanner.c banner.c srvc.c out.c  -o scanner 
