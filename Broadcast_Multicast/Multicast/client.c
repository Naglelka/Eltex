#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define ADDRESS "224.0.0.1"
#define PORT 7770
#define BUFFER_SIZE 128

int main()
{
    struct sockaddr_in server;
    int fd = 0;
    int res = 0;
    int recvByte;
    socklen_t serverSize = sizeof(server);
    char buff[BUFFER_SIZE];

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    printf("Socket create\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    struct ip_mreqn group;

    group.imr_address.s_addr = server.sin_addr.s_addr;
    group.imr_multiaddr.s_addr = inet_addr(ADDRESS);
    group.imr_ifindex = 0;

    res = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));
    if(res == -1) {
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    res = bind(fd, (struct sockaddr*)&server, sizeof(server));
    if(res == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    while(1) {
        memset(buff, 0, BUFFER_SIZE);
        recvByte = recvfrom(fd, buff, BUFFER_SIZE, 0, (struct sockaddr*)&server, &serverSize);
        if(recvByte == -1) {
            perror("Recvfrom error");
            exit(EXIT_FAILURE);
        }
        printf("Received message: %s, number of bytes: %d\n", buff, recvByte);
    }
    close(fd);
    exit(EXIT_FAILURE);
}