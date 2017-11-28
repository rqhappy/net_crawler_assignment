#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

int main()
{
	char* so = "/news.sohu.com/nihao/";
	char* so2 = "http://news.sohu.com/nihao/";
	char *str, *tofree, *str2, *tofree2;
	str = tofree = strdup(so);
	str2 = tofree2 = strdup(so2);

	char* token = strsep(&str, "/");
	char* token2 = strsep(&str2, "/");
	printf("token:%s str:%s str2:%s token2:%s\n", token, str, token2, str2);
	printf("token == ""?:%d\n", strcmp(token,""));
	return 0;

}
