#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>

#define SIZE 255
#define PORT 5015

int main()
{
    struct sockaddr_in server;
    int fd = 0;
    int res = 0;
    int sendByte = 0;
    int recvByte = 0;
    char buff[SIZE];

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    res = connect(fd, (struct sockaddr*)&server, sizeof(server));
    if(res == -1) {
        perror("Connect error");
        exit(EXIT_FAILURE);
    }

    sprintf(buff, "Hello server");

    sendByte = send(fd, buff, strlen(buff), 0);
    if(sendByte == -1) {
        perror("Send error");
        exit(EXIT_FAILURE);
    }

    recvByte = recv(fd, buff, SIZE, 0);
    if(recvByte == -1) {
        perror("Recv error");
        exit(EXIT_FAILURE);
    }
    
    printf("Received message: %s, number of bytes: %d\n", buff, recvByte);

    close(fd);
    exit(EXIT_SUCCESS);
}