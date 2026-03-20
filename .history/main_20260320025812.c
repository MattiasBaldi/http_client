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

/* 1. URL PARSING */

/* Available string functions for URL parsing:
 *
 * strdup(str)                      - duplicates entire string, returns new pointer
 * strstr(haystack, needle)         - finds substring, returns pointer or NULL
 * strchr(str, char)                - finds first occurrence of character, returns pointer or NULL
 * strlen(str)                      - returns length of string
 * strncpy(dest, src, n)            - copies n characters from src to dest
 * strcmp(str1, str2)               - compares two strings, returns 0 if equal
 * atoi(str)                        - converts string to integer
 * malloc(size)                     - allocates memory, returns pointer
 * free(ptr)                        - frees allocated memory
 */
 
int parse_url(char *url, parsed_url *out) {
  if (!url || !out) return 1; 

  // Protocol
  char protocol[MAX_PROTOCOL];
  sscanf(url, "%[^:]:", protocol); 

  // Validate
  if (strcmp(protocol, "http") != 0 && strcmp(protocol,"https") != 0)
  {
    printf("Protocol must be http or https\n"); 
    return 1; 
  }

  strcpy(out->protocol, protocol); 

  // Host
  sscanf(url, "%*[^:]://%255[^:/]", out->host);
  
  // Port
  strcpy(out->port, (strcmp(protocol, "https") == 0) ? "443" : "80");

  // Path
  sscanf(url, "%*[^:]://%*[^/]%255s", out->path);    

  return 0; 
}

#ifndef TEST
int main(int argc, char *argv[]) {

  // define init
  int sockfd = -1;                 // socket handler
  parsed_url *parsed = NULL;       // parsed url
  struct addrinfo *res = NULL;     // dns info
  SSL_CTX *ctx = NULL;             // OpenSSL context
  SSL *ssl = NULL;                 // OpenSSL connection
  int status = 0;                  // Track success/failure of main

  // Validate params
  if (argc != 3) {
    printf("Params missing or too many\n");
    status = 1; 
    goto cleanup; 
  }

  // Init request
  request http_request = {
    .method = argv[1],
    .url = argv[2]
  };

  // CONNECTION

  // 1. Parse URL
  parsed = malloc(sizeof(parsed_url));
  int url_status = parse_url(http_request.url, parsed);
  if (url_status) {
    printf("Issues with parsing, ending program\n");
    status = 1; 
    goto cleanup; 
  }

  // 2. Perform DNS Lookup
  struct addrinfo hints = {}; // comes from netdb.h https://man7.org/linux/man-pages/man3/getaddrinfo.3.html#:~:text=IPv6%20dependencies.%0A%0A%20%20%20%20%20%20%20The-,addrinfo,-structure%20used%20by
  hints.ai_family = AF_UNSPEC; // IPv4 or IPv&
  hints.ai_socktype = SOCK_STREAM; // TCP Socket

  int dns_status = getaddrinfo(parsed->host, parsed->port, &hints, &res);
  if (dns_status != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(dns_status)); //  - gai_strerror() - converts DNS error codes to human-readable strings
    status = 1; 
    goto cleanup; 
  }

  // 3. Combine into socket (port from parsed_url and ip) 

  /**
   * sockfd = socket file descriptor                                                  
   * A "file descriptor" is just an integer that the operating system uses to refer to
   * an open file, socket, pipe, or other I/O resource. When you call socket(), it   
   * returns an int that acts as a handle to that socket.                                                                                  
   */

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
    goto cleanup; 
  }
  freeaddrinfo(res); // Free DNS results

  // 5. Establish TLS handshake if https
  if (strcmp(parsed->protocol, "https") == 0) {
    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
      fprintf(stderr, "SSL context creation failed\n");
      status = 1;
      goto cleanup;
    }
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

    ssl = SSL_new(ctx);
    if (!ssl) {
      fprintf(stderr, "SSL connection creation failed\n");
      status = 1;
      goto cleanup;
    }

    SSL_set_fd(ssl, sockfd);
    if (SSL_connect(ssl) <= 0) {
      fprintf(stderr, "TLS handshake failed\n");
      status = 1;
      goto cleanup;
    }
  }

  // REQUEST: Format HTTP request
  char request[1024];
  int req_len = snprintf(request, sizeof(request),
    "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
    parsed->path[0] ? parsed->path : "/", parsed->host);

  if (req_len < 0 || (size_t)req_len >= sizeof(request)) {
    fprintf(stderr, "Request too long\n");
    status = 1;
    goto cleanup;
  }

  // Send request
  if (ssl) {
    if (SSL_write(ssl, request, req_len) <= 0) {
      fprintf(stderr, "SSL write failed\n");
      status = 1;
      goto cleanup;
    }
  } else {
    if (send(sockfd, request, req_len, 0) == -1) {
      perror("send failed");
      status = 1;
      goto cleanup;
    }
  }

  // RESPONSE: Receive response
  char buffer[4096];
  ssize_t bytes_received;

  if (ssl) {
    while ((bytes_received = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0) {
      buffer[bytes_received] = '\0';
      fputs(buffer, stdout);
    }
  } else {
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
      buffer[bytes_received] = '\0';
      fputs(buffer, stdout);
    }
    if (bytes_received == -1) {
      perror("recv failed");
    }
  }

  // Finish
  status = 0;
  goto cleanup; 

  // cleanup
  cleanup:
    if(ssl) SSL_free(ssl);
    if(ctx) SSL_CTX_free(ctx);
    if(parsed) free(parsed);
    if(sockfd != -1) close(sockfd);
    return status; 
}
#endif