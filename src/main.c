#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"

#define BLINK_DELAY_MS 100
#define LED PORTA2 // Arduino Mega digital pin 24
#define UART_BAUD 9600
#define UART_STATUS_MASK 0x00FF

microrl_t rl;
microrl_t *prl = &rl;


static inline void start_cli(void)
{
    uart0_puts_p(CONSOLE_BACKSPACE);
    uart1_puts_p(PSTR("\r\n"));
    microrl_init(prl, uart0_puts);
    microrl_set_execute_callback(prl, cli_execute);
}

static inline void init_sys_timer(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12);
    TCCR1B |= _BV(CS12);
    OCR1A = 62549;
    TIMSK1 |= _BV(OCIE1A);
}

static inline void init_uart1(void)
{
    uart1_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart1_puts_p(ERR_CONSOLE_STARTUP);
    uart1_puts_p(PSTR("\r\n"));
    uart1_puts_p(PROG_VER);
}

static inline void init_uart0(void)
{
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart0_puts_p(CONSOLE_STARTUP);
    uart0_puts_p(PSTR("\r\n"));
    uart0_puts_p(STUDENT_NAME);
    uart0_puts_p(PSTR("\r\n"));
}

void init_leds()
{
    DDRA |= _BV(DDB2);
}


static inline void heartbeat(void)
{
    static time_t prev_time;
    char ascii_buf[11] = {0x00};
    time_t now = time(NULL);

    if (prev_time != now) {
        ltoa(now, ascii_buf, 10);
        uart1_puts_p(PSTR("Uptime: "));
        uart1_puts(ascii_buf);
        uart1_puts_p(PSTR(" seconds\r"));
        PORTA ^= _BV(LED);
        prev_time = now;
    }
}

void main(void)
{
    init_uart0();
    init_uart1();
    init_leds();
    lcd_init();
    lcd_clrscr();
    lcd_puts_P(STUDENT_NAME);
    init_sys_timer();
    start_cli();
    sei();

    while (1) {
        heartbeat();
        microrl_insert_char (prl, uart0_getc() & UART_STATUS_MASK);
    }
}

ISR(TIMER1_COMPA_vect)
{
    system_tick();
}