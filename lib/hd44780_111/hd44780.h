/*****************************************************************************
Title  :   HD44780 Library
Author :   SA Development
Version:   1.11
Modifications for: Arduino Mega 2560
                    Itead Studio Arduino 1602 LED Keypad Shield
Modified by: Silver Kits <silver.kits@eesti.ee> October 2016
*****************************************************************************/
#ifndef HD44780_H
#define HD44780_H


//LCD Commands for HD44780
#define LCD_CLR                 0    // DB0: clear display

#define LCD_HOME                1    // DB1: return to home position

#define LCD_ENTRY_MODE          2    // DB2: set entry mode
#define LCD_ENTRY_INC           1    // DB1: 1=increment, 0=decrement
#define LCD_ENTRY_SHIFT         0    // DB0: 1=display shift on

#define LCD_DISPLAYMODE         3    // DB3: turn lcd/cursor on
#define LCD_DISPLAYMODE_ON      2    // DB2: turn display on
#define LCD_DISPLAYMODE_CURSOR  1    // DB1: turn cursor on
#define LCD_DISPLAYMODE_BLINK   0    // DB0: blinking cursor

#define LCD_MOVE                4    // DB4: move cursor/display
#define LCD_MOVE_DISP           3    // DB3: move display (0-> cursor)
#define LCD_MOVE_RIGHT          2    // DB2: move right (0-> left)

#define LCD_FUNCTION            5    // DB5: function set
#define LCD_FUNCTION_8BIT       4    // DB4: set 8BIT mode (0->4BIT mode)
#define LCD_FUNCTION_2LINES     3    // DB3: two lines (0->one line)
#define LCD_FUNCTION_10DOTS     2    // DB2: 5x10 font (0->5x7 font)

#define LCD_CGRAM               6    // DB6: set CG RAM address
#define LCD_DDRAM               7    // DB7: set DD RAM address

#define LCD_BUSY                7    // DB7: LCD is busy

// LCD columns and rows definitions
#define LCD_ROW_1_START             0
#define LCD_ROW_2_START             64
#define LCD_ROW_1_LAST_VISIBLE_COL  15
#define LCD_ROW_1_LAST_COL          39
#define LCD_ROW_2_LAST_VISIBLE_COL  79
#define LCD_ROW_2_LAST_COL          103
#define LCD_COLS_MAX                103
#define LCD_VISIBLE_COLS            16

// Maximum character what can be displayed with 1 byte
#define LCD_MAX_CARACTER            255


void lcd_init();
void lcd_command(uint8_t cmd);

void lcd_clrscr();
void lcd_clr(uint8_t pos, uint8_t len);
void lcd_home();
void lcd_goto(uint8_t pos);

#if RW_LINE_IMPLEMENTED==1
uint8_t lcd_getc();
#endif

void lcd_putc(unsigned char c);
void lcd_puts(const char *s);
void lcd_puts_P(const char *progmem_s);
#if (LCD_DISPLAYS>1)
void lcd_use_display(int ADisplay);
#endif

#endif

