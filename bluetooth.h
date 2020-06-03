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

char bt_data[100];
char bt_byte;
char bt_str_rec;
char bt_byte_rec;

#endif /* BLUETOOTH_H_ */
