/*
 * plc_io.c
 *
 *  Created on: 23 janv. 2018
 *      Author: PC
 */

#include "grbl.h"
#include "cpu_map.h"
#include "system.h"
#include "grbl.h"
#include "plc_io.h"
#include "gpio.h"

extern SPI_HandleTypeDef hspi2;
extern uint32_t outputs_state;

uint32_t plc_output_set_state(uint8_t number, uint8_t state)
{
	if (number >= OUTPUT_MAX) return STATUS_PLC_OUTPUT_OVERFLOW;
	if (state) outputs_state |= (1 << number);
	else outputs_state &= ~(1 << number);
	HAL_SPI_Transmit_DMA(&hspi2,(uint8_t*)&outputs_state, 2);
	return STATUS_PLC_OK;
}

uint16_t plc_output_get_state(void)
{
  return((uint16_t)outputs_state);
}

// Returns input pin state as a uint32 bitfield.
uint32_t plc_input_get_state(void)
{
	uint32_t input = 0;
	uint16_t pin;

	pin = (GPIO_ReadPort(LIMIT_PIN_PORT) & LIMIT_MASK);
	if (bit_isfalse(settings.flags,BITFLAG_INVERT_LIMIT_PINS)) { pin ^= LIMIT_MASK; }
	pin >>= 8;
	input = pin;

	pin = ((GPIO_ReadPort(CONTROL_PIN_PORT) & CONTROL_MASK) >> CONTROL_SAFETY_DOOR_BIT) >> 8;
	input |= pin;

	pin = ((GPIO_ReadPort(PROBE_PORT) & PROBE_MASK) >> 6);
	input |= pin;

	return input;
}

uint32_t plc_wait_input_event(uint32_t pin, uint32_t edge, uint32_t timeout)
{

	return 0;
}

uint32_t plc_action_input_event(uint32_t pin, uint32_t edge, uint32_t timeout, uint32_t action)
{

	return 0;
}

