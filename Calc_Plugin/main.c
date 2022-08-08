#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <malloc.h>
#include <dirent.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define SO 3

struct Libr
{
    char nameLib[25];
} Libr;

int filter(const struct dirent *dir)
{
    const char *name = dir->d_name;
    int len = strlen(name);

    if(len <= SO)
        return 0;

    name +=len - SO;
    if (strcmp(name, ".so") == 0)
        return TRUE;
    else
        return FALSE;

};

int getLibName(struct Libr **libs, const char *dir_name, int *count, int *size)
{
    struct dirent **namelist;

    *count = scandir(dir_name, &namelist, filter, alphasort);
    if (count < 0)
        perror("scandir");
    for(int i = 0; i < *count; ++i)
    {
        *libs = realloc(*libs, (*size+1) * sizeof(Libr));
        strcpy((*libs)[i].nameLib, namelist[i]->d_name);
        (*size)++;
    }
};

int printMenu(struct Libr **libs, int *size)
{
    void *handle;
    void (*cosine)();

    for(int i = 0; i < *size; i++)
    {
        char folder[25] = "plugins/";
        strcat(folder, (*libs)[i].nameLib);
        handle = dlopen(folder, RTLD_LAZY);
        if(NULL == handle)
            {
                printf("Error: Library %s can't open\n", (*libs)[i].nameLib);
            }

        cosine = dlsym(handle, "PlaginName");
         if(NULL == cosine)
        {
            perror("Function not found");
        }

        printf("%d - ", i + 1);
        cosine();
        printf("\n");
        dlclose(handle);
    }
    printf("0 - Exit\n");
};

int runPlugin(struct Libr **libs, int *numMenu, int *size)
{
    int value1 = 0;
    int value2 = 0;
    void *handle;
    char folder[25] = "plugins/";
    int (*func)(int, int);

    printf("Введите первое число:");
    scanf("%d", &value1);
    printf("Введите второе число:");
    scanf("%d", &value2);
    
    strcat(folder, (*libs)[*numMenu - 1].nameLib);
    handle = dlopen(folder, RTLD_LAZY);
    func = dlsym(handle, "PlaginToDo");
    if(NULL == func)
        {
            perror("Function not exist");
        }
    printf("Результат выражения: %d", func(value1, value2));
    printf("\n");

    dlclose(handle);
    free(*libs);

};


int main(int argc, char **argv)
{
    while(1)
    {
        int size = 0;
        int count = 0;
        int numMenu = 0;
        struct Libr *libs = malloc((size+1) * sizeof(Libr));
        getLibName(&libs, "plugins", &count, &size);
        printMenu(&libs, &size);
        printf("Введите номер:");
        scanf("%d", &numMenu);
        if(numMenu == 0)
        {
            break;
        }

        runPlugin(&libs, &numMenu, &size);
    }
};