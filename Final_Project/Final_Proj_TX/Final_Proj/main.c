/*
 * Final_Proj.c
 *
 * Created: 4/24/2018 8:59:09 AM
 * Author : trace
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include "i2c_control.h"
#include "MPU6050.h"
#include "usart.h"
#include "ESP8266.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"

#define FOSC 16000000
#define BAUD  19200
#define MYUBRR FOSC/16/BAUD-1

void setup_timer(void);
nRF24L01 *setup_rf(void);
void initUART();
void ADC_Init();

volatile bool rf_interrupt = false;
volatile bool send_message = false;
volatile uint8_t adcValue;


int GYRO_XOUT_OFFSET = 0;		//initialize GYRO offset
int GYRO_XOUT_OFFSET_SUM = 0;	//initialize sum of GYRO offset
int GYRO_YOUT_OFFSET = 0;		//initialize GYRO offset
int GYRO_YOUT_OFFSET_SUM = 0;	//initialize sum of GYRO offset
int GYRO_ZOUT_OFFSET = 0;		//initialize GYRO offset
int GYRO_ZOUT_OFFSET_SUM = 0;	//initialize sum of GYRO offset

int GYRO_XOUT_OFFSET2 = 0;		//initialize GYRO offset
int GYRO_XOUT_OFFSET_SUM2 = 0;	//initialize sum of GYRO offset
int GYRO_YOUT_OFFSET2 = 0;		//initialize GYRO offset
int GYRO_YOUT_OFFSET_SUM2 = 0;	//initialize sum of GYRO offset
int GYRO_ZOUT_OFFSET2 = 0;		//initialize GYRO offset
int GYRO_ZOUT_OFFSET_SUM2 = 0;	//initialize sum of GYRO offset

int acc_xout = 0;
int acc_yout = 0;
int acc_zout = 0;

int gyro_xout = 0;
int gyro_yout = 0;
int gyro_zout = 0;

/*NEW 05.05.2018*/
int acc_xout2 = 0;
int acc_yout2 = 0;
int acc_zout2 = 0;

int gyro_xout2 = 0;
int gyro_yout2 = 0;
int gyro_zout2 = 0;
	

int angle = 0; //initialize angle
int gyro_ratex = 0; //initialize the gyro_rate
int gyro_ratey = 0; //initialize the gyro_rate
int gyro_ratez = 0; //initialize the gyro_rate

int xgyro_angle = 0; //initialize gyro_angle
int ygyro_angle = 0;
int zgyro_angle = 0;

/*NEW 05.05.2018*/
int angle2 = 0; //initialize angle
int gyro_ratex2 = 0; //initialize the gyro_rate
int gyro_ratey2 = 0; //initialize the gyro_rate
int gyro_ratez2 = 0; //initialize the gyro_rate

int xgyro_angle2 = 0; //initialize gyro_angle
int ygyro_angle2 = 0;
int zgyro_angle2 = 0;




void get_values(void)
{
	i2c_start(MPU6050_WRITE); //set the address of MPU6050 write register
	i2c_write(ACCEL_XOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ); //set the address to READ register
	acc_xout = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	acc_xout |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	
	i2c_start(MPU6050_WRITE); //set the address of MPU6050 write register
	i2c_write(ACCEL_YOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ); //set the address to READ register
	acc_yout = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	acc_yout |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	
	i2c_start(MPU6050_WRITE); //set the address of MPU6050 write register
	i2c_write(ACCEL_ZOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ); //set the address to READ register
	acc_zout = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	acc_zout |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	//***************************************************
	
	i2c_start(MPU6050_WRITE); //set the address of MPU6050 write register
	i2c_write(GYRO_XOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ); //set the address to READ register
	gyro_xout = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	gyro_xout |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	
	i2c_start(MPU6050_WRITE); //set the address of MPU6050 write register
	i2c_write(GYRO_YOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ); //set the address to READ register
	gyro_yout = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	gyro_yout |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	
	i2c_start(MPU6050_WRITE); //set the address of MPU6050 write register
	i2c_write(GYRO_ZOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ); //set the address to READ register
	gyro_zout = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	gyro_zout |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission


}

