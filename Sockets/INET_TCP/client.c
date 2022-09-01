#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <netinet/in.h>

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

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    new_fd = connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if(new_fd == 1) {
        perror("Connect error");
        exit(EXIT_FAILURE);
    }

    sprintf(buf, "Hello server");
    sendByte = send(fd, buf, strlen(buf), 0);
    if(sendByte == -1) {
        perror("Send error");
        exit(EXIT_FAILURE);
    }

    recvByte = recv(fd, buf, BUFF_SIZE, 0);
    if(recvByte == -1) {
        perror("Recv error");
        exit(EXIT_FAILURE);
    }

    printf("Received message: %s, number of bytes: %d\n", buf, recvByte);

    close(fd);
    exit(EXIT_SUCCESS);
}