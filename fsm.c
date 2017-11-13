#include"fsm.h"
#include"net_util.h"

int** init_fsm()
{
    int states = 255;
    printf("start init.");
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
    printf("init over.");
    return fsm;
}

char** search_url(unsigned char* page,int** fsm,int len)
{
    int now = 0;
    int l = 0;
    int num = 0;
    int ptr = 0;
    printf("start search.\n\n\n\n");
    printf("%d\n",len);
    int url_size = 300;
    int url_arr_size = 1000;
    char** urls = (char**)malloc(sizeof(char*)*url_arr_size);
    int* urls_lenth = (int*)malloc(sizeof(int)*url_arr_size);
    char* buf = (char*)malloc(sizeof(char)*url_size);
    while(ptr < len)
    {
        if(now == 8)
        {
            l = 0;
            while(ptr < len && page[ptr] !='"' && page[ptr] != '>')
            {
                buf[l] = page[ptr];
                l++;
                ptr++;
            }
        }
        now=fsm[now][page[ptr]];
        //printf("%c",page[ptr]);
        //printf("%d\n",now);
        if(now == 9)
        {
            if(num > url_arr_size-1)
            {
                printf("too many url to store, num:%d\n", num);
                break;
            }
            urls[num] = (char*)malloc(sizeof(char)*url_size);
            strncpy(urls[num], buf, l+1);
            urls_lenth[num] = l+1;
            num++;
            //for(int i = 0;i < l;i++)
            //{
            //    urls[num][i] = buf[i];
            //}
            
        }
        ptr++;
        //printf("%d",ptr);
    }
    printf("search over\n");
    printf("%d\n",num);
//    for(int i = 0;i < num;i++)
//        printf("%s\n",url[i]);
//    for(int i = 0;i < num;i++)
//        char_free(url[i]);
//    free(url);
//    if(url != NULL)
//        url = NULL;
    return urls;
}




