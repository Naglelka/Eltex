#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define SIZE_NAME 15
#define SIZE_LAST_NAME 15
#define SIZE_NUMBER 15

struct User 
{
    char name[SIZE_NAME];
    char lastname[SIZE_LAST_NAME];
    char number[SIZE_NUMBER];
};

enum Errors 
{
    ERROR_PTR = -1,
    ERROR_MEMOUT = -2,
};

int add(struct User listUser[], int);
int delete(struct User listUser[], int, int);
int find(struct User listUser[], int, char *);
int show(struct User listUser[], int);

int main(){
    struct User *listUser = NULL;
    listUser = (struct User *) malloc(sizeof(struct User));
    char findName[SIZE_NAME];
    int size = 0;
    char menu[] = "Меню:\n1 - Добавить запись\n2 - Посмотреть запись\n3 - Найти пользователя (по имени)\n4 - Удалить запись\n5 - Выйти";
    int index;


    while(1)
    {
        int numMenu;

        printf("%s\n", menu);
        scanf("%d", &numMenu);
        printf("\n");
        getchar();

        switch(numMenu)
            {
                //add
                case 1:
                        system("clear");
                        listUser = (struct User *)realloc(listUser, (size+1) * sizeof(struct User));
                        add(listUser, size);
                        size++;
                        system("clear");
                        break;
                
                //show
                case 2:
                        system("clear");
                        show(listUser, size);
                        getchar();
                        system("clear");
                        break;

                //find
                case 3:
                        system("clear");
                        printf("Введите имя:");
                        scanf("%s", findName);
                        find(listUser, size, findName);
                        getchar(); getchar();                       
                        system("clear");
                        break;

                //delete
                case 4:
                        system("clear");
                        printf("Введите номер удаляемой записи:");
                        scanf("%d", &index);
                        delete(listUser, size, index-1);
                        size--;
                        system("clear");
                        break;

                //exit
                case 5:
                        system("clear");
                        goto finally;
                        break;
            }
    }
    finally:
        free(listUser);
    return 0;
};

int add(struct User listUser [], int size)
{
    if(NULL == listUser)
		return ERROR_PTR;

    printf("Введите имя:");
    scanf("%s", listUser[size].name);
    printf("Введите фамилию:");
    scanf("%s", listUser[size].lastname);
    printf("Введите телефон:");
    scanf("%s", listUser[size].number);

}

int show(struct User listUser[], int size)
{
    if(NULL == listUser)
		return ERROR_PTR;

    for(int i = 0; i < size; i++)
    {
        printf("%d пользователь: \n", i + 1);
        printf("Имя пользователя: %s\n", listUser[i].name);
        printf("Фамилия пользователя: %s\n", listUser[i].lastname);
        printf("Номер пользователя: %s\n", listUser[i].number);
    }
    printf("\n");

}

int find(struct User listUser[], int size, char *findName)
{
    if(NULL == listUser)
		return ERROR_PTR;

    int count = 0;
    for(int i = 0; i < size; i++)
    {
        if(0 == strcmp(listUser[i].name, findName))
        {
            printf("%d пользователь: \n", i + 1);
            printf("Имя пользователя: %s \n", listUser[i].name);
            printf("Фамилия пользователя: %s \n", listUser[i].lastname);
            printf("Номер пользователя: %s \n", listUser[i].number);
            printf("\n");
            count++;
        }
    }
    if(!count)
        printf("Пользователь не найден!\n");

}

int delete(struct User listUser[], int size, int index)
{
    if(NULL == listUser)
		return ERROR_PTR;

    if(index >= size || index < 0)
    {
        return ERROR_MEMOUT;
    }
    else 
    {
        for(index; index < size; index++)
        {
            strcpy(listUser[index].name, listUser[index + 1].name);
            strcpy(listUser[index].lastname, listUser[index + 1].lastname);
            strcpy(listUser[index].number, listUser[index + 1].number);
        }
    listUser = (struct User *)realloc(listUser, (size-1) * sizeof(struct User));
    }

}