/*****************************************************************************
Title  :   HD44780 Library
Author :   SA Development
Version:   1.11
Modifications for: Arduino Mega 2560
                    Itead Studio Arduino 1602 LED Keypad Shield
Modified by: Silver Kits <silver.kits@eesti.ee> October 2016
*****************************************************************************/
#ifndef HD44780_SETTINGS_H
#define HD44780_SETTINGS_H

#define USE_ADELAY_LIBRARY       0
#define LCD_BITS                 4
#define RW_LINE_IMPLEMENTED      0
#define WAIT_MODE                0
#define DELAY_RESET              15

// Pin and port definitions for Arduino Mega 2560
#define LCD_DB4_PORT             PORTG
#define LCD_DB4_PIN              5
#define LCD_DB5_PORT             PORTE
#define LCD_DB5_PIN              3
#define LCD_DB6_PORT             PORTH
#define LCD_DB6_PIN              3
#define LCD_DB7_PORT             PORTH
#define LCD_DB7_PIN              4

#define LCD_RS_PORT              PORTH
#define LCD_RS_PIN               5

#define LCD_DISPLAYS             1

#define LCD_DISPLAY_LINES        2

#define LCD_E_PORT               PORTH
#define LCD_E_PIN                6

#endif /* HD44780_SETTINGS_H */
