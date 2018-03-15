;
; DA2_4.asm
;
; Created: 3/6/2018 10:45:18 AM
; Author : trace
;

.org 0
	jmp  START
.org 0x20
	jmp  TIM0_OVF			; Timer0 overflow interrupt vector

START:
	; Toggle PORTB.2 every .5 second
	SBI	 DDRB,2		;PB.2 as an output
	LDI	 R18,0			;PB.2 = 0
	OUT	 PORTB,R18	
	LDI	 R16,0x04		;R16 = 0x04: bit 2 = 1
	LDI  R21, 15		;initialize loop count to 30
Begin:
	LDI	 R19, 0x0		;load Timer0 = 0
	OUT	 TCNT0,R19		
 	OUT	 TCCR0A,R18 		;Timer0: normal mode, internal clock
	LDI	 R17,(1<<CS00) | (1<<CS02) ;Timer0: enabled, prescalar = 1024
	OUT	 TCCR0B, R17

	;enable interrupts
	LDI 	 R20, 0x01 	
	STS  TIMSK0, R20 	;enable overflow interrupt
	SEI 				;enable global interrupts
Loop: 	
	RJMP LOOP			;infinite loop	

;Timer0 overflow ISR
TIM0_OVF:
	LDI	 R20,0x0		;stop/disable Timer0
	OUT	 TCCR0B,R20	
	LDI	 R20,(1<<TOV0)	;R20 = 0x01
	OUT	 TIFR0,R20		;clear TOV0 flag 
	DEC  R21			;R21--
	BRNE DONE			;repeat if Timer0 hasn't overflowed 15 times

	LDI  R21, 15	;reinitialize loop count to 15
	EOR	 R18,R16		;toggle bit 2 of R18
	OUT	 PORTB,R18		;toggle PB.2
DONE:
	LDI	 R19, 0		;load Timer0 = 0
	OUT	 TCNT0,R19		
	LDI	 R17,(1<<CS00) | (1<<CS02) ;Timer0: enabled, prescalar = 1024
	OUT	 TCCR0B, R17
	RETI				;return from interrupt, interrupts enabled
