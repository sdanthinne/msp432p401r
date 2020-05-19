/*
 * spi.h
 *
 *  Created on: May 6, 2020
 *      Author: sdanthinne
 */

#ifndef SPI_H_
#define SPI_H_
#include "msp.h"
void setup_SPI_b0();
void write_byte_b0(uint8_t byte);
void write_byte_b0_cont(uint8_t byte);
void write_DAC(uint16_t bytes);

#endif /* SPI_H_ */
