/*
 * DAC.h
 *
 *  Created on: May 7, 2020
 *      Author: sdanthinne
 */

#ifndef DAC_H_
#define DAC_H_
#include "spi.h"

void setup_DAC();
void write_DAC(uint16_t bytes);

#endif /* DAC_H_ */
