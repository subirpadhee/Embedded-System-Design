/* Library - Function definitions */

#include "main.h"

/* Globals */
unsigned char errorFlag = 0;
unsigned char size_num = 0; /* Stores the buffer size as a number */
char *buffer_0; /* points to the starting address of buffer 0 */
char *buffer_1; /* points to the starting address of buffer 1 */
char *buffer_2; /* points to the starting address of buffer 2 */
char *buffer_3; /* points to the starting address of buffer 3 */
char *buffer_4; /* points to the starting address of buffer 4 */

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
            free(buffer_0);
            buffer_0 = NULL;
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

/* menu_buffer() displays the first menu-asking for bufer size.It handles the creation of buffers whenever called */
char menu_buffer()
{
    unsigned char buffer_size[5];
    char ch = 0;

    errorFlag = 0;
    buffer_0 = (char *)malloc(1400);
    if(NULL == buffer_0)
    {
        error(1);//Memory allocation issue
        free(buffer_0);
        return errorFlag;
    }
    putstring("\r\nPlease enter a buffer size ( Multiple of 4) in the range [4:200] \r\n");
    if(getstring(buffer_size))
    {
        size_num = checkInput(buffer_size, 'n');
        if (0 == size_num)
        {
            error(2);//Not a number
            free(buffer_0);
            buffer_0 = NULL;
            return errorFlag;
        }
        else if(size_num > 200)
        {
            error(3);
            free(buffer_0);
            buffer_0 = NULL;
            return errorFlag;
        }
        else//its a valid number
        {
            buffer_1 = (char *)malloc(size_num);
            if(NULL == buffer_1)
            {
                error(1);//Memory allocation issue
                free(buffer_0);
                free(buffer_1);
                buffer_0 = NULL;
                return errorFlag;
            }
            /* Allocate buffer_2 */
            buffer_2 = (char *)malloc(size_num);
            if(NULL == buffer_2)
            {
                error(1);//Memory allocation issue
                free(buffer_0);
                free(buffer_1);
                free(buffer_2);
                buffer_0 = NULL;
                buffer_1 = NULL;
                return errorFlag;
            }
            /* Allocate buffer_3 */
            buffer_3 = (char *)malloc(size_num);
            if(NULL == buffer_3)
            {
                error(1);//Memory allocation issue
                free(buffer_0);
                free(buffer_1);
                free(buffer_2);
                free(buffer_3);
                buffer_0 = NULL;
                buffer_1 = NULL;
                buffer_2 = NULL;
                return errorFlag;
            }
            /* Allocate buffer_4 */
            buffer_4 = (char *)malloc(3 * size_num);
            if(NULL == buffer_4)
            {
                error(1);//Memory allocation issue
                free(buffer_0);
                free(buffer_1);
                free(buffer_2);
                free(buffer_3);
                free(buffer_4);
                buffer_0 = NULL;
                buffer_1 = NULL;
                buffer_2 = NULL;
                buffer_3 = NULL;
                return errorFlag;
            }
            else
            {
                putstring("\r\nAll Buffers allocated successfully");
            }
        }
    }
    else
    {
        error(2);// Large number
    }
    return errorFlag;
}

/* checkInput() checks the user input to determine if its a valid input and also evaluate its value */
unsigned char checkInput(unsigned char *buffer_size, unsigned char ch)
{
    unsigned char ret = 0; //return value
    unsigned char i=0; // index variable
    unsigned char length;
    if(ch == 'c')//command/storage input check
    {
        putstring("\r\nch is c ");//Debug code
        return 1;
    }
    else if(ch == 'n')// buffer size input
    {
        length = strlen(buffer_size);
        for(i = 0; i< length; i++)
        {

            if((48 <= buffer_size[i]) && (buffer_size[i] <= 57))
                ret = (10 * ret) + (buffer_size[i] & 0x0F);
            else
            {
                return 0;//Not a number
            }
        }
        if((ret % 4) != 0)
        {
            return 0;//Not a multiple of 4
        }
        return ret;
    }
}

