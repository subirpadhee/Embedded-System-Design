/*  ESD-Final Project library - NIC ENC28J60
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/


#include "main.h"
/* Globals */
static unsigned int RXreadPtr = 0;

/* Function definitions */


void nic_init()
{
    SS = 0;
    printf_tiny("\r\nIn NIC init");

    send_SPI_nic(0xFF); // to reset the chip
    delay_1ms();

    ctrl_write(ECON1, 0x04); //select bank 0
    //ctrl_write(EIE, 0X88); //Enable interrupt
    //ctrl_read(ECON1);

    ctrl_write(ERXSTL, 0x00); //ERXSTL = 0x00;
    //ctrl_read(ERXSTL);
    ctrl_write(ERXSTH, 0x00); //ERXSTH = 0x00;
    //ctrl_read(ERXSTH);
    RXreadPtr = 0x0000;

    //getcharac();
    ctrl_write(ERXNDL, 0x19); //ERXNDL = 0x19;
    ctrl_write(ERXNDH, 0xAD); //ERXNDH = 0xAD;

    ctrl_write(ERXRDPTL, 0x00); //ERXRDPTL = 0x00; - check
    ctrl_write(ERXRDPTH, 0x00); //ERXRDPTH = 0x00; - check

    ctrl_write(ETXSTL, 0x19); //ETXSTL = 0x19;
    ctrl_write(ETXSTH, 0xAE); //ETXSTH = 0xAE;

    ctrl_write(ETXNDL, 0x1F); //ETXNDL = 0x1F;
    ctrl_write(ETXNDH, 0xFF); //ETXNDH = 0xFF;

    ctrl_write(EWRPTL, 0x19); //ETXSTL = 0x19;
    ctrl_write(EWRPTH, 0xAE); //ETXSTH = 0xAE;

    ctrl_write(ECON1, 0x05); //select bank 1
    ctrl_write(ERXFCON, 0x00);// CLEAR ERXFCON-ALLOW ALL FRAMES

    //delay_1ms();

    ctrl_write(ECON1, 0x06); //select bank 2
    //ctrl_write(MACON1, 0x0C);// SET TXPAUS, RXPAUS
    ctrl_write(MACON1, 0x01);// SET MARXEN

    ctrl_write(MACON3, 0xB5);// 1 0 1 1 0 1 0 1 AUTO 64/60 PADDING;APPEND CRC;NO PROP HEADER;HUGE FR;FR LEN NOT COMP;FULL DUP

    ctrl_write(MACON4, 0x40); // 0 1 0 0 0 0 0 0 - SET DEFER BIT

    ctrl_write(MAMXFLL, 0x05);
    ctrl_write(MAMXFLH, 0xEE); // MAX FRAME LENGTH 1518 BYTES

    ctrl_write(MABBIPG, 0x15); //B2B INTER PKT GAP FOR FULL DUPLEX

    //ctrl_write(MAIPGL, 0x12);

    ctrl_write(MAADR1, MAC5); // SET MAC ADDRESS
    ctrl_write(MAADR2, MAC4); // SET MAC ADDRESS
    ctrl_write(MAADR3, MAC3); // SET MAC ADDRESS
    ctrl_write(MAADR4, MAC2); // SET MAC ADDRESS
    ctrl_write(MAADR5, MAC1); // SET MAC ADDRESS
    ctrl_write(MAADR6, MAC0); // SET MAC ADDRESS

    ctrl_write(ECON1, 0x04); //ENABLE RX
}

void ctrl_write(unsigned char addr, unsigned char data_b)
{
    addr |= (CTRL_WR << 5);
    printf("\r\nADDR: %02X DATA:%02X", addr, data_b);
    //send_SPI(addr);
    //send_SPI(data_b);
    SS = 0;
    send_SPI_nic(addr);
    send_SPI_nic(data_b);
    SS = 1;
}

unsigned char ctrl_read(unsigned char addr)
{
    addr |= (CTRL_RD << 5);

    SS = 0;
    send_SPI_nic(addr);
    send_SPI_nic(0x00);
    send_SPI_nic(0x00);
    read_data_nic();
    SS = 1;

    return buffer;
}


void bfs(unsigned char addr, unsigned char data_b)
{
    addr |= (BFS << 5);
    printf("\r\nADDR: %02X DATA:%02X", addr, data_b);
    //send_SPI(addr);
    //send_SPI(data_b);
    SS = 0;
    send_SPI_nic(addr);
    send_SPI_nic(data_b);
    SS = 1;
}
void bfc(unsigned char addr, unsigned char data_b)
{
    addr |= (BFC << 5);
    printf("\r\nADDR: %02X DATA:%02X", addr, data_b);
    //send_SPI(addr);
    //send_SPI(data_b);
    SS = 0;
    send_SPI_nic(addr);
    send_SPI_nic(data_b);
    SS = 1;
}

void buffer_write(unsigned char *data_array, unsigned int length)
{
    unsigned int i = 0;

    send_SPI_nic(BUF_WR);

    for(i = 0; i < length; i++)
    {
        send_SPI_nic(data_array[i]);
    }

    SS = 1;
}

unsigned char buffer_read(unsigned char *data_array, unsigned int length)
{
    unsigned int i = 0;

    send_SPI_nic(BUF_RD);
    for(i = 0; i < length; i++)
    {
        send_SPI_nic(0x00);
        send_SPI_nic(0x00);
        data_array[i] = read_data_nic();
    }

    return 1;
}

void packets_send(unsigned char *str, unsigned int length)
{
    unsigned char ppc_byte = PPCTRL_BYTE;

    buffer_write(&ppc_byte, 1);
    buffer_write(str, length);
    ctrl_write(ETXNDL, (0x19 + length +1));
    ctrl_write(ETXNDH, (0xAE + length +1));
    bfs(ECON1,0x80);
    bfc(ECON1,0x80);
    bfc(EIR, 0x03);
    bfs(ECON1, 0x08);//ENABLE TX
    printf("\r\n EIR-%02X", ctrl_read(EIR));
    /*printf("\r\n ECON1-%02X", ctrl_read(ECON1));
    printf("\r\n ECON2-%02X", ctrl_read(ECON2));*/

    while(ctrl_read(EIR) & 0x08 != 0x08);
    if((ctrl_read(ESTAT) & 0x02) == 0x02)
        printf("\r\nTx Aborted!");
    else
        printf("\r\nTx Done!");
    printf("\r\nETAT:%02X", ctrl_read(ESTAT));
    bfc(ECON1, 0x08);//DISABLE TX
    //printf("\r\nPhysical status reg %02X %02X", ((PHSTAT2>>8)&0XFF), ((PHSTAT2)&0XFF));
}
unsigned char packets_recv(unsigned char *str)
{
    unsigned int length = 0, i = 0;

    bfs(ECON1, 0x01);// SELECT BANK 1
    if((length = ctrl_read(EPKTCNT)) == 0)
    {
        printf("\r\nNothing received");
        return 1;
    }
    ctrl_write(ERDPTL, (RXreadPtr & 0xFF)); //POINT TO THE LOCATION TO READ FROM;
    ctrl_write(ERDPTH, ((RXreadPtr >> 8) & 0xFF)); //POINT TO THE LOCATION TO READ FROM;

    //Read packets
    buffer_read(str, length);
    //Read length
    //buffer_read(str, length);

    printf("\r\nNumber of pkts: %d", length);

    for(i = 0; i < length; i++)
    {
        if(i%16 == 0)
            printf_tiny("\r\n");
        printf("%02X ", str[i]);
    }

    return 1;
}

