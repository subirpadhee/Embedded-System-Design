/*  ESD-Final Project library - glcd.c
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/

#include "main.h"
#include "glcd.h"


/* FUNCTION DEFINITIONS */

void glcd_init()
{
    CS1 = 1;
    CS2 = 1;
    send_command(0x3F);
    delay_1ms();
    delay_1ms();
    delay_1ms();
    clear_glcd();

    column_select(0);
    page_select(0);
}

void glcd(unsigned char str[], unsigned char c)
{
    volatile static unsigned char p = 0;
    unsigned int i = 0, len = 0, j = 0;
    static unsigned char counter = 0;
    unsigned char record_screen[7][22] = {{'\0'}};
    unsigned char array_char[][6] =   {
                                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char
                                        {0x00, 0x00, 0x06, 0x5F, 0x06, 0x00},  // Code for char !
                                        {0x00, 0x07, 0x03, 0x00, 0x07, 0x03},  // Code for char "
                                        {0x00, 0x24, 0x7E, 0x24, 0x7E, 0x24},  // Code for char #
                                        {0x00, 0x24, 0x2B, 0x6A, 0x12, 0x00},  // Code for char $
                                        {0x00, 0x63, 0x13, 0x08, 0x64, 0x63},  // Code for char %
                                        {0x00, 0x36, 0x49, 0x56, 0x20, 0x50},  // Code for char &
                                        {0x00, 0x00, 0x07, 0x03, 0x00, 0x00},  // Code for char '
                                        {0x00, 0x00, 0x3E, 0x41, 0x00, 0x00},  // Code for char (
                                        {0x00, 0x00, 0x41, 0x3E, 0x00, 0x00},  // Code for char )
                                        {0x00, 0x08, 0x3E, 0x1C, 0x3E, 0x08},  // Code for char *
                                        {0x00, 0x08, 0x08, 0x3E, 0x08, 0x08},  // Code for char +
                                        {0x00, 0x00, 0xE0, 0x60, 0x00, 0x00},  // Code for char ,
                                        {0x00, 0x08, 0x08, 0x08, 0x08, 0x08},  // Code for char -
                                        {0x00, 0x00, 0x60, 0x60, 0x00, 0x00},  // Code for char .
                                        {0x00, 0x20, 0x10, 0x08, 0x04, 0x02},  // Code for char /
                                        {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E},  // Code for char 0
                                        {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00},  // Code for char 1
                                        {0x00, 0x62, 0x51, 0x49, 0x49, 0x46},  // Code for char 2
                                        {0x00, 0x22, 0x49, 0x49, 0x49, 0x36},  // Code for char 3
                                        {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10},  // Code for char 4
                                        {0x00, 0x2F, 0x49, 0x49, 0x49, 0x31},  // Code for char 5
                                        {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30},  // Code for char 6
                                        {0x00, 0x01, 0x71, 0x09, 0x05, 0x03},  // Code for char 7
                                        {0x00, 0x36, 0x49, 0x49, 0x49, 0x36},  // Code for char 8
                                        {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E},  // Code for char 9
                                        {0x00, 0x00, 0x6C, 0x6C, 0x00, 0x00},  // Code for char :
                                        {0x00, 0x00, 0xEC, 0x6C, 0x00, 0x00},  // Code for char ;
                                        {0x00, 0x08, 0x14, 0x22, 0x41, 0x00},  // Code for char <
                                        {0x00, 0x24, 0x24, 0x24, 0x24, 0x24},  // Code for char =
                                        {0x00, 0x00, 0x41, 0x22, 0x14, 0x08},  // Code for char >
                                        {0x00, 0x02, 0x01, 0x59, 0x09, 0x06},  // Code for char ?
                                        {0x00, 0x3E, 0x41, 0x5D, 0x55, 0x1E},  // Code for char @
                                        {0x00, 0x7E, 0x11, 0x11, 0x11, 0x7E},  // Code for char A
                                        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36},  // Code for char B
                                        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x22},  // Code for char C
                                        {0x00, 0x7F, 0x41, 0x41, 0x41, 0x3E},  // Code for char D
                                        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x41},  // Code for char E
                                        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x01},  // Code for char F
                                        {0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A},  // Code for char G
                                        {0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F},  // Code for char H
                                        {0x00, 0x00, 0x41, 0x7F, 0x41, 0x00},  // Code for char I
                                        {0x00, 0x30, 0x40, 0x40, 0x40, 0x3F},  // Code for char J
                                        {0x00, 0x7F, 0x08, 0x14, 0x22, 0x41},  // Code for char K
                                        {0x00, 0x7F, 0x40, 0x40, 0x40, 0x40},  // Code for char L
                                        {0x00, 0x7F, 0x02, 0x04, 0x02, 0x7F},  // Code for char M
                                        {0x00, 0x7F, 0x02, 0x04, 0x08, 0x7F},  // Code for char N
                                        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E},  // Code for char O
                                        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06},  // Code for char P
                                        {0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E},  // Code for char Q
                                        {0x00, 0x7F, 0x09, 0x09, 0x19, 0x66},  // Code for char R
                                        {0x00, 0x26, 0x49, 0x49, 0x49, 0x32},  // Code for char S
                                        {0x00, 0x01, 0x01, 0x7F, 0x01, 0x01},  // Code for char T
                                        {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F},  // Code for char U
                                        {0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F},  // Code for char V
                                        {0x00, 0x3F, 0x40, 0x3C, 0x40, 0x3F},  // Code for char W
                                        {0x00, 0x63, 0x14, 0x08, 0x14, 0x63},  // Code for char X
                                        {0x00, 0x07, 0x08, 0x70, 0x08, 0x07},  // Code for char Y
                                        {0x00, 0x71, 0x49, 0x45, 0x43, 0x00},  // Code for char Z
                                        {0x00, 0x00, 0x7F, 0x41, 0x41, 0x00},  // Code for char [
                                        {0x00, 0x02, 0x04, 0x08, 0x10, 0x20},  // Code for char BackSlash
                                        {0x00, 0x00, 0x41, 0x41, 0x7F, 0x00},  // Code for char ]
                                        {0x00, 0x04, 0x02, 0x01, 0x02, 0x04},  // Code for char ^
                                        {0x80, 0x80, 0x80, 0x80, 0x80, 0x80},  // Code for char _
                                        {0x00, 0x00, 0x03, 0x07, 0x00, 0x00},  // Code for char `
                                        {0x00, 0x20, 0x54, 0x54, 0x54, 0x78},  // Code for char a
                                        {0x00, 0x7F, 0x44, 0x44, 0x44, 0x38},  // Code for char b
                                        {0x00, 0x38, 0x44, 0x44, 0x44, 0x28},  // Code for char c
                                        {0x00, 0x38, 0x44, 0x44, 0x44, 0x7F},  // Code for char d
                                        {0x00, 0x38, 0x54, 0x54, 0x54, 0x08},  // Code for char e
                                        {0x00, 0x08, 0x7E, 0x09, 0x09, 0x00},  // Code for char f
                                        {0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C},  // Code for char g
                                        {0x00, 0x7F, 0x04, 0x04, 0x78, 0x00},  // Code for char h
                                        {0x00, 0x00, 0x00, 0x7D, 0x40, 0x00},  // Code for char i
                                        {0x00, 0x40, 0x80, 0x84, 0x7D, 0x00},  // Code for char j
                                        {0x00, 0x7F, 0x10, 0x28, 0x44, 0x00},  // Code for char k
                                        {0x00, 0x00, 0x00, 0x7F, 0x40, 0x00},  // Code for char l
                                        {0x00, 0x7C, 0x04, 0x18, 0x04, 0x78},  // Code for char m
                                        {0x00, 0x7C, 0x04, 0x04, 0x78, 0x00},  // Code for char n
                                        {0x00, 0x38, 0x44, 0x44, 0x44, 0x38},  // Code for char o
                                        {0x00, 0xFC, 0x44, 0x44, 0x44, 0x38},  // Code for char p
                                        {0x00, 0x38, 0x44, 0x44, 0x44, 0xFC},  // Code for char q
                                        {0x00, 0x44, 0x78, 0x44, 0x04, 0x08},  // Code for char r
                                        {0x00, 0x08, 0x54, 0x54, 0x54, 0x20},  // Code for char s
                                        {0x00, 0x04, 0x3E, 0x44, 0x24, 0x00},  // Code for char t
                                        {0x00, 0x3C, 0x40, 0x20, 0x7C, 0x00},  // Code for char u
                                        {0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C},  // Code for char v
                                        {0x00, 0x3C, 0x60, 0x30, 0x60, 0x3C},  // Code for char w
                                        {0x00, 0x6C, 0x10, 0x10, 0x6C, 0x00},  // Code for char x
                                        {0x00, 0x9C, 0xA0, 0x60, 0x3C, 0x00},  // Code for char y
                                        {0x00, 0x64, 0x54, 0x54, 0x4C, 0x00},  // Code for char z
                                        {0x00, 0x08, 0x3E, 0x41, 0x41, 0x00},  // Code for char {
                                        {0x00, 0x00, 0x00, 0x77, 0x00, 0x00},  // Code for char |
                                        {0x00, 0x00, 0x41, 0x41, 0x3E, 0x08},  // Code for char }
                                        {0x00, 0x02, 0x01, 0x02, 0x01, 0x00},  // Code for char ~
                                        {0x00, 0x3C, 0x26, 0x23, 0x26, 0x3C}  // Code for char  */

    };

    if(p == 8)
    {
        p = 0; //WRAP AROUND THE DISPLAY BY SELECTING PAGE 0
        clear_glcd();
    }
    page_select(p++); //INCREMENT TO THE NEXT PAGE FOR DISPLAYING THE NEXT STRING
    clear_page(p-1);

    column_select(c);
    len = strlen(str);
    len -= (counter++ % 2);//TRANSMITTED AND RECEIVED STRINGS ARE FORMATTED DIFFERNTLY. THIS TAKES CARE OF THAT

    for(i = 0; (i < len) && (i < 22); i++)
    {
        printf("\r\ni=%X, %c len:%d - %s\r",i,str[i], len, str);
        print_char(c+(i*6), array_char[str[i] -32]);
    }
}

