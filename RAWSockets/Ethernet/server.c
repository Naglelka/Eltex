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

#define PORT 7777
#define SIZE 255

int main()
{
    struct sockaddr_in server, client;
    int fd = 0;
    int res = 0;
    int recvByte = 0;
    int sendByte = 0;
    socklen_t len;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    res = bind(fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
    if(res == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }
    
    len = sizeof(struct sockaddr_in);

    while(1) {
        memset(&client, 0, sizeof(struct sockaddr_in));
        char buff[SIZE] = {0};
        recvByte = recvfrom(fd, buff, SIZE, 0, (struct sockaddr *)&client, &len);
        if(recvByte == -1) {
            perror("Recv error");
            exit(EXIT_FAILURE);
        }

        printf("client ip = %s, port = %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Received message: %s; Number of bytes: %d\n", buff, recvByte);
        strcat(buff, ": from server\0");

        sendByte = sendto(fd, buff, strlen(buff), 0, (struct sockaddr *)&client, len);
        if(sendByte == -1) {
            perror("Send server error");
            exit(EXIT_FAILURE);
        }
    }
    close(fd);
    exit(EXIT_SUCCESS);
}