#define BLINK_DELAY_MS 500
#define __ASSERT_USE_STDERR

#include <stdio.h>
#include <avr/io.h>
#include <assert.h>
#include <util/delay.h>
#include "../lib/hd44780_111/hd44780.h"
#include "uart.h"
#include "hmi_msg.h"
#include "print_helper.h"

void blink(unsigned int port)
{
    PORTA |= port;
    _delay_ms(BLINK_DELAY_MS);
    PORTA &= ~port;
    _delay_ms(BLINK_DELAY_MS);
}


static inline void init_connection(void)
{
    simple_uart0_init();
    stdin = stdout = &simple_uart0_io;
    fprintf_P(stdout, PROG_VER, FW_VERSION, __DATE__, __TIME__);
    fprintf_P(stdout, LIBC_VER, __AVR_LIBC_VERSION_STRING__, __VERSION__);
    fprintf(stdout, "\n");
    fprintf_P(stdout, STUDENT_NAME);
    fprintf(stdout, "\n");
}

void init_leds()
{
    DDRB |= _BV(DDB7);
    DDRA |= (_BV(DDA0)) | (_BV(DDA2)) | (_BV(DDA4));
    PORTB &= ~_BV(PORTB7);
}


void main(void)
{
    init_connection();
    init_leds();
    lcd_init();
    lcd_home();
    lcd_puts_P(STUDENT_NAME);
    unsigned char a[128];

    for (unsigned char i = 0; i < sizeof(a); i++) {
        a[i] = i;
    }

    fprintf(stdout, "\n");
    print_ascii_tbl(stdout);
    fprintf(stdout, "\n");
    print_for_human(stdout, a, sizeof(a) - 1);

    while (1) {
        char input[20];
        fprintf_P(stdout, ENTER_NUMBER);
        scanf("%s", input);
        fprintf(stdout, input);
        int number = atoi(input);

        if (number >= 0 && number < 10) {
            fprintf_P(stdout, ENTERED_NUMBER);
            fprintf_P(stdout, NUMBERS[number]);
        } else {
            fprintf_P(stdout, WRONG_NUMBER);
        }

        blink(_BV(PORTA0));
        blink(_BV(PORTA2));
        blink(_BV(PORTA4));
    }
}
