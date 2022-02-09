/***************************************************************
  
        bwd_fun.c       Function Table
                        for Bywater BASIC Interpreter
  
                        Copyright (c) 1993, Ted A. Campbell
                        Bywater Software
  
                        email: tcamp@delphi.com
  
        Copyright and Permissions Information:
  
        All U.S. and international rights are claimed by the author,
        Ted A. Campbell.
  
   This software is released under the terms of the GNU General
   Public License (GPL), which is distributed with this software
   in the file "COPYING".  The GPL specifies the terms under
   which users may copy and use the software in this distribution.
  
   A separate license is available for commercial distribution,
   for information on which you should contact the author.
  
***************************************************************/

/*---------------------------------------------------------------*/
/* NOTE: Modifications marked "JBV" were made by Jon B. Volkoff, */
/* 11/1995 (eidetics@cerf.net).                                  */
/*                                                               */
/* Those additionally marked with "DD" were at the suggestion of */
/* Dale DePriest (daled@cadence.com).                            */
/*                                                               */
/* Version 3.00 by Howard Wulf, AF5NE                            */
/*                                                               */
/* Version 3.10 by Howard Wulf, AF5NE                            */
/*                                                               */
/*---------------------------------------------------------------*/



/* FUNCTION TABLE */

#include "bwbasic.h"

