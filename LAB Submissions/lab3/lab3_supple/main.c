/* Lab3 Supplemental */

#include "main_sup.h"

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

    while(1)
    {
        menu_first();
    }
    return 0;
}

