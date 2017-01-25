/*  LAB-4 library source file -Challenge1
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"

/* Function definitions */

/*  computes time elapsed between start and stop of timer 2 */
void compute_time()
{
    long long time_computed = 0;
    long UB = 0;

    UB = 0x00FF & TH2;
    //printf("\r\n UB:%l ov_cnt:%d", UB, ov_cnt);

    time_computed = (((UB << 8) + TL2) * 1085);
    time_computed = (time_computed/1000) + ((71105 * ov_cnt));
    printf("\r\n\r\n\t\t\t\t\t\t\tTime taken for the operation is %ld usec", time_computed);
    printf("\r\n\t\t\t\t\t\t\t   [ TH2:%02X TL2:%02X Overflow count: %d ]", TH2, TL2, ov_cnt);
}

/*  Search a string from the text written on the LCD */
unsigned char search2()
{
    volatile unsigned char i = 0, j = 0, no_match = 0, len = 0 ;//loop vars, flags, length var
    unsigned char str[64] = {'\0'};//stores the string to be searched-max 64 chars
    unsigned char ch = '\0';//stores each char extracted from the DDRAM
    unsigned char pos = '\0';//stores the position of the extracted character on the DDRAM
    volatile unsigned char posBEG = '\0';//stores the position of the first matched character on the DDRAM
    volatile unsigned char posFIRST = 0x5F;//used to prevent repetition searches
    unsigned char revert = 0;//used to restore the position of the curson in case entire string does not match
    volatile unsigned char first_time = 1; //flag

    putstring("\r\n\r\n\t\t\t\t\t\t\t*****************************************");
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t*            Word   Search              *");
    putstring("\r\n\t\t\t\t\t\t\t*     Enter THE STRING TO SEARCH        *");
    putstring("\r\n\t\t\t\t\t\t\t*                                       *");
    putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");

    getstring_search(str);
    len = strlen(str);
    lcdgotoxy(0,0);
    TR2 = 1;//to compute time
    if(len == 1)
    {
        for(i = 0; i < 64 ; i++)
        {
            ch = ddram_pos(&posBEG);
            if(posBEG == posFIRST)
            {
                return 1;
            }

            if(ch == str[0])
            {
                if(first_time)
                {
                    posFIRST = posBEG;
                    first_time = 0;
                }
                printf("\r\n\t\t\t\t\t\t\t   Found a match for %s  at 0x%02X", str, posBEG);
            }
        }
    }
    for(i = 0; i < 128 ; i++)
    {
        ch = ddram_pos(&posBEG);
        if(posBEG == posFIRST)
        {
            return 1;
        }

        if(ch == str[j])
        {
            revert = i+1;
            if((i >=64) && (j == 0))
                break;
            j++;
            for(; ((j < len) && (no_match == 0));i++)
            {
                ch = ddram_pos(&pos);
                if(ch != str[j++])
                {
                    no_match = 1;
                    j = 0;//for next match search
                    i = i-j;
                }
            }
            if(no_match == 0)
            {
                printf("\r\n\t\t\t\t\t\t\t   Found a match for %s  at 0x%02X", str, posBEG);
                no_match = 1;
                j = 0;//for next match search
            }
            if(no_match)
            {
                i--;//to negate the one extra i++
                lcdgotoxy(revert/16 ,revert%16);
                NOP;
            }
        }
        else
        {
            //get next char from lcd
        }
        no_match = 0;

    }
    putstring("\r\n\t\t\t\t\t\t\t\t Search over!");
}

/* extracts charaters and their position from the LCD(DDRAM) in the order they are displayed */
unsigned char ddram_pos(unsigned char *pos)
{
    unsigned char ch = '\0';
    RS = 0;
    RWbar = 1;
    NOP;
    RD_DATA;
    *pos = (*(xdata unsigned char *)RD_ADDR) & 0x7F; //Read the DDRAM address of the current cursor position

    /* The cursor is incremented linearly and not in the order characters are displayed on the LCD.
    The following lines make sure the cursor position is set in the order of display */

    if(*pos == 0x0F)
    {
        lcdgotoxy(1,0);
    }
    else if(*pos == 0x4F)
    {
        lcdgotoxy(2,0);
    }
    else if(*pos == 0x1F)
    {
        lcdgotoxy(3,0);
    }
    else if(*pos == 0x5F)
    {
        lcdgotoxy(0,0);
    }
    RS = 1;
    RWbar = 1;
    NOP;
    ch = *(xdata unsigned char *)RD_ADDR; //Read the character pointed to by the current cursor position on the LCD
    NOP;
    return ch;
}
