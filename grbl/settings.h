/*
  settings.h - eeprom configuration handling
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

#ifndef settings_h
#define settings_h

#include "grbl.h"

#define MAX_PARAM_SETTINGS 276

typedef enum {						    // value typing for config and JSON
	TYPE_EMPTY = -1,					// value struct is empty (which is not the same as "NULL")
	TYPE_NULL = 0,						// value is 'null' (meaning the JSON null value)
	TYPE_PARENT,						// object is a parent to a sub-object
	TYPE_FLOAT,							// value is a floating point number
	TYPE_UINT8,						    // value is a unsigned
	TYPE_UINT16,					    // value is a unsigned
	TYPE_UINT32,					    // value is a unsigned
	TYPE_INT8,						    // value is a signed
	TYPE_INT16,						    // value is a signed
	TYPE_INT32,						    // value is a signed
	TYPE_STRING,						// value is in string field
	TYPE_BOOL,							// value is "true" (1) or "false"(0)
	TYPE_DATA,							// value is blind cast to uint32_t
	TYPE_ARRAY							// value is array element count, values are CSV ASCII in string field
} valueType;

typedef struct nvObject {				// depending on use, not all elements may be populated
	uint32_t index;                     // index of setting register
	valueType valuetype;                // see valueType enum
	int8_t precision;					// decimal precision for reporting (JSON)
	float *value;						// numeric value
	char *string;	    			    // pointer to array of characters from shared character array
} nvObj_t; 								// OK, so it's not REALLY an object

typedef uint8_t (*fptrCmd)(void *nv, float *target);        // required for cfg table access
typedef void (*fptrPrint)(void *nv);	// required for PROGMEM access

typedef struct cfgItem {
	uint32_t index;                     // index of setting register
	uint8_t controller;
	char *string;	    			    // pointer to array of characters from shared character array
	char *help;	    			    // pointer to array of characters from shared character array
	fptrPrint print;					// print binding: aka void (*print)(nvObj_t *nv);
	fptrCmd get;						// GET binding aka uint8_t (*get)(nvObj_t *nv)
	fptrCmd set;						// SET binding aka uint8_t (*set)(nvObj_t *nv)
	valueType valuetype;                // see valueType enum
	int32_t precision;					// decimal precision for display or bit position
	float def_value;					// default value for config item
	void *target;						// target for reading/writing config value
} cfgItem_t;


// Version of the EEPROM data. Will be used to migrate existing data from older versions of Grbl
// when firmware is upgraded. Always stored in byte 0 of eeprom
#define SETTINGS_VERSION 10  // NOTE: Check settings_reset() when moving to next version.

// Define bit flag masks for the boolean settings in settings.flag.
#define BITFLAG_REPORT_INCHES      bit(0)
#define BITFLAG_LASER_MODE         bit(1)
#define BITFLAG_INVERT_ST_ENABLE   bit(2)
#define BITFLAG_HARD_LIMIT_ENABLE  bit(3)
#define BITFLAG_HOMING_ENABLE      bit(4)
#define BITFLAG_SOFT_LIMIT_ENABLE  bit(5)
#define BITFLAG_INVERT_LIMIT_PINS  bit(6)
#define BITFLAG_INVERT_PROBE_PIN   bit(7)

// Define status reporting boolean enable bit flags in settings.status_report_mask
#define BITFLAG_RT_STATUS_WMPOSITION_TYPE     bit(0)  //      1
#define BITFLAG_RT_STATUS_BUFFER_STATE        bit(1)  //      2
                                                      //      4
#define BITFLAG_RT_STATUS_OVR                 bit(3)  //      8
#define BITFLAG_RT_STATUS_IO                  bit(4)  //     16
                                                      //     32
#define BITFLAG_RT_STATUS_WAIT_END_MOTION     bit(6)  //     64
#define BITFLAG_RT_STATUS_U_AXIS              bit(7)  //    128
#define BITFLAG_RT_STATUS_A_AXIS              bit(8)  //    256
#define BITFLAG_RT_STATUS_B_AXIS              bit(9)  //    512
#define BITFLAG_RT_STATUS_INPUTS              bit(10) //   1024
#define BITFLAG_RT_STATUS_OUTPUTS             bit(11) //   2048

// Define settings restore bitflags.
#define SETTINGS_RESTORE_DEFAULTS bit(0)
#define SETTINGS_RESTORE_PARAMETERS bit(1)
#define SETTINGS_RESTORE_STARTUP_LINES bit(2)
#define SETTINGS_RESTORE_BUILD_INFO bit(3)
#ifndef SETTINGS_RESTORE_ALL
  #define SETTINGS_RESTORE_ALL 0xFF // All bitflags
#endif

// Define EEPROM memory address location values for Grbl settings and parameters
// NOTE: The Atmega328p has 1KB EEPROM. The upper half is reserved for parameters and
// the startup script. The lower half contains the global settings and space for future
// developments.
#define EEPROM_ADDR_GLOBAL         1U
#define EEPROM_ADDR_PARAMETERS     512U
#define EEPROM_ADDR_STARTUP_BLOCK  768U
#define EEPROM_ADDR_BUILD_INFO     942U

// Define EEPROM address indexing for coordinate parameters
#define SETTING_INDEX_G54    0
#define SETTING_INDEX_G55    1
#define SETTING_INDEX_G56    2
#define SETTING_INDEX_G57    3
#define SETTING_INDEX_G58    4
#define SETTING_INDEX_G59    5

#define N_COORDINATE_SYSTEM 6  // Number of supported work coordinate systems (from index 1)
#define SETTING_INDEX_NCOORD N_COORDINATE_SYSTEM+1 // Total number of system stored (from index 0)
// NOTE: Work coordinate indices are (0=G54, 1=G55, ... , 6=G59)

#define SETTING_INDEX_G28    N_COORDINATE_SYSTEM    // Home position 1
#define SETTING_INDEX_G30    N_COORDINATE_SYSTEM+1  // Home position 2
// #define SETTING_INDEX_G92    N_COORDINATE_SYSTEM+2  // Coordinate offset (G92.2,G92.3 not supported)

// Define Grbl axis settings numbering scheme. Starts at START_VAL, every INCREMENT, over N_SETTINGS.
#define AXIS_N_SETTINGS                      4
#define AXIS_N_COORD_OFFSET                  8
#define AXIS_SETTINGS_START_VAL            100 // NOTE: Reserving settings values >= 100 for axis settings. Up to 199.
#define AXIS_SETTINGS_START_COORD_OFFSET   200 // NOTE: Reserving settings values >= 200 for coord offset settings. Up to 299.
#define AXIS_SETTINGS_INCREMENT             10  // Must be greater than the number of axis settings

// Global persistent settings (Stored from byte EEPROM_ADDR_GLOBAL onwards)
typedef struct {
  // Axis settings
  float steps_per_mm[N_AXIS];
  float max_rate[N_AXIS];
  float acceleration[N_AXIS];
  float max_travel[N_AXIS];

  float g54[N_AXIS];
  float g55[N_AXIS];
  float g56[N_AXIS];
  float g57[N_AXIS];
  float g58[N_AXIS];
  float g59[N_AXIS];
  float g28[N_AXIS];
  float g30[N_AXIS];

  // Remaining Grbl settings
  uint32_t pulse_microseconds;
  uint32_t step_invert_mask;
  uint32_t dir_invert_mask;
  uint32_t stepper_idle_lock_time; // If max value 255, steppers do not disable.
  uint32_t status_report_mask; // Mask to indicate desired report data.
  float junction_deviation;
  float arc_tolerance;

  float rpm_max;
  float rpm_min;

  uint32_t flags;  // Contains default boolean settings

  uint32_t homing_dir_mask;
  float homing_feed_rate;
  float homing_seek_rate;
  uint32_t homing_debounce_delay;
  float homing_pulloff;
  uint32_t wait_end_motion;  // Tracks end off all motion
} settings_t;
extern settings_t settings;

// Initialize the configuration subsystem (load settings from EEPROM)
void settings_init();

// Helper function to clear and restore EEPROM defaults
void settings_restore(uint8_t restore_flag);

// A helper method to set new settings from command line
uint8_t settings_store_global_setting(uint32_t parameter, float value);

// Helper function
uint8_t settings_print_help(uint32_t index);

// Stores the protocol line variable as a startup line in EEPROM
void settings_store_startup_line(uint8_t n, char *line);

// Reads an EEPROM startup line to the protocol line variable
uint8_t settings_read_startup_line(uint8_t n, char *line);

// Stores build info user-defined string
void settings_store_build_info(char *line);

// Reads build info user-defined string
uint8_t settings_read_build_info(char *line);

// Writes selected coordinate data to EEPROM
void settings_write_coord_data(uint8_t coord_select, float *coord_data);

// Reads selected coordinate data from EEPROM
uint8_t settings_read_coord_data(uint8_t coord_select, float *coord_data);

uint8_t get_settings(void *nv, float *target);
uint8_t set_settings(void *nv, float *target);
uint8_t get_settings_null(void *nv, float *target);
uint8_t set_settings_null(void *nv, float *target);
uint8_t settings_print_global_setting(uint32_t index);

#endif
