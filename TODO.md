# HTTP Client TODO

## Phase 1: Core HTTP Client (Hardcoded headers & empty body)

### URL Parsing
- [ ] Parse command-line arguments (method, url)
- [ ] Extract hostname from URL
- [ ] Extract port from URL (default 80 for http, 443 for https)
- [ ] Extract path from URL (default to "/" if not provided)
- [ ] Determine scheme (http vs https)

### Connection Establishment
- [ ] DNS lookup (hostname → IP address)
- [ ] Create TCP socket
- [ ] Connect socket to IP:port
- [ ] TLS handshake (if HTTPS)

### Request Formatting
- [ ] Format request line: `METHOD /path HTTP/1.1\r\n`
- [ ] Build required headers:
  - [ ] Host header (from URL)
  - [ ] Content-Length: 0 (empty body)
  - [ ] User-Agent (optional)
- [ ] Send formatted request over socket

### Response Parsing
- [ ] Receive response bytes from socket
- [ ] Parse status line (extract status code)
- [ ] Parse headers
- [ ] Extract response body
- [ ] Print/return result

### Testing
- [ ] Test with simple HTTP request (http://example.com)
- [ ] Test with HTTPS request (https://example.com)
- [ ] Test with different paths
- [ ] Verify status codes and response bodies

---

## Phase 2: Extended Features (Later)

### Command-line flags
- [ ] Add `-X` flag for method
- [ ] Add `-H` flag for custom headers
- [ ] Add `-d` flag for request body

### Error Handling
- [ ] Handle DNS resolution failures
- [ ] Handle connection failures
- [ ] Handle TLS errors
- [ ] Handle malformed responses
- [ ] Handle timeouts

### Response features
- [ ] Handle chunked transfer encoding
- [ ] Handle redirects (3xx status codes)
- [ ] Handle content compression (gzip, deflate)

---

## Current Status
- [ ] Phase 1 started
