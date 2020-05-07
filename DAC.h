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

static const uint16_t wave[512] =
     {
    512,518,524,530,537,543,549,555,
    562,568,574,580,587,593,599,605,
    611,617,624,630,636,642,648,654,
    660,666,672,678,684,690,696,701,
    707,713,719,725,730,736,741,747,
    753,758,764,769,774,780,785,790,
    796,801,806,811,816,821,826,831,
    836,841,846,850,855,860,864,869,
    873,878,882,886,890,895,899,903,
    907,911,915,919,922,926,930,933,
    937,940,944,947,950,953,957,960,
    963,966,968,971,974,977,979,982,
    984,986,989,991,993,995,997,999,
    1001,1003,1004,1006,1008,1009,1011,1012,
    1013,1014,1015,1017,1017,1018,1019,1020,
    1021,1021,1022,1022,1022,1023,1023,1023,
    1023,1023,1023,1023,1022,1022,1022,1021,
    1021,1020,1019,1018,1017,1017,1015,1014,
    1013,1012,1011,1009,1008,1006,1004,1003,
    1001,999,997,995,993,991,989,986,
    984,982,979,977,974,971,968,966,
    963,960,957,953,950,947,944,940,
    937,933,930,926,922,919,915,911,
    907,903,899,895,890,886,882,878,
    873,869,864,860,855,850,846,841,
    836,831,826,821,816,811,806,801,
    796,790,785,780,774,769,764,758,
    753,747,741,736,730,725,719,713,
    707,701,696,690,684,678,672,666,
    660,654,648,642,636,630,624,617,
    611,605,599,593,587,580,574,568,
    562,555,549,543,537,530,524,518,
    512,505,499,493,486,480,474,468,
    461,455,449,443,436,430,424,418,
    412,406,399,393,387,381,375,369,
    363,357,351,345,339,333,327,322,
    316,310,304,298,293,287,282,276,
    270,265,259,254,249,243,238,233,
    227,222,217,212,207,202,197,192,
    187,182,177,173,168,163,159,154,
    150,145,141,137,133,128,124,120,
    116,112,108,104,101,97,93,90,
    86,83,79,76,73,70,66,63,
    60,57,55,52,49,46,44,41,
    39,37,34,32,30,28,26,24,
    22,20,19,17,15,14,12,11,
    10,9,8,6,6,5,4,3,
    2,2,1,1,1,0,0,0,
    0,0,0,0,1,1,1,2,
    2,3,4,5,6,6,8,9,
    10,11,12,14,15,17,19,20,
    22,24,26,28,30,32,34,37,
    39,41,44,46,49,52,55,57,
    60,63,66,70,73,76,79,83,
    86,90,93,97,101,104,108,112,
    116,120,124,128,133,137,141,145,
    150,154,159,163,168,173,177,182,
    187,192,197,202,207,212,217,222,
    227,233,238,243,249,254,259,265,
    270,276,282,287,293,298,304,310,
    316,322,327,333,339,345,351,357,
    363,369,375,381,387,393,399,406,
    412,418,424,430,436,443,449,455,
    461,468,474,480,486,493,499,505,
     };


#endif /* DAC_H_ */