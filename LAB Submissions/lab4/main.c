/*  LAB-4 main source file
    SUBIR KUMAR PADHEE
    ECEN5613
 */

#include "main.h"

/* Globals */
//volatile unsigned char semaph = 0;
volatile unsigned char reset_flag = 0;
unsigned char cnt_iox = 0;
unsigned char port_io_val = 0x80;
unsigned int time_val[3] = {0, 0, 0};
unsigned char alarm_status = 0;
unsigned char alarm_status_d = 0;
unsigned char WD_flag = 0;
volatile unsigned char ov_cnt = 0;


_sdcc_external_startup()
{
    TI = 0;
    RI = 0;
    SCON = 0x50;
    TMOD = 0x20;
    TH1 = 253;
    TL1 = 253; // for 9600 baud rate of the RS-232 communication
    TR1 = 1;
    AUXR |= 0x0C;
    FLAG = 0;

    eereset(); //EEPROM RESET-DONE AT POWER ON

    return 0;
}


int main(void)
{
    unsigned char ch = 0;
    unsigned char row_val[8] = {0x0C, 0x12, 0x10, 0x15, 0x17, 0x0D, 0x07, 0x00};// CU logo data

    lcdinit();

    /* Ext INT 1 for IO expander - Interrupt settings */
    EA = 1;
    EX1 = 1;
    IT1 = 1; // Falling Edge trigger

    timer_0_init(); //For Clock
    timer_2_init(); //to compute time taken by function execution

    pcatimer_init();

     //Watchdog initialization
    WDTPRG = 0X07;
    WDTRST = 0x01E;
    WDTRST = 0x0E1;

    menu();

    return 0;
}
