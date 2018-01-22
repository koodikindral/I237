#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "hmi_msg.h"
#include "cli_microrl.h"
#include "print_helper.h"
#include "rfid.h"
#include "../lib/andy_brown_memdebug/memdebug.h"
#include "../lib/matejx_avr_lib/mfrc522.h"

#define NUM_ELEMS(x) (sizeof(x) / sizeof((x)[0]))

void cli_print_help(const char *const *argv);
void cli_example(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_number(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
void cli_print_free(const char *const *argv);
void cli_print_list(const char *const *argv);
void cli_add(const char *const *argv);
void cli_remove(const char *const *argv);
void cli_print_read(const char *const *argv);

typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;

const char help_cmd[] PROGMEM = "help";
const char help_help[] PROGMEM = "Get help";
const char example_cmd[] PROGMEM = "example";
const char example_help[] PROGMEM =
    "Prints out all provided 3 arguments Usage: example <argument> <argument> <argument>";
const char ver_cmd[] PROGMEM = "version";
const char ver_help[] PROGMEM = "Print firmware version";
const char ascii_cmd[] PROGMEM = "ascii";
const char ascii_help[] PROGMEM = "Print ASCII tables";
const char number_cmd[] PROGMEM = "number";
const char number_help[] PROGMEM =
    "Print and display matching number Usage: number <decimal number>";
const char free_cmd[] PROGMEM = "free";
const char free_help[] PROGMEM = "Print the amount of free memory.";
const char list_cmd[] PROGMEM = "list";
const char list_help[] PROGMEM = "Print all of the added cards";
const char add_cmd[] PROGMEM = "add";
const char add_help[] PROGMEM = "Add a card to the list";
const char remove_cmd[] PROGMEM = "remove";
const char remove_help[] PROGMEM = "Remove a card from the list";
const char read_cmd[] PROGMEM = "read";
const char read_help[] PROGMEM = "Print info about the current card.";

const cli_cmd_t cli_cmds[] = {
    {free_cmd, free_help, cli_print_free, 0},
    {list_cmd, list_help, cli_print_list, 0},
    {add_cmd, add_help, cli_add, 2},
    {remove_cmd, remove_help, cli_remove, 1},
    {read_cmd, read_help, cli_print_read, 0},
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {example_cmd, example_help, cli_example, 3},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {number_cmd, number_help, cli_handle_number, 1},
};

void cli_print_free(const char *const *argv)
{
    (void) argv;
    char print_buf[256] = {0x00};
    extern int __heap_start, *__brkval;
    int v;
    int space;
    static int prev_space;
    space = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    uart0_puts_p(PSTR("Heap statistics\r\n"));
    sprintf_P(print_buf, PSTR("Used: %u\r\nFree: %u\r\n"), getMemoryUsed(),
              getFreeMemory());
    uart0_puts(print_buf);
    uart0_puts_p(PSTR("\r\nSpace between stack and heap:\r\n"));
    sprintf_P(print_buf, PSTR("Current  %d\r\nPrevious %d\r\nChange   %d\r\n"),
              space, prev_space, space - prev_space);
    uart0_puts(print_buf);
    uart0_puts_p(PSTR("\r\nFreelist\r\n"));
    sprintf_P(print_buf, PSTR("Freelist size:             %u\r\n"),
              getFreeListSize());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Blocks in freelist:        %u\r\n"),
              getNumberOfBlocksInFreeList());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest block in freelist: %u\r\n"),
              getLargestBlockInFreeList());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest freelist block:    %u\r\n"),
              getLargestAvailableMemoryBlock());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest allocable block:   %u\r\n"),
              getLargestNonFreeListBlock());
    uart0_puts(print_buf);
    prev_space = space;
}

void cli_print_list(const char *const *argv)
{
    (void )argv;
    rfid_print_list();
}

void cli_add(const char *const *argv)
{
    rfid_add(argv);
}

void cli_remove(const char *const *argv)
{
    rfid_remove(argv);
}

void cli_print_read(const char *const *argv)
{
    (void) argv;
    rfid_read();
}

void cli_print_help(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PSTR("Following commands are implemented:\r\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        uart0_puts_p(cli_cmds[i].cmd);
        uart0_puts_p(PSTR(" : "));
        uart0_puts_p(cli_cmds[i].help);
        uart0_puts_p(PSTR("\r\n"));
    }
}

void cli_example(const char *const *argv)
{
    uart0_puts_p(PSTR("This command had following arguments:\r\n"));

    for (uint8_t i = 1; i < 4; i++) {
        uart0_puts(argv[i]);
        uart0_puts_p(PSTR("\r\n"));
    }
}

void cli_print_ver(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PROG_VER);
    uart0_puts_p(PSTR("\r\n"));
    uart0_puts_p(LIBC_VER);
    uart0_puts_p(PSTR("\r\n"));
}


void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    print_ascii_tbl();
    uart0_puts("\r\n");
    unsigned char array[128] = {0};

    for (unsigned char i = 0; i < sizeof(array); i++) {
        array[i] = i;
    }

    print_for_human(array, sizeof(array));
}

void print_student_name()
{
    lcd_clrscr();
    lcd_puts_P(STUDENT_NAME);
    lcd_goto(0x40);
}

void cli_handle_number(const char *const *argv)
{
    for (size_t i = 0; i < strlen(argv[1]); i++) {
        if (!isdigit(argv[1][i])) {
            uart0_puts_p(PSTR("Argument is not a decimal number!\r\n"));
            print_student_name();
            return;
        }
    }

    int in_int = atoi(argv[1]);

    if (in_int > 9 || in_int < 0) {
        uart0_puts_p(PSTR("Please enter a number between 0 and 9\r\n"));
    } else {
        uart0_puts_p((PGM_P)pgm_read_word(&NUMBERS[in_int]));
        uart0_puts_p(PSTR("\r\n"));
        print_student_name();
        lcd_puts_P((PGM_P)pgm_read_word(&NUMBERS[in_int]));
    }
}


void cli_print_cmd_error(void)
{
    uart0_puts_p(PSTR("Command not implemented.\r\n\tUse <help> to get help.\r\n"));
}


void cli_print_cmd_arg_error(void)
{
    uart0_puts_p(
        PSTR("To few or too many arguments for this command\r\n\tUse <help>\r\n"));
}


int cli_execute(int argc, const char *const *argv)
{
    uart0_puts_p(PSTR("\r\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}