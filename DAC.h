/*
 * DAC.h
 *
 *  Created on: May 7, 2020
 *      Author: sdanthinne
 */

#ifndef DAC_H_
#define DAC_H_
#include "spi.h"
#include "DCO.h"
#include "timer.h"

void setup_DAC();
void write_DAC(uint16_t bytes);
void write_SINE();
void write_TRIANGLE();
void set_timer_square();




#endif /* DAC_H_ */
