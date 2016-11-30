#ifndef UART_H
#define UART_H

#include<avr/io.h>
#include<util/delay.h>

#ifndef BAUDRATE
#define buad_rate 9600
#endif

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#define ubrr ((F_CPU/(16UL*buad_rate))-1)

void usart_int (void)
{
    UBRRL = ubrr;
    UBRRH = (ubrr >> 8);
    UCSRB |= (1 << RXEN) | (1 << TXEN);
    UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void usart_send (unsigned char data)
{
    while (!(UCSRA & (1 << UDRE)));
    UDR = data;
}

unsigned char usart_receive (void)
{
    while (!(UCSRA & (1 << RXC)));
    return UDR;
}

#endif
