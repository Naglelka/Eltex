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

    mqd_t server = mq_open("/serv", O_RDONLY);
    mqd_t client = mq_open("/cli", O_WRONLY);

    struct mq_attr *attr = (struct mq_attr *)malloc(sizeof(struct mq_attr));
    mq_getattr(server, attr);
    char *buf = (char *)malloc(sizeof(char) * attr->mq_msgsize);

    printf("Waiting message\n");
    mq_receive(server, buf, attr->mq_msgsize + 1, &priority);
    printf("Message: %s\n", buf);

    printf("Send message\n");
    sprintf(buf, "Hello server");
    mq_send(client, buf, strlen(buf), 0);

    mq_close(server);
    mq_close(client);
    exit(EXIT_SUCCESS);
}