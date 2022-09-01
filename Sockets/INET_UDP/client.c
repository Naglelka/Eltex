#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <netinet/in.h>

#define ADDRESS "INET_UDP"
#define BUFF_SIZE 255
#define PORT 5010

int main()
{
    struct sockaddr_in server;
    int fd = 0;
    int res = 0;
    int recvByte = 0;
    int sendByte = 0;
    int serverSize = sizeof(struct sockaddr_in);
    char buf[BUFF_SIZE];
    memset(&server, 0, sizeof(struct sockaddr_in));
    memset(buf, 0, sizeof(buf));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    sprintf(buf, "Hello server");
    sendByte = sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server));
    if(sendByte == -1) {
        perror("Send error");
        exit(EXIT_FAILURE);
    }

    recvByte = recvfrom(fd, buf, BUFF_SIZE, 0, (struct sockaddr *)&server, &serverSize);
    if(recvByte == -1) {
        perror("Recv error");
        exit(EXIT_FAILURE);
    }

    printf("Received message: %s, number of bytes: %d\n", buf, recvByte);

    close(fd);
    exit(EXIT_SUCCESS);
}