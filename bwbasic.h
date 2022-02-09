/***************************************************************
  
        bwbasic.h       Header File
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



#include         <assert.h>
#include         <ctype.h>
#include         <errno.h>
#include         <float.h>
#include         <limits.h>
#include         <locale.h>
#include         <math.h>
#include         <setjmp.h>
#include         <signal.h>
#include         <stdarg.h>
#include         <stddef.h>
#include         <stdio.h>
#include         <stdlib.h>
#include         <string.h>
#include         <time.h>



#ifndef TRUE
#define TRUE    -1
#endif
#ifndef FALSE
#define FALSE   0
#endif



/***************************************************************
  
   bwbasic.h   Part I: Definitions
  
***************************************************************/

/***************************************************************
  
   bwbasic.h:  Part I-A: Define Major Hardware Implementation
  
         Gone is the simplicity of earlier versions.

         TTY is the default implementation.
         It is the most minimal, but the most
         universal hardware implementation.
  
         If you use TTY then check the settings
         in bwx_tty.c for your system.
         
  
***************************************************************/



/*
**
**
** Here is where you should
** #include implementation-specific files
** and #define your preferences
**
**
*/



#if HAVE_ISNAN

/* 
If I understand correctly, 
my development environment is:
  
GCC 4.8 for Linux 
via CCTools for Android 
in a BusyBox environment 
under a Java simulation 
of an ARM processor 
on a Barnes & Noble
$69 Nook Tablet.

What could possibly go wrong? 

When you finish laughing,
it actually works well.

I compile with:
gcc -DHAVE_ISNAN -ansi -o bwbasic bw*.c

If you are using a Unix derivative,
then you may want to do something 
similar to the following.
*/

#include <unistd.h>
#include <sys/stat.h>
#define DIRECTORY_CMDS     TRUE
#define MKDIR_ONE_ARG      FALSE
#define PERMISSIONS        0x0777
#define DEF_EDITOR         "nano"
#endif /* HAVE_ISNAN */

#if __CMS__
/* GCC 3 for CMS on Hercules */
#include "bwd_six.h"
#define PROFILENAME        "PROFILE BAS"
#define DEBUGFILENAME      "DEBUG OUT"
#define LPRINTFILENAME     "LPRINT OUT"
#define ERRFILE            "ERROR OUT"
#define EXPONENT_DIGITS    4  /* E-9999 to E+9999 */
#define PROMPT             "bwBASIC:\n"
#define DIRECTORY_CMDS     FALSE
#define HAVE_ISNAN          FALSE
#endif /* __CMS__ */

#if __MVS__
/* GCC 3 for MVS on Hercules */
#include "bwd_six.h"
#define PROFILENAME        "dd:profile"
#define DEBUGFILENAME      "dd:debug"
#define LPRINTFILENAME     "dd:lprint"
#define ERRFILE            "dd:errout"
#define EXPONENT_DIGITS    4  /* E-9999 to E+9999 */
#define PROMPT             "bwBASIC:\n"
#define DIRECTORY_CMDS     FALSE
#define HAVE_ISNAN          FALSE
#endif /* __MVS__ */

#if __GNUC__
/* __FUNCTION__ is the current function name */
#else
#define __FUNCTION__ "n/a"
#endif





/***************************************************************
  
   bwbasic.h:  This ends the section of definitions that
         users of bwBASIC will normally need to
         specify. The following are internally defined.
  
         Note that you may need to #define the default
         FILES command and the default EDITOR above.
  
***************************************************************/



/*
**
**
** If you want to change any of these values, 
** then you should #define them above.
**
**
*/

#ifndef DEBUG_BUILD
/* MAINTAINER */
#define DEBUG_BUILD    FALSE  /* only TRUE for DEBUG versions */
#endif
#ifndef HAVE_ISNAN
#define HAVE_ISNAN      FALSE  /* isnan() exists when compiling with -ansi */
#endif
#ifndef PROFILE
#define PROFILE         TRUE  /* interpret profile at beginning */
#endif 
#ifndef PROFILENAME
/* only used when PROFILE == TRUE */
#define PROFILENAME    "profile.bas"   /* Filename for profile execution */
#endif 
#ifndef REDIRECT_STDERR
#define REDIRECT_STDERR FALSE /* Redirect stderr to file ERRFILE */
#endif 
#ifndef ERRFILE
/* only used when REDIRECT_STDERR == TRUE */
#define ERRFILE         "err.out"   /* Filename for redirected stderr */
#endif 
#ifndef DEBUGFILENAME
/* MAINTAINER */
#define DEBUGFILENAME  "DEBUG.OUT"     /* Filename for DEBUG output */
#endif 
#ifndef LPRINTFILENAME
/* LPRINT */
#define LPRINTFILENAME  "LPRINT.OUT"   /* Filename for LPRINT output */
#endif 
#ifndef DIRECTORY_CMDS
/* MKDIR, CHDIR, RMDIR */
#define DIRECTORY_CMDS       TRUE  /* It better be for UNIX systems (JBV) */
#endif
#ifndef MKDIR_ONE_ARG
/* only used when DIRECTORY_CMDS == TRUE */
#define MKDIR_ONE_ARG      FALSE /* TRUE if your mkdir has one argument */
#endif
#ifndef PERMISSIONS
/* only used when MKDIR_ONE_ARG == FALSE */
#define PERMISSIONS        644 /* permissions to set in Unix-type system */
#endif
#ifndef DEFVNAME_PROMPT
#define DEFVNAME_PROMPT "BWB.PROMPT$"  /* variable name for PROMPT */
#endif
#ifndef PROMPT
#define PROMPT          "bwBASIC: " /* default value for PROMPT */
#endif
#ifndef DEFVNAME_EDITOR
#define DEFVNAME_EDITOR "BWB.EDITOR$"  /* variable name for EDITOR */
#endif
#ifndef DEF_EDITOR
#define DEF_EDITOR      "vi"  /* default value for EDITOR */
#endif
#ifndef DEFVNAME_FILES
#define DEFVNAME_FILES  "BWB.FILES$"   /* variable name for FILES */
#endif
#ifndef DEF_FILES
#define DEF_FILES       "ls -Fx"/* default value for FILES */
#endif
#ifndef DEFVNAME_COLORS
#define DEFVNAME_COLORS "BWB.COLORS"   /* variable name for COLORS */
#endif
#ifndef DEF_COLORS
#define DEF_COLORS      256   /* default value for COLORS */
#endif
#ifndef DEFVNAME_IMPL
#define DEFVNAME_IMPL   "BWB.IMPLEMENTATION$"   /* variable name for IMPLEMENTATION */
#endif
#ifndef IMP_IDSTRING
#define IMP_IDSTRING    "TTY" /* default value for IMPLEMENTATION */
#endif
#ifndef EXECLEVELS
#define EXECLEVELS     255 /* maximum EXEC stack levels */
#endif
#ifndef MAX_DIMS
#define MAX_DIMS         3 /* maximum # of dimensions */
#endif
#ifndef ESTACKSIZE
#define ESTACKSIZE      64 /* elements in expression stack */
#endif
#ifndef BROKEN_STRCPY
#define BROKEN_STRCPY TRUE /* only set this true if your C library's strcpy() is broken */
#endif

#if 0
/* these are no longer used in version 3.00 */
#define IMP_FNCINKEY 0     /* 0 if INKEY$ is not implemented, 1 if it is */
#define IMP_CMDCLS   0     /* 0 if CLS is not implemented, 1 if it is */
#define IMP_CMDLOC   0     /* 0 if LOCATE is not implemented, 1 if it is */
#define IMP_CMDCOLOR    0  /* 0 if COLOR is not implemented, 1 if it is */
#endif


/*
**
**
** UNLESS YOU ARE WORKING ON THE INTERNALS, YOU PROBABLY SHOULD NOT GO BEYOND HERE 
**
**
*/



#define VERSION         "3.00"   /* Current version number */



#ifndef AUTOMATED_REGRESSION
/* for automated testing */
#define AUTOMATED_REGRESSION      FALSE
#endif



/*
The relationship of numeric values is REQUIRED to be:
  
  MAXNUM >= MAXDBL >= MAXSNG >= MAXCUR >= MAXLNG >= MAXINT >= MAXLEN >= MAXBYT >= MAXDEV > 0
  MINNUM <= MINDBL <= MINSNG <= MINCUR <= MINLNG <= MININT < 0
  MINLEN == MINBYT == MINDEV == 0
  
  MAXBYT == 255 
  MAXINT >= 32000
  MININT <= -32000
  MAXSNG >= 1E37
  MINSNG <= -1E37
  
*/



/* BASIC NUMBER [MINNUM,MAXNUM] */
#ifndef BasicNumberSuffix
#define BasicNumberSuffix '\0'   /* N//A */
#endif
typedef double  BasicNumberType;
#ifndef BasicNumberScanFormat
#define BasicNumberScanFormat  "%lf"
#endif
#ifndef BasicNumberPrintFormat
#define BasicNumberPrintFormat "%f"
#endif
#ifndef SIGNIFICANT_DIGITS
#define SIGNIFICANT_DIGITS 6  /* Number of significant digits, minimum is 6 */
#endif
#ifndef EXPONENT_DIGITS
#define EXPONENT_DIGITS    3  /* Number of exrad digits      , minimum is 2 */
#endif



/* BASIC DOUBLE [MINDBL,MAXDBL] */
#ifndef BasicDoubleSuffix
#define BasicDoubleSuffix '#'
#endif
typedef double  BasicDoubleType;



/* BASIC SINGLE [MINSNG,MAXSNG] */
#ifndef BasicSingleSuffix
#define BasicSingleSuffix  '!'
#endif
typedef float   BasicSingleType;



/* BASIC CURRENCY [MINCUR,MAXCUR] */
#ifndef BasicCurrencySuffix
#define BasicCurrencySuffix  '@'
#endif
typedef long int BasicCurrencyType;



