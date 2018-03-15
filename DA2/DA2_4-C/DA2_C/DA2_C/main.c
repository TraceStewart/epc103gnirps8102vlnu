/*
* DA2_C.c
 *
 * Created: 3/1/2018 11:19:22 AM
 * Author : trace
 */ 

#include "avr/io.h"
#include <avr/interrupt.h>

volatile int ovrflw;	// global variable for keeping track of # of times Timer0 overflows

int main(void) {
	ovrflw = 0;				// initialize itr to keep track of # of times
	// Timer0 overflows
	
	// port initialization
	DDRB = (1<<2);	// set PB2 as output
	PORTB = 0;		// initialize PORTB to 0

	
	// initialize timer0 with starting value of 0, normal mode with no pre scaler
	TCNT0 = 0;
	TCCR0A = 0;
	TCCR0B |= 1;
	
	// enable interrupts
	TIMSK0 |= (1 << TOIE0);		// enable overflow interrupt
	sei();						// enable global interrupts

	while(1) ;				// loop forever
}


// this interrupt service routine (ISR) runs whenever an overflow on Timer0 occurs
ISR (TIMER0_OVF_vect) {
	if (ovrflw == 15624) {
		PORTB ^= (1<<2);		// toggle PB2
		ovrflw = 0;				// reinitialize itr
}
else
ovrflw++;	// increment itr
}