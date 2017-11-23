#include "net_util.h"
int connection(const char * port, const char * address)
{
	int sock=0;
	struct sockaddr_in addr;
	if((sock=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		printf("failed to create socket\n");
		return -1;
	}
    // set non-block model
    //fcntl(sock,F_SETFL,O_NONBLOCK);
    
    bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(address);
	addr.sin_port = htons(atoi(port));

	if(connect(sock, (const struct sockaddr*)&addr, sizeof(struct sockaddr_in))==-1)
	{
		printf("connect error,error:%d ip:%s port:%s\n:", errno, address, port);
		return -1;
	}
	return sock;
}

int send_req(sock_d s, const char* url, CONN_STAT status)
{
	int url_len = (int)strlen(url);
	int req_len = (14 + url_len + ua_len + conn_len + 1);
	req_len += status->host_len;
	//GET / HTTP/1.1
	//Host: 
	//Connection: 
	//User-Agent:
	//(\n)
	char *req_line=(char*)calloc(req_len, sizeof(char));
    //memset(req_line, 0, req_len*sizeof(char));
    int index = 0;
    strncpy(req_line+index, "GET ", 4);
    index += 4;
    strncpy(req_line+index, url, url_len);
    index += url_len;
    strncpy(req_line+index, " HTTP/1.1\n", 10);
    index += 10;
    strncpy(req_line+index, conn, conn_len);
    index += conn_len;
    strncpy(req_line+index, status->host, status->host_len);
    index += status->host_len;
    strncpy(req_line+index, user_agent, ua_len);
    index += ua_len;
    strncpy(req_line+index, "\n", 1);
    
    printf("sock: %d, %s %lu %lu %d", s, req_line, status->host_len, strlen(req_line), req_len);
	ssize_t send_byte = send(s, req_line, req_len, 0);
    if (send_byte == -1) {
        printf("send error:%d\n", errno);
        return errno;
    }else{
        printf("send %lu bytes\n", send_byte);
    }
    free_conn_state(status);
    char_free(req_line);
    
    return 0;
}


unsigned char * recv_page(const unsigned long long  len, sock_d sock)
{
    int buf_size = 500;
    char* buf = (char*)malloc(sizeof(char)*buf_size);
	unsigned char* page = (unsigned char*)malloc(sizeof(unsigned char)*len);
    unsigned long long l = len;
	
	unsigned long long count = 0;
	unsigned long long  pos = 0;
	while(l > 0)
	{
		count = l > (unsigned long long)buf_size ? buf_size : l;
		recv(sock, buf, sizeof(unsigned char)*count, 0);
		strncpy((char*)page+pos, buf, count);
		pos += count;
		l -= count;
	}
    printf("*************count:%llu, pos:%llu, len:%llu l:%llu\n", count, pos, len, l);
	char_free(buf);
	return page;
}

int analysis_h(sock_d sock, int *err)
{
    char *aline = (char*)malloc(sizeof(char)*300);
    int page_len = 0;
    int line_len = 0;
    int flag = 0;//1 means (200 ok)
    //analysis response hand state
    printf("recv sock:%d\n", sock);
    while (recv(sock, aline+line_len, sizeof(char), 0) > 0) {
        printf("%c", aline[line_len]);
        line_len++;
        if (aline[line_len-1] == '\r') {
            line_len--;
        }
        if (aline[line_len-1] == '\n') {
            aline[line_len-1] = '\0';
            char* dup, *tofree;
            dup = tofree = strdup(aline);
            strsep(&dup, " ");
            if (dup == NULL) {
                printf("what happened????\n");
                continue;
            }
            if (strcmp(dup, "200 OK") == 0) {
                flag = 1;
            }
            free(tofree);
            line_len = 0;
            break;
        }
    }
    //analysis response hand body
    while (recv(sock, aline+line_len, sizeof(char), 0) > 0) {
        //printf("%c", aline[line_len]);
        line_len++;
        if (aline[line_len-1] == '\r') {
            line_len--;
        }else if (aline[line_len-1] == '\n'){
            if (line_len == 1) {
                break;
            }
            aline[line_len-1] = '\0';
            char *dup, *tofree;
            dup = tofree = strdup(aline);
            char* token = strsep(&dup, " ");
            if (strcmp(token, "Content-Length:") == 0) {
                //get page_length
                page_len = atoi(dup);
            }
            if (strcmp(token, "Connection:") == 0 && strcmp(dup, "close") == 0) {
                //remote server close the connection
                *err = 1;
            }
            free(tofree);
            line_len = 0;
        }
    }
    
    if (flag == 0) {
        recv_page(page_len, sock);
        page_len = 0;
    }
    free(aline);
    return page_len ;
}

int analysis_head (sock_d sock)
{
    int i = 0,j = 0;
    char *buf = (char*)malloc(sizeof(char)*1000);
    char **head = (char**)malloc(sizeof(char*)*100);
    int len = 0;
    char d[]="'\n','\'',' ','\r'";
    printf("start analysis\n");
    while(recv(sock, buf + i, sizeof(char), 0) > 0)
    {
        printf("%c",buf[i]);
        if(i > 2)
            if(buf[i] == '\n' && buf[i-2] == '\n')
            {
                j = 0;
                head[j] = strsep(&buf,d);
                while(head[j] != NULL)
                {
                    j++;
                    head[j] = strsep(&buf,d);
                }
                break;
            }
        i++;
    }
    /*if ((strcmp(head[1],"200") != 0 || strcmp(head[2],"OK") != 0))
        return -1;*/
    i = 0;
    while((strcmp(head[i],"Content-Length:"))!=0)
        i++;
    int n = 0,add = 0,p = 0;
    while(head[i+1][n]!='\0')
    {
        n++;
    }
    for(p = n;p > 0;p--)
    {
        add = (int)head[i+1][n-p]-48;
        for(int m = 0;m < p-1;m++)
            add *= 10;
        len += add;
    }
    for(i = 0;i < j;i++)
        head[i] = NULL;
    char_free(buf);
    free(head);
    if(head != NULL)
        head = NULL;
    //printf("len:%d\n",len);
    
    if ((strcmp(head[1],"200") != 0 || strcmp(head[2],"OK") != 0))
    {
        recv_page(len, sock);
        return -1;
    }
    return len;
}
char* host_gen(char* host)
{
    unsigned int l =(unsigned int)strlen(host) + 8;
    char *s =(char*)calloc(l, sizeof(char));
    int index = 0;
    strncpy(s+index, "Host: ", 6);
    index += 6;
    strncpy(s+index, host, strlen(host));
    index += strlen(host);
    strncpy(s+index, "\n", 1);
    
    return s;
}
void free_conn_state(CONN_STAT cs)
{
    free(cs->host);
    free(cs);
    cs = NULL;
}
void char_free(char * ptr)
{
    free(ptr);
    if(ptr != NULL)
        ptr = NULL;
}
