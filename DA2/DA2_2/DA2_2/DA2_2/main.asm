;
; DA2_2.asm
;
; Created: 3/6/2018 10:18:53 AM
; Author : trace
;

.org 0x0000
	ldi r16,0b00001111 ; Make the lower 4 bits output
	out ddrb,r16 ; for port b.
	LDI R20,5 ;to set prescaler
	STS TCCR1B,R20 ;Prescaler: 1024
top:
	CBI DDRD, 2
	SBIS PIND, 0 //skip next inst if pind=0
	RJMP top
	sbi portb,2 ; Set bit 0 immediate of port b
	rcall delay ; Calling a subroutine.	
	cbi portb,2 ; Clear bit 0 immediate of port b
	rjmp top ; Relative jump to label top

delay:
	LDS R29, TCNT1H ;loading upper bit of counter to R29
	LDS R28, TCNT1L ;loading lower bit of counter to R28
	CPI R28,0xFF ;comparing if lower is 0x84 10,000
	BRSH body
	RJMP delay
body:
	CPI R29,0x3D ;3906
	BRSH done
	RJMP delay
done:
	LDI R20,0x00
	STS TCNT1H,R20 ;resetting the counter to 0 for next round
	LDI R20,0x00
	STS TCNT1L,R20 ;resetting the counter to 0 for next round
	RET