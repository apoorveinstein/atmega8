#include<avr/io.h>
#include<util/delay.h>

#define buad_rate 9600
#define F_CPU 1000000UL
#define ubrr ((F_CPU/(16UL*buad_rate))-1)

#define RS 0
#define RW 1
#define EN 2

void usart_int();
void usart_send(unsigned char);
unsigned char usart_receive();

void dis_data(unsigned char);
void dis_cmd(unsigned char);
void LCD_data(unsigned char);
void LCD_cmd(unsigned char);
void LCD_int();
void LCD_str(unsigned char*);
void LCD_no(unsigned int);

void main()
{
usart_int();
DDRC=0b0001111;
DDRB=0b11111111;
LCD_int();
unsigned char x;
while(1)
{
x=usart_receive();
usart_send(x);

dis_cmd(0x01);
_delay_ms(1);
dis_cmd(0x8F);
LCD_no(x);
_delay_ms(10);
}
}

void usart_int()
{
UBRRL=ubrr;
UBRRH=(ubrr>>8);
UCSRB|=(1<<RXEN)|(1<<TXEN);
UCSRC|=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}

void usart_send(unsigned char data)
{
while(!(UCSRA&(1<<UDRE)));
UDR=data;
}

unsigned char usart_receive()
{
while(!(UCSRA&(1<<RXC)));
return UDR;
}

void LCD_data(unsigned char y)
{
PORTB=y;

PORTB|=(1<<RS);//data register

PORTB&=~(1<<RW);//write
PORTB|=(1<<EN);//enable
_delay_ms(1);
PORTB&=~(1<<EN);//not enable
}
void LCD_cmd(unsigned char x)
{
PORTB=x;

PORTB&=~(1<<RS);

PORTB&=~(1<<RW);
PORTB|=(1<<EN);
_delay_ms(1);
PORTB&=~(1<<EN);
}
void LCD_str(unsigned char *str)
{
int i=0;
while(str[i]!='\0')
{
dis_data(str[i]);
i++;
}
_delay_ms(1);
}

void LCD_no(unsigned int value)
{ 
 dis_cmd(0x04);
 if(value==0)
 {
 dis_data(48);
 }
 unsigned int a=0;
 while(value!=0)
 {
  a=value%10;
  dis_data(a+48);
  value=value/10;
 }
 dis_cmd(0x06);
 }

void dis_data(unsigned char y)
{
char div1;
div1=y&0xf0;

LCD_data(div1);
div1=((y<<4)&0xf0);
LCD_data(div1);
_delay_ms(1);
}

void dis_cmd(unsigned char x)
{
char div1;
div1=x&0xf0;
LCD_cmd(div1);
div1=((x<<4)&0xf0);
LCD_cmd(div1);
_delay_ms(1);
}

void LCD_int()
{
dis_cmd(0x02);
dis_cmd(0x28);
dis_cmd(0x06);
dis_cmd(0x0C);
}
