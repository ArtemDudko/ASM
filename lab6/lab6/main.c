/*
 * Lab6.c
 *
 * Created: 3/14/2022
 *  Author: Artem Dudko, Nikolas Tapanainen
 */ 

//wiring: in video it's connected to PORTC, we did PORTD
//Input Capture Pin is PB0
//



//must also enable sprintf in libraries
//project > properties > toolchain > AVR Linker > General > check use vprintf
//misc > other linker flags > -lprintf_flt

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
//new includes
#include <avr/interrupt.h>
#include <stdio.h>

#define EBIT 5 // connected to PC5
#define RSBIT 4 // connected to PC4 
#define TRIGBIT 2
#define ECHOBIT 0
//#define RWBIT 5 // connected to PB1

inline void E_HIGH() { PORTC |= 1<<EBIT; } // 
inline void E_LOW() { PORTC &= ~(1<<EBIT); } //

inline void RS_HIGH() { PORTC |= 1<<RSBIT; }   //
inline void RS_LOW() { PORTC &= ~(1<<RSBIT); } //
	
inline void TRIG_HIGH() { PORTE |= 1<<TRIGBIT; } //pulse sent to UM
inline void TRIG_LOW() { PORTE &= ~(1<<TRIGBIT); } 
	
inline void ECHO_HIGH() { PORTB |= 1<<ECHOBIT; } //Pulse recieved back from UM, length of pulse is distance from object
inline void ECHO_LOW() { PORTB &= ~(1<<ECHOBIT); } 


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
	void GoToXY(uint8_t x,uint8_t y); //goto XY subroutine
	//new
	volatile uint16_t new_value=0, previous_value = 0; //include volatile so optimizer doesn't get rid of these


void send_signal() {
	TRIG_HIGH();
	_delay_us(10);
	TRIG_LOW();
}

int main(void) 
{
	uint16_t echo_rise = 0, echo_fall = 0;
	
	//new
	DDRE |= 1 << TRIGBIT; 
    float freq = 0.0; 
	char s[20]; 
	//COM bits 00
	//WGM - normal - 0000
	TCCR1A = 0b00<<COM1A0 | 0b00<<COM1B0 | 0b00<<WGM10;
	
	//new
	
	//ICPn - Input capture pin, when a change occurs here with the right edge detect,
	// the value of TCNTn is written to ICRn
	
	//ICNC1 = 0 Noise cancellation NOT enabled
	//ICES1 = 1 Input capture on rising edge
	//CS = 011 - divide by 64
	TCCR1B = 0<<ICNC1 | 1<<ICES1 | 0b00<<WGM12 | 0b011<<CS10;
	
	//TIMSK (Timer/Counter Interrupt Mask Register)
	//Need to set ICIE1 so interrupt is triggered when  ICF1 is set by hardware in response to an edge detect
	//ICF1 is in register TIFR1 (timer interrupt flag register)
	TIMSK1 = 1<<ICIE1;
	
	
	
	
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
	
	sei();
	while (1)
	{
		_delay_ms(500);
		send_signal();
		
	//	cli(); //to prevent interrupt occuring in middle of operation
	/*	uint16_t diff = new_value - previous_value; //global variable is the only way to work with an interrupt routine
		sei();
		freq = 250000.0f/diff;
		sprintf(s,"%10.1f",freq);
		GoToXY(2,0); printstring(s);
		//sprintf(s,"%8u",diff);*/
		//GoToXY(5,1); printstring(s);
		
	}
	
}

//new
ISR (TIMER1_CAPT_vect)
{
	previous_value = new_value;
	new_value = ICR1;
	
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

void GoToXY(uint8_t x,uint8_t y)
{
	writecommand(0x80 | ( y*0x40 + x));
}