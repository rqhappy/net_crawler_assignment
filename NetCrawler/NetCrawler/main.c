#include <stdio.h>
#include <sys/socket.h>
#include <signal.h>
#include "net_util.h"
#include "thread_pool.h"
#include "threadwork.h"
#include "linked_queue.h"

int main(int argc, const char * argv[]) {
    
    char* addr = "127.0.0.1";
    char* port = "8080";
    
    signal(SIGPIPE,SIG_IGN);
    thread_pool_t pool =  thread_pool_create(T_COUNT);
    l_queue q= init_queue();
    char *index = (char*)malloc(sizeof(char)*18);
    strcat(index, "/");
    strcat(index, addr);
    strcat(index, "/");
    //strncpy(index, "/127.0.0.1/", 11);
    index[strlen(addr)+2] = '\0';
    enqueue(q, index, (int)strlen(index));
    sock_d* socks =  sock_init(port, addr);
    var_init();
    //init bloom
    BLOOM *bloom = bloom_init(bloom_size,10,simple_hash,RS_hash,JS_hash,
                              PJW_hash,ELF_hash,BKDR_hash,SDBM_hash,DJB_hash,AP_hash,CRC_hash);
    unsigned long long *u_count =(unsigned long long*)malloc(sizeof(unsigned long long));
    u_count[0] = 1;
    for (int i = 0; i < T_COUNT; i++) {
        struct arguments *arg = (struct arguments*)malloc(sizeof(struct arguments));
        arg->socks = socks;
        arg->q = q;
        arg->url_count = u_count;
        arg->b = bloom;
        arg->number = i;
        arg->port = port;
        arg->port_len = (int)strlen(port);
        arg->host = addr;
        arg->host_len = (int)strlen(addr);
        thread_pool_add_task(pool, (void*)t_task, (void*)arg);
    }
    puts("press enter to terminate ...");
    getchar();
     
     
     
     
     
     
     
    /*
     // insert code here...
     if(argc < 4)
     {
     printf("Usage: crawler (ip_address) (port) (url.txt)\n");
     return 0;
     }:
     //char ip[50];
     //hostname_to_ip(argv[2],ip);
     sock_d sock = connection("8080","127.0.0.1");
     CONN_STAT index_state = (CONN_STAT)malloc(sizeof(CONN_STAT));
     index_state->host = "Host: 127.0.0.1\n";
     index_state->host_len = strlen(index_state->host);
     index_state->refer = NULL;
     index_state->refer_len = 0;
     send_req(sock, "/", index_state);
     int len = analysis_head(sock);
     if(len == -1)
     {
     printf("request fail");
     }
     BLOOM *bloom = bloom_init(bloom_size,10,simple_hash,RS_hash,JS_hash,
     PJW_hash,ELF_hash,BKDR_hash,SDBM_hash,DJB_hash,AP_hash,CRC_hash);
     printf("init over\n");
     unsigned char * page = recv_page(len, sock);
     printf("recv_page ok\n");
     //char page[]="<a href=\"/adasdafasdsffggfsd\"><a href=\"/adassdfsfdsffggfsd\"><a href=\"/adasdffsdgsffggfsd\"><a href=\"/adasdafasdsffggfsd\"><a href=\"/adassfggsdsffggfsd\">";
     int ** fsm = init_fsm();
     char ** url = search_url(page,fsm,len);
     int i = 0;
     int n = 0;
     while(url[i] != NULL)
     {
     if(bloom_check(bloom,url[i]) == 0)
     {
     n++;
     //入队
     }
     i++;
     }
     printf("%d\n",n);
     bloom_destroy(bloom);
     printf("destory over\n");
     //BLOOM *b = bloom_init(BLOOM_SIZE, 10, hash_list);
     //shutdown(sock, SHUT_RDWR);
     */
    return 0;
}