/* Displays the menu offering the user choice. It handles the various user commands by making use of other routines */
void menu_rest()
{
    unsigned char ch;
    unsigned int i = 0; // loop index to store all in buffer 0
    unsigned int j = 0; // loop index to loop in (and to store letters in buffers 1, 2, 3 from) buffer 0
    unsigned char k = 0; // loop index to loop in buffer 1, 2, 3
    unsigned char buf_sel = 1; // selects buffer to write into
    unsigned char x = 0, z = 0; // To store no of chars entered
    unsigned int u = 0, v = 0; // For printing buf_0 contents
    unsigned char b1 = 0, b2 = 0, b3 = 0; // No. of chars in buffers 1, 2 , 3 respectively
    unsigned char printonce = 1; //flag to make sure error message is printed only once
    unsigned int b4 = 0;//loop index for buffer 4 population
    unsigned int b4i = 0; //loop index for sorting
    unsigned char temp = 0; //temporary variable for storing buffer value
    unsigned char maxi = 0; //variable to hold maxi diff between entries in buffer 4

    putstring("\r\n\t\t\t\t\t\t\t************************************");
    putstring("\r\n\t\t\t\t\t\t\t*                                  *");
    putstring("\r\n\t\t\t\t\t\t\t* Enter Storage Characters         *");
    putstring("\r\n\t\t\t\t\t\t\t* Enter '+' to initiate a transfer *");
    putstring("\r\n\t\t\t\t\t\t\t* Enter '?' to generate a report   *");
    putstring("\r\n\t\t\t\t\t\t\t* Enter '=' to display contents    *");
    putstring("\r\n\t\t\t\t\t\t\t* Enter '@' to empty memory        *");
    putstring("\r\n\t\t\t\t\t\t\t* Enter '-' for buffer 4 op        *");
    putstring("\r\n\t\t\t\t\t\t\t*                                  *");
    putstring("\r\n\t\t\t\t\t\t\t************************************\r\n");

    while(1)
    {
        ch = getcharac();
        z++;
        switch(checkChar(ch, ALL))
        {
        /* Store the character in buffer_0 */
        case STORAGE:
            DEBUGPORT(0xBBBB,0x55);/* Debugging */
            x++;
            if(i < 1400)
            {
                buffer_0[i++] = ch;
                putstring("\tStored! \r\n");
            }

            break;
        /* Store the letters in buffer 0 in buffers 1,2,3 */
        case PLUS:
            for(;j < i; j++)
            {
                if(checkChar(buffer_0[j], LETTERS))
                {
                    switch(buf_sel)
                    {
                    case 1:
                        buffer_1[k] = buffer_0[j];
                        k++;
                        if(k == size_num)
                        {
                            k = 0;
                            buf_sel++;
                        }
                        break;

                    case 2:
                        buffer_2[k] = buffer_0[j];
                        k++;
                        if(k == size_num)
                        {
                            k = 0;
                            buf_sel++;
                        }
                        break;

                    case 3:
                        buffer_3[k] = buffer_0[j];
                        k++;
                        if(k == size_num)
                        {
                            k = 0;
                            buf_sel++;
                        }
                        break;
                    default:
                        if(printonce)
                        {
                            printonce = 0;
                            putstring("\r\nAll three Buffers full! Can't execute '+' unless data is emptied");
                        }

                    }
                }
            }

            switch(buf_sel)
            {
            case 1:
                b1 = k;
                b2 = 0;
                b3 = 0;
                break;
            case 2:
                b1 = size_num;
                b2 = k;
                b3 = 0;
                break;
            case 3:
                b1 = size_num;
                b2 = size_num;
                b3 = k;
                break;
            default:
                b1 = size_num;
                b2 = size_num;
                b3 = size_num;
            }
            break;

        /* Generates a report, displays the contents of the buffers. Empties the buffers */
        case REPORT:
            DEBUGPORT(0xCCCC,0x77); /* For debugging*/

            printf_tiny("\r\n\r\n\t\t\t\t***********************************************************************************************\r\n");
            putstring("\t\t\t\t*                                                                                             *");
            printf("\r\n\t\t\t\t* Buffer Number\tStart Address\tEnd Address\tAllocated Size\tStored data\tEmpty Space   *  \t");
            printf_tiny("\r\n\t\t\t\t* ------------------------------------------------------------------------------------------- *");
            printf("\r\n\t\t\t\t* buffer_0\t%p\t%p\t%d\t\t%d\t\t%d\t      *", buffer_0, buffer_0+1400, 1400, i,1400-i);
            printf("\r\n\t\t\t\t* buffer_1\t%p\t%p\t%d\t\t%d\t\t%d\t      *", buffer_1, buffer_1+size_num, size_num, b1,size_num-b1);
            printf("\r\n\t\t\t\t* buffer_2\t%p\t%p\t%d\t\t%d\t\t%d\t      *", buffer_2, buffer_2+size_num, size_num, b2,size_num-b2);
            printf("\r\n\t\t\t\t* buffer_3\t%p\t%p\t%d\t\t%d\t\t%d\t      *", buffer_3, buffer_3+size_num, size_num, b3,size_num-b3);
            putstring("\r\n\t\t\t\t*                                                                                             *");
            printf_tiny("\r\n\r\n\t\t\t\t***********************************************************************************************");
            printf_tiny("\r\n\t\t\t\t***********************************************************************************************");
            printf_tiny("\r\n\r\n\t\t\t\t* Number of characters received since last '?':                                               *");
            printf_tiny("\r\n\t\t\t\t* ------------------------------------------------------------------------------------------- *");
            printf_tiny("\r\n\t\t\t\t* Storage: %d                                                                                 *",x);
            printf_tiny("\r\n\t\t\t\t* Commands: %d                                                                                 *",z-x);
            printf_tiny("\r\n\t\t\t\t* Total: %d                                                                                   *",z);
            //printf_tiny("\r\n\t\t\t%d\t\t%d\t%d", x, z-x, z);//get the values
            printf_tiny("\r\n\r\n\t\t\t\t***********************************************************************************************");

            /*  Print the contents and Empty the buffer 0 */
            printf_tiny("\r\n\t\t\t\t  *******************************************************************************************\r\n");
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_0 contents: ");
            printf_tiny("\r\n\t\t\t\t  -------------------------------------------------------------------------------------------\r\n");
            putstring("\t\t\t\t\t\t");
            for(v = 0, u = 0; v < i; v++)
            {
                putcharac(buffer_0[v]);
                putcharac(' ');
                u++;
                if(u%32 == 0)
                {
                    putcharac('\r');
                    putcharac('\n');
                    putstring("\t\t\t\t\t\t");
                }
            }
            putcharac('\r');
            putcharac('\n');
            //free(buffer_0); // empty the buffer
            for(;i>0; i--)
            {
                buffer_0[i] = '\0';
            }
            buffer_0[0] = '\0';
            i = 0;
            j = 0;
            x = 0;
            z = 0; // total characters

            buf_sel = 1; //Re-initialise flag so that Buffer 1,2,3 can get re-populated
            k = 0; //Re-initialize loop index for writing onto buffer 1,2,3

            /*  Buffer 1*/
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_1 contents:");
            printf_tiny("\r\n\t\t\t\t  -------------------------------------------------------------------------------------------\r\n");
            printBuffer(b1, 1);
            //free(buffer_1);
            b1 = 0;

            /*  Buffer 2*/
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_2 contents:");
            printf_tiny("\r\n\t\t\t\t  -------------------------------------------------------------------------------------------\r\n");
            printBuffer(b2, 2);
            //free(buffer_2);
            b2 = 0;

            /*  Buffer 3*/
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_3 contents:");
            printf_tiny("\r\n\t\t\t\t  -------------------------------------------------------------------------------------------\r\n");
            printBuffer(b3, 3);
            //free(buffer_3);
            b3 = 0;
            //printf_tiny("\r\n\r\n\r\n\t\t\t\t*******************************************************************************************\r\n");

            /*  Buffer 4*/
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_4 contents:");
            printf_tiny("\r\n\t\t\t\t  -------------------------------------------------------------------------------------------\r\n");
            //printBuffer(b4, 4);
            //free(buffer_3);
            //b4 = 0;
            printf_tiny("\r\n\r\n\r\n\t\t\t\t  *******************************************************************************************\r\n");


            break;

        /* Displays the contents of the buffers without deleting their contents */
        case DISPLAY:
            //printf("\r\n b1: %d, b2: %d, b3 %d", b1, b2, b3);//Debug Code
            /*  Print the contents of buffer 0 */
            printf_tiny("\r\n\r\n\t\t\t\t*******************************************************************************************\r\n");
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_0 contents:");
            printf_tiny("\r\n\t\t\t\t-------------------------------------------------------------------------------------------\r\n");
            putstring("\t\t\t\t\t");

            for(v = 0, u = 0; v < i; v++)
            {
                if(u%16 == 0)
                {
                    putcharac('\r');
                    putcharac('\n');
                    putstring("\t\t\t\t\t");
                    printf("%p ",&buffer_0[u]);
                }
                printf_tiny("%x ",buffer_0[v]);
                putcharac(' ');
                u++;
            }
            putcharac('\r');
            putcharac('\n');

            /*  Buffer 1*/
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_1 contents:");
            printf_tiny("\r\n\t\t\t\t-------------------------------------------------------------------------------------------\r\n");
            justprintBuffer(b1, 1);
            //free(buffer_1);

            /*  Buffer 2*/
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_2 contents:");
            printf_tiny("\r\n\t\t\t\t-------------------------------------------------------------------------------------------\r\n");
            justprintBuffer(b2, 2);

            /*  Buffer 3*/
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_3 contents:");
            printf_tiny("\r\n\t\t\t\t-------------------------------------------------------------------------------------------\r\n");
            justprintBuffer(b3, 3);
            //printf_tiny("\r\n\r\n\r\n\t\t\t\t*******************************************************************************************\r\n");

            /*  Buffer 4*/
            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_4 contents:");
            printf_tiny("\r\n\t\t\t\t-------------------------------------------------------------------------------------------\r\n");
            //justprintBuffer(b4, 4);
            putstring("\t\t\t\t\t");
            //printf("%p ",&buffer_4[0]);
            printf_tiny("\r\n\r\n\r\n\t\t\t\t*******************************************************************************************\r\n");


            break;

        /* free the buffers and start the program afresh */
        case EMPTY:
            free(buffer_0);
            free(buffer_1);
            free(buffer_2);
            free(buffer_3);
            free(buffer_4);
            buffer_0 = NULL;
            buffer_1 = NULL;
            buffer_2 = NULL;
            buffer_3 = NULL;
            i = 0;
            j = 0;
            k = 0;
            buf_sel = 1;
            x = 0;
            z = 0;
            printonce = 1;
            putstring("\r\n Ready to Restart!");

            while(menu_buffer());

            putstring("\r\n\t\t\t\t\t\t\t************************************");
            putstring("\r\n\t\t\t\t\t\t\t*                                  *");
            putstring("\r\n\t\t\t\t\t\t\t* Enter Storage Characters         *");
            putstring("\r\n\t\t\t\t\t\t\t* Enter '+' to initiate a transfer *");
            putstring("\r\n\t\t\t\t\t\t\t* Enter '?' to generate a report   *");
            putstring("\r\n\t\t\t\t\t\t\t* Enter '=' to display contents    *");
            putstring("\r\n\t\t\t\t\t\t\t* Enter '@' to empty memory        *");
            putstring("\r\n\t\t\t\t\t\t\t*                                  *");
            putstring("\r\n\t\t\t\t\t\t\t************************************\r\n");

            break;
        case BUF4:
            //printf_tiny("\r\nsize_num- %d, b1-%d, b2-%d, b3-%d", size_num, b1, b2, b3);
            /* Empty buffer 4 as required in the question */
            for(b4 = 0; b4 <(3*size_num); b4++)
            {
                buffer_4[b4] = '\0';
            }
            /* populate buffer_4 with contents from first 3 buffers */
            for(b4=0; b4 < (b1+b2+b3); b4++)
            {
                if(b4 < b1)
                    buffer_4[b4] = buffer_1[b4];
                else if(b4 < (b1+ b2))
                    buffer_4[b4] = buffer_2[b4-b1];
                else if(b4 < (b1+b2+b3))
                    buffer_4[b4] = buffer_3[b4-b2-b1];
            }

            /* Convert all letters to capitals */
            for(b4=0; b4 < (b1+b2+b3); b4++)
            {
                if((buffer_4[b4] >= 'a') && (buffer_4[b4] <='z'))
                    buffer_4[b4] -= 32;
            }

            #ifdef FAST
            CKCON0 = 0x01; /* enable X2 mode */
            #endif
            P1_2 ^= 1; /* To measure time taken using oscilloscope */
            /* Bubble Sort */
            for(b4=0; b4 < (b1+b2+b3); b4++)
            {
                for(b4i = b4+1; b4i < (b1+b2+b3); b4i++)
                {
                    if(buffer_4[b4] > buffer_4[b4i])
                       {
                           temp = buffer_4[b4i];
                           buffer_4[b4i] = buffer_4[b4];
                           buffer_4[b4] = temp;
                       }
                }
            }

            P1_2 ^= 1; /* To measure time taken using oscilloscope */
            #ifdef FAST
            CKCON0 = 0x00; /* Disable X2 mode */
            #endif
            /* Find maxi difference between successive entries */
            maxi = 0;
            for(b4=1; b4 < (b1+b2+b3); b4++)
            {
                if((buffer_4[b4] - buffer_4[b4 - 1]) > maxi)
                    maxi = buffer_4[b4] - buffer_4[b4 - 1];
            }

            putstring("\r\n\t\t\t\t\t\t\t\t\tBuffer_4 contents:");
            printf_tiny("\r\n\t\t\t\t-------------------------------------------------------------------------------------------\r\n");
            printBuffer((b1+b2+b3), 4);
            //putstring("\r\nThe maxiimum difference between successive characters is: ");
            //putcharac(maxi);
            printf_tiny("\r\nThe maxiimum difference between successive characters is:%d", maxi);
            break;

            default:
                putstring("\r\nThat means nothing to me Sir! \r\n");
        }
    }
}

