#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> // for read, write and fork

// Struct of  Message transfer
typedef struct parameter{
  int type; // 0 is client and 1 is server
  char* client_path;
  char* server_path;
  int request_type; // 0 is upload 1 is download
  int content_length;
} parameter;

// Array of parameter
static const char* pmArray[5] = {
                                  "Type", 
                                  "Client-path", 
                                  "Server-path", 
                                  "Request-type", 
                                  "Content-length"
                                };

// parsing function 2 parameters
void parseMess(int fd, parameter* pm);

// function to handle parameter
void initPm(parameter* pm);
void clearPm(parameter* pm);

// function to generate head of mess
int length_head(parameter* pm);
void gen_head_mess(parameter* pm, char* buff, int sz);

// function to generate head parameter from argument
void gen_pm(parameter* pm, int type, char* cp, char* sp, int request, int content);