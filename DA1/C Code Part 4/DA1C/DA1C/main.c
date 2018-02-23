/*
 * DA1C.c
 *
 * Created: 2/20/2018 9:07:27 AM
 * Author : trace
 */ 

#include <avr/io.h>

int main(void)
{
   volatile unsigned int xl = 0;
   volatile unsigned int xh = 0;
   volatile unsigned int x = 0; 
   volatile unsigned int y = 0;
   volatile unsigned int z = 0;
   
   for(xl=36; xl<=255; xl++) //This loop is for counting from 0x0222 to 0x2FD
   {
	
	if(xl % 5 == 0) //if xl is divisible by 5, add to y
	{
		y = xl + y;
	}
	if(xl % 5 != 0) //if xl isnt divisible by 5, add to z
	{
		z = xl + z;	
	}
   }
   
   for(xh=0; xh<=1; xh++) //This loop is for counting from 0x02FE to 0x02FF to account for the overflow
   {
	
	if(xh % 5 == 0) //if xh is divisible by 5, add to y
	{
		y = xh + y;
	}
	if(xh % 5 != 0) //if xh isnt divisible by 5, add to z
	{
		z = xh + z;	
	}
   }
   
   
   for(xh=3; xh<=80; xh++) //This loops is for counting from 0x0300 to 0x034E
   {
	
	if(xh % 5 == 0) //if xh is divisible by 5, add to y
	{
		y = xh + y;
	}
	if(xh % 5 != 0) //if xh isnt divisible by 5, add to z
	{
		z = xh + z;	
	}
   }
   
}
