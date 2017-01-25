/*  LAB-4 library source file - CLOCK
    SUBIR KUMAR PADHEE
    ECEN5613
*/

#include "main.h"

/* Globals */

/* Function definitions */

/* UI for Clock, Alarm */
void menu_time()
{
    unsigned char ch = 0; //HOLDS USER CHOICE FROM THE MENU
    unsigned char time_str[5] = {'\0'}; //STORES USER INPUT FOR ALARM TIME SETTING

    while(1)
    {
        putstring("\r\n\r\n\t\t\t\t\t\t\t*****************************************");
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'P' TO PAUSE THE CLOCK          *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'S' TO RE-START THE CLOCK       *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'R' TO RESET THE CLOCK          *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'T' TO SET AN ALARM TIME        *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'D' TO DISABLE AN ALARM         *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'E' TO ENABLE AN ALARM          *");
        putstring("\r\n\t\t\t\t\t\t\t* Enter 'W' TO KNOW ALARM STATUS        *");
        putstring("\r\n\t\t\t\t\t\t\t*                                       *");
        putstring("\r\n\t\t\t\t\t\t\t*****************************************\r\n");
        //printf("\r\n RESET FLAG:%d!", reset_flag);
        ch = getcharac();

        switch(checkChar(ch))
        {
        /* */
        case PAUSE:
            DISABLE;
            break;
        case START:
            ENABLE;
            break;
        case RESET:
            reset_flag = 1;
            printf("\r\n RESET FLAG:%d!", reset_flag);
            break;
        case ALARM:
            putstring("\r\n\t\t\t\t\t\t\t*               SELECT                  *");
            putstring("\r\n\t\t\t\t\t\t\t*    ALARM1     ALARM2     ALARM3       *");
            putstring("\r\n\t\t\t\t\t\t\t*      (1)       (2)        (3)         *");
            ch = getcharac();
            ch -= '0';
            if((1 == ch) || (2 == ch) || (3 == ch))
            {
                putstring("\r\n\t\t\t\t\t\t\t*   What time to set? [MMSS]            *");
            }
            else
                continue;
            getstring_alarm(time_str);
            if((time_val[ch -1] = compute_alarmtime(time_str)) == 0)
            {
                error(7);
                continue;
            }
            printf("\r\nTime set in sec %d", time_val[ch -1]);

            break;
        case DALARM:
            putstring("\r\n\t\t\t\t\t\t\t*               SELECT                  *");
            putstring("\r\n\t\t\t\t\t\t\t*    ALARM1     ALARM2     ALARM3       *");
            putstring("\r\n\t\t\t\t\t\t\t*      (1)       (2)        (3)         *");
            ch = getcharac();
            ch -= '0';
            if((1 == ch) || (2 == ch) || (3 == ch))
            {
                lcdgotoxy(ch-1, 0);
                lcdputstr("                ");
                alarm_status_d |= (0x01 << (ch-1));
                alarm_status &= ~(0x01 << (ch-1));
            }
            break;

        case EALARM:
            putstring("\r\n\t\t\t\t\t\t\t*               SELECT                  *");
            putstring("\r\n\t\t\t\t\t\t\t*    ALARM1     ALARM2     ALARM3       *");
            putstring("\r\n\t\t\t\t\t\t\t*      (1)       (2)        (3)         *");
            ch = getcharac();
            ch -= '0';
            if((1 == ch) || (2 == ch) || (3 == ch))
            {
                alarm_status |= 0x01 << (ch-1);
                alarm_status_d &= ~(0x01 << (ch-1));
            }
            break;
        case STATUS:
            putstring("\r\n\t\t\t\t\t\t\t*               STATUS                  *");
            printf_tiny("\r\n\t\t\t\t\t\t\t*               %d                    *", (alarm_status & 0x07));

            break;
        default:
            return;
        }
    }
}

/* Computes time to be set in the alarms from string inputs given by the user */
unsigned int compute_alarmtime(unsigned char *str)
{
    unsigned int timeval_s = 0;
    unsigned char i = 0;
    for(i = 0; i < 4; i++)
    {
        if((str[i] < '0') || (str[i] > '9'))
            return 0;
    }
    printf("\r\nString %s", str);
    printf("\r\ntimeval %d", timeval_s);
    timeval_s += (((str[0]-'0') * 10) + (str[1] - '0')) * 60;
    printf("\r\ntimeval %d", timeval_s);
    timeval_s += (((str[2]-'0') * 10) + (str[3] - '0'));
    printf("\r\ntimeval %d", timeval_s);

    return timeval_s;
}