/* BASIC LONG [MINLNG,MAXLNG] */
#ifndef BasicLongSuffix
#define BasicLongSuffix  '&'
#endif
typedef long int BasicLongType;



/* BASIC INTEGER [MININT,MAXINT]*/
#ifndef BasicIntegerSuffix
#define BasicIntegerSuffix '%'
#endif
typedef short int BasicIntegerType;



/* BASIC BYTE [MINDEV,MAXDEV] */
#if 0
#ifndef BasicByteSuffix
#define BasicByteSuffix  '\0' /* N/A */
#endif
#endif
typedef unsigned char BasicByteType;



/* BASIC DEVICE [MINDEV,MAXDEV]*/
#ifndef BasicFileNumberPrefix
#define BasicFileNumberPrefix  '#'
#endif
typedef unsigned char BasicFileNumberType;
#ifndef BasicFileNumberMax
#define BasicFileNumberMax     7   
/* max file # (1..7), file # 0 is the console */
#endif



/* BASIC LENGTH [MINLEN,MAXLEN] */
#ifndef BasicStringSuffix
#define BasicStringSuffix     '$'
#endif
typedef unsigned int BasicStringLengthType;
#ifndef BasicStringLengthMax
#define BasicStringLengthMax   4096 /* max length of BASIC string */
#endif



/* BASIC keywork length */
#ifndef BasicNameLengthMax
#define BasicNameLengthMax 40 /* variable,function,subroutine,command */
#endif



/* BASIC linenumber */
typedef int     BasicLineNumberType;
#define BasicLineNumberMax (INT_MAX - 32) /* maximum BASIC line number */
#define BasicLineNumberMin 1  /* minimum USER line number */



/* UniqueID */
typedef int     BasicUniqueType;



#define MAX_PRECEDENCE  20 /* highest (last) level of precedence */
#define DEF_SUBSCRIPT   11 /* default subscript, including 0.  0...10 */



/* Derivative definitions */



#define CURTASK
#define LOCALTASK



/* define variable types based on last character */

#define STRING          '$'
#define PRN_TAB      0x02  /* send TAB followed by TAB number */
#define PRN_SPC      0x01  /* send SPC followed by SPC number */



/* Operations defined */
#define OP_ERROR        -255  /* operation error (break out) */
#define OP_NULL         0  /* null: operation not defined yet */
#define NUMBER          1  /* number held as internal variable in uvar */
#define CONST_STRING    2  /* string constant */
#define CONST_NUMERICAL 3  /* numerical constant */
#define FUNCTION        4  /* function header */
#define VARIABLE        5  /* external variable pointed to by xvar */
#define PARENTHESIS     6  /* begin parenthetical expression */
#define OP_ADD          7  /* addition sign '+' */
#define OP_SUBTRACT     8  /* subtraction sign '-' */
#define OP_MULTIPLY     9  /* multiplication sign '*' */
#define OP_DIVIDE       10 /* division sign '/' */
#define OP_MODULUS      11 /* modulus "MOD" */
#define OP_EXPONENT     12 /* exponentiation '^' */
#define OP_INTDIVISION  13 /* integer division sign '\' */
#define OP_NEGATION     14 /* negation '-' ??? */
#define OP_STRJOIN      15 /* string join ';' */
#define OP_STRTAB       16 /* string tab ',' */
#define OP_EQUALS       17 /* either logical equal operator */
#define OP_ASSIGN       18 /* assignment operator */
#define OP_NOTEQUAL     20 /* inequality */
#define OP_LESSTHAN     21 /* less than */
#define OP_GREATERTHAN  22 /* greater than */
#define OP_LTEQ         23 /* less than or equal to */
#define OP_GTEQ         24 /* greater than or equal to */
#define OP_NOT          25 /* negation */
#define OP_AND          26 /* conjunction */
#define OP_OR           27 /* disjunction */
#define OP_XOR          28 /* exclusive or */
#define OP_IMPLIES      29 /* implication */
#define OP_EQUIV        30 /* equivalence */
#define OP_TERMINATE    31 /* terminate expression parsing */
#define OP_USERFNC      32  /* user-defined function */
#define OP_POSATION     33 /* leading '+' */
#define NUM_OPERATORS   28  /* number of operators defined */



/* Device input/output modes */
#define DEVMODE_CLOSED     0
#define DEVMODE_INPUT      1
#define DEVMODE_OUTPUT     2
#define DEVMODE_RANDOM     4
#define DEVMODE_APPEND     8
#define DEVMODE_BINARY     32
#define DEVMODE_READ    (DEVMODE_INPUT | DEVMODE_RANDOM | DEVMODE_BINARY )
#define DEVMODE_WRITE   (DEVMODE_OUTPUT | DEVMODE_RANDOM | DEVMODE_APPEND | DEVMODE_BINARY )



#define CONSOLE_FILE_NUMBER  0x00
#define  LPRINT_FILE_NUMBER  0xFF



/* codes for EXEC stack and for function-sub-label lookup table */
#define EXEC_NORM        0
#define EXEC_GOSUB       1
#define EXEC_WHILE       2
#define EXEC_FOR         3
#define EXEC_FUNCTION    4
#define EXEC_CALLSUB     5
#define EXEC_IFTRUE      6
#define EXEC_IFFALSE     7
#define EXEC_MAIN        8
#define EXEC_SELTRUE     9
#define EXEC_SELFALSE   10
#define EXEC_LABEL      11
#define EXEC_DO         12
#define EXEC_ON         13
#define EXEC_UNTIL      14



/* OPTION VERSION bitmask, upto 32 flavors of BASIC */
#define E78 0x00000001     /* ECMA-55                 , 1978 */
#define E86 0x00000002     /* ECMA-116                , 1986 */
#define M80 0x00000004     /* Microsoft BASIC-80      , 1980 */
#define M85 0x00000008     /* Microsoft GW-BASIC      , 1985 */
#define M88 0x00000010     /* Microsoft QBASIC        , 1988 */
#define M90 0x00000020     /* Microsoft VBDOS         , 1990 */
#define M91 0x00000040     /* Microsoft VB1           , 1991 */
#define M92 0x00000080     /* Microsoft VB2           , 1992 */
#define M93 0x00000100     /* Microsoft VB3           , 1993 */
#define M95 0x00000200     /* Microsoft VB4           , 1995 */
#define M97 0x00000400     /* Microsoft VB5           , 1997 */
#define M98 0x00000800     /* Microsoft VB6           , 1998 */
#define D64 0x00001000     /* Dartmouth               , 1964 */
#define T83 0x00002000     /* TRS-80 Model III        , 1980 */
#define T84 0x00004000     /* TRS-80 Model 4          , 1983 */
#define B14 0x80000000     /* Bywater BASIC           , 2014 */
#define NUM_VERSIONS 16    /* upto 32 flavors of BASIC */



/* OptionFlags */
#define OPTION_STRICT_ON     0x0001   /* Do NOT allow implicit DIM */
#define OPTION_ANGLE_DEGREES 0x0002   /* Use degrees instead of radians */
#define OPTION_BUGS_ON       0x0004   /* ON ... , FOR ... */
#define OPTION_LABELS_ON     0x0008   /* Labels allowed */
#define OPTION_COMPARE_TEXT  0x0010   /* Case-Insensitive string comparison */
#define OPTION_BASE_ONE      0x0020   /* Lower array bound is 1 */
#define OPTION_COVERAGE_ON   0x0040   /* track BASIC lines actually
                * executed */
#define OPTION_TRACE_ON      0x0080   /* dump BASIC stack trace when FATAL
                * error */
#define OPTION_ERROR_GOSUB   0X0100   /* error causes GOSUB instead of GOTO */

/* LineFlags */
#define LINE_EXECUTED        0x01   /* line was executed */
#define LINE_NUMBERED        0x02   /* line was manually numbered */
#define LINE_RESERVED04      0x04   /* reserved for future use */
#define LINE_RESERVED08      0x08   /* reserved for future use */
#define LINE_RESERVED10      0x10   /* reserved for future use */
#define LINE_RESERVED20      0x20   /* reserved for future use */
#define LINE_RESERVED40      0x40   /* reserved for future use */
#define LINE_RESERVED80      0x80   /* reserved for future use */


#define ERROR_PENDING   ( bwb_Warning_Pending() || (err_number < 0) )

/* function ParameterTypes */

#define PSTR( N ) ( 1 << ( N - 1 ) )   /* STRING parameter # 1 is bit 0 SET */
#define PNUM( N ) ( 0              )   /* NUMBER parameter # 1 is bit 0 CLR */
#define PNONE     ( 0              )   /* function has no parameters */

#define P1STR PSTR(1)
#define P2STR PSTR(2)
#define P3STR PSTR(3)
#define P4STR PSTR(4)

#define P1NUM PNUM(1)
#define P2NUM PNUM(2)
#define P3NUM PNUM(3)
#define P4NUM PNUM(4)



/* function ParameterTests */
/*            0x87654321 <- Parameter Number (max # of Range-Checked parameters to INTRINSIC functions is 8) */
#define P1ERR 0X00000000   /* INTERNAL ERROR */
#define P1ANY 0X00000001   /* X is any valid number  , A$ is any valid
             * string      */
#define P1BYT 0x00000002   /* MIN_BYT <= X <= MAX_BYT, LEN(A$) >=
             * sizeof(char)     */
#define P1INT 0x00000003   /* INT_MIN <= X <= INT_MAX, LEN(A$) >=
             * sizeof(short)      */
#define P1LNG 0x00000004   /* LONG_MIN <= X <= LONG_MAX, LEN(A$) >=
             * sizeof(long)     */
#define P1CUR 0x00000005   /* LONG_MIN <= X <= LONG_MAX, LEN(A$) >=
             * sizeof(long) */
#define P1FLT 0x00000006   /* MIN_FLT <= X <= MAX_FLT, LEN(A$) >=
             * sizeof(float)    */
#define P1DBL 0x00000007   /* MIN_DBL <= X <= MAX_DBL, LEN(A$) >=
             * sizeof(double)   */
