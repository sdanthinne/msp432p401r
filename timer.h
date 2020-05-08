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
#define FREQ_1_5_MHZ CS_CTL0_DCORSEL_0

#define MAX_NUM_SAMPLES_1 46
#define MAX_NUM_SAMPLES 47
#define HIGH_VOLTAGE 930
#define LOW_VOLTAGE 310
#define TWENTY_MS 3750
#define TEN_MS 1875
#define INCREMENT 13
#define SAMPLE_INTERVAL 40

void set_DCO(uint32_t selected_freq);

#endif /* TIMER_H_ */
