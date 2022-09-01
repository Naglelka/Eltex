#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ADDRESS "INET_TCP"
#define BUFF_SIZE 255
#define PORT 5005

int main()
{
    struct sockaddr_in server;
    int fd = 0;
    int res = 0;
    int new_fd = 0;
    int recvByte = 0;
    int sendByte = 0;
    char buf[BUFF_SIZE];
    memset(&server, 0, sizeof(struct sockaddr_in));
    memset(buf, 0, sizeof(buf));

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    printf("Socket create\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    res = bind(fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
    if(res == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    res = listen(fd, 5);
    if(res == -1) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    new_fd = accept(fd, (struct sockaddr*)NULL, NULL);
    if(new_fd == 1) {
        perror("Accept error");
        exit(EXIT_FAILURE);
    }

    recvByte = recv(new_fd, buf, BUFF_SIZE, 0);
    if(recvByte == -1) {
        perror("Recv error");
        exit(EXIT_FAILURE);
    }

    printf("Received message: %s, number of bytes: %d\n", buf, recvByte);
    sprintf(buf, "Hello client");

    sendByte = send(new_fd, buf, strlen(buf), 0);
    if(sendByte == -1) {
        perror("Send error");
        exit(EXIT_FAILURE);
    }

    close(new_fd);
    close(fd);
    exit(EXIT_SUCCESS);
}