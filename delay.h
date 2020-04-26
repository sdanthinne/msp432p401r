/*
 * delay.h
 *
 *  Created on: Apr 10, 2020
 *      Author: sdanthinne
 */

#ifndef DELAY_H_
#define DELAY_H_
#include "msp.h"
#define DIV_AMT 12

#define OVERHEAD1 34
#define OVERHEAD3 20
#define OVERHEAD6 12
#define OVERHEAD12 6
#define OVERHEAD24 3
#define OVERHEAD48 0
void delay_us(uint32_t delay);


#endif /* DELAY_H_ */
