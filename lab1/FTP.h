#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// Struct of  Message transfer
typedef struct parameter{
  string client_path;
  int length_cp;
  string server_path;
  int length_sp;
  int request_type; // 0 is upload 1 is download
  int content_length;
} parameter;