#ifndef MAIN_H
#define MAIN_H

#include <openssl/ssl.h>

#define MAX_PROTOCOL 20
#define MAX_PORT 20
#define MAX_HOST 256
#define MAX_PATH 256

typedef struct {
  char protocol[MAX_PROTOCOL];
  char host[MAX_HOST];
  char port[MAX_PORT];
  char path[MAX_PATH];
  char *err; 
} parsed_url;                                                                                 

typedef struct {
  char *method; 
  char *url; 
  char *headers; 
  char *body; 
  char *err; 
} parsed_request; 

int parse_url(char *url, parsed_url *out);
SSL *set_tls(int sockfd);
int send_request(int sockfd, SSL *ssl, parsed_request *req, parsed_url *url);
int read_response(int sockfd, SSL *ssl);

#endif
