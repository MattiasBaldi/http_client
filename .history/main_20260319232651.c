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

parsed_url parse_url(char *url) {
  parsed_url result = {0};

  // TODO: implement using functions above

  printf("parsed url running\n"); 

  // Protocol

  // Host
  
  // Port

  // Path
  

  return result;
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

  parsed_url parsed = parse_url("https://test.com");
  return 0;

  /*  !! CONNECTION !! */

  // 1. Parse Url -> parsed_url

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

  return 1;
}

#ifndef TEST
int main(int argc, char *argv[]) {
  return app_run(argc, argv);
}
#endif