/* Library - Function definitions */

#include "main_sup.h"

/* Globals */
unsigned char errorFlag = 0;

/* putcharac prints a single character on the terminal */
void putcharac(char c)
{
    SBUF = c;
    while(TI == 0);
    TI = 0;
}

/* putstring prints a string on the terminal */
void putstring(char * str)
{
    while(*str)
    {
        putcharac(*str++);
    }
}

/* getcharac reads a single character from the terminal */
char getcharac()
{
    while(RI == 0);
    RI = 0;
    putcharac(SBUF);
    return SBUF;
}

/* getstring reads a string from the terminal */
unsigned char getstring(char * str)
{
    unsigned char i = 0;
    do
    {
        *str = getcharac();
        str++;
        i++;
        if(i == 4)
        {
            error(3);
            //free(buffer_0);
            //buffer_0 = NULL;
            return 0;
        }
    }while(*(str-1) != '\r');
    *(str-1) = '\0';

    return 1;
}

/* the error() function prints the error messages based on the error code it is called with. It then sets the error flag */
void error(unsigned char c)
{
    switch(c)
    {
    case 1:
        putstring("\r\nNot enough memory to allocate to buffer.Check your Heap");
        errorFlag = 1;
        break;
    case 2:
        putstring("\r\nInvalid Entry.");
        errorFlag = 1;
        break;
    case 3:
        putstring("\r\nEntered number is too large/ Wrong keys pressed!");
        errorFlag = 1;
        break;

        default:
            putstring("\r\nUnforeseen error");
            errorFlag = 1;
    }
}

