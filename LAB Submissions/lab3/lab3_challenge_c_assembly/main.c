
#include <stdio.h>
#include <stdlib.h>
#include <at89c51ed2.h>
#include <mcs51reg.h>
#include <mcs51/8051.h>
#include <string.h>

/* Function Prototypes*/
unsigned int c_func(unsigned char, unsigned char, unsigned char);

/* Externs*/
extern char asm_func(unsigned char, unsigned char, unsigned char);
unsigned char subir_g = 9;

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
    char a = 10, b = 3, c = 2; /* arguments for function call */
    unsigned int result_asm = 0;
    unsigned int result_c = 0;
    unsigned char result_g = 0;

    printf_tiny("\r\nThe global var is: %d", subir_g); /* Print the global variable */

    result_c = (unsigned int)((char)(a/b)) * c; /* compute locally */

    result_asm = c_func(a,b,c); /* call the c routine which calls the asm routine to compute the value */

    printf_tiny("\r\nThe result from asm file is: %d", result_asm);
    printf_tiny("\r\nThe result computed in C file is: %d", result_c);

    if(result_asm == result_c)
    {
        printf_tiny("\r\nBoth the results are the same!");
    }
    else
    {
        printf_tiny("\r\nBoth the results are NOT the same!");
    }

    return 0;
}

/* Just calls the assembly routine to do the computation */
unsigned int c_func(unsigned char x, unsigned char y, unsigned char z)
{
    return asm_func(x, y, z);
}

/* This function is called from the assembly file. It print 'V' if an argument is received */
void sum(char value)
{
	if(value)
	{
		putchar('V');
	}
	return;
}

/*Prints a character on the Terminal*/
void putchar(char c)
{
    SBUF = c;
    while(TI == 0);
    TI = 0;
}




