#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct initUser
{
    struct User user;
    char empty;
};

int init();
int add();
int delete();
int find();
int show();

int main(){
    struct initUser listUser[SIZE_LIST_USER];
    char findName[SIZE_NAME];

    char menu[] = "Меню:\n1 - Добавить запись\n2 - Посмотреть запись\n3 - Найти пользователя (по имени)\n4 - Удалить запись\n5 - Выйти";

    int index;

        for(int i = 0; i < SIZE_LIST_USER; i++)
        {
            init(&listUser[i]);
        }

        system("clear");

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
                        if(add(&listUser, SIZE_LIST_USER) < 0)
                        printf("Список заполнен\n");
                        system("clear");
                        break;
                
                //show
                case 2:
                        system("clear");
                        show(&listUser, SIZE_LIST_USER);
                        getchar();
                        system("clear");
                        break;

                //find
                case 3:
                        system("clear");
                        printf("Введите имя:");
                        scanf("%s", findName);
                        find(&listUser, SIZE_LIST_USER, findName);
                        getchar();
                        getchar();
                        system("clear");
                        break;

                //delete
                case 4:
                        system("clear");
                        printf("Введите номер удаляемой записи:");
                        scanf("%d", &index);
                        delete(&listUser, SIZE_LIST_USER, index-1);
                        system("clear");
                        break;

                //exit
                case 5:
                        system("clear");
                        return 0;
                        break;
            }
        }
    return 0;
};

int init(struct initUser *user)
{
    user->empty = 1;
	return 0;
}

int add(struct initUser (*user)[], int size)
{
        for(int i = 0; i < size; i++)
        {
            if((*user)[i].empty)
            {
                (*user)[i].empty = 0;
                printf("Введите имя:");
                scanf("%s", (*user)[i].user.name);
                printf("Введите фамилию:");
                scanf("%s", (*user)[i].user.lastname);
                printf("Введите телефон:");
                scanf("%s", (*user)[i].user.number);

                return 0;
            }
        }
    return -1;
}

int show(struct initUser (*user)[], int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%d пользователь: \n", i + 1);
        if((*user)[i].empty)
        {
            printf("Пустая запись\n");
        }
        else
        {
            printf("Имя пользователя: %s\n", (*user)[i].user.name);
            printf("Фамилия пользователя: %s\n", (*user)[i].user.lastname);
            printf("Номер пользователя: %s\n", (*user)[i].user.number);
        }
        printf("\n");
    }
    return 0;
}

int find(struct initUser (*user)[], int size, char *findName)
{
    int count = 0;
    for(int i = 0; i < size; i++)
    {
        if(!((*user)[i].empty) && !strcmp((*user)[i].user.name, findName))
        {
            printf("%d пользователь: \n", i + 1);
            printf("Имя пользователя: %s \n", (*user)[i].user.name);
            printf("Фамилия пользователя: %s \n", (*user)[i].user.lastname);
            printf("Номер пользователя: %s \n", (*user)[i].user.number);
            printf("\n");
            count++;
        }
    }
            
    if(!count)
        printf("Пользователь не найден!\n");
    return -1;
}

int delete(struct initUser (*user)[], int size, int index)
{
    if(index >= size || index < 0)
    {
        return -1;
    }
    
    (*user)[index].empty = 1;
	return 0;
}

