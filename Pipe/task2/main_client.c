#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    char buff[40] = {'\0'};
    int pipe = open("pipes", O_RDONLY);
    read(pipe, buff, 6);
    printf("%s", buff);

    exit(EXIT_SUCCESS);
}