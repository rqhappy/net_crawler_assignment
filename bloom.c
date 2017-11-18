#include "bloom.h" 
#include "hash.h"

BLOOM * bloom_init(size_t size, int n, ...)
{
    BLOOM *bloom;
    va_list l;
    int i;
    
    bloom=malloc(sizeof(BLOOM));
    bloom->a=calloc((size+7)/8, sizeof(unsigned char));
    bloom->funcs=(hashfunc_t*)malloc(n*sizeof(hashfunc_t));

    va_start(l, n);
    for(i = 0; i < n; i++) {
        bloom->funcs[i]=va_arg(l, hashfunc_t);
    }
    va_end(l);

    bloom->n = n;
    bloom->asize=size;
    pthread_mutex_init(&testlock, NULL);
    return bloom;
}

void bloom_destroy(BLOOM *bloom)
{
    free(bloom->a);
    free(bloom->funcs);
    free(bloom);

}

void bloom_set(unsigned char *a,size_t n)
{
    a[n/8] |= (1<<(n%8));
}

unsigned char bloom_get(unsigned char *a,size_t n)
{
	return a[n/8] & (1<<(n%8));
}

int bloom_check(BLOOM *bloom, const char *s)
{
    size_t i;
    int r = 1;
    pthread_mutex_lock(&testlock);
    for(i = 0; i < bloom->n; i++) {
        if(!(bloom_get(bloom->a, bloom->funcs[i](s)%bloom->asize))) 
        {
            r = 0;
            break;
        }
    }
    for(i = 0; i < bloom->n; i++) {
        bloom_set(bloom->a, bloom->funcs[i](s)%bloom->asize);
    }
    pthread_mutex_unlock(&testlock);
    return r;
}


