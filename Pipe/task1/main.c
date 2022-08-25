#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int child;
    int fildes[2];

    pipe(fildes);
    child = fork();
    if(child) {
        write(fildes[1], "Hello", 5);
    } else {
        char buff[5];
        read(fildes[0], buff, 5);
        printf("%s\n", buff);
    }
    exit(EXIT_SUCCESS);
}