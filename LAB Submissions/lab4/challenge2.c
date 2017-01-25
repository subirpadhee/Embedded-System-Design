/*  LAB-4 library source file -Challenge 2
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"

/* Function definitions */

/* writes a byte to the EEPROM - Hard coded values are used for demonstration purpose and computing time */
unsigned char byte_write()
{
    P1_7 ^= 1;
    datasend_i2c(0x00, 0x00, 0x23);
    do
    {
        if(start_i2c())
            return(error(1));//send start failed
    }while(sendbyte_i2c(0xA0));

    P1_7 ^= 1;
    //putstring("\r\nByte Written");
}

/* Writes values to an entire page of the EEPROM. Address and data are received as arguments in the function call */
unsigned char page_write(unsigned char block, unsigned char addr, unsigned char length, unsigned char *data_b)__critical
{
    unsigned char i = 0;

    P1_7 ^= 1;
    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    if(sendbyte_i2c(block))
        return(error(2));//send control byte failed
    if(sendbyte_i2c(addr))
        return(error(3));//send address failed
    for(i = 0; i < (16 - length); i++)
    {
        if(sendbyte_i2c(data_b[i]))
            return(error(4));//send data failed
    }
    if(stop_i2c())//Stop seq
        error(1);//Stop sequence failed

    /* ACK polling*/
    do
    {
        if(start_i2c())
            return(error(1));//send start failed
    }while(sendbyte_i2c(0xA0));

    P1_7 ^= 1;
    return 0;
}