void calc_values(void)
//This function calculates the angle of IMU. The angle is calculated by taking the gyro's x value and
//converting from degrees/second to degrees. The gyro contains a good amount of DRIFT from outside noise,
//the accelerometer is used to attempt to offset this noise
{
	gyro_ratex = (gyro_xout - GYRO_XOUT_OFFSET)/1.0131; //the gyro's rate is calculated
	xgyro_angle += gyro_ratex; //the angle is added to the rate divided by sample rate to get the angle of the IMU

	gyro_ratey = (gyro_yout - GYRO_YOUT_OFFSET)/1.0131; //the gyro's rate is calculated
	ygyro_angle += gyro_ratey; //the angle is added to the rate divided by sample rate to get the angle of the IMU
	
	gyro_ratez = (gyro_zout - GYRO_ZOUT_OFFSET)/1.0131; //the gyro's rate is calculated
	zgyro_angle += gyro_ratez; //the angle is added to the rate divided by sample rate to get the angle of the IMU
}

void gyro_calibrate(void)
//This function calibrates the GYROSCOPE by getting the value of the gyro's x-axis and adding the
//values. The values are then averaged and the OFFSET of the gyroscope is obtained
{
	for (int i=0; i<1000; i++) //loop for 1000 iterations
	{
		get_values(); //get the GYRO values by calling get_values
		
		GYRO_XOUT_OFFSET_SUM += gyro_xout; //add next gyro_xout value to offset sum
		GYRO_YOUT_OFFSET_SUM += gyro_yout; //add next gyro_xout value to offset sum
		GYRO_ZOUT_OFFSET_SUM += gyro_zout; //add next gyro_xout value to offset sum
		
	}
	GYRO_XOUT_OFFSET = GYRO_XOUT_OFFSET_SUM/1000; //average the OFFSET to get the official offset
	GYRO_YOUT_OFFSET = GYRO_YOUT_OFFSET_SUM/1000; //average the OFFSET to get the official offset
	GYRO_ZOUT_OFFSET = GYRO_ZOUT_OFFSET_SUM/1000; //average the OFFSET to get the official offset
}

void MPU6050_init(void)
//This function initialize the MPU6050 by sending various commands to the IMU
{
	i2c_start(MPU6050_WRITE); //sets WRITE address of device
	i2c_write(PWR_MGMT_1); //sets address of power management 1
	i2c_write(0x00); //turns on the device
	i2c_stop(); //stop I2C connection
	
	i2c_start(MPU6050_WRITE);
	i2c_write(CONFIG); //set address to point to CONFIG register
	i2c_write(0x01); //write 0x01 to the register ()
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(GYRO_CONFIG); //set address to point to GYRO_CONFIG register
	i2c_write(0x10); //write 0x10 to register (1000 degrees/sec range)
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(ACCEL_CONFIG); //configures the accelerometer
	i2c_write(0x00); //write 0x00 to register (+/- 2g range)
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(INT_ENABLE); //set pointer to address of INTERRUPTS ENABLE
	i2c_write(0x00); //write 0x00 (no interrupts)
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(SIGNAL_PATH_RESET); //set pointer to address of MPU1 0x68
	i2c_write(0x00); //write 0x00 (no reset)
	i2c_stop();
}//end MPU6050_init