#define P1DEV 0x00000008   /* MIN_DEV <= X <= MAX_DEV, RESERVED */
#define P1LEN 0x00000009   /* MIN_STR <= X <= MAX_STR, RESERVED */
#define P1POS 0x0000000A   /* MIN_STR <  X <= MAX_STR, RESERVED */
#define P1COM 0x0000000B   /* X in (1,2,3,4) COMx    , RESERVED */
#define P1LPT 0x0000000C   /* X in (0,1,2,3) PRN,LPTx, RESERVED */
#define P1GTZ 0x0000000D   /* X  >    0              , RESERVED */
#define P1GEZ 0x0000000E   /* X  >=   0              , RESERVED */
#define P1NEZ 0x0000000F   /* X  <>   0              , RESERVED */





#define P2ERR (P1ERR << 4)
#define P2ANY (P1ANY << 4)
#define P2BYT (P1BYT << 4)
#define P2INT (P1INT << 4)
#define P2LNG (P1LNG << 4)
#define P2CUR (P1CUR << 4)
#define P2FLT (P1FLT << 4)
#define P2DBL (P1DBL << 4)
#define P2DEV (P1DEV << 4)
#define P2LEN (P1LEN << 4)
#define P2POS (P1POS << 4)
#define P2COM (P1COM << 4)
#define P2LPT (P1LPT << 4)
#define P2GTZ (P1GTZ << 4)
#define P2GEZ (P1GEZ << 4)
#define P2NEZ (P1NEZ << 4)

#define P3ERR (P1ERR << 8)
#define P3ANY (P1ANY << 8)
#define P3BYT (P1BYT << 8)
#define P3INT (P1INT << 8)
#define P3LNG (P1LNG << 8)
#define P3CUR (P1CUR << 8)
#define P3FLT (P1FLT << 8)
#define P3DBL (P1DBL << 8)
#define P3DEV (P1DEV << 8)
#define P3LEN (P1LEN << 8)
#define P3POS (P1POS << 8)
#define P3COM (P1COM << 8)
#define P3LPT (P1LPT << 8)
#define P3GTZ (P1GTZ << 8)
#define P3GEZ (P1GEZ << 8)
#define P3NEZ (P1NEZ << 8)

#define P4ERR (P1ERR << 12)
#define P4ANY (P1ANY << 12)
#define P4BYT (P1BYT << 12)
#define P4INT (P1INT << 12)
#define P4LNG (P1LNG << 12)
#define P4CUR (P1CUR << 12)
#define P4FLT (P1FLT << 12)
#define P4DBL (P1DBL << 12)
#define P4DEV (P1DEV << 12)
#define P4LEN (P1LEN << 12)
#define P4POS (P1POS << 12)
#define P4COM (P1COM << 12)
#define P4LPT (P1LPT << 12)
#define P4GTZ (P1GTZ << 12)
#define P4GEZ (P1GEZ << 12)
#define P4NEZ (P1NEZ << 12)






/* COMMANDS */
#define C_QUEST                          1 /* ?                              */
#define C_CALL                           2 /* CALL                           */
#define C_CASE                           3 /* CASE                           */
#define C_CASE_ELSE                      4 /* CASE ELSE                      */
#define C_CASE_IF                        5 /* CASE IF                        */
#define C_CASE_IS                        6 /* CASE IS                        */
#define C_CHAIN                          7 /* CHAIN                          */
#define C_CHANGE                         8 /* CHANGE                         */
#define C_CLEAR                          9 /* CLEAR                          */
#define C_CLOAD                         10 /* CLOAD                          */
#define C_CLOAD_                        11 /* CLOAD*                         */
#define C_CMDS                          12 /* CMDS                           */
#define C_COMMON                        13 /* COMMON                         */
#define C_CONT                          14 /* CONT                           */
#define C_CSAVE                         15 /* CSAVE                          */
#define C_CSAVE_                        16 /* CSAVE*                         */
#define C_DATA                          17 /* DATA                           */
#define C_DEF                           18 /* DEF                            */
#define C_DEF_SUB                       19 /* DEF SUB                        */
#define C_DEFDBL                        20 /* DEFDBL                         */
#define C_DEFINT                        21 /* DEFINT                         */
#define C_DEFSNG                        22 /* DEFSNG                         */
#define C_DEFSTR                        23 /* DEFSTR                         */
#define C_DELETE                        24 /* DELETE                         */
#define C_DIM                           25 /* DIM                            */
#define C_DO                            26 /* DO                             */
#define C_DO_UNTIL                      27 /* DO UNTIL                       */
#define C_DO_WHILE                      28 /* DO WHILE                       */
#define C_EDIT                          29 /* EDIT                           */
#define C_ELSE                          30 /* ELSE                           */
#define C_ELSEIF                        31 /* ELSEIF                         */
#define C_END                           32 /* END                            */
#define C_END_FUNCTION                  33 /* END FUNCTION                   */
#define C_END_IF                        34 /* END IF                         */
#define C_END_SELECT                    35 /* END SELECT                     */
#define C_END_SUB                       36 /* END SUB                        */
#define C_ERASE                         37 /* ERASE                          */
#define C_EXIT                          38 /* EXIT                           */
#define C_EXIT_DO                       39 /* EXIT DO                        */
#define C_EXIT_FOR                      40 /* EXIT FOR                       */
#define C_EXIT_FUNCTION                 41 /* EXIT FUNCTION                  */
#define C_EXIT_SUB                      42 /* EXIT SUB                       */
#define C_EXIT_UNTIL                    43 /* EXIT UNTIL                     */
#define C_EXIT_WHILE                    44 /* EXIT WHILE                     */
#define C_FIELD                         45 /* FIELD                          */
#define C_FNCS                          46 /* FNCS                           */
#define C_FOR                           47 /* FOR                            */
#define C_FUNCTION                      48 /* FUNCTION                       */
#define C_GO                            49 /* GO                             */
#define C_GOSUB                         50 /* GOSUB                          */
#define C_GOTO                          51 /* GOTO                           */
#define C_HELP                          52 /* HELP                           */
#define C_IF                            53 /* IF                             */
#define C_IF_THEN                       54 /* IF THEN                        */
#define C_INPUT                         55 /* INPUT                          */
#define C_LET                           56 /* LET                            */
#define C_LINE                          57 /* LINE                           */
#define C_LIST                          58 /* LIST                           */
#define C_LOAD                          59 /* LOAD                           */
#define C_LOOP                          60 /* LOOP                           */
#define C_LOOP_UNTIL                    61 /* LOOP UNTIL                     */
#define C_LOOP_WHILE                    62 /* LOOP WHILE                     */
#define C_LPRINT                        63 /* LPRINT                         */
#define C_LSET                          64 /* LSET                           */
#define C_MAINTAINER                    65 /* MAINTAINER                     */
#define C_MERGE                         66 /* MERGE                          */
#define C_MID_                          67 /* MID$                           */
#define C_NAME                          68 /* NAME                           */
#define C_NEW                           69 /* NEW                            */
#define C_NEXT                          70 /* NEXT                           */
#define C_ON                            71 /* ON                             */
#define C_ON_ERROR_GOTO                 72 /* ON ERROR GOTO                  */
#define C_ON_TIMER                      73 /* ON TIMER                       */
#define C_OPEN                          74 /* OPEN                           */
#define C_OPTION                        75 /* OPTION                         */
#define C_OPTION_ANGLE_DEGREES          76 /* OPTION ANGLE DEGREES           */
#define C_OPTION_ANGLE_RADIANS          77 /* OPTION ANGLE RADIANS           */
#define C_OPTION_ARITHMETIC_DECIMAL     78 /* OPTION ARITHMETIC DECIMAL      */
#define C_OPTION_ARITHMETIC_FIXED       79 /* OPTION ARITHMETIC FIXED        */
#define C_OPTION_ARITHMETIC_NATIVE      80 /* OPTION ARITHMETIC NATIVE       */
#define C_OPTION_BASE_0                 81 /* OPTION BASE 0                  */
#define C_OPTION_BASE_1                 82 /* OPTION BASE 1                  */
#define C_OPTION_BUGS_OFF               83 /* OPTION BUGS OFF                */
#define C_OPTION_BUGS_ON                84 /* OPTION BUGS ON                 */
#define C_OPTION_COMMENT                85 /* OPTION COMMENT                 */
#define C_OPTION_COMPARE_BINARY         86 /* OPTION COMPARE BINARY          */
#define C_OPTION_COMPARE_DATABASE       87 /* OPTION COMPARE DATABASE        */
#define C_OPTION_COMPARE_TEXT           88 /* OPTION COMPARE TEXT            */
#define C_OPTION_COVERAGE_OFF           89 /* OPTION COVERAGE OFF            */
#define C_OPTION_COVERAGE_ON            90 /* OPTION COVERAGE ON             */
#define C_OPTION_DATE                   91 /* OPTION DATE                    */
#define C_OPTION_DISABLE_COMMAND        92 /* OPTION DISABLE COMMAND         */
#define C_OPTION_DISABLE_FUNCTION       93 /* OPTION DISABLE FUNCTION        */
#define C_OPTION_DISABLE_OPERATOR       94 /* OPTION DISABLE OPERATOR        */
#define C_OPTION_ENABLE_COMMAND         95 /* OPTION ENABLE COMMAND          */
#define C_OPTION_ENABLE_FUNCTION        96 /* OPTION ENABLE FUNCTION         */
#define C_OPTION_ENABLE_OPERATOR        97 /* OPTION ENABLE OPERATOR         */
#define C_OPTION_INDENT                 98 /* OPTION INDENT                  */
#define C_OPTION_LABELS_OFF             99 /* OPTION LABELS OFF              */
#define C_OPTION_LABELS_ON             100 /* OPTION LABELS ON               */
#define C_OPTION_STATEMENT             101 /* OPTION STATEMENT               */
#define C_OPTION_STRICT_OFF            102 /* OPTION STRICT OFF              */
#define C_OPTION_STRICT_ON             103 /* OPTION STRICT ON               */
#define C_OPTION_TERMINAL_ADM_3A       104 /* OPTION TERMINAL ADM-3A         */
#define C_OPTION_TERMINAL_ANSI         105 /* OPTION TERMINAL ANSI           */
#define C_OPTION_TERMINAL_NONE         106 /* OPTION TERMINAL NONE           */
#define C_OPTION_TIME                  107 /* OPTION TIME                    */
#define C_OPTION_TRACE_OFF             108 /* OPTION TRACE OFF               */
#define C_OPTION_TRACE_ON              109 /* OPTION TRACE ON                */
#define C_OPTION_VERSION               110 /* OPTION VERSION                 */
#define C_PRINT                        111 /* PRINT                          */
#define C_QUIT                         112 /* QUIT                           */
#define C_READ                         113 /* READ                           */
#define C_REM                          114 /* REM                            */
#define C_RENUM                        115 /* RENUM                          */
#define C_RESTORE                      116 /* RESTORE                        */
#define C_RESUME                       117 /* RESUME                         */
#define C_RETURN                       118 /* RETURN                         */
#define C_RSET                         119 /* RSET                           */
#define C_RUN                          120 /* RUN                            */
#define C_SAVE                         121 /* SAVE                           */
#define C_SELECT                       122 /* SELECT                         */
#define C_SELECT_CASE                  123 /* SELECT CASE                    */
#define C_STOP                         124 /* STOP                           */
#define C_SUB                          125 /* SUB                            */
#define C_SWAP                         126 /* SWAP                           */
#define C_SYSTEM                       127 /* SYSTEM                         */
#define C_TIMER                        128 /* TIMER                          */
#define C_UEND                         129 /* UEND                           */
#define C_UNTIL                        130 /* UNTIL                          */
#define C_USER_LBL                     131 /* USER LBL                       */
#define C_VARS                         132 /* VARS                           */
#define C_WEND                         133 /* WEND                           */
#define C_WHILE                        134 /* WHILE                          */
#define C_WRITE                        135 /* WRITE                          */
#define C_OPTION_ERROR_GOSUB           136 /* OPTION ERROR GOSUB             */
#define C_OPTION_ERROR_GOTO            137 /* OPTION ERROR GOTO              */
#define C_ON_ERROR_GOSUB               138 /* ON ERROR GOSUB                 */
#define C_ON_ERROR_RETURN_NEXT         139 /* ON ERROR RESUME NEXT           */
#define C_ON_ERROR_RESUME_NEXT         140 /* ON ERROR RETURN NEXT           */
#define C_TIMER_OFF                    141 /* TIMER OFF                      */
#define C_TIMER_ON                     142 /* TIMER ON                       */
#define C_TIMER_STOP                   143 /* TIMER STOP                     */
#define NUM_COMMANDS 143





