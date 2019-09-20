/*
  cpu_map.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software COOLANT_FLOOD_BITFoundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/* The cpu_map.h files serve as a central pin mapping selection file for different
   processor types or alternative pin layouts. This version of Grbl officially supports
   only the Arduino Mega328p. */


#ifndef cpu_map_h
#define cpu_map_h


#ifdef CPU_MAP_STM32F746I

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
#define STEP_PORT       GPIOI
#define X_STEP_BIT      0
#define Y_STEP_BIT      1  
#define Z_STEP_BIT      2
#define A_STEP_BIT      3


#ifdef X_AXIS
#define X_STEP_BIT_DEF (1<<X_STEP_BIT)
#else
#define X_STEP_BIT_DEF 0
#endif

#ifdef Y_AXIS
#define Y_STEP_BIT_DEF (1<<Y_STEP_BIT)
#else
#define Y_STEP_BIT_DEF 0
#endif

#ifdef Z_AXIS
#define Z_STEP_BIT_DEF (1<<Z_STEP_BIT)
#else
#define Z_STEP_BIT_DEF 0
#endif

#ifdef A_AXIS
#define A_STEP_BIT_DEF (1<<A_STEP_BIT)
#else
#define A_STEP_BIT_DEF 0
#endif

#define STEP_MASK       (X_STEP_BIT_DEF|Y_STEP_BIT_DEF|Z_STEP_BIT_DEF|A_STEP_BIT_DEF) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
#define DIRECTION_PORT      GPIOI
#define X_DIRECTION_BIT   6
#define Y_DIRECTION_BIT   7
#define Z_DIRECTION_BIT   8
#define A_DIRECTION_BIT   9


#ifdef X_AXIS
#define X_DIRECTION_BIT_DEF (1<<X_DIRECTION_BIT)
#else
#define X_DIRECTION_BIT_DEF 0
#endif
#ifdef Y_AXIS
#define Y_DIRECTION_BIT_DEF (1<<Y_DIRECTION_BIT)
#else
#define Y_DIRECTION_BIT_DEF 0
#endif
#ifdef Z_AXIS
#define Z_DIRECTION_BIT_DEF (1<<Z_DIRECTION_BIT)
#else
#define Z_DIRECTION_BIT_DEF 0
#endif
#ifdef A_AXIS
#define A_DIRECTION_BIT_DEF (1<<A_DIRECTION_BIT)
#else
#define A_DIRECTION_BIT_DEF 0
#endif


#define DIRECTION_MASK    (X_DIRECTION_BIT_DEF|Y_DIRECTION_BIT_DEF|Z_DIRECTION_BIT_DEF|A_DIRECTION_BIT_DEF) // All direction bits

  // Define stepper driver enable/disable output pin.
#define STEPPERS_DISABLE_PORT   GPIOA
#define STEPPERS_DISABLE_BIT    6  
#define STEPPERS_DISABLE_MASK   (1<<STEPPERS_DISABLE_BIT)
#define SetStepperDisableBit() GPIO_SetBits(STEPPERS_DISABLE_PORT,STEPPERS_DISABLE_MASK)
#define ResetStepperDisableBit() GPIO_ResetBits(STEPPERS_DISABLE_PORT,STEPPERS_DISABLE_MASK)


  // Define homing/hard limit switch input pins and limit interrupt vectors. 
  // NOTE: All limit bit pins must be on the same port
#define LIMIT_PIN_PORT   GPIOH
#define LIMIT_PORT       GPIOH
#define RCC_LIMIT_PORT   RCC_APB2Periph_GPIOH
//#define GPIO_LIMIT_PORT  GPIO_PortSourceGPIOB
#define X_LIMIT_BIT      12
#define Y_LIMIT_BIT      13
#define Z_LIMIT_BIT      14
#define A_LIMIT_BIT      15
#define B_LIMIT_BIT      15
#ifdef C_AXIS
#define C_LIMIT_BIT      99
#endif
#define U_LIMIT_BIT      15
#ifdef V_AXIS
#define V_LIMIT_BIT      99
#endif
#ifdef W_AXIS
#define W_LIMIT_BIT      99
#endif


#ifdef X_AXIS
#define LIMIT_X_BIT_DEF (1<<X_LIMIT_BIT)
#else
#define LIMIT_X_BIT_DEF 0
#endif
#ifdef Y_AXIS
#define LIMIT_Y_BIT_DEF (1<<Y_LIMIT_BIT)
#else
#define LIMIT_Y_BIT_DEF 0
#endif
#ifdef Z_AXIS
#define LIMIT_Z_BIT_DEF (1<<Z_LIMIT_BIT)
#else
#define LIMIT_Z_BIT_DEF 0
#endif
#ifdef A_AXIS
#define LIMIT_A_BIT_DEF (1<<A_LIMIT_BIT)
#else
#define LIMIT_A_BIT_DEF 0
#endif


