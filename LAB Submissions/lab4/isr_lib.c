/*  LAB-4 library source file - ISR
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"

/* Function definitions */

/* ISR for timer0 interrupt */
void isr_TIMER0(void) __interrupt (1)
{
    unsigned char i = 0; //LOOP VARIABLE
    unsigned char addr = 0x80; //STORES DDRAM ADDRESS
    volatile static unsigned char first_overflow = 1; //FLAG TO CONTROL RELOAD OF THE TIMER REGISTERS
    volatile static unsigned char desi_sec = 0; //STORES THE DESI SECOND VALUE
    volatile static unsigned char sec = 0; //STORES THE SECONDS VALUE
    volatile static unsigned int minute = 0; //STORES THE MINUTE VALUE

    /* The following sequence stores the cursor position before the ISR was called so that it can be restored before exiting the ISR.
    This makes sure activity on the LCD is not disrupted due to the ISR */

    RS = 0;
    RWbar = 1;
    NOP;
    //delay_1ms();
    RD_DATA;
    addr |= ((*(xdata unsigned char *)RD_ADDR) & 0x7F); //addr stores the cursor position

    /* If reset flag is set, reset the time holding variables */
    if(reset_flag)
    {
        first_overflow = 0;
        desi_sec = 0;
        sec = 0;
        minute = 0;
        reset_flag = 0;
    }

    /* The timer register values are chosen so as to count a desi second every second interrupt */
    /* During every other interrupt the timer registers are reloaded with 0 and 0 */
    if(first_overflow)
    {
        TH0 = 0x00;
        TL0 = 0x00;
        first_overflow = 0;
    }
    /* During every other interrupt the timer registers are reloaded with 0x98 and 0x8A */
    else
    {
        desi_sec++;
        TH0 = 0x98;
        TL0 = 0x8A;
        first_overflow = 1;
    }
    if(10 == desi_sec)
    {
        sec++;
        desi_sec = 0;
    }
    if(60 == sec)
    {
        minute++;
        sec = 0;
    }
    /* If FLAG is set, suspend any activity on the LCD because some other function is doing that.
    FLAG is used as a semaphore here */
    if(!FLAG)
    {
        lcdgotoxy(3,9);
        lcdputch((minute/10) + '0');
        lcdputch((minute % 10) + '0');
        lcdputch(':');
        lcdputch((sec/10) + '0');
        lcdputch((sec % 10) + '0');
        lcdputch(':');
        lcdputch(desi_sec + '0');
        //display_time();

    /* Alarm goes off if it has timed out */
    for(i = 0; i < 3; i++)
    {
        if(alarm_status & (0x01 << i))
        {
            if(((time_val[i]/60) == minute) && ((time_val[i] % 60) == sec))
            {
                lcdgotoxy(i,0);
                lcdputstr("                ");
                lcdgotoxy(i,0);
                lcdputstr("   WAKE UP!");
            }
        }
    }
    lcdgotoaddr(addr); //Restore cursor to its position before the ISR
    }
    return;
}

/* ISR for external interrupt number 1
This is called every time there is activity on one of the input pins of the I/O expander */
void isr_INT1(void) __interrupt (2)
{
    unsigned char i = 0, j = 0;
    unsigned char port_io_val_local = port_io_val;
    EX1 = 0;
    if(cnt_iox < 9)
        cnt_iox++;
    else if(cnt_iox > 9)
        cnt_iox = 0;

    lcdgotoxy(3,1);
    printf_tiny("\r\n cnt_iox: %d", cnt_iox);
    lcdputch(cnt_iox + '0');
    for(i = 0, j = 0; (i < 8) && (j < 4); i++)
    {
        if(port_io_val_local & (0x01 << i) == 0)
            ;//Do npthing
        else
        {
            port_io_val_local |= ((cnt_iox & (0x01 << j))?  1<< i : 0);
            j++;
        }
    }
    datasend_iox(port_io_val_local);
    printf("\tO/P Val on the port: %X", port_io_val_local);
    EX1 = 1;
}

/* ISR for PCA interrupts
The Watchdog is serviced hee*/
void isr_pca(void) __interrupt (6)
{
    if(CCF2) /* Check if interrupt is from Module 2 - SW Timer in this case */
    {
        {
            WDTRST = 0x01E;
            WDTRST = 0x0E1;
        }
        CCF2 = 0;
    }
    return;
}

/* ISR for Timer2
Timer 2 is used in the program to time execution time of functions */
void isr_TIMER2(void) __interrupt (5)
{
    TF2 = 0;
    ov_cnt++;
}

