;
; ece263hw7.asm
;
; Created: 2/24/2022 9:26:19 PM
; Author : adudk


/*
1)
	a)weather temp: signed char, scientific temp: signed int
	b)unsigned char
	c)unsigned int
	d)unsigned char
	e)signed char
	f)unsigned char
	g)unsigned long
	h)unsigned char
	i)unsigned char

2)
	a)0xD
	b)0x18
	c)0x41

3)
	a)
			0110 0101
		AND	0111 0110
		   =0110 0100 = 0x64

	b)
			0111 0000
		OR	0110 1011
		   =0111 1011 = 0x7B

	c)		
			1001 0101
		XOR	1010 1010
		   =0011 1111 = 0x3F

	d)
			0101 1101
		AND	0111 1000
		   =0101 1000 = 0x57

	e)
			1100 0101
		OR	0001 0010
		   =1101 0111 = 0xD7

	f)
			0110 1010
		XOR 0110 1110
		   =0000 0100 = 0x04
	g)
			0011 0111
		OR	0010 0110
		   =0011 0111 = 0x37

26)

#inlcude "avr/pgmspace.h"
#include <avr/eeprom.h>

const char PROGMEM myName[] = "Artem Dudko";

int main()
{
	for (usigned char i = 0; i<9; i++)
	{
		c = pgm_read_byte(&myName[i]);
		while(EECR & (1<<EEWE));
		EEAR = 0x5F + i;
		EEDR = c;
		EECR |= (1<<EEMWE);
		EECR |= (1<<EEWE);
	}
	while(1)
}
	*/