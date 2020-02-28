#include "FTP.h"

int string_2_int(char* data);
char* init_dynamic_string(char* data);
void push_data_2_struct(int p, parameter* pm, char* data);

void parseMess(int fd, parameter* pm){
  char type_data[20];
  char data[1000];

  char c;
  int index = 0;
  while(1){
    // clear buffer
    bzero(type_data, sizeof(type_data));
    bzero(data, sizeof(data));

    // read data
    read(fd, &c, sizeof(c));
    if(c == '\n'){
      break;
    }

    // read type of data
    while(1){
      type_data[index++] = c;
      if(c == ':'){
        type_data[index - 1] = '\0';
        break;
      }
      read(fd, &c, sizeof(c));
    }
    // read data
    index = 0;
    while(1){
      read(fd, &c, sizeof(c));
      data[index++] = c;
      if(c == '\n'){
        data[index - 1] = '\0';
        break;
      }
    }
    // handle data and type
    for(int i = 0; i < 5; i++){
      if(strcmp(pmArray[i], type_data) == 0){
        push_data_2_struct(i, pm, data);
        break;
      }
    }
  }
}

void push_data_2_struct(int p, parameter* pm, char* data){
  switch(p){
    case 0:
      pm->type = string_2_int(data);
      break;
    case 1:
      pm->client_path = init_dynamic_string(data);
      break;
    case 2:
      pm->server_path = init_dynamic_string(data);
      break;
    case 3:
      pm->request_type = string_2_int(data);
      break;
    case 4:
      pm->content_length = string_2_int(data);
      break;
    default:
      // command of error
      break;
  }
}

char* init_dynamic_string(char* data){
  int length = strlen(data);
  char* data_string = (char*) malloc (sizeof(length) + 1);
  strcpy(data_string, data);
  return data_string;
}

int string_2_int(char* data){
  int value = 0;
  for(int i = 0; i < strlen(data); i++){
    value = value * 10 + (data[i] - '0');
  }
  return value;
}

void initPm(parameter* pm){
  pm->type = 0;
  pm->client_path = NULL;
  pm->server_path = NULL;
  pm->request_type = 0;
  pm->content_length = 0;
}

void clearPm(parameter* pm){
  free(pm->client_path);
  free(pm->server_path);
  initPm(pm);
}