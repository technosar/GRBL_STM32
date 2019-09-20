/*
 * encoder.c
 *
 *  Created on: 18 févr. 2018
 *      Author: PC
 */
#include "encoder.h"

PORTPINDEF step_outbits;
PORTPINDEF dir_outbits;

extern settings_t settings;
extern PORTPINDEF step_port_invert_mask;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;

volatile int encoder_pos = 0;
volatile int encoder_pos_prev = 0;
volatile int encoder_step = 0;
volatile int cnt = 0;

pendant_t pendant;
TIM_HandleTypeDef htim4;

void encoder_init(void)
{

	return;

	/*Configure the TIM4 IRQ priority */
	HAL_NVIC_SetPriority(TIM4_IRQn, 6 ,0);
	/* Enable the TIM4 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	/* Enable TIM4 clock */
	__HAL_RCC_TIM4_CLK_ENABLE();


	htim4.Instance = TIM4;
	htim4.Init.Period = 1;
	htim4.Init.Prescaler = 0;
	htim4.Init.ClockDivision = 0;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&htim4) == HAL_OK)
	{
	  /* Start the TIM time Base generation in interrupt mode */
	  HAL_TIM_Base_Start_IT(&htim4);
	}

	TIM4->ARR = 0x2A30; // 10 KHz
	TIM4->EGR = TIM_EGR_UG;

    pendant.pulse_per_rev = 100; // 100 pulse per revolution
	pendant.phase = 4; // 4 pulse A,B decode
	pendant.resolution = 100; // resolution = 1/100 mm
	pendant.select = 0;
}

void _TIM4_IRQHandler(void)
{

	return;

	// encoder position
	  // 1/100 mm  -> 4 encodeur pulses
	  // 1/10 mm
	  // 1 mm
	  if (encoder_step == 0) {
		  cnt = htim5.Instance->CNT;
		  encoder_pos = ((int)cnt - encoder_pos_prev)/pendant.phase; // 4 pulses encodeurs
		  encoder_step = (int)((float)encoder_pos * settings.steps_per_mm[0]/(float)pendant.resolution);

		  dir_outbits = 0;
		  step_outbits = 0;
	  }
	  else {
		  encoder_pos_prev = cnt;
		  if (encoder_pos > 0) {
			  dir_outbits = (1<<X_DIRECTION_BIT);
			  encoder_step--;
		  }
		  else {
			  encoder_step++;
		  }

		  GPIO_WritePort(DIRECTION_PORT, (GPIO_ReadPort(DIRECTION_PORT) & ~DIRECTION_MASK) | (dir_outbits & DIRECTION_MASK));

		  step_outbits = (1<<X_STEP_BIT);
		  step_outbits ^= step_port_invert_mask;  // Apply step port invert mask
		  GPIO_WritePort(STEP_PORT, (GPIO_ReadPort(STEP_PORT) & ~STEP_MASK) | step_outbits);
		  // Enable step pulse reset timer so that The Stepper Port Reset Interrupt can reset the signal after
		  // exactly settings.pulse_microseconds microseconds, independent of the main Timer1 prescaler.
		#ifdef STM32F746I
		    TIM3->CNT = 0;
		    TIM3->EGR = TIM_EGR_UG;
		    __HAL_TIM_CLEAR_IT(&htim3, TIM3_IRQn);
		    HAL_NVIC_EnableIRQ(TIM3_IRQn);
		#endif

		  if (dir_outbits & (1<<X_DIRECTION_BIT)) { sys_position[X_AXIS]++; }
		  else { sys_position[X_AXIS]--; }
	  }
}
