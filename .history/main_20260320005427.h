#ifndef MAIN_H
#define MAIN_H

#define MAX_PROTOCOL 20
#define MAX_HOST 256
#define MAX_PATH 256

typedef struct {
  char protocol[MAX_PROTOCOL];
  char host[MAX_HOST];
  int port;
  char path[MAX_PATH];
} parsed_url;                                                                                 
                                                                                              
typedef struct {                                                                              
  char *method;                                                                               
  char *url;                                                                                  
} request;

typedef struct {
  hints;
  status; 
} addrinfo; 

int parse_url(char *url, parsed_url *out);    

#endif


struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    // Perform DNS lookup
    int status = getaddrinfo(host, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        goto cleanup;
    }
    // Create socket and connect
    int sockfd = -1;
    struct addrinfo *p;
    for (p = res; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) continue;
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