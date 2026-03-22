# http_client

A simple curl-inspired HTTP/HTTPS client written in C using POSIX sockets and OpenSSL. Handles DNS resolution, TCP connections, TLS handshakes, custom headers, request bodies, and HTTP proxy support with CONNECT tunneling for HTTPS.

> **Note:** This is an educational project built to learn networking and HTTP internals. Not intended for production use — use at your own risk.

## Usage

```bash
./main [options] <url>
```

| Flag | Description | Example |
|------|-------------|---------|
| `-X` | HTTP method (default: GET) | `-X POST` |
| `-H` | Custom header (repeatable) | `-H "Content-Type: application/json"` |
| `-d` | Request body | `-d '{"key": "value"}'` |
| `-x` | HTTP proxy | `-x http://127.0.0.1:8118` |

```bash
# Simple GET
./main https://example.com

# POST with body and headers
./main -X POST -H "Content-Type: application/json" -d '{"key": "value"}' https://httpbin.org/post

# Request through proxy
./main -x http://127.0.0.1:8118 https://ifconfig.me/ip
```

## Flow

| Socket Connection | HTTP(S) Request | HTTP(S) Response |
|:-:|:-:|:-:|
| ![Connection](./flow/connection.svg) | ![Request](./flow/request.svg) | ![Response](./flow/response.svg) |

# Resources
- [walkthrough text](https://medium.com/@trish07/simple-steps-to-build-an-http-client-in-c-8e225b5c718c)
- [walkthrough video](https://www.youtube.com/watch?v=Nw1mrIshK00)

### Http
- [http protocol](https://www.rfc-editor.org/rfc/rfc2616)
- [mozilla http guide](https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Overview)
- [http versions](https://www.youtube.com/watch?v=UMwQjFzTQXw)

### TLS & Handshake
- [simplified](https://toolkit.whysonil.dev/how-it-works/https/)
- [tls](https://www.youtube.com/watch?v=0TLDTodL7Lc)
- [tls handshake](https://www.youtube.com/watch?v=86cQJ0MMses)
- [tls handshake 2](https://www.youtube.com/watch?v=cuR05y_2Gxc)

### OpenSSL & TLS Implementation
- [Walkthrough - Up to Date](https://github.com/openssl/openssl/tree/master/demos/sslecho)
- [OpenSSL GitHub](https://github.com/openssl/openssl/tree/master/demos) - official code examples
- [Walkthrough - Deprecated](https://wiki.openssl.org/index.php/Simple_TLS_Client)
- [OpenSSL Manual Pages](https://www.openssl.org/docs/man1.1.1/) - comprehensive reference

### Request / Response Format
- [format](https://datatracker.ietf.org/doc/html/rfc7230#section-3.1)
- [more readable](https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Overview)

### Socket & DNS (POSIX/BSD)
- [getaddrinfo() - man page](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html)
- [socket() - man page](https://man7.org/linux/man-pages/man2/socket.2.html)
- [connect() - man page](https://man7.org/linux/man-pages/man2/connect.2.html)
- [POSIX Sockets API](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

### HTTP Proxy & CONNECT Tunneling
- [HTTP CONNECT method (RFC 7231)](https://datatracker.ietf.org/doc/html/rfc7231#section-4.3.6) - official spec for CONNECT tunneling
- [HTTP tunneling (MDN)](https://developer.mozilla.org/en-US/docs/Web/HTTP/Proxy_servers_and_tunneling) - proxy and tunneling overview
- [CONNECT method (MDN)](https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Methods/CONNECT) - CONNECT method reference
- [How HTTPS proxies work](https://en.wikipedia.org/wiki/HTTP_tunnel) - explains CONNECT tunnel concept