/*
 * Lab5.c
 *
 * Created: 2/27/2022 10:03:32 PM
 *  Author: Artem Dudko, Nikolas Tapanainen
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#define EBIT 5 // connected to PC5
#define RSBIT 4 // connected to PC4 
//#define RWBIT 5 // connected to PB1

inline void E_HIGH() { PORTC |= 1<<EBIT; } // Function for Enable pin (PORTD = PORTD | 0b00100000;)
inline void E_LOW() { PORTC &= ~(1<<EBIT); } //PORTD = PORTD & 0b11011111

inline void RS_HIGH() { PORTC |= 1<<RSBIT; }   //PORTD = PORTD | 0b000100000;
inline void RS_LOW() { PORTC &= ~(1<<RSBIT); } //PORTD = PORTD & 0b11101111;

	inline void delay250ns() // delay function
	{
		asm volatile(
		"swap r16\t\n"
		"swap r16\t\n"
		"swap r16\t\n"
		"swap r16\t\n");
	}
	
	void writecommand(uint8_t d); //write command
	void writedata(uint8_t d);
	void printstring(char s[]); //print string routine
	void GotoXY(uint8_t x,uint8_t y); //goto XY subroutine
	

int main(void) 
{
    DDRC |= 0x30; // rs, e
	DDRD |= 0xFF; //D is output
	_delay_ms(100); // let LCD initialize
	writecommand( 0x30 ); //function set 4 bit mode
	_delay_ms(5); // wait at least 4 ms
	writecommand( 0x38 ); // function set 2 lines, 5x8 font
	_delay_us(100); // wait at least 100us
	writecommand( 0x08 ); // Turn off display
	writecommand( 0x01 ); // clear display
	_delay_ms(2); // wait at least 2ms
	writecommand(0x06); // increment cursor no shifting
	writecommand(0x0F); // turn on display
	
	GotoXY(2,0); //col 2 row 0
	printstring("Nikolas!");
	GotoXY(5,1);
	printstring("Artem!");
	while(1)
	{
	}
}

void writecommand(uint8_t d)
{
	E_LOW(); // E = 0
	RS_LOW(); // RS = 0
	E_HIGH(); // E = 0
	
	PORTD = d ;//(PORTD & 0xF0) | (d>>8); //write upper nibble
	delay250ns();
	
	E_LOW(); // data captured - upper nibble
	delay250ns(); // E needs to be low 250ns min
	
	/*E_HIGH(); // E back to high
	PORTD = (PORTC & 0xF0) | (d & 0x0F); // write lower nibble
	delay250ns();*/
	
	E_LOW();
	_delay_us(39);
}

void writedata(uint8_t d)
{
	E_LOW(); // E = 0
	RS_HIGH(); // RS = 1
	E_HIGH(); // E = 0
	
	PORTD = d ;//(PORTD & 0xF0) | (d>>8); //write upper nibble
	delay250ns();
	
	E_LOW(); // data captured - upper nibble
	delay250ns(); // E needs to be low 250ns min
	
	/*E_HIGH();
	PORTD = (PORTC & 0xF0) | (d & 0x0F); // write lower nibble
	delay250ns();*/
	
	E_LOW();
	_delay_us(39);
}

void printstring(char s[])
{
	uint8_t i = 0;
	while(s[i])
	writedata(s[i++]);
	
}

void GotoXY(uint8_t x,uint8_t y)
{
	writecommand(0x80 | ( y*0x40 + x));
}