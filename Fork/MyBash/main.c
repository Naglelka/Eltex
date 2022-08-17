#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <malloc.h>

char *enterCommand()
{
    char *textCommand = malloc(sizeof(*textCommand));
    int size = 0;

    while(1)
    {
        textCommand[size] = getchar();
        size++;

        if(textCommand[size - 1] == '\n')
        {
            textCommand[size - 1] = '\0';
            break;
        }

        textCommand = realloc(textCommand, sizeof(*textCommand) * (size + 1));
    }
    return textCommand;
};

int splitString(char ***split, char *command)
{
    if(command == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if(strcmp(command, "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }

    int size = strlen(command);
    int count = 1;
    int len = 1;
    (*split) = malloc(sizeof(char *) * count);
    (*split)[count - 1] = malloc(sizeof(char) * count);
    (*split)[count - 1][len - 1] = '\0';

    for(int i = 0; i < size; i++)
    {
        if(command[i] == ' ')
        {
            (*split)[count - 1][len - 1] = '\0';
            count++;
            (*split) = realloc(*split, sizeof(char *) * count);
            len = 1;
            (*split)[count - 1] = malloc(sizeof(char) * len);
            (*split)[count - 1][len - 1] = '\0';
        }
        else
        {
            len++;
            (*split)[count - 1] = realloc((*split)[count - 1], sizeof(char) * len);
            (*split)[count - 1][len - 2] = command[i];
        }
    }
    (*split) = realloc(*split, sizeof(char *) * (count + 1));
    (*split)[count] = (char *)NULL;

    return count;
};

int main(int argc, char **argV)
{
    system("clear");
    while(1)
    {
        pid_t pid_child;
        char *command = NULL;
        char **argv = NULL;
        int argc = 0;

        printf("Mybash $ ");

        command = enterCommand();
        argc = splitString(&argv, command);

        pid_child = fork();

        if(pid_child == 0)
        {
            int error = execvp(argv[0], argv);
            perror("Error");

            for(int i = 0; i <= argc; i++)
            {
                free(argv[i]);
            }

            free(argv);
            free(command);
            exit(EXIT_FAILURE);
        }
        else
        {
            int code;
            wait(&code);
            if(code != 0)
            {
                printf("Error: process was terminated with the code %d\n", code);
            }
            for(int i = 0; i <= argc; i++)
            {
                free(argv[i]);
            }
            free(argv);
            free(command);
        }
    }
}