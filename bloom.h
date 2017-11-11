#ifndef _BLOOM_H  
#define _BLOOM_H 

#include<stdio.h>
#include<string.h>
#include<limits.h>
#include<stdarg.h>
#include<stdlib.h>
#define bloom_size 50000
typedef unsigned int (*hashfunc_t)(const char *);
typedef struct {
    size_t asize;
    unsigned char *a;
    int n;
    hashfunc_t *funcs;
} BLOOM;
BLOOM * bloom_init(size_t size, int n, ...);
void bloom_destroy(BLOOM *bloom);
void bloom_set(unsigned char *a,size_t n);
unsigned char bloom_get(unsigned char *a,size_t n);
void bloom_add(BLOOM *bloom, const char *s);
int bloom_check(BLOOM *bloom, const char *s);


#endif
