#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define ADDRESS "255.255.255.255"
#define PORT 7777

int main()
{
    struct sockaddr_in server;
    int fd = 0;
    int flag = 1;
    int res = 0;
    socklen_t serverSize = sizeof(server);
    char buff[] = "Hello client";

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    printf("Socket create\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(ADDRESS);

    if(setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) == -1) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    while(1) {
        res = sendto(fd, buff, strlen(buff), 0, (struct sockaddr *)&server, serverSize);
        if(res == -1) {
            perror("Sendto error");
            exit(EXIT_FAILURE);
        }
        printf("Message sent\n");
        sleep(1);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}