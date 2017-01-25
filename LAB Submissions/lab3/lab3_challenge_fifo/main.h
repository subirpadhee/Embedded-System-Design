#ifndef main_h
#define main_h

/* File includes */
#include <stdio.h>
#include <stdlib.h>
#include <at89c51ed2.h>
#include <mcs51reg.h>
#include <mcs51/8051.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>


/* Function Prototypes*/
#ifndef POLLING
void receive_interupt() __interrupt(4);
#endif
void putstring(char * str);
void putcharac(char c);
unsigned char getstring(char * str);
char getcharac();

/* Defines */
#define SIZE 10
//#define POLLING

/* Macros */

/* Externs*/
extern unsigned char FIFO[SIZE];
extern unsigned char *rd;
extern unsigned char *wr;
extern volatile unsigned char full;
extern volatile unsigned char round;

#endif // main
