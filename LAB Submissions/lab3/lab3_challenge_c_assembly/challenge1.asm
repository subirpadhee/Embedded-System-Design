.globl _asm_func_PARM_2 ; 2nd argument passed from the function called is stored in this variable
.globl _asm_func_PARM_3 ; 3rd argument passed from the function called is stored in this variable
.globl _asm_func
.area OSEG
_asm_func_PARM_2:
.ds 1 ; size occupied by the parameter - 1 byte
_asm_func_PARM_3:
.ds 1 ; size occupied by the parameter - 1 byte
.area CSEG
_asm_func: ; this routine is called from the c program
	MOV A, DPL ; Move the first param(stored in DPL) to Accumulator
	MOV B, _asm_func_PARM_2 ; Move the second param to B
	DIV AB 
	MOV B, _subir_g ;Move the value stored in the global variable subir_g to B(instead of moving the 3rd parameter. This is done to demonstrate that global var can be accessed from assembly prog)
	MUL AB
	MOV DPL, A; Move the lower byte of the result to DPL
	lcall _sum; call the c function sum() with the value in DPL as the argument
	MOV DPL, A ; Move the lower byte of the result to DPL
	MOV DPH, B ; Move the upper byte of the result to DPL
	RET