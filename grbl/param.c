/*
 * param.c
 *
 *  Created on: 9 nov. 2017
 *      Author: PC
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "param.h"
#include "report.h"
#include "system.h"
#include "gcode.h"
#include "plc_io.h"

setup _setup;

/****************************************************************************/

/* read_one_item

Returned Value: int
   If a reader function which is called returns an error code, that
   error code is returned.
   If any of the following errors occur, this returns the error code shown.
   Otherwise, it returns RS274NGC_OK.
   1. the first character read is not a known character for starting a
   word: NCE_BAD_CHARACTER_USED

Side effects:
   This function reads one item from a line of RS274/NGC code and inserts
   the information in a block. System parameters may be reset.

Called by: read_items.

When this function is called, the counter is set so that the position
being considered is the first position of a word. The character at
that position must be one known to the system.  In this version those
characters are: a,b,c,d,f,g,h,i,j,k,l,m,n,p,q,r,s,t,x,y,z,(,#.
However, read_items calls read_line_number directly if the first word
begins with n, so no read function is included in the "_readers" array
for the letter n. Thus, if an n word is encountered in the middle of
a line, this function reports NCE_BAD_CHARACTER_USED.

The function looks for a letter or special character and calls a
selected function according to what the letter or character is.  The
selected function will be responsible to consider all the characters
that comprise the remainder of the item, and reset the pointer so that
it points to the next character after the end of the item (which may be
the end of the line or the first character of another item).

After an item is read, the counter is set at the index of the
next unread character. The item data is stored in the block.

It is expected that the format of a comment will have been checked;
this is being done by close_and_downcase. Bad format comments will
have prevented the system from getting this far, so that this function
can assume a close parenthesis will be found when an open parenthesis
has been found, and that comments are not nested.

*/

