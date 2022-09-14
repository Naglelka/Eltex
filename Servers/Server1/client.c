#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define PORT 5005
#define BUFF_SIZE 255

int main()
{
    struct sockaddr_in server;
    int fd = 0;
    int sendByte = 0;
    int recvByte = 0;
    char buff[BUFF_SIZE];
    int res = 0;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    res = connect(fd, (struct sockaddr *)&server, sizeof(server));
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

    recvByte = recv(fd, buff, BUFF_SIZE, 0);
    if(recvByte == -1) {
        perror("Recv error");
        exit(EXIT_FAILURE);
    }
    printf("Received message: %s, number of bytes: %d\n", buff, recvByte);

    close(fd);
    exit(EXIT_SUCCESS);
}