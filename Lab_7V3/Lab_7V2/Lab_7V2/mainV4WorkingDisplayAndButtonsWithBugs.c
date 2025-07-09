/*
 * main.c
 *
 * Created: 3/21/2022 9:19:32 AM
 *  Author: Artem Dudko, Nikolas Tapanainen
 */ 


//#include <xc.h>
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
//new includes
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
volatile uint16_t val = 254;

uint8_t CheckButtons();
void ShowDigit(uint8_t val_var, uint8_t loc_var);
volatile uint8_t strobeDig = 0;//cycle_display_digits
volatile bool button_press = false;//cycle_display_digits


ISR(TIMER1_OVF_vect)
{
	uint16_t val_digit = val / (pow(10, strobeDig));  //choose value based on currently displayed digit, cdd counts up and resets to 0 after 3, cdd = 0 = first digit
	val_digit = val_digit % 10;
	
	
	ShowDigit(val_digit, strobeDig);
	
	strobeDig = (strobeDig + 1) % 4;
	TIFR1 = 0x1 << TOV1;
};


int main(void)
{
	DDRD = 0xFF;
	DDRB = 0xFF;
	DDRE = 0x00;
	
	TCCR1A = 0x00;
	TCCR1B = 0x01;
	TIMSK1 = (1<<TOIE1);
	sei();

    while(1)
    {
		_delay_ms(50);
		cli();
		if (CheckButtons() == 0)
		button_press = false;
		if (!button_press && (CheckButtons() == 1))
		{
			button_press = true;
			val++;
		}
		else if (!button_press && (CheckButtons() == 2))
		{
			button_press = true;
			val = 0;
		}
		else if (!button_press && (CheckButtons() == 3))
		{
			button_press = true;
			val--;
		}
		
		if (val == 10000)
		val = 0;
		else if (val > 10000)
		val = 9999;
		sei();
    }
}

void ShowDigit(uint8_t val_var, uint8_t loc_var)		//take in a 0-9 digit and display on portb
{
	
	if (loc_var == 0)
	PORTB = 8;
	else if (loc_var == 1)
	PORTB = 4;
	else if (loc_var == 2)
	PORTB = 2;
	else if (loc_var == 3)
	PORTB = 1;
	
	int val_to_pat[10] = //10 = 0
	{
	   // gfedcba
		0b00111111 ,	//0
		0b00000110 ,	//1
		0b01011011 ,	//2
		0b01001111 ,  
		0b01100110 ,
		0b01101101 ,
		0b01111101 ,
		0b00000111 ,
		0b01111111 ,
		0b01100111 ,	//9
		
	};
	PORTD = val_to_pat[val_var];

};

uint8_t CheckButtons()   //returns the button pressed, 0 = no new buttons, 1 = right was pressed, 2 = middle, 3 = left. Fxn ignores overlapping or continued inputs, must have no input at all to reset and take new inputs
{
	_delay_us(1);
	if(PINE == 0b00000111)
	return 0;
	else if(PINE == 0b00000110)
	return 1;
	else if(PINE == 0b00000101)
	return 2;
	else if(PINE == 0b00000011)
	return 3;
	
}



/*
uint8_t CheckButtons()
{
	uint8_t buttons_pressed;
	bool already_pressed;
	if(PINE == 0)							//might be PINE
	{
		buttons_pressed = 0;
		already_pressed = false;
	}
	else if (already_pressed == false && PORTE == 1)
	{
		//right button is down
		buttons_pressed = 1;
		already_pressed = true;
	}
	else if (already_pressed == false && PORTE == 2)
	{
		//right button is down
		buttons_pressed = 2;
		already_pressed = true;
	}
	else if (already_pressed == false && PORTE == 4)
	{
		//right button is down
		buttons_pressed = 3;
		already_pressed = true;
	}
	else
	{
		buttons_pressed = 0;
		already_pressed = true;
	}
	
	return buttons_pressed;
}*/

/*


#include <avr/io.h>
#include <util/delay.h>
//new includes
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

//volatile uint16_t val = 0;

//uint8_t CheckButtons();
//void ShowDigit(uint8_t val_var, uint8_t loc_var);

volatile uint16_t val = 1;

uint8_t CheckButtons();
void ShowDigit(uint8_t val_var, uint8_t loc_var);
volatile uint8_t cdd = 0;//cycle_display_digits

ISR(TIMER1_OVF_vect)
{
_delay_ms(400);
val++;
TIFR1 = 0x1 << TOV1;
//TCNT1 = (-1600);
};

int main(void)
{
DDRD = 0xFF;
DDRB = 0xFF;
DDRE = 0x00;
TCCR1A = 0x00;
TCCR1B = 0x01;
TIMSK1 = (1<<TOIE1);
sei();



while(1)
{
//uint8_t pressd = PINE ^ 0b00000111;
PORTB = 4;
PORTD = val;

}
*/

/*
#include <avr/io.h>
#include <util/delay.h>
//new includes
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

//volatile uint16_t val = 0;

//uint8_t CheckButtons();
//void ShowDigit(uint8_t val_var, uint8_t loc_var);

volatile uint16_t val = 1;

uint8_t CheckButtons();
void ShowDigit(uint8_t val_var, uint8_t loc_var);
volatile uint8_t cdd = 0;//cycle_display_digits

ISR(TIMER1_OVF_vect)
{
	_delay_ms(200);
	
	val++;
	
	PORTD = val;
	
	TIFR1 = 0x1 << TOV1;
	//TCNT1 = (-1600);
};

int main(void)
{
	DDRD = 0xFF;
	DDRB = 0xFF;
	DDRE = 0x00;
	TCCR1A = 0x00;
	TCCR1B = 0x01;
	TIMSK1 = (1<<TOIE1);
	sei();



	while(1)
	{
		//uint8_t pressd = PINE ^ 0b00000111;
		if(PINE == 0b00000111)  //no buttons connected
		PORTB = 8;
		else if (PINE == 0b00000110)
		PORTB = 4;
		else if (PINE == 0b00000101)
		PORTB = 2;
		else
		PORTB = 1;

	}
}
*/