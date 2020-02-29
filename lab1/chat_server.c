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
            while (1) {
                // it's client turn to chat, I wait and read message from client
                clearPm(&pm);
                parseMess(cli, &pm);
                read(cli, s, pm.content_length);
                // printf("client says: %s\n",s);

                // now it's my (server) turn
                printf("server>", s);
                scanf("%s", s);
                write(cli, s, strlen(s) + 1);
            }
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