int read_one_item( /* ARGUMENTS                                      */
 char * line,             /* string: line of RS274/NGC code being processed */
 int * counter,           /* pointer to a counter for position on the line  */
 float * parameters)     /* array of system parameters                     */
{
  int status;
  char letter;
  int index;

  letter = line[*counter];  /* check if in array range */
  CHK(((letter < 0) || (letter > 'Z')), NCE_BAD_CHARACTER_USED);
  CHP(read_parameter_setting(line, counter, parameters, &index));
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_parameter

Returned Value: int
   If read_integer_value returns an error code, this returns that code.
   If any of the following errors occur, this returns the error code shown.
   Otherwise, this returns RS274NGC_OK.
   1. The first character read is not # :
      NCE_BUG_FUNCTION_SHOULD_NOT_HAVE_BEEN_CALLED
   2. The parameter number is out of bounds:
      NCE_PARAMETER_NUMBER_OUT_OF_RANGE

Side effects:
   The value of the given parameter is put into what float_ptr points at.
   The counter is reset to point to the first character after the
   characters which make up the value.

Called by:  read_real_value

This attempts to read the value of a parameter out of the line,
starting at the index given by the counter.

According to the RS274/NGC manual [NCMS, p. 62], the characters following
# may be any "parameter expression". Thus, the following are legal
and mean the same thing (the value of the parameter whose number is
stored in parameter 2):
  ##2
  #[#2]

Parameter setting is done in parallel, not sequentially. For example
if #1 is 5 before the line "#1=10 #2=#1" is read, then after the line
is is executed, #1 is 10 and #2 is 5. If parameter setting were done
sequentially, the value of #2 would be 10 after the line was executed.

*/

int read_parameter( /* ARGUMENTS                                      */
 char * line,              /* string: line of RS274/NGC code being processed */
 int * counter,            /* pointer to a counter for position on the line  */
 float * float_ptr,      /* pointer to float to be read                   */
 float * parameters)      /* array of system parameters                     */
{
  int index;
  int status;

  CHK((line[*counter] != '#'), NCE_BUG_FUNCTION_SHOULD_NOT_HAVE_BEEN_CALLED);
  *counter = (*counter + 1);
  CHP(read_integer_value(line, counter, &index, parameters));
  CHK(((index < 1) || (index >= RS274NGC_MAX_PARAMETERS)),
      NCE_PARAMETER_NUMBER_OUT_OF_RANGE);
  if ((index >= 1000) && (index <= 1004)) {
	  *float_ptr = (float)_setup.parameters_ext[index-1000];
  }
  else *float_ptr = _setup.parameters[index];
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_parameter_setting

Returned Value: int
   If read_real_value or read_integer_value returns an error code,
   this returns that code.
   If any of the following errors occur, this returns the error code shown.
   Otherwise, it returns RS274NGC_OK.
   1. The first character read is not # :
      NCE_BUG_FUNCTION_SHOULD_NOT_HAVE_BEEN_CALLED
   2. The parameter index is out of range: PARAMETER_NUMBER_OUT_OF_RANGE
   3. An equal sign does not follow the parameter expression:
      NCE_EQUAL_SIGN_MISSING_IN_PARAMETER_SETTING

Side effects:
   counter is reset to the character following the end of the parameter
   setting. The parameter whose index follows "#" is set to the
   real value following "=".

Called by: read_one_item

When this function is called, counter is pointing at an item on the
line that starts with the character '#', indicating a parameter
setting when found by read_one_item.  The function reads characters
which tell how to set the parameter.

Any number of parameters may be set on a line. If parameters set early
on the line are used in expressions farther down the line, the
parameters have their old values, not their new values. This is
usually called setting parameters in parallel.

Parameter setting is not clearly described in [NCMS, pp. 51 - 62]: it is
not clear if more than one parameter setting per line is allowed (any
number is OK in this implementation). The characters immediately following
the "#" must constitute a "parameter expression", but it is not clear
what that is. Here we allow any expression as long as it evaluates to
an integer.

Parameters are handled in the interpreter by having a parameter table
and a parameter buffer as part of the machine settings. The parameter
table is passed to the reading functions which need it. The parameter
buffer is used directly by functions that need it. Reading functions
may set parameter values in the parameter buffer. Reading functions
may obtain parameter values; these come from parameter table.

The parameter buffer has three parts: (i) a counter for how many
parameters have been set while reading the current line (ii) an array
of the indexes of parameters that have been set while reading the
current line, and (iii) an array of the values for the parameters that
have been set while reading the current line; the nth value
corresponds to the nth index. Any given index will appear once in the
index number array for each time the parameter with that index is set
on a line. There is no point in setting the same parameter more than
one on a line because only the last setting of that parameter will
take effect.

The syntax recognized by this this function is # followed by an
integer expression (explicit integer or expression evaluating to an
integer) followed by = followed by a real value (number or
expression).

Note that # also starts a bunch of characters which represent a parameter
to be evaluated. That situation is handled by read_parameter.

*/

int read_parameter_setting(  /* ARGUMENTS                        */
 char * line,         /* string: line of RS274/NGC code being processed */
 int * counter,       /* pointer to a counter for position on the line  */
 float * parameters,
 int *id) /* array of system parameters                     */
{
  int index;
  float value;
  int status;

  _setup.parameter_occurrence = 0; /* initialize parameter buffer */

  if (line[*counter] != '#') return RS274NGC_ERROR;
  *counter = (*counter + 1);
  CHP(read_integer_value(line, counter, &index, parameters));

  *id =index;

  if ((index < 1) || (index >= RS274NGC_MAX_PARAMETERS)) return NCE_PARAMETER_NUMBER_OUT_OF_RANGE;
  if (line[*counter] != '=') {
	  if ((index >= 1000) && (index <= 1002)) { // X, Y and Z position
		  report_parameter((unsigned int)index, (float)_setup.parameters_ext[index-1000], TYPE_FLOAT);
	   }
	  else if (index == PARAM_PLC_BASE_INPUT) { // Inpput PROBE state read
		  report_parameter((unsigned int)index, probe_get_state(), TYPE_UINT32);
	  }
	   else report_parameter((unsigned int)index , _setup.parameters[index], TYPE_FLOAT);
  }
  else {
	  *counter = (*counter + 1);
	  CHP(read_real_value(line, counter, &value, parameters));

	  // write digital outputs
  	  if ((index >= PARAM_PLC_BASE_OUPUT) && (index < (PARAM_PLC_BASE_INPUT))) { // up to 128 outputs
  		plc_output_set_state(index - PARAM_PLC_BASE, (unsigned int)value);
  	  }
  	// write to internal memory
  	_setup.parameters[index] = value;
  }
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_integer_value

Returned Value: int
   If read_real_value returns an error code, this returns that code.
   If any of the following errors occur, this returns the error code shown.
   Otherwise, it returns RS274NGC_OK.
   1. The returned value is not close to an integer:
      NCE_NON_INTEGER_VALUE_FOR_INTEGER

Side effects:
   The number read from the line is put into what integer_ptr points at.

Called by:
   read_d
   read_l
   read_h
   read_m
   read_parameter
   read_parameter_setting
   read_t

This reads an integer (positive, negative or zero) from a string,
starting from the position given by *counter. The value being
read may be written with a decimal point or it may be an expression
involving non-integers, as long as the result comes out within 0.0001
of an integer.

This proceeds by calling read_real_value and checking that it is
close to an integer, then returning the integer it is close to.

*/

int read_integer_value(   /* ARGUMENTS                                      */
 char * line,             /* string: line of RS274/NGC code being processed */
 int * counter,           /* pointer to a counter for position on the line  */
 int * integer_ptr,       /* pointer to the value being read                */
 float * parameters)      /* array of system parameters                     */
{
  float float_value;
  int status;

  CHP(read_real_value(line, counter, &float_value, parameters));
  *integer_ptr = (int)floor(float_value);
  if ((float_value - *integer_ptr) > 0.9999)
    {
      *integer_ptr = (int)ceil(float_value);
    }
  else if ((float_value - *integer_ptr) > 0.0001) return NCE_NON_INTEGER_VALUE_FOR_INTEGER;
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_real_value

Returned Value: int
   If one of the following functions returns an error code,
   this returns that code.
      read_real_expression
      read_parameter
      read_unary
      read_real_number
   If no characters are found before the end of the line this
   returns NCE_NO_CHARACTERS_FOUND_IN_READING_REAL_VALUE.
   Otherwise, this returns RS274NGC_OK.

Side effects:
   The value read from the line is put into what float_ptr points at.
   The counter is reset to point to the first character after the
   characters which make up the value.

Called by:
   read_a
   read_b
   read_c
   read_f
   read_g
   read_i
   read_integer_value
   read_j
   read_k
   read_p
   read_parameter_setting
   read_q
   read_r
   read_real_expression
   read_s
   read_x
   read_y
   read_z

This attempts to read a real value out of the line, starting at the
index given by the counter. The value may be a number, a parameter
value, a unary function, or an expression. It calls one of four
other readers, depending upon the first character.

*/

int read_real_value(  /* ARGUMENTS                               */
 char * line,         /* string: line of RS274/NGC code being processed */
 int * counter,       /* pointer to a counter for position on the line  */
 float * float_ptr,   /* pointer to float to be read                    */
 float * parameters)  /* array of system parameters                     */
{
  char c;
  int status;

  c = line[*counter];
  if (c == 0) return NCE_NO_CHARACTERS_FOUND_IN_READING_REAL_VALUE;
  if (c == '[') CHP(read_real_expression (line, counter, float_ptr, parameters));
  else if (c == '#') CHP(read_parameter(line, counter, float_ptr, parameters));
  else if ((c >= 'A') && (c <= 'Z')) CHP(read_unary(line, counter, float_ptr, parameters));
  else
	  CHP(read_real_number(line, counter, float_ptr));

  return RS274NGC_OK;
}

/****************************************************************************/

/* read_real_number

Returned Value: int
   If any of the following errors occur, this returns the error shown.
   Otherwise, it returns RS274NGC_OK.
   1. The first character is not "+", "-", "." or a digit:
      NCE_BAD_NUMBER_FORMAT
   2. No digits are found after the first character and before the
      end of the line or the next character that cannot be part of a real:
      NCE_NO_DIGITS_FOUND_WHERE_REAL_NUMBER_SHOULD_BE
   3. sscanf fails: NCE_SSCANF_FAILED

Side effects:
   The number read from the line is put into what float_ptr points at.
   The counter is reset to point to the first character after the real.

Called by:  read_real_value

This attempts to read a number out of the line, starting at the index
given by the counter. It stops when the first character that cannot
be part of the number is found.

The first character may be a digit, "+", "-", or "."
Every following character must be a digit or "." up to anything
that is not a digit or "." (a second "." terminates reading).

This function is not called if the first character is NULL, so it is
not necessary to check that.

The temporary insertion of a NULL character on the line is to avoid
making a format string like "%3lf" which the LynxOS compiler cannot
handle.

*/

int read_real_number( /* ARGUMENTS                               */
 char * line,         /* string: line of RS274/NGC code being processed */
 int * counter,       /* pointer to a counter for position on the line  */
 float * float_ptr) /* pointer to float to be read                   */
{
  char c;          /* for character being processed    */
  int flag_digit;  /* set to ON if digit found         */
  int flag_point;  /* set to ON if decimal point found */
  int n;           /* for indexing line                */

  n = *counter;
  flag_point = OFF;
  flag_digit = OFF;

/* check first character */
  c = line[n];
  if (c == '+')
    {
      *counter = (*counter + 1); /* skip plus sign */
      n++;
    }
  else if (c == '-')
    {
      n++;
    }
  else if ((c != '.') && ((c < 48) || (c > 57)))
    return NCE_BAD_NUMBER_FORMAT;

/* check out rest of characters (must be digit or decimal point) */
  for (; (c = line[n]) != 0; n++)
    {
      if (( 47 < c) && ( c < 58))
        {
          flag_digit = ON;
        }
      else if (c == '.')
        {
          if (flag_point == OFF)
            {
              flag_point = ON;
            }
          else
            break; /* two decimal points, error appears on reading next item */
        }
      else
        break;
    }

  if (flag_digit == OFF) return NCE_NO_DIGITS_FOUND_WHERE_REAL_NUMBER_SHOULD_BE;
  line[n] = 0; /* temporary string termination for sscanf */
  if (sscanf(line + *counter, "%f", float_ptr) == 0)
    {
      line[n] = c;
      return NCE_SSCANF_FAILED;
    }
  else
    {
      line[n] = c;
      *counter = n;
    }
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_real_expression

Returned Value: int
   If any of the following functions returns an error code,
   this returns that code.
     read_real_value
     read_operation
     execute_binary
   If any of the following errors occur, this returns the error shown.
   Otherwise, it returns RS274NGC_OK.
   1. The first character is not [ :
      NCE_BUG_FUNCTION_SHOULD_NOT_HAVE_BEEN_CALLED

Side effects:
   The number read from the line is put into what value_ptr points at.
   The counter is reset to point to the first character after the real
   expression.

Called by:
 read_atan
 read_real_value
 read_unary

Example 1: [2 - 3 * 4 / 5] means [2 - [[3 * 4] / 5]] and equals -0.4.

Segmenting Expressions -

The RS274/NGC manual, section 3.5.1.1 [NCMS, page 50], provides for
using square brackets to segment expressions.

Binary Operations -

The RS274/NGC manual, section 3.5.1.1, discusses expression evaluation.
The manual provides for eight binary operations: the four basic
mathematical operations (addition, subtraction, multiplication,
division), three logical operations (non-exclusive ||, exclusive ||,
and AND2) and the modulus operation. The manual does not explicitly call
these "binary" operations, but implicitly recognizes that they are
binary. We have added the "power" operation of raising the number
on the left of the operation to the power on the right; this is
needed for many basic machining calculations.

There are two groups of binary operations given in the manual. If
operations are strung together as shown in Example 1, operations in
the first group are to be performed before operations in the second
group. If an expression contains more than one operation from the same
group (such as * and / in Example 1), the operation on the left is
performed first. The first group is: multiplication (*), division (/),
and modulus (MOD). The second group is: addition(+), subtraction (-),
logical non-exclusive || (||), logical exclusive || (XOR), and logical
AND (AND2). We have added a third group with higher precedence than
the first group. The third group contains only the power (**)
operation.

The logical operations and modulus are apparently to be performed on
any real numbers, not just on integers or on some other data type.

Unary Operations -

The RS274/NGC manual, section 3.5.1.2, provides for fifteen unary
mathematical operations. Two of these, BIN and BCD, are apparently for
converting between decimal and hexadecimal number representation,
although the text is not clear. These have not been implemented, since
we are not using any hexadecimal numbers. The other thirteen unary
operations have been implemented: absolute_value, arc_cosine, arc_sine,
arc_tangent, cosine, e_raised_to, fix_down, fix_up, natural_log_of,
round, sine, square_root, tangent.

The manual section 3.5.1.2 [NCMS, page 51] requires the argument to
all unary operations (except atan) to be in square brackets.  Thus,
for example "sin[90]" is allowed in the interpreter, but "sin 90" is
not. The atan operation must be in the format "atan[..]/[..]".

Production Rule Definitions in Terms of Tokens -

The following is a production rule definition of what this RS274NGC
interpreter recognizes as valid combinations of symbols which form a
recognized real_value (the top of this production hierarchy).

The notion of "integer_value" is used in the interpreter. Below it is
defined as a synonym for real_value, but in fact a constraint is added
which cannot be readily written in a production language.  An
integer_value is a real_value which is very close to an integer.
Integer_values are needed for array and table indices and (when
divided by 10) for the values of M codes and G codes. All numbers
(including integers) are read as real numbers and stored as doubles.
If an integer_value is required in some situation, a test for being
close to an integer is applied to the number after it is read.


arc_tangent_combo = arc_tangent expression divided_by expression .

binary_operation1 = divided_by | modulo | power | times .

binary_operation2 = and | exclusive_or | minus |  non_exclusive_or | plus .

combo1 = real_value { binary_operation1 real_value } .

digit = zero | one | two | three | four | five | six | seven |eight | nine .

expression =
   left_bracket
   (unary_combo | (combo1 { binary_operation2 combo1 }))
   right_bracket .

integer_value = real_value .

ordinary_unary_combo =  ordinary_unary_operation expression .

ordinary_unary_operation =
   absolute_value | arc_cosine | arc_sine | cosine | e_raised_to |
   fix_down | fix_up | natural_log_of | round | sine | square_root | tangent .

parameter_index = integer_value .

parameter_value = parameter_sign  parameter_index .

real_number =
   [ plus | minus ]
   (( digit { digit } decimal_point {digit}) | ( decimal_point digit {digit})).

real_value =
   real_number | expression | parameter_value | unary_combo.

unary_combo = ordinary_unary_combo | arc_tangent_combo .


Production Tokens in Terms of Characters -

absolute_value   = 'abs'
and              = 'and'
arc_cosine       = 'acos'
arc_sine         = 'asin'
arc_tangent      = 'atan'
cosine           = 'cos'
decimal_point    = '.'
divided_by       = '/'
eight            = '8'
exclusive_or     = 'xor'
e_raised_to      = 'exp'
five             = '5'
fix_down         = 'fix'
fix_up           = 'fup'
four             = '4'
left_bracket     = '['
minus            = '-'
modulo           = 'mod'
natural_log_of   = 'ln'
nine             = '9'
non_exclusive_or = 'or'
one              = '1'
parameter_sign   = '#'
plus             = '+'
power            = '**'
right_bracket    = ']'
round            = 'round'
seven            = '7'
sine             = 'sin'
six              = '6'
square_root      = 'sqrt'
tangent          = 'tan'
three            = '3'
times            = '*'
two              = '2'
zero             = '0'

When this function is called, the counter should be set at a left
bracket. The function reads up to and including the right bracket
which closes the expression.

The basic form of an expression is: [v1 bop v2 bop ... vn], where the
vi are real_values and the bops are binary operations. The vi may be
numbers, parameters, expressions, or unary functions. Because some
bops are to be evaluated before others, for understanding the order of
evaluation, it is useful to rewrite the general form collecting any
subsequences of bops of the same precedence. For example, suppose the
expression is: [9+8*7/6+5-4*3**2+1]. It may be rewritten as:
[9+[8*7/6]+5-[4*[3**2]]+1] to show how it should be evaluated.

The manual provides that operations of the same precedence should be
processed left to right.

The first version of this function is commented out. It is suitable
for when there are only two precendence levels. It is an improvement
over the version used in interpreters before 2000, but not as general
as the second version given here.

The first version of this function reads the first value and the first
operation in the expression. Then it calls either read_rest_bop1 or
read_rest_bop2 according to whether the first operation is a bop1 or a
bop2.  Read_rest_bop1 resets the next_operation to either a right
bracket or a bop2. If it is reset to a bop2, read_rest_bop2 is called
when read_rest_bop1 returns.

*/

int read_real_expression( /* ARGUMENTS                               */
 char * line,             /* string: line of RS274/NGC code being processed */
 int * counter,           /* pointer to a counter for position on the line  */
 float * value,          /* pointer to double to be read                   */
 float * parameters)     /* array of system parameters                     */
{
  int next_operation;
  int status;

  CHK((line[*counter] != '['), NCE_BUG_FUNCTION_SHOULD_NOT_HAVE_BEEN_CALLED);
  *counter = (*counter + 1);
  CHP(read_real_value(line, counter, value, parameters));
  CHP(read_operation(line, counter, &next_operation));
  if (next_operation == RIGHT_BRACKET) {
	   /* nothing to do */
  }
  else if (next_operation < AND2)  /* next operation is a bop1, times-like */
    {
      CHP(read_rest_bop1(line, counter, value, &next_operation, parameters));
      if (next_operation == RIGHT_BRACKET) {
    	  /* next_operation has been reset */
      }
      else /* next_operation is now a bop2, plus-like */
        CHP(read_rest_bop2(line, counter, value, next_operation, parameters));
    }
  else  /* next operation is a bop2, plus-like */
    CHP(read_rest_bop2(line, counter, value, next_operation, parameters));
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_rest_bop1

Returned Value: int
  If any of the following functions returns an error code,
  this returns that code.
     execute_binary1
     read_real_value
     read_operation
  Otherwise, it returns RS274NGC_OK.

Side effects:
   The value argument is set to the value of the expression.
   The counter is reset to point to the first character after the real
   expression.

Called by:
  read_real_expression
  read_rest_bop2

The value argument has a value in it when this is called. This repeatedly
gets the next_value and the next_operation, performs the last_operation
on the value and the next_value and resets the last_operation to the
next_operation. Observe that both the value and the last_operation
are passed back to the caller.

This is commented out since it is not used in the uncommented version
of read_real_expression. It has been tested.

*/

int read_rest_bop1(/* ARGUMENTS                                      */
 char * line,             /* string: line of RS274/NGC code being processed */
 int * counter,           /* pointer to a counter for position on the line  */
 float * value,          /* pointer to double to be calculated             */
 int * last_operation,    /* last operation read, reset to next operation   */
 float * parameters)     /* array of system parameters                     */
{
  float next_value;
  int next_operation;
  int status;

  for(; ; )
    {
      CHP(read_real_value(line, counter, &next_value, parameters));
      CHP(read_operation(line, counter, &next_operation));
      CHP(execute_binary1(value, *last_operation, &next_value));
      *last_operation = next_operation;
      if (next_operation >= AND2) /* next op is a bop2 or right bracket */
        break;
    }
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_rest_bop2

Returned Value: int
  If any of the following functions returns an error code,
  this returns that code.
     execute_binary2
     read_real_value
     read_operation
     read_rest_bop1
  Otherwise, it returns RS274NGC_OK.

Side effects:
   The value argument is set to the value of the expression.
   The counter is reset to point to the first character after the real
   expression.

Called by:  read_real_expression

The value argument has a value in it when this is called. This repeatedly
gets the next_value and the next_operation, performs the last_operation
on the value and the next_value and resets the last_operation to the
next_operation. If the next_operation is ever a bop1 read_rest_bop1 is
called to set the next_value.

This is commented out since it is not used in the uncommented version
of read_real_expression. It has been tested.

*/

int read_rest_bop2(/* ARGUMENTS                                      */
 char * line,             /* string: line of RS274/NGC code being processed */
 int * counter,           /* pointer to a counter for position on the line  */
 float * value,          /* pointer to double to be calculated             */
 int last_operation,      /* last operation read                            */
 float * parameters)     /* array of system parameters                     */
{
  float next_value;
  int next_operation;
  int status;

  for(; ; last_operation = next_operation)
    {
      CHP(read_real_value(line, counter, &next_value, parameters));
      CHP(read_operation(line, counter, &next_operation));
      if (next_operation < AND2)  /* next operation is a bop1 */
        {
          CHP(read_rest_bop1(line, counter, &next_value,
                             &next_operation, parameters));
        }
      CHP(execute_binary2(value, last_operation, &next_value));
      if (next_operation == RIGHT_BRACKET)
        break;
    }
  return RS274NGC_OK;
}

/****************************************************************************/

/* execute_binary1

Returned Value: int
   If any of the following errors occur, this returns the error shown.
   Otherwise, it returns RS274NGC_OK.
   1. operation is unknown: NCE_BUG_UNKNOWN_OPERATION
   2. An attempt is made to divide by zero: NCE_ATTEMPT_TO_DIVIDE_BY_ZERO
   3. An attempt is made to raise a negative number to a non-integer power:
      NCE_ATTEMPT_TO_RAISE_NEGATIVE_TO_NON_INTEGER_POWER

Side effects:
   The result from performing the operation is put into what left points at.

Called by: read_real_expression.

This executes the operations: DIVIDED_BY, MODULO, POWER, TIMES.

*/

int execute_binary1( /* ARGUMENTS                       */
 float * left,             /* pointer to the left operand     */
 int operation,             /* integer code for the operation  */
 float * right)            /* pointer to the right operand    */
{
  switch (operation)
    {
    case DIVIDED_BY:
      CHK((*right == 0.0), NCE_ATTEMPT_TO_DIVIDE_BY_ZERO);
      *left = (*left / *right);
      break;
    case MODULO: /* always calculates a positive answer */
      *left = fmod(*left, *right);
      if (*left < 0.0)
        {
          *left = (*left + fabs(*right));
        }
      break;
    case POWER:
      CHK(((*left < 0.0) && (floor(*right) != *right)),
          NCE_ATTEMPT_TO_RAISE_NEGATIVE_TO_NON_INTEGER_POWER);
      *left = pow(*left, *right);
      break;
    case TIMES:
      *left = (*left * *right);
      break;
    default:
      ERM(NCE_BUG_UNKNOWN_OPERATION);
    }
  return RS274NGC_OK;
}

/****************************************************************************/

/* execute_binary2

Returned Value: int
   If any of the following errors occur, this returns the error code shown.
   Otherwise, it returns RS274NGC_OK.
   1. operation is unknown: NCE_BUG_UNKNOWN_OPERATION

Side effects:
   The result from performing the operation is put into what left points at.

Called by: read_real_expression.

This executes the operations: AND2, EXCLUSIVE_OR, MINUS,
NON_EXCLUSIVE_OR, PLUS. The RS274/NGC manual [NCMS] does not say what
the calculated value of the three logical operations should be. This
function calculates either 1.0 (meaning true) or 0.0 (meaning false).
Any non-zero input value is taken as meaning true, and only 0.0 means
false.


*/

int execute_binary2( /* ARGUMENTS                       */
 float * left,             /* pointer to the left operand     */
 int operation,             /* integer code for the operation  */
 float * right)            /* pointer to the right operand    */
{
  switch (operation)
    {
    case AND2:
      *left = ((*left == 0.0) || (*right == 0.0)) ? 0.0 : 1.0;
      break;
    case EXCLUSIVE_OR:
      *left = (((*left == 0.0) && (*right != 0.0)) ||
                    ((*left != 0.0) && (*right == 0.0))) ? 1.0 : 0.0;
      break;
    case MINUS:
      *left = (*left - *right);
      break;
    case NON_EXCLUSIVE_OR:
      *left = ((*left != 0.0) || (*right != 0.0)) ? 1.0 : 0.0;
      break;
    case PLUS:
      *left = (*left + *right);
      break;
    default:
      ERM(NCE_BUG_UNKNOWN_OPERATION);
    }
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_unary

Returned Value: int
   If any of the following functions returns an error code,
   this returns that code.
     execute_unary
     read_atan
     read_operation_unary
     read_real_expression
   If any of the following errors occur, this returns the error code shown.
   Otherwise, it returns RS274NGC_OK.
   1. the name of the unary operation is not followed by a left bracket:
      NCE_LEFT_BRACKET_MISSING_AFTER_UNARY_OPERATION_NAME

Side effects:
   The value read from the line is put into what double_ptr points at.
   The counter is reset to point to the first character after the
   characters which make up the value.

Called by:  read_real_value

This attempts to read the value of a unary operation out of the line,
starting at the index given by the counter. The atan operation is
handled specially because it is followed by two arguments.

*/

int read_unary( /* ARGUMENTS                                      */
 char * line,          /* string: line of RS274/NGC code being processed */
 int * counter,        /* pointer to a counter for position on the line  */
 float * double_ptr,  /* pointer to double to be read                   */
 float * parameters)  /* array of system parameters                     */
{
  int operation;
  int status;

  CHP(read_operation_unary (line, counter, &operation));
  CHK((line[*counter] != '['),
      NCE_LEFT_BRACKET_MISSING_AFTER_UNARY_OPERATION_NAME);
  CHP(read_real_expression (line, counter, double_ptr, parameters));

  if (operation == ATAN)
    CHP(read_atan(line, counter, double_ptr, parameters));
  else
    CHP(execute_unary(double_ptr, operation));
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_operation_unary

Returned Value: int
   If the operation is not a known unary operation, this returns one of
   the following error codes:
   NCE_UNKNOWN_WORD_STARTING_WITH_A
   NCE_UNKNOWN_WORD_STARTING_WITH_C
   NCE_UNKNOWN_WORD_STARTING_WITH_E
   NCE_UNKNOWN_WORD_STARTING_WITH_F
   NCE_UNKNOWN_WORD_STARTING_WITH_L
   NCE_UNKNOWN_WORD_STARTING_WITH_R
   NCE_UNKNOWN_WORD_STARTING_WITH_S
   NCE_UNKNOWN_WORD_STARTING_WITH_T
   NCE_UNKNOWN_WORD_WHERE_UNARY_OPERATION_COULD_BE
   Otherwise, this returns RS274NGC_OK.

Side effects:
   An integer code for the name of the operation read from the
   line is put into what operation points at.
   The counter is reset to point to the first character after the
   characters which make up the operation name.

Called by:
   read_unary

This attempts to read the name of a unary operation out of the line,
starting at the index given by the counter. Known operations are:
abs, acos, asin, atan, cos, exp, fix, fup, ln, round, sin, sqrt, tan.

*/

int read_operation_unary( /* ARGUMENTS                               */
 char * line,             /* string: line of RS274/NGC code being processed */
 int * counter,           /* pointer to a counter for position on the line  */
 int * operation)         /* pointer to operation to be read                */
{
  char c;

  c = line[*counter];
  *counter = (*counter + 1);
  switch (c)
    {
    case 'A':
      if((line[*counter] == 'B') && (line[(*counter)+1] == 'S'))
        {
          *operation = ABS;
          *counter = (*counter + 2);
        }
      else if(strncmp((line + *counter), "COS", 3) == 0)
        {
          *operation = ACOS;
          *counter = (*counter + 3);
        }
      else if(strncmp((line + *counter), "SIN", 3) == 0)
        {
          *operation = ASIN;
          *counter = (*counter + 3);
        }
      else if(strncmp((line + *counter), "TAN", 3) == 0)
        {
          *operation = ATAN;
          *counter = (*counter + 3);
        }
      else
        ERM(NCE_UNKNOWN_WORD_STARTING_WITH_A);
      break;
    case 'C':
      if((line[*counter] == 'O') && (line[(*counter)+1] == 'S'))
        {
          *operation = COS;
          *counter = (*counter + 2);
        }
      else
        ERM(NCE_UNKNOWN_WORD_STARTING_WITH_C);
      break;
    case 'E':
      if((line[*counter] == 'X') && (line[(*counter)+1] == 'P'))
        {
          *operation = EXP;
          *counter = (*counter + 2);
        }
      else
        ERM(NCE_UNKNOWN_WORD_STARTING_WITH_E);
      break;
    case 'F':
      if((line[*counter] == 'I') && (line[(*counter)+1] == 'X'))
        {
          *operation = FIX;
          *counter = (*counter + 2);
        }
      else if((line[*counter] == 'U') && (line[(*counter)+1] == 'P'))
        {
          *operation = FUP;
          *counter = (*counter + 2);
        }
      else
        ERM(NCE_UNKNOWN_WORD_STARTING_WITH_F);
      break;
    case 'L':
      if(line[*counter] == 'N')
        {
          *operation = LN;
          *counter = (*counter + 1);
        }
      else
        ERM(NCE_UNKNOWN_WORD_STARTING_WITH_L);
      break;
    case 'R':
      if(strncmp((line + *counter), "OUND", 4) == 0)
        {
          *operation = ROUND;
          *counter = (*counter + 4);
        }
      else
        ERM(NCE_UNKNOWN_WORD_STARTING_WITH_R);
      break;
    case 'S':
      if((line[*counter] == 'I') && (line[(*counter)+1] == 'N'))
        {
          *operation = SIN;
          *counter = (*counter + 2);
        }
      else if(strncmp((line + *counter), "QRT", 3) == 0)
        {
          *operation = SQRT;
          *counter = (*counter + 3);
        }
      else
        ERM(NCE_UNKNOWN_WORD_STARTING_WITH_S);
      break;
    case 'T':
      if((line[*counter] == 'A') && (line[(*counter)+1] == 'N'))
        {
          *operation = TAN;
          *counter = (*counter + 2);
        }
      else
        ERM(NCE_UNKNOWN_WORD_STARTING_WITH_T);
      break;
    default:
      ERM(NCE_UNKNOWN_WORD_WHERE_UNARY_OPERATION_COULD_BE);
    }
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_operation

Returned Value: int
   If any of the following errors occur, this returns the error code shown.
   Otherwise, it returns RS274NGC_OK.
   1. The operation is unknown:
      NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_A
      NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_M
      NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_O
      NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_X
      NCE_UNKNOWN_OPERATION
   2. The line ends without closing the expression: NCE_UNCLOSED_EXPRESSION

Side effects:
   An integer representing the operation is put into what operation points
   at.  The counter is reset to point to the first character after the
   operation.

Called by: read_real_expression

This expects to be reading a binary operation (+, -, /, *, **, and,
mod, or, xor) or a right bracket (]). If one of these is found, the
value of operation is set to the symbolic value for that operation.
If not, an error is reported as described above.

*/

int read_operation( /* ARGUMENTS                                      */
 char * line,              /* string: line of RS274/NGC code being processed */
 int * counter,            /* pointer to a counter for position on the line  */
 int * operation)          /* pointer to operation to be read                */
{
  char c;

  c = line[*counter];
  *counter = (*counter + 1);
  switch(c)
    {
    case '+':
      *operation = PLUS;
      break;
    case '-':
      *operation = MINUS;
      break;
    case '/':
      *operation = DIVIDED_BY;
      break;
    case '*':
      if(line[*counter] == '*')
        {
          *operation = POWER;
          *counter = (*counter + 1);
        }
      else
        *operation = TIMES;
      break;
    case ']':
      *operation = RIGHT_BRACKET;
      break;
    case 'A':
      if((line[*counter] == 'N') && (line[(*counter)+1] == 'D'))
        {
          *operation = AND2;
          *counter = (*counter + 2);
        }
      else
        ERM(NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_A);
      break;
    case 'M':
      if((line[*counter] == 'O') && (line[(*counter)+1] == 'D'))
        {
          *operation = MODULO;
          *counter = (*counter + 2);
        }
      else
        ERM(NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_M);
      break;
    case 'O':
      if(line[*counter] == 'R')
        {
          *operation = NON_EXCLUSIVE_OR;
          *counter = (*counter + 1);
        }
      else
        ERM(NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_O);
      break;
    case 'X':
      if((line[*counter] == 'O') && (line[(*counter)+1] == 'R'))
        {
          *operation = EXCLUSIVE_OR;
          *counter = (*counter + 2);
        }
      else
        ERM(NCE_UNKNOWN_OPERATION_NAME_STARTING_WITH_X);
      break;
    case 0:
      ERM(NCE_UNCLOSED_EXPRESSION);
      break;
    default:
      ERM(NCE_UNKNOWN_OPERATION);
    }
  return RS274NGC_OK;
}

/****************************************************************************/

/* read_atan

Returned Value: int
   If read_real_expression returns an error code, this returns that code.
   If any of the following errors occur, this returns the error code shown.
   Otherwise, it returns RS274NGC_OK.
   1. The first character to read is not a slash:
      NCE_SLASH_MISSING_AFTER_FIRST_ATAN_ARGUMENT
   2. The second character to read is not a left bracket:
      NCE_LEFT_BRACKET_MISSING_AFTER_SLASH_WITH_ATAN

Side effects:
   The computed value is put into what double_ptr points at.
   The counter is reset to point to the first character after the
   characters which make up the value.

Called by:
   read_unary

When this function is called, the characters "atan" and the first
argument have already been read, and the value of the first argument
is stored in double_ptr.  This function attempts to read a slash and
the second argument to the atan function, starting at the index given
by the counter and then to compute the value of the atan operation
applied to the two arguments.  The computed value is inserted into
what double_ptr points at.

The computed value is in the range from -180 degrees to +180 degrees.
The range is not specified in the RS274/NGC manual [NCMS, page 51],
although using degrees (not radians) is specified.

*/

int read_atan( /* ARGUMENTS                                      */
 char * line,         /* string: line of RS274/NGC code being processed */
 int * counter,       /* pointer to a counter for position on line      */
 float * double_ptr, /* pointer to double to be read                   */
 float * parameters) /* array of system parameters                     */
{
  float argument2;
  int status;

  CHK((line [*counter] != '/'), NCE_SLASH_MISSING_AFTER_FIRST_ATAN_ARGUMENT);
  *counter = (*counter + 1);
  CHK((line[*counter] != '['),
      NCE_LEFT_BRACKET_MISSING_AFTER_SLASH_WITH_ATAN);
  CHP(read_real_expression(line, counter, &argument2, parameters));
  *double_ptr = atan2(*double_ptr, argument2); /* value in radians */
  *double_ptr = ((*double_ptr * 180.0)/PI);    /* convert to degrees */
  return RS274NGC_OK;
}

/****************************************************************************/

/* execute_unary

Returned Value: int
   If any of the following errors occur, this returns the error code shown.
   Otherwise, it returns RS274NGC_OK.
   1. the operation is unknown: NCE_BUG_UNKNOWN_OPERATION
   2. the argument to acos is not between minus and plus one:
      NCE_ARGUMENT_TO_ACOS_OUT_RANGE
   3. the argument to asin is not between minus and plus one:
      NCE_ARGUMENT_TO_ASIN_OUT_RANGE
   4. the argument to the natural logarithm is not positive:
      NCE_ZERO_OR_NEGATIVE_ARGUMENT_TO_LN
   5. the argument to square root is negative:
      NCE_NEGATIVE_ARGUMENT_TO_SQRT

Side effects:
   The result from performing the operation on the value in double_ptr
   is put into what double_ptr points at.

Called by: read_unary.

This executes the operations: ABS, ACOS, ASIN, COS, EXP, FIX, FUP, LN
ROUND, SIN, SQRT, TAN

All angle measures in the input or output are in degrees.

*/

int execute_unary( /* ARGUMENTS                       */
 float * double_ptr,     /* pointer to the operand          */
 int operation)           /* integer code for the operation  */
{
  switch (operation)
    {
    case ABS:
      if (*double_ptr < 0.0)
        *double_ptr = (-1.0 * *double_ptr);
      break;
    case ACOS:
      CHK(((*double_ptr < -1.0) || (*double_ptr > 1.0)),
          NCE_ARGUMENT_TO_ACOS_OUT_OF_RANGE);
      *double_ptr = acos(*double_ptr);
      *double_ptr = ((*double_ptr * 180.0)/ PI);
      break;
    case ASIN:
      CHK(((*double_ptr < -1.0) || (*double_ptr > 1.0)),
          NCE_ARGUMENT_TO_ASIN_OUT_OF_RANGE);
      *double_ptr = asin(*double_ptr);
      *double_ptr = ((*double_ptr * 180.0)/ PI);
      break;
    case COS:
      *double_ptr = cos((*double_ptr * PI)/180.0);
      break;
    case EXP:
      *double_ptr = exp(*double_ptr);
      break;
    case FIX:
      *double_ptr = floor(*double_ptr);
      break;
    case FUP:
      *double_ptr = ceil(*double_ptr);
      break;
    case LN:
      CHK((*double_ptr <= 0.0), NCE_ZERO_OR_NEGATIVE_ARGUMENT_TO_LN);
      *double_ptr = log(*double_ptr);
      break;
    case ROUND:
      *double_ptr = (double)
        ((int) (*double_ptr + ((*double_ptr < 0.0) ? -0.5 : 0.5)));
      break;
    case SIN:
      *double_ptr = sin((*double_ptr * PI)/180.0);
      break;
    case SQRT:
      CHK((*double_ptr < 0.0), NCE_NEGATIVE_ARGUMENT_TO_SQRT);
      *double_ptr = sqrt(*double_ptr);
      break;
    case TAN:
      *double_ptr = tan((*double_ptr * PI)/180.0);
      break;
    default:
      ERM(NCE_BUG_UNKNOWN_OPERATION);
    }
  return RS274NGC_OK;
}


