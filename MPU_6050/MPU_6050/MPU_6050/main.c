/*
	Jiang Yifan
	August 2014
*/


#define F_CPU 8000000UL

#include <inttypes.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include "mpu6050.h"
#include "mpu6050_reg.h"
#include "i2c.h"

#define BAUD  9600
#define MYUBRR F_CPU/16/BAUD-1

void timer_setup();
void get_time(double* dt);
volatile double count;
const double unit_t = 8/16000000;

// initialize UART
void uart_init();
void uart_putchar(char data);
char uart_getchar(void);
void uart_putstring(char * s);
//send int16 as 2 chars
//care needed on PC side
void uart_putint16(int16_t data);
//send double as 8 chars
//care needed on PC side
void uart_putdouble(double data);

int main(void){

sei();  
uart_init();
i2c_init();

DDRB |= _BV(5);

uint8_t ret;

int16_t accel_buff[3], gyro_buff[3];
double accelX, accelY, accelZ;
double gyroX, gyroY, gyroZ;
double biasX, biasY;
double phi_accel, theta_accel;
double phi_innov, theta_innov;
double phi_est, theta_est;
double phi_prev, theta_prev;

double dt;
char s[30];

// initialize & test MPU5060 availability
ret = i2c_start(MPU6050_ADDRESS+I2C_WRITE);
if(~ret){
	PORTB |= _BV(5);
	_delay_ms(200);
	PORTB &= ~(_BV(5));
}

mpu6050_init();
timer_setup();

// find gyro bias
biasX = 0;
biasY = 0;
uint8_t i;
for(i=0; i<20; i++){
	mpu6050_read_gyro_ALL(gyro_buff);
	biasX += gyro_buff[0];
	biasY += gyro_buff[1];
	/*
	PORTB |= _BV(5);
	_delay_ms(20);
	PORTB &= ~(_BV(5));
	*/
}
biasX = biasX/20*(3.14159/180)/1000/32768;
biasY = biasY/20*(3.14159/180)/1000/32768;

// initialization for Kalman filter
double P = 0.0;
double Q = 0.001;
double R = 0.03;
double Pp, K;
mpu6050_read_accel_ALL(accel_buff);
phi_prev = atan2(accelY, accelZ); // row
theta_prev = atan2(-accelX, sqrt(accelY*accelY+accelZ*accelZ)); // pitch

for(;;){
	get_time(&dt);
	mpu6050_read_accel_ALL(accel_buff);
	mpu6050_read_gyro_ALL(gyro_buff);
	
	// acceleration (m/s^2)
	accelX = accel_buff[0]*9.8*2/32768;
	accelY = accel_buff[1]*9.8*2/32768;
	accelZ = accel_buff[2]*9.8*2/32768;
	// gyro rate (rad/s)
	gyroX = gyro_buff[0]*(3.14159/180)/1000/32768;
	gyroY = gyro_buff[1]*(3.14159/180)/1000/32768;
	gyroZ = gyro_buff[2]*(3.14159/180)/1000/32768;

	// estimation
	phi_est = phi_prev + dt*(gyroX - biasX);
	theta_est = theta_prev + dt*(gyroY - biasY);
	Pp = P+Q;

	// innovation
	phi_accel = atan2(accelY, accelZ); // row
	phi_innov = phi_accel - phi_est;
	theta_accel = atan2(-accelX, sqrt(accelY*accelY+accelZ*accelZ)); // pitch
	theta_innov = theta_accel - theta_est;

	// Kalman gain
	K = Pp/(Pp+R);

	// correction
	phi_prev = phi_prev + K*phi_innov;
	theta_prev = theta_prev + K*theta_innov;
	P = (1-K)*Pp;
	
	uart_putchar('\n');
	_delay_ms(10);
	uart_putchar(phi_prev); //phi, row
	uart_putdouble(theta_prev); //theta, pitch	
	uart_putdouble(dt);
	//_delay_ms(10);
		
}

}//end of main



void timer_setup(){
	TCCR1A = 0x00;
	TIMSK1 |= _BV(TOIE1);
	TCCR1B |= _BV(CS11);  
	TCCR1B &= ~( _BV(CS12)  | _BV(CS10)); // prescaler=8

}



void get_time(double * dt){
	cli();
		uint8_t l = TCNT1L;
		uint8_t h = TCNT1H;
		uint16_t step = h<<8 | l;
		*dt = (double)step*5e-7 + count*0.032768;
		count = 0;
	sei();
}


// timer 1 overflow interrupt handler
SIGNAL(TIMER1_OVF_vect){
	count += 1;
	//TCNT1H = 0x00;
	//TCNT1L = 0x00;

}

// initialize UART
void uart_init() {
	
	/*Set Baud Rate*/
	UBRR0H = (MYUBRR>>8);      //Shift MSB "top" of UBRR0H 0100 0100 >> 8 -> UBRR0H 0000 0000
	UBRR0L = MYUBRR;           //UBRR0L 0100 0100
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   //Enable Rec and Trans
	UCSR0B |= (1 << RXCIE0);                        //Enable Rec INT
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //Set frame 8-bit, 1 STP

	
}

void uart_putchar(char data) {
	UDR0 = data;                  // Display character on serial (i.e., PuTTY) terminal
	_delay_ms(400);             // delay for 200 ms
}

char uart_getchar(void) {
	while ( !(UCSR0A & (1<<RXC0)) ) ;
	return UDR0;
}

void uart_putstring(char * s) {
	unsigned int i = 0;
	while(s[i] != 0)
	uart_putchar(s[i++]);
	
}

void uart_putint16(int16_t data) {
	uint8_t a = data & 0xFF;
	uart_putchar(a);
	a = data >> 8;
	uart_putchar(a);

}


void uart_putdouble(double data){
	char * a = &data;
	uart_putchar(*(a++));
	uart_putchar(*(a++));
	uart_putchar(*(a++));
	uart_putchar(*a);

}


