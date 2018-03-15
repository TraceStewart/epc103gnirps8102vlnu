/*
* DA2_C.c
 *
 * Created: 3/1/2018 11:19:22 AM
 * Author : trace
 */ 

#define	F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>

int main ()
{
	DDRB = 1<<2;
	
	while(1)
	{
		_delay_ms(250);
		PORTB ^= 1<<2;
	}
	
}