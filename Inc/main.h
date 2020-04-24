/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2020 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LIMIT_Z_Pin GPIO_PIN_2
#define LIMIT_Z_GPIO_Port GPIOE
#define LIMIT_A_Pin GPIO_PIN_3
#define LIMIT_A_GPIO_Port GPIOE
#define LIMIT_B_Pin GPIO_PIN_4
#define LIMIT_B_GPIO_Port GPIOE
#define LIMIT_C_Pin GPIO_PIN_5
#define LIMIT_C_GPIO_Port GPIOE
#define LIMIT_U_Pin GPIO_PIN_6
#define LIMIT_U_GPIO_Port GPIOE
#define PWM_OUT_Pin GPIO_PIN_6
#define PWM_OUT_GPIO_Port GPIOF
#define LED_TX_Pin GPIO_PIN_4
#define LED_TX_GPIO_Port GPIOH
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define STEPPERS_DISABLE_BIT_Pin GPIO_PIN_2
#define STEPPERS_DISABLE_BIT_GPIO_Port GPIOB
#define CLK_X_Pin GPIO_PIN_0
#define CLK_X_GPIO_Port GPIOG
#define DIR_X_Pin GPIO_PIN_1
#define DIR_X_GPIO_Port GPIOG
#define LIMIT_V_Pin GPIO_PIN_7
#define LIMIT_V_GPIO_Port GPIOE
#define PROBE_Pin GPIO_PIN_14
#define PROBE_GPIO_Port GPIOE
#define UI3_Pin GPIO_PIN_15
#define UI3_GPIO_Port GPIOE
#define UI4_Pin GPIO_PIN_8
#define UI4_GPIO_Port GPIOD
#define UI5_Pin GPIO_PIN_9
#define UI5_GPIO_Port GPIOD
#define UI6_Pin GPIO_PIN_10
#define UI6_GPIO_Port GPIOD
#define UI7_Pin GPIO_PIN_11
#define UI7_GPIO_Port GPIOD
#define CONTROL_SAFETY_DOOR_Pin GPIO_PIN_12
#define CONTROL_SAFETY_DOOR_GPIO_Port GPIOD
#define CONTROL_SAFETY_DOOR_EXTI_IRQn EXTI15_10_IRQn
#define CONTROL_CYCLE_START_Pin GPIO_PIN_13
#define CONTROL_CYCLE_START_GPIO_Port GPIOD
#define CONTROL_CYCLE_START_EXTI_IRQn EXTI15_10_IRQn
#define CONTROL_FEED_HOLD_Pin GPIO_PIN_14
#define CONTROL_FEED_HOLD_GPIO_Port GPIOD
#define CONTROL_FEED_HOLD_EXTI_IRQn EXTI15_10_IRQn
#define CONTROL_RESET_Pin GPIO_PIN_15
#define CONTROL_RESET_GPIO_Port GPIOD
#define CONTROL_RESET_EXTI_IRQn EXTI15_10_IRQn
#define CLK_Y_Pin GPIO_PIN_2
#define CLK_Y_GPIO_Port GPIOG
#define DIR_Y_Pin GPIO_PIN_3
#define DIR_Y_GPIO_Port GPIOG
#define CLK_Z_Pin GPIO_PIN_4
#define CLK_Z_GPIO_Port GPIOG
#define DIR_Z_Pin GPIO_PIN_5
#define DIR_Z_GPIO_Port GPIOG
#define CLK_A_Pin GPIO_PIN_6
#define CLK_A_GPIO_Port GPIOG
#define DIR_A_Pin GPIO_PIN_7
#define DIR_A_GPIO_Port GPIOG
#define CLK_B_Pin GPIO_PIN_8
#define CLK_B_GPIO_Port GPIOG
#define DIR_B_Pin GPIO_PIN_9
#define DIR_B_GPIO_Port GPIOG
#define CLK_C_Pin GPIO_PIN_10
#define CLK_C_GPIO_Port GPIOG
#define DIR_C_Pin GPIO_PIN_11
#define DIR_C_GPIO_Port GPIOG
#define CLK_U_Pin GPIO_PIN_12
#define CLK_U_GPIO_Port GPIOG
#define DIR_U_Pin GPIO_PIN_13
#define DIR_U_GPIO_Port GPIOG
#define CLK_V_Pin GPIO_PIN_14
#define CLK_V_GPIO_Port GPIOG
#define DIR_V_Pin GPIO_PIN_15
#define DIR_V_GPIO_Port GPIOG
#define LED_RX_Pin GPIO_PIN_7
#define LED_RX_GPIO_Port GPIOB
#define LIMIT_X_Pin GPIO_PIN_0
#define LIMIT_X_GPIO_Port GPIOE
#define LIMIT_Y_Pin GPIO_PIN_1
#define LIMIT_Y_GPIO_Port GPIOE

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
