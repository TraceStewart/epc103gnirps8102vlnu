#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "usart.h"
#define BAUD 19200

void usart_init(void)
//This function intializes the USART module on the ATmega328p
{
	unsigned int baudrate;
	
	// Set baud rate:  UBRR = [F_CPU/(16*BAUD)] -1
	baudrate = ((F_CPU/16)/BAUD) - 1;
	UBRR0H = (unsigned char) (baudrate >> 8);
	UBRR0L = (unsigned char) baudrate;

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);		// Enable receiver and transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);	// Set data frame: 8 data bits, 1 stop bit, no parity
}

void usart_send(unsigned char data)
//This function takes a character datatype and loads it in to the register UDR0 for tramission
{
	UDR0 = data; //put DATA into buffer
	_delay_ms(10);
}

void usart_int(unsigned int data)
//This function takes a character datatype and loads it in to the register UDR0 for tramission
{
	UDR0 = data; //put DATA into buffer
	_delay_ms(10);
}

void usart_sends(char *s)
//This function takes in a string/character array and sends it one byte at a time to transmit a string
{
	unsigned int i = 0;
	while(s[i] != 0)
	usart_send(s[i++]);
}

unsigned int usart_receive(void)
//This function receives data from the RX0 pin and assigns that byte to the data buffer
{
	unsigned char data=0; //clear data of previous information
	while (!(UCSR0A & (1<<RXC0))); //wait until receive is complete
	data=UDR0; //assign data to UDR0
	
	return data; //return the byte of data
}