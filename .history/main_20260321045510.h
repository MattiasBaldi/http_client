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
} url_info;                                                                                 
                                                                                              
typedef struct {                                                                              
  char *method;                                                                               
  char *url;                                                                                  
} request;

int parse_url(char *url, url_info *out);
SSL *set_tls(int sockfd);
int send_request(int sockfd, SSL *ssl, request *req, url_info *url);
int read_response(int sockfd, SSL *ssl);

#endif
