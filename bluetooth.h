/*
 * uart.h
 *
 *  Created on: May 30, 2020
 *      Author: crapp
 */

#ifndef UART_H_
#define UART_H_

#define UCBRF__2 (2 << 4)
#define UCBRF__10 (10 << 4)
#define UCBRF__12 (12 << 4)
#define UCBRF__MAX (15 << 4)

#define RETURN 0x0A // return character

void setup_bt_uart();
void setup_bluetooth();
void write_string_uart(char *str);
void write_bt_command(char * str);


#endif /* UART_H_ */
