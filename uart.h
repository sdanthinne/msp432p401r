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

void setup_uart();

#endif /* UART_H_ */
