/*
 * param.h
 *
 *  Created on: 9 nov. 2017
 *      Author: PC
 */

#ifndef PARAM_H_
#define PARAM_H_

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define RS274NGC_OK 0
#define RS274NGC_ERROR -1

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

#define ADC_REFERENCE_VOLTAGE_MV               3300.0f
#define ADC_MAX_OUTPUT_VALUE                   4095.0f
#define TEMP_SENSOR_AVG_SLOPE_MV_PER_CELSIUS      2.5f
#define TEMP_SENSOR_VOLTAGE_MV_AT_25            760.0f


// on-off switch settings
#define OFF 0
#define ON 1

// number of parameters in parameter table
#define RS274NGC_MAX_PARAMETERS  6000

#define PARAM_PLC_BASE           3000
#define PARAM_PLC_BASE_OUPUT     PARAM_PLC_BASE
#define PARAM_PLC_BASE_INPUT     PARAM_PLC_BASE+128

#define PARAM_BASE               5400

#define PARAM_CUR_X              PARAM_BASE+20
#define PARAM_CUR_Y              PARAM_BASE+21
#define PARAM_CUR_Z              PARAM_BASE+22
#define PARAM_CUR_A              PARAM_BASE+23
#define PARAM_CUR_B              PARAM_BASE+24
#define PARAM_CUR_C              PARAM_BASE+25
#define PARAM_CUR_U              PARAM_BASE+26
#define PARAM_CUR_V              PARAM_BASE+27
#define PARAM_CUR_W              PARAM_BASE+28

#define PARAM_CUR_FEED_RATE      PARAM_BASE+100
#define PARAM_CUR_SPINDLE_SPEED  PARAM_BASE+101

#define PARAM_PENDANT_PHASE      PARAM_BASE+140
#define PARAM_PENDANT_REVOLUTION PARAM_BASE+141
#define PARAM_PENDANT_SELECT     PARAM_BASE+142
#define PARAM_PENDANT_FACTOR     PARAM_BASE+143

#define PARAM_CUR_TOOL_NUMBER    PARAM_BASE

#define PARAM_SYSTEM             5700
#define PARAM_FEED_OVERRIDE      PARAM_SYSTEM+10
#define PARAM_RAPID_OVERRIDE     PARAM_SYSTEM+11
#define PARAM_SPINDLE_OVERRIDE   PARAM_SYSTEM+12

//float parameters [RS274NGC_MAX_PARAMETERS];     // system parameters

#define NCE_ARGUMENT_TO_ACOS_OUT_OF_RANGE                    8
#define NCE_ARGUMENT_TO_ASIN_OUT_OF_RANGE                    9
#define NCE_ATTEMPT_TO_DIVIDE_BY_ZERO                       10
#define NCE_ATTEMPT_TO_RAISE_NEGATIVE_TO_NON_INTEGER_POWER  11
#define NCE_BAD_CHARACTER_USED                              12
#define NCE_BAD_NUMBER_FORMAT                               14
#define NCE_BUG_FUNCTION_SHOULD_NOT_HAVE_BEEN_CALLED        32
#define NCE_BUG_UNKNOWN_OPERATION                           37
#define NCE_EQUAL_SIGN_MISSING_IN_PARAMETER_SETTING         77
#define NCE_LEFT_BRACKET_MISSING_AFTER_SLASH_WITH_ATAN      96
#define NCE_LEFT_BRACKET_MISSING_AFTER_UNARY_OPERATION_NAME 97
#define NCE_NEGATIVE_ARGUMENT_TO_SQRT                      121
#define NCE_NO_CHARACTERS_FOUND_IN_READING_REAL_VALUE      133
#define NCE_NO_DIGITS_FOUND_WHERE_REAL_NUMBER_SHOULD_BE    134
#define NCE_NON_INTEGER_VALUE_FOR_INTEGER                  135
#define NCE_PARAMETER_NUMBER_OUT_OF_RANGE                  142
#define NCE_SLASH_MISSING_AFTER_FIRST_ATAN_ARGUMENT        156
#define NCE_SSCANF_FAILED                                  161
#define NCE_UNCLOSED_EXPRESSION                            172
#define NCE_UNKNOWN_OPERATION                              175
#define NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_A         176
#define NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_M         177
#define NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_O         178
#define NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_X         179
#define NCE_UNKNOWN_WORD_STARTING_WITH_A                   180
#define NCE_UNKNOWN_WORD_STARTING_WITH_C                   181
#define NCE_UNKNOWN_WORD_STARTING_WITH_E                   182
#define NCE_UNKNOWN_WORD_STARTING_WITH_F                   183
#define NCE_UNKNOWN_WORD_STARTING_WITH_L                   184
#define NCE_UNKNOWN_WORD_STARTING_WITH_R                   185
#define NCE_UNKNOWN_WORD_STARTING_WITH_S                   186
#define NCE_UNKNOWN_WORD_STARTING_WITH_T                   187
#define NCE_UNKNOWN_WORD_WHERE_UNARY_OPERATION_COULD_BE    188
#define NCE_ZERO_OR_NEGATIVE_ARGUMENT_TO_LN                196

enum {
	// binary operations
	NO_OPERATION=0,
	DIVIDED_BY,
	MODULO,
	POWER,
	TIMES,
	AND2,
	EXCLUSIVE_OR,
	MINUS,
	NON_EXCLUSIVE_OR,
	PLUS,
	RIGHT_BRACKET
};

// unary operations
// These are not enums because the "&" operator is used in
// reading the operation names and is illegal with an enum

#define ABS 1
#define ACOS 2
#define ASIN 3
#define ATAN 4
#define COS 5
#define EXP 6
#define FIX 7
#define FUP 8
#define LN 9
#define ROUND 10
#define SIN 11
#define SQRT 12
#define TAN 13

typedef struct setup_struct {
  float parameters[RS274NGC_MAX_PARAMETERS];     // system parameters
  uint32_t parameters_ext[5];
  int parameter_occurrence;                      // parameter buffer index
  int parameter_numbers[50];                     // parameter number buffer
  float parameter_values[50];                    // parameter value buffer
} setup;

#define ERM(error_code) if (1) {                    \
  return error_code;                                \
  } else

#define CHP(try_this)                               \
  if ((status = (try_this)) != RS274NGC_OK) {       \
     return status;                                 \
  } else

#define CHK(bad, error_code) if (bad) {             \
  return error_code;                                \
  } else

int read_one_item(char * line, int * counter, float * parameters);
int read_operation(char * line, int * counter, int * operation);
int read_integer_value(char * line, int * counter, int * integer_ptr, float * parameters);
int read_parameter_setting(char * line, int * counter, float * parameters, int *id);
int read_real_number(char * line, int * counter, float * float_ptr);
int read_real_value(char * line, int * counter, float * float_ptr, float * parameters);
int read_parameter(char * line, int * counter, float * float_ptr, float * parameters);
int read_real_expression(char * line, int * counter, float * hold2, float * parameters);
int read_rest_bop1(char * line, int * counter, float * value, int * last_operation, float * parameters);
int read_rest_bop2(char * line, int * counter, float * value, int last_operation, float * parameters);
int execute_binary1(float * left, int operation, float * right);
int execute_binary2(float * left, int operation, float * right);
int read_unary(char * line, int * counter, float * double_ptr, float * parameters);
int read_operation_unary(char * line, int * counter, int * operation);
int read_atan(char * line, int * counter, float * double_ptr, float * parameters);
int execute_unary(float * double_ptr, int operation);

#endif /* PARAM_H_ */
