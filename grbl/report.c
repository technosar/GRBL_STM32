/*
  report.c - reporting and messaging methods
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

/*
  This file functions as the primary feedback interface for Grbl. Any outgoing data, such
  as the protocol status messages, feedback messages, and status reports, are stored here.
  For the most part, these functions primarily are called from protocol.c methods. If a
  different style feedback is desired (i.e. JSON), then a user can change these following
  methods to accomodate their needs.
*/

#include "grbl.h"
#include "usbd_cdc_if.h"

char str_report[2048];

const char alarm_str[10][32] = { "\r\n",
								"Hard limit\r\n",
								"Soft limit\r\n",
								"Abort during cycle\r\n",
								"Probe fail initial\r\n",
								"Probe fail contact\r\n",
								"Homing fail reset\r\n",
								"Homing fail door\r\n",
								"Homing fail pulloff\r\n",
								"Homing fail approach\r\n"
};

// Handles the primary confirmation protocol response for streaming interfaces and human-feedback.
// For every incoming line, this method responds with an 'ok' for a successful command or an
// 'error:'  to indicate some error event with the line or some critical system error during
// operation. Errors events can originate from the g-code parser, settings module, or asynchronously
// from a critical error, such as a triggered hard limit. Interface should always monitor for these
// responses.
void report_status_message(uint8_t status_code)
{
  switch(status_code) {
    case STATUS_OK: // STATUS_OK
      sprintf(str_report,"ok\r\n");
      CDC_send_str(str_report, strlen(str_report));
      break;
    default:
      sprintf(str_report,"error:%d\r\n", status_code);
      CDC_send_str(str_report, strlen(str_report));
  }
}


// Prints alarm messages.
void report_alarm_message(uint8_t alarm_code)
{
	sprintf(str_report,"ALARM:%d %s\r\n", alarm_code, alarm_str[alarm_code]);
	CDC_send_str(str_report, strlen(str_report));
}

// Prints feedback messages. This serves as a centralized method to provide additional
// user feedback for things that are not of the status/alarm message protocol. These are
// messages such as setup warnings, switch toggling, and how to exit alarms.
// NOTE: For interfaces, messages are always placed within brackets. And if silent mode
// is installed, the message number codes are less than zero.
void report_feedback_message(uint8_t message_code)
{
	  switch(message_code) {
	    case MESSAGE_CRITICAL_EVENT:
	    	sprintf(str_report,"[MSG:Reset to continue]\r\n");
	    	break;
	    case MESSAGE_ALARM_LOCK:
	    	sprintf(str_report,"[MSG:'$H'|'$X' to unlock]\r\n");
	      break;
	    case MESSAGE_ALARM_UNLOCK:
	    	sprintf(str_report,"[MSG:Caution: Unlocked]\r\n");
	      break;
	    case MESSAGE_ENABLED:
	    	sprintf(str_report,"[MSG:Enabled]\r\n");
	      break;
	    case MESSAGE_DISABLED:
	    	sprintf(str_report,"[MSG:Disabled]\r\n");
	      break;
	    case MESSAGE_SAFETY_DOOR_AJAR:
	    	sprintf(str_report,"[MSG:Check Door]\r\n");
	      break;
	    case MESSAGE_CHECK_LIMITS:
	    	sprintf(str_report,"[MSG:Check Limits]\r\n");
	      break;
	    case MESSAGE_PROGRAM_END:
	    	sprintf(str_report,"[MSG:Pgm End]\r\n");
	      break;
	    case MESSAGE_RESTORE_DEFAULTS:
	    	sprintf(str_report,"[MSG:Restoring defaults]\r\n");
	      break;
	    case MESSAGE_SPINDLE_RESTORE:
	    	sprintf(str_report,"[MSG:Restoring spindle]\r\n");
	      break;
	    case MESSAGE_SLEEP_MODE:
	    	sprintf(str_report,"[MSG:Sleeping]\r\n");
	      break;
	  }
	  CDC_send_str(str_report, strlen(str_report));
}


// Welcome message
void report_init_message()
{
  sprintf(str_report,"\r\nGrbl %s  ['$' for help]\r\n", GRBL_VERSION);
  CDC_send_str(str_report, strlen(str_report));
}

