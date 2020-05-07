/*
 * DCO.h
 *
 *  Created on: Apr 9, 2020
 *      Author: sdanthinne
 */

#ifndef DCO_H_
#define DCO_H_

#define FREQ_1_5_MHz CS_CTL0_DCORSEL_0 //1.5 MHz constant
#define FREQ_3_MHz CS_CTL0_DCORSEL_1
#define FREQ_6_MHz CS_CTL0_DCORSEL_2
#define FREQ_12_MHz CS_CTL0_DCORSEL_3
#define FREQ_24_MHz CS_CTL0_DCORSEL_4
#define FREQ_48_MHz CS_CTL0_DCORSEL_5
void setDCO(uint32_t freq);

#endif /* DCO_H_ */
