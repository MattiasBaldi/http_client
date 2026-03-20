#ifndef MAIN_H
#define MAIN_H


#define MAX_PROTOCOL 20
#define MAX_HOST 256
#define MAX_PATH 256

typedef struct {                       
  char *protocol[];
  char *host;                                                                                 
  int port;                                                                                   
  char *path;                                                                                 
} parsed_url;                                                                                 
                                                                                              
typedef struct {                                                                              
  char *method;                                                                               
  char *url;                                                                                  
} request; 

parsed_url parse_url(char *url);    

#endif