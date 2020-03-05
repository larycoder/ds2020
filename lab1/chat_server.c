#include "FTP.h"

int main() {
    int ss, cli, pid;
    struct sockaddr_in ad;
    char s[100];
    socklen_t ad_length = sizeof(ad);
    parameter pm;

    // init parameter of protocol
    initPm(&pm);

    // create the socket
    ss = socket(AF_INET, SOCK_STREAM, 0);

    // bind the socket to port 12345
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    ad.sin_port = htons(12345);
    bind(ss, (struct sockaddr *)&ad, ad_length);

    // then listen
    listen(ss, 0);

    while (1) {
        // an incoming connection
        cli = accept(ss, (struct sockaddr *)&ad, &ad_length);

        pid = fork();
        if (pid == 0) {
            // I'm the son, I'll serve this client
            printf("client connected\n");

            // I wait and read message from client
            clearPm(&pm);
            parseMess(cli, &pm);

            // open file  from server side
            FILE* fpt = open_server_file(&pm);
            if(fpt == NULL){
                char error[] = "Can not open file on server";
                pm.type = 1;
                pm.content_length = strlen(error) + 1;
                send_data(cli, &pm, error, sizeof(error));
                close(cli);
                return 0;
            }

            // process request from client
            if(pm.request_type == 0){ // request is upload
                // store data to buffer
                char buff[pm.content_length];
                read_content(cli, &pm, buff);

                // push buffer to file
                fwrite(buff, pm.content_length, 1, fpt);
                fclose(fpt);

                // send confirm message
                char confirm[] = "Successful";
                pm.type = 1;
                pm.content_length = strlen(confirm) + 1;
                send_data(cli, &pm, confirm, sizeof(confirm));
            }
            else{ // request is download
                // store data to buffer
                char buff[file_size(fpt)];
                fread(buff, sizeof(buff), 1, fpt);
                fclose(fpt);
                
                // push buffer to socket
                pm.type = 1;
                pm.content_length = sizeof(buff);
                send_data(cli, &pm, buff, sizeof(buff));
            }
            close(cli);
            return 0;
        }
        else {
            // I'm the father, continue the loop to accept more clients
            continue;
        }
    }
    // disconnect
    close(cli);

}

