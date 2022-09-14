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
#define BUFFER_SIZE 128

int main()
{
    struct sockaddr_in server;
    int fd = 0;
    int res = 0;
    int recvByte;
    socklen_t serverSize = sizeof(server);
    char buff[BUFFER_SIZE];

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    printf("Socket create\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(ADDRESS);

    res = bind(fd, (struct sockaddr*)&server, sizeof(server));
    if(res == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    while(1) {
        memset(buff, 0, BUFFER_SIZE);
        recvByte = recvfrom(fd, buff, BUFFER_SIZE, 0, (struct sockaddr*)&server, &serverSize);
        if(recvByte == -1) {
            perror("Recvfrom error");
            exit(EXIT_FAILURE);
        }
        printf("Received message: %s, number of bytes: %d\n", buff, recvByte);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}