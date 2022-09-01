#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <fcntl.h>

#define ADDRESS "LOCAL_TCP"
#define BUFF_SIZE 255

int main()
{
    struct sockaddr_un server, client;
    int fd = 0;
    int res = 0;
    int new_fd = 0;
    int recvByte = 0;
    int sendByte = 0;
    int clientSize = sizeof(client);
    char *buf = (char*)malloc(sizeof(char) * BUFF_SIZE);
    memset(&server, 0, sizeof(struct sockaddr_un));
    memset(&client, 0, sizeof(struct sockaddr_un));

    fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    printf("Socket create\n");
    
    server.sun_family = AF_LOCAL;
    strncpy(server.sun_path, ADDRESS, sizeof(server.sun_path) - 1);

    res = bind(fd, (struct sockaddr*)&server, sizeof(struct sockaddr_un));
    if(res == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    res = listen(fd, 5);
    if(res == -1) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    new_fd = accept(fd, (struct sockaddr*)&client, &clientSize);
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

    free(buf);
    close(new_fd);
    close(fd);
    exit(EXIT_SUCCESS);
}