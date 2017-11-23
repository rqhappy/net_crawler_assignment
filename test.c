#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
int main()
{
	char* g = "www.google.cn";
	char* so = "news.sohu.com";
	char str[32];
	struct hostent *g_h = gethostbyname(g);
	struct hostent *so_h = gethostbyname(so);

	printf("ipaddr:%s\n", inet_ntop(g_h->h_addrtype, g_h->h_addr_list[0], str, sizeof(str)));
	
	

}
