#include <stdio.h>
#include <sys/socket.h>
#include "net_util.h"
//#include "fsm.h"
//#include "bloom.h"
//#include "hash.h"
#include "thread_pool.h"
#include "thread_work.h"
#include "linked_queue.h"

int main(int argc, const char * argv[]) {
    
    int thread = 4;
    thread_pool_t pool =  thread_pool_create(thread);
    l_queue q= init_queue();
    sock_d* socks =  sock_init("8080", "127.0.0.1");
    var_init();
    struct arguments *arg = (struct arguments*)malloc(sizeof(struct arguments));
    arg->number = thread;
    arg->socks = socks;
    arg->q = q;
    
    for (int i = 0; i < thread; i++) {
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
