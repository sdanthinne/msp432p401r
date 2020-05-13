/*
 * fgen.h
 *
 *  Created on: May 11, 2020
 *      Author: sdanthinne
 */

#ifndef FGEN_H_
#define FGEN_H_
#define MIN_OUT_RATE 7 //the minimum amount of time it takes to write out to the DAC. (in us)
#define SAMPLE_TIME (MIN_OUT_RATE + TIME_MARGIN)
#define TIME_MARGIN 1 //TODO: some margin of time to allow for errors. (in us)
#define DCO_SPEED 24
void main_fg(void);
#endif /* FGEN_H_ */
