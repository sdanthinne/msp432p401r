/*
 * uart.h
 *
 *  Created on: May 18, 2020
 *      Author: aknapen, crapp, sdanthinne
 */

#ifndef UART_H_
#define UART_H_

#include "uart.h"

#define UCBRF__2 (2 << 4)
#define UCBRF__10 (10 << 4)
#define UCBRF__12 (12 << 4)
#define UCBRF__MAX (15 << 4)

#define RETURN 0x0A // return character

uint8_t is_ready; // Indicates whether or not to write to DAC
uint32_t dac_in; // input value to DAC

void setup_uart();
void write_string_uart(char *str);
void read_string_uart();

char uart_byte;
uint8_t uart_str_rec;

#endif /* UART_H_ */
