#ifndef thread_work_h
#define thread_work_h

#include <pthread.h>
#include <sys/select.h>
#include "net_util.h"
#include "linked_queue.h"
#include "net_util.h"
#include "fsm.h"
#include "bloom.h"
#include "hash.h"

#define T_COUNT 4
#define SOCK_PRE_T 3


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
void t_task(void*);


#endif /* thread_work_h */