// Grbl help message
void report_grbl_help() {
	sprintf(str_report,"[HLP:$$ $# $G $I $N $x=val $Nx=line $J=line $SLP $C $X $H ~ ! ? ctrl-x]\r\n");
	CDC_send_str(str_report, strlen(str_report));
}


// Grbl global settings print out.
// NOTE: The numbering scheme here must correlate to storing in settings.c
void report_grbl_settings() {

  // Print Grbl settings.
  sprintf(str_report,"$%d=%d\r\n",                            0, settings.pulse_microseconds);
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",      1, settings.stepper_idle_lock_time);
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",      2, settings.step_invert_mask);
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",      3, settings.dir_invert_mask);
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",      4, bit_istrue(settings.flags, BITFLAG_INVERT_ST_ENABLE));
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",      5, bit_istrue(settings.flags, BITFLAG_INVERT_LIMIT_PINS));
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",      6, bit_istrue(settings.flags, BITFLAG_INVERT_PROBE_PIN));
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",     10, settings.status_report_mask);
  sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  11, settings.junction_deviation);
  sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  12, settings.arc_tolerance);
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",     13, bit_istrue(settings.flags,BITFLAG_REPORT_INCHES));
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",     20, bit_istrue(settings.flags,BITFLAG_SOFT_LIMIT_ENABLE));
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",     21, bit_istrue(settings.flags,BITFLAG_HARD_LIMIT_ENABLE));
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",     22, bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE));
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",     23, settings.homing_dir_mask);
  sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  24, settings.homing_feed_rate);
  sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  24, settings.homing_seek_rate);
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",     26, settings.homing_debounce_delay);
  sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  27, settings.homing_pulloff);
  sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  30, settings.rpm_max);
  sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  31, settings.rpm_min);
#ifdef VARIABLE_SPINDLE
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",     32, bit_istrue(settings.flags,BITFLAG_LASER_MODE));
#else
  sprintf(str_report + strlen(str_report), "$%d=%d\r\n",     32, 0);
#endif

  // Print axis settings
  uint8_t idx, set_idx;
  uint8_t val = AXIS_SETTINGS_START_VAL;
  for (set_idx=0; set_idx<AXIS_N_SETTINGS; set_idx++) {
    for (idx=0; idx<N_AXIS; idx++) {
      switch (set_idx) {
        case 0: sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  val+idx, settings.steps_per_mm[idx]); break;
        case 1: sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  val+idx, settings.max_rate[idx]); break;
        case 2: sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  val+idx, settings.acceleration[idx]/(60.0f*60.0f)); break;
        case 3: sprintf(str_report + strlen(str_report), "$%d=%.3f\r\n",  val+idx, settings.max_travel[idx]); break;
      }
    }
    val += AXIS_SETTINGS_INCREMENT;
  }
  CDC_send_str(str_report, strlen(str_report));
}


// Prints current probe parameters. Upon a probe command, these parameters are updated upon a
// successful probe or upon a failed probe with the G38.3 without errors command (if supported).
// These values are retained until Grbl is power-cycled, whereby they will be re-zeroed.
void report_probe_parameters()
{
  uint8_t idx;
  // Report in terms of machine position.
  float print_position[N_AXIS];
  system_convert_array_steps_to_mpos(print_position, sys_probe_position);

  for (idx=0; idx < N_AXIS; idx++) {
	  if (bit_istrue(settings.flags,BITFLAG_REPORT_INCHES)) {
		  print_position[idx] *= INCH_PER_MM;
	  }
  }
  sprintf(str_report,"[PRB:%.3f,%.3f,%.3f:%d]\r\n", print_position[0], print_position[1], print_position[2], sys.probe_succeeded);
  CDC_send_str(str_report, strlen(str_report));
}