/* menu_first() displays the first menu-asking for user input */
char menu_first()
{
    volatile unsigned char ch;
    char gen = 0;
    unsigned char pw = 78;
    unsigned char load = 0;
    unsigned char exitflag = 0;
    static unsigned int hso_value = 0xFF;
    CR = 0;

    while(1)
    {
        putstring("\r\n\t\t\t\t\t\t\t************************************");
        putstring("\r\n\t\t\t\t\t\t\t*                                  *");
        putstring("\r\n\t\t\t\t\t\t\t*         Welcome Aboard           *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'P' to play with PWM       *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'H' for High Speed output  *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'T' for Software Timer     *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'U' to see UART arrival    *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'W' for Watchdog           *");
        putstring("\r\n\t\t\t\t\t\t\t*                                  *");
        putstring("\r\n\t\t\t\t\t\t\t************************************\r\n");



        ch = getcharac();

        switch(checkChar(ch))
        {
        /* Store the character in buffer_0 */
        case PWMN:

            EA = 1; // Enable Interrupts
            EC = 1; //Enable PCA interrupts
            /* Using Module 0 of PCA for PWM - Output on P1.3/CEX0 */
            load = (unsigned char)((100 - pw) * 256/100); //Its 56 for 78% duty cycle
            //printf_tiny("\r\n %d", load);
            CCAP0L = load;
            CCAP0H = load;
            CCAPM0 = 0x43; //Setting bit ECOM, PWM, ECCF
            CMOD |= 0x00; //Init CMOD to make PCA use 1/6th Periph Clock Frequency
            while(1)
            {
                switch(pwm_menu())
                {
                case RUN:
                    CR = 1;
                    //print_pwm(pw);// print with 78 % duty cycle
                    break;

                case STOP:
                    CR = 0;
                    //print_pwm(0);
                    break;

                case INCREASE:
                    CR = 0;
                    pw = (unsigned char)(pw * 1.05); // 5% increase
                    pw = (pw <=  95) ? pw : 95; // limit increase till 95 % pulse width
                    load = (unsigned char)((100 - pw) * 256/100); //compute value to be loaded on to the CCAP0L reg
                    CCAP0L = load; //56 ensures 78% duty cycle
                    CCAP0H = load;
                    CR = 1;
                    //print_pwm(pw);
                    break;

                case DECREASE:
                    CR = 0;
                    pw = (unsigned char)(pw * 0.95); // 5% increase
                    pw = (pw >=  5) ? pw : 5; // limit decrease till 5 % pulse width
                    load = (unsigned char)((100 - pw) * 256/100); //compute value to be loaded on to the CCAP0L reg
                    CCAP0L = load; //56 ensures 78% duty cycle
                    CCAP0H = load;
                    CR = 1;
                    //print_pwm(load);
                    break;

                case IDLE:
                    EA = 1; // Enable interrupts
                    //IEN1 |= 0x01;//KBDH = 1;
                    EX1 = 1;
                    IT1 = 1;
                    PCON |= 0x01;
                    CR = 1;
                    break;

                case PDOWN:
                    EA = 1; // Enable interrupts
                    //IEN1 |= 0x01;//KBDH = 1;
                    //EX1 = 1;
                    PCON |= 0x02;
                    CR = 1;
                    break;

                case EXIT:
                    exitflag = 1;
                    break;

                default:
                    putstring("\r\n Please Enter a valid Character");
                }

                if(exitflag)
                {
                    exitflag = 0;
                    break;
                }
            }

            break; //case PWM ends

        case HSO:
            //CMOD = 0x0C; //Init CMOD to make PCA use 1/6th Periph Clock Frequency
            EA = 1; // Enable Interrupts
            EC = 1; //Enable PCA interrupts
            CCAP1L = 0xFF;
            CCAP1H = 0x00;
            CCAPM1 = 0x4D; //- 1 0 0 1 1 0 1
            CMOD |= 0x00; //Init CMOD to make PCA use 1/6th Periph Clock Frequency
            CR = 1;

            while(1)
            {
                switch(hso_menu())
                {
                case HALVE:
                    CR = 0;
                    if((CCAP1H == 0xFF) && (CCAP1H == 0xFF))
                    {
                        putstring("\r\nCannot decrease frequency any further");
                    }
                    /*else if(CCAP1L != 0xFF)
                        CCAP1H |= CCAP1H << 1;
                    else if((CCAP1L == 0xFF) && (CCAP1H == 0x00))
                        CCAP1H = 0x01;
                    else if((CCAP1L == 0xFF) && (CCAP1H != 0x00))
                        CCAP1H |= (CCAP1H << 1);
                    */
                    else
                    {
                        hso_value = (unsigned int)(hso_value * 2);
                        CCAP1H = 0xFF;//(hso_value>>8) & 0XFF;
                        CCAP1L = 0xFF;//hso_value & 0xFF;
                    }
                    CCAPM1 = 0x4D; //- 1 0 0 1 1 0 1
                    CR = 1; // Output on P1.4/CEX1
                    break;

                case DOUBLE:
                    CR = 0;
                    if(CCAP1L == 0x01)
                    {
                        putstring("\r\nCannot increase frequency any further");
                    }
                    /*else if(CCAP1L != 0xFF)
                        CCAP1H |= CCAP1H >> 1;
                    else if((CCAP1L == 0xFF) && (CCAP1H == 0x00))
                        CCAP1L = 0x7F;
                    else if((CCAP1L == 0xFF) && (CCAP1H != 0x00))
                        CCAP1H = (CCAP1H >> 1);
                    */
                    else
                    {
                        hso_value = (unsigned int)(hso_value/2);
                        CCAP1H = (hso_value>>8) & 0XFF;
                        CCAP1L = hso_value & 0xFF;
                    }
                    CCAPM1 = 0x4D; //- 1 0 0 1 1 0 1
                    CR = 1; // Output on P1.4/CEX1
                    break;

                case EXIT:
                    exitflag = 1;
                    break;
                default:
                    putstring("\r\n Enter a valid character");
                    break;
                }
                if(exitflag)
                {
                    exitflag = 0;
                    break;
                }
            }
            break;

        case TIMER:
            EA = 1; // Enable Interrupts
            EC = 1; //Enable PCA interrupts
            CCAP2L = 0xFF;
            CCAP2H = 0xFF;
            CCAPM2 = 0x49; //- 1 0 0 1 1 0 1
            CMOD |= 0x00; //Init CMOD to make PCA use 1/6th Periph Clock Frequency
            CR = 1;

            break;
        case UART:
            EA = 1; // Enable Interrupts
            EC = 1; //Enable PCA interrupts
            CCAPM3 = 0x11; //- 0 0 1 0 0 0 1
            CMOD |= 0x00; //Init CMOD to make PCA use 1/6th Periph Clock Frequency
            CR = 1;
            putcharac('u');

            break;
        case WD:
            EA = 1; // Enable Interrupts
            EC = 1; //Enable PCA interrupts
            CCAP2L = 0xFF;
            CCAP2H = 0xFF;
            CCAPM4 = 0x49; //- 1 0 0 1 1 0 1
            CMOD |= 0x40; //Init CMOD to make PCA use 1/6th Periph Clock Frequency and WD
            CR = 1;

            break;
        default:
            putstring("\r\nPlease enter a valid option");

        }
    }
}

