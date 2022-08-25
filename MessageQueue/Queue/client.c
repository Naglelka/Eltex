#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define NAME "/MyServer"

int main() {
  int res = 0;
  mqd_t mqd;
  long len = 0;

  char *messageBuffer;
  char *userName;
  char *outputMessage;

  struct mq_attr attr;

  mqd = mq_open(NAME, O_WRONLY);

  if (mqd) {
    userName = calloc(255, sizeof(char));
    printf("Write your UserName: ");
    fgets(userName, 255, stdin);

    res = mq_send(mqd, "USER CONNECT\n", 14, 0);
    
    if(res == -1)
        exit(EXIT_FAILURE);

    printf("Hello, %s\nServer connect %s\n", userName, NAME);
    res = mq_getattr(mqd, &attr);

    if(res == -1)
        exit(EXIT_FAILURE);

    printf("To exit print exit\n");

    len = attr.mq_msgsize;

    while (1) {
      printf("Send message: ");
      messageBuffer = calloc(255, sizeof(char));
      outputMessage = calloc(len, sizeof(char));
      fgets(messageBuffer, len, stdin);
      strncat(outputMessage, userName, strlen(userName) - 1);
      strncat(outputMessage, ": ", 3);
      strncat(outputMessage, messageBuffer, strlen(messageBuffer));

      if (!strncmp(messageBuffer, "exit\n", 6)) {
        free(messageBuffer);
        messageBuffer = calloc(255, sizeof(char));
        strncat(messageBuffer, "USER DISCONNECT\n", 17);
        mq_send(mqd, messageBuffer, strlen(messageBuffer), 0);
        break;
      }
      if (!mq_send(mqd, outputMessage, strlen(outputMessage), 0)) {
        printf("Message sent\n");
        free(outputMessage);
        free(messageBuffer);
      } else {
        printf("Message not sent\n");
        free(outputMessage);
        free(messageBuffer);
      }
    }

    res = mq_close(mqd);

    if(res == -1)
        exit(EXIT_FAILURE);

    printf("Good bye\n");

    free(outputMessage);
    free(userName);
    free(messageBuffer);
  } else {
    printf("No connection to server\n");
  }
  exit(0);
}