#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 255
#define PORT_TCP 7777
#define PORT_UDP 7778

int main()
{
    struct sockaddr_in server_TCP, server_UDP, client;
    int fd_tcp;
    int fd_udp;
    int new_fd = 0;
    socklen_t clientSize = sizeof(client);
    int recvByte;
    int sendByte;
    int res = 0;
    int large_fd = 0;
    char buff[BUFFER_SIZE];
    fd_set rfds;

    server_TCP.sin_family = AF_INET;
    server_TCP.sin_port = htons(PORT_TCP);
    server_TCP.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    server_UDP.sin_family = AF_INET;
    server_UDP.sin_port = htons(PORT_UDP);
    server_UDP.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    fd_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if(fd_tcp == -1) {
        perror("Socket UDP error");
        exit(EXIT_FAILURE);
    }

    printf("Socket TCP = %d\n", fd_tcp);

    if(bind(fd_tcp, (struct sockaddr*)&server_TCP, sizeof(server_TCP)) == -1) {
        perror("Bind TCP error");
        exit(EXIT_FAILURE);
    }

    fd_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd_udp == -1) {
        perror("Socket UDP error");
        exit(EXIT_FAILURE);
    }
    
    if(bind(fd_udp, (struct sockaddr*)&server_UDP, sizeof(server_UDP)) == -1) {
        perror("Bind UDP error");
        exit(EXIT_FAILURE);
    }

    res = listen(fd_tcp, 5);
    if(res == -1) {
        perror("Listen error");
        exit(EXIT_FAILURE);  
    }

    large_fd = fd_tcp > fd_udp ? fd_tcp : fd_udp;

    while(1) {
        FD_ZERO(&rfds);
        FD_SET(fd_tcp, &rfds);
        FD_SET(fd_udp, &rfds);

        res = select(large_fd + 1, &rfds, NULL, NULL, NULL);
        if(res < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }
        if(FD_ISSET(fd_udp, &rfds)) {
            recvByte = recvfrom(fd_udp, buff, BUFFER_SIZE, 0, (struct sockaddr*)&client, &clientSize);
            printf("Socket UDP = %d\n", fd_udp);
            if(recvByte == -1) {
                perror("Recvfrom error");
                exit(EXIT_FAILURE);
            }

            printf("Received message from UDP: %s, number of bytes: %d\n", buff, recvByte);
            sprintf(buff, "Hello client");

            sendByte = sendto(fd_udp, buff, strlen(buff), 0, (struct sockaddr *)&client, clientSize);
            if(sendByte == -1) {
                perror("Send error");
                exit(EXIT_FAILURE);
            }
        } else if(FD_ISSET(fd_tcp, &rfds)) {              
            new_fd = accept(fd_tcp, 0, 0);
            if(new_fd == -1) {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            recvByte = recv(new_fd, buff, BUFFER_SIZE, 0);
            if(recvByte == -1) {
                perror("Recv error");
                exit(EXIT_FAILURE);
            }

            printf("Received message from TCP: %s, number of bytes: %d\n", buff, recvByte);
            sprintf(buff, "Hello client");

            sendByte = send(new_fd, buff, strlen(buff), 0);
            if(sendByte == -1) {
                perror("Send error");
                exit(EXIT_FAILURE);
            }
            close(new_fd);
        }
    }
    close(fd_tcp);
    close(fd_udp);
    exit(EXIT_SUCCESS);
}