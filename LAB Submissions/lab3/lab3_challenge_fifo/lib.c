/* Library - Function definitions */

#include "main.h"

void receive_interupt() __interrupt(4)
{
    if(RI)
    {
        if(SBUF == '-') /* '-' is command for POP */
        {
            if( (!round && (rd < wr)) ||  (round && (rd >= wr)) ) /* round is 1 when wr has crossed over but rd hasn't*/
            {
                SBUF = *rd++;
                *(rd-1) = '\0';
                RI = 0;
            }
            else
                RI= 0;
        }
        else if((*wr) == '\0')
        {
            *wr++ = SBUF;
            RI = 0;
        }
        else
        {
            full = 1;
            RI = 0;
        }
    }
    else
    {
        TI = 0;

    }
}

/* putcharac prints a single character on the terminal */
void putcharac(char c)
{
    SBUF = c;
    while(TI == 0);
    TI = 0;
}

/* putstring prints a string on the terminal */
void putstring(char * str)
{
    while(*str)
    {
        putcharac(*str++);
    }
}
void putchar(char ch)
{

}

char getcharac()
{
    while(RI == 0);
    RI = 0;
    //putcharac(SBUF);
    return SBUF;
}

/* getstring reads a string from the terminal */
unsigned char getstring(char * str)
{
    unsigned char i = 0;
    do
    {
        *str = getcharac();
        str++;
    }while(*(str-1) != '\r');
    *(str-1) = '\0';

    return 1;
}

