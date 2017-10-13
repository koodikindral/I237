#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000

void main (void)
{
    DDRB |= (_BV(DDB5)) | (_BV(DDB4)) | (_BV(DDB6));

    while (1) {
        PORTB |= _BV(PORTB5);
        _delay_ms(BLINK_DELAY_MS);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(BLINK_DELAY_MS);

        PORTB |= _BV(PORTB4);
        _delay_ms(BLINK_DELAY_MS);
        PORTB &= ~_BV(PORTB4);
        _delay_ms(BLINK_DELAY_MS);
    }
}