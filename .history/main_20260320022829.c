#include <stdio.h>  // Standard I/O functions (printf, fprintf)
#include <stdlib.h> // Memory management (malloc, free)
#include <string.h> // String manipulation (strncmp, strchr)
#include <sys/types.h>
#include <sys/socket.h> // Socket programming (socket, connect)
#include <netdb.h> // DNS resolution (getaddinfo)

#include <unistd.h> // POSIX API (close)
#include <arpa/inet.h> // Internet operations (inet_ntop)
//#include <openssl/ssl.h> // TLS - TODO: install OpenSSL headers
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

  // Validate params
  if (argc != 3) {
    printf("Params missing or too many\n");
    return 1;
  }

  // Init request
  request http_request = {
    .method = argv[1],
    .url = argv[2]
  };

  // CONNECTION

  // 1. Parse URL
  parsed_url *parsed = malloc(sizeof(parsed_url));
  if (!parsed) {
    printf("Memory allocation failed\n");
    return 1;
  }

  int url_status = parse_url(http_request.url, parsed);
  if (url_status) {
    printf("Issues with parsing, ending program\n");
    free(parsed);
    return 1;
  }

  // 2. Perform DNS Lookup
  struct addrinfo hints = {}, *res; // comes from netdb.h https://man7.org/linux/man-pages/man3/getaddrinfo.3.html#:~:text=IPv6%20dependencies.%0A%0A%20%20%20%20%20%20%20The-,addrinfo,-structure%20used%20by
  hints.ai_family = AF_UNSPEC; // IPv4 or IPv&
  hints.ai_socktype = SOCK_STREAM; // TCP Socket

  int dns_status = getaddrinfo(parsed->host, parsed->port, &hints, &res);
  if (dns_status != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(dns_status)); //  - gai_strerror() - converts DNS error codes to human-readable strings
    free(parsed);
    return 1; 
  }

  // 3. Combine into socket (port from parsed_url and ip)
  int sockfd = -1; 

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
    free(parsed); 
  }
  freeaddrinfo(res); // Free DNS results

  // 5. Establish TLS handshake
  

  // REQUEST

  // 1. Format request into HTTP format

  // 2. Send

  // RESPONSE

  // 1. Wait for response

  // 2. Parse response

  // 3. Return response

  // cleanup
  cleanup: 
    if(parsed) free(parsed);
    if(addrinfo) freeaddrinfo(res); 

  return 0;
}
#endif