#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>

#define ADDRESS "LOCAL_TCP"
#define BUFF_SIZE 255

int main()
{
    struct sockaddr_un server;
    int fd = 0;
    int res = 0;
    int new_fd = 0;
    int recvByte = 0;
    int sendByte = 0;
    char *buf = (char*)malloc(sizeof(char) * BUFF_SIZE);

    fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    
    memset(&server, 0, sizeof(struct sockaddr_un));
    
    server.sun_family = AF_LOCAL;
    strncpy(server.sun_path, ADDRESS, sizeof(server.sun_path) - 1);

    new_fd = connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr_un));
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

    free(buf);
    close(new_fd);
    close(fd);
    exit(EXIT_SUCCESS);
}