#include <avr/io.h>
#include <stdio.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

/* http://www.cs.mun.ca/~rod/Winter2007/4723/notes/serial/serial.html */

void simple_uart0_init(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}
void simple_uart1_init(void)
{
    UBRR1H = UBRRH_VALUE;
    UBRR1L = UBRRL_VALUE;
#if USE_2X
    UCSR1A |= _BV(U2X1);
#else
    UCSR1A &= ~(_BV(U2X1));
#endif
    UCSR1C = _BV(UCSZ11) | _BV(UCSZ10); /* 8-bit data */
    UCSR1B = _BV(TXEN1); /* Enable only TX */
}

int simple_uart0_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        simple_uart0_putchar('\r', stream);
    }

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}
int simple_uart1_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        simple_uart1_putchar('\r', stream);
    }

    loop_until_bit_is_set(UCSR1A, UDRE1);
    UDR1 = c;
    return 0;
}

int  simple_uart0_getchar(FILE *stream)
{
    (void) stream;
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}
