/*
 * lcd.h
 *
 *  Created on: Apr 14, 2020
 *      Author: sdanthinne,aknapen,crapp
 */

#ifndef LCD_H_
#define LCD_H_
#include "msp.h"
#include "delay.h"

void setup_lcd();

void setup_lcd_4();

void clear_display();

void clear_display_4();

void wakeup_lcd_4();

void write_char(uint8_t c);

void write_char_4(uint8_t c);

void write_string(uint8_t * c);

void write_string_4(uint8_t * c);

void setup_lcd_pins();

void setup_lcd_pins_4();

void home_lcd();

void home_lcd_4();

void set_address_counter_4(uint8_t row, uint8_t col);

#endif /* LCD_H_ */
