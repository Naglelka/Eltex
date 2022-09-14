#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define PORT 5005
#define BUFF_SIZE 255

int count = 0;

void* redirect(void* num)
{
    int *clientfd = (int*)num;
    int recvByte = 0;
    int sendByte = 0;
    char buff[BUFF_SIZE];

    recvByte = recv(*clientfd, buff, BUFF_SIZE, 0);
    if(recvByte == -1) {
        perror("Recv error");
        exit(EXIT_FAILURE);
    }

    printf("Received message: %s, number of bytes: %d\n", buff, recvByte);
    sprintf(buff, "Hello client");

    sendByte = send(*clientfd, buff, strlen(buff), 0);
    if(sendByte == -1) {
        perror("Send error");
        exit(EXIT_FAILURE);
    }

    memset(buff, 0, BUFF_SIZE);
    close(*clientfd);
    count--;
}

int main()
{
    struct sockaddr_in server, client;
    int fd = 0;
    int clientfd[10];
    int len = 0;
    int res = 0;
    int new_fd = 0;
    pthread_t tid[10];

    len = sizeof(client);
    fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    res = bind(fd, (struct sockaddr*)&server, sizeof(server));
    if(res == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    listen(fd, 10);

    while(1) {
        new_fd = accept(fd, (struct sockaddr*)&client, &len);

        count++;

        if(count > 10) {
            printf("The maximum number of clients has been reached");
            exit(EXIT_FAILURE);
        }
        clientfd[count] = new_fd;
        pthread_create(&tid[count], NULL, redirect, &clientfd[count]);
    }
    for(int i = 0; i < 10; i++)
    {
        pthread_join(tid[i], NULL);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}