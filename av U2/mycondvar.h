#ifndef MYCONDVAR_H
#define MYCONDVAR_H

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

struct condvar_node {
    pthread_mutex_t* thread_mutex;
    pthread_mutex_t* block_mutex;
    condvar_node* next;
    condvar_node* prev;
    bool notified;
};

struct condvar {
    condvar_node* front;
    condvar_node* back;
    pthread_mutex_t* operation_mutex;
};

void condvar_init(struct condvar* c) {
    c->back = NULL;
    c->front = NULL;
    *(c->operation_mutex) = PTHREAD_MUTEX_INITIALIZER;
}

void condvar_destroy(struct condvar* c) {
    condvar_node* curr = c->back;
    while (curr != NULL) {
        free(curr->block_mutex);
        free(curr->thread_mutex);
    }
}

void condvar_wait(struct condvar* c, pthread_mutex_t* m) {
    condvar_node* newNode = (condvar_node*)malloc(sizeof(condvar_node));
    newNode->thread_mutex = m;
    newNode->block_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    *(newNode->block_mutex) = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(newNode->block_mutex);
    if (!c->back) {

    }
    pthread_mutex_unlock(m);
}
void condvar_signal(struct condvar* c);
void condvar_broadcast(struct condvar* c);

#endif