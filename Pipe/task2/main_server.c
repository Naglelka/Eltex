#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    mkfifo("pipes", 0666);
    char buff[] = "Hello";
    int pipe = open("pipes", O_WRONLY);
    write(pipe, buff, 6);
    close(pipe);
    
    exit(EXIT_SUCCESS);
}