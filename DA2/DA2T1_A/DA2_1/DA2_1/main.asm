;
; DA2_1.asm
;
; Created: 3/6/2018 10:12:34 AM
; Author : trace
;


start:		
	; constant declaration (value & register)
	.EQU 	LOOPCNT = 244	; loop count for timer
	CLR		R0				; R0 = 0
	
	; port initialization
	LDI		R16, (1<<2)
	OUT		DDRB, R16		; Pb.2 output
	OUT		PORTB, R0		; PORTB = 0

	; variable initialization
	LDI		R16, (1<<2)		; R16 = 0x04: bit 2 = 1
	LDI		R17, LOOPCNT	; initialize loopCnt (loop count)
	CLR		R1				; counter = 0

init:
	OUT		TCNT0, R0					; initialize Timer0 to 0
	OUT		TCCR0A, R0					; Timer0: normal, internal clk
	LDI		R18, (1<<CS00) | (1<<CS01)	; Timer0: enabled, 64 prescalar
	OUT		TCCR0B, R18

timerLp:
	IN		R2, TIFR0		; read TOV0 (overflow)
	SBRS		R2, 0			; if (TOV0 is set), skip to next instr
	RJMP		timerLp			; jump back to timerLp
	CLR		R18				; stop Timer0
	OUT		TCCR0B, R18
	LDI		R18, (1<<TOV0)	; clear TOV0 flag
	OUT		TIFR0, R18
	DEC		R17				; loopCnt--
	BRNE		init

toggle:
	IN		R6, PORTB		; R6 = PORTB
	EOR		R6, R16			; toggle bit 0 of R0
	OUT		PORTB, R6		; toggle Pb.0
	LDI		R17, LOOPCNT	; reinitialize loopCnt
	JMP		init
 
 