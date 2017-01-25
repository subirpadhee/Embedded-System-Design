/*  LAB-4 library source file - I/O EXPANDER
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"

/* Function definitions */

/* function to send data to I/O expander through I2C */
unsigned char datasend_iox(unsigned char databyte)
{
    unsigned char block = 0x40;

    //printf_tiny("DATA: %X", databyte);
    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    if(sendbyte_i2c(block))
        return(error(2));//send data failed
    if(sendbyte_i2c(databyte))
        return(error(4));//send data failed
    if(stop_i2c())//Stop seq
        error(5);//Start sequence failed
}

/* Function to read a byte of data from the port pins of the IOEXPANDER using I2C */
unsigned char dataread_iox()
{
    unsigned char databyte = 0;
    unsigned char i = 0;
    unsigned char block = 0x41;

    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    if(sendbyte_i2c(block))
        return(error(2));//send data failed

    /* Read data*/
    for(i = 0; i < 8; i++)
    {
        SCL = 1;
        NOP;
        databyte |= (SDA << (7-i)) ;// READ (7-i)th bit of from EEPROM
        NOP;
        SCL = 0;
    }
    SDA = 1;
    SCL = 1;
    NOP;
    SCL = 0;

    if(stop_i2c())//Stop seq
        error(5);//Stop sequence failed

    return(databyte);
}

/* I/O expander UI */
void ioexpander()
{
    unsigned char i = 0;
    unsigned char port_io[9] = {'\0'};
    unsigned char ch = '\0';

    while(1)
    {
        putstring("\r\n\t\t\t\t\t\t************************************************************");
        putstring("\r\n\t\t\t\t\t\t*                                                          *");
        putstring("\r\n\t\t\t\t\t\t* Do you wish to configure I/O pins [Y/N]?                 \r\n*");

        ch = getcharac();
        if(('Y' == ch) || ('y' == ch))
        {
            putstring("\r\n\t\t\t\t\t\t* Enter 1's for the pins you want as INPUT; 0's for OUTPUT *");
            putstring("\r\n\t\t\t\t\t\t* P7 P6 P5 P4 P3 P2 P1 P0                                  *");
            putstring("\r\n\t\t\t\t\t\t* ");
            getstring_custchar(port_io);
            putstring("\t\t\t\t\t\t\t\t\t* ");
            //printf_tiny("Entered: %s", port_io);

            port_io_val = binStr2dec(port_io, 8);

            //printf_tiny("Entered: %d", port_io_val);
            datasend_iox(port_io_val);
        }
        else
        {
            datasend_iox(0x80);
        }

        putstring("\r\n\t\t\t\t\t\t*                                                          *");
        putstring("\r\n\t\t\t\t\t\t* Press 'C' to Check Status                                *");
        putstring("\r\n\t\t\t\t\t\t* Press 'R' to Reset the counter                           *");
        putstring("\r\n\t\t\t\t\t\t* Press 'E' to get back to previous menu                   \r\n*");

        ch = getcharac();
        //port_io_val = 0;
        if(('C' == ch) || ('c' == ch))
        {
            port_io_val = dataread_iox();
            putstring("\r\n\t\t\t\t\t\t*                                                          *");
            printf("\r\n\t\t\t\t\t\t* \t\t\tSTATUS: %X       \t\t\t*", port_io_val);
            for(i = 0; i < 8; i++)
            {
                printf("\r\n\t\t\t\t\t\t*\t\t\tPin%d\t%s  \t\t  *",(7-i), ((port_io_val & (0x80 >> i)) ? "INPUT" : "OUTPUT"));
            }
            putstring("\r\n\t\t\t\t\t\t************************************************************");
        }
        else if(('R' == ch) || ('r' == ch))
        {
            cnt_iox = 0;
        }
        else if(('E' == ch) || ('e' == ch))
            break;
    }
}

