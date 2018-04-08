/*
 * DAMidterm.c
 *
 * Created: 3/26/2018 9:04:20 PM
 * Author : trace
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <stdlib.h>
#define BAUD  9600

volatile int ovrflw;	// global variable for keeping track of # of times Timer0 overflows
volatile uint8_t ADCvalue; // Global variable, set to volatile if used with ISR

// functions
void initUART();
void writeChar(unsigned char c);
void writestring(char *c);
void writefloat(float c);

int main(void){

	initUART();		// Initialize UART
	
	// initialize ADC
	DDRC = 0;			// Set PORTC as input for adc
	DIDR0 = 0x1;		// Disable digital input on ADC0 pin
	ADMUX = 0;			// ADC0 (PC.0) used as analog input
	ADMUX |= (1 << REFS0);	// use AVcc as the reference
	ADMUX |= (1 << ADLAR);	// Right adjust for 8 bit resolution
	

	ADCSRA = 0x87;		// Enable ADC, system clock, 10000111
	ADCSRB = 0x0;		// Free running mode
	
	// initialize timer0 with starting value of 0, normal mode with no prescaler
	TCNT0 = 0;
	TCCR0A = 0;
	TCCR0B |= 2;
	
	// enable interrupts
	TIMSK0 |= (1 << TOIE0);		// enable overflow interrupt
	sei();						// enable global interrupts
	
	while (1);
	
	return 0;
}

void initUART() {
	unsigned int baudrate;
	
	// Set baud rate:  UBRR = [F_CPU/(16*BAUD)] -1
	baudrate = ((F_CPU/16)/BAUD) - 1;
	UBRR0H = (unsigned char) (baudrate >> 8);
	UBRR0L = (unsigned char) baudrate;

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);		// Enable receiver and transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);	// Set data frame: 8 data bits, 1 stop bit, no parity

}

void writeChar(unsigned char c) {
	UDR0 = c;			// Display character on serial (i.e., PuTTY) terminal
	_delay_ms(10);		// delay for 200 ms
}

void writestring(char *c){
	unsigned int i = 0;
	while(c[i] != 0)
	writeChar(c[i++]);
}



// this interrupt service routine (ISR) runs whenever an overflow on Timer0 occurs
ISR (TIMER0_OVF_vect) {
	
	// Variable Declarations
	char output[6];						// Output string based on ADC
	float temperature;						// Voltage received by ADC then edited for Temperature
	
	char *AT = "AT \r\n";
	char *CIPMUX = "AT+CIPMUX=0 \r\n";
	char *ATCW = "AT+CWJAP=\"Virus Detected\",\"Stewarts5+3\" \r\n";
	char *CIPSTART = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80 \r\n";
	char *CIPSEND = "AT+CIPSEND=44";
	char *SEND_DATA = "GET /update?key=M52FZABUR6UTS03B&field2=";
	char *ENTER = "\r\n";
	
	if (ovrflw == 7500) {
		
		ADCSRA |= (1 << ADSC);			// Start conversion
		while((ADCSRA&(1<<ADIF))==0);	// Wait for conversion to finish
		
		ADCvalue = ADCH;			// Only need to read the high value for 8 bit then equation for Fahrenheit
		temperature = (ADCvalue * 5.0 / 256) * 100;	// Temperature
		dtostrf(temperature, 0, 0, output);	// Float to char* conversion
	
		ovrflw = 0;				// reinitialize ovrflw
		
	_delay_ms(200);
	initUART(); // initialize usart
	_delay_ms(500);
	writestring(AT);
	_delay_ms(2000);
	writestring(CIPMUX);
	_delay_ms(5000);
	writestring(ATCW);
	_delay_ms(5000);
	writestring(CIPSTART);
	_delay_ms(5000);
	writestring(CIPSEND);
	writeChar('\r');
	writeChar('\n');
	_delay_ms(5000);
	writestring(SEND_DATA);
	_delay_ms(1000);
	writestring(output);
	_delay_ms(500);
	writestring(ENTER);
	_delay_ms(1000);
	
	for(int i=0; i < 1000; i++)
	{
	_delay_ms(1000);	
	}
		

	}
	else
	ovrflw++;	// increment ovrflw
}


