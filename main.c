/*
 * GccApplication12.c
 *
 * Created: 16-07-2022 22:55:29
 * Author : mathu
 */ 

#ifndef F_CPU

# define F_CPU 16000000UL // clock speed is 16MHz

#endif


#include<avr/io.h>        // AVR header

#include<util/delay.h>    // delay header
#include <string.h>
#include <stdlib.h>
#include <math.h>



#define LCD_DATA PORTB          // port B is selected as LCD data port

#define ctrl PORTD              //  port D is selected as LCD command port

#define en PD7                  // enable signal is connected to port D pin 7

#define rw PD6                  // read/write signal is connected to port D pin 6

#define rs PD5                  // register select signal is connected to port D pin 5





//**************sending command on LCD***************//


void LCD_cmd(unsigned char cmd)

{

	LCD_DATA = cmd;      // data lines are set to send command*

	PORTD  &= ~(1<<rs);  // RS sets 0

	PORTD  &= ~(1<<rw);  // RW sets 0

	PORTD  |= (1<<en);   // make enable from high to low

	_delay_ms(10);

	PORTD  &= ~(1<<en);

	

	return;

}



//*****************write data on LCD*****************//

void LCD_write(unsigned char data)

{

	LCD_DATA= data;       // data lines are set to send command

	PORTD  |= (1<<rs);    // RS sets 1

	PORTD  &= ~(1<<rw);   // RW sets 0

	PORTD  |= (1<<en);    // make enable from high to low

	_delay_ms(10);

	PORTD &= ~(1<<en);

	

	return ;

}

void init_LCD(void)

{

	LCD_cmd(0x38);           // initialization in 8bit mode of 16X2 LCD

	_delay_ms(1);

	LCD_cmd(0x01);          // make clear LCD

	_delay_ms(1);

	LCD_cmd(0x02);          // return home

	_delay_ms(1);

	LCD_cmd(0x06);          // make increment in cursor

	_delay_ms(1);

	LCD_cmd(0x80);          // “8” go to first line and “0” is for 0th position

	_delay_ms(1);

LCD_cmd(0xC0);          

_delay_ms(1);

	

	return;

}
//*****************write STRING on LCD*****************//

void LCD_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	{
		char big = a[i];
		LCD_write(big);
	}
	
}

void adc(){
	ADMUX=0b01000000;
	ADCSRA=0b10000111;
}





int main(void)

{
	
	int t=0;
	DDRA=0b00000000;
	PINA=0b00000001;
	char a[5];
    char b[7];
	DDRB=0b11111111;              // set LCD data port as output

	DDRD=0b11100000;              // set LCD signals (RS, RW, E) as out put

	init_LCD();             // initialize LCD

	_delay_ms(100);         // delay of 100 Milli seconds

	

	LCD_cmd(0x0C);          // display on, cursor off

	_delay_ms(100);
	adc();
	
	int i;
	
	while(1){
		ADCSRA|=(1<<ADSC);
		while(!(ADCSRA &(1<<ADIF)));
		t=ADC;
		
		
		int b;
		b=3.5*20*log(5*t/(1023*0.775));
		
if(b<0){
	_delay_ms(10);
}
else{
		itoa(b,a,10);
		LCD_Write_String(a);

		
	}
		LCD_cmd(0xC0);
		LCD_Write_String("db");
				_delay_ms(100);
				LCD_cmd(0x01);
		
	}
	

	_delay_ms(10);
	
	



	_delay_ms(10);

	

	return 0;




}

