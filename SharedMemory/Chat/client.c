#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define NAME "/MyServer"

typedef struct {
  sem_t mutex;
  int countClient;
  int count;
  char message[255];

} shm;

int main() {
  shm *info;
  int fd = 0;
  int offClient = 1;
  char *message;
  char *userName;
  char *outputMessage;


  fd = shm_open(NAME, O_RDWR, 0777);
  info = mmap(NULL, sizeof(shm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  userName = calloc(30, sizeof(char));
  printf("Write your UserName: ");
  fgets(userName, 30, stdin);

  ++info->countClient;

  while (offClient) {
    message = calloc(255, sizeof(char));
    outputMessage = calloc(255, sizeof(char));
    printf("Write message: ");
    fgets(message, 255, stdin);

    if (!strncmp(message, "exit\n", 6)) {
      strncat(outputMessage, userName, strlen(userName) - 1);
      strncat(outputMessage, " DISCONNECT\n", 13);
      sem_wait(&info->mutex);
      memset(info->message, '\0', 255);
      strncpy(info->message, outputMessage, strlen(outputMessage));
      info->count = info->count + 1;
      info->countClient = info->countClient - 1;
      sem_post(&info->mutex);
      offClient = 0;
      printf("Good bye\n");
      break;
    }
    strncat(outputMessage, userName, strlen(userName) - 1);
    strncat(outputMessage, ": ", 3);
    strncat(outputMessage, message, strlen(message));

    sem_wait(&info->mutex);
    memset(info->message, '\0', 255);
    strncpy(info->message, outputMessage, strlen(outputMessage));
    info->count = info->count + 1;
    sem_post(&info->mutex);

    free(outputMessage);
    free(message);
  }

  free(outputMessage);
  free(userName);
  free(message);

  exit(EXIT_SUCCESS);
}