// Prints Grbl NGC parameters (coordinate offsets, probing)
void report_ngc_parameters()
{
  float coord_data[N_AXIS];
  uint8_t coord_select;

  for (coord_select = 0; coord_select <= SETTING_INDEX_NCOORD; coord_select++)
  {
	  if (!(settings_read_coord_data(coord_select,coord_data)))
	  {
		  report_status_message(STATUS_SETTING_READ_FAIL);
	      return;
	  }

	  switch (coord_select)
	  {
	  	  case 6 : sprintf(str_report,"[G28:%.3f,%.3f,%.3f]\r\n", coord_data[0], coord_data[1], coord_data[2]);
	    			break;
	  	  case 7 : sprintf(str_report,"[G30:%.3f,%.3f,%.3f]\r\n", coord_data[0], coord_data[1], coord_data[2]);
	    	    	break;
	  	  default : sprintf(str_report,"[G%d:%.3f,%.3f,%.3f]\r\n", coord_select+54, coord_data[0], coord_data[1], coord_data[2]);
	    		break;
	  }
	  CDC_send_str(str_report, strlen(str_report));
  }

  // Print G92,G92.1 which are not persistent in memory
  sprintf(str_report,"[G92:%.3f,%.3f,%.3f]\r\n", gc_state.coord_offset[0], gc_state.coord_offset[1], gc_state.coord_offset[2]);
  CDC_send_str(str_report, strlen(str_report));

  // Print tool length offset value
  sprintf(str_report,"[TLO:%.3f]\r\n", gc_state.tool_length_offset);
  CDC_send_str(str_report, strlen(str_report));
  report_probe_parameters(); // Print probe parameters. Not persistent in memory.
}


// Print current gcode parser mode state
void report_gcode_modes()
{
  sprintf(str_report ,"[GC:");
  switch (gc_state.modal.motion) {
  	  case MOTION_MODE_SEEK : sprintf(str_report + strlen(str_report),"G0"); break;
  	  case MOTION_MODE_LINEAR : sprintf(str_report + strlen(str_report),"G1"); break;
  	  case MOTION_MODE_CW_ARC : sprintf(str_report + strlen(str_report),"G2"); break;
  	  case MOTION_MODE_CCW_ARC : sprintf(str_report + strlen(str_report),"G3"); break;
  	  case MOTION_MODE_NONE :  sprintf(str_report + strlen(str_report),"G80"); break;
  	  default:
  		  sprintf(str_report,"G38.%d", gc_state.modal.motion - (MOTION_MODE_PROBE_TOWARD-2));
  }

  sprintf(str_report + strlen(str_report)," G%d", gc_state.modal.coord_select+54);
  sprintf(str_report + strlen(str_report)," G%d", gc_state.modal.plane_select+17);
  sprintf(str_report + strlen(str_report)," G%d", 21-gc_state.modal.units);
  sprintf(str_report + strlen(str_report)," G%d", gc_state.modal.distance+90);
  sprintf(str_report + strlen(str_report)," G%d", 94-gc_state.modal.feed_rate);

  if (gc_state.modal.program_flow) {
	  switch (gc_state.modal.program_flow) {
	  	  case PROGRAM_FLOW_PAUSED :
	    	  sprintf(str_report + strlen(str_report)," M0");
	    	  break;
	      // case PROGRAM_FLOW_OPTIONAL_STOP : serial_write('1'); break; // M1 is ignored and not supported.
	      case PROGRAM_FLOW_COMPLETED_M2 :
	      case PROGRAM_FLOW_COMPLETED_M30 :
	        sprintf(str_report + strlen(str_report)," M%d", gc_state.modal.program_flow);
	        break;
	  }
  }

  switch (gc_state.modal.spindle) {
  	  case SPINDLE_ENABLE_CW :
	    	sprintf(str_report + strlen(str_report)," M3");
	    	break;
  	  case SPINDLE_ENABLE_CCW :
	    	sprintf(str_report + strlen(str_report)," M4");
	    	break;
  	  case SPINDLE_DISABLE :
	    	sprintf(str_report + strlen(str_report)," M5");
	    	break;
  }

#ifdef ENABLE_M7
  if (gc_state.modal.coolant) { // Note: Multiple coolant states may be active at the same time.
	  if (gc_state.modal.coolant & PL_COND_FLAG_COOLANT_MIST)
	  {
		  sprintf(str_report + strlen(str_report)," M7");
	  }
	  if (gc_state.modal.coolant & PL_COND_FLAG_COOLANT_FLOOD)
	  {
		  sprintf(str_report + strlen(str_report)," M8");
	  }
  } else {
	  sprintf(str_report + strlen(str_report)," M9");
  }
#else
  if (gc_state.modal.coolant)
  {
	  sprintf(str_report + strlen(str_report)," M8");
  }
  else
  {
	  sprintf(str_report + strlen(str_report)," M9");
  }
#endif

#ifdef ENABLE_PARKING_OVERRIDE_CONTROL
  if (sys.override_ctrl == OVERRIDE_PARKING_MOTION) {
	  sprintf(str_report + strlen(str_report)," M56");
  }
#endif

  sprintf(str_report + strlen(str_report)," T%d", gc_state.tool);

  sprintf(str_report + strlen(str_report)," F%.0f", gc_state.feed_rate);

#ifdef VARIABLE_SPINDLE
  sprintf(str_report + strlen(str_report)," S%.0f", gc_state.spindle_speed);
#endif

  sprintf(str_report + strlen(str_report),"]\r\n");
  CDC_send_str(str_report, strlen(str_report));
}

