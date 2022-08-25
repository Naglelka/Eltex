#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SERVER_NAME "/MyServer"

int main() {
  int res = 0;
  int countClient = 0;
  int off = 1;
  mqd_t mqd;

  unsigned int prioritety;
  void *buffer;
  ssize_t numRead;

  struct mq_attr attr;

  mqd = mq_open(SERVER_NAME, O_RDONLY | O_CREAT, 0777, NULL);

  if (mqd) {
    printf("Server start %s\n", SERVER_NAME);
  }
  res = mq_getattr(mqd, &attr);

  if(res == -1)
    exit(EXIT_FAILURE);

  long len = 0;
  len = attr.mq_msgsize;

  while (off) {
    buffer = calloc(len, sizeof(char));
    numRead = mq_receive(mqd, buffer, len, &prioritety);
    if (numRead != -1) {
      if (!strncmp((char *)buffer, "USER CONNECT\n", 14)) {
        ++countClient;
      }
      if (!strncmp((char *)buffer, "USER DISCONNECT\n", 17)) {
        --countClient;
      }
      printf("%s", (char *)buffer);
      free(buffer);
    }
    if (!countClient) {
      off = 0;
    }
  }

  res = mq_close(mqd);

  if(res == -1)
    exit(EXIT_FAILURE);

  printf("Server %s close\n", SERVER_NAME);

  res = mq_unlink(SERVER_NAME);
  if(res == -1)
    exit(EXIT_FAILURE);

  printf("Server %s unlink\n", SERVER_NAME);

  exit(EXIT_SUCCESS);
}