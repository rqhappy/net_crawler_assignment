#ifndef _NET_UTIL_H_
#define _NET_UTIL_H_

typedef int sock_d;
typedef struct conn_state{
		char* host;
		unsigned long host_len;
		char* refer;
		unsigned long refer_len;

} *CONN_STAT;


#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

sock_d connection(const char*, const char*);
void send_req(sock_d, const char*, CONN_STAT);
int hostname_to_ip(char* , char*);
int analysis_head(sock_d);
unsigned char *recv_page(const unsigned long long, sock_d);
void char_free(char *);

static char* user_agent = "User-Agent: Spider\n";
static int ua_len = 19;
static char* accept_f = "Accept: text/html\n";
static int a_len = 18;
static char* conn = "Connection: keep-alive\n";
static int conn_len = 23;

#endif
