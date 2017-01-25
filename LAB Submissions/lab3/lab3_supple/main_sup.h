#ifndef main_h
#define main_h

/* File includes */
#include <stdio.h>
#include <stdlib.h>
#include <at89c51ed2.h>
#include <mcs51reg.h>
#include <mcs51/8051.h>
#include <string.h>

/* Function Prototypes*/
void putcharac(char c);
char getcharac();
void putstring(char * str);
unsigned char getstring(char * str);
void error(unsigned char code);
char menu_first();
void menu_rest();
char checkChar(char ch);
char menu_first();
char pwm_menu();
char hso_menu();
void print_pwm(unsigned char pw);
void isr_six(void) __interrupt (6);
void isr_two(void) __interrupt (2);



/* Defines */
#define HEAP_SIZE 0x7D0   // 0x7D0 is 2000 bytes
#define PWMN 1 /* Used for Switch-Case routine-represents that a user input is a Command to get to the PWM menu */
#define HSO 2 /* Used for Switch-Case routine-represents that a user input is a Command to generate HSO */
#define TIMER 3 /* Used for Switch-Case routine-represents that a user input is a Command to start sw timer */
#define UART 4 /* Used for Switch-Case routine-represents that a user input is a Command to check for start of Rx on RS-232 */
#define RUN 5 /* Used for Switch-Case routine-represents that a user input is a Command to check for start of Rx on RS-232 */
#define STOP 6 /* Used for Switch-Case routine-represents that a user input is a Command to start PWM */
#define INCREASE 7 /* Used for Switch-Case routine-represents that a user input is a Command to Increase duty cycle of PWM */
#define DECREASE 8 /* Used for Switch-Case routine-represents that a user input is a Command to Decrease duty cycle of PWM */
#define IDLE 9 /* Used for Switch-Case routine-represents that a user input is a Command to Enter Idle mode */
#define PDOWN 10 /* Used for Switch-Case routine-represents that a user input is a Command to enter Power Down mode */
#define EXIT 11 /* Used for Switch-Case routine-represents that a user input is a Command to get to the previous menu */
#define DOUBLE 12 /* Used for Switch-Case routine-represents that a user input is a Command to Increase Frequency of HSO */
#define HALVE 13 /* Used for Switch-Case routine-represents that a user input is a Command to Decrease Frequency of HSO */
#define WD 14 /* Used for Switch-Case routine-represents that a user input is a Command to start the Watchdog Timer */
/* Macros */
#ifdef DEBUG
#define DEBUGPORT(x,y) dataout(x, y); //MOV the value x to the address 0xFFFF
#else
#define DEBUGPORT(x)
#endif

/* Externs*/
extern unsigned char errorFlag; /* declared as extern due to possible use in multiple files */

#endif // main


