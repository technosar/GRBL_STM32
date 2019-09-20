/*
 * plc.h
 *
 *  Created on: 23 janv. 2018
 *      Author: PC
 */

#ifndef PLC_H_
#define PLC_H_

#define OUTPUT_MAX 8

#define STATUS_PLC_OK 0
#define STATUS_PLC_OUTPUT_OVERFLOW 410

uint32_t output_set_state(uint8_t number, uint8_t state);
uint32_t output_get_state(void);
uint32_t input_get_state(void);

#endif /* PLC_H_ */
