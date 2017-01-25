/*  ESD-Final Project library - W5100
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/
#include "main.h"
#include "w5100.h"
#include "glcd.h"

/* Globals */
__xdata unsigned int* ptr;
__xdata at 0x7000 unsigned char buffer_rx[4095];

/* Function definitions */

void nic()
{
    volatile unsigned char connection_st = 0;

    w5100_init();
    delay_1ms();
    setup_server(); // SET UP THE SERVER
    //while(!setup_server());// SET UP THE SERVER
    while(1)
    {
        connection_st = read_SPI_W5100(S0_SR);
        switch(connection_st)
        {
        case SOCK_CLOSED:
            setup_server(); // SET UP THE SERVER
            break;

        case SOCK_ESTABLISHED:
            //printf("\r\n\tESTD");
            receiveFromClient();// STORE THE PACKETS RECEIVED AND ACT ACCORDINGLY
            break;

        case SOCK_FIN_WAIT:
        case SOCK_CLOSING:
        case SOCK_TIME_WAIT:
        case SOCK_CLOSE_WAIT:
        case SOCK_LAST_ACK:
            write_SPI_W5100(S0_CR,CLOSE); // CLOSE THE SOCKET IF THE CONNECTION STATUS IS ANYTHING BUT CLOSED OR ESTABLISHED
            while(read_SPI_W5100(S0_SR));

            break;

        default:
            //printf_tiny("\r\nSome status not handled!");
            break;
        }
    }
}

void w5100_init()
{
    write_SPI_W5100(MR, 0x80); //RESET
    delay_1ms();
    //printf("\r\nReading MR: %d\n\n",read_SPI_W5100(MR));

        /* Set Network information */
    //GATEWAY SET TO 192.168.1.0
    write_SPI_W5100(GAR0, 192);
    write_SPI_W5100((GAR0+1), 168);
    write_SPI_W5100((GAR0+2), 0);
    write_SPI_W5100((GAR0+3), 1);

    delay_1ms();

     printf("\r\nDefault Gateway: %d.%d.%d.%d\n\n",read_SPI_W5100(GAR0 + 0),read_SPI_W5100(GAR0 + 1),\
          read_SPI_W5100(GAR0 + 2),read_SPI_W5100(GAR0 + 3));

    //SOURCE MAC ADDRESS SET TO FC.3F.DB.38.3A.6A
    write_SPI_W5100((SHAR0), 0xFC);
    write_SPI_W5100((SHAR0+1), 0x3F);
    write_SPI_W5100((SHAR0+2), 0xDB);
    write_SPI_W5100((SHAR0+3), 0x38);
    write_SPI_W5100((SHAR0+4), 0x3A);
    write_SPI_W5100((SHAR0+5), 0x6A);

    delay_1ms();
    printf("\r\nMAC Address: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n\n",read_SPI_W5100(SHAR0 + 0),read_SPI_W5100(SHAR0 + 1),\
          read_SPI_W5100(SHAR0 + 2),read_SPI_W5100(SHAR0 + 3),read_SPI_W5100(SHAR0 + 4),read_SPI_W5100(SHAR0 + 5));

    //SUBNET MASK SET TO 255.255.255.0
    write_SPI_W5100(SUBR0, 255);
    write_SPI_W5100((SUBR0+1), 255);
    write_SPI_W5100((SUBR0+2), 255);
    write_SPI_W5100((SUBR0+3), 0);

    delay_1ms();
    printf("\r\nSubnet Mask: %d.%d.%d.%d\n\n",read_SPI_W5100(SUBR0 + 0),read_SPI_W5100(SUBR0 + 1),\
          read_SPI_W5100(SUBR0 + 2),read_SPI_W5100(SUBR0 + 3));

    //SOURCE IP SET TO 192.168.0.3
    write_SPI_W5100(SIP0, 192);
    write_SPI_W5100((SIP0+1), 168);
    write_SPI_W5100((SIP0+2), 0);
    write_SPI_W5100((SIP0+3), 3);

    delay_1ms();

    printf("\r\nConfigured IP: %d.%d.%d.%d\n\n",read_SPI_W5100(SIP0 + 0),read_SPI_W5100(SIP0 + 1),\
          read_SPI_W5100(SIP0 + 2),read_SPI_W5100(SIP0 + 3));

    //MEMORY FOR EACH SOCKET - S0:8KB REST:0
    write_SPI_W5100(RMSR, 0x03);
    write_SPI_W5100(TMSR, 0x03);

    delay_1ms();

    //MODE REG
    write_SPI_W5100(IMR, 0xCF); // IP CONFLICT, DEST UNREACHABLE, ALL SOCKETS
    write_SPI_W5100((RTR0), 0x0F); // RTR SET TO 400ms
    write_SPI_W5100(RTR0+1, 0xA0); //
    write_SPI_W5100(RCR, 0x05); // RCR SET TO 5 TRIES

    printf_tiny("Initialization routine over!\n");
}

