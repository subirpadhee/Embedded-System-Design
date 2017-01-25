/*  ESD-Final Project library - NIC
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/

#include "main.h"
#include "w5100.h"
#include "glcd.h"

/* Globals */
unsigned char RCVD[1600] = {'\0'};


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

    return 0;
}

void serialInit()
{

    TMOD = 0x20;
    SCON = 0x50;

    TH1 = -3;
    TI = 0;
    RI = 0;
    TR1 = 1;
}

int main(void)
{
    delay_custom(1000);

    //serialInit();
    printf_tiny("\r\nInitializing...");
    SPI_init(); //Initialize the SPI

    /* glcd code starts here */
    glcd_init(); //Initialize the glcd
    delay_1ms();
    /* glcd code ends here */

    /* W5100 code starts */
    RESET_W5100 = 0; //Hardware reset for the w5100 network module
    delay_1ms();
    RESET_W5100 = 1;

    nic(); //call the network module. Its an infinite loop over there
    /* W5100 code ends */

    return 0;
}
