#include"hash.h"
#include<string.h>

unsigned int simple_hash(char *str)  
{  
    register unsigned int hash;  
    register unsigned char *p;  

    for(hash = 0, p = (unsigned char *)str; *p ; p++)  
        hash = 31 * hash + *p;  

    return (hash & 0x7FFFFFFF);  
}  

/* RS Hash Function */  
unsigned int RS_hash(char *str)  
{  
         unsigned int b = 378551;  
         unsigned int a = 63689;  
         unsigned int hash = 0;  

         while (*str)  
         {  
                 hash = hash * a + (*str++);  
                 a *= b;  
         }  

         return (hash & 0x7FFFFFFF);  
}  

/* JS Hash Function */  
unsigned int JS_hash(char *str)  
{  
         unsigned int hash = 1315423911;  

         while (*str)  
         {  
                 hash ^= ((hash << 5) + (*str++) + (hash >> 2));  
         }  

         return (hash & 0x7FFFFFFF);  
}  

/* P. J. Weinberger Hash Function */  
unsigned int PJW_hash(char *str)  
{  
         unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);  
         unsigned int ThreeQuarters     = (unsigned int)((BitsInUnignedInt   * 3) / 4);  
         unsigned int OneEighth         = (unsigned int)(BitsInUnignedInt / 8);  

         unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);  
         unsigned int hash              = 0;  
         unsigned int test              = 0;  

         while (*str)  
         {  
                 hash = (hash << OneEighth) + (*str++);  
                 if ((test = hash & HighBits) != 0)  
                 {  
                         hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));  
                 }  
         }  

         return (hash & 0x7FFFFFFF);  
}  

/* ELF Hash Function */  
unsigned int ELF_hash(char *str)  
{  
         unsigned int hash = 0;  
         unsigned int x     = 0;  

         while (*str)  
         {  
                 hash = (hash << 4) + (*str++);  
                 if ((x = hash & 0xF0000000L) != 0)  
                 {  
                         hash ^= (x >> 24);  
                         hash &= ~x;  
                 }  
         }  

         return (hash & 0x7FFFFFFF);  
}  

/* BKDR Hash Function */  
unsigned int BKDR_hash(char *str)  
{  
         unsigned int seed = 131; // 31 131 1313 13131 131313 etc..  
         unsigned int hash = 0;  

         while (*str)  
         {  
                 hash = hash * seed + (*str++);  
         }  

         return (hash & 0x7FFFFFFF);  
}  

/* SDBM Hash Function */  
unsigned int SDBM_hash(char *str)  
{  
         unsigned int hash = 0;  

         while (*str)  
         {  
                 hash = (*str++) + (hash << 6) + (hash << 16) - hash;  
         }  

         return (hash & 0x7FFFFFFF);  
}  

/* DJB Hash Function */  
unsigned int DJB_hash(char *str)  
{  
         unsigned int hash = 5381;  

         while (*str)  
         {  
                 hash += (hash << 5) + (*str++);  
         }  

         return (hash & 0x7FFFFFFF);  
}  

/* AP Hash Function */  
unsigned int AP_hash(char *str)  
{  
         unsigned int hash = 0;  
         int i;  
         for (i=0; *str; i++)  
         {  
                 if ((i & 1) == 0)  
                 {  
                         hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));  
                 }  
                 else  
                 {  
                         hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));  
                 }  
         }  

         return (hash & 0x7FFFFFFF);  
}  

unsigned int CRC_hash(char *str)  
{  
    unsigned int        nleft   = (unsigned int)strlen(str);
    unsigned long long  sum     = 0;  
    unsigned short int *w       = (unsigned short int *)str;  
    unsigned short int  answer  = 0;  
 
    while ( nleft > 1 ) {  
        sum += *w++;  
        nleft -= 2;  
    }   
    if ( 1 == nleft ) {  
        *( unsigned char * )( &answer ) = *( unsigned char * )w ;  
        sum += answer;  
    }
    sum = ( sum >> 16 ) + ( sum & 0xFFFF );   
    sum += ( sum >> 16 );   
    answer = ~sum;  
    return (answer & 0xFFFFFFFF);  
}
