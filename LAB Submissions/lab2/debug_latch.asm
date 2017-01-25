ORG 0x00
AJMP MAIN

ORG 0x0B;ISR for Timer 0
CPL P1.2; For ISR time measurement
AJMP TIMER0

ORG 0x80
TIMER0:

DJNZ R7, RETURN
MOV TH0, #0xBD; Reload Timer for the next run
MOV TL0, #0x21
CPL P1.0; For the LED
CPL P1.1; For debugging
MOV R7, #0x05
MOV A, R2
MOVX @DPTR, A; Write value held in R2 to external memory referenced by R5
MOV 0X60, A
CJNE R2, #0XFF, RETURN
MOV R2, #0X80; Re-initialize R2 if it has reached its upper limit of 0xFF
RETURN: 
CPL P1.2
RETI

ORG 0xB0
MAIN:
MOV TMOD, #0x01; use Timer 0
MOV TH0, #0xBD; load 48417 as initial value
MOV TL0, #0x21
SETB EA; enable interrupts in general
SETB ET0; enable Timer 0 interrupt
MOV R7, #0x05; 303 ms is achieved when timer overflows 5 times
MOV R2, #0X7F; Holds (initial value-1) to be written onto external memory when PC is in ISR
MOV R3, #0X00; Holds initial value to be written onto external memory when PC is in MAIN loop
MOV DPTR, #0X00; External memory address to be written onto
MOV 0X60, #0X00 
MOV A, #0X00
SETB TR0; start timer

LOOP1:
CJNE A, 0X60, LOOP2
INC R2; FOR ISR
ADD A, #0X01; to make sure A is changed and CJNE above results in a jump to LOOP2 till next time ISR is executed
INC R3; FOR MAIN
CJNE R3, #0X7F, inRANGE;
MOV R3, #0X00; Re-initialize R3 to 0 if its about to overflow
inRANGE:
MOV A, R3
MOVX @DPTR, A; Write the value in R3 to external memory
LOOP2:
AJMP LOOP1

END