/* FUNCTION TO SETUP THE SERVER */
unsigned char setup_server()
{
    // Send Close Command
    write_SPI_W5100(S0_CR,CLOSE);
    // Waiting until the S0_CR is clear
    while(read_SPI_W5100(S0_CR) == OPEN)
    {
        read_SPI_W5100(S0_SR);
        return;
    }

    write_SPI_W5100(S0_MR, 0x01); //SOCKET 0 - ON TCP
    //printf("\r\nReading SO_MR: %02X",read_SPI_W5100(S0_MR));
    write_SPI_W5100(S0_PORT0, 0x13);//S0_PORT 5097 - 0X13E9
    write_SPI_W5100((S0_PORT0+1), 0x88);//S0_PORT 5097 - 0X13E9
    printf("\r\nPORT number for connection: 0x%X%X",read_SPI_W5100(S0_PORT0), read_SPI_W5100(S0_PORT0+1));

    write_SPI_W5100(S0_CR, OPEN); // INITIALIZE S0 AS TCP SERVER SOCKET
    delay_1ms();

    if(read_SPI_W5100(S0_SR) != SOCK_INIT)
    {
        write_SPI_W5100(S0_CR, CLOSE);
        printf("\r\nCould not setup server.  S0_SR:%02X ",read_SPI_W5100(S0_SR));
        return 0;//FAIL
    }
    else
        printf_tiny("\r\nServer Setup!");
    write_SPI_W5100(S0_CR, LISTEN); //MAKE THE SERVER READY TO ACCEPT CONNECTIONS
    delay_1ms();
    if(read_SPI_W5100(S0_SR) != SOCK_LISTEN)
    {
        write_SPI_W5100(S0_CR, CLOSE);
        printf_tiny("\r\nCould not Listen");
        return 0;//FAIL
    }
    else
        printf_tiny("\r\nI'm listening !");

    return 1; //SUCCESSFULLY SETUP THE SERVER
}

/* FUNCTION TO SEND PACKETS TO THE CLIENT */
unsigned char sendToClient(unsigned char *buffer_tx)
{
    unsigned int buf_sz = 0, upper_size = 0, remaining_size = 0; //HOLD SIZE OF DATA TO BE SENT
    unsigned int offset_tx = 0; // OFFSET TO THE BASE OF TX BUFFER TO WHICH DATA TO BE TRANSMITTED IS WRITTEN
    unsigned int offset_NOmask = 0; // RAW OFFSET TO THE BASE OF TX BUFFER TO WHICH DATA TO BE TRANSMITTED IS WRITTEN
    unsigned int start_addr = 0; //ABSOLUTE ADDRESS TO WHICH DATA TO BE TRANSMITTED IS WRITTEN
    unsigned int j = 0; // LOOP VARIABLE
    unsigned char lb = 0; //TEMPORARY VARIABLE
    unsigned int ub = 0, free_sz = 0; //TEMPORARY VARIABLE
    unsigned int i = 0; // LOOP VARIABLE

    ub = read_SPI_W5100(S0_TX_FSR0);
    delay_1ms();
    lb = read_SPI_W5100(S0_TX_FSR0+1);

    free_sz = ((ub & 0x00FF) << 8) | lb; //READ THE FREE SPACE AVAILABLE IN THE TX BUFFER
    NOP;

    if(free_sz != 0)
    {
        offset_NOmask = (((unsigned int)read_SPI_W5100(S0_TX_WR0) & 0x00FF) << 8) | read_SPI_W5100(S0_TX_WR0 + 1);
        offset_tx = offset_NOmask & S0_TX_MASK;
        NOP;
        start_addr = S0_TX_BASE + offset_tx; //DETERMINE THE ABSOLUTE ADDRESS TO WHICH DATA TO BE TRANSMITTED IS WRITTEN
        i=0;
        buf_sz = strlen(buffer_tx);

        if(offset_tx + buf_sz > S0_TX_MASK + 1) //IF DATA TO BE TRANSMITTED NEEDS TO BE WRAPPED AROUND THE BUFFER
        {
            upper_size = S0_TX_MASK +1 -offset_tx;
            for(i= 0; i < upper_size; i++)
            {
                write_SPI_W5100(start_addr+i, buffer_tx[j++]);
            }
            remaining_size = buf_sz - upper_size;
            for(i= 0; i < remaining_size; i++)
            {
                write_SPI_W5100(S0_TX_BASE+i, buffer_tx[j++]);
            }
        }
        else
        {
            for(i= 0; i < buf_sz; i++)
            {
                write_SPI_W5100(start_addr+i, buffer_tx[j++]);
                delay_1ms();
            }
        }
        write_SPI_W5100(S0_TX_WR0,((offset_NOmask + buf_sz) & 0xFF00) >> 8 ); // UPDATE THE WRITE POINTER AFTER THE WRITE
        write_SPI_W5100(S0_TX_WR0 + 1,((offset_NOmask + buf_sz) & 0x00FF));

        write_SPI_W5100(S0_CR, SEND); //INDICATE TO THE CHIP TO INITIATE TRANSMISSION
        NOP;
    }
    else
    {
        printf_tiny("\r\nCan't Send-No memory");
    }
}

