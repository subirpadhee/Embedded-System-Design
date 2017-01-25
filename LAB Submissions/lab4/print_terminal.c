/*  LAB-4 library source file - TERMINAL
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"

/* Function definitions */

/* putcharac prints a single character on the terminal */
void putcharac(char c)
{
    SBUF = c;
    while(TI == 0);
    TI = 0;
}
/* used bu printf() */
void putchar(char c)
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

/* getcharac reads a single character from the terminal */
char getcharac()
{
    while(RI == 0);
    RI = 0;
    putcharac(SBUF);
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
        i++;
        if(i == 5)
        {
            error(7);
            return 0;
        }
    }while(*(str-1) != '\r');
    *(str-1) = '\0';

    return 1;
}

/* getstring reads a string from the terminal */
unsigned char getstring_alarm(char * str)
{
    unsigned char i = 0;
    do
    {
        *str = getcharac();
        str++;
        i++;//at this point, i = no of chars stored
        if(i == 6)
        {
            error(7);
            return 0;
        }
    }while(*(str-1) != '\r');
    *(str-1) = '\0';
    if(i != 5)
        error(7);

    return 1;
}

/* getstring reads a string from the terminal */
unsigned char getstring_custchar(char * str)
{
    do
    {
        *str = getcharac();
        str++;
    }while(*(str-1) != '\r');
    *(str-1) = '\0';

    return 1;
}

/* getstring reads a string from the terminal */
unsigned char getstring_search(char * str)
{
    unsigned char i = 0;
    do
    {
        *str = getcharac();
        str++;
        i++;
        if(i == 65)
        {
            error(7);
            return 0;
        }
    }while(*(str-1) != '\r');
    *(str-1) = '\0';

    return 1;
}

/* getstring reads a string from the terminal */
unsigned char getstring_blockfill(char * str)
{
    unsigned char i = 0;
    do
    {
        *str = getcharac();
        str++;
        i++;//at this point, i = no of chars stored
        if(i == 4)
        {
            error(7);
            return 0;
        }
    }while(*(str-1) != '\r');
    *(str-1) = '\0';

    return 1;
}
