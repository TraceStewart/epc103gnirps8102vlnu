/*
 * Stepper Motor.c
 *
 * Created: 3/23/2018 7:29:15 AM
 * Author : trace
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000L
#include <util/delay.h>
#include <stdlib.h>

volatile int ovrflw;	// global variable for keeping track of # of times Timer0 overflows
volatile uint8_t ADCvalue; // Global variable, set to volatile if used with ISR

int main(void)
{
	DDRB = 0xFF;
	
	// initialize ADC
	DDRC = 0;			// Set PORTC as input for adc
	DIDR0 = 0x1;		// Disable digital input on ADC0 pin
	ADMUX = 0;			// ADC0 (PC.0) used as analog input
	ADMUX |= (1 << REFS0);	// use AVcc as the reference
	ADMUX |= (1 << ADLAR);	// Right adjust for 8 bit resolution
	

	ADCSRA = 0x87;		// Enable ADC, system clock, 10000111
	ADCSRB = 0x0;		// Free running mode
	
// set up timer with prescaler = 64 and CTC mode
    TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
  
    // initialize counter
    TCNT1 = 0;
  
    // initialize compare value
    OCR1A = 0;
  
    // enable compare interrupt
    TIMSK1 |= (1 << OCIE1A);
	//sei();						// enable global interrupts
	
	
	
	while(1)
	{
		ADCSRA |= (1 << ADSC);			// Start conversion
		while((ADCSRA&(1<<ADIF))==0);	// Wait for conversion to finish

		if(ADCH < 245){
		ADCvalue = ADCH + 10;			// Only need to read the high value for 8 bit then equation for Fahrenheit
			PORTB = 0x0C;
			sei();	
			PORTB = 0x09;
			sei();	
			PORTB = 0x03;
			sei();	
			PORTB = 0x06;
			sei();	

}
	}
}

ISR (TIMER1_COMPA_vect) {
		
		for(int i=0;i<=ADCvalue;i++)
			{
				_delay_ms(1);
			}
	
}
