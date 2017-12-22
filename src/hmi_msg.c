#include <avr/pgmspace.h>

const char PROG_VER[] PROGMEM =
    "Version: "FW_VERSION" built on: "__DATE__" "__TIME__"\n";
const char LIBC_VER[] PROGMEM =
    "avr-libc version: "__AVR_LIBC_VERSION_STRING__" avr-gcc version: "__VERSION__"\n";

const char CONSOLE_STARTUP[] PROGMEM = "Command console started";
const char ERR_CONSOLE_STARTUP[] PROGMEM = "Error console started";
const char CONSOLE_BACKSPACE[] PROGMEM =
    "Use backspace to delete entry and enter to confirm.";
const char OUT_OF_RANGE_NUM_MESSAGE[] PROGMEM = "Enter number between 0 and 9!";
const char ENTERED_NUM_MESSAGE[] PROGMEM = "You entered number ";
const char STUDENT_NAME[] PROGMEM = "Gert Vesterberg";

static const char no0[] PROGMEM = "zero";
static const char no1[] PROGMEM = "one";
static const char no2[] PROGMEM = "two";
static const char no3[] PROGMEM = "three";
static const char no4[] PROGMEM = "four";
static const char no5[] PROGMEM = "five";
static const char no6[] PROGMEM = "six";
static const char no7[] PROGMEM = "seven";
static const char no8[] PROGMEM = "eight";
static const char no9[] PROGMEM = "nine";

PGM_P const NUMBERS[] PROGMEM = {
    no0,
    no1,
    no2,
    no3,
    no4,
    no5,
    no6,
    no7,
    no8,
    no9
};