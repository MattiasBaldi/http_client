#ifndef MAIN_H
#define MAIN_H

typedef struct {                       
  char *protocol;
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