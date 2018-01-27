#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

int main() {
    int sock, i, j;
    struct sockaddr_in server;
    const char *data[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        printf("Could not create socket");
    }
    //puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_port = htons(12097);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }

    puts("Lets start\nConnected by ('127.0.0.1', 12097)\n");

    //keep communicating with server
    for(i = 0; i < 14; i++) {
        char day[1024];
        int ack = i%2;
        int ret = 1;

        strcpy(day, data[i%7]);

S:      printf("Sender sent a message: %s %d\n", day, ack);

        //Send some data
        send(sock , day, 2048, 0);
        send(sock , &ack, sizeof(int), 0);

        if(ret != 1) {
            recv(sock, &ret, sizeof(int), 0);
            ret = 1;
            goto P;
        }

        //Receive a reply from the server
        recv(sock, &ret, sizeof(int), 0);
        recv(sock, &ack, sizeof(int), 0);

        // send a correct ACK
P:      if(ret == 1)
            printf("Sender received a valid ACK for %d , send next message\n", ack);

        // A corrupt ACK is sent
        if(ret == 2) {
            printf("Sender received a corrupted ACK, keep waiting\n");
            for(j = 0; j < 5; j++)
                printf("Continue waiting\n");
            printf("Timeout. Send the message again\n");
            goto S;
        }

        // Receiver do not send ACK
        if(ret == 3) {
            for(j = 0; j < 5; j++)
                printf("Continue waiting\n");
            printf("Timeout. Send the message again\n");
            goto S;
        }

        // Receiver incorrectly responds with ACK 1 or 0
        if(ret == 4) {
            printf("Sender received an ACK with wrong sequency number, keep waiting\n");
            for(j = 0; j < 5; j++)
                printf("Continue waiting\n");
            printf("Timeout. Send the message again\n");
            goto S;
        }
    }

    close(sock);
    return 0;
}