/* DISPLAYS A SINGLE CHARACTER ON GLCD */
void print_char(unsigned char column_curr, unsigned char *array_char)
{
    unsigned char i = 0, old = 0;
    unsigned char current_c = 0;

    current_c = column_curr;

    for(i = 0; i < 6; i++)
    {
        column_select(current_c+i);
        fill_column(current_c+i, array_char[i]);
    }
}

/* SEND A COMMAND TO THE GLCD  CONTROLLER */
void send_command(unsigned char command)
{
    P0 = command;
    RWbar = 0;
    RS = 0;
    E = 0;
    delay_custom(300);
    E = 1;
    delay_custom(300);
    E = 0;
}

/* SEND DATA TO FILL ONE COLUMN- 8 PIXELS */
void fill_column(unsigned char column_curr, unsigned char data_b)
{
    //static unsigned char flag = 1;

    if(column_curr < 64)
    {
        CS1 = 1;
        CS2 = 0;
    }
    else
    {
        CS1 = 0;
        CS2 = 1;
    }

    P0 = data_b;
    RWbar=0;
    RS=1;
    E=0;
    delay_custom(300);
    E = 1;
    delay_custom(300);
    E = 0;
}

/* SELECTS THE DESIRED PAGE ON GLCD */
void page_select(unsigned char page)
{
    unsigned char inst_byte = 0xB8;

    inst_byte |= page;
    P0 = inst_byte;
    CS1 = 1;
    CS2 = 1;
    RS = 0;
    RWbar = 0;

    E=1;
    NOP;
    E=0;
}

/* SELECTS THE DESIRED COLUMN ON GLCD */
void column_select(unsigned char column)
{
    unsigned char inst_byte = 0x40;

    inst_byte |= column;


    if(column < 64)
    {
        CS1 = 1;
        CS2 = 0;
    }
    else
    {
        CS1 = 0;
        CS2 = 1;
    }
    P0 = inst_byte;

    RS = 0;
    RWbar = 0;

    E=1;
    NOP;
    E=0;
}

/* CLEARS THE ENTIRE GLCD */
void clear_glcd()
{
    unsigned char i = 0, j = 0;

    for(i = 0; i < 8; i++)
    {
        page_select(i);
        column_select(0);
        for(j = 0; j < 128; j++)
        {
            fill_column(j, 0x00);
            delay_1ms();
        }
    }
}

/* CLEARS A PAGE ON THE GLCD */
void clear_page(unsigned char page)
{
    unsigned char i = 0, j = 0;

    page_select(page);
    column_select(0);
    for(j = 0; j < 128; j++)
        fill_column(j, 0x00);
}


