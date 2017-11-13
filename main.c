#include <stdio.h>
#include <sys/socket.h>
#include "net_util.h"
#include "fsm.h"
#include "bloom.h"
#include "hash.h"

#include <stdio.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    /*if(argc < 4)
     {
     printf("Usage: crawler (ip_address) (port) (url.txt)\n");
     return 0;
     }:*/
    //char ip[50];
    //hostname_to_ip(argv[2],ip);
    sock_d sock = connection("8080","127.0.0.1");
    send_req(sock, "/", NULL);
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
    return 0;
}
