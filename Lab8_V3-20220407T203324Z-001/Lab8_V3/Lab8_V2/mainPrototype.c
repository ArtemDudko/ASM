/*
 * Lab8.c
 *
 * Created: 3/25/2022 10:20:16 AM
 * Author : nikta
 */ 

#include <avr/io.h>
#include "mBuffer.h"

void uart_9600();
void setLED(uint8_t,uint8_t,uint8_t);
void initPWM();

#define BUFFSIZE 64

uint8_t txBuffer[BUFFSIZE];
uint8_t rxBuffer[BUFFSIZE];

MBUFFER TXQ;
MBUFFER RXQ;

int uart_putchar(char c,FILE* stream);
int uart_getchar(char c,FILE* stream);

FILE
FILE

int main(void)
{
    bufinit(&TXQ,cxBuffer, BUFFSIZE);
	bufinit(&RXQ, rxBuffer,BUFFSIZE);
	stdout = &mystdout;
	stdin = &mystdin;
	uart_9600();
	printf("enter name: ");
	fgets(name,40-1,stdin);
	name[strien]
    while (1) 
    {
    }
}

int uart_putchar(char c,FILE* stream)
{
	cli();
	enqueue(&TXQ, c);
	sei();
	UCSR0B |= 1<<UDRIE0;
}

int uart_getchar(FILE* stream)
{
	uint8_t t;
	while (isEmpty(&RXQ))
	;
	cli();
	t = dequeue(&RXQ)
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
			uart_getchar('\n',stdout);
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

