/*
* DA2_C.c
 *
 * Created: 3/1/2018 11:19:22 AM
 * Author : trace
 */ 

#define	F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1<<2);
	DDRD &= ~(1<<2);//Makes firs pin of PORTD as Input
	PORTD |= (1<<2);
	
	while(1) //infinite loop
	{
		if(PIND & (1<<PD0) == 1) //If switch is pressed
		{
			_delay_ms(200);
			PORTB ^= 1<<2; //Toggle Led
			_delay_ms(1000);
			PORTB ^= 1<<2; //Toggle Led
		}
	}
}