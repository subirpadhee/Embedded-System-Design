ORG 00H
MOV A, 70H ; dividend X
MOV B, 71H ; divisor Y
MOV A, B ; Load divisor onto A
JZ ERRCODE2
MOV R1, #00H
;MOV A,70H
CPL A
INC A
MOV 72H, A ; -ve divisor stored in 72H
CLR A
ADD A, 70H ; Dividend(X) loaded to A
ADD A, #0FCH ; FC is -4: 4 subtracted
             ;A now has X-4
MOV 20H, A
MOV R6, A ; A backed up in R6
;ADD A, #0FCH
ANL A, #80H
ADD A, #80H
JZ ERRCODE1
MOV A,R6 ; Restore A: A has X-4
DIVISION: 
ADD A, 72H ; Subtract the divisor 
INC R1
MOV R6, A ; Back up A
ADD A, 72H ; Subtract the divisor
ANL A, #80H ; To check if its -ve 
MOV R2, A
MOV A, R6
CJNE R2, #80H, DIVISION
MOV 21H, R1
MOV 22H, A
MOV A, R1
RL A
RL A
RL A
RL A ; 4 times RL=> x16
MOV 23H, A
MOV 30H, #00H ; Error code 0
JMP ENDLOOP

ERRCODE1:
MOV 30H, #01H
JMP ENDLOOP

ERRCODE2:
MOV 30H, #02H

ENDLOOP:
JMP $
END










