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

#define ADDRESS "INET_UDP"
#define BUFF_SIZE 255
#define PORT 5010

int main()
{
    struct sockaddr_in server, client;
    int fd = 0;
    int res = 0;
    int recvByte = 0;
    int sendByte = 0;
    socklen_t len;
    char buf[BUFF_SIZE];
    memset(&server, 0, sizeof(struct sockaddr_in));
    memset(&client, 0, sizeof(struct sockaddr_in));
    memset(buf, 0, sizeof(buf));

    fd = socket(AF_INET, SOCK_DGRAM, 0);
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

    len = sizeof(struct sockaddr_in);

    recvByte = recvfrom(fd, buf, BUFF_SIZE, 0, (struct sockaddr *)&client, &len);
    if(recvByte == -1) {
        perror("Recv error");
        exit(EXIT_FAILURE);
    }

    printf("Received message: %s, Number of bytes: %d\n", buf, recvByte);
    sprintf(buf, "Hello client");

    sendByte = sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&client, len);
    if(sendByte == -1) {
        perror("Send server error");
        exit(EXIT_FAILURE);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}
