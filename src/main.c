#include <avr/io.h>
#include <util/delay.h>

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

    while (1) {
        blink(_BV(PORTA0));
        blink(_BV(PORTA2));
        blink(_BV(PORTA4));
    }
}
