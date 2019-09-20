/*
 * encoder.h
 *
 *  Created on: 18 févr. 2018
 *      Author: PC
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "grbl.h"
#include "gpio.h"
#include "settings.h"
#include "system.h"


typedef struct {
	uint32_t pulse_per_rev;
	uint8_t phase;
	uint32_t select;
	uint8_t resolution;
} pendant_t;
extern pendant_t pendant;

void encoder_init(void);

#endif /* ENCODER_H_ */