// Prints specified startup line
void report_startup_line(uint8_t n, char *line)
{
  sprintf(str_report,"$N%d=%s\r\n", n, line);
  CDC_send_str(str_report, strlen(str_report));
}

void report_execute_startup_message(char *line, uint8_t status_code)
{
  sprintf(str_report,">%s:%d\r\n", line, status_code);
  CDC_send_str(str_report, strlen(str_report));
}

// Prints build info line
void report_build_info(char *line)
{
  sprintf(str_report,"[VER: %s.%s:%s]\r\n", GRBL_VERSION, GRBL_VERSION_BUILD, line);
  CDC_send_str(str_report, strlen(str_report));
}


// Prints the character string line Grbl has received from the user, which has been pre-parsed,
// and has been sent into protocol_execute_line() routine to be executed by Grbl.
void report_echo_line_received(char *line)
{
  sprintf(str_report + strlen(str_report),"[echo:%s]\r\n", line);
  CDC_send_str(str_report, strlen(str_report));
}


 // Prints real-time data. This function grabs a real-time snapshot of the stepper subprogram
 // and the actual location of the CNC machine. Users may change the following function to their
 // specific needs, but the desired real-time data report must be as short as possible. This is
 // requires as it minimizes the computational overhead and allows grbl to keep running smoothly,
 // especially during g-code programs with fast, short line segments and high frequency reports (5-20Hz).
