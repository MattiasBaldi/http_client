#include <stdio.h>  // Standard I/O functions (printf, fprintf)
#include <stdlib.h> // Memory management (malloc, free)
#include <string.h> // String manipulation (strncmp, strchr)
#include <sys/socket.h> // Socket programming (socket, connect)
#include <netdb.h> // DNS resolution (getaddinfo)
#include <unistd.h> // POSIX API (close)
#include <arpa/inet.h> // Internet operations (inet_ntop)
// #include <openssl/ssl.h> // TLS - TODO: install OpenSSL headers
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

/* DNS RESOLVING */
int app_run(int argc, char *argv[]) {

  // validation of params
  if (argc != 3) {
    printf("Params missing or too many");
    return 1;
  }

  // init request
  request http_request = {
    .method = argv[1],
    .url = argv[2]
  };


  /*  !! CONNECTION !! */

  // 1. Parse Url -> parsed_url
  parsed_url *parsed = malloc(sizeof(parsed_url)); 
  parse_url("https://test.com", parsed);

  // 2. Resolve DNS -> ip

  // 3. Combine into socket (port from parsed_url and ip)

  // 4. Establish a socket connection

  // 5. Establish TLS handshake

  /*  !! REQUEST  !! */

  // 1. FORMAT REQUEST INTO HTTP REQUEST FORMAT

  // 2. SEND

  /*  !! RESPONSE !! */

  // 1. WAIT FOR THE RESPONSE

  // 2. PARSE THE RESPONSE

  // 3. RETURN THE RESPONSE


  // Free memory
  free(parsed); 

  return 1;
}

#ifndef TEST
int main(int argc, char *argv[]) {
  return app_run(argc, argv);
}
#endif