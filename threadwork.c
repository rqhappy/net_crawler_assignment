#include "threadwork.h"

pthread_mutex_t q_lock;
pthread_mutex_t o_lock;
pthread_mutex_t c_lock;
pthread_cond_t q_ready;
CONN_STAT c_state;
int max_sock;

sock_d* sock_init(const char* port, const char* address)
{
    sock_d *socks = (sock_d*)malloc(sizeof(sock_d)*SOCK_PRE_T*T_COUNT);
    max_sock = 0;
    c_state = (CONN_STAT)malloc(sizeof(struct conn_state));
    //Host: *.*.*.*\n
    unsigned int l =(unsigned int)strlen(address) + 7;
    char *s =(char*)malloc(sizeof(char)*l+1);
    strcat(s, "Host: ");
    strcat(s, address);
    strcat(s, "\n");
    strcat(s, "\0");
    //printf("host: %s len: %d addr_len:%d\n", s, l, strlen(address));
    c_state->host = s;
    c_state->host_len = l;
    for(int i = 0; i < SOCK_PRE_T*T_COUNT;i++)
    {
        socks[i] = connection(port, address);
        if (socks[i] > max_sock) {
            max_sock = socks[i];
        }
        if (socks[i] == -1) {
            printf("sock_init failed\n");
            return NULL;
        }
    }
    return socks;
}
void var_init()
{
    pthread_mutex_init(&q_lock, NULL);
    pthread_mutex_init(&c_lock, NULL);
    pthread_mutex_init(&o_lock, NULL);
    pthread_cond_init(&q_ready, NULL);
    
}

