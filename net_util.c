#include "net_util.h"
int hostname_to_ip(char* hostname, char* ip)
{
    struct hostent* he;
    struct in_addr** addr_list;
    int i;
 
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
 
        herror("gethostbyname");
        return 1;
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
 
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
 
    return 1;
}

int connection(const char * port, const char * address)
{
	int sock=0;
	struct sockaddr_in addr;
	if((sock=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		printf("failed to create socket\n");
		return -1;
	}
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

void send_req(sock_d s, const char* url, CONN_STAT status)
{
	int url_len = (int)strlen(url);
	int req_len = (14 + url_len + ua_len + a_len + conn_len + 1);
	if(status != NULL) req_len += status->host_len + status->refer_len;
	//GET /* HTTP/1.1
	//Host: 
	//Connection: 
	//User-Agent: 
	//Accept:
	//Referer:
	//(\n)
	char *req_line=(char*)malloc(sizeof(char)*req_len);
    strcat(req_line , "GET ");
	strcat(req_line , url);
    strcat(req_line , " HTTP/1.0\n");
	if(status != NULL)
	{
		strcat(req_line , status->host);
		strcat(req_line , status->refer);
	}
	strcat(req_line , conn);
	strcat(req_line , user_agent);
	strcat(req_line , accept_f);
	strcat(req_line , "\n");
    printf("%s",req_line);
	send(s, req_line, req_len, 0);
    char_free(req_line);
}
int analysis_head (sock_d sock)
{
    int i = 0,j = 0;
    char *buf = (char*)malloc(sizeof(char)*500);
    char **head = (char**)malloc(sizeof(char*)*100);
    int len = 0;
    char d[]="'\n',',' ','\r'";
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
    //if ((strcmp(head[1],"200") != 0 || strcmp(head[2],"OK") != 0))
     //   return -1;
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
    printf("len:%d\n",len);
    return len;
}

unsigned char * recv_page(const unsigned long long  len, sock_d sock)
{
    int buf_size = 500;
    char* buf = (char*)malloc(sizeof(char)*buf_size);
	unsigned char* page = (unsigned char*)malloc(sizeof(unsigned char)*len);
    //int i = 0;
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
    /*while(len >= buf_size)
	{
        recv(sock, buf, sizeof(char) * buf_size, 0);
        if(i == 0)
            strncpy(page,buf,buf_size);
        else
            strncat(page,buf,buf_size);
        i++;
        len -= buf_size;
	}
    recv(sock, buf, sizeof(char) * len, 0);
	if(i == 0)
        strncpy(page,buf,len);
    else
        strncat(page,buf,len);
    */
	char_free(buf);
    for(unsigned long long  i = 0;i < len;i++)
       printf("%c",page[i]);
    printf("\n");
	return page;
}
void char_free(char * ptr)
{
    free(ptr);
    if(ptr != NULL)
        ptr = NULL;
}
