#include "linked_queue.h"
#include <stdio.h>

l_node* new_node(unsigned char* url, unsigned int len)
{
    l_node *r = (struct linked_node*)malloc(sizeof(struct linked_node));
    r->next = NULL;
    if (url != NULL) {
        r->url = url;
        r->url_len = len;
    }
    return r;
}

l_queue init_queue()
{
    l_queue q = (struct linked_queue*)malloc(sizeof(struct linked_queue));
    q->l_head = (struct linked_node*)new_node(NULL, 0);
    q->l_tail = (struct linked_node*)new_node(NULL, 0);
    q->l_tail->next = q->l_head;
    q->l_head->next = q->l_tail;
    q->q_len = 0;
    return q;
}

int is_empty(l_queue q){
    if (q->l_tail->next == q->l_head) {
        return 1;
    }
    return 0;
}

void enqueue(l_queue q, unsigned char* url, unsigned int url_len)
{
    l_node *r = new_node(url, url_len);
    
    if(is_empty(q)){
        q->l_head->next = r;
        q->l_tail->next = r;
    }else{
        q->l_tail->next->next = r;
        q->l_tail->next = r;
    }
    q->q_len++;
}

// free when l_node used in outside
l_node* dequeue(l_queue q)
{
    if(!is_empty(q)){
        l_node *r = q->l_head->next;
        q->l_head->next = r->next;
        if (q->l_head->next == NULL) {
            q->l_tail->next = q->l_head;
            q->l_head->next = q->l_tail;
        }
        q->q_len--;
        return r;
    }
    return NULL;
}

void free_node(l_node* l_n)
{
    if (l_n->url != NULL) {
        free(l_n->url);
        l_n->url = NULL;
    }
    free(l_n);
    l_n = NULL;
}

//call this function when queue is empty
void free_queue(l_queue q)
{
    free_node(q->l_head);
    free_node(q->l_tail);
    free(q);
}

