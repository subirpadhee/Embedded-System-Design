/* Lab 3 Required elements and and Challenge -Buffer 4 implementation */

#include "main.h"

/* Globals */
unsigned char xdata heap[HEAP_SIZE];

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


int main(void)
{
    init_dynamic_memory((MEMHEADER xdata *)heap, HEAP_SIZE);

    while(menu_buffer());
    menu_rest();
    P1_0 = 0;

    return 0;
}

