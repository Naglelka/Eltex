#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <malloc.h>
#include <errno.h>

int splitCommand(char *command)
{
    char buf[50];
    char *ptr;
    int count = 0;

    strncpy(buf, command, strlen(command));
    ptr = strtok(buf, "|");

    while (ptr != NULL) {
		count++;
		ptr = strtok(NULL, "|");
	}
    return count;
}

void writeArray(char *command, char **commandPart, int count)
{
    char *ptr; 

    ptr = strtok(command, "|");
    for(int i = 0; i < count; i++) {
        commandPart[i] = strdup(ptr);
        ptr = strtok(NULL, "|");
    }
}

int splitCommandPart(char *part)
{
    char* ptr;
	char buf[50];
	int count = 0;

    strncpy(buf, part, strlen(part));
    ptr = strtok(buf, " ");

    while (ptr != NULL) {
		count++;
		ptr = strtok(NULL, " ");
	}
    return count;
}

void writeArrayLexeme(char **argv, char *commandPart, char *path, int count)
{
    char* ptr;

    ptr = strtok(commandPart, " ");
    for(int i = 0; i < count; i++) {
        if(i == 0) {
            argv[i] = (char*)malloc(strlen(path) + strlen(ptr) + 1);
            strcpy(argv[i], path);
            strncat(argv[i], ptr, strlen(ptr));
            ptr = strtok(NULL, " ");
        } else {
            argv[i] = strdup(ptr);
            ptr = strtok(NULL, " ");
        }
    }
    argv[count] = NULL;
}

int main()
{
    pid_t pid;
    char command[50];
    int partCount = 0;
    int lexemeCount = 0;
    char *commandPart[5];
    int fildes[2];
    char *argv[50];
    char path[]="/bin/";
    extern int errno;
    int i, j;
    int cmp;

    system("clear");
    while(1)
    {
        printf("Mybash $ ");

        fgets(command, 50, stdin);
        i = strlen(command) - 1;
		if (command[i] == '\n')
		{
			command[i]='\0';
		}
	
		cmp = strcmp("exit", command);
    		if (cmp == 0)
    		{
    			exit(EXIT_SUCCESS);
    		}

        partCount = splitCommand(command);
        writeArray(command, commandPart, partCount);

        pipe(fildes);

        for( j = 0; j < partCount; j++) {
            lexemeCount = splitCommandPart(commandPart[j]);
            writeArrayLexeme(argv, commandPart[j], path, lexemeCount);

            pid = fork();
             if(pid == 0) {
                if(((j%2) == 0) && (partCount > 1))
				{
					close(fildes[0]);
					dup2(fildes[1], 1);
				}
				else
				{
					close(fildes[1]);
					dup2(fildes[0], 0);
				}
                execve(argv[0], argv, NULL);
				
				if (errno == ENOENT)
				{
					printf("Комманда '%s' не найдена!\n", command);
				}
				exit(1);
             } else {
				for (i = 0; i < lexemeCount; i++)
				{
					free(argv[i]);
				}
			}	
        }
        close(fildes[0]);
		close(fildes[1]);

		for (i = 0; i < partCount; i++)
		{
            free(commandPart[i]);
			wait(NULL);
		}	
    }
    return 0;
}