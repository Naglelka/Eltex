#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    pid = fork();

   if(pid) {
    printf("PARENT: pid - %d, ppid - %d\n", getpid(), getppid());
    wait(0);
    exit(EXIT_SUCCESS);
   } else if (!pid) {
    printf("CHILD (in parent process): pid - %d, ppid - %d\n", getpid(), getppid());
    execvp("./exec", argv);
    exit(EXIT_SUCCESS);
   } else {
    printf("ERROR");
    exit(EXIT_FAILURE);
   }
}