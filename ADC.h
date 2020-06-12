/*
 * ADC.h
 *
 *  Created on: Jun 5, 2020
 *      Author: aknapen, crapp, sdanthinne
 */

#ifndef ADC_H_
#define ADC_H_

#include "msp.h"

void setup_ADC();
int read_ADC();

uint16_t adc_flag;

#endif /* ADC_H_ */
