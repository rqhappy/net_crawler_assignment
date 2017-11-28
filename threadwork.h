#ifndef threadwork_h
#define threadwork_h

#include <pthread.h>
#include <sys/select.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include "net_util.h"
#include "linked_queue.h"
#include "net_util.h"
#include "fsm.h"
#include "bloom.h"
#include "hash.h"
//#include "trie_tree.h"
#include "ternary_tree.h"


#define T_COUNT 5
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
    ternary_tree ternary_t;
    //AC_TREE ac_t;
} *ARGS;

extern pthread_cond_t main_ready;
extern pthread_mutex_t main_mutex;
extern pthread_mutex_t q_lock;
extern pthread_mutex_t o_lock;
extern pthread_mutex_t c_lock;
extern pthread_cond_t q_ready;
extern CONN_STAT c_state;
extern int max_sock;
extern unsigned char* wait_bit;


sock_d* sock_init(const char*, const char*);

void var_init();

void t_task(void* args);

int check_bit();

void send_routine(l_node*, ARGS, int, fd_set*, int*, unsigned char**);

void combine_files();

#endif /* thread_work_h */
