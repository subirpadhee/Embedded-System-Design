/*  ESD-Final Project header - glcd.h
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/

#ifndef glcd_h
#define glcd_h

#define RS P1_3
#define RWbar P1_2
#define BF P0_7
#define CS2 P3_4//T0
#define CS1 P3_5 //T1
#define E P3_2


void glcd(unsigned char *str, unsigned char c);
void glcd_init();
void page_select(unsigned char page);
void column_select(unsigned char column);
void fill_column(unsigned char column_curr, unsigned char data_b);
void print_char(unsigned char column_curr, unsigned char *arrary_char);
void send_command(unsigned char command);
void delay(unsigned int delay_time);
void clear_glcd();
void clear_page(unsigned char page);










#endif //glcd.h
