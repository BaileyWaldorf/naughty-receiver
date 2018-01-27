#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

int Naughty_Receiver(int ack);

int main() {
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons(12097);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 20);

    //Accept and incoming connection
    puts("Connected by client on [127.0.0.9]\n");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }
    //puts("Connection accepted");

    //Receive a message from sender
    int i;
    for(i = 0; i < 14; i++) {
        char day[10];
        int ack;
        int ret;

        recv(client_sock, day, 2048, 0);
        printf("Receiver just correctly received a message: %s", day);
        recv(client_sock, &ack, sizeof(int), 0);
        printf(" %d\n", ack);


        printf("How do you respond?\n");
        printf("(1) send a correct ACK; (2) send a corrupted ACK; (3) do not send ACK; (4) send a wrong ACK\n");

        ret = Naughty_Receiver(ack);

        //Send the message back to sender
        write(client_sock , &ret, sizeof(int));
        write(client_sock , &ack, sizeof(int));

        if(ret != 1) {
            recv(client_sock, day, 2048, 0);
            recv(client_sock, &ack, sizeof(int), 0);
            printf("Receiver just correctly received a duplicated message: %s %d\n", day, ack);
            printf("Receiver responds with ACK %d\n", ack);
            ret = 1;
            write(client_sock , &ret, sizeof(int));
        }
    }

    fflush(stdout);

    return 0;
}

int Naughty_Receiver(int ack) {
    // seed random number between 1-4
    int randCase = rand() % 4;
    randCase++;

    switch(randCase) {
        case 1:
            printf("Receiver correctly responds with ACK %d\n", ack);
            break;
        case 2:
            printf("A corrupted ACK is sent\n");
            break;
        case 3:
            printf("Receiver do not send ACK\n" );
            break;
        case 4:
            // switch ACK
            if(ack == 0)
                ack = 1;
            else
                ack = 0;

            printf("Receiver incorrectly responds with ACK %d\n", ack);
         break;
    }

    return randCase;
}
