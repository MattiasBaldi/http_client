# HTTP Client Learning Notes

## URL Parsing - String Functions

**Available C string functions for URL parsing:**

- `strdup(str)` - duplicates entire string, returns new pointer
- `strstr(haystack, needle)` - finds substring, returns pointer or NULL
- `strchr(str, char)` - finds first occurrence of character, returns pointer or NULL
- `strlen(str)` - returns length of string
- `strncpy(dest, src, n)` - copies n characters from src to dest
- `strcmp(str1, str2)` - compares two strings, returns 0 if equal
- `atoi(str)` - converts string to integer
- `malloc(size)` - allocates memory, returns pointer
- `free(ptr)` - frees allocated memory

Used in `parse_url()` to extract protocol, host, port, and path from URLs.

---

## Socket File Descriptors

**What is a file descriptor?**

A "file descriptor" is just an integer that the operating system uses to refer to an open file, socket, pipe, or other I/O resource. When you call `socket()`, it returns an int that acts as a handle to that socket.

In our code:
- `sockfd = -1` means "no socket yet"
- `sockfd >= 0` means a valid socket is open
- Always check the return value to know if the socket creation succeeded

---

## DNS Resolution with getaddrinfo

The `struct addrinfo` structure comes from `<netdb.h>` and is used to store DNS lookup results.

**Reference:** [getaddrinfo man page](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html)

**Error handling:**
- `gai_strerror()` converts DNS error codes to human-readable error strings
- Use it when `getaddrinfo()` returns non-zero

---

## printf Family of Functions

Three related functions for formatted output:

- **`printf()`** - prints to screen/stdout
- **`fprintf()`** - prints to a file stream
- **`snprintf()`** - "string number printf" - formats a string into a memory buffer (safe, with size limit)

**snprintf is crucial** because it prevents buffer overflow:
```c
int req_len = snprintf(buffer, sizeof(buffer), format, args);
// Always check: req_len >= 0 && req_len < sizeof(buffer)
```

---

## TLS/SSL Handshake Process

The TLS (Transport Layer Security) handshake establishes encrypted communication with HTTPS servers.

**Reference:** [OpenSSL TLS Client Example](https://github.com/openssl/openssl/blob/master/demos/sslecho/main.c)

**Four-step process:**

1. **Initialize SSL context**
   - `TLS_client_method()` - auto-negotiates TLS 1.2/1.3
   - `SSL_CTX_new()` - creates the context
   - `SSL_CTX_set_default_verify_paths()` - loads system's default CA certificates for verifying server certificates

2. **Create SSL connection object**
   - `SSL_new(ssl_ctx)` - creates SSL object from context
   - `SSL_set_fd(ssl, sockfd)` - attaches socket to SSL connection

3. **Perform TLS handshake**
   - `SSL_connect(ssl)` - performs the actual handshake
   - Check return value: `1 = success`, `<= 0 = error`

4. **Verify server certificate (security check)**
   - `SSL_get_verify_result(ssl)` - validates the certificate chain
   - Check: `verify_result == X509_V_OK` means valid

**After successful handshake:** All data sent through the SSL object is automatically encrypted/decrypted. Replace `send()` with `SSL_write()` and `recv()` with `SSL_read()`.

---

## recv() - Reading Socket Data

`recv()` is a blocking socket function that reads incoming data from a connected socket.

**What it does:**
- Waits for data to arrive from the server (BLOCKING - doesn't return until data is available or connection closes)
- Reads up to N bytes from the socket into a buffer
- Returns: number of bytes received, `-1` on error, or `0` when connection closes

**Signature:**
```c
ssize_t recv(int sockfd, void *buf, size_t len, int flags)
```

**Parameters:**
- `sockfd` - the socket file descriptor (our connected socket)
- `buf` - pointer to buffer where received data goes
- `len` - maximum number of bytes to read into the buffer
- `flags` - options (0 for default behavior)

**How it fits in request-response flow:**
1. Send HTTP request using `send()` (done)
2. Call `recv()` to get server's response (HTTP status, headers, body)
3. Loop `recv()` because responses can be larger than buffer, arrive in chunks
4. When `recv()` returns `0`, connection is closed - you have the complete response
5. When `recv()` returns `-1`, an error occurred

**For HTTPS:** Use `SSL_read()` instead (same logic, but automatically decrypts)

---

## SSL vs HTTP Communication

**Plain HTTP (unencrypted):**
```c
send(sockfd, request, req_len, 0)
recv(sockfd, buffer, sizeof(buffer) - 1, 0)
```

**HTTPS (encrypted):**
```c
SSL_write(ssl, request, req_len)
SSL_read(ssl, buffer, sizeof(buffer) - 1)
```

Same logic, but data is encrypted over the wire and automatically decrypted by the SSL layer.

---

# C Headers Explained

## What is a Header File?

A header file (`.h`) declares what functions and types exist in your program without showing how they work. It's like a contract between files.

**Analogy:** If your program is a library, the header is the catalog that tells you what books are available. The `.c` file is the actual books.

## Why Headers?

When `test_main.c` needs to call `parse_url()` from `main.c`, the compiler needs to know:
1. Does `parse_url()` exist?
2. What arguments does it take?
3. What does it return?

The header file answers all these questions **before linking**.

## Declaration vs Definition

**Declaration** (in header):
```c
parsed_url parse_url(char *url);  // "This function exists"
```

**Definition** (in source file):
```c
parsed_url parse_url(char *url) {  // "Here's how it actually works"
  // implementation
}
```

## Header Guards

Prevents a header from being included multiple times in the same compilation:

```c
#ifndef MAIN_H
#define MAIN_H

// Content here

#endif
```

Without guards, if header is included twice, you get "duplicate definition" errors.

## What Goes in Headers

**DO include:**
- Type definitions (structs, enums)
- Function declarations
- Constants (`#define`)
- Macro definitions

**DON'T include:**
- Function implementations (goes in `.c`)
- Large blocks of code
- Global variable definitions (use `extern` for declarations)

## Compilation Flow

```
test_main.c includes main.h
        ↓
Preprocessor expands: replaces #include with header contents
        ↓
Compiler sees function declarations from header
        ↓
Compiler trusts those functions exist, creates object file
        ↓
Linker finds actual function definitions in main.o
        ↓
Links them together into executable
```

## Common Pattern

**main.h:**
```c
#ifndef MAIN_H
#define MAIN_H

typedef struct { ... } MyType;
MyType do_something(char *input);

#endif
```

**main.c:**
```c
#include "main.h"

MyType do_something(char *input) {
  // implementation
}
```

**test.c:**
```c
#include "main.h"  // Now I know do_something exists

void test_something() {
  MyType result = do_something("test");
  // Test assertions
}
```

## Conditional Compilation

Headers can use preprocessor directives:

```c
#ifdef DEBUG
  void debug_print(char *msg);
#endif
```

Or in Makefile: `-DDEBUG` adds `DEBUG` define.

## Key Takeaway

Headers are the **interface** to your code. They tell other files what's available without exposing implementation details.
