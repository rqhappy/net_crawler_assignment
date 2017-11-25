#include "trie_tree.h"

AC_TREE tree_init()
{
    pthread_mutex_init(&tree_lock, NULL);
    AC_TREE root = (AC_TREE)malloc(sizeof(AC_NODE));
    root->number = 0;
    root->val = 0x00;
    root->children = (AC_NODE**)malloc(sizeof(AC_NODE*)*CHILDREN);
    for (int i = 0; i < CHILDREN; i++) {
        root->children[i] = NULL;
    }
    return root;
}

unsigned long str_to_num(AC_TREE root, const char* str, int str_len)
{
    pthread_mutex_lock(&tree_lock);
    AC_NODE *temp = root;
    for (int i = 0; i < str_len; i++) {
        if (temp->children[str[i]] == NULL) {
            AC_NODE *new_node = (AC_NODE*)malloc(sizeof(AC_NODE));
            new_node->val = str[i];
            new_node->children = (AC_NODE**)malloc(sizeof(AC_NODE*)*CHILDREN);
            new_node->number = -1;
            for (int i = 0; i < CHILDREN; i++) {
                new_node->children[i] = NULL;
            }
            temp->children[str[i]] = new_node;
        }
        temp = temp->children[str[i]];
    }
    if (temp->number == -1) {
        
        temp->number = url_count;
        url_count++;
    }
    pthread_mutex_unlock(&tree_lock);
    return temp->number;
}

void free_tree(AC_NODE *node)
{
    for (int i = 0; i <CHILDREN ; i++) {
        if (node->children[i] !=NULL) {
            free(node->children);
            node->children[i] = NULL;
        }
    }
    free(node);
    node = NULL;
}
