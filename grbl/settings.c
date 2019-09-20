/*
  settings.c - eeprom configuration handling
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
#include "grbl.h"
#include "report.h"

settings_t settings;


cfgItem_t cfgArray[] = {
//   index Format                                            print_func,      get_func           set_func           type         precision / bit            Default value                     target for get/set
	{ 0, 0, "%c%d=%d\r\n", "Step pulse, microseconds\r\n",          text_print,      get_settings,      set_settings,      TYPE_UINT32, 0,                         DEFAULT_STEP_PULSE_MICROSECONDS , (uint32_t*)&settings.pulse_microseconds },
    { 1, 0, "%c%d=%d\r\n", "Step idle delay, milliseconds\r\n",     text_print,      get_settings,      set_settings,      TYPE_UINT32, 0,                         DEFAULT_STEPPER_IDLE_LOCK_TIME  , (uint32_t*)&settings.stepper_idle_lock_time },
	{ 2, 0, "%c%d=%d\r\n", "Step port invert, mask\r\n",            text_print,      get_settings,      set_settings,      TYPE_UINT32, 0,                         DEFAULT_STEPPING_INVERT_MASK    , (uint32_t*)&settings.step_invert_mask },
	{ 3, 0, "%c%d=%d\r\n", "Direction port invert, mask\r\n",       text_print,      get_settings,      set_settings,      TYPE_UINT32, 0,                         DEFAULT_DIRECTION_INVERT_MASK   , (uint32_t*)&settings.dir_invert_mask },
	{ 4, 0, "%c%d=%c\r\n", "Step enable invert, boolean\r\n",       text_print,      get_settings,      set_settings,      TYPE_BOOL,   BITFLAG_INVERT_ST_ENABLE,  DEFAULT_INVERT_ST_ENABLE        , (uint32_t*)&settings.flags },
	{ 5, 0, "%c%d=%c\r\n", "Limit pins invert, boolean\r\n",        text_print,      get_settings,      set_settings,      TYPE_BOOL,   BITFLAG_INVERT_LIMIT_PINS, DEFAULT_INVERT_LIMIT_PINS       , (uint32_t*)&settings.flags },
	{ 6, 0, "%c%d=%c\r\n", "Probe pin invert, boolean\r\n",         text_print,      get_settings,      set_settings,      TYPE_BOOL,   BITFLAG_INVERT_PROBE_PIN,  DEFAULT_INVERT_PROBE_PIN        , (uint32_t*)&settings.flags },
	{ 7, 1, "", "",                                                 text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 8, 1, "", "",                                                 text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 9, 1, "", "",                                                 text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 10, 0, "%c%d=%d\r\n", "Status report, mask\r\n",              text_print,      get_settings,      set_settings,      TYPE_UINT32, 0,                         DEFAULT_STATUS_REPORT_MASK      , (uint32_t*)&settings.status_report_mask },
	{ 11, 0, "%c%d=%.3f\r\n", "Junction deviation, mm\r\n",         text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_JUNCTION_DEVIATION      , (uint32_t*)&settings.junction_deviation },
	{ 12, 0, "%c%d=%.3f\r\n", "Arc tolerance, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_ARC_TOLERANCE           , (uint32_t*)&settings.arc_tolerance },
	{ 13, 0, "%c%d=%c\r\n", "Report inches, boolean\r\n",           text_print,      get_settings,      set_settings,      TYPE_BOOL ,  BITFLAG_REPORT_INCHES,     DEFAULT_REPORT_INCHES           , (uint32_t*)&settings.flags },
	{ 14, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 15, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 16, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 17, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 18, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 19, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 20, 0, "%c%d=%c\r\n", "Soft limits, boolean\r\n",             text_print,      get_settings,      set_settings,      TYPE_BOOL ,  BITFLAG_SOFT_LIMIT_ENABLE, DEFAULT_SOFT_LIMIT_ENABLE       , (uint32_t*)&settings.flags },
	{ 21, 0, "%c%d=%c\r\n", "Hard limits, boolean\r\n",             text_print,      get_settings,      set_settings,      TYPE_BOOL ,  BITFLAG_HARD_LIMIT_ENABLE, DEFAULT_HARD_LIMIT_ENABLE       , (uint32_t*)&settings.flags },
	{ 22, 0, "%c%d=%c\r\n", "Homing cycle, boolean\r\n",            text_print,      get_settings,      set_settings,      TYPE_BOOL ,  BITFLAG_HOMING_ENABLE,     DEFAULT_HOMING_ENABLE           , (uint32_t*)&settings.flags },
	{ 23, 0, "%c%d=%d\r\n", "Homing dir invert, mask\r\n",          text_print,      get_settings,      set_settings,      TYPE_UINT32, 0,                         DEFAULT_HOMING_DIR_MASK         , (uint32_t*)&settings.homing_dir_mask },
	{ 24, 0, "%c%d=%.3f\r\n", "Homing feed, mm/min\r\n",            text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_HOMING_FEED_RATE        , (uint32_t*)&settings.homing_feed_rate },
	{ 25, 0, "%c%d=%.3f\r\n", "Homing seek, mm/min\r\n",            text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_HOMING_SEEK_RATE        , (uint32_t*)&settings.homing_seek_rate },
	{ 26, 0, "%c%d=%d\r\n", "Homing debounce, milliseconds\r\n",    text_print,      get_settings,      set_settings,      TYPE_UINT32, 0,                         DEFAULT_HOMING_DEBOUNCE_DELAY   , (uint32_t*)&settings.homing_debounce_delay },
	{ 27, 0, "%c%d=%.3f\r\n", "Homing pull-off, mm\r\n",            text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_HOMING_PULLOFF          , (uint32_t*)&settings.homing_pulloff },
	{ 28, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 29, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 30, 0, "%c%d=%.3f\r\n", "Max spindle speed, RPM\r\n",         text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_SPINDLE_RPM_MAX         , (uint32_t*)&settings.rpm_max },
	{ 31, 0, "%c%d=%.3f\r\n", "Min spindle speed, RPM\r\n",         text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_SPINDLE_RPM_MIN         , (uint32_t*)&settings.rpm_min },
	{ 32, 0, "%c%d=%c\r\n", "Laser mode, boolean\r\n",              text_print,      get_settings,      set_settings,      TYPE_BOOL ,  BITFLAG_LASER_MODE,        DEFAULT_LASER_MODE              , (uint32_t*)&settings.flags },
	{ 33, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 34, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 35, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 36, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 37, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 38, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 39, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 40, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 41, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 42, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 43, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 44, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 45, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 46, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 47, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 48, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 49, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 50, 1, "%c%d=%d\r\n", "Default gcode plane [0=G17,1=G18,2=G19]\r\n", text_print, get_settings,    set_settings,      TYPE_UINT8,  0,                         PLANE_SELECT_XY                  , (uint8_t*)&gc_state.modal.plane_select },
	{ 51, 1, "%c%d=%d\r\n", "Default gcode units mode [0=G20,1=G21]\r\n",  text_print, get_settings,    set_settings,      TYPE_UINT8,  0,                         UNITS_MODE_MM                    , (uint8_t*)&gc_state.modal.units },
	{ 52, 1, "%c%d=%d\r\n", "Default gcode coord system [0-5 (G54-G59)]\r\n",  text_print, get_settings,set_settings,      TYPE_UINT8,  0,                         COORD_SYSTEM                     , (uint8_t*)&gc_state.modal.coord_select },
	{ 53, 1, "%c%d=%d\r\n", "Default gcode path control [0=G61,1=G61.1,2=G64]\r\n",  text_print, get_settings,set_settings,TYPE_UINT8,  0,                         EXACT_PATH                       , (uint8_t*)&gc_state.modal.control },
	{ 54, 1, "%c%d=%d\r\n", "Default gcode distance mode [0=G90,1=G91]\r\n",  text_print, get_settings, set_settings,      TYPE_UINT8,  0,                         DISTANCE_MODE_ABSOLUTE           , (uint8_t*)&gc_state.modal.distance },
	{ 55, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 56, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 57, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 58, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 59, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 60, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 61, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 62, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 63, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 64, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 65, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 66, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 67, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 68, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 69, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 70, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 71, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 72, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 73, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 74, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 75, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 76, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 77, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 78, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 79, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 80, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 81, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 82, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 83, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 84, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 85, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 86, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 87, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 88, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 89, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 90, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 91, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 92, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 93, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 94, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 95, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 96, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 97, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 98, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 99, 1, "", "",                                                text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 100, 0, "%c%d=%.3f\r\n", "X steps/mm\r\n",                    text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_X_STEPS_PER_MM          , (uint32_t*)&settings.steps_per_mm[X_AXIS] },
	{ 101, 0, "%c%d=%.3f\r\n", "Y steps/mm\r\n",                    text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_Y_STEPS_PER_MM          , (uint32_t*)&settings.steps_per_mm[Y_AXIS] },
	{ 102, 0, "%c%d=%.3f\r\n", "Z steps/mm\r\n",                    text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_Z_STEPS_PER_MM          , (uint32_t*)&settings.steps_per_mm[Z_AXIS] },
	{ 103, 1, "%c%d=%.3f\r\n", "A steps/mm\r\n",                    text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_A_STEPS_PER_MM          , (uint32_t*)&settings.steps_per_mm[A_AXIS] },
	{ 104, 1, "", "",                    							text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 105, 1, "", "",                   							text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 106, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 107, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 108, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 109, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 110, 0, "%c%d=%.3f\r\n", "X Max rate, mm/min\r\n",            text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_X_MAX_RATE              , (uint32_t*)&settings.max_rate[X_AXIS] },
	{ 111, 0, "%c%d=%.3f\r\n", "Y Max rate, mm/min\r\n",            text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_Y_MAX_RATE              , (uint32_t*)&settings.max_rate[Y_AXIS] },
	{ 112, 0, "%c%d=%.3f\r\n", "Z Max rate, mm/min\r\n",            text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_Z_MAX_RATE              , (uint32_t*)&settings.max_rate[Z_AXIS] },
	{ 113, 1, "%c%d=%.3f\r\n", "A Max rate, mm/min\r\n",            text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_A_MAX_RATE              , (uint32_t*)&settings.max_rate[A_AXIS] },
	{ 114, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 115, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 116, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 117, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 118, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 119, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 120, 0, "%c%d=%.3f\r\n", "X Acceleration, mm/sec^2\r\n",      text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_X_ACCELERATION          , (uint32_t*)&settings.acceleration[X_AXIS] },
	{ 121, 0, "%c%d=%.3f\r\n", "Y Acceleration, mm/sec^2\r\n",      text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_Y_ACCELERATION          , (uint32_t*)&settings.acceleration[Y_AXIS] },
	{ 122, 0, "%c%d=%.3f\r\n", "Z Acceleration, mm/sec^2\r\n",      text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_Z_ACCELERATION          , (uint32_t*)&settings.acceleration[Z_AXIS] },
	{ 123, 1, "%c%d=%.3f\r\n", "A Acceleration, mm/sec^2\r\n",      text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_A_ACCELERATION          , (uint32_t*)&settings.acceleration[A_AXIS] },
	{ 124, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 125, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 126, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 127, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 128, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 129, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 130, 0, "%c%d=%.3f\r\n", "X Max travel, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_X_MAX_TRAVEL            , (uint32_t*)&settings.max_travel[X_AXIS] },
	{ 131, 0, "%c%d=%.3f\r\n", "Y Max travel, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_Y_MAX_TRAVEL            , (uint32_t*)&settings.max_travel[Y_AXIS] },
	{ 132, 0, "%c%d=%.3f\r\n", "Z Max travel, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_Z_MAX_TRAVEL            , (uint32_t*)&settings.max_travel[Z_AXIS] },
	{ 133, 1, "%c%d=%.3f\r\n", "A Max travel, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         DEFAULT_A_MAX_TRAVEL            , (uint32_t*)&settings.max_travel[A_AXIS] },
	{ 134, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 135, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 136, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 137, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 138, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 139, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 140, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 141, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 142, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 143, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 144, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 145, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 146, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 147, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 148, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 149, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 150, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 151, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 152, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 153, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 154, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 155, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 156, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 157, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 158, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 159, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 160, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 161, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 162, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 163, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 164, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 165, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 166, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 167, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 168, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 169, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 170, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 171, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 172, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 173, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 174, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 175, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 176, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 177, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 178, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 179, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 180, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 181, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 182, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 183, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 184, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 185, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 186, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 187, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 188, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 189, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 190, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 191, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 192, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 193, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 194, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 195, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 196, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 197, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 198, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 199, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 200, 1, "%c%d=%.3f\r\n", "g54 X offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g54[X_AXIS] },
	{ 201, 1, "%c%d=%.3f\r\n", "g54 Y offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g54[Y_AXIS] },
	{ 202, 1, "%c%d=%.3f\r\n", "g54 Z offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g54[Z_AXIS] },
	{ 203, 1, "%c%d=%.3f\r\n", "g54 A offset, deg\r\n",             text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g54[A_AXIS] },
	{ 204, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 205, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 206, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 207, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 208, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 209, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 210, 1, "%c%d=%.3f\r\n", "g55 X offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g55[X_AXIS] },
	{ 211, 1, "%c%d=%.3f\r\n", "g55 Y offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g55[Y_AXIS] },
	{ 212, 1, "%c%d=%.3f\r\n", "g55 Z offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g55[Z_AXIS] },
	{ 213, 1, "%c%d=%.3f\r\n", "g55 A offset, deg\r\n",             text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g55[A_AXIS] },
	{ 214, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 215, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 216, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 217, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 218, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 219, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 220, 1, "%c%d=%.3f\r\n", "g56 X offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g56[X_AXIS] },
	{ 221, 1, "%c%d=%.3f\r\n", "g56 Y offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g56[Y_AXIS] },
	{ 222, 1, "%c%d=%.3f\r\n", "g56 Z offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g56[Z_AXIS] },
	{ 223, 1, "%c%d=%.3f\r\n", "g56 A offset, deg\r\n",             text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g56[A_AXIS] },
	{ 224, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 225, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 226, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 227, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 228, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 229, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 230, 1, "%c%d=%.3f\r\n", "g57 X offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g57[X_AXIS] },
	{ 231, 1, "%c%d=%.3f\r\n", "g57 Y offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g57[Y_AXIS] },
	{ 232, 1, "%c%d=%.3f\r\n", "g57 Z offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g57[Z_AXIS] },
	{ 233, 1, "%c%d=%.3f\r\n", "g57 A offset, deg\r\n",             text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g57[A_AXIS] },
	{ 234, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 235, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 236, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 237, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 238, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 239, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 240, 1, "%c%d=%.3f\r\n", "g58 X offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g58[X_AXIS] },
	{ 241, 1, "%c%d=%.3f\r\n", "g58 Y offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g58[Y_AXIS] },
	{ 242, 1, "%c%d=%.3f\r\n", "g58 Z offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g58[Z_AXIS] },
	{ 243, 1, "%c%d=%.3f\r\n", "g58 A offset, deg\r\n",             text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g58[A_AXIS] },
	{ 244, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 245, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 246, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 247, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 248, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 249, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 250, 1, "%c%d=%.3f\r\n", "g59 X offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g59[X_AXIS] },
	{ 251, 1, "%c%d=%.3f\r\n", "g59 Y offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g59[Y_AXIS] },
	{ 252, 1, "%c%d=%.3f\r\n", "g59 Z offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g59[Z_AXIS] },
	{ 253, 1, "%c%d=%.3f\r\n", "g59 A offset, deg\r\n",             text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g59[A_AXIS] },
	{ 254, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 255, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 256, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 257, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 258, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 259, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 260, 1, "%c%d=%.3f\r\n", "g28 X offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g28[X_AXIS] },
	{ 261, 1, "%c%d=%.3f\r\n", "g28 Y offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g28[Y_AXIS] },
	{ 262, 1, "%c%d=%.3f\r\n", "g28 Z offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g28[Z_AXIS] },
	{ 263, 1, "%c%d=%.3f\r\n", "g28 A offset, deg\r\n",             text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g28[A_AXIS] },
	{ 264, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 265, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 266, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 267, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 268, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 269, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 270, 1, "%c%d=%.3f\r\n", "g30 X offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g30[X_AXIS] },
	{ 271, 1, "%c%d=%.3f\r\n", "g30 Y offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g30[Y_AXIS] },
	{ 272, 1, "%c%d=%.3f\r\n", "g30 Z offset, mm\r\n",              text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g30[Z_AXIS] },
	{ 273, 1, "%c%d=%.3f\r\n", "g30 A offset, deg\r\n",             text_print,      get_settings,      set_settings,      TYPE_FLOAT,  3,                         0                               , (uint32_t*)&settings.g30[A_AXIS] },
	{ 274, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ 275, 1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL },
	{ -1,  1, "", "",                                               text_print_null, get_settings_null, set_settings_null, TYPE_NULL,   0,  0  , NULL }
};

uint8_t get_settings(void *nv, float *target)
{
	return STATUS_OK;
}

uint8_t set_settings(void *nv, float *target)
{
	return STATUS_OK;
}

uint8_t get_settings_null(void *nv, float *target)
{
	return STATUS_OK;
}

uint8_t set_settings_null(void *nv, float *target)
{
	return STATUS_OK;
}

// Method to store startup lines into EEPROM
void settings_store_startup_line(uint8_t n, char *line)
{
  #ifdef FORCE_BUFFER_SYNC_DURING_EEPROM_WRITE
    protocol_buffer_synchronize(); // A startup line may contain a motion and be executing. 
  #endif
  uint32_t addr = n*(LINE_BUFFER_SIZE+1)+EEPROM_ADDR_STARTUP_BLOCK;
  memcpy_to_eeprom_with_checksum(addr,(char*)line, LINE_BUFFER_SIZE);
}


// Method to store build info into EEPROM
// NOTE: This function can only be called in IDLE state.
void settings_store_build_info(char *line)
{
  // Build info can only be stored when state is IDLE.
  memcpy_to_eeprom_with_checksum(EEPROM_ADDR_BUILD_INFO,(char*)line, LINE_BUFFER_SIZE);
}


// Method to store coord data parameters into EEPROM
void settings_write_coord_data(uint8_t coord_select, float *coord_data)
{
  #ifdef FORCE_BUFFER_SYNC_DURING_EEPROM_WRITE
    protocol_buffer_synchronize();
  #endif
  uint32_t addr = coord_select*(sizeof(float)*N_AXIS+1) + EEPROM_ADDR_PARAMETERS;
  memcpy_to_eeprom_with_checksum(addr,(char*)coord_data, sizeof(float)*N_AXIS);
}


// Method to store Grbl global settings struct and version number into EEPROM
// NOTE: This function can only be called in IDLE state.
void write_global_settings()
{
  eeprom_put_char(0, SETTINGS_VERSION);
  memcpy_to_eeprom_with_checksum(EEPROM_ADDR_GLOBAL, (char*)&settings, sizeof(settings_t));
}


// Method to restore EEPROM-saved Grbl global settings back to defaults.
void settings_restore(uint8_t restore_flag) {
  if (restore_flag & SETTINGS_RESTORE_DEFAULTS) {
    settings.pulse_microseconds = DEFAULT_STEP_PULSE_MICROSECONDS;
    settings.stepper_idle_lock_time = DEFAULT_STEPPER_IDLE_LOCK_TIME;
    settings.step_invert_mask = DEFAULT_STEPPING_INVERT_MASK;
    settings.dir_invert_mask = DEFAULT_DIRECTION_INVERT_MASK;
    settings.status_report_mask = DEFAULT_STATUS_REPORT_MASK;
    settings.junction_deviation = DEFAULT_JUNCTION_DEVIATION;
    settings.arc_tolerance = DEFAULT_ARC_TOLERANCE;

    settings.rpm_max = DEFAULT_SPINDLE_RPM_MAX;
    settings.rpm_min = DEFAULT_SPINDLE_RPM_MIN;

    settings.homing_dir_mask = DEFAULT_HOMING_DIR_MASK;
    settings.homing_feed_rate = DEFAULT_HOMING_FEED_RATE;
    settings.homing_seek_rate = DEFAULT_HOMING_SEEK_RATE;
    settings.homing_debounce_delay = DEFAULT_HOMING_DEBOUNCE_DELAY;
    settings.homing_pulloff = DEFAULT_HOMING_PULLOFF;

    settings.flags = 0;
    if (DEFAULT_REPORT_INCHES) { settings.flags |= BITFLAG_REPORT_INCHES; }
    if (DEFAULT_LASER_MODE) { settings.flags |= BITFLAG_LASER_MODE; }
    if (DEFAULT_INVERT_ST_ENABLE) { settings.flags |= BITFLAG_INVERT_ST_ENABLE; }
    if (DEFAULT_HARD_LIMIT_ENABLE) { settings.flags |= BITFLAG_HARD_LIMIT_ENABLE; }
    if (DEFAULT_HOMING_ENABLE) { settings.flags |= BITFLAG_HOMING_ENABLE; }
    if (DEFAULT_SOFT_LIMIT_ENABLE) { settings.flags |= BITFLAG_SOFT_LIMIT_ENABLE; }
    if (DEFAULT_INVERT_LIMIT_PINS) { settings.flags |= BITFLAG_INVERT_LIMIT_PINS; }
    if (DEFAULT_INVERT_PROBE_PIN) { settings.flags |= BITFLAG_INVERT_PROBE_PIN; }

    settings.steps_per_mm[X_AXIS] = DEFAULT_X_STEPS_PER_MM;
    settings.steps_per_mm[Y_AXIS] = DEFAULT_Y_STEPS_PER_MM;
    settings.steps_per_mm[Z_AXIS] = DEFAULT_Z_STEPS_PER_MM;
    settings.steps_per_mm[A_AXIS] = DEFAULT_A_STEPS_PER_MM;

    settings.max_rate[X_AXIS] = DEFAULT_X_MAX_RATE;
    settings.max_rate[Y_AXIS] = DEFAULT_Y_MAX_RATE;
    settings.max_rate[Z_AXIS] = DEFAULT_Z_MAX_RATE;
    settings.max_rate[A_AXIS] = DEFAULT_A_MAX_RATE;


    settings.acceleration[X_AXIS] = DEFAULT_X_ACCELERATION;
    settings.acceleration[Y_AXIS] = DEFAULT_Y_ACCELERATION;
    settings.acceleration[Z_AXIS] = DEFAULT_Z_ACCELERATION;
    settings.acceleration[A_AXIS] = DEFAULT_A_ACCELERATION;

    settings.max_travel[X_AXIS] = (-DEFAULT_X_MAX_TRAVEL);
    settings.max_travel[Y_AXIS] = (-DEFAULT_Y_MAX_TRAVEL);
    settings.max_travel[Z_AXIS] = (-DEFAULT_Z_MAX_TRAVEL);
    settings.max_travel[A_AXIS] = (-DEFAULT_A_MAX_TRAVEL);


    write_global_settings();
  }

  if (restore_flag & SETTINGS_RESTORE_PARAMETERS) {
    uint8_t idx;
    float coord_data[N_AXIS];
    memset(&coord_data, 0, sizeof(coord_data));
    for (idx=0; idx <= SETTING_INDEX_NCOORD; idx++) { settings_write_coord_data(idx, coord_data); }
  }

  if (restore_flag & SETTINGS_RESTORE_STARTUP_LINES) {
    #if N_STARTUP_LINE > 0
      eeprom_put_char(EEPROM_ADDR_STARTUP_BLOCK, 0);
      eeprom_put_char(EEPROM_ADDR_STARTUP_BLOCK+1, 0); // Checksum
    #endif
    #if N_STARTUP_LINE > 1
      eeprom_put_char(EEPROM_ADDR_STARTUP_BLOCK+(LINE_BUFFER_SIZE+1), 0);
      eeprom_put_char(EEPROM_ADDR_STARTUP_BLOCK+(LINE_BUFFER_SIZE+2), 0); // Checksum
    #endif
  }

  if (restore_flag & SETTINGS_RESTORE_BUILD_INFO) {
    eeprom_put_char(EEPROM_ADDR_BUILD_INFO , 0);
    eeprom_put_char(EEPROM_ADDR_BUILD_INFO+1 , 0); // Checksum
  }
}


// Reads startup line from EEPROM. Updated pointed line string data.
uint8_t settings_read_startup_line(uint8_t n, char *line)
{
  uint32_t addr = n*(LINE_BUFFER_SIZE+1)+EEPROM_ADDR_STARTUP_BLOCK;
  if (!(memcpy_from_eeprom_with_checksum((char*)line, addr, LINE_BUFFER_SIZE))) {
    // Reset line with default value
    line[0] = 0; // Empty line
    settings_store_startup_line(n, line);
    return(false);
  }
  return(true);
}


// Reads startup line from EEPROM. Updated pointed line string data.
uint8_t settings_read_build_info(char *line)
{
  if (!(memcpy_from_eeprom_with_checksum((char*)line, EEPROM_ADDR_BUILD_INFO, LINE_BUFFER_SIZE))) {
    // Reset line with default value
    line[0] = 0; // Empty line
    settings_store_build_info(line);
    return(false);
  }
  return(true);
}


// Read selected coordinate data from EEPROM. Updates pointed coord_data value.
uint8_t settings_read_coord_data(uint8_t coord_select, float *coord_data)
{
  //uint32_t addr = coord_select*(sizeof(float)*N_AXIS+1) + EEPROM_ADDR_PARAMETERS;
	switch (coord_select) {
		case SETTING_INDEX_G54 :
						memcpy((char*)coord_data, settings.g54, sizeof(float)*N_AXIS);
					break;
		case SETTING_INDEX_G55 :
						memcpy((char*)coord_data, settings.g55, sizeof(float)*N_AXIS);
					break;
		case SETTING_INDEX_G56 :
						memcpy((char*)coord_data, settings.g56, sizeof(float)*N_AXIS);
					break;
		case SETTING_INDEX_G57 :
						memcpy((char*)coord_data, settings.g57, sizeof(float)*N_AXIS);
					break;
		case SETTING_INDEX_G58 :
						memcpy((char*)coord_data, settings.g58, sizeof(float)*N_AXIS);
					break;
		case SETTING_INDEX_G59 :
						memcpy((char*)coord_data, settings.g59, sizeof(float)*N_AXIS);
					break;
		case SETTING_INDEX_G28 :
						memcpy((char*)coord_data, settings.g28, sizeof(float)*N_AXIS);
					break;
		case SETTING_INDEX_G30 :
						memcpy((char*)coord_data, settings.g30, sizeof(float)*N_AXIS);
					break;
	}
/*
  if (!(memcpy_from_eeprom_with_checksum((char*)coord_data, addr, sizeof(float)*N_AXIS))) {
    // Reset with default zero vector
		coord_data[X_AXIS] = 0.0f;
		coord_data[Y_AXIS] = 0.0f;
		coord_data[Z_AXIS] = 0.0f;
		coord_data[U_AXIS] = 0.0f;
		coord_data[A_AXIS] = 0.0f;
		coord_data[B_AXIS] = 0.0f;
		settings_write_coord_data(coord_select,coord_data);
    return(false);
  }*/
  return(true);
}


