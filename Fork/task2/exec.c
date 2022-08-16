#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    printf("CHILD (in child process): pid - %d, ppid - %d\n", getpid(), getppid());
    exit(EXIT_SUCCESS);
}