/* Print routine used for report generation and amptying the contents of the buffers */
void printBuffer(unsigned int sz, char buffer)
{
    unsigned int u = 0, v = 0; // For printing buf contents
    putstring("\t\t\t\t\t\t");
    for(u = 0, v = 0; v < sz; v++)
    {
        switch(buffer)
        {
        case 1:
            putcharac(buffer_1[v]);
            buffer_1[v] = '\0';
            break;
        case 2:
            putcharac(buffer_2[v]);
            buffer_2[v] = '\0';
            break;
        case 3:
            putcharac(buffer_3[v]);
            buffer_3[v] = '\0';
            break;
        case 4:
            putcharac(buffer_4[v]);
            //buffer_3[v] = '\0';
            break;
        default:
            return;
        }
        putcharac(' ');
        u++;
        if(u%32 == 0)
        {
            putcharac('\r');
            putcharac('\n');
            putstring("\t\t\t\t\t\t");
        }
    }
    putcharac('\r');
    putcharac('\n');
}

/* Print routine used for displaying the contents of the buffers, without deleting them */
void justprintBuffer(unsigned char sz, char buffer)
{
    unsigned char u = 0, v = 0; // For printing buf contents
    putstring("\t\t\t\t\t");

    for(u = 0, v = 0; v < sz; v++)
    {
        if(u%16 == 0)
        {
            putcharac('\r');
            putcharac('\n');
            putstring("\t\t\t\t\t");
            switch(buffer)
            {
            case 1:
                printf("%p ",&buffer_1[u]);
                break;
            case 2:
                printf("%p ",&buffer_2[u]);
                break;
            case 3:
                printf("%p ",&buffer_3[u]);
                break;
            }
        }
        switch(buffer)
        {
        case 1:
            printf_tiny("%x ",buffer_1[v]);
            break;
        case 2:
            printf_tiny("%x ",buffer_2[v]);
            break;
        case 3:
            printf_tiny("%x ",buffer_3[v]);
            break;
        default:
            return;
        }
        putcharac(' ');

        u++;
    }
}

/* Checks if a character input by the user is a Storage character or not. Also checks if a Storage character is a letter or a number */
char checkChar(char ch, char type)
{
    if(type == ALL)
    {
        if((ch >= 48 && ch <= 57) || (ch >= 65 && ch <=90) || (ch >= 97 && ch <=122) )
            return 10;
        else if(ch == '+')
            return 20;
        else if(ch == '?')
            return 30;
        else if(ch == '=')
            return 40;
        else if(ch == '@')
            return 50;
        else if(ch == '-')
            return 70;
            else
                return 60;
    }
    else if(type == LETTERS)
    {
        if((ch >= 65 && ch <=90) || (ch >= 97 && ch <=122) )
            return 1;
        else
            return 0;
    }
}

/* Used for debugging */
void dataout(unsigned int address, unsigned char value )
{
    xdata unsigned char *ptr = (xdata unsigned char *)address;
    *ptr = value;
}

void putchar(char c)
{
    SBUF = c;
    while(TI == 0);
    TI = 0;
}