/* FUNCTIONS */
#define F_ABS_X_N                        1 /* N  = ABS( X )                  */
#define F_ACOS_X_N                       2 /* N  = ACOS( X )                 */
#define F_ANGLE_X_Y_N                    3 /* N  = ANGLE( X, Y )             */
#define F_ARGC_N                         4 /* N  = ARGC                      */
#define F_ARGT_X_S                       5 /* S$ = ARGT$( X )                */
#define F_ARGV_X_N                       6 /* N  = ARGV( X )                 */
#define F_ARGV_X_S                       7 /* S$ = ARGV$( X )                */
#define F_ASC_A_N                        8 /* N  = ASC( A$ )                 */
#define F_ASIN_X_N                       9 /* N  = ASIN( X )                 */
#define F_ATN_X_N                       10 /* N  = ATN( X )                  */
#define F_CCUR_X_N                      11 /* N  = CCUR( X )                 */
#define F_CDBL_X_N                      12 /* N  = CDBL( X )                 */
#define F_CEIL_X_N                      13 /* N  = CEIL( X )                 */
#define F_CHDIR_A_N                     14 /* N  = CHDIR( A$ )               */
#define F_CHR_X_S                       15 /* S$ = CHR$( X )                 */
#define F_CINT_X_N                      16 /* N  = CINT( X )                 */
#define F_CLNG_X_N                      17 /* N  = CLNG( X )                 */
#define F_CLOSE_X_N                     18 /* N  = CLOSE( X )                */
#define F_CLS_N                         19 /* N  = CLS                       */
#define F_COLOR_X_Y_N                   20 /* N  = COLOR( X, Y )             */
#define F_COS_X_N                       21 /* N  = COS( X )                  */
#define F_COSH_X_N                      22 /* N  = COSH( X )                 */
#define F_COT_X_N                       23 /* N  = COT( X )                  */
#define F_CSC_X_N                       24 /* N  = CSC( X )                  */
#define F_CSNG_X_N                      25 /* N  = CSNG( X )                 */
#define F_CVC_A_N                       26 /* N  = CVC( A$ )                 */
#define F_CVD_A_N                       27 /* N  = CVD( A$ )                 */
#define F_CVI_A_N                       28 /* N  = CVI( A$ )                 */
#define F_CVL_A_N                       29 /* N  = CVL( A$ )                 */
#define F_CVS_A_N                       30 /* N  = CVS( A$ )                 */
#define F_DATE_N                        31 /* N  = DATE                      */
#define F_DATE_S                        32 /* S$ = DATE$                     */
#define F_DEF_FN_N                      33 /* N  = DEF FN                    */
#define F_DEG_X_N                       34 /* N  = DEG( X )                  */
#define F_ENVIRON_A_N                   35 /* N  = ENVIRON( A$ )             */
#define F_ENVIRON_A_S                   36 /* S$ = ENVIRON$( A$ )            */
#define F_EOF_X_N                       37 /* N  = EOF( X )                  */
#define F_EPS_X_N                       38 /* N  = EPS( X )                  */
#define F_ERL_N                         39 /* N  = ERL                       */
#define F_ERR_N                         40 /* N  = ERR                       */
#define F_ERR_S                         41 /* S$ = ERR$                      */
#define F_ERROR_X_N                     42 /* N  = ERROR( X )                */
#define F_ERROR_X_A_N                   43 /* N  = ERROR( X, A$ )            */
#define F_EXP_X_N                       44 /* N  = EXP( X )                  */
#define F_FILEATTR_X_Y_N                45 /* N  = FILEATTR( X, Y )          */
#define F_FILES_N                       46 /* N  = FILES                     */
#define F_FILES_A_N                     47 /* N  = FILES( A$ )               */
#define F_FIX_X_N                       48 /* N  = FIX( X )                  */
#define F_FP_X_N                        49 /* N  = FP( X )                   */
#define F_FRE_N                         50 /* N  = FRE                       */
#define F_FRE_X_N                       51 /* N  = FRE( X )                  */
#define F_FRE_A_N                       52 /* N  = FRE( A$ )                 */
#define F_FREEFILE_N                    53 /* N  = FREEFILE                  */
#define F_GET_X_N                       54 /* N  = GET( X )                  */
#define F_GET_X_Y_N                     55 /* N  = GET( X, Y )               */
#define F_HEX_X_S                       56 /* S$ = HEX$( X )                 */
#define F_INKEY_S                       57 /* S$ = INKEY$                    */
#define F_INP_X_N                       58 /* N  = INP( X )                  */
#define F_INPUT_X_S                     59 /* S$ = INPUT$( X )               */
#define F_INPUT_X_Y_S                   60 /* S$ = INPUT$( X, Y )            */
#define F_INSTR_A_B_N                   61 /* N  = INSTR( A$, B$ )           */
#define F_INSTR_X_A_B_N                 62 /* N  = INSTR( X, A$, B$ )        */
#define F_INT_X_N                       63 /* N  = INT( X )                  */
#define F_IP_X_N                        64 /* N  = IP( X )                   */
#define F_KILL_A_N                      65 /* N  = KILL( A$ )                */
#define F_LCASE_A_S                     66 /* S$ = LCASE$( A$ )              */
#define F_LEFT_A_X_S                    67 /* S$ = LEFT$( A$, X )            */
#define F_LEN_A_N                       68 /* N  = LEN( A$ )                 */
#define F_LOC_X_N                       69 /* N  = LOC( X )                  */
#define F_LOCATE_X_Y_N                  70 /* N  = LOCATE( X, Y )            */
#define F_LOF_X_N                       71 /* N  = LOF( X )                  */
#define F_LOG_X_N                       72 /* N  = LOG( X )                  */
#define F_LOG10_X_N                     73 /* N  = LOG10( X )                */
#define F_LOG2_X_N                      74 /* N  = LOG2( X )                 */
#define F_LPOS_N                        75 /* N  = LPOS                      */
#define F_LTRIM_A_S                     76 /* S$ = LTRIM$( A$ )              */
#define F_LWIDTH_X_N                    77 /* N  = LWIDTH( X )               */
#define F_MAX_X_Y_N                     78 /* N  = MAX( X, Y )               */
#define F_MAXBYT_N                      79 /* N  = MAXBYT                    */
#define F_MAXCUR_N                      80 /* N  = MAXCUR                    */
#define F_MAXDBL_N                      81 /* N  = MAXDBL                    */
#define F_MAXDEV_N                      82 /* N  = MAXDEV                    */
#define F_MAXINT_N                      83 /* N  = MAXINT                    */
#define F_MAXLEN_A_N                    84 /* N  = MAXLEN( A$ )              */
#define F_MAXLNG_N                      85 /* N  = MAXLNG                    */
#define F_MAXLVL_N                      86 /* N  = MAXLVL                    */
#define F_MAXNUM_N                      87 /* N  = MAXNUM                    */
#define F_MAXSNG_N                      88 /* N  = MAXSNG                    */
#define F_MID_A_X_S                     89 /* S$ = MID$( A$, X )             */
#define F_MID_A_X_Y_S                   90 /* S$ = MID$( A$, X, Y )          */
#define F_MIN_X_Y_N                     91 /* N  = MIN( X, Y )               */
#define F_MINBYT_N                      92 /* N  = MINBYT                    */
#define F_MINCUR_N                      93 /* N  = MINCUR                    */
#define F_MINDBL_N                      94 /* N  = MINDBL                    */
#define F_MINDEV_N                      95 /* N  = MINDEV                    */
#define F_MININT_N                      96 /* N  = MININT                    */
#define F_MINLNG_N                      97 /* N  = MINLNG                    */
#define F_MINNUM_N                      98 /* N  = MINNUM                    */
#define F_MINSNG_N                      99 /* N  = MINSNG                    */
#define F_MKC_X_S                      100 /* S$ = MKC$( X )                 */
#define F_MKD_X_S                      101 /* S$ = MKD$( X )                 */
#define F_MKDIR_A_N                    102 /* N  = MKDIR( A$ )               */
#define F_MKI_X_S                      103 /* S$ = MKI$( X )                 */
#define F_MKL_X_S                      104 /* S$ = MKL$( X )                 */
#define F_MKS_X_S                      105 /* S$ = MKS$( X )                 */
#define F_MOD_X_Y_N                    106 /* N  = MOD( X, Y )               */
#define F_NAME_A_B_N                   107 /* N  = NAME( A$, B$ )            */
#define F_NULL_X_N                     108 /* N  = NULL( X )                 */
#define F_OCT_X_S                      109 /* S$ = OCT$( X )                 */
#define F_OPEN_A_X_B_N                 110 /* N  = OPEN( A$, X, B$ )         */
#define F_OPEN_A_X_B_Y_N               111 /* N  = OPEN( A$, X, B$, Y )      */
#define F_ORD_A_N                      112 /* N  = ORD( A$ )                 */
#define F_OUT_X_Y_N                    113 /* N  = OUT( X, Y )               */
#define F_PEEK_X_N                     114 /* N  = PEEK( X )                 */
#define F_PI_N                         115 /* N  = PI                        */
#define F_POKE_X_Y_N                   116 /* N  = POKE( X, Y )              */
#define F_POS_N                        117 /* N  = POS                       */
#define F_POS_X_N                      118 /* N  = POS( X )                  */
#define F_POS_A_B_N                    119 /* N  = POS( A$, B$ )             */
#define F_POS_A_B_X_N                  120 /* N  = POS( A$, B$, X )          */
#define F_PUT_X_N                      121 /* N  = PUT( X )                  */
#define F_PUT_X_Y_N                    122 /* N  = PUT( X, Y )               */
#define F_RAD_X_N                      123 /* N  = RAD( X )                  */
#define F_RANDOMIZE_N                  124 /* N  = RANDOMIZE                 */
#define F_RANDOMIZE_X_N                125 /* N  = RANDOMIZE( X )            */
#define F_REMAINDER_X_Y_N              126 /* N  = REMAINDER( X, Y )         */
#define F_REPEAT_X_Y_S                 127 /* S$ = REPEAT$( X, Y )           */
#define F_REPEAT_X_A_S                 128 /* S$ = REPEAT$( X, A$ )          */
#define F_RESET_N                      129 /* N  = RESET                     */
#define F_RIGHT_A_X_S                  130 /* S$ = RIGHT$( A$, X )           */
#define F_RMDIR_A_N                    131 /* N  = RMDIR( A$ )               */
#define F_RND_N                        132 /* N  = RND                       */
#define F_RND_X_N                      133 /* N  = RND( X )                  */
#define F_ROUND_X_Y_N                  134 /* N  = ROUND( X, Y )             */
#define F_RTRIM_A_S                    135 /* S$ = RTRIM$( A$ )              */
#define F_SEC_X_N                      136 /* N  = SEC( X )                  */
#define F_SEEK_X_N                     137 /* N  = SEEK( X )                 */
#define F_SEEK_X_Y_N                   138 /* N  = SEEK( X, Y )              */
#define F_SGN_X_N                      139 /* N  = SGN( X )                  */
#define F_SHELL_A_N                    140 /* N  = SHELL( A$ )               */
#define F_SIN_X_N                      141 /* N  = SIN( X )                  */
#define F_SINH_X_N                     142 /* N  = SINH( X )                 */
#define F_SPACE_X_S                    143 /* S$ = SPACE$( X )               */
#define F_SPC_X_S                      144 /* S$ = SPC( X )                  */
#define F_SQR_X_N                      145 /* N  = SQR( X )                  */
#define F_STR_X_S                      146 /* S$ = STR$( X )                 */
#define F_STRING_X_A_S                 147 /* S$ = STRING$( X, A$ )          */
#define F_STRING_X_Y_S                 148 /* S$ = STRING$( X, Y )           */
#define F_TAB_X_S                      149 /* S$ = TAB( X )                  */
#define F_TAN_X_N                      150 /* N  = TAN( X )                  */
#define F_TANH_X_N                     151 /* N  = TANH( X )                 */
#define F_TIME_N                       152 /* N  = TIME                      */
#define F_TIME_S                       153 /* S$ = TIME$                     */
#define F_TIMER_N                      154 /* N  = TIMER                     */
#define F_TRIM_A_S                     155 /* S$ = TRIM$( A$ )               */
#define F_TROFF_N                      156 /* N  = TROFF                     */
#define F_TRON_N                       157 /* N  = TRON                      */
#define F_TRUNCATE_X_Y_N               158 /* N  = TRUNCATE( X, Y )          */
#define F_UCASE_A_S                    159 /* S$ = UCASE$( A$ )              */
#define F_VAL_A_N                      160 /* N  = VAL( A$ )                 */
#define F_WAIT_X_Y_N                   161 /* N  = WAIT( X, Y )              */
#define F_WAIT_X_Y_Z_N                 162 /* N  = WAIT( X, Y, Z )           */
#define F_WIDTH_X_N                    163 /* N  = WIDTH( X )                */
#define F_WIDTH_X_Y_N                  164 /* N  = WIDTH( X, Y )             */
#define NUM_FUNCTIONS 164






