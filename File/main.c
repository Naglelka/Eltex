#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void printFile(int file)
{
    int size;
    char *buffer;
    int err;

    size = lseek(file, 0L, SEEK_END);
    if(size == -1L)
    {
        perror("Move error");
    };

    err = lseek(file, 0L, SEEK_SET);
    if(err == -1L)
    {
        perror("Move error");
    };

    buffer = malloc(sizeof(*buffer) * size);
    err = read(file, buffer, size);
    if(err == -1)
    {
        perror("Move error");
    };

    printf("%s\n", buffer);
    free(buffer);
};

void reverseOutput(int file)
{
    int err;
    char symbol;
    int position;

    position = lseek(file, 0L, SEEK_END);
    if(position == -1L)
    {
        perror("Move error");
    };

    for(int i = position-1; i >= 0; --i)
    {
        err = lseek(file, i, SEEK_SET);
        if(err == -1)
        {
            perror("Move error");
        };
        
        err = read(file, &symbol, 1);
        if(err == -1)
        {
            perror("Move error");
        };
        if(symbol != '\n')
        printf("%c", symbol);
    };
    printf("\n");
};

int main()
{
    int file;
    char text[25] = "\n";
    int fileWr;
    int size_text = 25;

    file = open("test", O_CREAT|O_RDWR, S_IREAD|S_IWRITE);
    if(file == -1)
    {
        perror("Open failed on input file");
    }
    printf("Введите строку для записи в файл:");
    scanf("%s", text);
    getchar();

    fileWr = write(file, text, size_text);
    if(fileWr == -1)
    {
        perror("Error in the record");
    }
    printFile(file);
    reverseOutput(file);
};