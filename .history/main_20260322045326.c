#include <stdio.h>  // Standard I/O functions (printf, fprintf)
#include <stdlib.h> // Memory management (malloc, free)
#include <string.h> // String manipulation (strncmp, strchr)
#include <sys/types.h>
#include <sys/socket.h> // Socket programming (socket, connect)
#include <netdb.h> // DNS resolution (getaddinfo)
#include <unistd.h> // POSIX API (close)
#include <arpa/inet.h> // Internet operations (inet_ntop)
#include <openssl/ssl.h> // TLS
#include <openssl/err.h> // OpenSSL error handling
#include "main.h" // Function declarations and type definitions

// parse request
int parse_request(int argc, char *argv[], request *out)
{
  if (!out) return 1;
  if (argc == 0)
  {
    out->err = "No arguments given, please provide them";
    return 1;
  }
  memset(out, 0, sizeof(*out));

  for (int i = 0; i < argc; i++) {
    // Method (optional, defaults to GET)
    if (strcmp(argv[i], "-X") == 0) {
      if (i + 1 < argc) {
        out->method = argv[i+1];
        i++;
      } else {
        out->err = "-X requires an argument";
        return 1;
      }
    }
    
    // Headers (optional, multiple allowed)
    else if (strcmp(argv[i], "-H") == 0) {
      if (i + 1 < argc && out->headers_count < MAX_HEADERS) {
        out->headers[out->headers_count++] = argv[i+1];
        i++;
      } else {
        out->err = "-H requires an argument";
        return 1;
      }
    }

    // Proxy (optional)
    else if(strcmp(argv[i], "-x") == 0)
    {
      if(i + 1 < argc)
      {
          out->proxy = argv[i+1]; 
          i++; // skip next
      }
      else {
        out->err = "-x requires an argument"; 
        return 1; 
      }
    }

    // Body (optional)
    else if (strcmp(argv[i], "-d") == 0) {
      if (i + 1 < argc) {
        out->body = argv[i+1];
        i++; // skip next
      } else {
        out->err = "-d requires an argument";
        return 1;
      }
    }
    // URL (required, must start with http:// or https://)
    else if (strncmp(argv[i], "http://", 7) == 0 || strncmp(argv[i], "https://", 8) == 0) {
      out->url = argv[i];
    }
  }

  if (out->url == NULL) {
    out->err = "URL required";
    return 1;
  }

  if (out->method == NULL) out->method = "GET";
  return 0;
}

// See notes: notes.md#url-parsing---string-functions for string function reference
int parse_url(char *url_str, url *out) {
  if (!url_str || !out) return 1;
  memset(out, 0, sizeof(*out));

  // Protocol
  sscanf(url_str, "%[^:]:", out->protocol);

  // Validate
  if (strcmp(out->protocol, "http") != 0 && strcmp(out->protocol,"https") != 0)
  {
    printf("Protocol must be http or https\n");
    return 1;
  }

  // Host
  sscanf(url_str, "%*[^:]://%255[^:/]", out->host);
  if (strlen(out->host) == 0) {
    printf("Host is missing\n");
    return 1;
  }

  // Port (custom from URL, or default based on protocol)
  char *host_start = strstr(url_str, "://") + 3;
  char *colon = strchr(host_start, ':');
  char *slash = strchr(host_start, '/');
  if (colon && (!slash || colon < slash)) {
    sscanf(colon + 1, "%19[0-9]", out->port);
  } else {
    strcpy(out->port, (strcmp(out->protocol, "https") == 0) ? "443" : "80");
  }

  // Path
  sscanf(url_str, "%*[^:]://%*[^/]%255s", out->path);
  if (strlen(out->path) == 0) {                                                                                                                                  
    strcpy(out->path, "/");                                                                                                                                    
  }              

  return 0;
}

// See notes.md#ssl-vs-http-communication for send/recv differences
SSL *set_tls(int sockfd) {
  // Step 1: Initialize SSL context
  const SSL_METHOD *tls_method = TLS_client_method();
  SSL_CTX *ssl_ctx = SSL_CTX_new(tls_method);
  SSL_CTX_set_default_verify_paths(ssl_ctx);

  // Step 2: Create SSL connection object
  SSL *ssl = SSL_new(ssl_ctx);
  SSL_set_fd(ssl, sockfd);

  // Step 3: Perform TLS handshake and verify certificate
  if (SSL_connect(ssl) != 1 || SSL_get_verify_result(ssl) != X509_V_OK) {
    fprintf(stderr, "TLS connection failed\n");
    SSL_free(ssl);
    SSL_CTX_free(ssl_ctx);
    return NULL;
  }

  return ssl;
}

