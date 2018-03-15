;
; DA2_5.asm
;
; Created: 3/10/2018 4:29:23 PM
; Author : trace
;

.ORG 0 ;location for reset
	JMP MAIN
.ORG 0x02 ;location for EXT_INT0
	JMP EX0_ISR

MAIN: 
	LDI R20,HIGH(RAMEND)
	OUT SPH,R20
	LDI R20,LOW(RAMEND)
	OUT SPL,R20
	
	SBI DDRB,2 ;PB2 = output
	SBI PORTD,2 ;pull-up activated
	LDI R20,1<<INT0 ;Enable INT0
	OUT EIMSK,R20
	LDI R20, (1<<ISC00 | 1<<ISC01) ;Fall Edge
	STS EICRA,R20
	SEI ;Set I (Enable Interrupts)

HERE:
	JMP HERE

EX0_ISR: 
	LDI R20, 1<<INTF0
	STS EIFR, R20 ; clear flag
	IN R21,PORTB
	LDI R22,0x04
	EOR R21,R22
	OUT PORTB,R21
RETI