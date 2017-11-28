#ifndef _HASH_H  
#define _HASH_H  

static char* hash_list[10]= {
    "simple_hash",
    "RS_hash",
    "JS_hash",
    "PJW_hash",
    "ELF_hash",
    "BKDR_hash",
    "SDBM_hash",
    "DJB_hash",
    "AP_hash",
    "CRC_hash"};
/* A Simple Hash Function */  
unsigned int simple_hash(unsigned char *str);

/* RS Hash Function */  
unsigned int RS_hash(unsigned char *str);

/* JS Hash Function */  
unsigned int JS_hash(unsigned char *str);

/* P. J. Weinberger Hash Function */  
unsigned int PJW_hash(unsigned char *str);

/* ELF Hash Function */  
unsigned int ELF_hash(unsigned char *str);

/* BKDR Hash Function */  
unsigned int BKDR_hash(unsigned char *str);

/* SDBM Hash Function */  
unsigned int SDBM_hash(unsigned char *str);

/* DJB Hash Function */  
unsigned int DJB_hash(unsigned char *str);

/* AP Hash Function */  
unsigned int AP_hash(unsigned char *str);

/* CRC Hash Function */  
unsigned int CRC_hash(unsigned char *str);  


#endif