// See notes.md#printf-family-of-functions
int send_request(int sockfd, SSL *ssl, request *req, url *parsed_url) {
  char req_buf[4096];
  int offset = 0;

  // Request line + default headers
  offset += snprintf(req_buf + offset, sizeof(req_buf) - offset,
    "%s %s HTTP/1.1\r\n"
    "Host: %s\r\n"
    "User-Agent: MyHTTPClient/1.0\r\n",
    req->method, parsed_url->path, parsed_url->host);

  // Custom headers from -H flags
  for (int i = 0; i < req->headers_count; i++) {
    offset += snprintf(req_buf + offset, sizeof(req_buf) - offset,
      "%s\r\n", req->headers[i]);
  }

  // Body headers + body (if present)
  if (req->body) {
    int body_len = strlen(req->body);
    offset += snprintf(req_buf + offset, sizeof(req_buf) - offset,
      "Content-Type: application/json\r\n"
      "Content-Length: %d\r\n",
      body_len);
  }

  // End of headers + body
  offset += snprintf(req_buf + offset, sizeof(req_buf) - offset, "Connection: close\r\n\r\n");

  if (req->body) {
    offset += snprintf(req_buf + offset, sizeof(req_buf) - offset, "%s", req->body);
  }

  int req_len = offset;

  if (req_len < 0 || (size_t)req_len >= sizeof(req_buf)) {
    fprintf(stderr, "Request too long\n");
    return 1;
  }

  if (ssl != NULL) {
    if (SSL_write(ssl, req_buf, req_len) <= 0) {
      perror("send failed");
      return 1;
    }
  } 
  else {
    if (send(sockfd, req_buf, req_len, 0) == -1) {
      perror("send failed");
      return 1;
    }
  }

  return 0;
}

// See notes.md#recv---reading-socket-data
int read_response(int sockfd, SSL *ssl) {
  char buffer[4096];
  ssize_t bytes_received;

  if (ssl != NULL) {
    while ((bytes_received = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0) {
      buffer[bytes_received] = '\0';
      fputs(buffer, stdout);
    }
    if (bytes_received < 0) {
      perror("ssl_read failed");
      return 1;
    }
  } 
  else {
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
      buffer[bytes_received] = '\0';
      fputs(buffer, stdout);
    }
    if (bytes_received == -1) {
      perror("recv failed");
      return 1;
    }
  }

  return 0;
}

#ifndef TEST
int main(int argc, char *argv[]) {

  // program status
  int status = 0;
  int sockfd = -1;
  url *parsed_url = NULL;
  struct addrinfo *res = NULL;
  SSL *ssl = NULL;

  // 1. Parse Request (skip argv[0] program name)
  request http_request = {};
  if (parse_request(argc - 1, argv + 1, &http_request)) {
    printf("Error: %s\n", http_request.err);
    status = 1;
    goto cleanup;
  }

  // 1. Parse URL
  parsed_url = malloc(sizeof(*parsed_url));
  int url_status = parse_url(http_request.url, parsed_url);
  if (url_status) {
    printf("Issues with parsing, ending program\n");
    status = 1;
    goto cleanup;
  }

  // 1. Parse proxy (if provided)
  if (http_request.proxy)
  {
      // 1. Parse proxy (if provided)
      char *proxy = malloc(sizeof(*parsed_url)); 
      proxy_status = parse_url(http_request.proxy, proxy); 

      if (proxy_status)
      {
        printf("Issues with parsing proxy, ending program\n"); 
        status = 1; 
        goto cleanup; 
      }
  }

  // CONNECTION

  // 2. Perform DNS Lookup (see notes.md#dns-resolution-with-getaddrinfo)
  struct addrinfo hints = {};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  char* host = http_request.proxy ? http_request.proxy : parsed_url->host; 

  int dns_status = getaddrinfo(parsed_url->host, parsed_url->port, &hints, &res);
  if (dns_status != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(dns_status));
    status = 1;
    goto cleanup;
  }

  // 3. Create socket and establish connection (see notes.md#socket-file-descriptors)
  struct addrinfo *p;
  for (p = res; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) continue;

  // 4. Establish socket connection
  if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      continue;
    }
    break;
  }
  if (!p) {
    fprintf(stderr, "Failed to connect to any address\n");
    freeaddrinfo(res);
    status = 1;
    goto cleanup;
  }
  freeaddrinfo(res); // Free DNS results

  // 5. Establish TLS handshake if https (see notes.md#tlsssl-handshake-process)
  if (strcmp(parsed_url->protocol, "https") == 0) {
    ssl = set_tls(sockfd);
    if (!ssl) {
      status = 1;
      goto cleanup;
    }
  }

  // REQUEST
  if (send_request(sockfd, ssl, &http_request, parsed_url)) {
    status = 1;
    goto cleanup;
  }

  // RESPONSE
  if (read_response(sockfd, ssl)) {
    status = 1;
    goto cleanup;
  }

  // Finish
  status = 0;
  goto cleanup;

  // cleanup
  cleanup:
    if (parsed_url) free(parsed_url);
    if (ssl) SSL_free(ssl);
    if (sockfd != -1) close(sockfd);
    return status;

}
#endif