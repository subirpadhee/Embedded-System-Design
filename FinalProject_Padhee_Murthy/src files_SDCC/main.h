/*  ESD-Final Project library - NIC
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/

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
unsigned char getChar1();
void putcharac(char c);
void putstring(char * str);
unsigned char getcharac();
unsigned char getstring(unsigned char * str);
unsigned char error(unsigned char c);
void delay_custom(unsigned char t);
void delay_1ms();
void delay_custom_us(unsigned int delay);

/* SPI */
void SPI_init();
//void isr_SPI(void) __interrupt (9);
unsigned char send_SPI(unsigned char value);
unsigned char read_data();

/* NIC */
//void isr_INT1(void) __interrupt (2);
void nic_init();
void ctrl_write(unsigned char addr, unsigned char data_b);
unsigned char ctrl_read(unsigned char addr);
unsigned char sd_single_read(unsigned long sector);
void readBytesSd();
unsigned char read_buf();
unsigned char read_data_nic();
unsigned char send_SPI_nic(unsigned char value);
void buffer_write(unsigned char *data_array, unsigned int length);
unsigned char buffer_read(unsigned char *data_array, unsigned int length);
void bfs(unsigned char addr, unsigned char data_b);
void bfc(unsigned char addr, unsigned char data_b);
void packets_send(unsigned char *str, unsigned int length);
unsigned char packets_recv(unsigned char *str);
void nic();
unsigned char getcharac_chat();

/* SD card */
void sdcard_init();

/* Defines */
#define time20_ms 20
#define time5_ms 5
#define time1_ms 1
#define time100_ms 100

/* SPI */
#define SS P1_4
#define RESET_W5100 P1_1

#define MISO P1_5
#define SCL P1_6
#define MOSI P1_7


/* NIC */
/* MAC ADDRESS */
#define MAC5 0xFC
#define MAC4 0x3F
#define MAC3 0xDB
#define MAC2 0x38
#define MAC1 0x3A
#define MAC0 0x6A
/* OPCODES */
#define CTRL_WR 0x02
#define CTRL_RD 0x00
#define BUF_WR 0x7A
#define BUF_RD 0x3A
#define BFS 0x04
#define BFC 0x05
#define PPCTRL_BYTE 0x00

/* ALL BANKS */
#define EIE 0x1B
#define EIR 0x1C
#define ESTAT 0x1D
#define ECON2 0x1E
#define ECON1 0x1F
/*BANK 0 */
#define EIE 0x1B
#define ERXSTL 0x08
#define ERXSTH 0x09
#define ERXNDL 0x0A
#define ERXNDH 0x0B
#define ERXRDPTL 0x0C
#define ERXRDPTH 0x0D
#define EWRPTL 0x02
#define EWRPTH 0x03
#define ERDPTL 0x00
#define ERDPTH 0x01

#define ETXSTL 0x04
#define ETXSTH 0x05
#define ETXNDL 0x06
#define ETXNDH 0x07

/*BANK 1 */
#define ERXFCON 0x18
#define EPKTCNT 0x19

/*  BANK 2*/
#define MACON1 0x00
#define MACON3 0x02
#define MACON4 0x03
#define MAMXFLL 0x0A
#define MAMXFLH 0x0B
#define MABBIPG 0x04
#define MAIPGL 0x06

/* BANK 3 */
#define MAADR1 0x04
#define MAADR2 0x05
#define MAADR3 0x02
#define MAADR4 0x03
#define MAADR5 0x00
#define MAADR6 0x01



/* Macros */
#define NOP (P1_0 ^= 1)

/* Externs*/
extern unsigned char errorFlag; /* declared as extern due to possible use in multiple files */
extern unsigned char buffer;
extern volatile unsigned char tx_over;
void delay_custom_ms(unsigned int delay);

unsigned char errorFlag = 0; //ERROR FLAG-SET  WHEN AN ERROR OCCURS

#endif // main

