#include <stdio.h>
#include <pthread.h>

int a = 0;

void *func(void *arg)
{
    int tmp;

    for(int i = 0; i < 1000000; i++)
    {
        tmp = a;
        tmp ++;
        a = tmp;
    }

    return 0;
}

int main(void)
{
    pthread_t tid[5];
    int status;

    for(int i = 0; i < 5; i++)
    {
        pthread_create(&tid[i], NULL, func, (void*)&i);
    }

    for(int i = 0; i < 5; i++)
    {
        pthread_join(tid[i], (void**) &status);
    }

    printf("%d\n", a);
    return 0;
}