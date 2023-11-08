#include <stdio.h> // printf/sprintf
#include <stdlib.h> // malloc
#include <pthread.h> // funcoes de multi-threading
#include <unistd.h> // sleep
#include <stdbool.h> // bool
#include <string.h> // strcat 
#include "mycondvar.h" // minha implementacao

#define NUM_REPORTERS 5
#define BUFFER_SIZE 100

pthread_mutex_t m;
struct condvar worker_ready;
struct condvar reporter_ready;
bool condition = false;
char op_order[BUFFER_SIZE*NUM_REPORTERS*3] = "";

void* worker(void* arg) {
    for (int i = 0; i < NUM_REPORTERS; i++) {
        // enquanto a condicao for verdadeira, esperar por um reporter torna-la falsa.
        pthread_mutex_lock(&m);
        while (condition) {
            strcat(op_order, "    worker espera por um reporter estar pronto.\n");
            condvar_wait(&reporter_ready, &m);
        }
        condition = true;
        strcat(op_order, "    worker terminou o trabalho.\n");
        condvar_signal(&worker_ready);
        pthread_mutex_unlock(&m);
    }

    return NULL;
}

void* reporter(void* arg) {
    int index = (int)arg;
    char buffer[BUFFER_SIZE];
    
    pthread_mutex_lock(&m);
    while(!condition) {
        sprintf(buffer, "reporter %d espera pelo worker estar pronto.\n", index);
        strcat(op_order, buffer);
        condvar_wait(&worker_ready, &m);
    }

    condition = false;

    sprintf(buffer, "reporter %d reportou.\n", index);
    strcat(op_order, buffer);
    condvar_signal(&reporter_ready);

    pthread_mutex_unlock(&m);

    return NULL;
}

int main() {
    pthread_mutex_init(&m, NULL);
    condvar_init(&worker_ready);
    condvar_init(&reporter_ready);
    pthread_t threads[NUM_REPORTERS];
    pthread_t worker_thread;

    pthread_create(&worker_thread, NULL, worker, NULL);
    for (int i = 0; i < NUM_REPORTERS; i++) {
        pthread_create(threads + i, NULL, reporter, (void*)i);
    }

    for (int i = 0; i < NUM_REPORTERS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf(op_order);

    return 0;
}