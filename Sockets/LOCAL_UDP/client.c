#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <fcntl.h>

#define ADDRESS "LOCAL_UDP"
#define CLIENT_ADDR "CLIENT_UDP"
#define BUFF_SIZE 255

int main ()
{
    struct sockaddr_un server, client;
    int fd = 0;
    int res = 0;
    int recvByte = 0;
    int sendByte = 0;
    int serverSize = sizeof(struct sockaddr_un);
    char *buf = (char*)malloc(sizeof(char) * BUFF_SIZE);

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(struct sockaddr_un));
    memset(&client, 0, sizeof(struct sockaddr_un));

    client.sun_family = AF_LOCAL;
    strncpy(client.sun_path, CLIENT_ADDR, sizeof(client.sun_path) - 1);

    res = bind(fd, (struct sockaddr *)&client, sizeof(client));
    if (res < 0) {
        perror("bind:");
        exit(EXIT_FAILURE);
    }

    server.sun_family = AF_LOCAL;
    strncpy(server.sun_path, ADDRESS, sizeof(server.sun_path) - 1);

    sprintf(buf, "Hello server");
    connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr_un));

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
    close(fd);
    exit(EXIT_SUCCESS);
}