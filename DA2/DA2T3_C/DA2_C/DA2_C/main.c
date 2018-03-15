/*
* DA2_C.c
 *
 * Created: 3/1/2018 11:19:22 AM
 * Author : trace
 */ 

#include <avr/io.h>

int main(void)
{
	unsigned int i;				// variable for the loop below
	
	DDRB = (1<<2);	// set PB2
	PORTB = 0;						// initialize PORTB to 0

	
	// initialize timer0 with no prescalar and normal mode
	TCCR0A = 0;
	TCCR0B = 1;
	TCNT0 = 0;		// initialize Timer0 = 0
	
	while (1){
		for(i=0; i<15624; i++){
			while((TIFR0 & (1<<0)) == 0) ;	// wait until overflow flag is set
			TIFR0 |= 1;					// clear overflow flag
		}
		PORTB ^= (1<<2);			// toggle PB2
	}
}
