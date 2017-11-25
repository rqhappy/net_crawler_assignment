#include"fsm.h"
#include"net_util.h"

int** init_fsm()
{
    int states = 255;
    //printf("start init.");
    int i;
    int ** fsm = (int**)malloc(sizeof(int*)*11);
    for(i = 0;i <= 10;i++)
        fsm[i] = (int*)malloc(sizeof(int)*(states+1));
    for(i = 0;i <= states;i++)
        fsm[0][i] = 0;
    fsm[0]['<']=1;

    for(i = 0;i <= states;i++)
        fsm[1][i] = 0;
    fsm[1]['a'] = 2;

    for(i = 0;i <= states;i++)
        fsm[2][i] = 2;
    fsm[2]['h'] = 3;
    fsm[2]['>'] = 0;

    for(i = 0;i <= states;i++)
        fsm[3][i] = 2;
    fsm[3]['r'] = 4;
    fsm[3]['>'] = 0;

    for(i = 0;i <= states;i++)
        fsm[4][i]=2;
    fsm[4]['e'] = 5;
    fsm[4]['>'] = 0;

    for(i = 0;i <= states;i++)
        fsm[5][i] = 2;
    fsm[5]['f']=6;
    fsm[5]['>']=0;

    for(i = 0;i <= states;i++)
        fsm[6][i] = 2;
    fsm[6]['='] = 7;
    fsm[6]['>'] = 0;

    for(i=0;i <= states;i++)
        fsm[7][i] = 0;
    fsm[7]['"']=10;
    fsm[7][' ']=7;

    for(i=0;i <= states;i++)
        fsm[10][i] = 10;
    fsm[10]['/'] = 8;
    fsm[10]['"'] = 0;
    fsm[10]['>'] = 0;
    fsm[10]['#'] = 0;

    for(i=0;i <= states;i++)
        fsm[8][i] = 8;
    fsm[8]['>'] = 0;
    fsm[8]['"'] = 9;

    for(i=0;i <= states;i++)
        fsm[9][i] = 0;
    //printf("init over.");
    return fsm;
}

char** search_url(unsigned char* page,int** fsm,int len, int* urls_lenth)
{
    int now = 0;
    int l = 0;
    int num = 0;
    int ptr = 0;
    char** urls = (char**)malloc(sizeof(char*)*URL_ARR_SIZE);
    for (int i = 0; i < URL_ARR_SIZE; i++) {
        urls[i] = NULL;
    }
    unsigned char* buf = (unsigned char*)malloc(sizeof(unsigned char)*URL_SIZE);
    while(ptr < len)
    {
        if(now == 8)
        {
            l = 0;
            while(ptr < len && page[ptr] !='"' && page[ptr] != '>')
            {
                if (page[ptr] != '\n') {
                    buf[l] = page[ptr];
                    l++;
                }
                ptr++;
            }
        }
        now=fsm[now][page[ptr]];
        if(now == 9)
        {
            if(num > URL_ARR_SIZE-1)
            {
                printf("too many url to store, num:%d\n", num);
                break;
            }
            urls[num] = (char*)malloc(sizeof(char)*URL_SIZE);
            memcpy(urls[num], buf, l);
            memcpy(urls[num]+l, "\0", 1);
            
            //strncpy(urls[num], buf, l);
            //strncpy(urls[num]+l, "\0", 1);
            urls_lenth[num] = l;
            num++;
            
        }
        ptr++;
    }
    free(page);
    page = NULL;
    free(buf);
    buf = NULL;
    return urls;
}




