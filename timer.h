/*
 * timer.h
 *
 *  Created on: Apr 20, 2020
 *      Author: Alexander
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "msp.h"

#define FREQ_24_MHZ CS_CTL0_DCORSEL_4

void set_DCO(uint32_t selected_freq);
void set_timer();

#endif /* TIMER_H_ */