/*Added Functions TRACE/GUILLERMO*/
///////////////////////////////////
///////////////////////////////////
void MPU6050_init2(void)
//This function initialize the MPU6050 by sending various commands to the IMU
{
	i2c_start(MPU6050_WRITE2); //sets WRITE address of device
	i2c_write(PWR_MGMT_1); //sets address of power management 1
	i2c_write(0x00); //turns on the device
	i2c_stop(); //stop I2C connection
	
	i2c_start(MPU6050_WRITE2);
	i2c_write(CONFIG); //set address to point to CONFIG register
	i2c_write(0x01); //write 0x01 to the register ()
	i2c_stop();
	
	i2c_start(MPU6050_WRITE2);
	i2c_write(GYRO_CONFIG); //set address to point to GYRO_CONFIG register
	i2c_write(0x10); //write 0x10 to register (1000 degrees/sec range)
	i2c_stop();
	
	i2c_start(MPU6050_WRITE2);
	i2c_write(ACCEL_CONFIG); //configures the accelerometer
	i2c_write(0x00); //write 0x00 to register (+/- 2g range)
	i2c_stop();
	
	i2c_start(MPU6050_WRITE2);
	i2c_write(INT_ENABLE); //set pointer to address of INTERRUPTS ENABLE
	i2c_write(0x00); //write 0x00 (no interrupts)
	i2c_stop();
	
	i2c_start(MPU6050_WRITE2);
	i2c_write(SIGNAL_PATH_RESET2); //set pointer to address of MPU2 0x69
	i2c_write(0x00); //write 0x00 (no reset)
	i2c_stop();
}//end MPU6050_init2

void get_values2(void)
{
	i2c_start(MPU6050_WRITE2); //set the address of MPU6050 write register
	i2c_write(ACCEL_XOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ2); //set the address to READ register
	acc_xout2 = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	acc_xout2 |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	
	i2c_start(MPU6050_WRITE2); //set the address of MPU6050 write register
	i2c_write(ACCEL_YOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ2); //set the address to READ register
	acc_yout2 = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	acc_yout2 |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	
	i2c_start(MPU6050_WRITE2); //set the address of MPU6050 write register
	i2c_write(ACCEL_ZOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ2); //set the address to READ register
	acc_zout2 = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	acc_zout2 |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	//***************************************************
	
	i2c_start(MPU6050_WRITE); //set the address of MPU6050 write register
	i2c_write(GYRO_XOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ); //set the address to READ register
	gyro_xout2 = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	gyro_xout2 |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	
	i2c_start(MPU6050_WRITE); //set the address of MPU6050 write register
	i2c_write(GYRO_YOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ); //set the address to READ register
	gyro_yout2 = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	gyro_yout2 |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission
	
	
	i2c_start(MPU6050_WRITE); //set the address of MPU6050 write register
	i2c_write(GYRO_ZOUT_H); //set pointer to GYRO_XOUT_H register
	i2c_stop();
	
	i2c_start(MPU6050_READ); //set the address to READ register
	gyro_zout2 = ((uint8_t) i2c_read_ack())<<8; //read the xout value and acknowledge data received (continue SLAVE to MASTER transmission)
	gyro_zout2 |= i2c_read_nack(); //read xout_lower bits without acknowledge (stops SLAVE to MASTER transmission)
	i2c_stop(); //stop transmission


}//end getvalues2

void calc_values2(void)
//This function calculates the angle of IMU. The angle is calculated by taking the gyro's x value and
//converting from degrees/second to degrees. The gyro contains a good amount of DRIFT from outside noise,
//the accelerometer is used to attempt to offset this noise
{
	gyro_ratex2 = (gyro_xout2 - GYRO_XOUT_OFFSET2)/1.0131; //the gyro's rate is calculated
	xgyro_angle2 += gyro_ratex2; //the angle is added to the rate divided by sample rate to get the angle of the IMU

	gyro_ratey2 = (gyro_yout2 - GYRO_YOUT_OFFSET2)/1.0131; //the gyro's rate is calculated
	ygyro_angle2 += gyro_ratey2; //the angle is added to the rate divided by sample rate to get the angle of the IMU
	
	gyro_ratez2 = (gyro_zout2 - GYRO_ZOUT_OFFSET2)/1.0131; //the gyro's rate is calculated
	zgyro_angle2 += gyro_ratez2; //the angle is added to the rate divided by sample rate to get the angle of the IMU
}//end calc_values2