void send_routine(l_node* n, ARGS arg, int i, fd_set *sets, int* readyfs, char** ho)
{
    char *t_host, *string, *tofree;
    tofree = string = strdup(n->url+1);
    t_host = strsep(&string, "/");
    
    CONN_STAT cs = (CONN_STAT)malloc(sizeof(CONN_STAT));
    if (strcmp(t_host, arg->host) != 1) {
        char str[32];
        struct hostent *so_h  = gethostbyname(t_host);
        if (so_h != NULL) {
            sprintf(str, "%s", inet_ntop(so_h->h_addrtype, so_h->h_addr_list[0], str, sizeof(str)));
            printf("htoip:-----%s-----",str);
            cs->host = host_gen(str);
            cs->host_len = strlen(cs->host);
            
            so_h = NULL;
        }else{
            printf("name server can not parse host:%s\n", t_host);
            free_node(n);
            free(cs);
            free(tofree);
            return;
        }
        
    }else{
        //default host condition
        cs->host = host_gen(arg->host);
        cs->host_len = strlen(cs->host);
    }
    
    printf("thread:%ld sock: %d cs->host:%s str:%s str_len:%lu n->url:%s\n", pthread_self(),
           arg->socks[SOCK_PRE_T * arg->number +i],cs->host, n->url+strlen(t_host)+1, strlen(n->url+strlen(t_host)+1), n->url);
    int state = send_req(arg->socks[SOCK_PRE_T * arg->number + i], n->url+strlen(t_host)+1, cs);
    if (state == 32) {
        printf("send msg with a closed pipe\n");
    }
    FD_SET(arg->socks[arg->number*SOCK_PRE_T+i], sets);
    readyfs[i] = 1;
    if (ho[i] != NULL) {
        free(ho[i]);
    }
    ho[i] = strdup(n->url);
    
    free_node(n);
    free(tofree);
    
    
}
void t_task(void* args)
{
    struct arguments *arg = (struct arguments*)args;
    
    //init ready_for_send array
    int ready_for_send[SOCK_PRE_T];//0 means ready
    for (int i = 0; i < SOCK_PRE_T; i++) {
        ready_for_send[i] = 0;
    }
    //init hosts
    char** hosts = (char**)malloc(sizeof(char*)*SOCK_PRE_T);
    for (int i =0; i < SOCK_PRE_T; i++) {
        hosts[i] = NULL;
    }
    //init fd_set
    fd_set sets;
    FD_ZERO(&sets);
    //init fsm
    int ** fsm = init_fsm();
    //init output file
    char f_name[16];
    sprintf(f_name, "%d.txt",arg->number);
    FILE *f = fopen(f_name, "w");
    
    //init struct timeval
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    
    while(1)
    {
        for (int i = 0; i < SOCK_PRE_T; i++) {
            int skip = 0;
            int sum = 0;
            for (int j = 0; j < SOCK_PRE_T; j++) {
                if (ready_for_send[j] == 1) {
                    //can skip dequeue
                    skip = 1;
                    sum += 1;
                    
                }
            }
            pthread_mutex_lock(&q_lock);
            if ((is_empty(arg->q) && skip) || ready_for_send[i] == 1) {
                //skip dequque
                pthread_mutex_unlock(&q_lock);
            }else{
                while (is_empty(arg->q)) {
                    pthread_cond_wait(&q_ready, &q_lock);
                }
                l_node *n = dequeue(arg->q);
                pthread_mutex_unlock(&q_lock);
                send_routine(n, arg, i, &sets, ready_for_send, hosts);
            }
            int result;
            for (int k = 0; k < SOCK_PRE_T*T_COUNT; k++) {
                if (arg->socks[k] > max_sock) {
                    max_sock = arg->socks[k];
                }
            }
            
            if (sum == SOCK_PRE_T) {
                printf("jam!!\n");
                result = select(max_sock+1, &sets, NULL, NULL, NULL);
            }else{
                result = select(max_sock+1, &sets, NULL, NULL, &timeout);
            }
            for (int j = 0; j < SOCK_PRE_T; j++) {
                //printf("is_set:%d\n", FD_ISSET(arg->socks[arg->number*SOCK_PRE_T+i], &sets));
                
                sock_d c_sock =arg->socks[arg->number*SOCK_PRE_T+j];
                //printf("c_sock:%d\n", c_sock);
                if(FD_ISSET(c_sock, &sets)){
                    //read data
                    ready_for_send[j] = 0;
                    FD_CLR(c_sock, &sets);
                    int err = 0;
                    int len = analysis_h(c_sock, &err);
                    printf("len:%d sock:%d\n", len, c_sock);
                    if(len == 0)
                    {
                        printf("request fail\n");
                    }else{
                        unsigned char * page = recv_page(len, c_sock);
                        int* urls_lenth = (int*)malloc(sizeof(int)*URL_ARR_SIZE);
                        char ** url = search_url(page, fsm, len, urls_lenth);
                        //to be free url[][]
                        
                        for (int k = 0; k < URL_ARR_SIZE; k++) {
                            if (url[k] != NULL) {
                                fwrite(url[k], sizeof(char), urls_lenth[k], f);
                                fwrite(" ", sizeof(char), 1, f);
                                fwrite(hosts[j], sizeof(char),strlen(hosts[j]), f);
                                fwrite("\n", sizeof(char), 1, f);
                                
                                //printf("write %lu bytes to file:%s\n", ws, f_name);
                                fwrite("\n", sizeof(char), 1, f);
                                pthread_mutex_lock(&q_lock);
                                if(bloom_check(arg->b, url[k]) == 0)
                                {
                                    enqueue(arg->q, url[k], urls_lenth[k]);
                                }
                                //printf("enqueue: %s\n", url[k]);
                                pthread_mutex_unlock(&q_lock);
                                pthread_cond_signal(&q_ready);
                            }
                            
                        }
                    }
                    if (err == 1) {
                        printf("reconnect to server!!\n");
                        shutdown(c_sock, SHUT_WR);
                        sock_d s_temp = connection(arg->port, arg->host);
                        arg->socks[arg->number*SOCK_PRE_T+j] = s_temp;
                        
                    }
                    
                    
                    //printf("thread:%ld url[0]:%s", pthread_self(), url[0]);
                    
                }
            }
            // recover the fd_set
            for (int j = 0 ; j < SOCK_PRE_T; j++) {
                if (ready_for_send[j] == 1) {
                    FD_SET(arg->socks[arg->number*SOCK_PRE_T+j], &sets);
                }
            }
            
        }
        
    }
}
