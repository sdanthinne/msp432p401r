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
    uint8_t _byte[4];
    float _float;
    uint32_t _int;
} u;

void setup_arduino();

void send_number(float number);


#endif /* ARDUINO_H_ */
