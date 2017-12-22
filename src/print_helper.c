#include <stdio.h>
#include "print_helper.h"
#include <avr/pgmspace.h>
#include "../lib/andygock_avr-uart/uart.h"

void print_ascii_tbl(void)
{
    for (char c = ' '; c <= '~'; c++)    {
        uart0_putc(c);
    }

    uart0_puts_p(PSTR("\r\n"));
}


void print_for_human(const unsigned char *array,
                     const size_t len)
{
    for (size_t i = 0; i < len; i++)    {
        if (array[i] >= ' ' && array[i] <= '~')    {
            uart0_putc(array[i]);
        }    else {
            uart0_puts_p(PSTR("\"0x"));
            uart0_putc((array[i] >> 4) + ((array[i] >> 4) <= 9 ?
                                          0x30 : 0x37));
            uart0_putc((array[i] & 0x0F) + ((array[i] & 0x0F) <= 9
                                            ? 0x30 : 0x37));
            uart0_putc('"');
        }
    }

    uart0_puts_p(PSTR("\r\n"));
}

