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

    while (1) {
        // after connected, it's client turn to chat
        printf("client>");
        scanf("%s", s);

        // setup parameter of header
        clearPm(&pm);
        gen_pm(&pm, argv[2], argv[3], 0, strlen(s) + 1);

        // send some data to server
        
        // send header
        char head[length_head(&pm)];
        gen_head_mess(&pm, head, sizeof(head));
        write(serv, head, strlen(head));
        
        //send content
        write(serv, s, strlen(s) + 1);

        // then it's server turn
        read(serv, s, sizeof(s));

        printf("server says: %s\n", s);
    }
}


