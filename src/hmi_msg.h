#include <avr/pgmspace.h>
#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

const char PROG_VER[] PROGMEM = "\nVersion: %s built on: %s %s";
const char LIBC_VER[] PROGMEM = "\navr-libc version: %s avr-gcc version: %s";
const char STUDENT_NAME[] PROGMEM = "Gert Vesterberg";

const char ENTER_NUMBER[] PROGMEM = "\nPlease enter a number>";
const char WRONG_NUMBER[] PROGMEM = "\nPlease enter a number between 0 and 9";
const char ENTERED_NUMBER[] PROGMEM = "\nYou have entered: ";
const char NUMBERS[10][6] PROGMEM = {"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};

#endif