/*
  cpu_map.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
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

// Define step pulse output pins. NOTE: All step bit pins must be on the same port.
#define STEP_PORT       GPIOG
#define X_STEP_BIT      0
#define Y_STEP_BIT      2
#define Z_STEP_BIT      4
#define A_STEP_BIT      6
#define B_STEP_BIT      8
#define C_STEP_BIT      10
#define U_STEP_BIT      12
#define V_STEP_BIT      14

#define X_STEP_BIT_DEF (1<<X_STEP_BIT)
#define Y_STEP_BIT_DEF (1<<Y_STEP_BIT)
#define Z_STEP_BIT_DEF (1<<Z_STEP_BIT)
#define A_STEP_BIT_DEF (1<<A_STEP_BIT)
#define B_STEP_BIT_DEF (1<<B_STEP_BIT)
#define C_STEP_BIT_DEF (1<<C_STEP_BIT)
#define U_STEP_BIT_DEF (1<<U_STEP_BIT)
#define V_STEP_BIT_DEF (1<<V_STEP_BIT)

#define STEP_MASK       (X_STEP_BIT_DEF|Y_STEP_BIT_DEF|Z_STEP_BIT_DEF|A_STEP_BIT_DEF|B_STEP_BIT_DEF|C_STEP_BIT_DEF|U_STEP_BIT_DEF|V_STEP_BIT_DEF) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
#define DIRECTION_PORT    GPIOG
#define X_DIRECTION_BIT   1
#define Y_DIRECTION_BIT   3
#define Z_DIRECTION_BIT   5
#define A_DIRECTION_BIT   7
#define B_DIRECTION_BIT   9
#define C_DIRECTION_BIT   11
#define U_DIRECTION_BIT   13
#define V_DIRECTION_BIT   15


#define X_DIRECTION_BIT_DEF (1<<X_DIRECTION_BIT)
#define Y_DIRECTION_BIT_DEF (1<<Y_DIRECTION_BIT)
#define Z_DIRECTION_BIT_DEF (1<<Z_DIRECTION_BIT)
#define A_DIRECTION_BIT_DEF (1<<A_DIRECTION_BIT)
#define B_DIRECTION_BIT_DEF (1<<B_DIRECTION_BIT)
#define C_DIRECTION_BIT_DEF (1<<C_DIRECTION_BIT)
#define U_DIRECTION_BIT_DEF (1<<U_DIRECTION_BIT)
#define V_DIRECTION_BIT_DEF (1<<V_DIRECTION_BIT)


#define DIRECTION_MASK    (X_DIRECTION_BIT_DEF|Y_DIRECTION_BIT_DEF|Z_DIRECTION_BIT_DEF|A_DIRECTION_BIT_DEF|B_DIRECTION_BIT_DEF|C_DIRECTION_BIT_DEF|U_DIRECTION_BIT_DEF|V_DIRECTION_BIT_DEF) // All direction bits
// Define homing/hard limit switch input pins and limit interrupt vectors.
// NOTE: All limit bit pins must be on the same port
#define LIMIT_PIN_PORT   GPIOE

#define X_LIMIT_BIT      0
#define Y_LIMIT_BIT      1
#define Z_LIMIT_BIT      2
#define A_LIMIT_BIT      3
#define B_LIMIT_BIT      4
#define C_LIMIT_BIT      5
#define U_LIMIT_BIT      6
#define V_LIMIT_BIT      7

#define LIMIT_X_BIT_DEF (1<<X_LIMIT_BIT)
#define LIMIT_Y_BIT_DEF (1<<Y_LIMIT_BIT)
#define LIMIT_Z_BIT_DEF (1<<Z_LIMIT_BIT)
#define LIMIT_A_BIT_DEF (1<<A_LIMIT_BIT)
#define LIMIT_B_BIT_DEF (1<<B_LIMIT_BIT)
#define LIMIT_C_BIT_DEF (1<<C_LIMIT_BIT)
#define LIMIT_U_BIT_DEF (1<<U_LIMIT_BIT)
#define LIMIT_V_BIT_DEF (1<<V_LIMIT_BIT)


#define LIMIT_MASK       (LIMIT_X_BIT_DEF|LIMIT_Y_BIT_DEF|LIMIT_Z_BIT_DEF|LIMIT_A_BIT_DEF|LIMIT_B_BIT_DEF|LIMIT_C_BIT_DEF|LIMIT_U_BIT_DEF|LIMIT_V_BIT_DEF) // All limit bits

// Define probe switch input pin.
#define PROBE_PORT                    GPIOE
#define PROBE_BIT                     14
#define PROBE_MASK                    (1<<PROBE_BIT)

  // Start of PWM & Stepper Enabled Spindle
#ifdef VARIABLE_SPINDLE

  // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
#define SPINDLE_PWM_FREQUENCY       10000                   // KHz

#endif // End of VARIABLE_SPINDLE
#define SPINDLE_PWM_MAX_VALUE       (1000000 / SPINDLE_PWM_FREQUENCY)
#ifndef SPINDLE_PWM_MIN_VALUE
#define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
#endif
#define SPINDLE_PWM_OFF_VALUE     0
#define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)

  // Define stepper driver enable/disable output pin.
#define STEPPERS_DISABLE_PORT   GPIOB
#define STEPPERS_DISABLE_BIT    2
#define STEPPERS_DISABLE_MASK   (1<<STEPPERS_DISABLE_BIT)
#define SetStepperDisableBit() GPIO_SetBits(STEPPERS_DISABLE_PORT,STEPPERS_DISABLE_MASK)
#define ResetStepperDisableBit() GPIO_ResetBits(STEPPERS_DISABLE_PORT,STEPPERS_DISABLE_MASK)

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
#define CONTROL_PIN_PORT              GPIOD
#define CONTROL_PORT                  GPIOD
#define CONTROL_RESET_BIT             15
#define CONTROL_FEED_HOLD_BIT         14
#define CONTROL_CYCLE_START_BIT       13
#define CONTROL_SAFETY_DOOR_BIT       12
#define CONTROL_MASK                 ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))

/*
#ifdef CPU_MAP_CUSTOM_PROC
  // For a custom pin map or different processor, copy and edit one of the available cpu
  // map files and modify it to your needs. Make sure the defined name is also changed in
  // the config.h file.
#endif
*/

#endif
