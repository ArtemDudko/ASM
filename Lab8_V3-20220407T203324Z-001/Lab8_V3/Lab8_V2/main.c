/*
 * Lab8.c
 *
 * Created: 3/25/2022 10:20:16 AM
 * Author : nikta
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


#define F_CPU 16000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include <util/delay.h>
#include "mbuffer.h"


#define BUFFSIZE 160

uint8_t txBuffer[BUFFSIZE];
uint8_t rxBuffer[BUFFSIZE];

MBUFFER TXQ;
MBUFFER RXQ;

int uart_putchar(char c,FILE* stream);
int uart_getchar(FILE* stream);

FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE mystdin  = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);


void uart_9600();
void setLED(uint8_t,uint8_t,uint8_t);
void initPWM();

int main(void)
{
	 initPWM();
	 setLED(255,255,255);
	 char command_in[60];
	 char val_in[5];
	 int color_val_r = 0;
	 int color_val_g = 0;
	 int color_val_b = 0;
	 int debug = 0;
	 bufInit(&TXQ, txBuffer, BUFFSIZE);
	 bufInit(&RXQ, rxBuffer,BUFFSIZE);
	 stdout = &mystdout;
	 stdin = &mystdin;
	 uart_9600();
	 
	 
	 
	 while(1)
	 {
		 printf("\r\n\nEnter Command: ");
		 
		 fgets(command_in,40-1,stdin);
		 command_in[strlen(command_in)-2]='\0';
		 
		 if(strcmp(command_in, "?") == 0)
		 {
			 printf("\r\nList of commands:");
			 printf("\r\nR - set red value");
			 printf("\r\nG - set green value");
			 printf("\r\nB - set blue value");
			 printf("\r\nW - write value to LED");
			 printf("\r\nS - status");
			 printf("\r\n? - display commands \r\n");
		 }
		 else if(command_in[0] == 'G')
		 {
			 printf("\r\nValue: ");		
			 scanf("%d", &color_val_g);		
			 printf("Green LED set to: %d", color_val_g);			
		 }
		 else if(command_in[0] == 'R')
		 {
			 printf("\r\nValue: ");
			 scanf("%d", &color_val_r);			
			 printf("Red LED set to: %d", color_val_r);		
		 }
		 else if(command_in[0] == 'B')
		 {
			 printf("\r\nValue: ");
			 scanf("%d", &color_val_b);
			 printf("Blue LED set to: %d", color_val_b);		
		 }
		 else if(strcmp(command_in, "W") == 0)
		 {
			 setLED(color_val_r, color_val_g, color_val_b);
			 printf("\r\nR=%d, G=%d, B=%d sent to LED.", color_val_r, color_val_g, color_val_b);
		 }
		 else if(strcmp(command_in, "S") == 0)
		 {
			 printf("\r\nRed=%d, Green=%d, Blue=%d", color_val_r, color_val_g, color_val_b);
		 }
		 else
		 {
			 printf("\n\rCommand not recognized, for help use '?'");
		 }
		
		 
	 }
}

int uart_putchar(char c,FILE* stream)
{
	cli();
	enqueue(&TXQ, c);
	sei();
	UCSR0B |= 1<<UDRIE0;
	return 0;
}

int uart_getchar(FILE* stream)
{
	uint8_t t;
	while (isEmpty(&RXQ))
	;
	cli();
	t = dequeue(&RXQ);
	sei();
	return t;
}

ISR (USART0_UDRE_vect)
{
	char t;
	if (isEmpty(&TXQ))
	{
		UCSR0B &= ~(1<<UDRIE0);
	}
	else
	{
		t = dequeue(&TXQ);
		UDR0 = t;
	}
}

ISR (USART0_RX_vect)
{
	char t;
	uint8_t count;
	count = available(&RXQ);
	if (count > 3)
	{
		t = UDR0;
		if (t =='\r')
		{
			enqueue(&RXQ,'\r');
			enqueue(&RXQ,'\n');
			uart_putchar('\r',stdout);
			uart_putchar('\n',stdout);
		}
		else
		{
			enqueue(&RXQ,t);
			uart_putchar(t,stdout);
		}
	}
	else
	{
		uart_putchar('\a',stdout);
	}
}

void uart_9600()
{
	UBRR0 = 103;
	UCSR0B = 1<<RXCIE0 | 0<<TXCIE0 | 1<<UDRIE0 | 1<<RXEN0 | 1<<TXEN0 | 0<<UCSZ02;
	UCSR0C = 0b00<<UPM00 | 0b00<<UPM00 | 0<<USBS0 | 0b11<<UCSZ00;
	
}

void initPWM()
{
	DDRD |= 1<<6;
	TCCR0A = 0b10<<COM0A0 | 0b11<<WGM00;
	TCCR0B = 0b0<<WGM02 | 0b011<<CS00;
	
	DDRB |= 1<<1;
	TCCR1A = 0b10<<COM1A0 | 0b01<<WGM10;
	TCCR1B = 1<<WGM12 | 0b011<<CS10;
	
	DDRB |= 1<<3;
	TCCR2A = 0b10<<COM2A0 | 0b11<<WGM20;
	TCCR2B = 0b0<<WGM22 | 0b100<<CS20;
	
	DDRC |= 1<<3;
	PORTC |= 1<<3;
}

void setLED(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0A = 255 - r;
	OCR1A = 255 - g;
	OCR2A = 255 - b;
}