#include <stdio.h>  // Standard I/O functions (printf, fprintf)
#include <stdlib.h> // Memory management (malloc, free)
#include <string.h> // String manipulation (strncmp, strchr)
#include <sys/socket.h> // Socket programming (socket, connect)
#include <netdb.h> // DNS resolution (getaddinfo)
#include <unistd.h> // POSIX API (close)
#include <arpa/inet.h> // Internet operations (inet_ntop)

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



/* 1. URL PARSING */
int parse_url(const char *url, char **host, char **post, char **path) {
    // Check if URL starts with http://
    if (strncmp(url, "http:///", 7) != 0) {
      fprintf(stderr, "Error: URL must start with http://\n"); 
      return -1; 
    }
}