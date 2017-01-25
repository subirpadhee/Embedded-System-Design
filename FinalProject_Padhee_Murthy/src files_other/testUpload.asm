/*  ESD-Final Project library - testupload.asm
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/
/*Author 		: Subir Kumar Padhee, Shrivathsa Murthy
  Date 			: 20/4/16
  Objective 	: asm code to print a string "BBBLO" to terminal
					
  bug reports? write to shmu7023@colorado.edu, supa2799@colorado.edu
  
*/
// We start from 0x400
ORG 400H
MOV TMOD,#20H
MOV TH1,#-3
MOV SCON,#50H
SETB TR1

// write 5 characters
CLR TI
MOV SBUF,#'B'
HERE1:
JNB TI,HERE1

CLR TI
MOV SBUF,#'B'
HERE2:
JNB TI,HERE2

CLR TI
MOV SBUF,#'B'
HERE3:
JNB TI,HERE3

CLR TI
MOV SBUF,#'L'
HERE4:
JNB TI,HERE4

CLR TI
MOV SBUF,#'0'
HERE5:
JNB TI,HERE5
END