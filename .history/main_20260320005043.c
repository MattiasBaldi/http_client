#include <stdio.h>  // Standard I/O functions (printf, fprintf)
#include <stdlib.h> // Memory management (malloc, free)
#include <string.h> // String manipulation (strncmp, strchr)
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
    printf("Protocol must be http or https"); 
    return 1; 
  }

  strcpy(out->protocol, protocol); 

  // Host
  sscanf(url, "%*[^/]://%255[^:/]", out->host); 
  
  // Port
  out->port = (strcmp(protocol, "https") == 0) ? 443 : 80;

  // Path
  sscanf(url, "%*[^/]%255s", out->path);

  return 0; 
}

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

  int err = parse_url(http_request.url, parsed);
  if (err) {
    printf("Issues with parsing, ending program\n");
    free(parsed);
    return 1;
  }

  // 2. Resolve DNS -> IP

  // 3. Combine into socket (port from parsed_url and ip)

  // 4. Establish socket connection

  // 5. Establish TLS handshake

  // REQUEST

  // 1. Format request into HTTP format

  // 2. Send

  // RESPONSE

  // 1. Wait for response

  // 2. Parse response

  // 3. Return response

  free(parsed);
  return 0;
}