#include <avr/io.h>
#include <util/delay.h>
#include "../lib/hd44780_111/hd44780.h"

#define BLINK_DELAY_MS 500

void blink (unsigned int port)
{
    PORTA |= port;
    _delay_ms(BLINK_DELAY_MS);
    PORTA &= ~port;
    _delay_ms(BLINK_DELAY_MS);
}

void main (void)
{
    DDRB |= _BV(DDB7);
    DDRA |= (_BV(DDA0)) | (_BV(DDA2)) | (_BV(DDA4));
    PORTB &= ~_BV(PORTB7);

    lcd_init();
    lcd_home();


    while (1) {
        lcd_clrscr();
        lcd_puts("Roheline");
        blink(_BV(PORTA0));

        lcd_clrscr();
        lcd_puts("Punane");
        blink(_BV(PORTA2));

        lcd_clrscr();
        lcd_puts("Sinine");
        blink(_BV(PORTA4));
    }
}
