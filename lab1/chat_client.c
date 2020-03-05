#include "FTP.h"

int main(int argc, char* argv[]) {
    int so;
    char s[100];
    parameter pm;
    struct sockaddr_in ad;

    socklen_t ad_length = sizeof(ad);
    struct hostent *hep;

    // init struct header of protocol
    initPm(&pm);

    // create socket
    int serv = socket(AF_INET, SOCK_STREAM, 0);

    // init address
    hep = gethostbyname(argv[1]);
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr = *(struct in_addr *)hep->h_addr_list[0];
    ad.sin_port = htons(12345);

    // connect to server
    connect(serv, (struct sockaddr *)&ad, ad_length);

    // after connected, sending request to server
    
    // setup parameter of header
    clearPm(&pm);
    gen_pm(&pm, 0, argv[3], argv[4], 0, 1);
    if(strcmp(argv[2], "-u") == 0){ // upload request
        pm.request_type = 0;
    }
    else if(strcmp(argv[2], "-d") == 0){ // download request
        pm.request_type = 1;
    }
    else{
        printf("Wrong input\n");
        return 0;
    }

    // open file from client
    FILE* fpt = open_client_file(&pm);
    if(fpt == NULL){
        printf("Can not open file on client\n");
        close(serv);
        return 0;
    }

    // sending request
    if(pm.request_type == 0){ // request upload file
        // store data to buffer
        char buff[file_size(fpt)];
        fread(buff, sizeof(buff), 1, fpt);
        fclose(fpt);

        // push buffer to socket
        pm.content_length = sizeof(buff);
        send_data(serv, &pm, buff, sizeof(buff));

        // server response
        clearPm(&pm);
        parseMess(serv, &pm);
        read_content(serv, &pm, s);
        printf("server says: %s\n", s);
    }
    else if(pm.request_type == 1){ // request download file
        // sending download request
        char w[] = "\0";
        pm.content_length = 1;
        send_data(serv, &pm, w, sizeof(w));

        // server response
        clearPm(&pm);
        parseMess(serv, &pm);

        // store data to buffer
        char buff[pm.content_length];
        read_content(serv, &pm, buff);

        // push buffer to file
        fwrite(buff, pm.content_length, 1, fpt);
        fclose(fpt);
        printf("successful download file\n");
    }

    // close client
    close(serv);
    return 0;
}


