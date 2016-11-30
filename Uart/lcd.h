#ifndef LCD_H
#define LCD_H

#include<avr/io.h>
#include<util/delay.h>

#ifndef __PORT
#define __PORT PORTB
#endif

#define RS 0
#define RW 1
#define EN 2

void LCD_data (unsigned char data)
{
    __PORT = data;
    __PORT |= (1 << RS);//data register
    __PORT &= ~(1 << RW);//write
    __PORT |= (1 << EN);//enable
    _delay_ms (1);
    __PORT &= ~(1 << EN);//not enable
}

void LCD_cmd (unsigned char cmd)
{
    __PORT = cmd;
    __PORT &= ~(1 << RS);
    __PORT &= ~(1 << RW);
    __PORT |= (1 << EN);
    _delay_ms (1);
    __PORT &= ~(1 << EN);
}

void dis_data (unsigned char data)
{
    char div;
    div = (data & 0xF0);
    LCD_data (div);
    div = ((data << 4) & 0xF0);
    LCD_data (div);
    _delay_ms (1);
}

void dis_cmd (unsigned char cmd)
{
    char div;
    div = (cmd & 0xF0);
    LCD_cmd (div);
    div = ((cmd << 4) & 0xF0);
    LCD_cmd (div);
    _delay_ms (1);
}

void LCD_str (unsigned char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        dis_data (str[i]);
        i++;
    }
    _delay_ms (1);
}

void LCD_no (unsigned int value)
{ 
    dis_cmd (0x04);
    if (value == 0)
    {
        dis_data (48);
    }
    unsigned int a = 0;
    while (value != 0)
    {
        a = (value % 10);
        dis_data (a + 48);
        value = (value / 10);
    }
    dis_cmd (0x06);
}

void LCD_int (void)
{
    dis_cmd (0x02);
    dis_cmd (0x28);
    dis_cmd (0x06);
    dis_cmd (0x0C);
}

#endif
