#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"
#include "rfid.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"

#define BLINK_DELAY_MS 100
#define LED PORTA2 // Arduino Mega digital pin 24
#define LED_DOOR PORTA4
#define UART_BAUD 9600
#define UART_STATUS_MASK 0x00FF


time_t change_time = -2;
time_t last_time;
char lastuid[20];
char uid_string[20];
bool check_status = false;

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
    DDRA |= _BV(DDB4);
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

static void status_message(char *status)
{
    lcd_clrscr();
    lcd_puts_P(STUDENT_NAME);
    lcd_goto(0x40);
    lcd_puts(status);
}

void status_callback(int status, char *user)
{
    switch (status) {
    case 0 :
        status_message("Door is closed");
        PORTA &= ~_BV(LED_DOOR);
        check_status = true;
        break;

    case 1:
        status_message("Opened: ");
        lcd_puts(user);
        check_status = false;
        PORTA |= _BV(LED_DOOR);
        break;

    default:
        status_message("Access Denied");
        PORTA &= ~_BV(LED_DOOR);
        check_status = false;
        break;
    }
}


void check_state()
{
    Uid uid;
    Uid *uid_ptr = &uid;

    if (PICC_IsNewCardPresent()) {
        strcpy(uid_string, "");
        PICC_ReadCardSerial(uid_ptr);

        for (byte i = 0; i < uid.size; i++) {
            char suid[20];
            itoa(uid.uidByte[i], suid, 10);
            strcat(uid_string, suid);
        }

        last_time = time(NULL);
    }

    if (time(NULL) - last_time > 1) {
        strcpy(lastuid, "");
        strcpy(uid_string, "");
    }

    if (strcmp(lastuid, uid_string) != 0) {
        char * user = find(uid_string);

        if (user == NULL) {
            status_callback(2, NULL);
        } else {
            status_callback(1, user);
        }

        change_time = time(NULL);
        strcpy(lastuid, uid_string);
    }

    if (time(NULL) - change_time >= 2) {
        if (check_status == false) {
            status_callback(0, NULL);
        }
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
    MFRC522_init();
    PCD_Init();
    sei();

    while (1) {
        heartbeat();
        microrl_insert_char (prl, uart0_getc() & UART_STATUS_MASK);
        check_state();
    }
}

ISR(TIMER1_COMPA_vect)
{
    system_tick();
}