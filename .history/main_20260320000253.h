#ifndef MAIN_H
#define MAIN_H

#define MAX_PROTOCOL 20
#define MAX_HOST 256
#define MAX_PATH 256

typedef struct {                       
  char protocol[MAX_PROTOCOL];
  char host[MAX_HOST];                                                                                 
  int port;                                                                                   
  char *path[MAX_PATH];                                                                                 
} parsed_url;                                                                                 
                                                                                              
typedef struct {                                                                              
  char *method;                                                                               
  char *url;                                                                                  
} request; 

int parse_url(char *url, parsed_url *out);    

#endif