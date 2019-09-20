/*
  nuts_bolts.c - Shared functions
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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
#include <ctype.h>
#include "grbl.h"
#include "param.h"


#define MAX_INT_DIGITS 8 // Maximum number of digits in int32 (and float)

extern setup _setup;

uint32_t timestamp = 0;
volatile uint32_t tick = 0;

// Extracts a floating point value from a string. The following code is based loosely on
// the avr-libc strtod() function by Michael Stumpf and Dmitry Xmelkov and many freely
// available conversion method examples, but has been highly optimized for Grbl. For known
// CNC applications, the typical decimal value is expected to be in the range of E0 to E-4.
// Scientific notation is officially not supported by g-code, and the 'E' character may
// be a g-code word on some CNC systems. So, 'E' notation will not be recognized.
// NOTE: Thanks to Radu-Eosif Mihailescu for identifying the issues with using strtod().
uint8_t read_float(char *line, int *char_counter, float *float_ptr)
{
  read_real_value(line, char_counter, float_ptr, _setup.parameters);
  return(true);
}


// Non-blocking delay function used for general operation and suspend features.
void delay_sec(float seconds, uint8_t mode)
{
	uint32_t i = (uint32_t)seconds;
	uint32_t tickcount = timestamp;

	while ((timestamp - tickcount)< i) {
		if (sys.abort) { return; }
		if (mode == DELAY_MODE_DWELL) {
			protocol_execute_realtime();
		} else { // DELAY_MODE_SYS_SUSPEND
		  // Execute rt_system() only to avoid nesting suspend loops.
		  protocol_exec_rt_system();
		  if (sys.suspend & SUSPEND_RESTART_RETRACT) { return; } // Bail, if safety door reopens.
		}
//@		_delay_ms(DWELL_TIME_STEP); // Delay DWELL_TIME_STEP increment
	}
}

inline void delay(uint32_t time, uint32_t load) {
    tick = timestamp;
    while ((timestamp - tick) < time);
}

inline void delay_ms(uint32_t time) {
    delay(time, 0);
}


// Simple hypotenuse computation function.
float hypot_f(float x, float y)
{
	return(sqrtf(x*x + y*y));
}


float convert_delta_vector_to_unit_vector(float *vector)
{
  uint8_t idx;
  float magnitude = 0.0f;
  for (idx=0; idx<N_AXIS; idx++)
  {
    if (vector[idx] != 0.0f)
    {
      magnitude += vector[idx]*vector[idx];
    }
  }
  magnitude = sqrtf(magnitude);
  float inv_magnitude = 1.0f/magnitude;
  for (idx=0; idx<N_AXIS; idx++)
  {
	  vector[idx] *= inv_magnitude;
  }
  return(magnitude);
}


float limit_value_by_axis_maximum(float *max_value, float *unit_vec)
{
  uint8_t idx;
  float limit_value = SOME_LARGE_VALUE;
  for (idx=0; idx<N_AXIS; idx++) {
    if (unit_vec[idx] != 0)
    {  // Avoid divide by zero.
      limit_value = min(limit_value,fabsf(max_value[idx]/unit_vec[idx]));
    }
  }
  return(limit_value);
}
