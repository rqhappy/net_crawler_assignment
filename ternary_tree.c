#include "ternary_tree.h"

ternary_tree init_ternary_tree()
{
    pthread_mutex_init(&t_tree_lock, NULL);
    t_f = fopen(T_F_PATH, "w");
    ternary_tree root = (ternary_tree)malloc(sizeof(ternary_node));
    root->center=NULL;
    root->left=NULL;
    root->right=NULL;
    root->number = -1;
    return root;
}
void ternary_fclose()
{
    if (t_f != NULL) {
        fclose(t_f);
        t_f = NULL;
    }
}

void free_ternary_tree(ternary_node *node)
{
    if (node->left != NULL) {
        free_ternary_tree(node->left);
    }
    if (node->right != NULL) {
        free_ternary_tree(node->right);
    }
    if (node->center != NULL) {
        free_ternary_tree(node->center);
    }
    free(node);
    node = NULL;
}
long insert(ternary_tree root, unsigned char *str, unsigned long str_len){
    ternary_node *h = root->center;
    ternary_node *pre = root;//始终指向h的前一个节点
    int location = CENTER;
    int i;
    for (i = 0; i < str_len;) {
        if(h == NULL){
            h = (ternary_node*)malloc(sizeof(ternary_node));
            h->ch = str[i];
            h->left = NULL; h->center = NULL; h->right = NULL; h->number = -1;
            if(location == CENTER)
                pre->center = h;
            else if(location == LEFT)
                pre->left = h;
            else
                pre->right = h;
            
            pre = h;
            h = h->center;
            location = CENTER;
            i++;
        } else{
            pre = h;
            if (str[i] == h->ch){
                h = h->center;
                location = CENTER;
                i++;
            }else if (str[i] < h->ch){
                h = h->left;
                location = LEFT;
            }else{
                h = h->right;
                location = RIGHT;
            }
        }
    }
    
    if (pre->number == -1) {
        pre->number = t_url_count;
        fwrite(str, sizeof(unsigned char), str_len, t_f);
        fprintf(t_f, " %lu\n", t_url_count);
        t_url_count++;
    }
    return pre->number;
}
