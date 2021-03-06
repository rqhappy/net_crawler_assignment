#ifndef linked_queue_h
#define linked_queue_h

#include <stdlib.h>

typedef struct linked_node{
    struct linked_node* next;
    unsigned char* url;
    unsigned int url_len;
} l_node;

typedef struct linked_queue{
    struct linked_node *l_head, *l_tail;
    unsigned long q_len;
    
} *l_queue;

l_node* new_node(unsigned char*, unsigned int);
l_queue init_queue();
void free_node(l_node*);
void enqueue(l_queue, unsigned char*, unsigned int);
l_node* dequeue(l_queue);
int is_empty(l_queue);
void free_queue(l_queue);
#endif /* linked_queue_h */
