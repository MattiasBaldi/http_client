## Ressources
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
**Official Documentation:**
- [Walkthrough text](https://wiki.openssl.org/index.php/SSL/TLS_Client)
- [Simpler guiding](wiki.openssl.org)
- [OpenSSL Manual Pages](https://www.openssl.org/docs/man1.1.1/) - comprehensive reference
- [SSL_new() man page](https://www.openssl.org/docs/man1.1.1/man3/SSL_new.html) - create SSL connection
- [SSL_connect() man page](https://www.openssl.org/docs/man1.1.1/man3/SSL_connect.html) - perform handshake
- [SSL_read() / SSL_write() man page](https://www.openssl.org/docs/man1.1.1/man3/SSL_read.html) - encrypted I/O
- [SSL_CTX_new() man page](https://www.openssl.org/docs/man1.1.1/man3/SSL_CTX_new.html) - SSL context setup
- [ERR_get_error() man page](https://www.openssl.org/docs/man1.1.1/man3/ERR_get_error.html) - error handling

**Tutorials & Examples:**
- [up to data](https://github.com/openssl/openssl/tree/master/demos/sslecho)
- [OpenSSL Wiki - Simple TLS Client](https://wiki.openssl.org/index.php/Simple_TLS_Client) - step-by-step guide
- [OpenSSL GitHub - Demos](https://github.com/openssl/openssl/tree/master/demos) - official code examples
- [Linux Man Pages - SSL_connect(3ssl)](https://man7.org/linux/man-pages/man3/SSL_connect.3ssl.html) - detailed parameter guide
- [Informit - Simplest TLS Client](https://www.informit.com/articles/article.aspx?p=130881) - beginner-friendly walkthrough

**Key Functions to Learn:**
- SSL_CTX_new() - initialize SSL context
- TLS_client_method() - choose TLS protocol version
- SSL_CTX_set_default_verify_paths() - load certificates
- SSL_new() - create SSL connection object
- SSL_set_fd() - attach socket to SSL connection
- SSL_connect() - perform TLS handshake
- SSL_write() / SSL_read() - encrypted send/receive
- SSL_free() / SSL_CTX_free() - cleanup

### Request / Response Format
- [format](https://datatracker.ietf.org/doc/html/rfc7230#section-3.1)
- [more readable](https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Overview)

### Socket & DNS (POSIX/BSD)
- [getaddrinfo() - man page](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html)
- [socket() - man page](https://man7.org/linux/man-pages/man2/socket.2.html)
- [connect() - man page](https://man7.org/linux/man-pages/man2/connect.2.html)
- [POSIX Sockets API](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)

## System & Flow
1. ![Connection flow](./flow/connection.svg) 

2. ![Connection flow](./flow/request.svg) 

3. ![Connection flow](./flow/response.svg) 

/* OSI - Layer 7 - Application */
// URL Parsing
// DNS Resolution 
/* Request + Response */
// Request Formation
// Response Handling

/* OSI - Layer 4 - Transport */
/* Data + Transport */
// TCP Connection 
// Data Transmission
// Data Reception 