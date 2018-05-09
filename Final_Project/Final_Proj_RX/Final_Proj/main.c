/*
* Midterm2_Receive.c
*
* Created: 4/15/2018 2:47:49 PM
* Author : trace
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"

#define F_CPU 8000000UL

#define FOSC 16000000
#define BAUD  19200
#define MYUBRR FOSC/16/BAUD-1


nRF24L01 *setup_rf(void);
void process_message(char *message);
inline void prepare_led_pin(void);
inline void  set_led_high(void);
inline void  set_led_low(void);
void USART_Init();
void writestring(char *c);
void writeChar(unsigned char c);

volatile bool rf_interrupt = false;

int main(void) {
	
	USART_Init();        // Initialize UART
	
	bool on = false;
	
	uint8_t address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01};
	prepare_led_pin();
	sei();
	nRF24L01 *rf = setup_rf();
	nRF24L01 *rf2 = setup_rf();
	
	nRF24L01_listen(rf, 0, address);
	
	//Added 05.05.2018
	//_delay_ms(500);
	nRF24L01_listen(rf2, 1, address);
	
	uint8_t addr[5];
	uint8_t addr2[5];
	
	nRF24L01_read_register(rf, CONFIG, addr, 1);
	
	nRF24L01_read_register(rf2, CONFIG, addr2, 1);	//Added 05.05.2018

	while (true) {
		if (rf_interrupt) {
			rf_interrupt = false;
			while (nRF24L01_data_received(rf) && nRF24L01_data_received(rf2)) {
				nRF24L01Message msg;
				nRF24L01Message msg2;
				nRF24L01Message msg3;
				msg2.pipe_number = 1;
				
				
				nRF24L01_read_received_data(rf, &msg);
				
				nRF24L01_read_received_data(rf2, &msg2); //Added 05.05.2018
				
				
				process_message((char *)msg.data);
				on = !on;
				if(on){
				
				writestring("Receiving: ");
				writestring("\r\n");
				writestring("X ACC: ");
				writestring(msg.data);
				//writestring("    x");
				//writestring(msg2.data);
				writestring("\r\n");
				
				writestring("Y ACC: ");
				writestring(msg.data+8);
				//writestring("    x");
				//writestring(msg2.data+8);
				writestring("\r\n");
				
					
				writestring("Z ACC: ");
				writestring(msg.data+16);
				//writestring("    x");
				//writestring(msg2.data+16);
				writestring("\r\n");
				writestring("\r\n");
					
				
				//_delay_ms(3000);

				}
			}

			nRF24L01_listen(rf, 0, address);
			_delay_ms(1200);
			nRF24L01_listen(rf2, 1, address);
		}
	}

	return 0;
}

void USART_Init()
{
	
	/*Set Baud Rate*/
	UBRR0H = (MYUBRR>>8);      //Shift MSB "top" of UBRR0H 0100 0100 >> 8 -> UBRR0H 0000 0000
	UBRR0L = MYUBRR;           //UBRR0L 0100 0100
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   //Enable Rec and Trans
	UCSR0B |= (1 << RXCIE0);                        //Enable Rec INT
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //Set frame 8-bit, 1 STP
}//end USART_int

void writeChar(unsigned char c) {
	UDR0 = c;                  // Display character on serial (i.e., PuTTY) terminal
	_delay_ms(320);             // delay for 200 ms
}

void writestring(char *c){
	unsigned int i = 0;
	while(c[i] != 0)
	writeChar(c[i++]);
}


nRF24L01 *setup_rf(void) {
	nRF24L01 *rf = nRF24L01_init();
	rf->ss.port = &PORTB;
	rf->ss.pin = PB2;
	rf->ce.port = &PORTB;
	rf->ce.pin = PB1;
	rf->sck.port = &PORTB;
	rf->sck.pin = PB5;
	rf->mosi.port = &PORTB;
	rf->mosi.pin = PB3;
	rf->miso.port = &PORTB;
	rf->miso.pin = PB4;
	// interrupt on falling edge of INT0 (PD2)
	EICRA |= _BV(ISC01);
	EIMSK |= _BV(INT0);
	nRF24L01_begin(rf);
	return rf;
}

void process_message(char *message) {

	if (strcmp(message, "ON") == 0)
	set_led_high();
	else if (strcmp(message, "OFF") == 0)
	set_led_low();

}

inline void prepare_led_pin(void) {
	DDRB |= _BV(PB0);
	PORTB &= ~_BV(PB0);
}

inline void set_led_high(void) {
	PORTB |= _BV(PB0);
}

inline void set_led_low(void) {
	PORTB &= _BV(PB0);
}

// nRF24L01 interrupt
ISR(INT0_vect) {
	rf_interrupt = true;
}
