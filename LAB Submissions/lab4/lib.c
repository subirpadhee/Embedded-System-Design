/*  LAB-4 library source file
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"

/* Function definitions */

/* the error() function prints the error messages based on the error code it is called with. It then sets the error flag */
unsigned char error(unsigned char c)
{
    switch(c)
    {
    case 1:
        putstring("\r\n START EEPROM Write failure!");
        errorFlag = 1;
        break;
    case 2:
        putstring("\r\n CTRL EEPROM Write failure!");
        errorFlag = 1;
        break;
    case 3:
        putstring("\r\n ADDR EEPROM Write failure!");
        errorFlag = 1;
        break;
    case 4:
        putstring("\r\n DATA ADDR EEPROM Write failure!");
        errorFlag = 1;
        break;
    case 5:
        putstring("\r\n STOP ADDR EEPROM Write failure!");
        errorFlag = 1;
        break;
    case 6:
        putstring("\r\n ADDR EEPROM Write failure!");
        errorFlag = 1;
        break;
    case 7:
        putstring("\r\n Not a valid EEPROM address / Data that!");
        errorFlag = 1;
        break;
    case 8:
        putstring("\r\n Not a valid Data that!");
        errorFlag = 1;
        break;
    case 9:
        putstring("\r\n Not a valid Row/Column that!");
        errorFlag = 1;
        break;
    case 10:
        putstring("\r\n Start address bigger than End address!");
        errorFlag = 1;
        break;
    case 11:
        putstring("\r\n You must enter exactly no. of 1s/0s per row!");
        errorFlag = 1;
        break;
    case 12:
        putstring("\r\n Page could not be written!");
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
    while(t)
    {
        delay_1ms();
        t--;
    }
}

/*  Timer for clock */
void timer_0_init()
{
    TMOD |= 0x01;
    TH0 = 0x98;
    TL0 = 0x8A;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
    reset_flag = 0;
    return;
}

/*  Timer for timing function execution */
void timer_2_init()
{
    TH2 = 0x00;
    TL2 = 0x00;
    RCAP2H = 0x00;
    RCAP2L = 0x00;
    EA = 1;
    //T2EX = 1;
    EXF2 = 1;
    //TR2 = 1;
    return;
}

/*  Timer for Watchdog Servicing */
void pcatimer_init()
{
    EA = 1; // Enable Interrupts
    EC = 1; //Enable PCA interrupts
    CCAP2L = 0xFF;
    CCAP2H = 0x00;
    CCAPM2 = 0x49; //- 1 0 0 1 1 0 1
    CMOD |= 0x00; //Init CMOD to make PCA use 1/6th Periph Clock Frequency
    CR = 1;
}

/*  1 ms delay */
void delay_1ms()
{
    unsigned char i, j;
    for(i = 0; i <30; i++)
        for(j= 0; j < 12; j++);
}

/*  main menu function. Provides an interactive UI */
void menu()
{
    unsigned char ch;
    while(1)
    {
        putstring("\r\n\r\n\t\t\t\t\t\t\t*****************************************");
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*         Welcome Aboard                *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'W' TO WRITE TO THE EEPROM      *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'R' TO READ FROM THE EEPROM     *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'P' TO PRINT DATA ON LCD        *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'D' TO CLEAR/DELETE THE LCD     *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'E' FOR A HEXDUMP OF THE EEPROM *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'M' FOR A HEXDUMP OF THE DDRAM  *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'C' FOR A HEXDUMP OF THE CGRAM  *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'Q' FOR CUSTOM CHARACTER MENU   *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'T' FOR CLOCK FUNCTIONALITY     *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'L' FOR LCD FUNCTIONALITY       *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'X' FOR I/O EXPANDER            *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'K' TO RESET(WD)                *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'G' TO GOTO THE CHALLENGES MENU *");
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");

        ch = getcharac();

        switch(checkChar(ch))
        {
        /* */
        case WRITE:
            menu_write();
            break;

        case READ:
            menu_read();
            break;

        case DISPLAY:
            menu_display();
            break;

        case CLEAR:
            clear_disp();
            break;

        case EEPROM:
            hexdumpEEPROM();
            //hexdumpEEPROM_new();
            break;

        case DDRAM:
            DDRAMdump();
            break;

        case CGRAM:
            CGRAMdump();
            break;

        case CUSTOMCHAR:
            menu_custchar();
            break;

        case TIME_DISP:
            menu_time();
            break;

        case LCD:
            lcd_play();
            break;

        case EXPANDER:
            ioexpander();
            break;

        case KILL:
            CR = 0;
            break;

        case CHALLENGE:
            menu_challenge();
            break;

        default:
            putstring("\r\nPlease enter a valid option");
        }
    }
}

/* Checks character input by the user to determine further course of the program */
unsigned char checkChar(char ch)
{
    if((ch == 'W') || (ch == 'w'))
        return 1;
    else if((ch == 'R') || (ch == 'r'))
        return 2;
    else if((ch == 'P') || (ch == 'p'))
        return 3;
    else if((ch == 'D') || (ch == 'd'))
        return 4;
    else if((ch == 'E') || (ch == 'e'))
        return 5;
    else if((ch == 'M') || (ch == 'm'))
        return 6;
    else if((ch == 'C') || (ch == 'c'))
        return 7;
    else if((ch == 'Q') || (ch == 'q'))
        return 8;
    else if((ch == 'T') || (ch == 't'))
        return 9;
    else if((ch == 'L') || (ch == 'l'))
        return 10;
    else if((ch == 'Z') || (ch == 'z'))
        return 11;
    else if((ch == 'S') || (ch == 's'))
        return 12;
    else if((ch == 'X') || (ch == 'x'))
        return 14;
    else if((ch == 'K') || (ch == 'k'))
        return 15;
    else if((ch == 'G') || (ch == 'g'))
        return 16;
    else if((ch == 'B') || (ch == 'b'))
        return 17;
    else if((ch == 'F') || (ch == 'f'))
        return 18;
    else
        return 20;
}

/* checkInput() checks the user input to determine if its a valid input and also evaluate its value */
unsigned int checkInput(unsigned char *str)
{
    unsigned int ret = 0; //return value
    unsigned char i=0; // index variable
    unsigned char length;

    length = strlen(str);
    //printf("\r\nlength:%d", length);
    if(0 == length)
    {
        error(7);
        return 2048;
    }
    for(i = 0; i< length; i++)
    {
        if((48 <= str[i]) && (str[i] <= 57))
            ret = (16 * ret) + (str[i] & 0x0F);
        else if((65 <= str[i]) && (str[i] <= 70))
            ret = (16 * ret) + ((str[i] - 'A' + 10) & 0x0F);
        else if((97 <= str[i]) && (str[i] <= 102))
            ret = (16 * ret) + ((str[i] - 'a' + 10) & 0x0F);
        else
        {
            return 2048;//Not a number
        }
    }
    return ret;
}

/*  menu for the challenge questions */
void menu_challenge()
{
    unsigned char ch = '\0';
    unsigned char i = 0;
    unsigned char data_b[16] = {'\0'};
    unsigned int data_int = 0;
    unsigned char block = 0;
    unsigned char addr = 0;
    unsigned char eeAddrB[5];
    unsigned char data_fill[4] = {'\0'};
    unsigned int rcvdb = 0;
    unsigned char length = 0;

    while(1)
    {
        putstring("\r\n\r\n\t\t\t\t\t\t\t*****************************************");
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*         Welcome Aboard                *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'S' TO SEARCH A STRING          *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'R' TO READ FROM THE EEPROM     *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'P' FOR PAGE WRITE              *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'B' FOR BYTE WRITE              *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'F' FOR BLOCK FILL              *");
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");

        ch = getcharac();

        switch(checkChar(ch))
        {
        /* */
        case SEARCH:
            search2();
            TR2 = 0;
            compute_time();

            break;
        case PAGE:
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
            putstring("\r\n\t\t\t\t\t\t\t* First Data to Fill:                *");
            putstring("\r\n\t\t\t\t\t\t\t* 0x");
            getstring_blockfill(data_fill);
            if((data_int = checkInput(data_fill)) > 2047) //2048 is returned on error
            {
                error(8);
                putstring("\r\n\t\t\t\t\t\t\t*                                       *");
                putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");
                return;
            }
            for(i = 0; i < 16; i++)
            {

                data_b[i] = (data_int + i) & 0xFF;
            }
            putstring("\r\n\t\t\t\t\t\t\t*                                       *");
            putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");

            addr = rcvdb & 0xFF;
            block = (rcvdb >> 8) & 0x0F;

            block <<= 1;
            block |= 0xA0;//device = 0xA0 for EEPROM
            block &= 0xFE;

            //length = addr & 0x0F;
            page_write(block, addr, 0, data_b);//length passed is 0 since we want to write all 16 bytes
            break;

        case BYTE:
            byte_write();
            break;

        case FILL:
            blockfill();
            break;

        default:
            lcdgotoxy(0,0);
            lcdputstr("WHEN IN INDIA, DO INDIANS.WHEN IN AMERICA, DO IT LIKE INDIANS DO");
            return;
        }
    }

}
