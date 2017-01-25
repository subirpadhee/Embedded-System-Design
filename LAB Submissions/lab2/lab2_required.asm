ORG 0x00
AJMP MAIN

ORG 0x0B;ISR for Timer 0
CPL P1.2; For ISR time measurement
AJMP TIMER0; The entire code for the ISR may not fit in the limited address space, hence AJMP is used

ORG 0x80
TIMER0:

DJNZ R0, RETURN; 303 ms is achieved when timer overflows 5 times. R0 was initialized with 5.
MOV TH0, #0xBD; Reload Timer register for the next run
MOV TL0, #0x21; Reload Timer register for the next run
CPL P1.0; Output for the LED
MOV R0, #0x05; Reload R0 for the next run
CPL P1.1; For debugging/oscilloscope

RETURN: CPL P1.2
RETI

ORG 0xB0
MAIN:
MOV TMOD, #0x01; use Timer 0
MOV TH0, #0xBD; load 48417 as initial value
MOV TL0, #0x21; 0xBD21 IS 48417 in decimal
SETB TR0; start timer
SETB EA; enable interrupts in general
SETB ET0; enable Timer 0 interrupt
MOV R0, #0x05; 303 ms is achieved when timer overflows 5 times
JMP $ 

END

