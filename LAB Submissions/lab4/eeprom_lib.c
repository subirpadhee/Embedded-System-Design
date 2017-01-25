/*  LAB-4 library source file - EEPROM
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"

/* Function definitions */

/* PROVIDES UI FOR WRITING DATA INTO THE EEPROM */
void menu_write()
{
    unsigned char eeAddr[5]; //EEPROM ADDRESS - STRING
    unsigned char eeData[5]; //DATA TO BE STORED IN EEPROM - STRING
    unsigned int rcvd = 0; //ADDRESS - INTEGER
    unsigned char addr = 0; //WORD ADDRESS
    unsigned char data_b = 0; //DATA TO BE STORED IN EEPROM
    unsigned char block = 0; //CONTROL BYTE FOR EEPROM

    putstring("\r\n\t\t\t\t\t\t\t*****************************************");
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t* EEPROM Address                        *");
    putstring("\r\n\t\t\t\t\t\t\t* 0x");
    getstring(eeAddr);
    if((rcvd = checkInput(eeAddr)) > 2047)
    {
        error(7);
        return;
    }
    addr = rcvd & 0xFF;
    block = (rcvd >> 8) & 0xFF;
    putstring("\r\n\t\t\t\t\t\t\t* Data                                 *");
    putstring("\r\n\t\t\t\t\t\t\t* 0x");
    getstring(eeData);
    if((rcvd = checkInput(eeData)) > 255)
    {
        error(8);
        return;
    }
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");

    data_b = rcvd & 0xFF;

    datasend_i2c(block, addr, data_b);
}

/* UI to read data from the EEPROM */
void menu_read()
{
    unsigned char eeAddr[5]; //EEPROM ADDRESS - STRING
    unsigned int rcvd = 0; //ADDRESS - INTEGER
    unsigned char addr = 0; //WORD ADDRESS
    unsigned char data_b = 0; //DATA TO BE STORED IN EEPROM
    unsigned char block = 0; //CONTROL BYTE FOR EEPROM

    putstring("\r\n\t\t\t\t\t\t\t*****************************************");
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t* EEPROM Address                        *");
    putstring("\r\n\t\t\t\t\t\t\t* 0x");
    getstring(eeAddr);
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");

    if((rcvd = checkInput(eeAddr)) > 2047)
    {
        error(7);
        return;
    }
    addr = rcvd & 0xFF;
    block = (rcvd >> 8) & 0xFF;

    data_b = dataread_i2c(block, addr);
    printf("\r\n\t\t\t\t\t\t\t* %X%02X:%X                                *", block, addr, data_b);
}

/* Function to generate START sequence for I2C communication */
unsigned char start_i2c()
{
    /* Start condition creation */
    SDA = 1;
    SCL = 1;
    NOP;
    SDA = 0;
    NOP;
    SCL = 0; /* Ready to send data */

    return(SCL);//return 0 for success
}

/* Function to generate STOP sequence for I2C communication */
unsigned char stop_i2c()
{
    /* Stop condition creation */
    SDA = 0;
    SCL = 1;
    NOP;
    SDA = 1;
    NOP;
    SCL = 0; /* Ready for SDA to change */

    return(SCL);//return 00 for success
}

/* Function to write a byte to EEPROM using I2C */
unsigned char sendbyte_i2c(unsigned char databyte)
{
    unsigned char i = 0;
    unsigned char ACK = 1;

   /* Send data */
    for(i = 0; i < 8; i++)
    {
        SDA = (databyte & (0x80 >> i)) > 0 ? 1:0;// WRITE (7-i)th bit of databyte

        SCL = 1;
        NOP;
        SCL = 0;
    }

    SDA = 1; //Set SDA high so that Receiver can pull it down for ack
    SCL = 1;
    NOP;

    ACK = SDA;
    SCL = 0;

    //printf("\r\n ACK is: %d", ACK);
    return (ACK);// If ACK rcvd, then return 0
}

void eereset()
{
    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    if(sendbyte_i2c(RESET_byte))
        (error(4));//send data failed
    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    if(stop_i2c())//Stop seq
        error(5);//Start sequence failed
}

/* Function to write a byte of data to EEPROM at a specified address using I2C */
unsigned char datasend_i2c(unsigned char block, unsigned char addr, unsigned char databyte)
{
    block <<= 1;
    block |= 0xA0;//device = 0xA0 for EEPROM
    block &= 0xFE;

    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    if(sendbyte_i2c(block))
        return(error(2));//send data failed
    if(sendbyte_i2c(addr))
        return(error(3));//send data failed
    if(sendbyte_i2c(databyte))
        return(error(4));//send data failed
    if(stop_i2c())//Stop seq
        error(5);//Start sequence failed
}