/***************************************************************
  
        bwb_mes.h       Header File for Natural-Language-Specific
                        Text Messages for Bywater BASIC Interpreter
  
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
/*---------------------------------------------------------------*/


/* Standard English is taken as a default: if MES_SIGNON is not defined by
   this time (i.e., by some other language definition), then
   the following standard English definitions are utilized. */

#ifndef MES_SIGNON
#define MES_SIGNON      "Bywater BASIC Interpreter, version"
#define MES_COPYRIGHT_1 "Copyright (c) 1993, Ted A. Campbell"
#define MES_COPYRIGHT_2 "Copyright (c) 1995-1997, Jon B. Volkoff"
#define MES_COPYRIGHT_3 "Copyright (c) 2014-2015, Howard Wulf, AF5NE"
#define MES_LANGUAGE    " "
#define ERROR_HEADER    "ERROR in line"
#define ERRD_HEADER     "ERROR"
#define MATHERR_HEADER  "ERROR"
#define MES_BREAK       "Program interrupted at line"
#define ERR_OPENFILE    "Failed to open file %s"
#define ERR_GETMEM      "Failed to find memory"
#define ERR_LINENO      "Failed to link line number"
#define ERR_LNNOTFOUND  "Line number %d not found"
#define ERR_LOADNOFN    "LOAD: no filename specified"
#define ERR_NOLN        "No line number"
#define ERR_NOFN        "No file name"
#define ERR_RETNOGOSUB  "RETURN without GOSUB"
#define ERR_INCOMPLETE  "Incomplete statement"
#define ERR_ONNOGOTO    "ON without GOTO or GOSUB"
#define ERR_VALOORANGE  "Value is out of range"
#define ERR_SYNTAX      "Syntax error"
#define ERR_DEVNUM      "Invalid device number"
#define ERR_DEV         "Device error"
#define ERR_OPSYS       "Error in operating system command"
#define ERR_ARGSTR      "Argument must be a string"
#define ERR_DEFCHAR     "Incorrect argument for variable definition"
#define ERR_MISMATCH    "Type mismatch"
#define ERR_DIMNOTARRAY "Argument is not an array name"
#define ERR_OD          "Out of data"
#define ERR_OVERFLOW    "Overflow"
#define ERR_NF          "NEXT without FOR"
#define ERR_UF          "Undefined function"
#define ERR_DBZ         "Divide by zero"
#define ERR_REDIM       "Variable cannot be redimensioned"
#define ERR_OBDIM       "OPTION BASE must be called prior to DIM"
#define ERR_UC          "Unknown command"
#define ERR_NOPROGFILE  "Program file not specified"
#endif










/***************************************************************
  
   bwbasic.h   Part II: Structures
  
***************************************************************/
typedef unsigned long OptionVersionType;  /* OPTION VERSION bitmask,
                   * upto 32 flavors of BASIC */



/* Typdef structure for strings under Bywater BASIC */

typedef struct       /* bstr */
{
   /* unsigned int was unsigned char (JBV 9/4/97) */
   BasicStringLengthType length; /* length of string */
   char           *sbuffer;/* pointer to string buffer */
   int             rab; /* is it a random-access buffer? */
}               bstring;

/* Structure used for all variables under Bywater BASIC */

struct bwb_variable
{
   char            name[BasicNameLengthMax + 1];   /* name */
   int             type;   /* type, i.e., STRING or NUMBER */
   BasicNumberType *memnum;/* memory for number */
   bstring        *memstr; /* memory for string */
   size_t          array_units;  /* total number of units of memory, 1
                * == scalar */
   int            *array_sizes;  /* pointer to array of <dimensions>
                * integers, with sizes of each
                * dimension */
   int            *array_pos; /* current position in array */
   int             dimensions;   /* number of dimensions, 0 = not an
                * array */
   struct bwb_variable *next; /* next variable in chain */
   int             common; /* should this variable be common to chained
             * programs? */
   int             preset; /* preset variable: CLEAR should not alter */
   int             IsInDim;
};

/* Structure to represent program lines under Bywater BASIC */

