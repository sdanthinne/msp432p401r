/*
 * bluetooth.h
 *
 *  Created on: May 30, 2020
 *      Author: crapp
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "msp.h"

#define UCBRF__2 (2 << 4)
#define UCBRF__10 (10 << 4)
#define UCBRF__12 (12 << 4)
#define UCBRF__MAX (15 << 4)

#define RETURN 0x0A // return character

void setup_bt_uart();
void setup_bluetooth();
void write_bt_command(char * str);
void write_string_bt(char *str);

void read_bt_string();

volatile union data_in
{
    volatile uint32_t _int;
    volatile uint8_t _byte[4];
}data_in;

volatile uint16_t data_counter;

volatile char bt_data[100];
volatile char bt_byte;
volatile char bt_str_rec;
volatile char bt_byte_rec;

#endif /* BLUETOOTH_H_ */