unsigned char receiveFromClient()
{
    unsigned int rcvd_size = 0, upper_size = 0, remaining_size = 0; //HOLD SIZE OF DATA RECEIVED
    unsigned int offset_rx = 0; // OFFSET TO THE BASE OF RX BUFFER FROM WHICH DATA IS TO BE READ
    unsigned int offset_NOmask = 0;
    unsigned int start_addr = 0; // ABSOLUTE ADDRESS OF THERX BUFFER FROM WHICH DATA IS TO BE READ
    unsigned int j = 0; //LOOP VARIABLE
    unsigned char buffer_tx[100] = {'\0'}; //HOLDS DATA TO BE TRANSMITTED- INSTANT MESSENGER FUNCTIONALITY
    volatile static unsigned char CHAT_FLAG = 0,WEB_PAGE_FLAG=0; //FLAGS FOR IM AND HTTP SERVER APPLICATIONS
    unsigned int ub = 0, lb = 0; //TEMPORARY VARIABLES
    unsigned int i = 0; //LOOP VARIABLE

    for(i = 0; i < 100; i++)
    {
        buffer_tx[i] = '\0';
    }
    ub = read_SPI_W5100(S0_RX_RSR0);
    delay_1ms();
    lb = read_SPI_W5100(S0_RX_RSR0+1);
    rcvd_size = (ub << 8) | (lb & 0x00FF); // DETERMINE RECEIVED SIZE
    delay_1ms();
    if(rcvd_size == 0);
        printf_tiny("\r\nReceived nothing");

    if(rcvd_size != 0)
    {
        offset_NOmask = ((read_SPI_W5100(S0_RX_RD0) & 0x00FF) << 8) | read_SPI_W5100(S0_RX_RD0 + 1);
        offset_rx = offset_NOmask & S0_RX_MASK;
        if(CHAT_FLAG == 0)
            printf("\r\noffset_rx:%04X", offset_rx);

        start_addr = S0_RX_BASE + offset_rx; // DETERMINE START ADDRESS

        if(offset_rx + rcvd_size > S0_RX_MASK + 1) // IF RECEIVED PACKETS WRAP AROUND THE RECEIVE BUFFER
        {
            upper_size = S0_RX_MASK +1 -offset_rx;
            for(i= 0; i < upper_size; i++)
            {
                delay_1ms();
                buffer_rx[j++] = read_SPI_W5100(start_addr+i); //STORE THE DATA RECEIVED
            }
            remaining_size = rcvd_size - upper_size;
            for(i= 0; i < remaining_size; i++)
            {
                delay_1ms();
                buffer_rx[j++] = read_SPI_W5100(start_addr+i);
            }
        }
        else
        {
            for(i= 0; i < rcvd_size; i++)
            {
                delay_1ms();
                if(i%20 == 0)
                    printf("\r\n");
                buffer_rx[j++] = read_SPI_W5100(start_addr+i); //STORE THE DATA RECEIVED
                delay_1ms();
            }
        }
        buffer_rx[j] = '\0';

        write_SPI_W5100(S0_RX_RD0,((offset_NOmask + rcvd_size) & 0xFF00) >> 8 ); //UPDATE THE READ POINTER
        write_SPI_W5100(S0_RX_RD0 + 1,((offset_NOmask + rcvd_size) & 0x00FF));

        write_SPI_W5100(S0_CR, RECV); //INDICATE TO THE CHIP THAT RECEPTION IS COMPLETE. THE STATUS REGISTER CHANGES ITS VALUE ACCORDINGLY

        /* CHAT */
        if((buffer_rx[0] == '*') || (CHAT_FLAG == 1))
        {
            if(buffer_rx[0] == '*')
            {
                printf("\r\nGUEST:\tLET'S TALK!\r\n");
            }
            else
            {
                printf("GUEST> %s \r\n",buffer_rx);
                glcd(buffer_rx, 0); //DISPLAY RECEIVED STRING ON THE GLCD
            }

            CHAT_FLAG = 1;
        }

        else if ( (buffer_rx[1] == 'T') && (buffer_rx[2] == 'T') && (buffer_rx[3] == 'P')){
            printf_tiny("\r\nWebServer..\r\n");
            WEB_PAGE_FLAG =1;
        }
        //OPCODE
        //THE PRINTED STRINGS EXPLAIN THE CODE FLOW
        if((CHAT_FLAG == 0) && (WEB_PAGE_FLAG == 0))
        {
            printf_tiny("\r\nReceive done \r\n");
            delay_1ms();
            printf_tiny("Opcodes Received.. \r\n%s\r\n",buffer_rx);
            printf_tiny("Blank check... ");
            blankCheck();
            printf_tiny("Done\r\n");
            printf_tiny("DEBUG: .%s.\r\n",buffer_rx);
            parseTcpOpcodes(0x400,buffer_rx);
            printf_tiny("Programmed Code Memory from 0x400 to %x\r\n",0x400+rcvd_size);
            printf_tiny("\r\nDone.. Connect EA=0 to run the new code\r\n");
        }
        // CHAT
        if(CHAT_FLAG)
        {
            printf_tiny("\r\n.%s.\r\n", buffer_rx);
            if((buffer_rx[0] == 'b') && (buffer_rx[1] == 'y') && (buffer_rx[2] == 'e'))
            {
                CHAT_FLAG = 0;
                write_SPI_W5100(S0_CR,CLOSE);
                while(read_SPI_W5100(S0_SR));
                printf_tiny("\r\n Connection Closed!");
                return 1;
            }
            printf("ME>\t");
            getstring(buffer_tx);
            buffer_tx[strlen(buffer_tx)] = '\0';

            glcd(buffer_tx, 0); //DISPLAY THE TRANSMITTED STRING ONTHE GLCD
            printf("\r\n");
            sendToClient(buffer_tx);
            for(i = 0; i < 100; i++)
            {
                buffer_tx[i] = '\0';
                buffer_rx[i] = ' ';
            }
        }
        if(WEB_PAGE_FLAG)
        {
            printf_tiny("\r\n%s\r\n",buffer_rx);
            sendToClient("HTTP/1.1 200 OK\r\nCache-Control : no-cache, private\r\nContent-Length : 30\r\nDate : Mon, 25 Apr 2016 \r\n\r\n<?php\necho \"Hi there\";\n?>\r\n\r\n");
            delay_custom_ms(1000);
            write_SPI_W5100(S0_CR,CLOSE);
            while(read_SPI_W5100(S0_SR));
            delay_custom_ms(1000);
        }
    }
}

