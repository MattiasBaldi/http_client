#ifndef MAIN_H
#define MAIN_H

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

#endif
