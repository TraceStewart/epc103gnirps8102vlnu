/*
* DA2_C.c
 *
 * Created: 3/1/2018 11:19:22 AM
 * Author : trace
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
	DDRD = (1<<2); //PD2 Input
	//PORTD |= (1<<2); //pull-up activated
	DDRB = (1<<2); //Makes PB2 output
	
	EIMSK = 1<<INT0;					// Enable INT0
	EICRA = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
	
	sei();				//Enable Global Interrupt
	
	while(1);

}

//Interrupt Service Routine for INT0
ISR(INT0_vect)
{

		PORTB ^= (1<<PB2); //Toggle PB2
		_delay_ms(1000);
		PORTB ^= (1<<PB2); //Toggle PB2
		EIFR |= (1<<INTF0); // clear the INT0 flag
	
}