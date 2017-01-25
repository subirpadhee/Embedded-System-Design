/*  ESD-Final Project header - w5100.h
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/
#ifndef w5100_h
#define w5100_h

/* REGISTERS */
/* Common Registers*/

#define MR 0x0000 //MODE REG
#define GAR0 0x0001 //GATEWAY
#define SUBR0 0x0005 //SUBNET MASK
#define SHAR0 0x0009 //SOURCE MAC ADDRESS
#define SIP0 0x000F //SOURCE IP
#define IR 0x0015
#define IMR 0x0016
#define RTR0 0x0017
#define RCR 0x0019
#define RMSR 0x001A
#define TMSR 0x001B
#define SHAR0 0x0009

/* Socket Registers */
//SOCKET 0
#define S0_MR 0x0400
#define S0_CR 0x0401
#define S0_IR 0x0402
#define S0_SR 0x0403
#define S0_PORT0 0x0404
#define S0_DHAR0 0x0406
#define S0_DIPR0 0x040C
#define S0_DPORT0 0x0410
#define S0_TOS 0x0415
#define S0_TTL 0x0416
#define S0_TX_FSR0 0x0420 //Tx FREE SIZE
#define S0_TX_RD0 0x0422 //Tx READ POINTER
#define S0_TX_WR0 0x0424 //Tx WRITE POINTER
#define S0_RX_RSR0 0x0426 //RECEIVED SIZE
#define S0_RX_RD0 0x0428 //RX READ POINTER


#define S0_RX_BASE 0x6000
#define S0_TX_BASE 0x4000
#define S0_RX_MASK 0x1FFF
#define S0_TX_MASK 0x1FFF

//SOCKET 1
#define S1_MR 0x0400
#define S1_CR 0x0401
#define S1_IR 0x0402
#define S1_SR 0x0403
#define S1_PORT0 0x0404
#define S1_DHAR0 0x0406
#define S1_DIPR0 0x040C
#define S1_DPORT0 0x0410
#define S1_TOS 0x0415
#define S1_TTL 0x0416
#define S1_TX_FSR0 0x0420
#define S1_TX_RD0 0x0422
#define S1_TX_WR0 0x0424
#define S1_TX_RSR0 0x0426
#define S1_RX_RD0 0x0428

//SOCKET 2
#define S2_MR 0x0400
#define S2_CR 0x0401
#define S2_IR 0x0402
#define S2_SR 0x0403
#define S2_PORT0 0x0404
#define S2_DHAR0 0x0406
#define S2_DIPR0 0x040C
#define S2_DPORT0 0x0410
#define S2_TOS 0x0415
#define S2_TTL 0x0416
#define S2_TX_FSR0 0x0420
#define S2_TX_RD0 0x0422
#define S2_TX_WR0 0x0424
#define S2_TX_RSR0 0x0426
#define S2_RX_RD0 0x0428

//SOCKET 3
#define S3_MR 0x0400
#define S3_CR 0x0401
#define S3_IR 0x0402
#define S3_SR 0x0403
#define S3_PORT0 0x0404
#define S3_DHAR0 0x0406
#define S3_DIPR0 0x040C
#define S3_DPORT0 0x0410
#define S3_TOS 0x0415
#define S3_TTL 0x0416
#define S3_TX_FSR0 0x0420
#define S3_TX_RD0 0x0422
#define S3_TX_WR0 0x0424
#define S3_TX_RSR0 0x0426
#define S3_RX_RD0 0x0428

/* OPCODES */
#define OP_WR 0xF0
#define OP_RD 0x0F

/* COMMANDS  */
#define OPEN 0x01
#define LISTEN 0x02
#define CONNECT 0x04
#define DISCON 0x08
#define CLOSE 0x10
#define SEND 0x20
#define SEND_KEEP 0x22
#define RECV 0x40

/* SOCKET STATUS */
#define SOCK_CLOSED 0x00
#define SOCK_INIT 0x13
#define SOCK_LISTEN 0x14
#define SOCK_ESTABLISHED 0x17
#define SOCK_FIN_WAIT 0x18
#define SOCK_CLOSING 0x1A
#define SOCK_TIME_WAIT 0x1B
#define SOCK_CLOSE_WAIT 0x1C
#define SOCK_LAST_ACK 0x1D

/* NUMBERS AND ALPHA IN HEX*/

#define QUIT 0x1B
#define NUM0 0x30
#define NUM1 0x31
#define NUM3 0x33
#define NUM4 0x34
#define NUM7 0x37
#define NUM9 0x39

#define ALPHA_F 0x46
#define ALPHA_a 0x61
#define ALPHA_f 0x66
#define ALPHA_A 0x41

/* Function Definitions */
void w5100_init();
void write_SPI_W5100(unsigned int addr, unsigned char value);
unsigned char read_SPI_W5100(unsigned int addr);
unsigned char setup_server();
unsigned char receiveFromClient();
void parseTcpOpcodes(unsigned int baseAddress, unsigned char *buffer);
void dataout(unsigned int x, unsigned char y);
void blankCheck();
unsigned char sendToClient(unsigned char *buffer_tx);

#endif //w5100
