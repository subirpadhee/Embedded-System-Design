#ifndef main_h
#define main_h

/* File includes */
#include <stdio.h>
#include <stdlib.h>
#include <at89c51ed2.h>
#include <mcs51reg.h>
#include <mcs51/8051.h>
#include <string.h>
#include <math.h>

/* Function Prototypes*/
void putcharac(char c);
void putstring(char * str);
char getcharac();
unsigned char getstring(char * str);
unsigned char error(unsigned char c);
unsigned char dataread_i2c(unsigned char block, unsigned char addr);
void lcdinit();
void lcdbusywait();
void delay_custom(unsigned char t);
void delay_1ms();
void lcdgotoaddr(unsigned char addr);
void lcdgotoxy(unsigned char row, unsigned char column);
void lcdputch(unsigned char ch);
void lcdputstr(unsigned char *str);
unsigned char start_i2c();
unsigned char stop_i2c();
void eereset();
unsigned char sendbyte_i2c(unsigned char databyte);
unsigned char datasend_i2c(unsigned char block, unsigned char addr, unsigned char databyte);
unsigned char datasend_iox(unsigned char databyte);
void menu();
void menu_write();
void menu_read();
void menu_display();
void clear_disp();
void hexdumpEEPROM();
void DDRAMdump();
void CGRAMdump();
void lcd_play();
unsigned char checkChar(char ch);
unsigned int checkInput(unsigned char *str);
void num2str(unsigned int ch, unsigned char *str);
//void custom_smiley();
void lcdcreatechar(unsigned char ccode, unsigned char *row_vals);
unsigned char binStr2dec(unsigned char *str, unsigned char len);
void menu_custchar();
unsigned char getstring_custchar(char * str);
void isr_TIMER0(void) __interrupt (1);
void isr_INT1(void) __interrupt (2);
void menu_time();
void timer_0_init();
void ioexpander();
unsigned char getstring_alarm(char * str);
unsigned int compute_alarmtime(unsigned char *str);
void pcatimer_init();
void isr_pca(void) __interrupt (6);
void menu_challenge();
unsigned char search_string();
unsigned char getstring_search(char * str);
unsigned char byte_write();
unsigned char page_write(unsigned char block, unsigned char addr, unsigned char length, unsigned char *data_b);
void isr_TIMER2(void) __interrupt (5);
void timer_2_init();
void compute_time();
void blockfill();
unsigned char getstring_blockfill(char * str);
unsigned char search2();
unsigned char ddram_pos(unsigned char *);

//seq
unsigned char dataread_i2c_sequential_first(unsigned char block, unsigned char addr);
unsigned char dataread_i2c_sequential();
void hexdumpEEPROM_sequential();



/* Defines */

#define time20_ms 20
#define time5_ms 5
#define time1_ms 1
#define time100_ms 100
#define RS P1_3
#define RWbar P1_2
#define BF P0_7
#define FLAG P1_1
//#define RESET_FLAG P1_7
#define WR_ADDR 0xF000
#define RD_ADDR 0xF100
#define WR_DATA(x) (*(xdata unsigned char *)WR_ADDR = x)
#define RD_DATA (*(xdata unsigned char *)RD_ADDR = P0)
#define UNLOCK 0x30
#define FS_8b2L 0x38
#define DISP_OFF 0x08
#define DISP_ON 0x0C
#define ENTRY_MODE 0x06
#define CLR_DISP 0x01
#define CRSR_BLINK 0x0F
#define CRSR_RIGHT_SHIFT 0x14
#define ADDR_ROW0_BASE 0x80
#define ADDR_ROW1_BASE 0xC0
#define ADDR_ROW2_BASE 0x90
#define ADDR_ROW3_BASE 0xD0
#define SDA P1_5
#define SCL P1_4
#define RESET_byte 0xFF
#define WRITE 1
#define READ 2
#define DISPLAY 3
#define CLEAR 4
#define EEPROM 5
#define DDRAM 6
#define CGRAM 7
#define CUSTOMCHAR 8
#define TIME_DISP 9
#define LCD 10
#define HEXseq 11
#define PAUSE 3
#define START 12
#define RESET 2
#define EXPANDER 14
#define ALARM 9
#define DALARM 4
#define EALARM 5
#define KILL 15
#define STATUS 1
#define CHALLENGE 16
#define SEARCH 12
#define PAGE 3
#define BYTE 17
#define FILL 18
#define FAST
__sfr __at (0x8F) CKCON0;


/* Macros */
#define NOP (P1_0 ^= 1)
#define DISABLE (TR0 = 0)
#define ENABLE (TR0 = 1)

/* Externs*/
extern unsigned char errorFlag; /* declared as extern due to possible use in multiple files */

//volatile unsigned char semaph = 0;
extern volatile unsigned char reset_flag;//SET TO 1 WHEN CLOCK HAS TO BE RESET
extern unsigned char cnt_iox; //COUNTS NUMBER OF BUTTON PRESSES ON I/O EXPANDER INPUT
extern unsigned char port_io_val; //STORES THE STATUS OF THE I/O EXPANDER PORT PINS
extern unsigned int time_val[3]; //STORES THE USER INPUT FOR ALARM SETTING
extern unsigned char alarm_status; //STATUS OF THE 3 ALARMS- E/D - FOR ENABLING
extern unsigned char alarm_status_d; //STATUS OF THE 3 ALARMS- E/D -FOR DISABLING
extern unsigned char WD_flag; //WATCHDOG TIMER FLAG-USED FOR SETTING IT OFF
extern volatile unsigned char ov_cnt; //COUNTS THE NUMBER OF OVERFLOWS IN TIMER2

unsigned char errorFlag = 0; //ERROR FLAG-SET  WHEN AN ERROR OCCURS

#endif // main

