#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int priority = 0;

    mq_unlink("/serv");
    mq_unlink("/cli");
    mqd_t server = mq_open("/serv", O_WRONLY | O_CREAT, 0600, NULL);
    mqd_t client = mq_open("/cli", O_RDONLY | O_CREAT, 0600, NULL);

    struct mq_attr *attr = (struct mq_attr *)malloc(sizeof(struct mq_attr));
    mq_getattr(server, attr);
    char *buf = (char *)malloc(sizeof(char) * attr->mq_msgsize);

    sprintf(buf, "Hello client");
    printf("Send message\n");
    mq_send(server, buf, strlen(buf), 0);

    printf("Waiting message\n");
    memset(buf, 0, sizeof(char) * attr->mq_msgsize);
    mq_receive(client, buf, attr->mq_msgsize + 1, &priority);
    printf("Message: %s\n", buf);

    mq_close(server);
    mq_close(client);
    exit(EXIT_SUCCESS);
}