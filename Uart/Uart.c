#include "lcd.h"
#include "uart.h"

int main (void);

int main ()
{
    unsigned char x;
    usart_int ();
    DDRC = 0b0001111;
    DDRB = 0b11111111;
    LCD_int ();
    while (1)
    {
        x = usart_receive();
        usart_send (x);
        
        dis_cmd (0x01);
        _delay_ms (1);
        dis_cmd (0x8F);
        LCD_no (x);
        _delay_ms (10);
    }
}
