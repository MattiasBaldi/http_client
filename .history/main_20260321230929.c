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
int parse_request(char *argv[], int argc, request *out)
{

// optional
// get method -> fallback to GET
for (int i = 0; i < argc; i++)
{
  if (strcmp(argv[i], "-X") == 0)
  {
    out->method = argv[i+1];
    i++; // skip it  
  }
  else if () { 
    out->err = "-X requires an argument"; 
    return 1; 
  }
  else {
    out->method = "GET";
  }
}

// must
// get url

// optional
// get headers

// optional
// get body

// optional
// get proxies

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

  // Port
  strcpy(out->port, (strcmp(out->protocol, "https") == 0) ? "443" : "80");

  // Host
  sscanf(url_str, "%*[^:]://%255[^:/]", out->host);
  if (strlen(out->host) == 0) {
    printf("Host is missing\n");
    return 1;
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
  char req_buf[1024];
  int req_len = snprintf(req_buf, sizeof(req_buf),
    "%s %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: MyHTTPClient/1.0\r\nConnection: close\r\n\r\n",
    req->method, parsed_url->path, parsed_url->host);

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

  // Validate user input
  if (argc != 3) {
    printf("Params missing or too many\n");
    status = 1;
    goto cleanup;
  }

  // parse for headers

  request http_request = {
    .method = argv[1],
    .url = argv[2]
    // TODO: headers
    // TODO: body
    // TODO: proxies
  };

  // CONNECTION

  // 1. Parse URL
  parsed_url = malloc(sizeof(*parsed_url));
  int url_status = parse_url(http_request.url, parsed_url);
  if (url_status) {
    printf("Issues with parsing, ending program\n");
    status = 1;
    goto cleanup;
  }

  // 2. Perform DNS Lookup (see notes.md#dns-resolution-with-getaddrinfo)
  struct addrinfo hints = {};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

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
  SSL *ssl = NULL;
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