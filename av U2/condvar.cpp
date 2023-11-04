#include <iostream>
#include <thread>
#include <mutex>

struct condvar_node {
    std::mutex* thread_mutex;
    condvar_node* next;
    condvar_node* prev;
};

struct condvar {
    condvar_node* front;
    condvar_node* back;
    std::mutex* operation_mutex;
};

void condvar_init(struct condvar* c);
void condvar_wait(struct condvar* c, struct mutex* m);
void condvar_signal(struct condvar* c);
// void condvar_broadcast(struct condvar* c);

condvar cv;
std::mutex mtx;
bool condition;
int counter;

void worker_thread() {
        mtx.lock();
        counter = 0;
        for (volatile int i = 0; i < 100000; i++) {
            counter += 1;
        }

        condition = true;

    condvar_signal(&cv);
}

void report_thread() {
    
}

int main() {



    return 0;
}

void condvar_init(struct condvar* c) {
    c->back = NULL;
    c->front = NULL;
    c->operation_mutex = new std::mutex;
}

void condvar_signal(struct condvar* c) {
    c->operation_mutex->lock();
        if (c->front != NULL) {
            condvar_node* last_node = c->front;
            last_node->thread_mutex->unlock();
            c->front = c->front->prev;
            c->front->next = NULL;
            delete last_node->thread_mutex;
            delete last_node;
        }
    c->operation_mutex->unlock();
}

void condvar_wait(struct condvar* c, std::mutex* m) {
    m->unlock();

    c->operation_mutex->lock();
        std::mutex* queue_mutex = new std::mutex;
        queue_mutex->lock();
        condvar_node* newNode = new condvar_node;
        newNode->thread_mutex = queue_mutex;
        if (c->back == NULL) {
            c->back = newNode;
            c->front = newNode;
            newNode->next = NULL;
            newNode->prev = NULL;
        } else {
            condvar_node* nextNode = c->back;
            nextNode->prev = newNode;
            c->back = newNode;
            newNode->next = nextNode;
            newNode->prev = NULL;
        }
    c->operation_mutex->unlock();

    queue_mutex->lock();
}