// Reads Grbl global settings struct from EEPROM.
uint8_t read_global_settings() {
  // Check version-byte of eeprom
  uint8_t version = eeprom_get_char(0);
  if (version == SETTINGS_VERSION) {
    // Read settings-record and check checksum
    if (!(memcpy_from_eeprom_with_checksum((char*)&settings, EEPROM_ADDR_GLOBAL, sizeof(settings_t)))) {
      return(false);
    }
  } else {
    return(false);
  }
  return(true);
}

uint8_t settings_print_global_setting(uint32_t index)
{
	cfgItem_t *nv;

	if (index > MAX_PARAM_SETTINGS) return STATUS_SETTING_DISABLED;
	nv = &cfgArray[index];
	if (nv->controller == 0) {
		text_print(nv);
	}
	return(STATUS_OK);
}

uint8_t settings_print_help(uint32_t index)
{
	cfgItem_t *nv;

	if (index > MAX_PARAM_SETTINGS) return STATUS_SETTING_DISABLED;
	nv = &cfgArray[index];
	text_print_help(nv);
	return(STATUS_OK);
}


// A helper method to set settings from command line
uint8_t settings_store_global_setting(uint32_t parameter, float value) {
  uint8_t set_idx = 0;
  if (value < 0.0f) { return(STATUS_NEGATIVE_VALUE); }

  if ((parameter >= AXIS_SETTINGS_START_VAL) && (parameter < AXIS_SETTINGS_START_COORD_OFFSET) ) {
    // Store axis configuration. Axis numbering sequence set by AXIS_SETTING defines.
    // NOTE: Ensure the setting index corresponds to the report.c settings printout.
    parameter -= AXIS_SETTINGS_START_VAL;

    while (set_idx < AXIS_N_SETTINGS) {
      if (parameter < N_AXIS) {
        // Valid axis setting found.
        switch (set_idx) {
          case 0:
            #ifdef MAX_STEP_RATE_HZ
            if (value*settings.max_rate[parameter] >(MAX_STEP_RATE_HZ*60.0f)) { return(STATUS_MAX_STEP_RATE_EXCEEDED); }
            #endif
            settings.steps_per_mm[parameter] = value;
            break;
          case 1:
            #ifdef MAX_STEP_RATE_HZ
            if (value*settings.steps_per_mm[parameter] > (MAX_STEP_RATE_HZ*60.0f)) { return(STATUS_MAX_STEP_RATE_EXCEEDED); }
            #endif
            settings.max_rate[parameter] = value;
            break;
          case 2: settings.acceleration[parameter] = value*60*60; break; // Convert to mm/min^2 for grbl internal use.
          case 3: settings.max_travel[parameter] = -value; break;  // Store as negative for grbl internal use.
        }
        break; // Exit while-loop after setting has been configured and proceed to the EEPROM write call.
      } else {
        set_idx++;
        // If axis index greater than N_AXIS or setting index greater than number of axis settings, error out.
        if ((parameter < AXIS_SETTINGS_INCREMENT) || (set_idx == AXIS_N_SETTINGS)) { return(STATUS_INVALID_STATEMENT); }
        parameter -= AXIS_SETTINGS_INCREMENT;
      }
    }
  } else
  if (parameter >= AXIS_SETTINGS_START_COORD_OFFSET) {
	  set_idx = 0;
	  parameter -= AXIS_SETTINGS_START_COORD_OFFSET;
	  while (set_idx < AXIS_N_COORD_OFFSET) {
	       if (parameter < N_AXIS) {
	    	   switch (set_idx) {
	    	            case 0: settings.g54[parameter] = value;
	    	            	break;
	    	            case 1: settings.g55[parameter] = value;
	    	            	break;
	    	            case 2: settings.g56[parameter] = value;
	    	            	break;
	    	            case 3: settings.g57[parameter] = value;
	    	            	break;
	    	            case 4: settings.g58[parameter] = value;
	    	            	break;
	    	            case 5: settings.g59[parameter] = value;
	    	            	break;
	    	            case 6: settings.g28[parameter] = value;
	    	            	break;
	    	            case 7: settings.g30[parameter] = value;
	    	            	break;
	    	   }
	    	   break;
	       }
	       else {
	    	   set_idx++;
	    	   // If axis index greater than N_AXIS or setting index greater than number of axis settings, error out.
	    	   if ((parameter < AXIS_SETTINGS_INCREMENT) || (set_idx == AXIS_N_COORD_OFFSET)) { return(STATUS_INVALID_STATEMENT); }
	    	   parameter -= AXIS_SETTINGS_INCREMENT;
	       }
	  }
  }
  else {
    // Store non-axis Grbl settings
    uint32_t int_value = truncf(value);
    switch(parameter) {
      case 0:
        if (int_value < 1) {
        	settings.pulse_microseconds = DEFAULT_STEP_PULSE_MICROSECONDS;
        	return(STATUS_SETTING_STEP_PULSE_MIN);
        }
        settings.pulse_microseconds = int_value; break;
      case 1: settings.stepper_idle_lock_time = int_value; break;
      case 2:
        settings.step_invert_mask = int_value;
        st_generate_step_dir_invert_masks(); // Regenerate step and direction port invert masks.
        break;
      case 3:
        settings.dir_invert_mask = int_value;
        st_generate_step_dir_invert_masks(); // Regenerate step and direction port invert masks.
        break;
      case 4: // Reset to ensure change. Immediate re-init may cause problems.
        if (int_value) { settings.flags |= BITFLAG_INVERT_ST_ENABLE; }
        else { settings.flags &= ~BITFLAG_INVERT_ST_ENABLE; }
        break;
      case 5: // Reset to ensure change. Immediate re-init may cause problems.
        if (int_value) { settings.flags |= BITFLAG_INVERT_LIMIT_PINS; }
        else { settings.flags &= ~BITFLAG_INVERT_LIMIT_PINS; }
        break;
      case 6: // Reset to ensure change. Immediate re-init may cause problems.
        if (int_value) { settings.flags |= BITFLAG_INVERT_PROBE_PIN; }
        else { settings.flags &= ~BITFLAG_INVERT_PROBE_PIN; }
        probe_configure_invert_mask(false);
        break;
      case 10: settings.status_report_mask = int_value;
      	  	  if (settings.status_report_mask & BITFLAG_RT_STATUS_WAIT_END_MOTION) settings.wait_end_motion = 1;
      	  	  else settings.wait_end_motion = 0;
      	  	  break;
      case 11: settings.junction_deviation = value; break;
      case 12: settings.arc_tolerance = value; break;
      case 13:
        if (int_value) { settings.flags |= BITFLAG_REPORT_INCHES; }
        else { settings.flags &= ~BITFLAG_REPORT_INCHES; }
        system_flag_wco_change(); // Make sure WCO is immediately updated.
        break;
      case 20:
        if (int_value) {
          if (bit_isfalse(settings.flags, BITFLAG_HOMING_ENABLE)) { return(STATUS_SOFT_LIMIT_ERROR); }
          settings.flags |= BITFLAG_SOFT_LIMIT_ENABLE;
        } else { settings.flags &= ~BITFLAG_SOFT_LIMIT_ENABLE; }
        break;
      case 21:
        if (int_value) { settings.flags |= BITFLAG_HARD_LIMIT_ENABLE; }
        else { settings.flags &= ~BITFLAG_HARD_LIMIT_ENABLE; }
        limits_init(); // Re-init to immediately change. NOTE: Nice to have but could be problematic later.
        break;
      case 22:
        if (int_value) { settings.flags |= BITFLAG_HOMING_ENABLE; }
        else {
          settings.flags &= ~BITFLAG_HOMING_ENABLE;
          settings.flags &= ~BITFLAG_SOFT_LIMIT_ENABLE; // Force disable soft-limits.
        }
        break;
      case 23: settings.homing_dir_mask = int_value; break;
      case 24: settings.homing_feed_rate = value; break;
      case 25: settings.homing_seek_rate = value; break;
      case 26: settings.homing_debounce_delay = int_value; break;
      case 27: settings.homing_pulloff = value; break;
      case 30: settings.rpm_max = value; spindle_init(); break; // Re-initialize spindle rpm calibration
      case 31: settings.rpm_min = value; spindle_init(); break; // Re-initialize spindle rpm calibration
      case 32:
        #ifdef VARIABLE_SPINDLE
          if (int_value) { settings.flags |= BITFLAG_LASER_MODE; }
          else { settings.flags &= ~BITFLAG_LASER_MODE; }
        #else
				return(STATUS_SETTING_DISABLED_LASER);
        #endif
        break;
      case 50 : gc_state.modal.plane_select = (uint8_t)int_value;break;
      case 51 : gc_state.modal.units = (uint8_t)int_value;break;
      case 52 : gc_state.modal.coord_select = (uint8_t)int_value;break;
      case 53 : gc_state.modal.control = (uint8_t)int_value;break;
      case 54 : gc_state.modal.distance = (uint8_t)int_value;break;
      default:
        return(STATUS_INVALID_STATEMENT);
    }
  }
  write_global_settings();
  return(STATUS_OK);
}


// Initialize the config subsystem
void settings_init() {
  if(!read_global_settings()) {
    report_status_message(STATUS_SETTING_READ_FAIL);
    settings_restore(SETTINGS_RESTORE_ALL); // Force restore all EEPROM data.
    report_grbl_settings();
  }
}



