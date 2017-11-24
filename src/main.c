#define BLINK_DELAY_MS 500
#define __ASSERT_USE_STDERR

#include <stdio.h>
#include <avr/io.h>
#include <assert.h>
#include <util/delay.h>
#include "../lib/hd44780_111/hd44780.h"
#include "uart.h"

void blink (unsigned int port)
{
    PORTA |= port;
    _delay_ms(BLINK_DELAY_MS);
    PORTA &= ~port;
    _delay_ms(BLINK_DELAY_MS);
}

static inline void init_errcon(void)
{
    simple_uart1_init();
    stderr = &simple_uart1_out;
    fprintf(stderr, "Version: %s built on: %s %s\n",
            FW_VERSION, __DATE__, __TIME__);
    fprintf(stderr, "avr-libc version: %s avr-gcc version: %s\n",
            __AVR_LIBC_VERSION_STRING__, __VERSION__);
}

void main (void)
{
    DDRB |= _BV(DDB7);
    DDRA |= (_BV(DDA0)) | (_BV(DDA2)) | (_BV(DDA4));
    PORTB &= ~_BV(PORTB7);
    lcd_init();
    lcd_home();
    init_errcon();

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
