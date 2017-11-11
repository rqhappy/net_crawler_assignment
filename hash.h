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
unsigned int simple_hash(char *str);  

/* RS Hash Function */  
unsigned int RS_hash(char *str);  

/* JS Hash Function */  
unsigned int JS_hash(char *str);  

/* P. J. Weinberger Hash Function */  
unsigned int PJW_hash(char *str);  

/* ELF Hash Function */  
unsigned int ELF_hash(char *str);  

/* BKDR Hash Function */  
unsigned int BKDR_hash(char *str);  

/* SDBM Hash Function */  
unsigned int SDBM_hash(char *str);  

/* DJB Hash Function */  
unsigned int DJB_hash(char *str);  

/* AP Hash Function */  
unsigned int AP_hash(char *str);  

/* CRC Hash Function */  
unsigned int CRC_hash(char *str);  


#endif