void report_realtime_status()
{
  uint8_t idx;
  int32_t current_position[N_AXIS]; // Copy current state of the system position variable
  memcpy(current_position,sys_position,sizeof(sys_position));
  float print_position[N_AXIS];
  system_convert_array_steps_to_mpos(print_position,current_position);

  // Report current machine state and sub-states
  sprintf(str_report, "<");
  switch (sys.state) {
    case STATE_IDLE: sprintf(str_report + strlen(str_report),"Idle"); break;
    case STATE_CYCLE: sprintf(str_report + strlen(str_report),"Run"); break;
    case STATE_HOLD:
      if (!(sys.suspend & SUSPEND_JOG_CANCEL)) {
        if (sys.suspend & SUSPEND_HOLD_COMPLETE) { sprintf(str_report + strlen(str_report),"Hold:0"); } // Ready to resume
        else { sprintf(str_report + strlen(str_report),"Hold:1"); } // Actively holding
        break;
      } // Continues to print jog state during jog cancel.
      break;
    case STATE_JOG: sprintf(str_report + strlen(str_report),"Jog"); break;
    case STATE_HOMING: sprintf(str_report + strlen(str_report),"Home"); break;
    case STATE_ALARM: sprintf(str_report + strlen(str_report),"Alarm"); break;
    case STATE_CHECK_MODE: sprintf(str_report + strlen(str_report),"Check"); break;

    /*- `Hold:0` Hold complete. Ready to resume.
    - `Hold:1` Hold in-progress. Reset will throw an alarm.
    - `Door:0` Door closed. Ready to resume.
    - `Door:1` Machine stopped. Door still ajar. Can't resume until closed.
    - `Door:2` Door opened. Hold (or parking retract) in-progress. Reset will throw an alarm.
    - `Door:3` Door closed and resuming. Restoring from park, if applicable. Reset will throw an alarm.
    */
    case STATE_SAFETY_DOOR:
      if (sys.suspend & SUSPEND_INITIATE_RESTORE) {
    	  sprintf(str_report + strlen(str_report),"Door:3"); // Restoring
      } else {
        if (sys.suspend & SUSPEND_RETRACT_COMPLETE) {
          if (sys.suspend & SUSPEND_SAFETY_DOOR_AJAR) {
        	  sprintf(str_report + strlen(str_report),"Door:1"); // Door ajar
          } else {
        	  sprintf(str_report + strlen(str_report),"Door:0");
          } // Door closed and ready to resume
        } else {
        	sprintf(str_report + strlen(str_report),"Door:2"); // Retracting
        }
      }
      break;
      sprintf(str_report + strlen(str_report),"Sleep");
  }

  float wco[N_AXIS];
  if (bit_isfalse(settings.status_report_mask,BITFLAG_RT_STATUS_POSITION_TYPE) ||
      (sys.report_wco_counter == 0) ) {
    for (idx=0; idx< N_AXIS; idx++) {
      // Apply work coordinate offsets and tool length offset to current position.
      wco[idx] = gc_state.coord_system[idx]+gc_state.coord_offset[idx];
      if (idx == TOOL_LENGTH_OFFSET_AXIS) { wco[idx] += gc_state.tool_length_offset; }
      if (bit_isfalse(settings.status_report_mask,BITFLAG_RT_STATUS_POSITION_TYPE)) {
        print_position[idx] -= wco[idx];
      }
    }
  }

  // Report machine position
  if (bit_istrue(settings.status_report_mask,BITFLAG_RT_STATUS_POSITION_TYPE)) {
	  sprintf(str_report + strlen(str_report),"|MPos:");
  } else {
	  sprintf(str_report + strlen(str_report),"|WPos:");
  }
  sprintf(str_report + strlen(str_report),"%.3f,%.3f,%.3f", print_position[X_AXIS], print_position[Y_AXIS], print_position[Z_AXIS]);


  // Returns planner and serial read buffer states.
  #ifdef REPORT_FIELD_BUFFER_STATE
    if (bit_istrue(settings.status_report_mask,BITFLAG_RT_STATUS_BUFFER_STATE)) {
    	sprintf(str_report + strlen(str_report),"|Bf:%d,%d", (int)plan_get_block_buffer_available(), (int)serial_get_rx_buffer_available());
    }
  #endif

  #ifdef USE_LINE_NUMBERS
    #ifdef REPORT_FIELD_LINE_NUMBERS
      // Report current line number
      plan_block_t * cur_block = plan_get_current_block();
      if (cur_block != NULL) {
        uint32_t ln = cur_block->line_number;
        if (ln > 0) {
        	sprintf(str_report + strlen(str_report),"|Ln:%d", (int)ln);
        }
      }
    #endif
  #endif

  // Report realtime feed speed
  #ifdef REPORT_FIELD_CURRENT_FEED_SPEED
    #ifdef VARIABLE_SPINDLE
      sprintf(str_report + strlen(str_report),"|FS:%.0f,%.0f", st_get_realtime_rate(), sys.spindle_speed);
    #else
      sprintf(str_report + strlen(str_report),"|F:%.0f", st_get_realtime_rate());
    #endif      
  #endif

  #ifdef REPORT_FIELD_PIN_STATE
    uint8_t lim_pin_state = limits_get_state();
    uint8_t ctrl_pin_state = system_control_get_state();
    uint8_t prb_pin_state = probe_get_state();
    if (lim_pin_state | ctrl_pin_state | prb_pin_state) {
    	sprintf(str_report + strlen(str_report),"|Pn:");
      if (prb_pin_state) { sprintf(str_report + strlen(str_report),"P"); }
      if (lim_pin_state) {
        #ifdef ENABLE_DUAL_AXIS
          #if (DUAL_AXIS_SELECT == X_AXIS)
            if (bit_istrue(lim_pin_state,(bit(X_AXIS)|bit(N_AXIS)))) { serial_write('X'); }
            if (bit_istrue(lim_pin_state,bit(Y_AXIS))) { serial_write('Y'); }
          #endif
          #if (DUAL_AXIS_SELECT == Y_AXIS)
            if (bit_istrue(lim_pin_state,bit(X_AXIS))) { serial_write('X'); }
            if (bit_istrue(lim_pin_state,(bit(Y_AXIS)|bit(N_AXIS)))) { serial_write('Y'); }
          #endif
          if (bit_istrue(lim_pin_state,bit(Z_AXIS))) { serial_write('Z'); }
        #else
          if (bit_istrue(lim_pin_state,bit(X_AXIS))) { sprintf(str_report + strlen(str_report),"X"); }
          if (bit_istrue(lim_pin_state,bit(Y_AXIS))) { sprintf(str_report + strlen(str_report),"Y"); }
          if (bit_istrue(lim_pin_state,bit(Z_AXIS))) { sprintf(str_report + strlen(str_report),"Z"); }
        #endif
      }
      if (ctrl_pin_state) {
        #ifdef ENABLE_SAFETY_DOOR_INPUT_PIN
          if (bit_istrue(ctrl_pin_state,CONTROL_PIN_INDEX_SAFETY_DOOR)) { sprintf(str_report + strlen(str_report),"D"); }
        #endif
        if (bit_istrue(ctrl_pin_state,CONTROL_PIN_INDEX_RESET)) { sprintf(str_report + strlen(str_report),"R"); }
        if (bit_istrue(ctrl_pin_state,CONTROL_PIN_INDEX_FEED_HOLD)) { sprintf(str_report + strlen(str_report),"H"); }
        if (bit_istrue(ctrl_pin_state,CONTROL_PIN_INDEX_CYCLE_START)) { sprintf(str_report + strlen(str_report),"S"); }
      }
    }
  #endif

  #ifdef REPORT_FIELD_WORK_COORD_OFFSET
    if (sys.report_wco_counter > 0) { sys.report_wco_counter--; }
    else {
      if (sys.state & (STATE_HOMING | STATE_CYCLE | STATE_HOLD | STATE_JOG | STATE_SAFETY_DOOR)) {
        sys.report_wco_counter = (REPORT_WCO_REFRESH_BUSY_COUNT-1); // Reset counter for slow refresh
      } else { sys.report_wco_counter = (REPORT_WCO_REFRESH_IDLE_COUNT-1); }
      if (sys.report_ovr_counter == 0) { sys.report_ovr_counter = 1; } // Set override on next report.
      sprintf(str_report + strlen(str_report),"|WCO:");
      sprintf(str_report + strlen(str_report),"%.3f,%.3f,%.3f", wco[X_AXIS], wco[Y_AXIS], wco[Z_AXIS]);
    }
  #endif

  #ifdef REPORT_FIELD_OVERRIDES
    if (sys.report_ovr_counter > 0) { sys.report_ovr_counter--; }
    else {
      if (sys.state & (STATE_HOMING | STATE_CYCLE | STATE_HOLD | STATE_JOG | STATE_SAFETY_DOOR)) {
        sys.report_ovr_counter = (REPORT_OVR_REFRESH_BUSY_COUNT-1); // Reset counter for slow refresh
      } else { sys.report_ovr_counter = (REPORT_OVR_REFRESH_IDLE_COUNT-1); }
      sprintf(str_report + strlen(str_report),"|Ov:%d,%d,%d",sys.f_override, sys.r_override, sys.spindle_speed_ovr);

      uint8_t sp_state = spindle_get_state();
      uint8_t cl_state = coolant_get_state();
      if (sp_state || cl_state) {
    	  sprintf(str_report + strlen(str_report),"|A:");
        if (sp_state) { // != SPINDLE_STATE_DISABLE
          #ifdef VARIABLE_SPINDLE 
            #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
              serial_write('S'); // CW
            #else
              if (sp_state == SPINDLE_STATE_CW) { sprintf(str_report + strlen(str_report),"S"); } // CW
              else { sprintf(str_report + strlen(str_report),"C"); } // CCW
            #endif
          #else
            if (sp_state & SPINDLE_STATE_CW) { sprintf(str_report + strlen(str_report),"S"); } // CW
            else { sprintf(str_report + strlen(str_report),"C"); } // CCW
          #endif
        }
        if (cl_state & COOLANT_STATE_FLOOD) { sprintf(str_report + strlen(str_report),"F"); }
        #ifdef ENABLE_M7
          if (cl_state & COOLANT_STATE_MIST) { sprintf(str_report + strlen(str_report),"M"); }
        #endif
      }  
    }
  #endif

    sprintf(str_report + strlen(str_report),">\r\n");
    CDC_send_str(str_report, strlen(str_report));
}


#ifdef DEBUG
  void report_realtime_debug()
  {

  }
#endif
