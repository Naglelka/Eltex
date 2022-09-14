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

#define PORT 5010
#define MESSAGES 10
#define MESSAGE_SIZE 255

int statusThread[5] = {0};

struct infoThread {
    int index;
    char *name;
};

void* Redirect(void* param)
{
    struct infoThread *info = (struct infoThread*)param;
    mqd_t mq;
    int res;
    mq = mq_open(info->name, O_RDONLY);
    if(mq == -1) {
        perror("Mq_open in Redirect error");
        exit(EXIT_FAILURE);
    }

    while(1) {
        char inBuffer[MESSAGE_SIZE] = {0};
        res = mq_receive(mq, inBuffer, MESSAGE_SIZE, NULL);
        if(res == -1) {
            perror("Mq_recieve error");
            exit(EXIT_FAILURE);
        }

        int new_fd = atoi(inBuffer);

        char buff[MESSAGE_SIZE] = {0};

        int recvByte = recv(new_fd, buff, MESSAGE_SIZE, 0);
        if(res == -1) {
            perror("Recv error");
            exit(EXIT_FAILURE);
        }
            
        printf("Received message: %s, number of bytes: %d\n", buff, recvByte);
        sprintf(buff, "Hello client");

        int sendByte = send(new_fd, buff, strlen(buff), 0);
        if(sendByte == -1) {
            perror("Send error");
            exit(EXIT_FAILURE);
        }
    }
}

int main()
{
    struct sockaddr_in server;
    struct mq_attr mqAttr;
    struct infoThread info[10];
    int fd = 0;
    int res = 0;
    int new_fd[5] = {0};
    pthread_t tid[5];
    mqd_t mq[5];
    char **mqName = malloc(sizeof(char *) * 5);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    res = bind(fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
    if(res == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    res = listen(fd, 5);
    if(res == -1) {
        perror("Listen error");
        exit(EXIT_FAILURE);  
    }

    pthread_attr_t pthrAttr;
    pthread_attr_init(&pthrAttr);

    mqAttr.mq_flags = 0;
    mqAttr.mq_maxmsg = MESSAGES;
    mqAttr.mq_msgsize = MESSAGE_SIZE;
    mqAttr.mq_curmsgs = 0;

    for(int i = 0; i < 5; i++) {
        mqName[i] = malloc(sizeof(char) * 40);
        sprintf(mqName[i], "/client%d", i);
        mq[i] = mq_open(mqName[i], O_RDWR | O_CREAT, 0777, &mqAttr);
        if(mq[i] == -1) {
            perror("Mq_open error");
            exit(EXIT_FAILURE);
        }

        info[i].index = i;
        info[i].name = mqName[i];

        void *param = (void*)&info[i];

        res = pthread_create(&tid[i], &pthrAttr, Redirect, param);
        if(res < 0) {
            perror("Thread error");
            exit(EXIT_FAILURE);
        }
        statusThread[i] = 1;
    }

    for(int i = 0; ; i++) {
        if(i == 5) {
            i = 0;
        }

        if(!statusThread[i]) {
            continue;
        }

        new_fd[i] = accept(fd, NULL, NULL);
        if(new_fd[i] == -1) {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }

        char outBuffer[8] = {0};
        sprintf(outBuffer, "%d", new_fd[i]);
        res = mq_send(mq[i], outBuffer, strlen(outBuffer) + 1, 5);
        if(res == -1) {
            perror("Mq_send error");
            exit(EXIT_FAILURE);
        }

        statusThread[i] = 0;
    }
    
    for(int i = 0; i < 5; i++)
    {
        pthread_join(tid[i], NULL);
    }
    close(fd);
    free(mqName);
    exit(EXIT_SUCCESS);
}