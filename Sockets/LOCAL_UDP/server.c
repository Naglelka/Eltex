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
#define BUFF_SIZE 255

int main ()
{
    struct sockaddr_un server, client;
    int fd = 0;
    int res = 0;
    int recvByte = 0;
    int sendByte = 0;
    int len;
    char *buf = (char*)malloc(sizeof(char) * BUFF_SIZE);
    memset(&server, 0, sizeof(struct sockaddr_un));
    memset(&client, 0, sizeof(struct sockaddr_un));

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
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

    len = sizeof(client);

    recvByte = recvfrom(fd, buf, BUFF_SIZE, 0, (struct sockaddr *)&client, &len);
    if(recvByte == -1) {
        perror("Recv error");
        exit(EXIT_FAILURE);
    }

    printf("Received message: %s, Number of bytes: %d\n", buf, recvByte);
    sprintf(buf, "Hello client");

    sendByte = sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&client, len);
    if(sendByte == -1) {
        perror("Send error");
        exit(EXIT_FAILURE);
    }

    free(buf);
    close(fd);
    exit(EXIT_SUCCESS);
}