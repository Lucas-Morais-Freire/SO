#include <stdio.h> // printf/sprintf
#include <stdlib.h> // malloc
#include <pthread.h> // multi-threading functions
#include <unistd.h> // sleep
#include <stdbool.h> // bool
#include <string.h> // strcat 
#include "mycondvar.h"

#define NUM_REP_WORK 5
#define BUFFER_SIZE 100

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
struct condvar cv;
bool conditions[NUM_REP_WORK];
char op_order[BUFFER_SIZE*((NUM_REP_WORK*NUM_REP_WORK + 5*NUM_REP_WORK) >> 1)] = "";

void* worker(void* arg) {
    int index = (int)arg;
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "worker %d finished work.\n", index);
    
    pthread_mutex_lock(&m);
        strcat(op_order, buffer);
        conditions[index] = true;
        condvar_broadcast(&cv);
    pthread_mutex_unlock(&m);
    return NULL;
}

void* reporter(void* arg) {
    int index = (int)arg;
    char buffer[BUFFER_SIZE];

    pthread_mutex_lock(&m);
    while (!conditions[index]) {
        sprintf(buffer, "reporter %d waits.\n", index);
        strcat(op_order, buffer);
        condvar_wait(&cv, &m);
    }

    sprintf(buffer, "reporter %d reports.\n", index);
    strcat(op_order, buffer);

    pthread_mutex_unlock(&m);
    return NULL;
}

int main() {
    pthread_mutex_init(&m, NULL);
    condvar_init(&cv);
    pthread_t threads[2*NUM_REP_WORK];

    for (int i = 0; i < NUM_REP_WORK; i++) {
        conditions[i] = false;
    }

    for (int i = 0; i < NUM_REP_WORK; i++) {
        pthread_create(threads + 2*i, NULL, worker, (void*)i);
        pthread_create(threads + 2*i + 1, NULL, reporter, (void*)i);
    }

    for (int i = 0; i < 2*NUM_REP_WORK; i++) {
        pthread_join(threads[i], NULL);
    }

    printf(op_order);

    return 0;
}