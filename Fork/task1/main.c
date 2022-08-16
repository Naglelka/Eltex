#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid = fork();

   if(pid == 0) {
    printf("CHILD: pid - %d, ppid - %d\n", getpid(), getppid());
   } else {
    printf("PARENT: pid - %d, ppid - %d\n", getpid(), getppid());
   }
}