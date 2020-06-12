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

#define NL 0x0A // return character
#define CR 0x0D // carriage-return character

void setup_uart();
void write_string_uart(char *str);
void read_string_uart();
void write_UART_16bitnum(uint16_t value);
void write_UART_32bitnum(uint32_t value);

char uart_byte;
uint8_t uart_ready;
uint8_t uart_str_rec;

#endif /* UART_H_ */