void parseTcpOpcodes(unsigned int baseAddress, unsigned char *buffer){
    int  i=0;
    int n=0;
    unsigned char inputBuffer=0,j;
    n = strlen(buffer);

    printf_tiny("%d bytes\r\n%s\r\n",n/2,buffer);

    for ( i=0; buffer[i]!='\0';){
       // printf_tiny("DEBUG: buffer[%d]=%x\r\n",i,buffer[i]);
        inputBuffer=0;
        for(j=0;j<2;j++){
            unsigned char temp=0;
            if ( buffer[i] > ALPHA_F){
                temp = buffer[i]-0x57;
            }
            else if ( buffer[i] > NUM9){
                temp = buffer[i]-NUM7;
            }
            else{
                temp = buffer[i]-NUM0;
            }
            // convert input characters to integer
            inputBuffer = inputBuffer+(temp*powf(16,2-j-1));
            i++;
            }
    printf_tiny("Sending %x to write at %x\r\n",inputBuffer,baseAddress);
    dataout(baseAddress,inputBuffer);
    if ( i == 2)
        dataout(0x400,0x75); // backdoor method . don't do this. bug
    baseAddress++;
    }
    // we have written opcodes
    // write an infinite loop, so that it will not go further to execute
    // 80FE is the opcode
    //printf_tiny("Appended application opcodes of 0x80, 0xFE bytes in the end\r\n");

    // dataout(0x401,0x89); // backdoor method . don't do this. bug
    printf_tiny("Flashing hex file......");
    for ( i=baseAddress;i<0x8000;i++){
        dataout(baseAddress,0xFF);
        baseAddress++;
    }
    printf_tiny("Done\r\n");
    //dataout(baseAddress,0xFE);
    //baseAddress++;

    /* __asm
     mov a, 0x55
     mov dptr,#0x0200
     movc @dptr,a
     __endasm;
     */

}

 void dataout(unsigned int x, unsigned char y){
    /* __asm
     mov a, 0x55
     mov dptr,#0xCCCC
     movx @dptr,a
     __endasm; */
     // assign the address in x and value in y
    // printf("Writing %x to 0x%x\r\n",y,x);
     ptr =x;
    *ptr =y;
}

void  blankCheck(){
    unsigned int i=0;
    for ( i=0x400; i< 0x500; i++){
        dataout(i,0x0FF);
    }

}

