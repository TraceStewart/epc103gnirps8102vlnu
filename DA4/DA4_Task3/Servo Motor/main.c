/*
 * Servo Motor.c
 *
 * Created: 3/23/2018 7:28:44 AM
 * Author : trace
 */ 

#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int main(void)
{
	// initialize ADC
	DDRC = 0;			// Set PORTC as input for adc
	DIDR0 = 0x1;		// Disable digital input on ADC0 pin
	ADMUX = 0;			// ADC0 (PC.0) used as analog input
	ADMUX |= (1 << REFS0);	// use AVcc as the reference
	ADMUX |= (1 << ADLAR);	// Right adjust for 8 bit resolution
	
	ADCSRA = 0x87;		// Enable ADC, system clock, 10000111
	ADCSRB = 0x0;		// Free running mode
	
	//Configure	TIMER1
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);	//NON	Inverted	PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64	MODE	14(FAST	PWM)
	ICR1=4999;		//fPWM=50Hz	(Period	=	20ms	Standard).
	DDRB|=(1<<PB1);			//PWM Pin as Out
	
	while(1)
	{
		ADCSRA |= (1 << ADSC);			// Start conversion
		while((ADCSRA&(1<<ADIF))==0);	// Wait for conversion to finish
		
		//OCR1A=75;	//90 degrees
		//OCR1A=290;	//0 degrees
		OCR1A = (ADCH * 5 / 6) + 75;

	}

}