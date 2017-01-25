/*  LAB-4 library source file - LCD
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"


/* Function definitions */

/* UI for DISPLAYING EEPROM DATA ON LCD */
void menu_display()
{
    unsigned char eeAddr[5]; //EEPROM ADDRESS - STRING
    unsigned int rcvd = 0; //ADDRESS - INTEGER
    unsigned char row = 0; //ROW TO DISPLAY IN
    unsigned char addr = 0; //WORD ADDRESS
    unsigned char data_b = 0; //DATA STORED IN EEPROM
    unsigned char block = 0; //CONTROL BYTE FOR EEPROM
    unsigned char str[4] = {'\0'}; //STORES STRING TO BE DISPLAYED ON THE LCD

    putstring("\r\n\t\t\t\t\t\t\t*****************************************");
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t* EEPROM Address                        *");
    putstring("\r\n\t\t\t\t\t\t\t* 0x");
    getstring(eeAddr);
    putstring("\r\n\t\t\t\t\t\t\t* Row {0,1,2,3}:");
    row = getcharac();
    //putstring("          *");
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");

    if((rcvd = checkInput(eeAddr)) > 2047)
    {
        error(7);
        return;
    }
    if((row - '0') > 3)
    {
        error(9);
        return;
    }
    addr = rcvd & 0xFF;
    block = (rcvd >> 8) & 0xFF;

    data_b = dataread_i2c(block, addr);

    FLAG = 1; //to prevent clock update

    lcdgotoxy((row - '0'), 1);

    num2str(rcvd, &str[0]);
    lcdputstr(str);
    lcdputch(':');

    str[2] = '\0';
    str[3] = '\0';

    num2str(data_b, &str[0]);

    lcdputstr(&str[1]);

    FLAG = 0;
}

/* Initialize the LCD */
void lcdinit()
{
    delay_custom(time100_ms);

    RS = 0;
    RWbar = 0;
    delay_1ms();
    WR_DATA(UNLOCK);
    delay_1ms();

    delay_custom(time5_ms);

    RS = 0;
    RWbar = 0;
    delay_1ms();

    WR_DATA(UNLOCK);

    delay_1ms();

    delay_custom(time1_ms);

    RS = 0;
    RWbar = 0;
    delay_1ms();

    WR_DATA(UNLOCK);

    delay_1ms();

    lcdbusywait();

    RS = 0;
    RWbar = 0;
    delay_1ms();

    WR_DATA(FS_8b2L);

    delay_1ms();

    RS = 0;
    RWbar = 0;
    delay_1ms();

    WR_DATA(DISP_OFF);


    delay_1ms();

    RS = 0;
    RWbar = 0;
    delay_1ms();

    WR_DATA(DISP_ON);

    delay_1ms();

    lcdbusywait();

    RS = 0;
    RWbar = 0;
    delay_1ms();

    WR_DATA(CRSR_BLINK);

    delay_1ms();

    RS = 0;
    RWbar = 0;
    delay_1ms();

    WR_DATA(ENTRY_MODE);

    delay_1ms();

    lcdbusywait();

    RS = 0;
    RWbar = 0;
    delay_1ms();

    WR_DATA(CLR_DISP);

    delay_1ms();
}

void lcdbusywait()
{
    while(1)
    {
        RS = 0;
        RWbar = 1;
        NOP;
        RD_DATA;
        if(0 == (*(xdata unsigned char *)RD_ADDR & 0x80))
            break;
    }
    RS = 1;
    return;
}


/* Set address for display*/
void lcdgotoaddr(unsigned char addr)
{
    lcdbusywait();
    RS = 0;
    RWbar = 0;
    NOP;
    WR_DATA(addr);
}

/* Set address for display based on (row, column) input */
void lcdgotoxy(unsigned char row, unsigned char column)
{
    if((row > 3) || (column > 0x0F))
        error(0);/* Out of range address */
    else
    {
        switch(row)
        {
        case 0:
            lcdgotoaddr(ADDR_ROW0_BASE + column);
            break;
        case 1:
            lcdgotoaddr(ADDR_ROW1_BASE + column);
            break;
        case 2:
            lcdgotoaddr(ADDR_ROW2_BASE + column);
            break;
        case 3:
            lcdgotoaddr(ADDR_ROW3_BASE + column);
            break;

        }
    }
}
/* Write a character on the LCD */
void lcdputch(unsigned char ch)
{
    lcdbusywait();
    RS = 1;
    RWbar = 0;
    NOP;
    WR_DATA(ch);
}