#define LIMIT_MASK       (LIMIT_X_BIT_DEF|LIMIT_Y_BIT_DEF|LIMIT_Z_BIT_DEF|LIMIT_A_BIT_DEF) // All limit bits

  // Define spindle enable and spindle direction output pins.
#define SPINDLE_ENABLE_PORT   GPIOB
#define RCC_SPINDLE_ENABLE_PORT RCC_APB2Periph_GPIOB
#define SPINDLE_ENABLE_BIT    11  //
#ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
#define SPINDLE_DIRECTION_DDR   GPIOB
#define SPINDLE_DIRECTION_PORT  GPIOB
#define SPINDLE_DIRECTION_BIT   13  //
#endif
#define SetSpindleEnablebit()       GPIO_WriteBit(SPINDLE_ENABLE_PORT, 1 << SPINDLE_ENABLE_BIT, Bit_SET)
#define ResetSpindleEnablebit()     GPIO_WriteBit(SPINDLE_ENABLE_PORT, 1 << SPINDLE_ENABLE_BIT, Bit_RESET)
#define SetSpindleDirectionBit()    GPIO_WriteBit(SPINDLE_DIRECTION_PORT, 1 << SPINDLE_DIRECTION_BIT,Bit_SET)
#define ResetSpindleDirectionBit()  GPIO_WriteBit(SPINDLE_DIRECTION_PORT, 1 << SPINDLE_DIRECTION_BIT,Bit_RESET)

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
#define CONTROL_PIN_PORT              GPIOD
#define CONTROL_PORT                  GPIOD
#define RCC_CONTROL_PORT              RCC_APB2Periph_GPIOD
#define GPIO_CONTROL_PORT             GPIO_PortSourceGPIOD
#define CONTROL_RESET_BIT             3
#define CONTROL_FEED_HOLD_BIT         4
#define CONTROL_CYCLE_START_BIT       5
#define CONTROL_SAFETY_DOOR_BIT       6
#define CONTROL_MASK                 ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))

  // Define probe switch input pin.
#define PROBE_PORT                    GPIOH
#define RCC_PROBE_PORT                RCC_APB2Periph_GPIOH
#define PROBE_BIT                     8
#define PROBE_MASK                    (1<<PROBE_BIT)

  // Start of PWM & Stepper Enabled Spindle
#ifdef VARIABLE_SPINDLE

  // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
#define SPINDLE_PWM_FREQUENCY       10000                   // KHz
#define SPINDLE_PWM_DDR	            GPIOA
#define SPINDLE_PWM_PORT            GPIOA
#define RCC_SPINDLE_PWM_PORT        RCC_APB2Periph_GPIOA
#define SPINDLE_PWM_BIT	            8    
#endif // End of VARIABLE_SPINDLE
#define SPINDLE_PWM_MAX_VALUE       (1000000 / SPINDLE_PWM_FREQUENCY)
#ifndef SPINDLE_PWM_MIN_VALUE
#define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
#endif
#define SPINDLE_PWM_OFF_VALUE     0
#define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)

  //          Port A                Port I               Port H                    Port F                     Port C
  //   0      X_STEP_BIT            X_DIRECTION_BIT                                                           CONTROL_RESET_BIT
  //   1      Y_STEP_BIT            Y_DIRECTION_BIT                                                           CONTROL_FEED_HOLD_BIT
  //   2      Z_STEP_BIT            Z_DIRECTION_BIT                                                           CONTROL_CYCLE_START_BIT
  //   3      C_STEP_BIT            C_DIRECTION_BIT                                                           CONTROL_CYCLE_START_BIT
  //   4
  //   5
  //   6
  //   7
  //   8      STEPPERS_DISABLE_BIT                       PROBE_BIT                COOLANT_FLOOD_BIT
  //   9                                                                          COOLANT_MIST_BIT
  //   10                                                X_LIMIT_BIT
  //   11                                                Y_LIMIT_BIT
  //   12                                                Z_LIMIT_BIT
  //   13       										 A_LIMIT_BIT
//     14       										 B_LIMIT_BIT
  //   15       										 U_LIMIT_BIT

#endif
/*
#ifdef CPU_MAP_CUSTOM_PROC
  // For a custom pin map or different processor, copy and edit one of the available cpu
  // map files and modify it to your needs. Make sure the defined name is also changed in
  // the config.h file.
#endif
*/

#endif
