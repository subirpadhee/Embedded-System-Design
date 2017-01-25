/* Lab3 FIFO challenge */

#include "main.h"

/* Globals */
unsigned char FIFO[SIZE] = {'\0'};
unsigned char *rd = &FIFO[0]; /* Points to the location to be popped */
unsigned char *wr = &FIFO[0]; /* Points to the location to be written onto */
volatile unsigned char full = 0;
volatile unsigned char round = 0;/* flag to indicate (if set) that the write pointer has finished one run of the FIFO but the read pointer has not */
unsigned char start = 0;
unsigned char stop = 0;
unsigned char ch;


_sdcc_external_startup()
{
    #ifdef POLLING
    TI = 0;
    RI = 0;
    EA = 0;
    ES = 0;
    #endif
    AUXR |= 0x0C;                   // Setting the XRAM to use 1Kb of its memory
    SM0 = 0;                        //
    SM1 = 1;
    REN = 1;                        // REN on enabling helps to receive serial data

    TMOD = 0x022;                   // Configures Timer 1, 2 in 8 bit auto-reload mode
    SCON = 0x050;                   // Configuring UART in 8 bit a synchronus mode
    TH1 = 0x0FD;                    // BAUD RATE OF 9600
    TL1 = 0x0FD;
    #ifndef POLLING
    EA = 1;
    ES = 1;
    #endif
    TR1 = 1;                        // SET TIMER 1
    TH0 = 0x00;
    TL0 = 0x00;

    return 0;
}


int main(void)
{

    while(1)
    {
        if(wr == &FIFO[SIZE]) /* Generates a warning but its not a problem since only the address is beimg read*/
        {
            wr = &FIFO[0];
            round = 1;
        }
        if(rd == &FIFO[SIZE]) /* Generates a warning but its not a problem since only the address is beimg read*/
        {
            rd = &FIFO[0];
            round = 0;
        }

        if(TF0)
        {
            P1_0 ^= 1;
        }

     //pOLLING
    #ifdef POLLING
    ch = getcharac();
    if(ch == '-')
        {
            if( (!round && (rd < wr)) ||  (round && (rd >= wr)) )
            {
                putcharac(*rd++);
                *(rd-1) = '\0';
            }
        }
        else if((*wr) == '\0')
        {
            *wr++ = ch;
            //putcharac(*(wr-1));
        }
        else
        {
            full = 1;
        }
    #endif
    }
    return 0;
}
