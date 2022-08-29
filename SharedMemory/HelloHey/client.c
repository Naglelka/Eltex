#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

typedef struct {
    sem_t mutex;
    char message[20];
} shm;

int main()
{
    shm *data;
    int fd = shm_open("/serv", O_RDWR, 0777);

    ftruncate(fd, sizeof(shm));
    data = mmap(NULL, sizeof(shm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    printf("Waiting message\n");
    printf("Message: %s\n", data->message);
    sprintf(data->message, "Hello server\n");
    printf("Send message\n");
    sem_post(&data->mutex);

    close(fd);
    shm_unlink("/serv");
    exit(EXIT_SUCCESS);
}