
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000
#include <util/delay.h>
#include <stdlib.h>
#define BAUD 9600

#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)
int main(void)
{ 
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); 
	UBRR0L = BAUD_PRESCALE; //set the baud to 9600, have to split it into the two registers
	UBRR0H = (BAUD_PRESCALE >> 8); //high end of baud register



    while (1) 
    { 
		
		int a = PeopleCount[0];
		int b =PeopleCount[1];
		int c =PeopleCount[2];
		int d =PeopleCount[3];
		int e=1000a+100b+10c+d;
	  uart_sendint16(e);
    }
}

void uart_sendint16(uint16_t data) {
    /*
    Use this to send a 16bit long piece of data
    */
    while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
    UDR0 = data;//send the lower bits
    while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
    UDR0 = (data >> 8); //send the higher bits

}