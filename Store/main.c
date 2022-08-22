#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define STORE_COUNT 5
#define BUYER_COUNT 3

int finish = 1;
int store[STORE_COUNT];
pthread_mutex_t store_mutex[STORE_COUNT];

void *thread_loader(void *arg)
{
    int load = 500;

    while(finish) {
        int store_id = rand() % STORE_COUNT;
        pthread_mutex_lock(&store_mutex[store_id]);
        store[store_id] += load;
        pthread_mutex_unlock(&store_mutex[store_id]);
        printf("Loader added %d items in %d store\n", load, store_id + 1);
        printf("Now store %d has %d items\n", store_id + 1, store[store_id]);
        sleep(1);
    }
    return NULL;
}

void *thread_buyer(void *arg)
{
    int* buyer = (int *)arg;
    int need = 9500 + (rand() % 1000);
    printf("The %d buyer needs at the beginning of %d items\n", *buyer, need);
    
    while(need > 0) {
        int store_id = rand() % STORE_COUNT;
        pthread_mutex_lock(&store_mutex[store_id]);
        int items = store[store_id] < need ? store[store_id] : need;
        store[store_id] = store[store_id] - items;
        need = need - items;

        if(items) {
            printf("The %d buyer bought %d items in %d store\n", *buyer, items, store_id + 1);
            printf("Now %d buyer needs %d items\n", *buyer, need);
        }

        pthread_mutex_unlock(&store_mutex[store_id]);
        sleep(2);
    }
    printf("THE %d BUYER BOUGHT ALL ITEMS\n", *buyer);
    return NULL;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    for(int i = 0; i < STORE_COUNT; i++) {
        store[i] = 900 + (rand() % 200);
        printf("Store %d has %d items\n", i + 1, store[i]);

        pthread_mutex_init(&store_mutex[i], NULL);
    }

    pthread_t loader;
    pthread_create(&loader, NULL, thread_loader, NULL);

    pthread_t buyer[BUYER_COUNT];
    int params[BUYER_COUNT];
    for(int i = 0; i < BUYER_COUNT; i++) {
        params[i]  = i + 1;
        pthread_create(&buyer[i], NULL, thread_buyer, (void *)&params[i]);
    }

    for(int i = 0; i < BUYER_COUNT; i++) {
        pthread_join(buyer[i], NULL);
    }

    finish = 0;
    pthread_join(loader, NULL);

    for(int i = 0; i < STORE_COUNT; i++) {
        pthread_mutex_destroy(&store_mutex[i]);
    }
    exit(EXIT_SUCCESS);
}