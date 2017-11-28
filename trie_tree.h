#ifndef trie_tree_h
#define trie_tree_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define CHILDREN 256
#define F_PATH "u_to_num.txt"

typedef unsigned char byte;
typedef struct trie_node{
    byte val;
    long number;
    struct trie_node** children;
    
} AC_NODE, *AC_TREE;

static unsigned long url_count = 0;
static FILE* f;
static pthread_mutex_t tree_lock;

AC_TREE tree_init();
void trie_fclose();
long str_to_num(AC_TREE, const unsigned char*, int);

void free_tree(AC_TREE);
#endif /* trie_tree_h */
