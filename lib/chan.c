
#include <stdlib.h>
#include "go_concurrency.h"
#include <pthread.h>
#include <stdio.h>


Channel *make_chan() {
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
    Channel *c = malloc(sizeof(Channel));
    c->v = NULL;
    c->closed = 0;
    c->recvq = new_thread_queue();
    c->sendq = new_thread_queue();
    c->lock = lock;
    c->vcond = cond;
    return c;
} 

ThreadQueue *new_thread_queue() {
    ThreadQueue *tq = malloc(sizeof(ThreadQueue));
    tq->cond = NULL;
    tq->next = NULL;
    return tq;
}

void spawn_routine(void*(*start_function) (void *), void *args) {
    pthread_t thread;
    pthread_create(&thread, NULL, start_function, args );
    return; 
}

int select_chan_send(Channel *ch, void *v) {
    pthread_mutex_lock(&(ch->lock));
    int success = try_chan_send(ch, v);
    pthread_mutex_unlock(&(ch->lock));
    return success;
}

int try_chan_send(Channel *ch, void *v) {
    pthread_cond_t *recv_cond = ch->recvq->cond;
    
    if (recv_cond != NULL) {
        // If there is a receiver waiting for a value,
        // drop that receiver from the queue to claim it. 
        ch->recvq = ch->recvq->next;
        if (ch->recvq == NULL) {
            ch->recvq = new_thread_queue();
        }
        // Set the value of this channel to be the 
        // value sent in, then unlock that receiver's
        // thread so they can properly receive it.
        ch->v = v;
        // Wake the waiting receiver up
        pthread_cond_signal(recv_cond);
        return 1;
    };
    return 0;
}

void chan_send(Channel *ch, void *v) {
    pthread_mutex_lock(&(ch->lock));
    if (try_chan_send(ch, v)) {
        pthread_mutex_unlock(&(ch->lock));
        return;
    }
    // If no receiver is waiting for a value,
    // this routine needs to wait until one
    // shows up so it can send v to it and
    // sync up. 

    // This implementation uses a conditional
    // wait until there is an available receiver.
    
    // Go to end of sendq
    ThreadQueue *send_lock = ch->sendq;
    while (send_lock->next != NULL) {
        send_lock = send_lock->next;
    }
    send_lock->next = new_thread_queue();

    pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
    send_lock->cond = &cond;
    
    // cond_wait unlocks, and locks again after it return
    pthread_cond_wait(send_lock->cond,&(ch->lock));
    ch->v = v;
    pthread_cond_signal(&(ch->vcond));
    pthread_mutex_unlock(&(ch->lock));
    return;
}

int select_chan_recv(Channel *ch, void **v) {
    pthread_mutex_lock(&(ch->lock));
    int success = try_chan_recv(ch); 
    if(success) {
        *v = ch->v;
    } 
    pthread_mutex_unlock(&(ch->lock));
    return success;
}

int try_chan_recv(Channel *ch) {
    pthread_cond_t *send_cond = ch->sendq->cond;
    
    if (send_cond != NULL) {
        // If there is a sender waiting,
        // drop that sender from the queue to claim it. 
        ch->sendq = ch->sendq->next;
        if (ch->sendq == NULL) {
            ch->sendq = new_thread_queue();
        }
        pthread_cond_signal(send_cond);
        pthread_cond_wait(&(ch->vcond),&(ch->lock));
        return 1;
    }
    return 0;
}

void *chan_recv(Channel *ch) {
    pthread_mutex_lock(&(ch->lock));
    if (try_chan_recv(ch)) {
        void *ret_v = ch->v;
        pthread_mutex_unlock(&(ch->lock));
        return ret_v;
    }
    
    // Go to end of recvq
    ThreadQueue *recv_lock = ch->recvq;
    while (recv_lock->next != NULL) {
        recv_lock = recv_lock->next;
    }
    recv_lock->next = new_thread_queue();

    pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
    recv_lock->cond = &cond;
    // cond_wait unlocks, and locks again after it returns
    pthread_cond_wait(recv_lock->cond,&(ch->lock));
    void *ret_v = ch->v; 
    pthread_mutex_unlock(&(ch->lock));
    return ret_v;
}