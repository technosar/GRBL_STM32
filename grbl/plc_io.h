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

uint32_t plc_output_set_state(uint8_t number, uint8_t state);
uint16_t plc_output_get_state(void);
uint32_t plc_input_get_state(void);
uint32_t plc_wait_input_event(uint32_t pin, uint32_t edge, uint32_t timeout);

#endif /* PLC_H_ */
