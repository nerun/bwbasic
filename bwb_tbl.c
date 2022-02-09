/***************************************************************
  
   bwb_tbl.c       Command, Function, Operator, 
                   and Error-Message Tables
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
/*---------------------------------------------------------------*/




#include "bwbasic.h"


/* 
specifier	Replaced by	Example
%a	Abbreviated weekday name *	Thu
%A	Full weekday name *	Thursday
%b	Abbreviated month name *	Aug
%B	Full month name *	August
%c	Date and time representation *	Thu Aug 23 14:55:02 2001
%C	Year divided by 100 and truncated to integer (00-99)	20
%d	Day of the month, zero-padded (01-31)	23
%D	Short MM/DD/YY date, equivalent to %m/%d/%y	08/23/01
%e	Day of the month, space-padded ( 1-31)	23
%F	Short YYYY-MM-DD date, equivalent to %Y-%m-%d	2001-08-23
%g	Week-based year, last two digits (00-99)	01
%G	Week-based year	2001
%h	Abbreviated month name * (same as %b)	Aug
%H	Hour in 24h format (00-23)	14
%I	Hour in 12h format (01-12)	02
%j	Day of the year (001-366)	235
%m	Month as a decimal number (01-12)	08
%M	Minute (00-59)	55
%n	New-line character ('\n')	
%p	AM or PM designation	PM
%r	12-hour clock time *	02:55:02 pm
%R	24-hour HH:MM time, equivalent to %H:%M	14:55
%S	Second (00-61)	02
%t	Horizontal-tab character ('\t')	
%T	ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S	14:55:02
%u	ISO 8601 weekday as number with Monday as 1 (1-7)	4
%U	Week number with the first Sunday as the first day of week one (00-53)	33
%V	ISO 8601 week number (00-53)	34
%w	Weekday as a decimal number with Sunday as 0 (0-6)	4
%W	Week number with the first Monday as the first day of week one (00-53)	34
%x	Date representation *	08/23/01
%X	Time representation *	14:55:02
%y	Year, last two digits (00-99)	01
%Y	Year	2001
%z	ISO 8601 offset from UTC in timezone (1 minute=1, 1 hour=100)
If timezone cannot be termined, no characters	+100
%Z	Timezone name or abbreviation *
If timezone cannot be termined, no characters	CDT
%%	A % sign	%

*/


/* 
NOTE:
Bywater BASIC is an interpreter, not a compiler, so full compatibility is impossible.
There is NO plan to implement the VB1-VB6 forms, events, objects, DLL, OCX, and so on.
This is only about compatible intrinsic commands, functions, and operators.
*/

struct bwb_version bwb_vertable[NUM_VERSIONS] =
{
 /* Name,     Value , Descrption          , Year , ID  , REM , STMT, OptionFlags  ,
  * OptionDateFormat, OptionTimeFormat */
 {"BYWATER", B14, "Bywater BASIC", 2014, "B14", '\'', ':', OPTION_LABELS_ON | OPTION_BUGS_ON | OPTION_TRACE_ON, "%m/%d/%Y","%H:%M:%S"},
 {"DARTMOUTH", D64, "Dartmouth BASIC", 1964, "D64", '\0', '\0', FALSE, "%m/%d/%Y","%H:%M:%S"},
 {"ECMA-55", E78, "ECMA-55", 1978, "E78", '\0', '\0', FALSE, "%m/%d/%Y","%H:%M:%S"},
 {"BASIC-80", M80, "Microsoft BASIC-80", 1980, "M80", '\'', ':', OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"TRS-83", T83, "TRS-80 Model III", 1980, "T83", '\'', ':', OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"TRS-84", T84, "TRS-80 Model 4", 1983, "T84", '\'', ':', OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"GW-BASIC", M85, "Microsoft GW-BASIC", 1985, "M85", '\'', ':', OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"ECMA-116", E86, "ECMA-116", 1986, "E86", '!', '\0', OPTION_BASE_ONE | OPTION_LABELS_ON | OPTION_STRICT_ON, "%Y-%m-%d","%H:%M:%S"},
 {"QBASIC", M88, "Microsoft QBASIC", 1988, "M88", '\'', ':', OPTION_LABELS_ON | OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"VBDOS", M90, "Microsoft VBDOS", 1990, "M90", '\'', ':', OPTION_LABELS_ON | OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"VB1", M91, "Microsoft VB1", 1991, "M91", '\'', ':', OPTION_LABELS_ON | OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"VB2", M92, "Microsoft VB2", 1992, "M92", '\'', ':', OPTION_LABELS_ON | OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"VB3", M93, "Microsoft VB3", 1993, "M93", '\'', ':', OPTION_LABELS_ON | OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"VB4", M95, "Microsoft VB4", 1995, "M95", '\'', ':', OPTION_LABELS_ON | OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"VB5", M97, "Microsoft VB5", 1997, "M97", '\'', ':', OPTION_LABELS_ON | OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
 {"VB6", M98, "Microsoft VB6", 1998, "M98", '\'', ':', OPTION_LABELS_ON | OPTION_BUGS_ON, "%m/%d/%Y","%H:%M:%S"},
};



/***************************************************************
  
   Operator Table for Bywater BASIC
  
***************************************************************/
/* , B14 | M88 | M90 | M91 |M92 |M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78  */
struct bwb_op   exp_ops[NUM_OPERATORS] =
{
 /* Name, UniqueID, # , B14 | M88 | M90 | M91 |M92 |M93 | M95 | M97 | M98 |
  * M85 | M80 | E86 | D64 | E78  */
 {"NOT", OP_NOT, 2, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"AND", OP_AND, 13, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"OR", OP_OR, 14, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"XOR", OP_XOR, 15, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"IMP", OP_IMPLIES, 16, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"EQV", OP_EQUIV, 17, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"MOD", OP_MODULUS, 5, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80},
 {"<>", OP_NOTEQUAL, 8, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"<=", OP_LTEQ, 11, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"=<", OP_LTEQ, 11, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {">=", OP_GTEQ, 12, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"=>", OP_GTEQ, 12, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"<", OP_LESSTHAN, 9, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {">", OP_GREATERTHAN, 10, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"^", OP_EXPONENT, 0, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"[", OP_EXPONENT, 0, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"*", OP_MULTIPLY, 3, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"/", OP_DIVIDE, 3, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"\\", OP_INTDIVISION, 4, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"+", OP_ADD, 6, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"-", OP_SUBTRACT, 6, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"=", OP_EQUALS, 7, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"=", OP_ASSIGN, 18, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {";", OP_STRJOIN, 19, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {",", OP_STRTAB, 20, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"-", OP_NEGATION, 1, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"+", OP_POSATION, 1, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},
 {"#", OP_POSATION, 1, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},  /* EOF(#1) */
#if FALSE
 {"&", OP_STRJOIN, 19, B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 | M85 | M80 | E86 | D64 | E78},  /* ... A$ & B$ ... */
 {"LIKE", OP_STRLIKE, 7, B14 | M9X},   /* IF A$ LIKE "*.TXT" THEN ... */
#endif
};


/* EOF */
