#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>

#define PORT 5015
#define MESSAGES 10
#define MESSAGE_SIZE 255
#define Q_NAME "/clients"
#define SIZE 255

int fd;
mqd_t mqd;

void* Redirect(void* param)
{
    struct sockaddr_in client;
    int new_fd = 0;
    int res = 0;
    int len = 0;
    int recvByte = 0;
    int sendByte = 0;
    char buff[SIZE];
    char message[2];

    while(1) {
        res = mq_receive(mqd, message, 2, NULL);
        if(res == -1) {
            perror("Mq_recieve error");
            exit(EXIT_FAILURE);
        }
        new_fd = (int) message[0];
        len = sizeof(client);

        recvByte = recv(new_fd, buff, MESSAGE_SIZE, 0);
        if(res == -1) {
            perror("Recv error");
            exit(EXIT_FAILURE);
        }
        printf("Received message: %s, number of bytes: %d\n", buff, recvByte);
        sprintf(buff, "Hello client");

        sendByte = send(new_fd, buff, strlen(buff), 0);
        if(sendByte == -1) {
            perror("Send error");
            exit(EXIT_FAILURE);
        }
        memset(buff, 0, SIZE);
        close(new_fd);
    }
}

int main()
{
    struct sockaddr_in server, client;
    struct mq_attr mqAttr;
    int new_fd;
    int res = 0;
    int len;
    char status[2];
    pthread_t tid[10];

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    mqAttr.mq_msgsize = 2;
    mqAttr.mq_maxmsg = MESSAGES;

    mqd = mq_open(Q_NAME, O_RDWR |O_CREAT, 0777, &mqAttr);
    if(mqd == -1) {
        perror("Mq_open error");
        exit(EXIT_FAILURE);
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    res = bind(fd, (struct sockaddr*)&server, sizeof(server));
    if(res == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    res = listen(fd, 5);
    if(res == -1) {
        perror("Listen error");
        exit(EXIT_FAILURE);  
    }

    for(int i = 0; i < 10; i++) {
        res = pthread_create(&tid[i], NULL, Redirect, NULL);
        if(res < 0) {
            perror("Thread error");
            exit(EXIT_FAILURE);
        }
    }

    while(1) {
        new_fd = accept(fd, (struct sockaddr*)&client, &len);
        if(new_fd == -1) {
                perror("Accept error");
                exit(EXIT_FAILURE);
        }

        status[0] = (char) new_fd;
        status[1] = 0;

        res = mq_send(mqd, status, sizeof(status), 0);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(tid[i], NULL);
    }
}