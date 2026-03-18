#include <stdio.h>  // Standard I/O functions (printf, fprintf)
#include <stdlib.h> // Memory management (malloc, free)
#include <string.h> // String manipulation (strncmp, strchr)
#include <sys/socket.h> // Socket programming (socket, connect)
#include <netdb.h> // DNS resolution (getaddinfo)
#include <unistd.h> // POSIX API (close)
#include <arpa/inet.h> // Internet operations (inet_ntop)
#include <openssl/ssl.h> // TLS 

/* 1. URL PARSING */
typedef struct {
  char *protocol;
  char *domain; 
  int port; 
  char *path; 
  char *queries;
} parsed_url; 


parsed_url parse_url(char *url) {
  // get protocol

}

// Structs
typedef struct {
  char *method; 
  char *url; 
} request; 


/* DNS RESOLVING */
int main(int argc, char *argv[]) {

  // validation of params
  if (argc != 2) {
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