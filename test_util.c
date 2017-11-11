#include <stdio.h>
#include <sys/socket.h>
#include "net_util.h"
#include "fsm.h"
#include "bloom.h"
#include "hash.h"
//int main(int argc, const char* argv[])
//{
//    /*if(argc < 4)
//	{
//		printf("Usage: crawler (ip_address) (port) (url.txt)\n");
//		return 0;
//    }:*/
//    char ip[50];
//    //hostname_to_ip(argv[2],ip);
//    sock_d sock = connection("80","10.108.86.80");
//	send_req(sock, "/", NULL);
//    int len = analysis_head(sock);
//    if(len == -1)
//    {
//        printf("request fail");
//    }
//    BLOOM *bloom = bloom_init(50000,1,ELF_hash);
//    printf("init over\n");
//    bloom_destroy(bloom);
//    printf("destory over\n");
//    char * page = recv_page(len, sock);
//	printf("recv_page ok\n");
//    //char page[]="<a href=\"/adasdafasdsffggfsd\"><a href=\"/adassdfsfdsffggfsd\"><a href=\"/adasdffsdgsffggfsd\"><a href=\"/adasdafasdsffggfsd\"><a href=\"/adassfggsdsffggfsd\">";
//    char ** fsm = init_fsm();
//    char ** url = search_url(page,fsm,len);
//    //shutdown(sock, SHUT_RDWR);
//	return 0;
//}
