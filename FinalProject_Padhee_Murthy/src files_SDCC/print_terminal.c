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
void putchar( char c)
{
    SBUF = c;
    while(!TI);
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
unsigned char getcharac()
{
    while(RI == 0);
    RI = 0;
    putcharac(SBUF);
    return SBUF;
}

/* getcharac reads a single character from the terminal */
unsigned char getcharac_chat()
{
    while(RI == 0);
    RI = 0;
    putcharac(SBUF);
    return SBUF;
}

/*unsigned char getChar1(){
    while(!RI);
    RI=0;
   // putchar(SBUF);
    return SBUF;
}*/

/* getstring reads a string from the terminal - customized for the Instant Messenger Application */
unsigned char getstring(unsigned char * str)
{
    unsigned char i = 0;
    do
    {
        *str = getcharac_chat();
        str++;
        i++;
        if(i == 99)
        {
            //error(7);
            break;//return 0;
        }
    }while(*(str-1) != '\r');
    *(str-1) = '\0';

    return 1;
}
