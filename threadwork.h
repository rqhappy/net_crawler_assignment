#ifndef threadwork_h
#define threadwork_h

#include <pthread.h>
#include <sys/select.h>
#include <stdio.h>
#include "net_util.h"
#include "linked_queue.h"
#include "net_util.h"
#include "fsm.h"
#include "bloom.h"
#include "hash.h"
#include <netdb.h>
#include <unistd.h>

#define T_COUNT 1
#define SOCK_PRE_T 10


typedef struct arguments{
    l_queue q;
    sock_d* socks;
    int number;//(form 0 to n)
    unsigned long long *url_count;
    BLOOM *b;
    char *port;
    int port_len;
    char *host;
    int host_len;
} *ARGS;


extern pthread_mutex_t q_lock;
extern pthread_mutex_t o_lock;
extern pthread_mutex_t c_lock;
extern pthread_cond_t q_ready;
extern CONN_STAT c_state;
extern int max_sock;

sock_d* sock_init(const char*, const char*);

void var_init();

void t_task(void* args);

void send_routine(l_node*, ARGS, int, fd_set*, int*, char**);

#endif /* thread_work_h */
