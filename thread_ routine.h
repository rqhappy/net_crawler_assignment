#ifndef thread__routine_h
#define thread__routine_h

#define T_COUNT 8
#define SOCK_PRE_T 3
#include <stdio.h>
#include <pthread.h>
#include <sys/select.h>
#include "net_util.h"
#include "linked_queue.h"
#include "fsm.c"
#include "bloom.c"


struct arguments{
    l_queue q;
    sock_d* socks;
    int number;//(form 0 to n)
};
pthread_mutex_t q_lock;
pthread_mutex_t o_lock;
pthread_cond_t q_ready;
CONN_STAT state;
sock_d* sock_init();
void var_init();
void task(void*);


#endif /* thread__routine_h */
