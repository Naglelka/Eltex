#include <stdio.h>
#include "calc.h"
#include <stdlib.h>


int input(int*, int*);

int main()
{
    int res = 0;
    int value1, value2;
    char menu[] = "Меню:\n1 - Найти сумму\n2 - Найти произведение\n3 - Найти разность\n4 - Найти частное\n5 - Выйти";

    while(1)
    {
        int numMenu;

        printf("%s\n", menu);
        scanf("%d", &numMenu);

        switch(numMenu)
        {
            //addition
            case 1:
                    input(&value1, &value2);
                    res = addition(value1, value2);
                    getchar();
                    system("clear");
                    break;
            //multiplication
            case 2:
                    input(&value1, &value2);
                    res = multiplication(value1, value2);
                    getchar();
                    system("clear");
                    break;
            //subtraction
            case 3:
                    input(&value1, &value2);
                    res = subtraction(value1, value2);
                    getchar();
                    system("clear");
                    break;
            //division
            case 4:
                    input(&value1, &value2);
                    res = division(value1, value2);
                    getchar();
                    system("clear");
                    break;
            //exit
            case 5:
                    system("clear");
                    return 0;
                    break;
        }
        printf("Результат выражения: %d", res);
        printf("\n");
        getchar();
        system("clear");

    }
    return 0;
};

int input(int *param1, int *param2)
{
    printf("Введите первое число:");
    scanf("%d", param1);
    printf("Введите второе число:");
    scanf("%d", param2);
};