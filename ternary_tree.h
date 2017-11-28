#ifndef ternary_tree_h
#define ternary_tree_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define LEFT 0
#define CENTER 1
#define RIGHT 2
#define T_F_PATH "u_to_num.txt"

typedef struct node{
    unsigned char ch;
    long number;
    struct node *left, *center, *right;
}*ternary_tree, ternary_node;

static unsigned long t_url_count = 0;
static FILE* t_f;
static pthread_mutex_t t_tree_lock;

ternary_tree init_ternary_tree();
void ternary_fclose();
void free_ternary_tree(ternary_node *node);
long insert(ternary_tree t_tree, unsigned char *str, unsigned long str_len);

#endif /* ternary_tree_h */
