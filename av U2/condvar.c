#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_REPORTERS 1
#define NUM_WORKERS 1

pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool condicao = false;
int counter = 0;

void* thread_func(void* arg) {
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < 999999999; i++) {
        counter++;
    }

    condicao = true;

    printf("worker thread sinalizou o termino do trabalho.\n");
    pthread_cond_signal(&cond_var);

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);

    // sleep(2);
    pthread_mutex_lock(&mutex);
    // sleep(2);

    while (!condicao) {
        printf("Thread principal esta esperando pelo termino do trabalho.\n");
        pthread_cond_wait(&cond_var, &mutex);
    }

    printf("Thread principal verificou que a condicao foi alcancada.\n");
    printf("contador: %d\n", counter);

    pthread_mutex_unlock(&mutex);

    pthread_join(thread, NULL);

    pthread_cond_destroy(&cond_var); // Cleanup the condition variable

    return 0;
}