FunctionType bwb_prefuncs[ NUM_FUNCTIONS ] =
{
{
  F_ABS_X_N, /* UniqueID */
  "N  = ABS( X )", /* Syntax */
  "The absolute value of X.",  /* Description */
  "ABS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ACOS_X_N, /* UniqueID */
  "N  = ACOS( X )", /* Syntax */
  "The arccosine of X in radians, where 0 <= ACOS(X) <= PI.  X s"
"hall be in the range -1 <= X <= 1.",  /* Description */
  "ACOS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_ACS_X_N, /* UniqueID */
  "N  = ACS( X )", /* Syntax */
  "The arccosine of X in radians, where 0 <= ACS(X) <= PI.  X sh"
"all be in the range -1 <= X <= 1.",  /* Description */
  "ACS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB2 /* OptionVersionBitmask */
},
{
  F_ACSD_X_N, /* UniqueID */
  "N  = ACSD( X )", /* Syntax */
  "The arccosine of X in degrees, where 0 <= ACSD(X) <= 180.  X "
"shall be in the range -1 <= X <= 1.",  /* Description */
  "ACSD", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_ACSG_X_N, /* UniqueID */
  "N  = ACSG( X )", /* Syntax */
  "The arccosine of X in gradians, where 0 <= ACS(X) <= 200.  X "
"shall be in the range -1 <= X <= 1.",  /* Description */
  "ACSG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_ANGLE_X_Y_N, /* UniqueID */
  "N  = ANGLE( X, Y )", /* Syntax */
  "The angle in radians between the positive x-axis and the vect"
"or joining the origin to the point with coordinates (X, Y), w"
"here -PI < ANGLE(X,Y) <= PI.  X and Y must not both be 0.  No"
"te that the counterclockwise is positive, e.g., ANGLE(1,1) = "
"45 degrees.",  /* Description */
  "ANGLE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1ANY | P2ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_ARCSIN_X_N, /* UniqueID */
  "N  = ARCSIN( X )", /* Syntax */
  "The arcsine of X in radians, where -PI/2 <= ARCSIN(X) <= PI/2"
"; X shall be in the range -1 <= X <= 1.",  /* Description */
  "ARCSIN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_ARCTAN_X_N, /* UniqueID */
  "N  = ARCTAN( X )", /* Syntax */
  "The arctangent of X in radians, i.e. the angle whose tangent "
"is X, where -PI/2 < ARCTAN(X) < PI/2.",  /* Description */
  "ARCTAN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_ARGC_N, /* UniqueID */
  "N  = ARGC", /* Syntax */
  "The number of parameters passed to a FUNCTION or SUB.  If not"
" in a FUNCTION or SUB, returns -1.",  /* Description */
  "ARGC", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_ARGT4_X_S, /* UniqueID */
  "S$ = ARGT$( X )", /* Syntax */
  "The type of the X'th parameter to a FUNCTION or SUB.  If the "
"X'th parameter is a string, then return value is '$'.  If the"
" X'th parameter is a number, then return value is not '$'.  X"
" in [1,ARGC].",  /* Description */
  "ARGT$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_ARGV_X_N, /* UniqueID */
  "N  = ARGV( X )", /* Syntax */
  "The value of the X'th parameter to a FUNCTION or SUB.  X in ["
"1,ARGC] and ARGT$( X ) <> '$'.",  /* Description */
  "ARGV", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_ARGV4_X_S, /* UniqueID */
  "S$ = ARGV$( X )", /* Syntax */
  "The value of the X'th parameter to a FUNCTION or SUB.  X in ["
"1,ARGC] and ARGT$( X ) = '$'.",  /* Description */
  "ARGV$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_ASC_A_N, /* UniqueID */
  "N  = ASC( A$ )", /* Syntax */
  "The numeric code for the first letter in A$.  For example, AS"
"C(\"ABC\") returns 65 on ASCII systems.",  /* Description */
  "ASC", /* Name */
  BasicByteSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ASC_A_X_N, /* UniqueID */
  "N  = ASC( A$, X )", /* Syntax */
  "The numeric code of the X'th character in A$.  Same as ASC(MI"
"D$(A$,X)).",  /* Description */
  "ASC", /* Name */
  BasicByteSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2NUM, /* ParameterTypes */
  P1BYT | P2POS, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_ASCII_A_N, /* UniqueID */
  "N  = ASCII( A$ )", /* Syntax */
  "The numeric code for the first letter in A$.  For example, AS"
"CII(\"ABC\") returns 65 on ASCII systems.",  /* Description */
  "ASCII", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB2 /* OptionVersionBitmask */
},
{
  F_ASIN_X_N, /* UniqueID */
  "N  = ASIN( X )", /* Syntax */
  "The arcsine of X in radians, where -PI/2 <= ASIN(X) <= PI/2; "
"X shall be in the range -1 <= X <= 1.",  /* Description */
  "ASIN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_ASN_X_N, /* UniqueID */
  "N  = ASN( X )", /* Syntax */
  "The arcsine of X in radians, where -PI/2 <= ASN(X) <= PI/2; X"
" shall be in the range -1 <= X <= 1.",  /* Description */
  "ASN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_ASND_X_N, /* UniqueID */
  "N  = ASND( X )", /* Syntax */
  "The arcsine of X in degrees, where -90 <= ASN(X) <= 90; X sha"
"ll be in the range -1 <= X <= 1.",  /* Description */
  "ASND", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_ASNG_X_N, /* UniqueID */
  "N  = ASNG( X )", /* Syntax */
  "The arcsine of X in radians, where -100 <= ASNG(X) <= 100; X "
"shall be in the range -1 <= X <= 1.",  /* Description */
  "ASNG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_ATAN_X_N, /* UniqueID */
  "N  = ATAN( X )", /* Syntax */
  "The arctangent of X in radians, i.e. the angle whose tangent "
"is X, where -PI/2 < ATAN(X) < PI/2.",  /* Description */
  "ATAN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_ATN_X_N, /* UniqueID */
  "N  = ATN( X )", /* Syntax */
  "The arctangent of X in radians, i.e. the angle whose tangent "
"is X, where -PI/2 < ATN(X) < PI/2.",  /* Description */
  "ATN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ATND_X_N, /* UniqueID */
  "N  = ATND( X )", /* Syntax */
  "The arctangent of X in degrees, i.e. the angle whose tangent "
"is X, where -90 < ATND(X) < 90.",  /* Description */
  "ATND", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_ATNG_X_N, /* UniqueID */
  "N  = ATNG( X )", /* Syntax */
  "The arctangent of X in gradians, i.e. the angle whose tangent"
" is X, where -100 < ATND(X) < 100.",  /* Description */
  "ATNG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_BASE_N, /* UniqueID */
  "N  = BASE", /* Syntax */
  "The current OPTION BASE setting.",  /* Description */
  "BASE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_CATALOG_N, /* UniqueID */
  "N  = CATALOG", /* Syntax */
  "Displays all the file names.",  /* Description */
  "CATALOG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  D64 | G65 | G67 /* OptionVersionBitmask */
},
{
  F_CATALOG_A_N, /* UniqueID */
  "N  = CATALOG( A$ )", /* Syntax */
  "Displays all the file names matching A$.",  /* Description */
  "CATALOG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  D64 | G65 | G67 /* OptionVersionBitmask */
},
{
  F_CCUR_X_N, /* UniqueID */
  "N  = CCUR( X )", /* Syntax */
  "The currency (64-bit) integer value of X.",  /* Description */
  "CCUR", /* Name */
  BasicCurrencySuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1CUR, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_CDBL_X_N, /* UniqueID */
  "N  = CDBL( X )", /* Syntax */
  "The double-precision value of X.",  /* Description */
  "CDBL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DBL, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CEIL_X_N, /* UniqueID */
  "N  = CEIL( X )", /* Syntax */
  "The smallest integer not less than X.",  /* Description */
  "CEIL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_CHAR_X_Y_S, /* UniqueID */
  "S$ = CHAR( X, Y )", /* Syntax */
  "The string Y bytes long consisting of CHR$(X).  Same as STRIN"
"G$(Y,X).",  /* Description */
  "CHAR", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1BYT | P2LEN, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_CHAR4_X_S, /* UniqueID */
  "S$ = CHAR$( X )", /* Syntax */
  "The one-character string with the character corresponding to "
"the numeric code X.  On ASCII systems, CHAR$(65) returns 'A'."
"",  /* Description */
  "CHAR$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_CHDIR_A_N, /* UniqueID */
  "N  = CHDIR( A$ )", /* Syntax */
  "Changes to the directory named to A$.",  /* Description */
  "CHDIR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CHR_X_S, /* UniqueID */
  "S$ = CHR( X )", /* Syntax */
  "The one-character string with the character corresponding to "
"the numeric code X.  On ASCII systems, CHR(65) returns 'A'.Fo"
"r the IBM System/360 and System/370, this function returns th"
"e output of PRINT X, so A$ = CHR(X) is the opposite of X = NU"
"M(A$).",  /* Description */
  "CHR", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB1 | HB2 | VBX /* OptionVersionBitmask */
},
{
  F_CHR4_X_S, /* UniqueID */
  "S$ = CHR$( X )", /* Syntax */
  "The one-character string with the character corresponding to "
"the numeric code X.  On ASCII systems, CHR$(65) returns 'A'.",  /* Description */
  "CHR$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | D71 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CINT_X_N, /* UniqueID */
  "N  = CINT( X )", /* Syntax */
  "The short (16-bit) integer value of X.",  /* Description */
  "CINT", /* Name */
  BasicIntegerSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CLG_X_N, /* UniqueID */
  "N  = CLG( X )", /* Syntax */
  "The common logarithm of X; X shall be greater than zero.",  /* Description */
  "CLG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GTZ, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_CLK_X_S, /* UniqueID */
  "S$ = CLK( X )", /* Syntax */
  "The time of day in 24-hour notation according to ISO 3307.  F"
"or example, the value of CLK at 11:15 AM is '11:15:00'.  If t"
"here is no clock available, then the value of CLK shall be '9"
"9:99:99'.  The value of TIME$ at midnight is '00:00:00'.  The"
" value of parameter X is ignored.",  /* Description */
  "CLK", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_CLK_X_N, /* UniqueID */
  "N  = CLK( X )", /* Syntax */
  "The time elapsed since the previous midnight, expressed in ho"
"urs; e.g., the value of CLK at 3:15 PM is 15.25.  If there is"
" no clock available, then the value of CLK shall be -1.  The "
"value of CLK at midnight shall be zero (not 24).  The value o"
"f X is ignored.",  /* Description */
  "CLK", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  G65 | G67 /* OptionVersionBitmask */
},
{
  F_CLK4_S, /* UniqueID */
  "S$ = CLK$", /* Syntax */
  "The time of day in 24-hour notation according to ISO 3307.  F"
"or example, the value of TIME$ at 11:15 AM is '11:15:00'.  If"
" there is no clock available, then the value of TIME$ shall b"
"e '99:99:99'.  The value of TIME$ at midnight is '00:00:00'.",  /* Description */
  "CLK$", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | G67 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_CLNG_X_N, /* UniqueID */
  "N  = CLNG( X )", /* Syntax */
  "The long (32-bit) integer value of X.",  /* Description */
  "CLNG", /* Name */
  BasicLongSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LNG, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CLOG_X_N, /* UniqueID */
  "N  = CLOG( X )", /* Syntax */
  "The common logarithm of X; X shall be greater than zero.",  /* Description */
  "CLOG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GTZ, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_CLOSE_N, /* UniqueID */
  "N  = CLOSE", /* Syntax */
  "Close all open files.",  /* Description */
  "CLOSE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CLOSE_X_N, /* UniqueID */
  "N  = CLOSE( X )", /* Syntax */
  "Close file number X.",  /* Description */
  "CLOSE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DEV, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CLS_N, /* UniqueID */
  "N  = CLS", /* Syntax */
  "Clears the screen. Cursor is positioned at row 1, column 1.",  /* Description */
  "CLS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CODE_A_N, /* UniqueID */
  "N  = CODE( A$ )", /* Syntax */
  "The numeric code for the first letter in A$.  For example, CO"
"DE(\"ABC\") returns 65 on ASCII systems.",  /* Description */
  "CODE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_COLOR_X_Y_N, /* UniqueID */
  "N  = COLOR( X, Y )", /* Syntax */
  "Sets the foreground text color to X, and the background text "
"color to Y.",  /* Description */
  "COLOR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1BYT | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_COMMAND4_X_S, /* UniqueID */
  "S$ = COMMAND$( X )", /* Syntax */
  "The command line parameters.  COMMAND$(0) is the BASIC progra"
"m name.  COMMAND$(1) is the first parameter after the BASIC p"
"rogram name, and so on.  Support for parameters varies by ope"
"rating system, compiler, and so on.  X in [0..9]  ",  /* Description */
  "COMMAND$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | C77 | VBX /* OptionVersionBitmask */
},
{
  F_COS_X_N, /* UniqueID */
  "N  = COS( X )", /* Syntax */
  "The cosine of X, where X is in radians.",  /* Description */
  "COS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_COSD_X_N, /* UniqueID */
  "N  = COSD( X )", /* Syntax */
  "The cosine of X, where X is in degrees.",  /* Description */
  "COSD", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_COSG_X_N, /* UniqueID */
  "N  = COSG( X )", /* Syntax */
  "The cosine of X, where X is in gradians.",  /* Description */
  "COSG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_COSH_X_N, /* UniqueID */
  "N  = COSH( X )", /* Syntax */
  "The hyperbolic cosine of X.",  /* Description */
  "COSH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_COT_X_N, /* UniqueID */
  "N  = COT( X )", /* Syntax */
  "The cotangent of X, where X is in radians.",  /* Description */
  "COT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | G67 | I70 | I73 | E86 /* OptionVersionBitmask */
},
{
  F_COUNT_N, /* UniqueID */
  "N  = COUNT", /* Syntax */
  "The current cursor position in the line.",  /* Description */
  "COUNT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_CSC_X_N, /* UniqueID */
  "N  = CSC( X )", /* Syntax */
  "The cosecant of X, where X is in radians.",  /* Description */
  "CSC", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | E86 /* OptionVersionBitmask */
},
{
  F_CSH_X_N, /* UniqueID */
  "N  = CSH( X )", /* Syntax */
  "The hyperbolic cosine of X.",  /* Description */
  "CSH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_CSNG_X_N, /* UniqueID */
  "N  = CSNG( X )", /* Syntax */
  "The single-precision value of X.",  /* Description */
  "CSNG", /* Name */
  BasicSingleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1FLT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CUR_X_Y_S, /* UniqueID */
  "S$ = CUR( X, Y )", /* Syntax */
  "Locates the cursor to row X, column Y.",  /* Description */
  "CUR", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1BYT | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_CVC_A_N, /* UniqueID */
  "N  = CVC( A$ )", /* Syntax */
  "The currency (64-bit) integer value in A$, which was created "
"by MKC$.",  /* Description */
  "CVC", /* Name */
  BasicCurrencySuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1CUR, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_CVD_A_N, /* UniqueID */
  "N  = CVD( A$ )", /* Syntax */
  "The double-precision value in A$, which was created by MKD$.",  /* Description */
  "CVD", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1DBL, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CVI_A_N, /* UniqueID */
  "N  = CVI( A$ )", /* Syntax */
  "The short (16-bit) integer value in A$, which was created by "
"MKI$.",  /* Description */
  "CVI", /* Name */
  BasicIntegerSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CVL_A_N, /* UniqueID */
  "N  = CVL( A$ )", /* Syntax */
  "The long (32-bit) integer value in A$, which was created by M"
"KL$.",  /* Description */
  "CVL", /* Name */
  BasicLongSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1LNG, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_CVS_A_N, /* UniqueID */
  "N  = CVS( A$ )", /* Syntax */
  "The single-precision value in A$, which was created by MKS$.",  /* Description */
  "CVS", /* Name */
  BasicSingleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1FLT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_DAT4_S, /* UniqueID */
  "S$ = DAT$", /* Syntax */
  "Mark-II: The current date based on the computer's internal cl"
"ock as a string in the form 'MM/DD/YY'.  If there is no calen"
"dar available, then the value of DAT$ shall be '00/00/00'.",  /* Description */
  "DAT$", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  G67 /* OptionVersionBitmask */
},
{
  F_DATE_N, /* UniqueID */
  "N  = DATE", /* Syntax */
  "The current date in decimal form YYYDDD, where YYY are the nu"
"mber of years since 1900 and DDD is the ordinal number of the"
" current day of the year; e.g., the value of DATE on May 9, 1"
"977 was 77129.  If there is no calendar available, then the v"
"alue of DATE shall be -1.",  /* Description */
  "DATE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_DATE4_S, /* UniqueID */
  "S$ = DATE$", /* Syntax */
  "BASIC-80: The current date based on the computer's internal c"
"lock as a string in the form 'MM-DD-YYYY'.  ECMA-116: The date "
"in the string representation 'YYYYMMDD' according to ISO 2014"
".  For example, the value of DATE$ on May 9, 1977 was '197705"
"09'.  If there is no calendar available, then the value of DA"
"TE$ shall be '00000000'.",  /* Description */
  "DATE$", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_DATE4_X_S, /* UniqueID */
  "S$ = DATE$( X )", /* Syntax */
  "BASIC-80: The current date based on the computer's internal c"
"lock as a string in the form 'MM-DD-YYYY'.  ECMA-116: The date "
"in the string representation 'YYYYMMDD' according to ISO 2014"
".  For example, the value of DATE$ on May 9, 1977 was '197705"
"09'.  If there is no calendar available, then the value of DA"
"TE$ shall be '00000000'.  The value of parameter 'X' is ignor"
"ed.",  /* Description */
  "DATE$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  D71 /* OptionVersionBitmask */
},
{
  F_DEF_FN_N, /* UniqueID */
  "N  = DEF FN", /* Syntax */
  "User Function.",  /* Description */
  "DEF FN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_DEG_N, /* UniqueID */
  "N  = DEG", /* Syntax */
  "Configures the math functions to accept and return angles in "
"degrees.",  /* Description */
  "DEG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_DEG_X_N, /* UniqueID */
  "N  = DEG( X )", /* Syntax */
  "The number of degrees in X radians.",  /* Description */
  "DEG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB1 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_DEGREE_N, /* UniqueID */
  "N  = DEGREE", /* Syntax */
  "Configures the math functions to accept and return angles in "
"degrees.",  /* Description */
  "DEGREE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_DEGREE_X_N, /* UniqueID */
  "N  = DEGREE( X )", /* Syntax */
  "The number of degrees in X radians.",  /* Description */
  "DEGREE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_DET_N, /* UniqueID */
  "N  = DET", /* Syntax */
  "The determinant of the last MAT INV.  Zero means error.",  /* Description */
  "DET", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | G67 | I70 | D71 | I73 | HB2 /* OptionVersionBitmask */
},
{
  F_DIM_N, /* UniqueID */
  "N  = DIM( ... )", /* Syntax */
  "DIM( arrayname ).  The total number of dimensions of the arra"
"y.",  /* Description */
  "DIM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  255, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_ENVIRON_A_N, /* UniqueID */
  "N  = ENVIRON( A$ )", /* Syntax */
  "Sends the environment variable expression contained in A$ to "
"the host operating system.  A$ must contain the  '=' characte"
"r.",  /* Description */
  "ENVIRON", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ENVIRON4_A_S, /* UniqueID */
  "S$ = ENVIRON$( A$ )", /* Syntax */
  "The value of the environment variable named A$.",  /* Description */
  "ENVIRON$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_EOF_X_N, /* UniqueID */
  "N  = EOF( X )", /* Syntax */
  "If device number X is at the end-of-file, then -1, otherwise "
"0.",  /* Description */
  "EOF", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DEV, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_EPS_X_N, /* UniqueID */
  "N  = EPS( X )", /* Syntax */
  "The maximum of (X-X1,X2-X, sigma) where X1 and X2 are the pre"
"decessor and successor of X and signma is the smallest positi"
"ve value representable.  If X has no predecessor the X1=X and"
" if X has no successor the X2=X.  Note EPS(0) is the smallest"
" positive number representable by the implementation, and is "
"therefor implementation-defined.  Note also that EPS may prod"
"uce different results for different arithmetic options (see O"
"PTION ARITHMETIC).",  /* Description */
  "EPS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_ERL_N, /* UniqueID */
  "N  = ERL", /* Syntax */
  "The line number of the most recent error.",  /* Description */
  "ERL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ERR_N, /* UniqueID */
  "N  = ERR", /* Syntax */
  "The error number of the most recent error.",  /* Description */
  "ERR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ERR4_S, /* UniqueID */
  "S$ = ERR$", /* Syntax */
  "The last error message.",  /* Description */
  "ERR$", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ERRL_N, /* UniqueID */
  "N  = ERRL", /* Syntax */
  "The line number of the most recent error.",  /* Description */
  "ERRL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_ERRN_N, /* UniqueID */
  "N  = ERRN", /* Syntax */
  "The error number of the most recent error.",  /* Description */
  "ERRN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_ERROR_X_N, /* UniqueID */
  "N  = ERROR( X )", /* Syntax */
  "Simulate the error number in X.",  /* Description */
  "ERROR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ERROR_X_A_N, /* UniqueID */
  "N  = ERROR( X, A$ )", /* Syntax */
  "Simulate the error number in X, with a custom message in A$.",  /* Description */
  "ERROR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2STR, /* ParameterTypes */
  P1BYT | P2ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_ERROR4_S, /* UniqueID */
  "S$ = ERROR$", /* Syntax */
  "The last error message.",  /* Description */
  "ERROR$", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_EXAM_X_N, /* UniqueID */
  "N  = EXAM( X )", /* Syntax */
  "The value read from hardware address X.  Not supported on all"
" platforms.",  /* Description */
  "EXAM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LNG, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_EXP_X_N, /* UniqueID */
  "N  = EXP( X )", /* Syntax */
  "The exponential value of X, i.e., the value of the base of na"
"tural logarithms (e = 2.71828) raised to the power of X;  if "
"EXP(X) is less that machine infinitesimal, then its value sha"
"ll be replaced with zero.",  /* Description */
  "EXP", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_FETCH_X_N, /* UniqueID */
  "N  = FETCH( X )", /* Syntax */
  "The value read from hardware address X.  Not supported on all"
" platforms.",  /* Description */
  "FETCH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LNG, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_FILEATTR_X_Y_N, /* UniqueID */
  "N  = FILEATTR( X, Y )", /* Syntax */
  "For file X, if Y = 1 then returns open mode, otherwise return"
"s zero.",  /* Description */
  "FILEATTR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1DEV | P2INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_FILES_N, /* UniqueID */
  "N  = FILES", /* Syntax */
  "Displays all the file names.",  /* Description */
  "FILES", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_FILES_A_N, /* UniqueID */
  "N  = FILES( A$ )", /* Syntax */
  "Displays all the file names matching A$.",  /* Description */
  "FILES", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_FILL_X_Y_N, /* UniqueID */
  "N  = FILL( X, Y )", /* Syntax */
  "Sends Y to hardware address X.  Not supported on all platform"
"s.",  /* Description */
  "FILL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1LNG | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_FIX_X_N, /* UniqueID */
  "N  = FIX( X )", /* Syntax */
  "The truncated integer, part of X. FIX (X) is equivalent to SG"
"N(X)*INT(ABS(X)). The major difference between FIX and INT is"
" that FIX does not return the next lower number for negative "
"X.",  /* Description */
  "FIX", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_FLOAT_X_N, /* UniqueID */
  "N  = FLOAT( X )", /* Syntax */
  "The rounded integer valuet of X.",  /* Description */
  "FLOAT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  C77 /* OptionVersionBitmask */
},
{
  F_FLOW_N, /* UniqueID */
  "N  = FLOW", /* Syntax */
  "Turn tracing ON",  /* Description */
  "FLOW", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_FP_X_N, /* UniqueID */
  "N  = FP( X )", /* Syntax */
  "The fractional part of X, i.e. X - IP(X).",  /* Description */
  "FP", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_FRAC_X_N, /* UniqueID */
  "N  = FRAC( X )", /* Syntax */
  "The fractional part of X, i.e. X - IP(X).",  /* Description */
  "FRAC", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_FRE_N, /* UniqueID */
  "N  = FRE", /* Syntax */
  "The number of bytes of available memory.  This function is pr"
"ovided for backward compatibility only and it always returns "
"a fixed value of 32000.",  /* Description */
  "FRE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_FRE_A_N, /* UniqueID */
  "N  = FRE( A$ )", /* Syntax */
  "The number of bytes of available memory.  This function is pr"
"ovided for backward compatibility only and it always returns "
"a fixed value of 32000.The value of A$ is ignored.",  /* Description */
  "FRE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_FRE_X_N, /* UniqueID */
  "N  = FRE( X )", /* Syntax */
  "The number of bytes of available memory.  This function is pr"
"ovided for backward compatibility only and it always returns "
"a fixed value of 32000.  The value of X is ignored.",  /* Description */
  "FRE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_FREE_N, /* UniqueID */
  "N  = FREE", /* Syntax */
  "The number of bytes of available memory.  This function is pr"
"ovided for backward compatibility only and it always returns "
"a fixed value of 32000.",  /* Description */
  "FREE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_FREE_A_N, /* UniqueID */
  "N  = FREE( A$ )", /* Syntax */
  "The number of bytes of available memory.  This function is pr"
"ovided for backward compatibility only and it always returns "
"a fixed value of 32000.The value of A$ is ignored.",  /* Description */
  "FREE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_FREE_X_N, /* UniqueID */
  "N  = FREE( X )", /* Syntax */
  "The number of bytes of available memory.  This function is pr"
"ovided for backward compatibility only and it always returns "
"a fixed value of 32000.  The value of X is ignored.",  /* Description */
  "FREE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_FREEFILE_N, /* UniqueID */
  "N  = FREEFILE", /* Syntax */
  "The next available file number.",  /* Description */
  "FREEFILE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_GET_X_N, /* UniqueID */
  "N  = GET( X )", /* Syntax */
  "For file X, gets the next available record.",  /* Description */
  "GET", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DEV, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_GET_X_Y_N, /* UniqueID */
  "N  = GET( X, Y )", /* Syntax */
  "For file X, gets record number Y.  The first record number is"
" 1.",  /* Description */
  "GET", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1DEV | P2INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_GRAD_N, /* UniqueID */
  "N  = GRAD", /* Syntax */
  "Configures the math functions to accept and return angles in "
"gradians.",  /* Description */
  "GRAD", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_GRADIAN_N, /* UniqueID */
  "N  = GRADIAN", /* Syntax */
  "Configures the math functions to accept and return angles in "
"gradians.",  /* Description */
  "GRADIAN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_HCS_X_N, /* UniqueID */
  "N  = HCS( X )", /* Syntax */
  "The hyperbolic cosine of X.",  /* Description */
  "HCS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  I70 | I73 /* OptionVersionBitmask */
},
{
  F_HEX4_X_S, /* UniqueID */
  "S$ = HEX$( X )", /* Syntax */
  "The the hexadecimal (base 16) representation of X.",  /* Description */
  "HEX$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_HOME_N, /* UniqueID */
  "N  = HOME", /* Syntax */
  "Clears the screen. Cursor is positioned at row 1, column 1.",  /* Description */
  "HOME", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_HSN_X_N, /* UniqueID */
  "N  = HSN( X )", /* Syntax */
  "The hyperbolic sine of X.",  /* Description */
  "HSN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  I70 | I73 /* OptionVersionBitmask */
},
{
  F_HTN_X_N, /* UniqueID */
  "N  = HTN( X )", /* Syntax */
  "The hyperbolic tangent of X.",  /* Description */
  "HTN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  I70 | I73 /* OptionVersionBitmask */
},
{
  F_INDEX_A_B_N, /* UniqueID */
  "N  = INDEX( A$, B$ )", /* Syntax */
  "The position at which B$ occurs in A$, beginning at position "
"1.",  /* Description */
  "INDEX", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2STR, /* ParameterTypes */
  P1ANY | P2ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_INITIALIZE_N, /* UniqueID */
  "N  = INITIALIZE", /* Syntax */
  "This function is provided for backward compatibility only and"
" it always returns a fixed value of 0.",  /* Description */
  "INITIALIZE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  C77 /* OptionVersionBitmask */
},
{
  F_INKEY4_S, /* UniqueID */
  "S$ = INKEY$", /* Syntax */
  "The keypress, if available. If a keypress is not available, t"
"hen immediately returns a null string ('').  If not supported"
" by the platform, then always returns an empty string, so use"
" INPUT$(1) instead.",  /* Description */
  "INKEY$", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_INP_X_N, /* UniqueID */
  "N  = INP( X )", /* Syntax */
  "The value read from machine port X.  Not supported on all pla"
"tforms.",  /* Description */
  "INP", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_INPUT4_X_S, /* UniqueID */
  "S$ = INPUT$( X )", /* Syntax */
  "The string of X characters, read from the terminal.",  /* Description */
  "INPUT$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LEN, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_INPUT4_X_Y_S, /* UniqueID */
  "S$ = INPUT$( X, Y )", /* Syntax */
  "The string of X characters, read from file Y.",  /* Description */
  "INPUT$", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1LEN | P2DEV, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_INSTR_A_B_N, /* UniqueID */
  "N  = INSTR( A$, B$ )", /* Syntax */
  "The position at which B$ occurs in A$, beginning at position "
"1.",  /* Description */
  "INSTR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2STR, /* ParameterTypes */
  P1ANY | P2ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_INSTR_X_A_B_N, /* UniqueID */
  "N  = INSTR( X, A$, B$ )", /* Syntax */
  "The position at which B$ occurs in A$, beginning at position "
"X.",  /* Description */
  "INSTR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  3, /* ParameterCount */
  P1NUM | P2STR | P3STR, /* ParameterTypes */
  P1POS | P2ANY | P3ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_INT_X_N, /* UniqueID */
  "N  = INT( X )", /* Syntax */
  "The largest integer not greater than X; e.g. INT(1.3) = 1 and"
" INT(-1.3) = 2.",  /* Description */
  "INT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_INT5_X_N, /* UniqueID */
  "N  = INT%( X )", /* Syntax */
  "The rounded integer valuet of X.",  /* Description */
  "INT%", /* Name */
  BasicLongSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  C77 /* OptionVersionBitmask */
},
{
  F_IP_X_N, /* UniqueID */
  "N  = IP( X )", /* Syntax */
  "The integer part of X, i.e., SGN(X)*INT(ABS(X)).",  /* Description */
  "IP", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_KEY_S, /* UniqueID */
  "S$ = KEY", /* Syntax */
  "The keypress, if available. If a keypress is not available, t"
"hen immediately returns a null string ('').  If not supported"
" by the platform, then always returns an empty string, so use"
" INPUT$(1) instead.",  /* Description */
  "KEY", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_KEY4_S, /* UniqueID */
  "S$ = KEY$", /* Syntax */
  "The keypress, if available. If a keypress is not available, t"
"hen immediately returns a null string ('').  If not supported"
" by the platform, then always returns an empty string, so use"
" INPUT$(1) instead.",  /* Description */
  "KEY$", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_KILL_A_N, /* UniqueID */
  "N  = KILL( A$ )", /* Syntax */
  "Removes the file named in A$.",  /* Description */
  "KILL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LBOUND_N, /* UniqueID */
  "N  = LBOUND( ... )", /* Syntax */
  "LBOUND( arrayname [, dimension] ).  The lower bound of the ar"
"ray.  The dimension defaults to 1.  dimension in [1,DIM(array"
"name)]",  /* Description */
  "LBOUND", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  255, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LCASE4_A_S, /* UniqueID */
  "S$ = LCASE$( A$ )", /* Syntax */
  "The string of characters from the value associatedwith A$ by "
"replacing each upper-case-letter in the string by its lower-c"
"ase version.",  /* Description */
  "LCASE$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LEFT_A_X_S, /* UniqueID */
  "S$ = LEFT( A$, X )", /* Syntax */
  "The X left-most characters of A$, beginning from postion 1.",  /* Description */
  "LEFT", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2NUM, /* ParameterTypes */
  P1ANY | P2LEN, /* ParameterTests */
  NULL, /* NextPointer */
  D71 | HB1 | HB2 | VBX /* OptionVersionBitmask */
},
{
  F_LEFT4_A_X_S, /* UniqueID */
  "S$ = LEFT$( A$, X )", /* Syntax */
  "The X left-most characters of A$, beginning from postion 1.",  /* Description */
  "LEFT$", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2NUM, /* ParameterTypes */
  P1ANY | P2LEN, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LEN_A_N, /* UniqueID */
  "N  = LEN( A$ )", /* Syntax */
  "The length of A$.",  /* Description */
  "LEN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LGT_X_N, /* UniqueID */
  "N  = LGT( X )", /* Syntax */
  "The common logarithm of X; X shall be greater than zero.",  /* Description */
  "LGT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GTZ, /* ParameterTests */
  NULL, /* NextPointer */
  I70 | I73 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_LIN_X_S, /* UniqueID */
  "S$ = LIN( X )", /* Syntax */
  "The string X bytes long of newline characters.",  /* Description */
  "LIN", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LEN, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_LN_X_N, /* UniqueID */
  "N  = LN( X )", /* Syntax */
  "The natural logarithm of X; X shall be greater than zero.",  /* Description */
  "LN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GTZ, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_LOC_X_N, /* UniqueID */
  "N  = LOC( X )", /* Syntax */
  "The location of file X; the next record that GET or PUT state"
"ments will use.",  /* Description */
  "LOC", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DEV, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | G65 | G67 | D71 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LOCATE_X_Y_N, /* UniqueID */
  "N  = LOCATE( X, Y )", /* Syntax */
  "Locates the cursor to row X, column Y.",  /* Description */
  "LOCATE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1BYT | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LOF_X_N, /* UniqueID */
  "N  = LOF( X )", /* Syntax */
  "The length of file X.",  /* Description */
  "LOF", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DEV, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | G65 | G67 | D71 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LOG_X_N, /* UniqueID */
  "N  = LOG( X )", /* Syntax */
  "The natural logarithm of X; X shall be greater than zero.",  /* Description */
  "LOG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GTZ, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LOG10_X_N, /* UniqueID */
  "N  = LOG10( X )", /* Syntax */
  "The common logarithm of X; X shall be greater than zero.",  /* Description */
  "LOG10", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GTZ, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_LOG2_X_N, /* UniqueID */
  "N  = LOG2( X )", /* Syntax */
  "The base 2 logarithm of X; X shall be greater than zero.",  /* Description */
  "LOG2", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GTZ, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_LOGE_X_N, /* UniqueID */
  "N  = LOGE( X )", /* Syntax */
  "The natural logarithm of X; X shall be greater than zero.",  /* Description */
  "LOGE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GTZ, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_LPOS_N, /* UniqueID */
  "N  = LPOS", /* Syntax */
  "The current cursor position in the line for the printer.",  /* Description */
  "LPOS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LTRIM4_A_S, /* UniqueID */
  "S$ = LTRIM$( A$ )", /* Syntax */
  "The string of characters resulting from the value associated "
"with A$ by deleting all leading space characters.",  /* Description */
  "LTRIM$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_LTW_X_N, /* UniqueID */
  "N  = LTW( X )", /* Syntax */
  "The base 2 logarithm of X; X shall be greater than zero.",  /* Description */
  "LTW", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GTZ, /* ParameterTests */
  NULL, /* NextPointer */
  I70 | I73 /* OptionVersionBitmask */
},
{
  F_LWIDTH_X_N, /* UniqueID */
  "N  = LWIDTH( X )", /* Syntax */
  "For printer, sets the line width to X. Zero means no wrapping"
" will occur.",  /* Description */
  "LWIDTH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_MATCH_A_B_X_N, /* UniqueID */
  "N  = MATCH( A$, B$, X )", /* Syntax */
  "The character position, within the value assocated with A$, o"
"f the first character of the first occurence of the value ass"
"ociated with B$, starting at the Xth character of A$.  If the"
"re is not such occurence, then the value returned is zero.",  /* Description */
  "MATCH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  3, /* ParameterCount */
  P1STR | P2STR | P3NUM, /* ParameterTypes */
  P1ANY | P2ANY | P3POS, /* ParameterTests */
  NULL, /* NextPointer */
  C77 /* OptionVersionBitmask */
},
{
  F_MAX_X_Y_N, /* UniqueID */
  "N  = MAX( X, Y )", /* Syntax */
  "The larger of the parameters.",  /* Description */
  "MAX", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1ANY | P2ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_MAX_A_B_S, /* UniqueID */
  "S$ = MAX( A$, B$ )", /* Syntax */
  "The larger of the parameters.",  /* Description */
  "MAX", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2STR, /* ParameterTypes */
  P1ANY | P2ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB2 /* OptionVersionBitmask */
},
{
  F_MAXBYT_N, /* UniqueID */
  "N  = MAXBYT", /* Syntax */
  "The largest finite positive number representable as a BYTE; i"
"mplementation-defined.",  /* Description */
  "MAXBYT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MAXCUR_N, /* UniqueID */
  "N  = MAXCUR", /* Syntax */
  "The largest finite positive number representable as a CURRENC"
"Y; implementation-defined.",  /* Description */
  "MAXCUR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MAXDBL_N, /* UniqueID */
  "N  = MAXDBL", /* Syntax */
  "The largest finite positive number representable as a DOUBLE;"
" implementation-defined.",  /* Description */
  "MAXDBL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MAXDEV_N, /* UniqueID */
  "N  = MAXDEV", /* Syntax */
  "The largest finite positive number useable as a FILE NUMBER; "
"implementation-defined.",  /* Description */
  "MAXDEV", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MAXINT_N, /* UniqueID */
  "N  = MAXINT", /* Syntax */
  "The largest finite positive number representable as an INTEGE"
"R; implementation-defined.",  /* Description */
  "MAXINT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MAXLEN_A_N, /* UniqueID */
  "N  = MAXLEN( A$ )", /* Syntax */
  "The maximum length associated with the simple-string-variable"
" A$.",  /* Description */
  "MAXLEN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_MAXLNG_N, /* UniqueID */
  "N  = MAXLNG", /* Syntax */
  "The largest finite positive number representable as a LONG; i"
"mplementation-defined.",  /* Description */
  "MAXLNG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MAXLVL_N, /* UniqueID */
  "N  = MAXLVL", /* Syntax */
  "The maximum stack level; implementation-defined.",  /* Description */
  "MAXLVL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MAXNUM_N, /* UniqueID */
  "N  = MAXNUM", /* Syntax */
  "The largest finite positive number representable and manipula"
"ble by the implementation; implementation-defined.  MAXNUM ma"
"y represent diffent number for different arithmetic options ("
"see OPTION ARITHMETIC).",  /* Description */
  "MAXNUM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_MAXSNG_N, /* UniqueID */
  "N  = MAXSNG", /* Syntax */
  "The largest finite positive number representable as a SINGLE;"
" implementation-defined.",  /* Description */
  "MAXSNG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MEM_N, /* UniqueID */
  "N  = MEM", /* Syntax */
  "The number of bytes of available memory.  This function is pr"
"ovided for backward compatibility only and it always returns "
"a fixed value of 32000.",  /* Description */
  "MEM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_MID_A_X_S, /* UniqueID */
  "S$ = MID( A$, X )", /* Syntax */
  "The characters of A$, starting from postion X.",  /* Description */
  "MID", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2NUM, /* ParameterTypes */
  P1ANY | P2POS, /* ParameterTests */
  NULL, /* NextPointer */
  D71 | HB1 | HB2 | VBX /* OptionVersionBitmask */
},
{
  F_MID_A_X_Y_S, /* UniqueID */
  "S$ = MID( A$, X, Y )", /* Syntax */
  "The Y characters of A$, starting from postion X.",  /* Description */
  "MID", /* Name */
  BasicStringSuffix, /* ReturnType */
  3, /* ParameterCount */
  P1STR | P2NUM | P3NUM, /* ParameterTypes */
  P1ANY | P2POS | P3LEN, /* ParameterTests */
  NULL, /* NextPointer */
  D71 | HB1 | HB2 | VBX /* OptionVersionBitmask */
},
{
  F_MID4_A_X_S, /* UniqueID */
  "S$ = MID$( A$, X )", /* Syntax */
  "The characters of A$, starting from postion X.",  /* Description */
  "MID$", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2NUM, /* ParameterTypes */
  P1ANY | P2POS, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_MID4_A_X_Y_S, /* UniqueID */
  "S$ = MID$( A$, X, Y )", /* Syntax */
  "The Y characters of A$, starting from postion X.",  /* Description */
  "MID$", /* Name */
  BasicStringSuffix, /* ReturnType */
  3, /* ParameterCount */
  P1STR | P2NUM | P3NUM, /* ParameterTypes */
  P1ANY | P2POS | P3LEN, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_MIN_X_Y_N, /* UniqueID */
  "N  = MIN( X, Y )", /* Syntax */
  "The smaller of the parameters.",  /* Description */
  "MIN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1ANY | P2ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_MIN_A_B_S, /* UniqueID */
  "S$ = MIN( A$, B$ )", /* Syntax */
  "The smaller of the parameters.",  /* Description */
  "MIN", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2STR, /* ParameterTypes */
  P1ANY | P2ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB2 /* OptionVersionBitmask */
},
{
  F_MINBYT_N, /* UniqueID */
  "N  = MINBYT", /* Syntax */
  "The largest finite negative number representable as a BYTE; i"
"mplementation-defined.",  /* Description */
  "MINBYT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MINCUR_N, /* UniqueID */
  "N  = MINCUR", /* Syntax */
  "The largest finite negative number representable as a CURRENC"
"Y; implementation-defined.",  /* Description */
  "MINCUR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MINDBL_N, /* UniqueID */
  "N  = MINDBL", /* Syntax */
  "The largest finite negative number representable as a DOUBLE;"
" implementation-defined.",  /* Description */
  "MINDBL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MINDEV_N, /* UniqueID */
  "N  = MINDEV", /* Syntax */
  "The largest finite negative number useable as a FILE NUMBER; "
"implementation-defined.",  /* Description */
  "MINDEV", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MININT_N, /* UniqueID */
  "N  = MININT", /* Syntax */
  "The largest finite negative number representable as an INTEGE"
"R; implementation-defined.",  /* Description */
  "MININT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MINLNG_N, /* UniqueID */
  "N  = MINLNG", /* Syntax */
  "The largest finite negative number representable as a LONG; i"
"mplementation-defined.",  /* Description */
  "MINLNG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MINNUM_N, /* UniqueID */
  "N  = MINNUM", /* Syntax */
  "The largest finite negative number representable and manipula"
"ble by the implementation; implementation-defined.  MINNUM ma"
"y represent diffent number for different arithmetic options ("
"see OPTION ARITHMETIC).",  /* Description */
  "MINNUM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MINSNG_N, /* UniqueID */
  "N  = MINSNG", /* Syntax */
  "The largest finite negative number representable as a SINGLE;"
" implementation-defined.",  /* Description */
  "MINSNG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MKC4_X_S, /* UniqueID */
  "S$ = MKC$( X )", /* Syntax */
  "The internal representation of the currency (64-bit) integer "
"X as a string.",  /* Description */
  "MKC$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1CUR, /* ParameterTests */
  NULL, /* NextPointer */
  B15 /* OptionVersionBitmask */
},
{
  F_MKD4_X_S, /* UniqueID */
  "S$ = MKD$( X )", /* Syntax */
  "The internal representation of X as a string.",  /* Description */
  "MKD$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DBL, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_MKDIR_A_N, /* UniqueID */
  "N  = MKDIR( A$ )", /* Syntax */
  "Makes the directory named in A$.",  /* Description */
  "MKDIR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_MKI4_X_S, /* UniqueID */
  "S$ = MKI$( X )", /* Syntax */
  "The internal representation of the short (16-bit) integer X a"
"s a string.",  /* Description */
  "MKI$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_MKL4_X_S, /* UniqueID */
  "S$ = MKL$( X )", /* Syntax */
  "The internal representation of the long (32-bit) integer X as"
" a string.",  /* Description */
  "MKL$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LNG, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_MKS4_X_S, /* UniqueID */
  "S$ = MKS$( X )", /* Syntax */
  "The internal representation of X as a string.",  /* Description */
  "MKS$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1FLT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_MOD_X_Y_N, /* UniqueID */
  "N  = MOD( X, Y )", /* Syntax */
  "X modulo Y, i.e., X-Y*INT(X/Y).  Y shall not equal zero.",  /* Description */
  "MOD", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1ANY | P2NEZ, /* ParameterTests */
  NULL, /* NextPointer */
  E86 /* OptionVersionBitmask */
},
{
  F_NAME_A_B_N, /* UniqueID */
  "N  = NAME( A$, B$ )", /* Syntax */
  "Rename the file named A$ into B$.",  /* Description */
  "NAME", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2STR, /* ParameterTypes */
  P1BYT | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_NOFLOW_N, /* UniqueID */
  "N  = NOFLOW", /* Syntax */
  "Turn tracing OFF",  /* Description */
  "NOFLOW", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_NOTRACE_N, /* UniqueID */
  "N  = NOTRACE", /* Syntax */
  "Turn tracing OFF",  /* Description */
  "NOTRACE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_NULL_X_N, /* UniqueID */
  "N  = NULL( X )", /* Syntax */
  "Appends X null characters after each line printed by LPRINT o"
"r LLIST.",  /* Description */
  "NULL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_NUM_N, /* UniqueID */
  "N  = NUM", /* Syntax */
  "The number of values processed by the last MAT INPUT.  Zero m"
"eans error.",  /* Description */
  "NUM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | G67 | HB2 /* OptionVersionBitmask */
},
{
  F_NUM_A_N, /* UniqueID */
  "N  = NUM( A$ )", /* Syntax */
  "The value of the numeric-constant associated with A$, if the "
"string associated with A$ is a numeric-constant.  Leading and"
" trailing spaces in the string are ignored.  If the evaluatio"
"n of the numeric-constant would result in a value which cause"
"s an underflow, then the value returned shall be zero.  For e"
"xample, NUM(' 123.5 ' ) = 123.5, NUM('2.E-99') could be zero,"
" and NUM('MCMXVII') causes an exception.",  /* Description */
  "NUM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB2 /* OptionVersionBitmask */
},
{
  F_NUM4_X_S, /* UniqueID */
  "S$ = NUM$( X )", /* Syntax */
  "The string generated by the print-statement as the numeric-re"
"presentation of the value associated with X.",  /* Description */
  "NUM$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_OCT4_X_S, /* UniqueID */
  "S$ = OCT$( X )", /* Syntax */
  "The the octal (base 8) representation of X.",  /* Description */
  "OCT$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_OPEN_A_X_B_N, /* UniqueID */
  "N  = OPEN( A$, X, B$ )", /* Syntax */
  "Open file number X.  A$ is the mode: I, O, A, R.  B$ is the f"
"ile name.  Default the record length.",  /* Description */
  "OPEN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  3, /* ParameterCount */
  P1STR | P2NUM | P3STR, /* ParameterTypes */
  P1BYT | P2DEV | P3BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_OPEN_A_X_B_Y_N, /* UniqueID */
  "N  = OPEN( A$, X, B$, Y )", /* Syntax */
  "Open file number X.  A$ is the mode: I, O, A, R.  B$ is the f"
"ile name.  Y is the record length.",  /* Description */
  "OPEN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  4, /* ParameterCount */
  P1STR | P2NUM | P3STR | P4NUM, /* ParameterTypes */
  P1BYT | P2DEV | P3BYT | P4INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ORD_A_N, /* UniqueID */
  "N  = ORD( A$ )", /* Syntax */
  "The ordinal position of the character named by the string ass"
"ociated with A$ in the collating sequence of ASCII character "
"set, where the first member of the character set is in positi"
"on zero.  The acceptable values for the standard character se"
"t are shown in Table 1.",  /* Description */
  "ORD", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_OUT_X_Y_N, /* UniqueID */
  "N  = OUT( X, Y )", /* Syntax */
  "Sends Y to hardware port X.  Not supported on all platforms.",  /* Description */
  "OUT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1INT | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_PAUSE_X_N, /* UniqueID */
  "N  = PAUSE( X )", /* Syntax */
  "The program pauses for X times the value of OPTION SLEEP seco"
"nds.  If the result is zero, negative, or more than INT_MAX t"
"hen PAUSE does nothing.  The resolution is implementation def"
"ined.",  /* Description */
  "PAUSE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_PDL_X_N, /* UniqueID */
  "N  = PDL( X )", /* Syntax */
  "The value read from machine port X.  Not supported on all pla"
"tforms.",  /* Description */
  "PDL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_PEEK_X_N, /* UniqueID */
  "N  = PEEK( X )", /* Syntax */
  "The value read from hardware address X.  Not supported on all"
" platforms.",  /* Description */
  "PEEK", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LNG, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_PI_N, /* UniqueID */
  "N  = PI", /* Syntax */
  "The constant 3.14159 which is the ratio of the circumference "
"of a circle to its diameter.",  /* Description */
  "PI", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_PI_X_N, /* UniqueID */
  "N  = PI( X )", /* Syntax */
  "The constant 3.14159 which is the ratio of the circumference "
"of a circle to its diameter, times X.",  /* Description */
  "PI", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_PIN_X_N, /* UniqueID */
  "N  = PIN( X )", /* Syntax */
  "The value read from machine port X.  Not supported on all pla"
"tforms.",  /* Description */
  "PIN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_POKE_X_Y_N, /* UniqueID */
  "N  = POKE( X, Y )", /* Syntax */
  "Sends Y to hardware address X.  Not supported on all platform"
"s.",  /* Description */
  "POKE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1LNG | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_POS_N, /* UniqueID */
  "N  = POS", /* Syntax */
  "The current cursor position in the line.",  /* Description */
  "POS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_POS_X_N, /* UniqueID */
  "N  = POS( X )", /* Syntax */
  "The current cursor position in the line for file X.",  /* Description */
  "POS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DEV, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_POS_A_B_N, /* UniqueID */
  "N  = POS( A$, B$ )", /* Syntax */
  "The character position, within the value assocated with A$, o"
"f the first character of the first occurence of the value ass"
"ociated with B$, starting at the first character of A$.  If t"
"here is not such occurence, then the value returned is zero.",  /* Description */
  "POS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2STR, /* ParameterTypes */
  P1ANY | P2ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_POS_A_B_X_N, /* UniqueID */
  "N  = POS( A$, B$, X )", /* Syntax */
  "The character position, within the value assocated with A$, o"
"f the first character of the first occurence of the value ass"
"ociated with B$, starting at the Xth character of A$.  If the"
"re is not such occurence, then the value returned is zero.",  /* Description */
  "POS", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  3, /* ParameterCount */
  P1STR | P2STR | P3NUM, /* ParameterTypes */
  P1ANY | P2ANY | P3POS, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_PUT_X_N, /* UniqueID */
  "N  = PUT( X )", /* Syntax */
  "For file X, puts the next available record.",  /* Description */
  "PUT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DEV, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_PUT_X_Y_N, /* UniqueID */
  "N  = PUT( X, Y )", /* Syntax */
  "For file X, puts record number Y.  The first record number is"
" 1.",  /* Description */
  "PUT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1DEV | P2INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_RAD_N, /* UniqueID */
  "N  = RAD", /* Syntax */
  "Configures the math functions to accept and return angles in "
"radians.",  /* Description */
  "RAD", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_RAD_X_N, /* UniqueID */
  "N  = RAD( X )", /* Syntax */
  "The number of radians in X degrees.",  /* Description */
  "RAD", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_RADIAN_N, /* UniqueID */
  "N  = RADIAN", /* Syntax */
  "Configures the math functions to accept and return angles in "
"radians.",  /* Description */
  "RADIAN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_RAN_N, /* UniqueID */
  "N  = RAN", /* Syntax */
  "Seeds the pseudo-random number generator with TIME.",  /* Description */
  "RAN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_RAN_X_N, /* UniqueID */
  "N  = RAN( X )", /* Syntax */
  "Seeds the pseudo-random number generator with X.",  /* Description */
  "RAN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_RANDOM_N, /* UniqueID */
  "N  = RANDOM", /* Syntax */
  "Seeds the pseudo-random number generator with TIME.",  /* Description */
  "RANDOM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  D64 | G65 | G67 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_RANDOM_X_N, /* UniqueID */
  "N  = RANDOM( X )", /* Syntax */
  "Seeds the pseudo-random number generator with X.",  /* Description */
  "RANDOM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  D71 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_RANDOMIZE_N, /* UniqueID */
  "N  = RANDOMIZE", /* Syntax */
  "Seeds the pseudo-random number generator with TIME.",  /* Description */
  "RANDOMIZE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_RANDOMIZE_X_N, /* UniqueID */
  "N  = RANDOMIZE( X )", /* Syntax */
  "Seeds the pseudo-random number generator with X.",  /* Description */
  "RANDOMIZE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_REMAINDER_X_Y_N, /* UniqueID */
  "N  = REMAINDER( X, Y )", /* Syntax */
  "The remainder function, i.e., X-Y*IP(X/Y).  Y shall not equal"
" zero.",  /* Description */
  "REMAINDER", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1ANY | P2NEZ, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_RENAME_A_B_N, /* UniqueID */
  "N  = RENAME( A$, B$ )", /* Syntax */
  "Rename file A$ to B$.  If successful, the returns -1 else ret"
"urns 0.",  /* Description */
  "RENAME", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2STR, /* ParameterTypes */
  P1BYT | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  C77 /* OptionVersionBitmask */
},
{
  F_REPEAT4_X_A_S, /* UniqueID */
  "S$ = REPEAT$( X, A$ )", /* Syntax */
  "The string consisting of X copies of LEFT$(A$,1); X > 0.",  /* Description */
  "REPEAT$", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2STR, /* ParameterTypes */
  P1LEN | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_REPEAT4_X_Y_S, /* UniqueID */
  "S$ = REPEAT$( X, Y )", /* Syntax */
  "The string consisting of X copies of CHR$(Y); X > 0.",  /* Description */
  "REPEAT$", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1LEN | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_RESET_N, /* UniqueID */
  "N  = RESET", /* Syntax */
  "Close all open files.",  /* Description */
  "RESET", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_RIGHT_A_X_S, /* UniqueID */
  "S$ = RIGHT( A$, X )", /* Syntax */
  "The right-most X characters of A$.",  /* Description */
  "RIGHT", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2NUM, /* ParameterTypes */
  P1ANY | P2LEN, /* ParameterTests */
  NULL, /* NextPointer */
  D71 | HB1 | HB2 | VBX /* OptionVersionBitmask */
},
{
  F_RIGHT4_A_X_S, /* UniqueID */
  "S$ = RIGHT$( A$, X )", /* Syntax */
  "The right-most X characters of A$.",  /* Description */
  "RIGHT$", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1STR | P2NUM, /* ParameterTypes */
  P1ANY | P2LEN, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_RMDIR_A_N, /* UniqueID */
  "N  = RMDIR( A$ )", /* Syntax */
  "Removes the directory named in A$.",  /* Description */
  "RMDIR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_RND_N, /* UniqueID */
  "N  = RND", /* Syntax */
  "The next pseudo-random number in an implementation-defined se"
"quence of pseudo-random numbers uniformly distributed in the "
"range 0 <= RND < 1.",  /* Description */
  "RND", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_RND_X_N, /* UniqueID */
  "N  = RND( X )", /* Syntax */
  "Returns a pseudorandom number in the range [0,1].  The value "
"of X is ignored.",  /* Description */
  "RND", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_ROUND_X_Y_N, /* UniqueID */
  "N  = ROUND( X, Y )", /* Syntax */
  "The value of X rounded to Y decimal digits to the right of th"
"e decimal point (or -Y digits to the left if Y < 0); i.e., IN"
"T(X*10^Y+.5)/10^Y.  Y must be in [-32,32].",  /* Description */
  "ROUND", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1ANY | P2INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_RTRIM4_A_S, /* UniqueID */
  "S$ = RTRIM$( A$ )", /* Syntax */
  "The string of characters resulting from the value associated "
"with A$ by deleting all trailing space characters.",  /* Description */
  "RTRIM$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_SEC_X_N, /* UniqueID */
  "N  = SEC( X )", /* Syntax */
  "The secant of X, where X is in radians.",  /* Description */
  "SEC", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | I70 | I73 | E86 /* OptionVersionBitmask */
},
{
  F_SEEK_X_N, /* UniqueID */
  "N  = SEEK( X )", /* Syntax */
  "The location of file X; the next record that GET or PUT state"
"ments will use.",  /* Description */
  "SEEK", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1DEV, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_SEEK_X_Y_N, /* UniqueID */
  "N  = SEEK( X, Y )", /* Syntax */
  "For file X, move to record number Y; the first record number "
"is 1.",  /* Description */
  "SEEK", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1DEV | P2INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_SEG_A_X_Y_S, /* UniqueID */
  "S$ = SEG( A$, X, Y )", /* Syntax */
  "The Y characters of A$, starting from postion X.",  /* Description */
  "SEG", /* Name */
  BasicStringSuffix, /* ReturnType */
  3, /* ParameterCount */
  P1STR | P2NUM | P3NUM, /* ParameterTypes */
  P1ANY | P2POS | P3LEN, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_SEG4_A_X_Y_S, /* UniqueID */
  "S$ = SEG$( A$, X, Y )", /* Syntax */
  "The Y characters of A$, starting from postion X.",  /* Description */
  "SEG$", /* Name */
  BasicStringSuffix, /* ReturnType */
  3, /* ParameterCount */
  P1STR | P2NUM | P3NUM, /* ParameterTypes */
  P1ANY | P2POS | P3LEN, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_SGN_X_N, /* UniqueID */
  "N  = SGN( X )", /* Syntax */
  "The sign of X: -1 if X < 0, 0 if X = 0, and +1 if X > 0.",  /* Description */
  "SGN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_SHELL_A_N, /* UniqueID */
  "N  = SHELL( A$ )", /* Syntax */
  "The exit code resulting from the execution of an operating sy"
"stem command.",  /* Description */
  "SHELL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_SIN_X_N, /* UniqueID */
  "N  = SIN( X )", /* Syntax */
  "The sine of X, where X is in radians.",  /* Description */
  "SIN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_SIND_X_N, /* UniqueID */
  "N  = SIND( X )", /* Syntax */
  "The sine of X, where X is in degrees.",  /* Description */
  "SIND", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_SING_X_N, /* UniqueID */
  "N  = SING( X )", /* Syntax */
  "The sine of X, where X is in gradians.",  /* Description */
  "SING", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_SINH_X_N, /* UniqueID */
  "N  = SINH( X )", /* Syntax */
  "The hyperbolic sine of X.",  /* Description */
  "SINH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_SIZE_A_N, /* UniqueID */
  "N  = SIZE( A$ )", /* Syntax */
  "The total number of 1KB blockes required to contain the file."
"",  /* Description */
  "SIZE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  C77 /* OptionVersionBitmask */
},
{
  F_SIZE_N, /* UniqueID */
  "N  = SIZE( ... )", /* Syntax */
  "SIZE( arrayname ).  The total number of items in the array.",  /* Description */
  "SIZE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  255, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  E86 /* OptionVersionBitmask */
},
{
  F_SLEEP_X_N, /* UniqueID */
  "N  = SLEEP( X )", /* Syntax */
  "The program pauses for X times the value of OPTION SLEEP seco"
"nds.  If the result is zero, negative, or more than INT_MAX t"
"hen SLEEP does nothing.  The resolution is implementation def"
"ined.",  /* Description */
  "SLEEP", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_SNH_X_N, /* UniqueID */
  "N  = SNH( X )", /* Syntax */
  "The hyperbolic sine of X.",  /* Description */
  "SNH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_SPA_X_S, /* UniqueID */
  "S$ = SPA( X )", /* Syntax */
  "The string of X blank spaces.",  /* Description */
  "SPA", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LEN, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_SPACE_X_S, /* UniqueID */
  "S$ = SPACE( X )", /* Syntax */
  "The string of X blank spaces.",  /* Description */
  "SPACE", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LEN, /* ParameterTests */
  NULL, /* NextPointer */
  D71 | HB1 | HB2 | VBX /* OptionVersionBitmask */
},
{
  F_SPACE4_X_S, /* UniqueID */
  "S$ = SPACE$( X )", /* Syntax */
  "The string of X blank spaces.",  /* Description */
  "SPACE$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1LEN, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_SPC_X_S, /* UniqueID */
  "S$ = SPC( X )", /* Syntax */
  "The string of X blank spaces.  Only for use within the PRINT "
"command.",  /* Description */
  "SPC", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_SQR_X_N, /* UniqueID */
  "N  = SQR( X )", /* Syntax */
  "The non-negative square root of X;  X shall be non-negative.",  /* Description */
  "SQR", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GEZ, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_SQRT_X_N, /* UniqueID */
  "N  = SQRT( X )", /* Syntax */
  "The non-negative square root of X;  X shall be non-negative.",  /* Description */
  "SQRT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1GEZ, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_STR_X_Y_S, /* UniqueID */
  "S$ = STR( X, Y )", /* Syntax */
  "The string X bytes long consisting of CHR$(Y).",  /* Description */
  "STR", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1LEN | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_STR4_X_S, /* UniqueID */
  "S$ = STR$( X )", /* Syntax */
  "The string generated by the print-statement as the numeric-re"
"presentation of the value associated with X.",  /* Description */
  "STR$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_STRING_X_Y_S, /* UniqueID */
  "S$ = STRING( X, Y )", /* Syntax */
  "The string X bytes long consisting of CHR$(Y).",  /* Description */
  "STRING", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1LEN | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_STRING4_X_A_S, /* UniqueID */
  "S$ = STRING$( X, A$ )", /* Syntax */
  "The string X bytes long consisting of the first character of "
"A$.",  /* Description */
  "STRING$", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2STR, /* ParameterTypes */
  P1LEN | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_STRING4_X_Y_S, /* UniqueID */
  "S$ = STRING$( X, Y )", /* Syntax */
  "The string X bytes long consisting of CHR$(Y).",  /* Description */
  "STRING$", /* Name */
  BasicStringSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1LEN | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_STUFF_X_Y_N, /* UniqueID */
  "N  = STUFF( X, Y )", /* Syntax */
  "Sends Y to hardware address X.  Not supported on all platform"
"s.",  /* Description */
  "STUFF", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1LNG | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_TAB_X_S, /* UniqueID */
  "S$ = TAB( X )", /* Syntax */
  "The string required to advance to column X.  Only for use wit"
"hin the PRINT command.",  /* Description */
  "TAB", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_TAN_X_N, /* UniqueID */
  "N  = TAN( X )", /* Syntax */
  "The tangent of X, where X is in radians.",  /* Description */
  "TAN", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_TAND_X_N, /* UniqueID */
  "N  = TAND( X )", /* Syntax */
  "The tangent of X, where X is in degrees.",  /* Description */
  "TAND", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_TANG_X_N, /* UniqueID */
  "N  = TANG( X )", /* Syntax */
  "The tangent of X, where X is in gradians.",  /* Description */
  "TANG", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 /* OptionVersionBitmask */
},
{
  F_TANH_X_N, /* UniqueID */
  "N  = TANH( X )", /* Syntax */
  "The hyperbolic tangent of X.",  /* Description */
  "TANH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_TI_N, /* UniqueID */
  "N  = TI", /* Syntax */
  "The time elapsed since the previous midnight, expressed in se"
"conds; e.g., the value of TIME at 11:15 AM is 40500.  If ther"
"e is no clock available, then the value of TIME shall be -1. "
" The value of TIME at midnight shall be zero (not 86400).",  /* Description */
  "TI", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_TI4_S, /* UniqueID */
  "S$ = TI$", /* Syntax */
  "The time of day in 24-hour notation according to ISO 3307.  F"
"or example, the value of TIME$ at 11:15 AM is '11:15:00'.  If"
" there is no clock available, then the value of TIME$ shall b"
"e '99:99:99'.  The value of TIME$ at midnight is '00:00:00'.",  /* Description */
  "TI$", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_TIM_N, /* UniqueID */
  "N  = TIM", /* Syntax */
  "The time elapsed since the previous midnight, expressed in se"
"conds; e.g., the value of TIME at 11:15 AM is 40500.  If ther"
"e is no clock available, then the value of TIME shall be -1. "
" The value of TIME at midnight shall be zero (not 86400).",  /* Description */
  "TIM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  G67 | I70 | I73 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_TIM_X_N, /* UniqueID */
  "N  = TIM( X )", /* Syntax */
  "If X is 0, returns minutes in current hour.  If X is 1, retur"
"ns hours in current day.  If X is 2, returns days in current "
"year.  If X is 3, returns years since 1900.  Any other value "
"for X is an ERROR.",  /* Description */
  "TIM", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  G65 | G67 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_TIME_N, /* UniqueID */
  "N  = TIME", /* Syntax */
  "The time elapsed since the previous midnight, expressed in se"
"conds; e.g., the value of TIME at 11:15 AM is 40500.  If ther"
"e is no clock available, then the value of TIME shall be -1. "
" The value of TIME at midnight shall be zero (not 86400).",  /* Description */
  "TIME", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | E86 /* OptionVersionBitmask */
},
{
  F_TIME_X_N, /* UniqueID */
  "N  = TIME( X )", /* Syntax */
  "The time elapsed since the previous midnight, expressed in se"
"conds; e.g., the value of TIME at 11:15 AM is 40500.  If ther"
"e is no clock available, then the value of TIME shall be -1. "
" The value of TIME at midnight shall be zero (not 86400).  Th"
"e value of the parameter X is ignored.",  /* Description */
  "TIME", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  D71 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_TIME4_S, /* UniqueID */
  "S$ = TIME$", /* Syntax */
  "The time of day in 24-hour notation according to ISO 3307.  F"
"or example, the value of TIME$ at 11:15 AM is '11:15:00'.  If"
" there is no clock available, then the value of TIME$ shall b"
"e '99:99:99'.  The value of TIME$ at midnight is '00:00:00'.",  /* Description */
  "TIME$", /* Name */
  BasicStringSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_TIME4_X_S, /* UniqueID */
  "S$ = TIME$( X )", /* Syntax */
  "The time of day in 24-hour notation according to ISO 3307.  F"
"or example, the value of TIME$ at 11:15 AM is '11:15:00'.  If"
" there is no clock available, then the value of TIME$ shall b"
"e '99:99:99'.  The value of TIME$ at midnight is '00:00:00'. "
" The value of X is ignored.",  /* Description */
  "TIME$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB2 /* OptionVersionBitmask */
},
{
  F_TIMER_N, /* UniqueID */
  "N  = TIMER", /* Syntax */
  "The time in the system clock in seconds elapsed since midnigh"
"t.",  /* Description */
  "TIMER", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_TOP_N, /* UniqueID */
  "N  = TOP", /* Syntax */
  "The address of the top of available memory.  This function is"
" provided for backward compatibility only and it always retur"
"ns a fixed value of 32000.",  /* Description */
  "TOP", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  HB2 /* OptionVersionBitmask */
},
{
  F_TRACE_N, /* UniqueID */
  "N  = TRACE", /* Syntax */
  "Turn tracing ON",  /* Description */
  "TRACE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_TRIM4_A_S, /* UniqueID */
  "S$ = TRIM$( A$ )", /* Syntax */
  "The string resulting from removing both leading and trailing "
"spaces from A$.",  /* Description */
  "TRIM$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_TROFF_N, /* UniqueID */
  "N  = TROFF", /* Syntax */
  "Turn tracing OFF",  /* Description */
  "TROFF", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_TRON_N, /* UniqueID */
  "N  = TRON", /* Syntax */
  "Turn tracing ON",  /* Description */
  "TRON", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  0, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_TRUNCATE_X_Y_N, /* UniqueID */
  "N  = TRUNCATE( X, Y )", /* Syntax */
  "The value of X truncated to Y decimal digits to the right of "
"the decimal point (or -Y digits to the left if Y < 0); i.e., "
"IP(X*10^Y)/10^Y.  Y in [-32,32].",  /* Description */
  "TRUNCATE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1ANY | P2INT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 /* OptionVersionBitmask */
},
{
  F_UBOUND_N, /* UniqueID */
  "N  = UBOUND( ... )", /* Syntax */
  "UBOUND( arrayname [, dimension] ).  The upper bound of the ar"
"ray.  The dimension defaults to 1.  dimension in [1,DIM(array"
"name)]",  /* Description */
  "UBOUND", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  255, /* ParameterCount */
  PNONE, /* ParameterTypes */
  PNONE, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_UCASE4_A_S, /* UniqueID */
  "S$ = UCASE$( A$ )", /* Syntax */
  "The string of characters resulting from the value associated "
"with A$ by replacing each lower-case-letter in the string by "
"its upper-case version.",  /* Description */
  "UCASE$", /* Name */
  BasicStringSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | C77 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_UNSAVE_A_N, /* UniqueID */
  "N  = UNSAVE( A$ )", /* Syntax */
  "Removes the file named in A$.",  /* Description */
  "UNSAVE", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  D64 | G65 | G67 /* OptionVersionBitmask */
},
{
  F_VAL_A_N, /* UniqueID */
  "N  = VAL( A$ )", /* Syntax */
  "The value of the numeric-constant associated with A$, if the "
"string associated with A$ is a numeric-constant.  Leading and"
" trailing spaces in the string are ignored.  If the evaluatio"
"n of the numeric-constant would result in a value which cause"
"s an underflow, then the value returned shall be zero.  For e"
"xample, VAL(' 123.5 ' ) = 123.5, VAL('2.E-99') could be zero,"
" and VAL('MCMXVII') causes an exception.",  /* Description */
  "VAL", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1STR, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_VTAB_X_N, /* UniqueID */
  "N  = VTAB( X )", /* Syntax */
  "Savme as LOCATE X, 1.",  /* Description */
  "VTAB", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_WAIT_X_N, /* UniqueID */
  "N  = WAIT( X )", /* Syntax */
  "The program pauses for X times the value of OPTION SLEEP seco"
"nds.  If the result is zero, negative, or more than INT_MAX t"
"hen WAIT does nothing.  The resolution is implementation defi"
"ned.",  /* Description */
  "WAIT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1ANY, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 /* OptionVersionBitmask */
},
{
  F_WAIT_X_Y_N, /* UniqueID */
  "N  = WAIT( X, Y )", /* Syntax */
  "Waits for the value of (INP(X) AND Y) to become nonzero.  Not"
" supported on all platforms.",  /* Description */
  "WAIT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1INT | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_WAIT_X_Y_Z_N, /* UniqueID */
  "N  = WAIT( X, Y, Z )", /* Syntax */
  "Waits for the value of ((INP(X) XOR Z) AND Y) to become nonze"
"ro.  Not supported on all platforms.",  /* Description */
  "WAIT", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  3, /* ParameterCount */
  P1NUM | P2NUM | P3NUM, /* ParameterTypes */
  P1INT | P2BYT | P3BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_WIDTH_X_N, /* UniqueID */
  "N  = WIDTH( X )", /* Syntax */
  "For console, sets the line width to X. Zero means no wrapping"
" will occur.",  /* Description */
  "WIDTH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  1, /* ParameterCount */
  P1NUM, /* ParameterTypes */
  P1BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
{
  F_WIDTH_X_Y_N, /* UniqueID */
  "N  = WIDTH( X, Y )", /* Syntax */
  "If X in [MINDEV,MAXDEV] then for file X, sets the line width "
"to Y.  If X = 0, sets the console width to Y.  f X > MAXDEV, "
"then sets the console rows to X and the line width to Y.  A v"
"alue of zero for Y means no wrapping will occur.",  /* Description */
  "WIDTH", /* Name */
  BasicDoubleSuffix, /* ReturnType */
  2, /* ParameterCount */
  P1NUM | P2NUM, /* ParameterTypes */
  P1BYT | P2BYT, /* ParameterTests */
  NULL, /* NextPointer */
  B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX /* OptionVersionBitmask */
},
};







/* EOF */