struct bwb_line
{
   struct bwb_line *next;  /* pointer to next line in chain */
   BasicLineNumberType number;   /* line number */
   char           *buffer; /* buffer to hold the line */
   int             position;  /* current position in line */
   BasicUniqueType cmdnum; /* C_... */
   int             Startpos;  /* start of rest of line read from
                * buffer */
   struct bwb_line *OtherLine;   /* pointer to other line in loops */
   int             Indention;
   unsigned char   LineFlags;  /* '*' if executed, ' ' otherwise */
};

/* Structure used for all INTRINSIC functions under Bywater BASIC */

struct bwb_function
{
   BasicUniqueType UniqueID;  /* If USER, then LineNumber, else
                * F_... */
   const char     *Syntax;
   const char     *Description;
   char            Name[BasicNameLengthMax + 1];
   unsigned int    ReturnType;   /* MSB: 0x00==INTRINSIC, l->cmdnum;
                * LSB == STRING or NUMBER */
   unsigned char   ParameterCount;  /* 0..MAX_FARGS, 255 == VARIANT (...) */
   unsigned long   ParameterTypes;  /* parameter signature, must hold
                * MAX_FARGS bits */
   unsigned long   ParameterTests;  /* parameter checks   , must hold 8
                * nibbles, only first 8 parameters */
   struct bwb_function *next; /* NextPointer */
   OptionVersionType OptionVersionBitmask;   /* OPTION VERSION bitmask */
};

/* Structure to represent all INTRINSIC commands under Bywater BASIC */

struct bwb_command
{
   BasicUniqueType id;  /* C_... */
   const char     *Syntax;
   const char     *Description;
   char            name[BasicNameLengthMax + 1];
   OptionVersionType OptionVersionBitmask;   /* OPTION VERSION bitmask */
};


/* Structure to define DEVICE (file) for Bywater BASIC */

struct dev_element
{
   int             mode;   /* DEVMODE_ item */
   int             width;  /* width for OUTPUT (and APPEND); record
             * length for RANDOM; not used for INPUT or
             * BINARY */
   int             col; /* current column */
   char            filename[FILENAME_MAX + 1];  /* filename */
   FILE           *cfp; /* C file pointer for this device */
   char           *buffer; /* pointer to character buffer for RANDOM */
};

/* Structure to define expression stack elements under Bywater BASIC */

struct exp_ese
{
   int             operation; /* operation at this level */
   char            type;   /* type of operation at this level: STRING or
             * NUMBER */
   bstring         sval;   /* string */
   BasicNumberType nval;   /* number */
   char            string[BasicStringLengthMax + 1];  /* string for writing */
   struct bwb_variable *xvar; /* pointer to external variable */
   struct bwb_function *function;   /* pointer to function structure */
   int             array_pos[MAX_DIMS];   /* array for variable
                   * positions */
   int             pos_adv;/* position advanced in string */
   int             rec_pos;/* position marker for recursive calls */
};

/* structure for FUNCTION-SUB lookup table element */

struct fslte
{
   char           *name;
   unsigned char   ParameterCount;  /* 0..MAX_FARGS, 255 == VARIANT (...) */
   unsigned long   ParameterTypes;  /* parameter signature, must hold
                * MAX_FARGS bits */
   struct bwb_line *line;
   int             startpos;  /* starting position in line */
   struct fslte   *next;
   struct bwb_variable *local_variable;
   OptionVersionType OptionVersionBitmask;   /* OPTION VERSION bitmask */
};

/* Structure to define EXEC stack elements */

struct exse
{
   struct bwb_line *line;  /* line for execution */
   int             code;   /* EXEC_... */
   struct bwb_variable *local_variable;   /* local variable chain and
                   * current FOR counter
                   * variable */
   BasicNumberType for_step;  /* STEP value of FOR */
   BasicNumberType for_target;   /* target value of FOR */
   struct bwb_line *LoopTopLine; /* top line of FOR, DO, WHILE, UNTIL */
   struct bwb_line *LoopBottomLine; /* bottom line of NEXT, LOOP,
                   * WEND, UEND */
   BasicLineNumberType OnErrorGoto; /* ON ERROR GOTO line */
   struct exp_ese  expression;   /* expression of SELECT CASE */
};


/* Structure to define bwBASIC task: REMOVED */

/* Structure to define OPERATORS, such as +, -, *, /, and so on */

struct bwb_op
{
   const char     *symbol /* [ 8 ] */ ;   /* BASIC symbol for the
                   * operator */
   const int       operation; /* internal code for the operator */
   const int       precedence;   /* level of precedence, 0 = highest */
   OptionVersionType OptionVersionBitmask;   /* OPTION VERSION
                      * bitmask */
};

/* Structure to define VERSION information, for OPTION VERSION command */

struct bwb_version
{
   char           *Name;
   OptionVersionType OptionVersionBitmask;   /* OPTION VERSION bitmask */
   char           *Description;
   unsigned short  Year;
   char           *ID;
   char            OptionCommentChar;
   char            OptionStatementChar;
   unsigned short  OptionFlags;
   char           *OptionDateFormat;
   char           *OptionTimeFormat;
};




/***************************************************************
  
   bwbasic.h   Part III: Global Data
  
***************************************************************/



extern struct bwb_variable *ed;  /* EDITOR$ variable */
extern struct bwb_variable *fi;  /* FILES$ variable */
extern struct bwb_variable *pr;  /* PROMPT$ variable */
extern struct bwb_variable *im;  /* IMPLEMENTATION$ variable */
extern struct bwb_variable *co;  /* COLORS variable */

extern char     progfile[FILENAME_MAX + 1];  /* program filename */
extern int      rescan;    /* program needs to be rescanned */
extern int      number;    /* current line number */
extern struct bwb_line *bwb_l;   /* current line pointer */
extern struct bwb_line bwb_start;   /* starting line marker */
extern struct bwb_line bwb_end;  /* ending line marker */
extern struct bwb_line *data_line;  /* current line to read data */
extern int      data_pos;  /* position in data_line */
extern struct bwb_variable var_start;  /* variable list start marker */
extern struct bwb_variable var_end; /* variable list end marker */
extern struct bwb_function fnc_start;  /* function list start marker */
extern struct bwb_function fnc_end; /* function list end marker */
extern struct fslte fslt_start;  /* function-sub-label lookup table start
             * marker */
extern struct fslte fslt_end; /* function-sub-label lookup table end marker */
extern int      exsc;      /* EXEC stack counter */
extern int      expsc;     /* expression stack counter */
extern struct exse *excs;  /* EXEC stack */
extern struct exp_ese *exps;  /* Expression stack */
extern int      bwb_curtask;  /* current task */

extern char    *bwb_ebuf;
extern int      bwb_trace;
extern int      dim_base;  /* set by OPTION BASE */
extern int      option_degrees;  /* set by OPTION ANGLE */
extern FILE    *errfdevice;   /* output device for error messages */
extern struct   bwb_line *err_line;  /* ERL */
extern int      err_number;   /* ERR */
#if 0
extern char     err_gosubl[BasicNameLengthMax + 1];   /* line for error GOSUB */
extern int      err_fired;
#endif
extern char     ErrMsg[256];  /* ERR$ */
extern int      stopped_line;
extern struct dev_element *dev_table;  /* table of devices */
extern struct bwb_op exp_ops[NUM_OPERATORS]; /* the table itself, filled
                   * in in bwb_tbl.c */
extern struct bwb_command bwb_cmdtable[NUM_COMMANDS]; /* */
extern struct bwb_function bwb_prefuncs[NUM_FUNCTIONS];  /* table of predefined
                      * functions */
/* OptionVersionBitmask values for compatibility via OPTION VERSION ... */
extern struct bwb_version bwb_vertable[NUM_VERSIONS];
extern OptionVersionType OptionVersion;   /* VersionBitmask for compatibility
                * via OPTION VERSION ... */
extern char     OptionCommentChar;  /* ', ! */
extern char     OptionStatementChar; /* :, \ */
extern unsigned short OptionFlags;
extern char     OptionDateFormat[81];
extern char     OptionTimeFormat[81];
extern int      OptionIndentValue;
extern int      OptionTerminalType;

extern BasicLineNumberType tmr_gotol;
extern BasicNumberType tmr_count;
extern BasicNumberType tmr_expires;

extern int      IsInteractive;
extern int      IsCommandLineFile;
extern int      LPRINT_COLUMN;   /* LPRINT_FILE_NUMBER */
extern int      LPRINT_WIDTH; /* LPRINT_FILE_NUMBER */
extern int      LPRINT_NULLS; /* LPRINT_FILE_NUMBER */
extern jmp_buf  mark;

#if AUTOMATED_REGRESSION
extern FILE    *ExternalInputFile;  /* for automated testing - INPUT
                * responses */
#endif            /* AUTOMATED_REGRESSION */
extern int      MaintainerDebugOn;
extern int      DefaultVariableType[26];  /* bwb_variable->type, A-Z */



/****************************************************************
  
   External Definitions for Error Messages
  
****************************************************************/
extern char     err_openfile[];
extern char     err_getmem[];
extern char     err_noln[];
extern char     err_nofn[];
extern char     err_lnnotfound[];
extern char     err_incomplete[];
extern char     err_valoorange[];
extern char     err_syntax[];
extern char     err_devnum[];
extern char     err_dev[];
extern char     err_opsys[];
extern char     err_argstr[];
extern char     err_defchar[];
extern char     err_mismatch[];
extern char     err_dimnotarray[];
extern char     err_retnogosub[];
extern char     err_od[];
extern char     err_overflow[];
extern char     err_nf[];
extern char     err_uf[];
extern char     err_dbz[];
extern char     err_redim[];
extern char     err_obdim[];
extern char     err_uc[];
extern char     err_noprogfile[];



/***************************************************************
  
   bwbasic.h   Part IV: Function Prototypes
  
***************************************************************/



