/*  LAB-4 library source file - Challenge 3
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"

/* Function definitions */

/* Writes values to a block of memory of the EEPROM. Address and data are received as arguments in the function call */
void blockfill()
{
    unsigned char eeAddrB[5]; //STARRT ADDRESS - STRING
    unsigned char eeAddrE[5]; //END ADDRESS - STRING
    unsigned char data_fill[4] = {'\0'}; //DATA-STRING
    unsigned int rcvdb = 0; //START ADDRESS - INTEGER
    unsigned int rcvde = 0; //END ADDRESS - INTEGER
    unsigned char addr = 0; //STORES WORD ADDRESS
    unsigned char data_b[16] = {'\0'}; //ARRAY OF DATA FOR ONE PAGE
    unsigned int data_int; //DATA - INTEGER
    unsigned char block = 0; //STORES CONTROL BYTE
    unsigned char length = 0; //NUMBER OF BYTES TO BE WRITTEN TO A PARTICULAR PAGE OF THE EEPROM

    unsigned int i = 0; //LOOP VAR
    unsigned int j = 0; //LOOP VAR

    putstring("\r\n\t\t\t\t\t\t\t*****************************************");
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t* EEPROM Start Address                   *");
    putstring("\r\n\t\t\t\t\t\t\t* 0x");
    getstring(eeAddrB);
    if((rcvdb = checkInput(eeAddrB)) > 2047)
    {
        error(7);
        return;
    }
    putstring("\r\n\t\t\t\t\t\t\t* EEPROM End Address                     *");
    putstring("\r\n\t\t\t\t\t\t\t* 0x");
    getstring(eeAddrE);
    putstring("\r\n\t\t\t\t\t\t\t* Data to Fill:                          *");
    putstring("\r\n\t\t\t\t\t\t\t* 0x");
    getstring_blockfill(data_fill);
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");


    if((rcvde = checkInput(eeAddrE)) > 2047)
    {
        error(7);
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");
        return;
    }

    if((data_int = checkInput(data_fill)) > 2047) //2048 is returned on error
    {
        error(7);
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");
        return;
    }
    for(i = 0; i < 16; i++)
    {
        data_b[i] = data_int & 0xFF;
    }
    if(rcvdb > rcvde)
    {
        error(10);
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");
        return;
    }
    EA = 0;
    #ifdef FAST
    CKCON0 = 0x01; /* enable X2 mode */
    #endif
    //TR2 = 1;//to compute time
    P1_1 ^= 1;
    for(j = rcvdb; (j & 0xFF0) <= rcvde; j += 16)
    {
        length = 0;
        addr = j & 0xF0;
        block = (j >> 8) & 0x0F;

        block <<= 1;
        block |= 0xA0;//device = 0xA0 for EEPROM
        block &= 0xFE;
        if(j == rcvdb)
        {
            addr = j & 0xFF;
            length = addr & 0x0F;
        }
        if(j >= (rcvde & 0xFF0))
        {
            length = 15 - (rcvde & 0x00F);
        }
        if(page_write(block, addr, length, data_b))
            error(12);
    }
    P1_1 ^= 1;
    #ifdef FAST
    CKCON0 = 0x00; /* Disable X2 mode */
    #endif
    EA = 1;
    //TR2 = 0;//to compute time
}
