/*
 * arduino.h
 *
 *  Created on: Jun 2, 2020
 *      Author: sdanthinne
 *      communicates with a ATMega 2560 over SPI
 */

#ifndef ARDUINO_H_
#define ARDUINO_H_

#define FLOAT_SIZE 4
union float_split{
    uint8_t split_val[4];
    float f_val;
} u;

void init_arduino();

void send_number(float number);


#endif /* ARDUINO_H_ */