void gyro_calibrate2(void)
//This function calibrates the GYROSCOPE by getting the value of the gyro's x-axis and adding the
//values. The values are then averaged and the OFFSET of the gyroscope is obtained
{
	for (int i=0; i<1000; i++) //loop for 1000 iterations
	{
		get_values2(); //get the GYRO values by calling get_values
		
		GYRO_XOUT_OFFSET_SUM2 += gyro_xout2; //add next gyro_xout value to offset sum
		GYRO_YOUT_OFFSET_SUM2 += gyro_yout2; //add next gyro_xout value to offset sum
		GYRO_ZOUT_OFFSET_SUM2 += gyro_zout2; //add next gyro_xout value to offset sum
		
	}
	GYRO_XOUT_OFFSET2 = GYRO_XOUT_OFFSET_SUM2/1000; //average the OFFSET to get the official offset
	GYRO_YOUT_OFFSET2 = GYRO_YOUT_OFFSET_SUM2/1000; //average the OFFSET to get the official offset
	GYRO_ZOUT_OFFSET2 = GYRO_ZOUT_OFFSET_SUM2/1000; //average the OFFSET to get the official offset
}//end gyro_calibrate2

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//MAIN START
int main(void)
{
	
	  uint8_t to_address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
	  bool on = false;
	
	  sei();                     //enable global interrupts
	  setup_timer();
	  
	  nRF24L01 *rf = setup_rf();
	  
	
	PINC = 0xFF; //set pull up resistors to ensure proper data
	
	char xoutput[8];
	char youtput[8];
	char zoutput[8];

///NEW 05.05.2018
	char xoutput2[8];
	char youtput2[8];
	char zoutput2[8];
	
		//variable names appear to be repeated in main
		char xoutputgyro[6];
		char youtputgyro[6];
		char zoutputgyro[6];

		//variable names appear to be repeated in main
		char xoutputgyro2[6];
		char youtputgyro2[6];
		char zoutputgyro2[6];
	
	i2c_init();			//initialize all I2C/TWI interaface registers/module
	usart_init();		//initialize usart module on ATmega328p
	MPU6050_init();		//call to initialize the MPU6050
	MPU6050_init2();	//call to initalize MPU2
	gyro_calibrate();	//call to calibrate the gyro to zero it at a given position
	//gyro_calibrate2();  //call to calibrate 2nd gyro to zero it at a given position
	
	 
			
		  while (true) {
			  if (rf_interrupt) {
				  rf_interrupt = false;
				  int success = nRF24L01_transmit_success(rf);
				  if (success != 0)
				  nRF24L01_flush_transmit_message(rf);
			  }//end if under while(true)

			  if (send_message) {
				get_values();			 //get the values from the gyro and accel
				calc_values();			 //call function to calculate the values of the accel and gyro
		
		//NEW 05.05.2018
		get_values2();	 //get the values from the gyro and accel
		calc_values2();			//call function to calculate the values of the accel and gyro
				  
				  send_message = false;
				  on = !on;
				  nRF24L01Message msg;
				  nRF24L01Message msg2;
				  

				  
				  
				  //if (on)
				  //{
					  memcpy(msg.data, xoutput, 8);
					  memcpy(msg.data+8, youtput, 8);
					  memcpy(msg.data+16, zoutput, 8);
					  
					  ///NEW 05.05.2018
					  memcpy(msg2.data, xoutput2, 8);
					  memcpy(msg2.data+8, youtput2, 8);
					  memcpy(msg2.data+16, zoutput2, 8);
					  
				  //}		  
				  
				  
				 msg.length = 24 + 1;
				 msg2.length = 24 + 1;
				  
				  usart_sends("Transmitting: ");
				  usart_sends("\r\n");
				  itoa(acc_xout, xoutput, 10);	 //Int to char* conversion
				  itoa(acc_xout2, xoutput2, 10); //Int to char* conversion
				  usart_sends("X ACC: ");
				  usart_sends(xoutput);
				 // usart_sends("     ");
				//  usart_sends(xoutput2);
				  usart_sends("\r\n");
				  
				  itoa(acc_yout, youtput, 10);	//Int to char* conversion
				  itoa(acc_yout2, youtput2, 10);//Int to char* conversion
				  usart_sends("Y ACC: ");
				  usart_sends(youtput);
				 // usart_sends("     ");
				 // usart_sends(youtput2);
				  usart_sends("\r\n");
				  
				  itoa(acc_zout, zoutput, 10);	//Int to char* conversion
				  itoa(acc_zout2, zoutput2, 10);//Int to char* conversion
				  usart_sends("Z ACC: ");
				  usart_sends(zoutput);
				//  usart_sends("     ");
				//  usart_sends(zoutput2);
				  usart_sends("\r\n");
				  usart_sends("\r\n");
				     
				/*
				//NEW 05.05.2018
				 msg.length = 24 + 1;
				 msg2.length = 24 + 1;
				   usart_sends(msg.data);
				   usart_sends("     ");
				   usart_sends(msg2.data);
				   usart_sends("\r\n");
				   usart_sends(msg.data+8);
				   usart_sends("     ");
				   usart_sends(msg2.data+8);
				   usart_sends("\r\n");
				   usart_sends(msg.data+16);
				   usart_sends("     ");
				   usart_sends(msg2.data+16);
				   usart_sends("\r\n");
				   usart_sends("\r\n");
				  */
				  

				   nRF24L01_transmit(rf, to_address, &msg);
				  //_delay_ms(1000);
				  //nRF24L01_transmit(rf, to_address, &msg2);
				
			  }
			  
	_delay_ms(2000);
		
		
	

		  
//**************************		
/*
		itoa(xgyro_angle, xoutputgyro, 10);	// Float to char* conversion
		usart_sends("X Gyro: ");
		usart_sends(xoutputgyro);
		itoa(xgyro_angle2, xoutputgyro2, 10);	// Float to char* conversion
		usart_sends("     ");
		usart_sends(xoutputgyro2);
		usart_sends("\r\n");
		
		itoa(ygyro_angle, youtputgyro, 10);	// Float to char* conversion
		usart_sends("Y Gyro: ");
		usart_sends(youtputgyro);
		itoa(ygyro_angle2, youtputgyro2, 10);	// Float to char* conversion
		usart_sends("     ");
		usart_sends(youtputgyro2);
		usart_sends("\r\n");
		
		itoa(zgyro_angle, zoutputgyro, 10);	// Float to char* conversion
		usart_sends("Z Gyro: ");
		usart_sends(zoutputgyro);
		itoa(zgyro_angle2, zoutputgyro2, 10);	// Float to char* conversion
		usart_sends("     ");
		usart_sends(zoutputgyro2);
		usart_sends("\r\n");
		usart_sends("\r\n");

		*/

				gyro_calibrate();
				//gyro_calibrate2();  //call to calibrate 2nd gyro to zero it at a given position
				xgyro_angle = 0;
				ygyro_angle = 0;
				zgyro_angle = 0;
				xgyro_angle2 = 0;
				ygyro_angle2 = 0;
				zgyro_angle2 = 0;
//*/
		  }//end while
		
			return 0;
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

// setup timer to trigger interrupt every second when at 1MHz
void setup_timer(void) {
	TCCR1B |= _BV(WGM12);
	TIMSK1 |= _BV(OCIE1A);
	TIMSK1 |= _BV(TOIE1);
	OCR1A = 7812;
	//OCR1A = 33000;
	TCCR1B |= _BV(CS10) | _BV(CS11);
}

// each one second interrupt
ISR(TIMER1_COMPA_vect) {
	send_message = true;
	
	TIFR1 |= (1 << TOV1);      //Clr Flag
	
	
}

// nRF24L01 interrupt
ISR(INT0_vect) {
	rf_interrupt = true;
	
}

