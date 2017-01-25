/*  ESD-Final Project library - SD CARD
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/

#include "main.h"


/* Function definitions */
typedef union           //Union to send address bytes one by one
{
    unsigned char b[4];
    unsigned long ul;
} b_ul;

b_ul fsz;

void sdcard_init()
{
    unsigned char i = 0;
    //CMD0
    for(i = 0; i<11 ;i++)
    {
        send_SPI(0xFF);
    }

    send_SPI(0x40);
    send_SPI(0x00);
    send_SPI(0x00);
    send_SPI(0x00);
    send_SPI(0x00);
    send_SPI(0x95);
    send_SPI(0xFF);
    read_data();

    //CMD1
    SS =1;
        send_SPI(0xFF);
        delay_1ms();
        SS=0;
        send_SPI(0x48);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0x01);
        send_SPI(0xAA);
        send_SPI(0x87);

    do
    {
/*
        send_SPI(0x41);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0xFF);


        send_SPI(0x69);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0xFF);

        send_SPI(0x7A);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0xFF);
*/



        send_SPI(0xFF);
        printf_tiny("\r\n***************");
        read_data();
    }while(buffer != 0x00);
    // OCR

      send_SPI(0x7A);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0x00);
        send_SPI(0xFF);

    for(i = 0; i< 5;i++)
    {
        send_SPI(0xFF);
        printf_tiny("\r\n***************");
        read_data();
      //  delay_1ms();
    }



}

unsigned char sd_single_read(unsigned long sector)
{
    int counter,i,ctr0,ctr1,ctr2,ctr3;
    xdata unsigned char *buff;

    char save_data;
    b_ul temp1;

    temp1.ul = sector;

    //CMD 17 Single Data block read

    send_SPI(0xFF);            // Dummy byte & Wait for end of transmission
    send_SPI(0XFF);

    counter = 3;                 // Argument 4 bytes
    while(counter >=0)
    {
         send_SPI(temp1.b[counter]);
         counter--;

    }

    for(i=0;i<6;i++)
    {
        send_SPI(0xFF);
        //save_data = serial_data;
    }

    ctr0=(temp1.b[1] & 0xff)-1;
    ctr1= (temp1.b[1] & 0xff);
    ctr2= (temp1.b[2] & 0xff);
    ctr3= (temp1.b[3] & 0xff);

    // Print address
    printf("\n\r%02x%02x%02x%02x: \n\r",(temp1.b[3] & 0xff),(temp1.b[2] & 0xff),(temp1.b[1] & 0xff),(temp1.b[0] & 0xff));

    for(i=0;i<512;i++)
    {
        send_SPI(0xFF);
        save_data = read_data();

        delay_1ms();

        buff[i] = save_data;

        ctr0++;
        if(i == 256)
        {
            ctr1++;
            ctr0=0;
        }
        if(((i%16)==0) && (i!=0))
        {
          putchar('\n');
          putchar('\r');

          printf("0x%02x  ",read_data());
        }
        else
        {
            printf("0x%02x  ",read_data());
        }

    }
    for(i=0;i<4;i++)
    {
        send_SPI(0xFF);            // Dummy byte & Wait for end of transmission
        //save_data = serial_data;
    }
    return 1;
}


void readBytesSd()
{
    unsigned int i;
    send_SPI(0x51);
    for ( i=0; i<4;i++)
        send_SPI(0x00);
    send_SPI(0xFF);
    //CMD1
    SS =1;
    send_SPI(0xFF);
    delay_1ms();
    SS=0;

    for ( i=0; i < 514;i++)
    {
        send_SPI(0xFF);
        read_data();
    }

}
