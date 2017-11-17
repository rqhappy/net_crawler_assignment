#include "thread_ routine.h"

sock_d* sock_init(const char* port, const char* address)
{
    sock_d *socks = (sock_d*)malloc(sizeof(sock_d)*SOCK_PRE_T*T_COUNT);
    state = (CONN_STAT)malloc(sizeof(struct conn_state));
    //Host: *.*.*.*\n
    unsigned int l =(unsigned int)strlen(address) + 7;
    char *s =(char*)malloc(sizeof(char)*l);
    strncpy(s, "Host: ", 6);
    strncpy(s+6, address, l-7);
    strncpy(s+l-1, "\n", 1);
    state->host = s;
    state->host_len = l;
    for(int i = 0; i < SOCK_PRE_T*T_COUNT;i++)
    {
        socks[i] = connection(address, port);
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
    pthread_mutex_init(&o_lock, NULL);
    pthread_cond_init(&q_ready, NULL);
}
void task(void* args)
{
    struct arguments *arg = (struct arguments*)args;
    
    //init ready_for_send array
    int ready_for_send[SOCK_PRE_T];//0 means ready
    memset(ready_for_send, SOCK_PRE_T, sizeof(int));
    //init hosts
    char* hosts[SOCK_PRE_T];
    //init fd_set
    fd_set sets;
    FD_ZERO(&sets);
    for (int i = 0; i < SOCK_PRE_T; i++) {
        FD_SET(arg->socks[arg->number*SOCK_PRE_T+i], &sets);
    }
    
    //init struct timeval
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    
    //init bloom
    BLOOM *bloom = bloom_init(bloom_size,10,simple_hash,RS_hash,JS_hash,
                              PJW_hash,ELF_hash,BKDR_hash,SDBM_hash,DJB_hash,AP_hash,CRC_hash);
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
                char url[n->url_len];
                strncpy(url, n->url, n->url_len);
                free(n);
                
                int temp = SOCK_PRE_T * arg->number + i;
                send_req(arg->socks[temp], url, state);
                ready_for_send[i] = 1;
                hosts[i] = url;
            }
            int result;
            if (sum == SOCK_PRE_T) {
                result = select(FD_SETSIZE, &sets, NULL, NULL, NULL);
            }else{
                result = select(FD_SETSIZE, &sets, NULL, NULL, &timeout);
            }
            
            for (int j = 0; j < SOCK_PRE_T; j++) {
                if(FD_ISSET(arg->socks[arg->number*SOCK_PRE_T+i], &sets)){
                    //read data
                    int len = analysis_head(arg->socks[arg->number*SOCK_PRE_T+i]);
                    if(len == -1)
                    {
                        printf("request fail");
                    }
                    unsigned char * page = recv_page(len, arg->socks[arg->number*SOCK_PRE_T+i]);
                    int ** fsm = init_fsm();
                    char ** url = search_url(page,fsm,len);
                    
                    
                }
            }
            
            
        }
        
    }
}
