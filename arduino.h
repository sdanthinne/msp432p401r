/*
 * arduino.h
 *
 *  Created on: Jun 2, 2020
 *      Author: aknapen, crapp, sdanthinne
 *      communicates with a ATMega 2560 over SPI
 */

#ifndef ARDUINO_H_
#define ARDUINO_H_

#define FLOAT_SIZE 4


void setup_arduino();

void send_number(int number);


#endif /* ARDUINO_H_ */
