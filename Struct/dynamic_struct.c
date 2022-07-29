#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define SIZE_LIST_USER 10
#define SIZE_NAME 15
#define SIZE_LAST_NAME 15
#define SIZE_NUMBER 15

struct User 
{
    char name[SIZE_NAME];
    char lastname[SIZE_LAST_NAME];
    char number[SIZE_NUMBER];
};

int add(struct User listUser[SIZE_LIST_USER], int);
int delete(struct User listUser[SIZE_LIST_USER], int, int);
int find(struct User listUser[SIZE_LIST_USER], int, char *);
int show(struct User listUser[SIZE_LIST_USER], int);

int main(){
    struct User *listUser;
    listUser = malloc(sizeof(struct User) * SIZE_LIST_USER);
    char findName[SIZE_NAME];
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
                        add(listUser, SIZE_LIST_USER);
                        system("clear");
                        break;
                
                //show
                case 2:
                        system("clear");
                        show(listUser, SIZE_LIST_USER);
                        getchar();
                        system("clear");
                        break;

                //find
                case 3:
                        system("clear");
                        printf("Введите имя:");
                        scanf("%s", findName);
                        find(listUser, SIZE_LIST_USER, findName);
                        getchar();
                        getchar();
                        system("clear");
                        break;

                //delete
                case 4:
                        system("clear");
                        printf("Введите номер удаляемой записи:");
                        scanf("%d", &index);
                        delete(listUser, SIZE_LIST_USER, index-1);
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

int add(struct User listUser[SIZE_LIST_USER], int size)
{
    for(int i = 0; i < size; i++)
        {
            if(0 == strcmp(listUser[i].number, "\0"))
            {
                printf("Введите имя:");
                scanf("%s", listUser[i].name);
                printf("Введите фамилию:");
                scanf("%s", listUser[i].lastname);
                printf("Введите телефон:");
                scanf("%s", listUser[i].number);

                return 0; 
            }
        }
    return -1;
}

int show(struct User listUser[SIZE_LIST_USER], int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%d пользователь: \n", i + 1);
        if(0 == strcmp(listUser[i].number, "\0"))
        {
            printf("Пустая запись\n");
        }
        else
        {
            printf("Имя пользователя: %s\n", listUser[i].name);
            printf("Фамилия пользователя: %s\n", listUser[i].lastname);
            printf("Номер пользователя: %s\n", listUser[i].number);
        }
        printf("\n");
    }
    return 0;
}

int find(struct User listUser[SIZE_LIST_USER], int size, char *findName)
{
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
    return -1;
}

int delete(struct User listUser[SIZE_LIST_USER], int size, int index)
{
    if(index >= size || index < 0)
    {
        return -1;
    }
    else 
    {
        strcpy(listUser[index].name,"\0");
        strcpy(listUser[index].lastname,"\0");
        strcpy(listUser[index].number,"\0");
            return 0;
    }

}