;
; DA2.asm
;
; Created: 2/26/2018 1:22:46 PM
; Author : trace
;

;0.5 Second Period with 50% DC
RESET:
	SBI DDRB, 2 ;PB2 is output
	LDI R16, 0	;R16 = 0
	OUT PORTB, R16	;output Port B
	LDI R17, 0x02	;2nd bit = 1
	LDI R18, 30		;loop initialization
START:
	LDI R19, 0x0		;R19 = 0
	OUT TCNT0, R19	;Timer0 = 0
	OUT TCCR0A, R18	;Timer0 = normal
	LDI R20, (1 << CS00) | (1 << CS02)	
	OUT TCCR0B, R20	;Enable Timer0 and set prescalar = 1024
LOOP:
	IN	R21, TIFR0	;Check Timer0 flag register
	SBRS R21, 0	;if overflow, dont jmp
	RJMP	 LOOP
	LDI R21, 0x0		;R21 = 0
	OUT TCCR0B, R21	;Stop Timer0
	LDI R21, (1 << TOV0) 
	OUT TIFR0, R21	;Clear overflow flag
	DEC R21	; R21 = R21 - 1
	BRNE	 START
TOGGLE:
	EOR R16, R17		;R16 xor R17
	OUT PORTB, R18	;TOGGLE PB2
	LDI R21, 30	;Reinitialize loop
	RJMP START

