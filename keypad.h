/*
 * keypad.h
 *
 *  Created on: Apr 19, 2020
 *      Author: Aknapen,sdanthinne
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "lcd.h"

void setup_keypad();
uint8_t has_press();
uint8_t get_key_pressed();
void write_key_to_LCD(uint8_t key);
uint8_t get_number_pressed(uint8_t key);
uint8_t update_key_press(uint8_t prev_key);

/*
 * Assignments of keys to corresponding
 * hex values for the font table of the
 * LCD.
 */

#define KEY_ZERO       0x14
#define KEY_ONE        0x82
#define KEY_TWO        0x84
#define KEY_THREE      0x81
#define KEY_FOUR       0x42
#define KEY_FIVE       0x44
#define KEY_SIX        0x41
#define KEY_SEVEN      0x22
#define KEY_EIGHT      0x24
#define KEY_NINE       0x21
#define KEY_STAR       0x12
#define KEY_POUND      0x11
/**
 * values of the symbols on LCD ROM
 */
#define ZERO           0x30
#define ONE            0x31
#define TWO            0x32
#define THREE          0x33
#define FOUR           0x34
#define FIVE           0x35
#define SIX            0x36
#define SEVEN          0x37
#define EIGHT          0x38
#define NINE           0x39
#define STAR           0x2A
#define POUND          0x23


#endif /* KEYPAD_H_ */
