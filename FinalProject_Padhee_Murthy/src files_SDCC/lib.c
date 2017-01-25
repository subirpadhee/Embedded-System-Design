/*  ESD-Final Project library
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/

#include "main.h"
#include "w5100.h"
/* Globals */
unsigned char buffer = 0;
volatile unsigned char tx_over = 0;


/* Function definitions */

/* the error() function prints the error messages based on the error code it is called with. It then sets the error flag */
unsigned char error(unsigned char c)
{
    switch(c)
    {
    case 1:
        putstring("\r\n Some Error!");
        errorFlag = 1;
        break;

        default:
            putstring("\r\nUnforeseen error");
            errorFlag = 1;
    }
    return errorFlag;
}

/*  delay function */
void delay_custom(unsigned char t)
{
    unsigned char i, j;
    for(i = 0; i <t; i++)
        for(j= 0; j < 5; j++);
}

/*  1 ms delay */
void delay_1ms()
{
    unsigned char i, j;
    for(i = 0; i <30; i++)
        for(j= 0; j < 12; j++);
}

/*  1 us delay */
void delay_custom_us(unsigned int delay)
{
    unsigned int i = 0;
    for(i = 0; i < delay; i++)
        NOP;
}

/*   ms delay */
void delay_custom_ms(unsigned int delay)
{
    unsigned int i = 0;
    for(i = 0; i < delay; i++)
        delay_1ms();
}


/* Initialize SPI */
void SPI_init()
{
    SPCON = 0x74; //Clear- set clk to 000 mode, CPOL-CPHA to 00
    SPCON |= 0x10; //Set as Master
    SPCON |= 0x20; //Set SSDIS
    SPCON |= 0x40; //Enable SPI
}

/* SEND VALUE TO SLAVE ON SPI */
unsigned char send_SPI(unsigned char value)
{
    unsigned char i = 0;
    SS = 0;
    printf("\r\nSending.. %02X, ",value);

    for(i = 0; i < 8; i++)
    {
        if (value & 0x80)
            MOSI = 1;
        else
            MOSI = 0;
        SCL = 1;
        SCL = 0;
        value <<= 1;
    }
    return SS; //return 1 on success
}

/* READ VALUE FROM SLAVE ON SPI */
unsigned char read_data()
{
    unsigned char i = 0;
    for(i = 0; i < 8; i++)
    {
        SCL = 1;
        buffer <<= 1;
        SCL = 1;
        buffer |= MISO;
        SCL = 0;
        SCL = 0;
    }
    printf("Received :%02X.\r\n",buffer);
    return buffer;
}

/* SEND VALUE TO SLAVE ON SPI - CUSTOMIZED FOR THE NIC*/
unsigned char send_SPI_nic(unsigned char value)
{
    SS = 0;
    printf("\r\nSending.. %02X, ",value);

    SPDAT = value;
    while(SPSTA != 0x80);
    SPSTA &= 0x7F;

    return SS; //return 1 on success
}

/* READ VALUE FROM SLAVE ON SPI - CUSTOMIZED FOR THE NIC */
unsigned char read_data_nic()
{
    SS = 0;
    while(SPSTA != 0x80);
    buffer = SPDAT;
    SPSTA &= 0x7F;
    printf("Received :%02X.\r\n",buffer);
    return buffer;
}

/* SEND VALUE TO SLAVE ON SPI - CUSTOMIZED FOR THE W5100*/
void write_SPI_W5100(unsigned int addr, unsigned char value)
{
    SS = 0;
    //printf("\r\nSending %02X to %04X ",value, addr);

    SPDAT = OP_WR;
    while(SPSTA != 0x80);
    SPSTA &= 0x7F;

    SPDAT = ((addr & 0xFF00) >> 8);
    while(SPSTA != 0x80);
    SPSTA &= 0x7F;

    SPDAT = (addr & 0x00FF);
    while(SPSTA != 0x80);
    SPSTA &= 0x7F;

    SPDAT = value;
    while(SPSTA != 0x80);
    SPSTA &= 0x7F;

    SS = 1;
}

/* READ VALUE FROM SLAVE ON SPI - CUSTOMIZED FOR THE W5100 */
unsigned char read_SPI_W5100(unsigned int addr)
{
    SS = 0;

    SPDAT = OP_RD;
    while(SPSTA != 0x80);
    SPSTA &= 0x7F;

    SPDAT = ((addr & 0xFF00) >> 8);
    while(SPSTA != 0x80);
    SPSTA &= 0x7F;

    SPDAT = (addr & 0x00FF);
    while(SPSTA != 0x80);
    SPSTA &= 0x7F;

    SPDAT = 0x00;
    while(SPSTA != 0x80);
    buffer = SPDAT;
    SPSTA &= 0x7F;

    SS = 1;

    return buffer;
}
