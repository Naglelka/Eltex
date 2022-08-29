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

int main() 
{
  shm *info;
  int fd = 0;
  int offServer = 1;

  fd = shm_open(NAME, O_RDWR | O_CREAT, 0777);
  printf("Server %s start\n", NAME);
  info = mmap(NULL, sizeof(shm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  ftruncate(fd, sizeof(shm));

  info->count = 0;
  info->countClient = 0;
  sem_init(&info->mutex, 1, 10);

  while (offServer) {
    if (info->count > 0) {
      sem_wait(&info->mutex);
      printf("%s", info->message);
      info->count = info->count - 1;
      if (!info->countClient) {
        offServer = 0;
      }
      sem_post(&info->mutex);
    }
  }
  printf("Server %s unlink\n", NAME);
  close(fd);
  shm_unlink(NAME);
  exit(EXIT_SUCCESS);
}