/* Display a string*/
void lcdputstr(unsigned char *str)
{
    volatile unsigned char i = 0, j=0;
    while(*str != '\0')
    {
        RS = 0;
        RWbar = 1;
        NOP;
        RD_DATA;

        /* The cursor is incremented linearly and not in the order characters are displayed on the LCD.
        The following lines make sure the cursor position is set in the order of display */

        if((((*(xdata unsigned char *)RD_ADDR) & 0x7F) == 0x0F))
        {
            lcdgotoxy(1,0);
        }
        else if((((*(xdata unsigned char *)RD_ADDR) & 0x7F) == 0x4F))
        {
            lcdgotoxy(2,0);
        }
        else if((((*(xdata unsigned char *)RD_ADDR) & 0x7F) == 0x1F) )
        {
            lcdgotoxy(3,0);
        }
        else if((((*(xdata unsigned char *)RD_ADDR) & 0x7F) == 0x5F) )
        {
            lcdgotoxy(0,0);
        }
        lcdputch(*str++);
    }
}

/* Converts a number to string to be displayed on the LCD */
void num2str(unsigned int ch, unsigned char *str)
{
    unsigned int q = 0, r = 0;

    q = ch/256;

    if(q < 10)
        str[0] = q + '0';
    else
        str[0] = q + 55;

    r = ch % 256;
    ch = r/16;

    if(ch < 10)
        str[1] = ch + '0';
    else
        str[1] = ch + 55;

    r = r % 16;
    if(r < 10)
        str[2] = r + '0';
    else
        str[2] = r + 55;
    str[3] = '\0';
}

/* Function to clear the LCD display */
void clear_disp()
{
    RS = 0;
    RWbar = 0;
    NOP;

    WR_DATA(CLR_DISP);
    NOP;
}

/* Dumps the contents of DDRAM on the terminal */
void DDRAMdump()
{
    unsigned char i = 0;//LOOP VARIABLE

    lcdgotoxy(0,0);
    for(i = 0x00; i <= 0x27; i++)
    {
        if((i % 16) == 0)
        {
            if(0 == i)
            {
                putstring("\r\n\t\t\t\t\t\tAddress\t+0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F");
                putstring("\r\n\t\t\t\t\t\t-------------------------------------------------------");
            }
            printf_tiny("\r\n");
            printf("\t\t\t\t\t\t%02X\t", i);
        }
        RS = 1;
        RWbar = 1;
        NOP;
        RD_DATA;
        printf("%X ", *(xdata unsigned char *)RD_ADDR);
    }
    lcdgotoxy(1,0);
    for(i = 0x40; i <= 0x67; i++)
    {
        if((i % 16) == 0)
        {
            if(0 == i)
            {
                putstring("\r\n\t\t\t\t\t\tAddress\t+0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F");
                putstring("\r\n\t\t\t\t\t\t-------------------------------------------------------");
            }
            printf_tiny("\r\n");
            printf("\t\t\t\t\t\t%02X\t", i);
        }
        RS = 1;
        RWbar = 1;
        NOP;
        RD_DATA;
        printf("%X ", *(xdata unsigned char *)RD_ADDR);
    }
}

/* Dumps CGRAM contents on the terminal */
void CGRAMdump()__critical
{
    unsigned char i = 0;//Loop variable
    unsigned char row_val[8] = {0x0C, 0x12, 0x10, 0x15, 0x17, 0x0D, 0x07, 0x00};// CU logo data

    FLAG = 1;
    lcdcreatechar((001), row_val);
    lcdgotoxy(1,0);
    lcdputch(001);
    FLAG = 0;

    lcdbusywait();
    RS = 0;
    RWbar = 0;
    NOP;

    WR_DATA(0x40);

    for(i = 0x00; i < 0x40; i++)
    {
        RS = 1;
        RWbar = 1;
        NOP;

        if((i % 16) == 0)
        {
            if(0 == i)
            {
                putstring("\r\n\t\t\t\t\t\tAddress\t+0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F");
                putstring("\r\n\t\t\t\t\t\t-------------------------------------------------------");
            }
            printf_tiny("\r\n");
            printf("\t\t\t\t\t\t%02X\t", i);
        }
        RD_DATA;
        printf("%02X ", *(xdata unsigned char *)RD_ADDR);
    }
}

/*
void custom_smiley()
{
    lcdgotoaddr(0x58);
    lcdputch(0x1B);
    lcdgotoaddr(0x59);
    lcdputch(0x1B);
    lcdgotoaddr(0x5A);
    lcdputch(0x00);
    lcdgotoaddr(0x5B);
    lcdputch(0x04);
    lcdgotoaddr(0x5C);
    lcdputch(0x04);
    lcdgotoaddr(0x5D);
    lcdputch(0x11);
    lcdgotoaddr(0x5E);
    lcdputch(0x0E);
    lcdgotoaddr(0x5F);
    lcdputch(0x00);
}
*/