extern BasicNumberType
fnc_timer(BasicNumberType Seconds);
extern void
bwb_Timer_On(void);
extern void
bwb_Timer_Off(void);
extern struct bwb_line *
bwb_ON_TIMER(struct bwb_line * l);
extern struct bwb_line *
bwb_TIMER(struct bwb_line * l);
extern struct bwb_line *
bwb_TIMER_OFF(struct bwb_line * l);
extern struct bwb_line *
bwb_TIMER_ON(struct bwb_line * l);
extern struct bwb_line *
bwb_TIMER_STOP(struct bwb_line * l);
extern int
adv_element(char *buffer, int *pos, char *element);
extern int
adv_eos(char *buffer, int *pos);
extern int
adv_ws(char *buffer, int *pos);
extern void
BasicNumerc(BasicNumberType Input, char *Output);
extern void
break_mes(int x);
extern struct bwb_line *
bwb_CALL(struct bwb_line * l);
extern struct bwb_line *
bwb_CASE(struct bwb_line * l);
extern struct bwb_line *
bwb_CASE_ELSE(struct bwb_line * l);
extern struct bwb_line *
bwb_CASE_IF(struct bwb_line * l);
extern struct bwb_line *
bwb_CASE_IS(struct bwb_line * l);
extern struct bwb_line *
bwb_CHAIN(struct bwb_line * l);
extern struct bwb_line *
bwb_CHANGE(struct bwb_line * l);
extern struct bwb_line *
bwb_CLEAR(struct bwb_line * l);
extern struct bwb_line *
bwb_CLOAD(struct bwb_line * l);
extern struct bwb_line *
bwb_CLOAD_(struct bwb_line * l);
extern struct bwb_line *
bwb_CMDS(struct bwb_line * l);
extern struct bwb_line *
bwb_COMMON(struct bwb_line * l);
extern int
bwb_const(char *lb, char *sb, int *n);
extern struct bwb_line *
bwb_CONT(struct bwb_line * l);
extern struct bwb_line *
bwb_CSAVE(struct bwb_line * l);
extern struct bwb_line *
bwb_CSAVE_(struct bwb_line * l);
extern struct bwb_line *
bwb_DATA(struct bwb_line * l);
extern void
bwb_decexec(void);
extern struct bwb_line *
bwb_DEF(struct bwb_line * l);
extern struct bwb_line *
bwb_DEFDBL(struct bwb_line * l);
extern struct bwb_line *
bwb_DEFINT(struct bwb_line * l);
extern struct bwb_line *
bwb_DEFSNG(struct bwb_line * l);
extern struct bwb_line *
bwb_DEFSTR(struct bwb_line * l);
extern struct bwb_line *
bwb_DEF_SUB(struct bwb_line * l);
extern struct bwb_line *
bwb_DELETE(struct bwb_line * l);
extern struct bwb_line *
bwb_DIM(struct bwb_line * l);
extern struct bwb_line *
bwb_DO(struct bwb_line * l);
extern struct bwb_line *
bwb_DO_UNTIL(struct bwb_line * l);
extern struct bwb_line *
bwb_DO_WHILE(struct bwb_line * l);
extern struct bwb_line *
bwb_ELSE(struct bwb_line * l);
extern struct bwb_line *
bwb_ELSEIF(struct bwb_line * l);
extern struct bwb_line *
bwb_END(struct bwb_line * l);
extern struct bwb_line *
bwb_END_FUNCTION(struct bwb_line * l);
extern struct bwb_line *
bwb_END_IF(struct bwb_line * l);
extern struct bwb_line *
bwb_END_SELECT(struct bwb_line * l);
extern struct bwb_line *
bwb_END_SUB(struct bwb_line * l);
extern struct bwb_line *
bwb_ERASE(struct bwb_line * l);
extern int
bwb_error(char *message);
extern struct bwb_variable *
bwb_etov(struct bwb_variable * dst, struct exp_ese * src);
extern void
bwb_execline(void);
extern struct bwb_line *
bwb_EXIT(struct bwb_line * l);
extern struct bwb_line *
bwb_EXIT_DO(struct bwb_line * l);
extern struct bwb_line *
bwb_EXIT_FOR(struct bwb_line * l);
extern struct bwb_line *
bwb_EXIT_FUNCTION(struct bwb_line * l);
extern struct bwb_line *
bwb_EXIT_SUB(struct bwb_line * l);
extern struct bwb_line *
bwb_EXIT_UNTIL(struct bwb_line * l);
extern struct bwb_line *
bwb_EXIT_WHILE(struct bwb_line * l);
extern struct exp_ese *
bwb_exp(char *expression, int assignment, int *position);
extern struct bwb_line *
bwb_FIELD(struct bwb_line * l);
extern int
bwb_fload(FILE * file);
extern struct bwb_line *
bwb_FNCS(struct bwb_line * l);
extern struct bwb_line *
bwb_FOR(struct bwb_line * l);
extern int
bwb_freeline(struct bwb_line * l);
extern struct bwb_line *
bwb_FUNCTION(struct bwb_line * l);
extern int
bwb_gets(char *buffer);
extern int
bwb_getvarname(char *lb, char *sb, int *n);
extern struct bwb_line *
bwb_GO(struct bwb_line * l);
extern struct bwb_line *
bwb_GOSUB(struct bwb_line * l);
extern struct bwb_line *
bwb_GOTO(struct bwb_line * l);
extern struct bwb_line *
bwb_HELP(struct bwb_line * l);
extern struct bwb_line *
bwb_IF(struct bwb_line * l);
extern struct bwb_line *
bwb_IF_THEN(struct bwb_line * l);
extern void
bwb_incexec(void);
extern void
bwb_init(int argc, char **argv);
extern struct bwb_line *
bwb_INPUT(struct bwb_line * l);
extern int
bwb_interact(void);
extern int
bwb_isvar(char *buffer);
extern int
bwb_ladd(char *buffer, struct bwb_line * p);
extern struct bwb_line *
bwb_LET(struct bwb_line * l);
extern struct bwb_line *
bwb_LINE(struct bwb_line * l);
extern struct bwb_line *
bwb_LIST(struct bwb_line * l);
extern struct bwb_line *
bwb_LOAD(struct bwb_line * l);
extern struct bwb_line *
bwb_LOOP(struct bwb_line * l);
extern struct bwb_line *
bwb_LOOP_UNTIL(struct bwb_line * l);
extern struct bwb_line *
bwb_LOOP_WHILE(struct bwb_line * l);
extern struct bwb_line *
bwb_LPRINT(struct bwb_line * l);
extern struct bwb_line *
bwb_LSET(struct bwb_line * l);
extern void
bwb_mainloop(void);
extern struct bwb_line *
bwb_MAINTAINER(struct bwb_line * l);
extern struct bwb_line *
bwb_MERGE(struct bwb_line * l);
extern struct bwb_line *
bwb_MID_(struct bwb_line * l);
extern struct bwb_line *
bwb_NAME(struct bwb_line * l);
extern struct bwb_line *
bwb_NEW(struct bwb_line * l);
extern struct bwb_line *
bwb_NEXT(struct bwb_line * l);
extern struct bwb_line *
bwb_null(struct bwb_line * l);
extern int
bwb_numseq(char *buffer, BasicLineNumberType * start, BasicLineNumberType * end);
extern struct bwb_line *
bwb_ON(struct bwb_line * l);
extern struct bwb_line *
bwb_ON_ERROR_GOTO(struct bwb_line * l);
extern struct bwb_line *
bwb_ON_ERROR_GOSUB(struct bwb_line * l);
extern struct bwb_line *
bwb_ON_ERROR_RESUME_NEXT(struct bwb_line * l);
extern struct bwb_line *
bwb_ON_ERROR_RETURN_NEXT(struct bwb_line * l);
extern struct bwb_line *
bwb_OPEN(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ANGLE_DEGREES(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ANGLE_RADIANS(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ARITHMETIC_DECIMAL(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ARITHMETIC_FIXED(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ARITHMETIC_NATIVE(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_BASE_0(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_BASE_1(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_BUGS_OFF(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_BUGS_ON(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_COMMENT(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_COMPARE_BINARY(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_COMPARE_DATABASE(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_COMPARE_TEXT(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_COVERAGE_OFF(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_COVERAGE_ON(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_DATE(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_DISABLE_COMMAND(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_DISABLE_FUNCTION(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_DISABLE_OPERATOR(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ENABLE_COMMAND(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ENABLE_FUNCTION(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ENABLE_OPERATOR(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ERROR_GOSUB(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_ERROR_GOTO(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_INDENT(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_LABELS_OFF(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_LABELS_ON(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_STATEMENT(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_STRICT_OFF(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_STRICT_ON(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_TERMINAL_NONE(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_TERMINAL_ADM_3A(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_TERMINAL_ANSI(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_TIME(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_TRACE_OFF(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_TRACE_ON(struct bwb_line * l);
extern struct bwb_line *
bwb_OPTION_VERSION(struct bwb_line * l);
extern struct bwb_line *
bwb_PRINT(struct bwb_line * l);
extern struct bwb_line *
bwb_QUEST(struct bwb_line * l);
extern struct bwb_line *
bwb_QUIT(struct bwb_line * l);
extern struct bwb_line *
bwb_READ(struct bwb_line * l);
extern struct bwb_line *
bwb_REM(struct bwb_line * l);
extern struct bwb_line *
bwb_RESTORE(struct bwb_line * l);
extern struct bwb_line *
bwb_RESUME(struct bwb_line * l);
extern struct bwb_line *
bwb_RETURN(struct bwb_line * l);
extern struct bwb_line *
bwb_RSET(struct bwb_line * l);
extern struct bwb_line *
bwb_RUN(struct bwb_line * l);
extern struct bwb_line *
bwb_SAVE(struct bwb_line * l);
extern int
bwb_scan(void);
extern struct bwb_line *
bwb_SELECT(struct bwb_line * l);
extern struct bwb_line *
bwb_SELECT_CASE(struct bwb_line * l);
extern int
bwb_setexec(struct bwb_line * l, int position, int code);
extern struct bwb_line *
bwb_STOP(struct bwb_line * l);
extern int
bwb_stripcr(char *s);
extern struct bwb_line *
bwb_SUB(struct bwb_line * l);
extern struct bwb_line *
bwb_SWAP(struct bwb_line * l);
extern struct bwb_line *
bwb_SYSTEM(struct bwb_line * l);
extern struct bwb_line *
bwb_UEND(struct bwb_line * l);
extern struct bwb_line *
bwb_UNTIL(struct bwb_line * l);
extern struct bwb_line *
bwb_USER_LBL(struct bwb_line * l);
extern struct bwb_line *
bwb_VARS(struct bwb_line * l);
extern struct bwb_line *
bwb_vector(struct bwb_line * l);
extern struct bwb_variable *
bwb_vtov(struct bwb_variable * dst, struct bwb_variable * src);
extern void
bwb_Warning(int ErrorLevel, char *ErrorMessage);
extern void
bwb_Warning_Clear(void);
extern void
bwb_Warning_InvalidParameter(char *Message);
extern void
bwb_Warning_OutOfData(char *Message);
extern void
bwb_Warning_Overflow(char *Message);
extern void
bwb_Warning_AdvancedFeature(char *Message);
extern int
bwb_Warning_Pending(void);
extern struct bwb_line *
bwb_WEND(struct bwb_line * l);
extern struct bwb_line *
bwb_WHILE(struct bwb_line * l);
extern struct bwb_line *
bwb_WRITE(struct bwb_line * l);
extern struct bwb_line *
bwb_xlist(struct bwb_line * l, FILE * file);
extern struct bwb_line *
bwb_xload(struct bwb_line * l);
extern void
bwb_xnew(struct bwb_line * l);
extern struct bwb_line *
bwb_xtxtline(char *buffer);
extern struct bwb_line *
bwb_zline(struct bwb_line * l);
extern void    *
CALLOC(size_t nelem, size_t elsize, char *str);
extern int
dec_esc(void);
extern int
DetermineNumberOfDimensions(char *expression, int LastPosition);
extern int
dim_getparams(char *buffer, int *pos, int *n_params, int **pp);
extern int
dim_getparams_FIXED(char *buffer, int *pos, int params[]);
extern void
DumpAllCommandSwitchStatement(void);
extern void
DumpAllCommandSyntax(void);
extern void
DumpAllCommandTableDefinitions(void);
extern void
DumpAllCommandUniqueID(void);
extern void
DumpAllFuctionSyntax(void);
extern void
DumpAllFuctionTableDefinitions(void);
extern void
DumpAllFuctionUniqueID(void);
extern void
DumpAllFunctionSwitch(void);
extern void
DumpOneCommandSyntax(int n, int lp);
extern void
DumpOneFunctionSyntax(int n, int lp);
extern int
exp_findop(char *expression);
extern int
exp_function(char *expression);
extern BasicNumberType
exp_getnval(struct exp_ese * e);
extern int
exp_getival(struct exp_ese * e);
extern bstring *
exp_getsval(struct exp_ese * e);
extern int
exp_getvfname(char *source, char *destination);
extern int
exp_iscmd(char *expression);
extern int
exp_isfn(char *expression);
extern int
exp_isnc(char *expression);
extern int
exp_isop(char *expression);
extern int
exp_isvn(char *expression);
extern int
exp_numconst(char *expression);
extern int
exp_operation(int entry_level);
extern int
exp_paren(char *expression);
extern int
exp_strconst(char *expression);
extern int
exp_validarg(char *expression);
extern int
exp_variable(char *expression);
extern struct bwb_line *
find_BottomLineInCode(struct bwb_line * l);
extern struct bwb_line *
find_label(char *buffer);
extern void
fnc_add_deffn(struct fslte * F);
extern struct bwb_variable *
fnc_deffn(int argc, struct bwb_variable * argv, int unique_id);
extern struct bwb_function *
fnc_find(char *name);
extern struct bwb_function *
fnc_find_by_id(int unique_id);
extern struct bwb_function *
fnc_find_exact(char *name, int ParameterCount, int ParameterTypes);
extern int
fnc_init(int task);
extern struct bwb_variable *
fnc_intrinsic(int argc, struct bwb_variable * argv, int unique_id);
extern struct bwb_variable *
fnc_null(int argc, struct bwb_variable * argv, int unique_id);
extern void
FREE(void *ptr, char *str);
extern int
fslt_add(struct bwb_line * line);
extern int
fslt_addcallvar(struct bwb_variable * v);
extern int
fslt_addlocalvar(struct fslte * f, struct bwb_variable * v);
extern struct fslte *
fslt_find_name(char *name);
extern int
fslt_init(int task);
extern void
FunctionDefinitionCheck(struct bwb_function * f);
extern void
FunctionSyntax(struct bwb_function * f, char *Syntax);
extern void
FunctionUniqueID(struct bwb_function * f, char *UniqueID);
extern char    *
GetKeyword(struct bwb_line * l, char *OutputBuffer, char *Keyword);
extern char    *
GetRestOfLine(struct bwb_line * l, char *OutputBuffer);
extern int
ImplicitDim(char *tbuf, int n_params);
extern int
inc_esc(void);
extern int
inp_adv(char *b, int *c);
extern BasicNumberType
inp_numconst(char *expression, int *ResultCode);
extern int
int_qmdstr(char *buffer_a, char *buffer_b);
extern int
IsLastKeyword(struct bwb_line * l, char *Keyword);
extern int
is_cmd(char *buffer);
extern int
is_eol(char *buffer, int *position);
extern int
is_label(char *buffer);
extern int
is_let(char *buffer);
extern int
is_ln(char *buffer);
extern int
is_numconst(char *buffer);
extern int
line_start(struct bwb_line * l);
extern int
ln_asbuf(struct bwb_line * l, char *s);
extern int
NumberValueCheck(unsigned long ParameterTests, BasicNumberType X);
extern void
OptionBaseHelper(int newval, struct bwb_line * l);
extern void
OptionVersionSet(int i);
extern int
prn_iprintf(char *buffer);
extern int
prn_lprintf(char *buffer);
extern int
prn_precision(struct bwb_variable * v);
extern int
prn_xprintf(char *buffer);
extern int
ResetConsoleColumn(void);
extern int
scan_element(char *buffer, int *pos, char *element);
extern void
SortAllCommands(void);
extern void
SortAllFunctions(void);
extern int
StringLengthCheck(unsigned long ParameterTests, int s);
extern int
str_btob(bstring * d, bstring * s);
extern int
str_btoc(char *buffer, bstring * s);
extern bstring *
str_cat(bstring * a, bstring * b);
extern int
str_cmp(bstring * a, bstring * b);
extern int
str_ctob(bstring * s, char *buffer);
extern struct bwb_variable *
var_chain(struct bwb_variable * argv);
extern int
var_delcvars(void);
extern struct bwb_variable *
var_find(char *buffer);
extern BasicNumberType *
var_findnval(struct bwb_variable * v, int *pp);
extern bstring *
var_findsval(struct bwb_variable * v, int *pp);
extern struct bwb_variable *
var_free(struct bwb_variable * v);
extern          BasicNumberType
var_getnval(struct bwb_variable * nvar);
extern bstring *
var_getsval(struct bwb_variable * nvar);
extern int
var_init(int task);
extern int
var_make(struct bwb_variable * v, int type);
extern struct bwb_variable *
var_new(char *name);
extern struct bwb_variable *
var_pos(struct bwb_variable * firstvar, int p);
extern int
var_setnval(struct bwb_variable * v, BasicNumberType i);
extern          BasicNumberType
VerifyNumeric(BasicNumberType Value);
extern void
SetOnError(int LineNumber);
extern int
GetOnError(void);
extern int
ToUpper(int c);
extern int
ToLower(int c);
extern void
var_CLEAR(void);
extern void
break_handler(void);
extern struct bwb_line *
bwb_EDIT(struct bwb_line * l);
extern struct bwb_line *
bwb_RENUM(struct bwb_line * l);
extern int
bwx_CHDIR(char *A);
extern int
bwx_CLS(void);
extern int
bwx_COLOR(int X, int Y);
extern int
bwx_CONSOLE_WIDTH(void);
extern int
bwx_DEBUG(const char *A);
extern int
bwx_errmes(char *m);
extern int
bwx_FILES(char *A);
extern int
bwx_FRE(void);
extern int
bwx_INKEY(char *S);
extern int
bwx_INP(int X);
extern int
bwx_input(char *prompt, char *buffer);
extern int
bwx_INPUT(int x, char *S);
extern int
bwx_KILL(char *A);
extern int
bwx_LOCATE(int X, int Y);
extern int
bwx_LPRINT(char a);
extern int
bwx_LPRINT_WIDTH(void);
extern int
bwx_message(char *m);
extern int
bwx_MKDIR(char *A);
extern int
bwx_NAME(char *A, char *B);
extern int
bwx_OUT(int X, int Y);
extern int
bwx_OUTPUT_WIDTH(void);
extern int
bwx_PEEK(int X);
extern int
bwx_POKE(int X, int Y);
extern int
bwx_PRINT(char c);
extern int
bwx_RANDOM_RECORD_SIZE(void);
extern int
bwx_RMDIR(char *A);
extern int
bwx_SHELL(char *A);
extern void
bwx_STOP(void);
extern void
bwx_terminate(void);
extern BasicNumberType
bwx_TIMER(BasicNumberType Seconds);
extern int
bwx_WAIT(int X, int Y, int Z);
extern int
bwx_signon(void);



#if HAVE_ISNAN

/* these are intrinsic C functions in my environment using -ansi */

#else          /* ! HAVE_ISNAN */

extern int
isnan(double X);
extern int
isinf(double X);
extern double
rint(double X);
extern int
strcasecmp(const char *s1, const char *s2);
extern int
strncasecmp(const char *s1, const char *s2, size_t n);

#endif            /* ! HAVE_ISNAN */

#if BROKEN_STRCPY
#undef  strcpy
#define strcpy broken_strcpy
extern void 
broken_strcpy( char * Target, char * Source );
#endif

/* EOF */
