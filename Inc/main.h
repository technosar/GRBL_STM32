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
  * Copyright (c) 2019 STMicroelectronics International N.V. 
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

#define STEPPERS_DISABLE_BIT_Pin GPIO_PIN_2
#define STEPPERS_DISABLE_BIT_GPIO_Port GPIOB
#define LIMIT_X_Pin GPIO_PIN_8
#define LIMIT_X_GPIO_Port GPIOE
#define LIMIT_Y_Pin GPIO_PIN_9
#define LIMIT_Y_GPIO_Port GPIOE
#define LIMIT_Z_Pin GPIO_PIN_10
#define LIMIT_Z_GPIO_Port GPIOE
#define LIMIT_A_Pin GPIO_PIN_11
#define LIMIT_A_GPIO_Port GPIOE
#define IN_4_Pin GPIO_PIN_12
#define IN_4_GPIO_Port GPIOE
#define IN_5_Pin GPIO_PIN_13
#define IN_5_GPIO_Port GPIOE
#define IN_6_Pin GPIO_PIN_14
#define IN_6_GPIO_Port GPIOE
#define PROBE_Pin GPIO_PIN_15
#define PROBE_GPIO_Port GPIOE
#define IN_8_Pin GPIO_PIN_8
#define IN_8_GPIO_Port GPIOD
#define IN_9_Pin GPIO_PIN_9
#define IN_9_GPIO_Port GPIOD
#define IN_10_Pin GPIO_PIN_10
#define IN_10_GPIO_Port GPIOD
#define IN_10_EXTI_IRQn EXTI15_10_IRQn
#define IN_11_Pin GPIO_PIN_11
#define IN_11_GPIO_Port GPIOD
#define IN_11_EXTI_IRQn EXTI15_10_IRQn
#define IN_12_Pin GPIO_PIN_12
#define IN_12_GPIO_Port GPIOD
#define IN_12_EXTI_IRQn EXTI15_10_IRQn
#define IN_13_Pin GPIO_PIN_13
#define IN_13_GPIO_Port GPIOD
#define IN_13_EXTI_IRQn EXTI15_10_IRQn
#define IN_14_Pin GPIO_PIN_14
#define IN_14_GPIO_Port GPIOD
#define IN_14_EXTI_IRQn EXTI15_10_IRQn
#define IN_15_Pin GPIO_PIN_15
#define IN_15_GPIO_Port GPIOD
#define IN_15_EXTI_IRQn EXTI15_10_IRQn
#define CLK_X_Pin GPIO_PIN_0
#define CLK_X_GPIO_Port GPIOD
#define DIR_X_Pin GPIO_PIN_1
#define DIR_X_GPIO_Port GPIOD
#define CLK_Y_Pin GPIO_PIN_2
#define CLK_Y_GPIO_Port GPIOD
#define DIR_Y_Pin GPIO_PIN_3
#define DIR_Y_GPIO_Port GPIOD
#define CLK_Z_Pin GPIO_PIN_4
#define CLK_Z_GPIO_Port GPIOD
#define DIR_Z_Pin GPIO_PIN_5
#define DIR_Z_GPIO_Port GPIOD
#define CLK_A_Pin GPIO_PIN_6
#define CLK_A_GPIO_Port GPIOD
#define DIR_A_Pin GPIO_PIN_7
#define DIR_A_GPIO_Port GPIOD

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
#define LIMIT_GPIO_Port GPIOE
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