/* Function to read a byte of data from a specified address in EEPROM using I2C */
unsigned char dataread_i2c(unsigned char block, unsigned char addr)
{
    unsigned char databyte = 0;
    unsigned char i = 0;
    unsigned char SDA_var = 0;

    block <<= 1;
    block |= 0xA0;
    block &= 0xFE;

    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    if(sendbyte_i2c(block))
        return(error(2));//send data failed
    if(sendbyte_i2c(addr))
        return(error(3));//send data failed
    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    block |= 0x01;
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

/* function to display EEPROM contents on the terminal */
void hexdumpEEPROM()
{
    unsigned char eeAddrB[5]; //STARRT ADDRESS - STRING
    unsigned char eeAddrE[5]; //END ADDRESS - STRING
    unsigned int rcvdb = 0; //START ADDRESS - INTEGER
    unsigned int rcvde = 0; //END ADDRESS - INTEGER
    unsigned char addr = 0; //STORES WORD ADDRESS
    unsigned char data_b = 0; //ARRAY OF DATA FOR ONE PAGE
    unsigned char block = 0; //STORES CONTROL BYTE

    unsigned int i = 0;

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
    //putstring("              *");
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");


    if((rcvde = checkInput(eeAddrE)) > 2047)
    {
        error(7);
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");
        return;
    }

    if(rcvdb > rcvde)
    {
        error(10);
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");
        return;
    }

    for(i = 0; i <= (rcvde - rcvdb); i++)
    {
        addr = (rcvdb + i) & 0xFF;
        block = ((rcvdb +i) >> 8) & 0xFF;
        data_b = dataread_i2c(block, addr);
        if((i % 16) == 0)
        {
            if(0 == i)
            {
                putstring("\t\t\t\t\t\tAddress\t+0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F");
                putstring("\r\n\t\t\t\t\t\t-------------------------------------------------------");
            }
            printf_tiny("\r\n");
            printf("\t\t\t\t\t\t0x%03X\t", (i + rcvdb));
        }
        printf("%02X ", data_b);
    }
}

/*void hexdumpEEPROM_sequential()
{
    unsigned char eeAddrB[5];
    unsigned char eeAddrE[5];
    unsigned int rcvdb = 0;
    unsigned int rcvde = 0;
    unsigned char addr = 0;
    unsigned char data_b = 0;
    unsigned char block = 0;

    unsigned int i = 0;

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
    //putstring("              *");
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");


    if((rcvde = checkInput(eeAddrE)) > 2047)
    {
        error(7);
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");
        return;
    }

    if(rcvdb > rcvde)
    {
        error(10);
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");
        return;
    }

    dataread_i2c_sequential_first(block, addr);


    for(i = 0; i < (rcvde - rcvdb); i++)
    {
        addr = (rcvdb + i) & 0xFF;
        block = ((rcvdb +i) >> 8) & 0xFF;
        data_b = dataread_i2c_sequential();
        if((i % 16) == 0)
        {
            if(0 == i)
            {
                putstring("\t\t\t\t\t\tAddress\t+0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F");
                putstring("\r\n\t\t\t\t\t\t-------------------------------------------------------");
            }
            printf_tiny("\r\n");
            printf("\t\t\t\t\t\t0x%03x\t", i);
        }
        printf("%02X ", data_b);

        if(i == (rcvde - rcvdb)-1)
        {
            if(stop_i2c())//Stop seq
                error(1);//Stop sequence failed
        }
    }
}

unsigned char dataread_i2c_sequential_first(unsigned char block, unsigned char addr)
{
    unsigned char databyte = 0;
    unsigned char i = 0;
    unsigned char SDA_var = 0;

    block <<= 1;
    block |= 0xA0;
    block &= 0xFE;

    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    if(sendbyte_i2c(block))
        return(error(2));//send data failed
    if(sendbyte_i2c(addr))
        return(error(3));//send data failed
    if(start_i2c())//Start seq
        error(1);//Start sequence failed
    block |= 0x01;
    if(sendbyte_i2c(0xA1))
        return(error(2));//send data failed
    NOP;
}


unsigned char dataread_i2c_sequential()
{
    unsigned char databyte = 0;
    unsigned char i = 0;
    unsigned char SDA_var = 0;

    for(i = 0; i < 8; i++)
    {
        SCL = 1;
        NOP;
        databyte |= (SDA << (7-i)) ;// READ (7-i)th bit of from EEPROM
        NOP;
        SCL = 0;
    }
    SDA = 0;
    SCL = 1;
    NOP;
    SCL = 0;

    return(databyte);
}
*/