/* Function that allows user to generate a custom character */
void menu_custchar()
{
    static unsigned char c_code = 0; //STORES THE CODE OF THE CHARACTER GENERATED
    unsigned char row_val[8] = {'\0'}; //EACH INDEX STORES VALUE FOR A ROW OF THE CUSTOM CHARACTER
    unsigned char i = 0, j = 0;//LOOP VARIABLES
    unsigned char temp[20] = {'\0'}; //STORES STRING INPUT TO BE CONVERTED TO NUMBER
    unsigned char row = 0, column = 0;//LCD COORDINATES

    putstring("\r\n\t\t\t\t\t\t\t**********************************************");
    putstring("\r\n\t\t\t\t\t\t\t*                                            *");
    putstring("\r\n\t\t\t\t\t\t\t* Enter your custom character in 1's and 0's *");
    for(i = 0; i < 8; i++)
    {
        getstring_custchar(temp);
        printf_tiny("\r\n\t\t\t\t\t\t\t\t* You entered %s", temp);
        if(32 == (row_val[i] = binStr2dec(temp, 5)))
        for(j = 0; j < 6; j++)
            temp[j] = '\0';
    }
    putstring("\r\n\t\t\t\t\t\t\t* Enter the row to display this in           *");
    row = getcharac();
    putstring("\r\n\t\t\t\t\t\t\t* Enter the column to display this in        *");
    column = getcharac();
    if((row - '0' > 3) || (column - '0'> 15))
    {
        row = 0;
        column = 8;
    }
    printf_tiny("\r\n\t\t\t\t\t\t\t* The code for this character is %d         *", c_code);
    putstring("\r\n\t\t\t\t\t\t\t*                                            *");
    putstring("\r\n\t\t\t\t\t\t\t**********************************************\r\n");

    FLAG = 1;
    lcdcreatechar((c_code % 8), row_val);
    lcdgotoxy(row - '0', column - '0');
    lcdputch(c_code);
    lcdgotoxy(0,0);
    NOP;
    FLAG = 0;
    c_code++;

}

/* Creates custom character in the CGRAM  */
void lcdcreatechar(unsigned char ccode, unsigned char *row_vals)
{
    unsigned char cgram_addr = 0x40; //BASE ADDRESS WITH 6TH BIT SET
    unsigned char k = 0;

    FLAG = 1;
    cgram_addr |= ccode << 3;
    for(k = 0; k < 8; k++, cgram_addr++)
    {
        lcdgotoaddr(cgram_addr);
        lcdputch(row_vals[k]);
    }
    FLAG = 0;
}

/* binStr2dec() checks the user input to determine if its a valid input and also evaluate its value */
unsigned char binStr2dec(unsigned char *str, unsigned char len)
{
    unsigned int ret = 0; //return value
    unsigned char i=0; // index variable
    unsigned char length;

    length = strlen(str);
    if(length != len)
    {
        error(11);//insufficient input
        return 32;
    }
    for(i = 0; i< len; i++)
    {
        if((48 <= str[i]) && (str[i] <= 49))
            ret += (str[i] - '0') * (unsigned char)powf((float)2, (float)(len-1-i));//(2^(4-i));
        else
        {
            error(11);
            return 0;//Not binary inputs
        }
    }
    return ret;
}

/* Function tat allows user to display text on the LCD */
void lcd_play()
{
    unsigned char row = 0, column = 0; //LCD COORDINATES
    unsigned char str[100] = {'\0'}; // TAKES IN USER INPUT
    unsigned char ch = 3; //STORES CODE FOR CUSTOM CHARACTER

    putstring("\r\n\t\t\t\t\t\t\tLocation [single characters]      :");
    putstring("\r\n\t\t\t\t\t\t\tRow:");
    row = getcharac();
    if((row - '0') > 3)
    {
        error(9);
        return;
    }
    putstring("\r\n\t\t\t\t\t\t\tColumn (Leaving a space for the custom char):");
    column = getcharac();
    row -= '0';
    if((column >= 'A') && (column <= 'F'))
       column = column - 'A' + 10;
    else if((column >= 'a') && (column <= 'f') )
        column = column - 'a' + 0x0A;
    else if ((column >= '0') && (column <= '9'))
        column -= '0';
    else
    {
        error(9);
        return;
    }
    printf_tiny("\r\n\t\t\t\t\t\t\t(%d,%d)", row, column);

    putstring("\r\n\t\t\t\t\t\t\tLet's Print Something on the LCD: ");
    getstring_custchar(str);

    FLAG = 1;//to prevent update of clock
    NOP;
    lcdgotoxy(row, column);
    lcdputstr(str);
    NOP;
    FLAG = 0;
}
