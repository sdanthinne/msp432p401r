/*
 * uart.h
 *
 *  Created on: May 18, 2020
 *      Author: aknapen, crapp, sdanthinne
 */

#ifndef UART_H_
#define UART_H_

#define UCBRF__2 (2 << 4)
#define UCBRF__10 (10 << 4)
#define UCBRF__12 (12 << 4)
#define UCBRF__MAX (15 << 4)

#define NL 0x0A // return character
#define CR 0x0D // carriage-return character

uint8_t is_ready; // Indicates whether or not to write to DAC
uint32_t dac_in; // input value to DAC

void setup_uart();

#endif /* UART_H_ */
