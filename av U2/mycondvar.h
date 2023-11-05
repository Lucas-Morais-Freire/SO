#ifndef MYCONDVAR_H
#define MYCONDVAR_H

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

struct condvar_node {
    pthread_mutex_t priv_mutex;
    struct condvar_node* prev;
};

struct condvar {
    struct condvar_node* front;
    struct condvar_node* back;
    pthread_mutex_t operation_mutex;
};

void condvar_init(struct condvar* c) {
    c->back = NULL;
    c->front = NULL;
    pthread_mutex_init(&c->operation_mutex, NULL);
}

void condvar_wait(struct condvar* c, pthread_mutex_t* m) {
    // bloquear outras threads de modificar a fila.
    pthread_mutex_lock(&c->operation_mutex);

    // criar um mutex privado para travar esta thread.
    struct condvar_node new_node;
    pthread_mutex_init(&new_node.priv_mutex, NULL);
    pthread_mutex_lock(&new_node.priv_mutex);

    new_node.prev = NULL; // ele entra em ultimo na fila, entao ele nao tem anterior.
    if (c->back != NULL) {
        c->back->prev = &new_node; // se ha um no na traseira da fila, colocar este novo no atras dele.
    } else {
        c->front = &new_node; // se nao ha, colocar o novo no na frente.
    }
    c->back = &new_node; // o ponteiro da traseira apontara para o novo no em ambos os casos.

    // agora que o novo no ja esta na fila, permitir que outras threads tentem colocar mais nos ou retira-los.
    pthread_mutex_unlock(m);

    // permitir edicao da fila
    pthread_mutex_unlock(&c->operation_mutex);

    // colocar a thread para dormir.
    pthread_mutex_lock(&new_node.priv_mutex);

    // ao acordar, tentar readquirir o mutex de sincronizacao.
    pthread_mutex_unlock(&new_node.priv_mutex);
    pthread_mutex_lock(m);
}

void condvar_signal(struct condvar* c) {
    // bloquear outras threads de modificarem a fila.
    pthread_mutex_lock(&c->operation_mutex);

    // checar se ha alguma thread dormindo ou prestes a dormir na fila
    if (c->front != NULL) {
        struct condvar_node* releasing = c->front;
        // mover a frente da fila para a proxima thread
        c->front = releasing->prev;
        // se nao houver proxima thread, impedir que c->back aponte para o noh removido.
		if (c->front == NULL) {
			c->back = NULL;
		}
        // desbloquear a thread que foi removida da fila.
        pthread_mutex_unlock(&releasing->priv_mutex);
    }

    pthread_mutex_unlock(&c->operation_mutex);

}

void condvar_broadcast(struct condvar* c) {
    // bloquear outras threads de modificarem a fila.
    pthread_mutex_lock(&c->operation_mutex);

    // enquanto a fila nao estiver vazia...
    struct condvar_node* releasing = c->front;
    while (c->front != NULL) {
        // mover a frente da fila para a proxima thread
        c->front = releasing->prev;
        // se nao houver proxima thread, impedir que c->back aponte para o noh removido.
		if (c->front == NULL) {
			c->back = NULL;
		}
        // desbloquear a thread que foi removida da fila.
        pthread_mutex_unlock(&releasing->priv_mutex);
        // tentar remover o proximo elemento da fila
        releasing = c->front;
    }

    pthread_mutex_unlock(&c->operation_mutex);
}

#endif