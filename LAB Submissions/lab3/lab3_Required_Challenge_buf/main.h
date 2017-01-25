#ifndef main_h
#define main_h

/* File includes */
#include <stdio.h>
#include <stdlib.h>
#include <at89c51ed2.h>
#include <mcs51reg.h>
#include <mcs51/8051.h>
#include <string.h>
#include <time.h>

/* Function Prototypes*/
void putcharac(char c);
char getcharac();
void putstring(char * str);
unsigned char getstring(char * str);
void error(unsigned char code);
char menu_buffer();
unsigned char checkInput(unsigned char *buffer_size, unsigned char ch);
void menu_rest();
char checkChar(char ch, char type);
void printBuffer(unsigned int sz, char buffer);
void justprintBuffer(unsigned char sz, char buffer);
void dataout(unsigned int address, unsigned char value );


/* Defines */
#define HEAP_SIZE 0x7D0   // 0x7D0 is 2000 bytes
#define STORAGE 10 /* Used for Switch-Case routine-represents that a user input is a Storage Character */
#define PLUS 20 /* Used for Switch-Case routine-represents that a user input is a Command to transfer data from buffer 0 to the rest */
#define REPORT 30 /* Used for Switch-Case routine-represents that a user input is a Command to generate a report and empty contents */
#define DISPLAY 40 /* Used for Switch-Case routine-represents that a user input is a Command to generate a report */
#define EMPTY 50 /* Used for Switch-Case routine-represents that a user input is a Command restart */
#define BUF4 70 /* Used for Switch-Case routine-represents that a user input is a Command '-' */
#define ALL 'A' /* Used for Switch-Case routine in checkChar()-represents that the function must distinguish between Storage and Command characters  */
#define LETTERS 'L' /* Used for Switch-Case routine in checkChar()-represents that the function must distinguish between Letters and Numbers  */
#define DEBUG /* If defined, allows to debug tthe code by writing data into a specific memory address */
#define FAST
__sfr __at (0x8F) CKCON0;

/* Macros */
#ifdef DEBUG
#define DEBUGPORT(x,y) dataout(x, y); //MOV the value x to the address 0xFFFF
#else
#define DEBUGPORT(x)
#endif

/* Externs*/
extern unsigned char errorFlag; /* declared as extern due to possible use in multiple files */

#endif // main

