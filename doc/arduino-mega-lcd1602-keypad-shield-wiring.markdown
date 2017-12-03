# Arduino Mega Arduino LCD1602 Keypad shield wiring

## Introduction

This shield consists of three logical parts:

- 1602LCD with HD4780 Dot Matrix Liquid Crystal Display Controller/Driver.
- 6 button keypad.
- Analog pins, 5V and GND pass through.

<div class=pagebreak></div>

## Connection to Arduino Mega

Following illustration shows how shield is connected to Arduino mega board.

![arduino-mega-lcd1602-keypad-shield-placement.png](arduino-mega-lcd1602-keypad-shield-placement.png)

Image source: [sainsmart.com](http://www.sainsmart.com/media/catalog/product/2/_/2_16_6.jpg)

Additional installation details and tutorials can be found with help of keywords [Arduino 1602 lcd keypad shield tutorial](https://www.google.ee/webhp?q=Arduino+1602+lcd+keypad+shield+tutorial).

<div class=pagebreak></div>

## Wiring illustration

![arduino-mega-lcd1602-keypad-shield-wiring.png](arduino-mega-lcd1602-keypad-shield-wiring.png)

Author: [Lauri Võsandi](http://lauri.võsandi.com/arduino/lcd1602-key-shield.html#hd44780)

<div class=pagebreak></div>

## Wiring table

| Signal | ATMega2560 port and pin | Arduino Mega 2560 pin | LCD 1602 Keypad shield | 1602 LCD pin | HD44780 pin |
| --- | --- | --- | --- | --- | --- |
| **LCD** | | | | |
| Data bus DB4 | PORTG pin 5 | Digital pin 4 | 4 | 11 | DB4 |
| Data bus DB5 | PORTE pin 3 | Digital pin 5 | 5 | 12 | DB5 |
| Data bus DB6 | PORTH pin 3 | Digital pin 6 | 6 | 13 | DB6 |
| Data bus DB7 | PORTH pin 4 | Digital pin 7 | 7 | 14 | DB7 |
| Select register RS | PORTH pin 5 | Digital pin 8 | 8 | 4 | RS |
| Start read/write E | PORTH pin 6 | Digital pin 9 | 9 | 6 | E |
| Backlight control | PORTB pin 4 | Digital pin 10 | 10 | 16 | - |
| Backlight 5V (via variable resistor ) | - |  - | - | 3 | - |
| **Keypad** | | | | |
| Buttons (select, up, right, down and left) | | Analog pin 0 | 0 | - |
| Reset button | - | RESET | RESET | - |
| **Pass through** | | | | |
| Analog A1 .. A5 | | Analog pin 1 .. 5 | Analog pin 1 .. 5 | - |
| **Common** | | | | |
| 5V | - | 5V | VCC | 2 and 15 | Vcc |
| Ground (GND) | GND | GND | pin 1 | GND | GND |
