#include <stdio.h> // printf/sprintf
#include <stdlib.h> // malloc
#include <pthread.h> // multi-threading functions
#include <unistd.h> // sleep
#include <stdbool.h> // bool
#include <string.h> // strcat 

#define NUM_REP_WORK 5
#define BUFFER_SIZE 100

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

void* unlocker(void* arg) {
    sleep(2);

    printf("unlocking mutex\n");
    pthread_mutex_unlock(&m);

    return NULL;
}

void* locker(void* arg) {

    pthread_mutex_lock(&m);
    printf("going to sleep\n");
    pthread_mutex_lock(&m);
    printf("woke up\n");
    pthread_mutex_unlock(&m);

    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, locker, NULL);
    pthread_create(&t2, NULL, unlocker, NULL);

    pthread_join(t2, NULL);
    pthread_join(t1, NULL);

    return 0;
}