/* Checks character input by the user to determine further course of the program */
char checkChar(char ch)
{
    if((ch == 'P') || (ch == 'p'))
        return 1;
    else if((ch == 'H') || (ch == 'h'))
        return 2;
    else if((ch == 'T') || (ch == 't'))
        return 3;
    else if((ch == 'U') || (ch == 'u'))
        return 4;
    else if((ch == 'R') || (ch == 'r'))
        return 5;
    else if((ch == 'S') || (ch == 's'))
        return 6;
    else if((ch == 'B') || (ch == 'b'))
        return 7;
    else if((ch == 'D') || (ch == 'd'))
        return 8;
    else if((ch == 'I') || (ch == 'i'))
        return 9;
    else if((ch == 'L') || (ch == 'l'))
        return 10;
    else if((ch == 'E') || (ch == 'e'))
        return 11;
    else if(ch == '+')
        return 12;
    else if(ch == '-')
        return 13;
    else if((ch == 'W') || (ch == 'w'))
        return 14;
    else
        return 15;
}

char pwm_menu()
{
    unsigned char ch;

    putstring("\r\n\t\t\t\t\t\t\t*******************************************");
    putstring("\r\n\t\t\t\t\t\t\t*                                         *");
    putstring("\r\n\t\t\t\t\t\t\t* Press 'R' to Run PWM                    *");
    putstring("\r\n\t\t\t\t\t\t\t* Press 'S' to Stop PWM                   *");
    putstring("\r\n\t\t\t\t\t\t\t* Press 'B' to Increase Duty Cycle by 5%  *");
    putstring("\r\n\t\t\t\t\t\t\t* Press 'D' to Decrease Duty Cycle by 5%  *");
    putstring("\r\n\t\t\t\t\t\t\t* Press 'I' to enter Idle mode            *");
    putstring("\r\n\t\t\t\t\t\t\t* Press 'L' to enter Power down mode      *");
    putstring("\r\n\t\t\t\t\t\t\t* Press 'E' to Go back to previous menu   *");
    putstring("\r\n\t\t\t\t\t\t\t*                                         *");
    putstring("\r\n\t\t\t\t\t\t\t*******************************************\r\n");

    ch = getcharac();

    return checkChar(ch);
}

char hso_menu()
{
    unsigned char ch;

    putstring("\r\n\t\t\t\t\t\t\t*******************************************");
    putstring("\r\n\t\t\t\t\t\t\t*                                         *");
    putstring("\r\n\t\t\t\t\t\t\t* Enter '+' to double the o/p frequency   *");
    //putstring("\r\n\t\t\t\t\t\t\t* Enter '-' to halve the o/p frequency    *");
    putstring("\r\n\t\t\t\t\t\t\t* Press 'E' to Go back to previous menu   *");
    putstring("\r\n\t\t\t\t\t\t\t*                                         *");
    putstring("\r\n\t\t\t\t\t\t\t*******************************************\r\n");

    ch = getcharac();

    return checkChar(ch);
}

void print_pwm(unsigned char pw)
{
    unsigned char i = 0; // loop index

    putstring("\r\n\t\t\t\t\t\t\t |");
    pw = (unsigned char)pw/10;
    for(i=0; i < (100-pw) ; i++ )
    {
        putcharac('_');
    }
    for(; i <= 100; i++)
    {
        putcharac('-');
    }
    putstring("|                                       ");
}

/* ISR for PCA interrupts*/
void isr_six(void) __interrupt (6)
{
    if(CCF0) /* Check if interrupt is from Module 0 - PWM in this case */
    {
        CCF0 = 0;
    }
    if(CCF1) /* Check if interrupt is from Module 1 - HSO in this case */
    {
        CCF1 = 0;
        CL = 0x00;
        CH = 0x00;

    }
    if(CCF2) /* Check if interrupt is from Module 2 - SW Timer in this case */
    {
        P1_0 ^= 1;
        CCF2 = 0;
    }
    if(CCF3) /* Check if interrupt is from Module 3 - Capture start of Rx in this case */
    {
        CCF3 = 0;
        putstring("\r\n Reception started on Serial port");
    }
    return;
}

/* ISR for Ext interrupt 1*/
void isr_two(void) __interrupt (2)
{
    putstring("\r\n Idle Mode Exited");
    menu_first();
    return;
}
