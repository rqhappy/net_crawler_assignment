#ifndef FSM_H
#define FSM_H

#include<stdio.h>
#include<stdlib.h>

#define URL_SIZE 300
#define URL_ARR_SIZE 1000
//int url_size = 300;
//int url_arr_size = 1000;

int** init_fsm();
char** search_url(unsigned char*, int**, int, int*);
#endif // FSM_H
