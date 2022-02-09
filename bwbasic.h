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
/* Version 3.10 by Howard Wulf, AF5NE                            */
/*                                                               */
/*---------------------------------------------------------------*/



#ifndef TRUE
#define TRUE    -1
#endif
#ifndef FALSE
#define FALSE   0
#endif


#define NEW_VIRTUAL        TRUE

#define THE_PRICE_IS_RIGHT TRUE


#if FALSE
#include         <assert.h>
#include         <ctype.h>
#endif
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
#if FALSE
#include         <string.h>
#endif
#include         <time.h>

#ifndef isnan
#define isnan(x) (x != x)
#endif
#ifndef isinf
#define isinf(x) (!isnan(x) && isnan(x - x))
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

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
/* UNIX-style OS. ------------------------------------------- */

#define HAVE_UNIX TRUE

#elif defined(_DOS) || defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
/* DOS-style OS. ------------------------------------------- */

#define HAVE_DOS  TRUE

#endif

#if HAVE_UNIX
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
gcc -ansi -o bwbasic bw*.c

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
#if __GNUC__
#define HAVE_UNIX_GCC      TRUE
#else
#define HAVE_UNIX_GCC      FALSE
#endif
#endif /* HAVE_UNIX */

#if HAVE_DOS

#if        __WATCOM__
#include <direct.h>
#else  /* !__WATCOM__ */
#include <dir.h>
#endif /* !__WATCOM__ */
#define DIRECTORY_CMDS     TRUE

#define MKDIR_ONE_ARG      TRUE
#define PERMISSIONS        0        /* not used */
#define DEF_EDITOR         "edit"   /* for windows use "notepad" */
#define DEF_FILES          "dir /w" /* default value for FILES */
#define HAVE_UNIX_GCC      FALSE
#define EXECLEVELS         15       /* maximum EXEC stack levels */

#endif /* HAVE_DOS */

#if __CMS__
/* GCC 3 for CMS on Hercules */
#include "bwd_six.h"
#define PROFILENAME        "PROFILE BAS"
#define DEBUGFILENAME      "DEBUG OUT"
#if FALSE
/* bw3.1: printer goes to STDERR */
#define LPRINTFILENAME     "LPRINT OUT"
#endif
#if FALSE
/* bw3.1: use the operating system to redirect STDERR */
#define ERRFILE            "ERROR OUT"
#endif
#define EXPONENT_DIGITS    4  /* E-9999 to E+9999 */
#define PROMPT             "bwBASIC:\n"
#define DIRECTORY_CMDS     FALSE
#define HAVE_UNIX_GCC      FALSE
#endif /* __CMS__ */

#if __MVS__
/* GCC 3 for MVS on Hercules */
#include "bwd_six.h"
#define PROFILENAME        "dd:profile"
#define DEBUGFILENAME      "dd:debug"
#if FALSE
/* bw3.1: printer goes to STDERR */
#define LPRINTFILENAME     "dd:lprint"
#endif
#if FALSE
/* bw3.1: use the operating system to redirect STDERR */
#define ERRFILE            "dd:errout"
#endif
#define EXPONENT_DIGITS    4  /* E-9999 to E+9999 */
#define PROMPT             "bwBASIC:\n"
#define DIRECTORY_CMDS     FALSE
#define HAVE_UNIX_GCC      FALSE
#endif /* __MVS__ */

#if __GNUC__ || __WATCOM__
/* __FUNCTION__ is the current function name */
#else
#define __FUNCTION__ "n/a"
#endif

#if __WATCOM__
extern int putenv( const char * buffer );
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
#ifndef HAVE_UNIX_GCC
#define HAVE_UNIX_GCC  FALSE  /* isnan() exists when compiling with -ansi */
#endif
#ifndef PROFILE
#define PROFILE         TRUE  /* interpret PROFILENAME at beginning */
#endif 
#ifndef PROFILENAME
/* only used when PROFILE == TRUE */
#define PROFILENAME    "profile.bas"   /* Filename for profile execution */
#endif 
#if FALSE
/* bw3.1: use the operating system to redirect STDERR */
#ifndef REDIRECT_STDERR
#define REDIRECT_STDERR FALSE /* Redirect stderr to file ERRFILE */
#endif 
#ifndef ERRFILE
/* only used when REDIRECT_STDERR == TRUE */
#define ERRFILE         "err.out"   /* Filename for redirected stderr */
#endif 
#endif
#ifndef DEBUGFILENAME
/* MAINTAINER */
#define DEBUGFILENAME  "DEBUG.OUT"     /* Filename for DEBUG output */
#endif 
#if FALSE
/* bw3.1: printer goes to STDERR, which can be redirected using OPTION STDERR "LPRINT.OUT" */
#ifndef LPRINTFILENAME
#define LPRINTFILENAME  "LPRINT.OUT"   /* Filename for LPRINT output */
#endif 
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
#define DEFVNAME_EDITOR "BWB.EDITOR$"  /* variable name for EDIT command */
#endif
#ifndef DEF_EDITOR
#define DEF_EDITOR      "vi"  /* default value for EDITOR */
#endif
#ifndef DEFVNAME_FILES
#define DEFVNAME_FILES  "BWB.FILES$"   /* variable name for FILES command */
#endif
#ifndef DEF_FILES
#define DEF_FILES       "ls -Fx"/* default value for BWB.FILES$ */
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
#ifndef DEFVNAME_RENUM
#define DEFVNAME_RENUM "BWB.RENUM$"  /* variable name for RENUM command */
#endif
#ifndef DEF_RENUM
#define DEF_RENUM      "renum"  /* default value for BWB.RENUM$ */
#endif



#ifndef EXECLEVELS
#define EXECLEVELS     255 /* maximum EXEC stack levels */
#endif
#ifndef MAX_DIMS
#define MAX_DIMS       3 /* maximum # of dimensions */
#endif

#if FALSE
/* bw3.1: not used */
#ifndef ESTACKSIZE
#define ESTACKSIZE      64 /* elements in expression stack */
#endif
#endif

/*
**
**
** UNLESS YOU ARE WORKING ON THE INTERNALS, YOU PROBABLY SHOULD NOT GO BEYOND HERE 
**
**
*/



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
#ifndef ZONE_WIDTH
/* ECMA-55, Section 14.4 */
#define ZONE_WIDTH          ( SIGNIFICANT_DIGITS + EXPONENT_DIGITS + 6 )
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



/* BASIC BYTE [MINBYT,MAXBYT] */
#ifndef BasicByteSuffix
#define BasicByteSuffix  '~' /* tilde */
#endif
typedef unsigned char BasicByteType;



/* BASIC DEVICE [MINDEV,MAXDEV]*/
#ifndef BasicFileNumberPrefix
#define BasicFileNumberPrefix  '#'
#endif
#ifndef BasicFileNumberMax
#define BasicFileNumberMax     ( FOPEN_MAX - 3 /* stdin, stdout, stderr */ )   
/* max file # (1..7), file # 0 is the console */
#endif


/* BASIC LENGTH [MINLEN,MAXLEN] */
#ifndef BasicStringSuffix
#define BasicStringSuffix     '$'
#endif
#ifndef BasicStringLengthMax
#define BasicStringLengthMax   4096 /* max length of BASIC string */
#endif
typedef unsigned int BasicStringLengthType;

/* the character that surrounds string constants, commonly a single or double quote */
#ifndef BasicQuoteChar
#define BasicQuoteChar '"' /* double-quote */
#endif


/* the character that represents a left parenthesis, commonly '(', sometimes '[' */
#ifndef BasicLeftParenChar
#define BasicLeftParenChar '('
#endif

/* the character that represents a right parenthesis, commonly ')', sometimes ']' */
#ifndef BasicRightParenChar
#define BasicRightParenChar ')'
#endif

/* the character that terminates a C string, commonly CHR$(0) */
#ifndef BasicNulChar
#define BasicNulChar '\0' /* NUL */
#endif


/* BASIC keyword length */
#ifndef BasicNameLengthMax
#define BasicNameLengthMax 40 /* variable,function,subroutine,command,label */
#endif



/* BASIC linenumber  */
typedef int     BasicLineNumberType;
#define BasicLineNumberMax      (INT_MAX-1) /* maximum USER line number */
#define BasicLineNumberMin      (      0+1) /* minimum USER line number */
#define BasicLineNumberDigits   (        5) /* 1 - 99999 */
 


/* UniqueID */
typedef unsigned short BasicUniqueType; /* 0..65535 */


#define MIN( X, Y ) ( (X) < (Y) ? (X) : (Y) )
#define MAX( X, Y ) ( (X) > (Y) ? (X) : (Y) )

#define PRN_SPC      0x01  /* send SPC followed by SPC number */
#define PRN_TAB      0x02  /* send TAB followed by TAB number */


/* Device input/output modes */
#define DEVMODE_CLOSED     0
#define DEVMODE_INPUT      1
#define DEVMODE_OUTPUT     2
#define DEVMODE_RANDOM     4
#define DEVMODE_APPEND     8
#define DEVMODE_BINARY     32
#define DEVMODE_READ    (DEVMODE_INPUT  | DEVMODE_RANDOM                  | DEVMODE_BINARY )
#define DEVMODE_WRITE   (DEVMODE_OUTPUT | DEVMODE_RANDOM | DEVMODE_APPEND | DEVMODE_BINARY )



/* codes for EXEC stack and for function-sub-label lookup table */
#define EXEC_NORM        0
#define EXEC_GOSUB       1
#define EXEC_FOR         3
#define EXEC_FUNCTION    4
#define EXEC_POPPED     15



/* OPTION VERSION bitmask, upto 32 BASIC dialects */
#define D64 (1UL<< 0)     /* Dartmouth  DTSS BASIC    , 1964 */
#define G65 (1UL<< 1)     /* General Electric, Mark I , 1965 */
#define G67 (1UL<< 2)     /* General Electric, Mark II, 1967 */
#define I70 (1UL<< 3)     /* IBM System/360           , 1970 */
#define D71 (1UL<< 4)     /* DEC BASIC-PLUS for PDP-11, 1971 */
#define I73 (1UL<< 5)     /* IBM System/370           , 1973 */
#define C77 (1UL<< 6)     /* CBASIC-II                , 1977 */
#define E78 (1UL<< 7)     /* ECMA-55                  , 1978 */
#define HB1 (1UL<< 8)     /* BASIC Handbook, 1st Ed   . 1978 */
#define HB2 (1UL<< 9)     /* BASIC Handbook, 2nd Ed   , 1981 */
#define M80 (1UL<<10)     /* Microsoft BASIC-80       , 1980 */
#define T80 (1UL<<11)     /* TRS-80 Model I/III/4     , 1981 */
#define M85 (1UL<<12)     /* Microsoft GW-BASIC       , 1985 */
#define E86 (1UL<<13)     /* ECMA-116                 , 1986 */
#define M88 (1UL<<14)     /* Microsoft QBASIC         , 1988 */
#define VBX (1UL<<15)     /* Microsoft VBX            , 1990 */
#define B15 (1UL<<16)     /* Bywater BASIC            , 2015 */
#define NUM_VERSIONS 17 /* upto 32 BASIC dialects          */

/* OptionFlags */
#define OPTION_STRICT_ON      0x0001   /* Do NOT allow implicit DIM */
#define OPTION_ANGLE_DEGREES  0x0002   /* Use degrees instead of radians */
#define OPTION_BUGS_ON        0x0004   /* ON ... , FOR ... */
#define OPTION_LABELS_ON      0x0008   /* Labels allowed */
#define OPTION_COMPARE_TEXT   0x0010   /* Case-Insensitive string comparison */
#define OPTION_RESERVED_0020  0x0020   /* reserved for future use */
#define OPTION_COVERAGE_ON    0x0040   /* track BASIC lines actually executed */
#define OPTION_TRACE_ON       0x0080   /* dump BASIC stack trace when FATAL error */
#define OPTION_ERROR_GOSUB    0X0100   /* error causes GOSUB instead of GOTO */
#define OPTION_EXPLICIT_ON    0x0200   /* all variables must be declared via DIM */
#define OPTION_ANGLE_GRADIANS 0x0400   /* Use gradians instead of radians */
#define OPTION_RESERVED_0800  0x0800   /* reserved for future use */
#define OPTION_RESERVED_1000  0x1000   /* reserved for future use */
#define OPTION_RESERVED_2000  0x2000   /* reserved for future use */
#define OPTION_RESERVED_4000  0x4000   /* reserved for future use */
#define OPTION_RESERVED_8000  0x8000   /* reserved for future use */



/* LineFlags */
#define LINE_EXECUTED        0x01   /* line was executed */
#define LINE_NUMBERED        0x02   /* line was manually numbered */
#define LINE_BREAK           0x04   /* BREAK line */
#define LINE_RESERVED_08     0x08   /* reserved for future use */
#define LINE_RESERVED_10     0x10   /* reserved for future use */
#define LINE_RESERVED_20     0x20   /* reserved for future use */
#define LINE_RESERVED_40     0x40   /* reserved for future use */
#define LINE_RESERVED_80     0x80   /* reserved for future use */



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
#define P1ANY 0X00000001   /* X is any valid number  , A$ is any valid string      */
#define P1BYT 0x00000002   /* MIN_BYT <= X <= MAX_BYT, LEN(A$) >= sizeof(char)     */
#define P1INT 0x00000003   /* INT_MIN <= X <= INT_MAX, LEN(A$) >= sizeof(short)    */
#define P1LNG 0x00000004   /* LONG_MIN <= X <= LONG_MAX, LEN(A$) >= sizeof(long)   */
#define P1CUR 0x00000005   /* LONG_MIN <= X <= LONG_MAX, LEN(A$) >= sizeof(long)   */
#define P1FLT 0x00000006   /* MIN_FLT <= X <= MAX_FLT, LEN(A$) >= sizeof(float)    */
#define P1DBL 0x00000007   /* MIN_DBL <= X <= MAX_DBL, LEN(A$) >= sizeof(double)   */
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
#define C_APPEND                         2 /* APPEND                         */
#define C_AS                             3 /* AS                             */
#define C_AUTO                           4 /* AUTO                           */
#define C_BACKSPACE                      5 /* BACKSPACE                      */
#define C_BREAK                          6 /* BREAK                          */
#define C_BYE                            7 /* BYE                            */
#define C_CALL                           8 /* CALL                           */
#define C_CASE                           9 /* CASE                           */
#define C_CASE_ELSE                     10 /* CASE ELSE                      */
#define C_CHAIN                         11 /* CHAIN                          */
#define C_CHANGE                        12 /* CHANGE                         */
#define C_CLEAR                         13 /* CLEAR                          */
#define C_CLOAD                         14 /* CLOAD                          */
#define C_CLOAD8                        15 /* CLOAD*                         */
#define C_CLOSE                         16 /* CLOSE                          */
#define C_CLR                           17 /* CLR                            */
#define C_CMDS                          18 /* CMDS                           */
#define C_COMMON                        19 /* COMMON                         */
#define C_CONSOLE                       20 /* CONSOLE                        */
#define C_CONT                          21 /* CONT                           */
#define C_CREATE                        22 /* CREATE                         */
#define C_CSAVE                         23 /* CSAVE                          */
#define C_CSAVE8                        24 /* CSAVE*                         */
#define C_DATA                          25 /* DATA                           */
#define C_DEF                           26 /* DEF                            */
#define C_DEF_SUB                       27 /* DEF SUB                        */
#define C_DEFBYT                        28 /* DEFBYT                         */
#define C_DEFCUR                        29 /* DEFCUR                         */
#define C_DEFDBL                        30 /* DEFDBL                         */
#define C_DEFINT                        31 /* DEFINT                         */
#define C_DEFLNG                        32 /* DEFLNG                         */
#define C_DEFSNG                        33 /* DEFSNG                         */
#define C_DEFSTR                        34 /* DEFSTR                         */
#define C_DELETE                        35 /* DELETE                         */
#define C_DELIMIT                       36 /* DELIMIT                        */
#define C_DIM                           37 /* DIM                            */
#define C_DO                            38 /* DO                             */
#define C_DSP                           39 /* DSP                            */
#define C_EDIT                          40 /* EDIT                           */
#define C_ELSE                          41 /* ELSE                           */
#define C_ELSEIF                        42 /* ELSEIF                         */
#define C_END                           43 /* END                            */
#define C_END_FUNCTION                  44 /* END FUNCTION                   */
#define C_END_IF                        45 /* END IF                         */
#define C_END_SELECT                    46 /* END SELECT                     */
#define C_END_SUB                       47 /* END SUB                        */
#define C_ERASE                         48 /* ERASE                          */
#define C_EXCHANGE                      49 /* EXCHANGE                       */
#define C_EXIT                          50 /* EXIT                           */
#define C_EXIT_DO                       51 /* EXIT DO                        */
#define C_EXIT_FOR                      52 /* EXIT FOR                       */
#define C_EXIT_FUNCTION                 53 /* EXIT FUNCTION                  */
#define C_EXIT_SUB                      54 /* EXIT SUB                       */
#define C_EXIT_UNTIL                    55 /* EXIT UNTIL                     */
#define C_EXIT_WHILE                    56 /* EXIT WHILE                     */
#define C_FEND                          57 /* FEND                           */
#define C_FIELD                         58 /* FIELD                          */
#define C_FILE                          59 /* FILE                           */
#define C_FILES                         60 /* FILES                          */
#define C_FNCS                          61 /* FNCS                           */
#define C_FNEND                         62 /* FNEND                          */
#define C_FOR                           63 /* FOR                            */
#define C_FUNCTION                      64 /* FUNCTION                       */
#define C_GET                           65 /* GET                            */
#define C_GO                            66 /* GO                             */
#define C_GO_SUB                        67 /* GO SUB                         */
#define C_GO_TO                         68 /* GO TO                          */
#define C_GOODBYE                       69 /* GOODBYE                        */
#define C_GOSUB                         70 /* GOSUB                          */
#define C_GOTO                          71 /* GOTO                           */
#define C_HELP                          72 /* HELP                           */
#define C_IF                            73 /* IF                             */
#define C_IF_END                        74 /* IF END                         */
#define C_IF_MORE                       75 /* IF MORE                        */
#define C_IF_THEN                       76 /* IF THEN                        */
#define C_IMAGE                         77 /* IMAGE                          */
#define C_INPUT                         78 /* INPUT                          */
#define C_LET                           79 /* LET                            */
#define C_LINE                          80 /* LINE                           */
#define C_LIST                          81 /* LIST                           */
#define C_LLIST                         82 /* LLIST                          */
#define C_LOAD                          83 /* LOAD                           */
#define C_LOOP                          84 /* LOOP                           */
#define C_LPRINT                        85 /* LPRINT                         */
#define C_LPRINTER                      86 /* LPRINTER                       */
#define C_LSET                          87 /* LSET                           */
#define C_MAINTAINER                    88 /* MAINTAINER                     */
#define C_MARGIN                        89 /* MARGIN                         */
#define C_MAT                           90 /* MAT                            */
#define C_MAT_GET                       91 /* MAT GET                        */
#define C_MAT_INPUT                     92 /* MAT INPUT                      */
#define C_MAT_PRINT                     93 /* MAT PRINT                      */
#define C_MAT_PUT                       94 /* MAT PUT                        */
#define C_MAT_READ                      95 /* MAT READ                       */
#define C_MAT_WRITE                     96 /* MAT WRITE                      */
#define C_MERGE                         97 /* MERGE                          */
#define C_MID4                          98 /* MID$                           */
#define C_NAME                          99 /* NAME                           */
#define C_NEW                          100 /* NEW                            */
#define C_NEXT                         101 /* NEXT                           */
#define C_OF                           102 /* OF                             */
#define C_OLD                          103 /* OLD                            */
#define C_ON                           104 /* ON                             */
#define C_ON_ERROR                     105 /* ON ERROR                       */
#define C_ON_ERROR_GOSUB               106 /* ON ERROR GOSUB                 */
#define C_ON_ERROR_GOTO                107 /* ON ERROR GOTO                  */
#define C_ON_ERROR_RESUME              108 /* ON ERROR RESUME                */
#define C_ON_ERROR_RESUME_NEXT         109 /* ON ERROR RESUME NEXT           */
#define C_ON_ERROR_RETURN              110 /* ON ERROR RETURN                */
#define C_ON_ERROR_RETURN_NEXT         111 /* ON ERROR RETURN NEXT           */
#define C_ON_TIMER                     112 /* ON TIMER                       */
#define C_OPEN                         113 /* OPEN                           */
#define C_OPTION                       114 /* OPTION                         */
#define C_OPTION_ANGLE                 115 /* OPTION ANGLE                   */
#define C_OPTION_ANGLE_DEGREES         116 /* OPTION ANGLE DEGREES           */
#define C_OPTION_ANGLE_GRADIANS        117 /* OPTION ANGLE GRADIANS          */
#define C_OPTION_ANGLE_RADIANS         118 /* OPTION ANGLE RADIANS           */
#define C_OPTION_ARITHMETIC            119 /* OPTION ARITHMETIC              */
#define C_OPTION_ARITHMETIC_DECIMAL    120 /* OPTION ARITHMETIC DECIMAL      */
#define C_OPTION_ARITHMETIC_FIXED      121 /* OPTION ARITHMETIC FIXED        */
#define C_OPTION_ARITHMETIC_NATIVE     122 /* OPTION ARITHMETIC NATIVE       */
#define C_OPTION_BASE                  123 /* OPTION BASE                    */
#define C_OPTION_BUGS                  124 /* OPTION BUGS                    */
#define C_OPTION_BUGS_OFF              125 /* OPTION BUGS OFF                */
#define C_OPTION_BUGS_ON               126 /* OPTION BUGS ON                 */
#define C_OPTION_COMMENT               127 /* OPTION COMMENT                 */
#define C_OPTION_COMPARE               128 /* OPTION COMPARE                 */
#define C_OPTION_COMPARE_BINARY        129 /* OPTION COMPARE BINARY          */
#define C_OPTION_COMPARE_DATABASE      130 /* OPTION COMPARE DATABASE        */
#define C_OPTION_COMPARE_TEXT          131 /* OPTION COMPARE TEXT            */
#define C_OPTION_COVERAGE              132 /* OPTION COVERAGE                */
#define C_OPTION_COVERAGE_OFF          133 /* OPTION COVERAGE OFF            */
#define C_OPTION_COVERAGE_ON           134 /* OPTION COVERAGE ON             */
#define C_OPTION_DATE                  135 /* OPTION DATE                    */
#define C_OPTION_DISABLE               136 /* OPTION DISABLE                 */
#define C_OPTION_DISABLE_COMMAND       137 /* OPTION DISABLE COMMAND         */
#define C_OPTION_DISABLE_FUNCTION      138 /* OPTION DISABLE FUNCTION        */
#define C_OPTION_DISABLE_OPERATOR      139 /* OPTION DISABLE OPERATOR        */
#define C_OPTION_ENABLE                140 /* OPTION ENABLE                  */
#define C_OPTION_ENABLE_COMMAND        141 /* OPTION ENABLE COMMAND          */
#define C_OPTION_ENABLE_FUNCTION       142 /* OPTION ENABLE FUNCTION         */
#define C_OPTION_ENABLE_OPERATOR       143 /* OPTION ENABLE OPERATOR         */
#define C_OPTION_ERROR                 144 /* OPTION ERROR                   */
#define C_OPTION_ERROR_GOSUB           145 /* OPTION ERROR GOSUB             */
#define C_OPTION_ERROR_GOTO            146 /* OPTION ERROR GOTO              */
#define C_OPTION_EXPLICIT              147 /* OPTION EXPLICIT                */
#define C_OPTION_IMAGE                 148 /* OPTION IMAGE                   */
#define C_OPTION_IMPLICIT              149 /* OPTION IMPLICIT                */
#define C_OPTION_INDENT                150 /* OPTION INDENT                  */
#define C_OPTION_LABELS                151 /* OPTION LABELS                  */
#define C_OPTION_LABELS_OFF            152 /* OPTION LABELS OFF              */
#define C_OPTION_LABELS_ON             153 /* OPTION LABELS ON               */
#define C_OPTION_PRINT                 154 /* OPTION PRINT                   */
#define C_OPTION_ROUND                 155 /* OPTION ROUND                   */
#define C_OPTION_ROUND_BANK            156 /* OPTION ROUND BANK              */
#define C_OPTION_ROUND_MATH            157 /* OPTION ROUND MATH              */
#define C_OPTION_ROUND_TRUNCATE        158 /* OPTION ROUND TRUNCATE          */
#define C_OPTION_SLEEP                 159 /* OPTION SLEEP                   */
#define C_OPTION_STATEMENT             160 /* OPTION STATEMENT               */
#define C_OPTION_STDERR                161 /* OPTION STDERR                  */
#define C_OPTION_STDIN                 162 /* OPTION STDIN                   */
#define C_OPTION_STDOUT                163 /* OPTION STDOUT                  */
#define C_OPTION_STRICT                164 /* OPTION STRICT                  */
#define C_OPTION_STRICT_OFF            165 /* OPTION STRICT OFF              */
#define C_OPTION_STRICT_ON             166 /* OPTION STRICT ON               */
#define C_OPTION_TERMINAL              167 /* OPTION TERMINAL                */
#define C_OPTION_TERMINAL_ADM          168 /* OPTION TERMINAL ADM            */
#define C_OPTION_TERMINAL_ANSI         169 /* OPTION TERMINAL ANSI           */
#define C_OPTION_TERMINAL_NONE         170 /* OPTION TERMINAL NONE           */
#define C_OPTION_TIME                  171 /* OPTION TIME                    */
#define C_OPTION_TRACE                 172 /* OPTION TRACE                   */
#define C_OPTION_TRACE_OFF             173 /* OPTION TRACE OFF               */
#define C_OPTION_TRACE_ON              174 /* OPTION TRACE ON                */
#define C_OPTION_USING                 175 /* OPTION USING                   */
#define C_OPTION_VERSION               176 /* OPTION VERSION                 */
#define C_PAUSE                        177 /* PAUSE                          */
#define C_POP                          178 /* POP                            */
#define C_PRINT                        179 /* PRINT                          */
#define C_PUT                          180 /* PUT                            */
#define C_QUIT                         181 /* QUIT                           */
#define C_READ                         182 /* READ                           */
#define C_RECALL                       183 /* RECALL                         */
#define C_REM                          184 /* REM                            */
#define C_RENAME                       185 /* RENAME                         */
#define C_RENUM                        186 /* RENUM                          */
#define C_RENUMBER                     187 /* RENUMBER                       */
#define C_RESET                        188 /* RESET                          */
#define C_RESTORE                      189 /* RESTORE                        */
#define C_RESUME                       190 /* RESUME                         */
#define C_RETURN                       191 /* RETURN                         */
#define C_RSET                         192 /* RSET                           */
#define C_RUN                          193 /* RUN                            */
#define C_SAVE                         194 /* SAVE                           */
#define C_SCRATCH                      195 /* SCRATCH                        */
#define C_SELECT                       196 /* SELECT                         */
#define C_SELECT_CASE                  197 /* SELECT CASE                    */
#define C_STEP                         198 /* STEP                           */
#define C_STOP                         199 /* STOP                           */
#define C_STORE                        200 /* STORE                          */
#define C_SUB                          201 /* SUB                            */
#define C_SUBEND                       202 /* SUBEND                         */
#define C_SWAP                         203 /* SWAP                           */
#define C_SYSTEM                       204 /* SYSTEM                         */
#define C_TEXT                         205 /* TEXT                           */
#define C_THEN                         206 /* THEN                           */
#define C_TIMER                        207 /* TIMER                          */
#define C_TIMER_OFF                    208 /* TIMER OFF                      */
#define C_TIMER_ON                     209 /* TIMER ON                       */
#define C_TIMER_STOP                   210 /* TIMER STOP                     */
#define C_TLOAD                        211 /* TLOAD                          */
#define C_TO                           212 /* TO                             */
#define C_TRACE                        213 /* TRACE                          */
#define C_TRACE_OFF                    214 /* TRACE OFF                      */
#define C_TRACE_ON                     215 /* TRACE ON                       */
#define C_TSAVE                        216 /* TSAVE                          */
#define C_UEND                         217 /* UEND                           */
#define C_UNTIL                        218 /* UNTIL                          */
#define C_USE                          219 /* USE                            */
#define C_USER_LBL                     220 /* USER LBL                       */
#define C_VARS                         221 /* VARS                           */
#define C_WEND                         222 /* WEND                           */
#define C_WHILE                        223 /* WHILE                          */
#define C_WRITE                        224 /* WRITE                          */
#define NUM_COMMANDS 224






/* FUNCTIONS */
#define F_ABS_X_N                        1 /* N  = ABS( X )                  */
#define F_ACOS_X_N                       2 /* N  = ACOS( X )                 */
#define F_ACS_X_N                        3 /* N  = ACS( X )                  */
#define F_ACSD_X_N                       4 /* N  = ACSD( X )                 */
#define F_ACSG_X_N                       5 /* N  = ACSG( X )                 */
#define F_ANGLE_X_Y_N                    6 /* N  = ANGLE( X, Y )             */
#define F_ARCSIN_X_N                     7 /* N  = ARCSIN( X )               */
#define F_ARCTAN_X_N                     8 /* N  = ARCTAN( X )               */
#define F_ARGC_N                         9 /* N  = ARGC                      */
#define F_ARGT4_X_S                     10 /* S$ = ARGT$( X )                */
#define F_ARGV_X_N                      11 /* N  = ARGV( X )                 */
#define F_ARGV4_X_S                     12 /* S$ = ARGV$( X )                */
#define F_ASC_A_N                       13 /* N  = ASC( A$ )                 */
#define F_ASC_A_X_N                     14 /* N  = ASC( A$, X )              */
#define F_ASCII_A_N                     15 /* N  = ASCII( A$ )               */
#define F_ASIN_X_N                      16 /* N  = ASIN( X )                 */
#define F_ASN_X_N                       17 /* N  = ASN( X )                  */
#define F_ASND_X_N                      18 /* N  = ASND( X )                 */
#define F_ASNG_X_N                      19 /* N  = ASNG( X )                 */
#define F_ATAN_X_N                      20 /* N  = ATAN( X )                 */
#define F_ATN_X_N                       21 /* N  = ATN( X )                  */
#define F_ATND_X_N                      22 /* N  = ATND( X )                 */
#define F_ATNG_X_N                      23 /* N  = ATNG( X )                 */
#define F_BASE_N                        24 /* N  = BASE                      */
#define F_CATALOG_N                     25 /* N  = CATALOG                   */
#define F_CATALOG_A_N                   26 /* N  = CATALOG( A$ )             */
#define F_CCUR_X_N                      27 /* N  = CCUR( X )                 */
#define F_CDBL_X_N                      28 /* N  = CDBL( X )                 */
#define F_CEIL_X_N                      29 /* N  = CEIL( X )                 */
#define F_CHAR_X_Y_S                    30 /* S$ = CHAR( X, Y )              */
#define F_CHAR4_X_S                     31 /* S$ = CHAR$( X )                */
#define F_CHDIR_A_N                     32 /* N  = CHDIR( A$ )               */
#define F_CHR_X_S                       33 /* S$ = CHR( X )                  */
#define F_CHR4_X_S                      34 /* S$ = CHR$( X )                 */
#define F_CINT_X_N                      35 /* N  = CINT( X )                 */
#define F_CLG_X_N                       36 /* N  = CLG( X )                  */
#define F_CLK_X_S                       37 /* S$ = CLK( X )                  */
#define F_CLK_X_N                       38 /* N  = CLK( X )                  */
#define F_CLK4_S                        39 /* S$ = CLK$                      */
#define F_CLNG_X_N                      40 /* N  = CLNG( X )                 */
#define F_CLOG_X_N                      41 /* N  = CLOG( X )                 */
#define F_CLOSE_N                       42 /* N  = CLOSE                     */
#define F_CLOSE_X_N                     43 /* N  = CLOSE( X )                */
#define F_CLS_N                         44 /* N  = CLS                       */
#define F_CODE_A_N                      45 /* N  = CODE( A$ )                */
#define F_COLOR_X_Y_N                   46 /* N  = COLOR( X, Y )             */
#define F_COMMAND4_X_S                  47 /* S$ = COMMAND$( X )             */
#define F_COS_X_N                       48 /* N  = COS( X )                  */
#define F_COSD_X_N                      49 /* N  = COSD( X )                 */
#define F_COSG_X_N                      50 /* N  = COSG( X )                 */
#define F_COSH_X_N                      51 /* N  = COSH( X )                 */
#define F_COT_X_N                       52 /* N  = COT( X )                  */
#define F_COUNT_N                       53 /* N  = COUNT                     */
#define F_CSC_X_N                       54 /* N  = CSC( X )                  */
#define F_CSH_X_N                       55 /* N  = CSH( X )                  */
#define F_CSNG_X_N                      56 /* N  = CSNG( X )                 */
#define F_CUR_X_Y_S                     57 /* S$ = CUR( X, Y )               */
#define F_CVC_A_N                       58 /* N  = CVC( A$ )                 */
#define F_CVD_A_N                       59 /* N  = CVD( A$ )                 */
#define F_CVI_A_N                       60 /* N  = CVI( A$ )                 */
#define F_CVL_A_N                       61 /* N  = CVL( A$ )                 */
#define F_CVS_A_N                       62 /* N  = CVS( A$ )                 */
#define F_DAT4_S                        63 /* S$ = DAT$                      */
#define F_DATE_N                        64 /* N  = DATE                      */
#define F_DATE4_S                       65 /* S$ = DATE$                     */
#define F_DATE4_X_S                     66 /* S$ = DATE$( X )                */
#define F_DEF_FN_N                      67 /* N  = DEF FN                    */
#define F_DEG_N                         68 /* N  = DEG                       */
#define F_DEG_X_N                       69 /* N  = DEG( X )                  */
#define F_DEGREE_N                      70 /* N  = DEGREE                    */
#define F_DEGREE_X_N                    71 /* N  = DEGREE( X )               */
#define F_DET_N                         72 /* N  = DET                       */
#define F_DIM_N                         73 /* N  = DIM( ... )                */
#define F_ENVIRON_A_N                   74 /* N  = ENVIRON( A$ )             */
#define F_ENVIRON4_A_S                  75 /* S$ = ENVIRON$( A$ )            */
#define F_EOF_X_N                       76 /* N  = EOF( X )                  */
#define F_EPS_X_N                       77 /* N  = EPS( X )                  */
#define F_ERL_N                         78 /* N  = ERL                       */
#define F_ERR_N                         79 /* N  = ERR                       */
#define F_ERR4_S                        80 /* S$ = ERR$                      */
#define F_ERRL_N                        81 /* N  = ERRL                      */
#define F_ERRN_N                        82 /* N  = ERRN                      */
#define F_ERROR_X_N                     83 /* N  = ERROR( X )                */
#define F_ERROR_X_A_N                   84 /* N  = ERROR( X, A$ )            */
#define F_ERROR4_S                      85 /* S$ = ERROR$                    */
#define F_EXAM_X_N                      86 /* N  = EXAM( X )                 */
#define F_EXP_X_N                       87 /* N  = EXP( X )                  */
#define F_FETCH_X_N                     88 /* N  = FETCH( X )                */
#define F_FILEATTR_X_Y_N                89 /* N  = FILEATTR( X, Y )          */
#define F_FILES_N                       90 /* N  = FILES                     */
#define F_FILES_A_N                     91 /* N  = FILES( A$ )               */
#define F_FILL_X_Y_N                    92 /* N  = FILL( X, Y )              */
#define F_FIX_X_N                       93 /* N  = FIX( X )                  */
#define F_FLOAT_X_N                     94 /* N  = FLOAT( X )                */
#define F_FLOW_N                        95 /* N  = FLOW                      */
#define F_FP_X_N                        96 /* N  = FP( X )                   */
#define F_FRAC_X_N                      97 /* N  = FRAC( X )                 */
#define F_FRE_N                         98 /* N  = FRE                       */
#define F_FRE_A_N                       99 /* N  = FRE( A$ )                 */
#define F_FRE_X_N                      100 /* N  = FRE( X )                  */
#define F_FREE_N                       101 /* N  = FREE                      */
#define F_FREE_A_N                     102 /* N  = FREE( A$ )                */
#define F_FREE_X_N                     103 /* N  = FREE( X )                 */
#define F_FREEFILE_N                   104 /* N  = FREEFILE                  */
#define F_GET_X_N                      105 /* N  = GET( X )                  */
#define F_GET_X_Y_N                    106 /* N  = GET( X, Y )               */
#define F_GRAD_N                       107 /* N  = GRAD                      */
#define F_GRADIAN_N                    108 /* N  = GRADIAN                   */
#define F_HCS_X_N                      109 /* N  = HCS( X )                  */
#define F_HEX4_X_S                     110 /* S$ = HEX$( X )                 */
#define F_HOME_N                       111 /* N  = HOME                      */
#define F_HSN_X_N                      112 /* N  = HSN( X )                  */
#define F_HTN_X_N                      113 /* N  = HTN( X )                  */
#define F_INDEX_A_B_N                  114 /* N  = INDEX( A$, B$ )           */
#define F_INITIALIZE_N                 115 /* N  = INITIALIZE                */
#define F_INKEY4_S                     116 /* S$ = INKEY$                    */
#define F_INP_X_N                      117 /* N  = INP( X )                  */
#define F_INPUT4_X_S                   118 /* S$ = INPUT$( X )               */
#define F_INPUT4_X_Y_S                 119 /* S$ = INPUT$( X, Y )            */
#define F_INSTR_A_B_N                  120 /* N  = INSTR( A$, B$ )           */
#define F_INSTR_X_A_B_N                121 /* N  = INSTR( X, A$, B$ )        */
#define F_INT_X_N                      122 /* N  = INT( X )                  */
#define F_INT5_X_N                     123 /* N  = INT%( X )                 */
#define F_IP_X_N                       124 /* N  = IP( X )                   */
#define F_KEY_S                        125 /* S$ = KEY                       */
#define F_KEY4_S                       126 /* S$ = KEY$                      */
#define F_KILL_A_N                     127 /* N  = KILL( A$ )                */
#define F_LBOUND_N                     128 /* N  = LBOUND( ... )             */
#define F_LCASE4_A_S                   129 /* S$ = LCASE$( A$ )              */
#define F_LEFT_A_X_S                   130 /* S$ = LEFT( A$, X )             */
#define F_LEFT4_A_X_S                  131 /* S$ = LEFT$( A$, X )            */
#define F_LEN_A_N                      132 /* N  = LEN( A$ )                 */
#define F_LGT_X_N                      133 /* N  = LGT( X )                  */
#define F_LIN_X_S                      134 /* S$ = LIN( X )                  */
#define F_LN_X_N                       135 /* N  = LN( X )                   */
#define F_LOC_X_N                      136 /* N  = LOC( X )                  */
#define F_LOCATE_X_Y_N                 137 /* N  = LOCATE( X, Y )            */
#define F_LOF_X_N                      138 /* N  = LOF( X )                  */
#define F_LOG_X_N                      139 /* N  = LOG( X )                  */
#define F_LOG10_X_N                    140 /* N  = LOG10( X )                */
#define F_LOG2_X_N                     141 /* N  = LOG2( X )                 */
#define F_LOGE_X_N                     142 /* N  = LOGE( X )                 */
#define F_LPOS_N                       143 /* N  = LPOS                      */
#define F_LTRIM4_A_S                   144 /* S$ = LTRIM$( A$ )              */
#define F_LTW_X_N                      145 /* N  = LTW( X )                  */
#define F_LWIDTH_X_N                   146 /* N  = LWIDTH( X )               */
#define F_MATCH_A_B_X_N                147 /* N  = MATCH( A$, B$, X )        */
#define F_MAX_X_Y_N                    148 /* N  = MAX( X, Y )               */
#define F_MAX_A_B_S                    149 /* S$ = MAX( A$, B$ )             */
#define F_MAXBYT_N                     150 /* N  = MAXBYT                    */
#define F_MAXCUR_N                     151 /* N  = MAXCUR                    */
#define F_MAXDBL_N                     152 /* N  = MAXDBL                    */
#define F_MAXDEV_N                     153 /* N  = MAXDEV                    */
#define F_MAXINT_N                     154 /* N  = MAXINT                    */
#define F_MAXLEN_A_N                   155 /* N  = MAXLEN( A$ )              */
#define F_MAXLNG_N                     156 /* N  = MAXLNG                    */
#define F_MAXLVL_N                     157 /* N  = MAXLVL                    */
#define F_MAXNUM_N                     158 /* N  = MAXNUM                    */
#define F_MAXSNG_N                     159 /* N  = MAXSNG                    */
#define F_MEM_N                        160 /* N  = MEM                       */
#define F_MID_A_X_S                    161 /* S$ = MID( A$, X )              */
#define F_MID_A_X_Y_S                  162 /* S$ = MID( A$, X, Y )           */
#define F_MID4_A_X_S                   163 /* S$ = MID$( A$, X )             */
#define F_MID4_A_X_Y_S                 164 /* S$ = MID$( A$, X, Y )          */
#define F_MIN_X_Y_N                    165 /* N  = MIN( X, Y )               */
#define F_MIN_A_B_S                    166 /* S$ = MIN( A$, B$ )             */
#define F_MINBYT_N                     167 /* N  = MINBYT                    */
#define F_MINCUR_N                     168 /* N  = MINCUR                    */
#define F_MINDBL_N                     169 /* N  = MINDBL                    */
#define F_MINDEV_N                     170 /* N  = MINDEV                    */
#define F_MININT_N                     171 /* N  = MININT                    */
#define F_MINLNG_N                     172 /* N  = MINLNG                    */
#define F_MINNUM_N                     173 /* N  = MINNUM                    */
#define F_MINSNG_N                     174 /* N  = MINSNG                    */
#define F_MKC4_X_S                     175 /* S$ = MKC$( X )                 */
#define F_MKD4_X_S                     176 /* S$ = MKD$( X )                 */
#define F_MKDIR_A_N                    177 /* N  = MKDIR( A$ )               */
#define F_MKI4_X_S                     178 /* S$ = MKI$( X )                 */
#define F_MKL4_X_S                     179 /* S$ = MKL$( X )                 */
#define F_MKS4_X_S                     180 /* S$ = MKS$( X )                 */
#define F_MOD_X_Y_N                    181 /* N  = MOD( X, Y )               */
#define F_NAME_A_B_N                   182 /* N  = NAME( A$, B$ )            */
#define F_NOFLOW_N                     183 /* N  = NOFLOW                    */
#define F_NOTRACE_N                    184 /* N  = NOTRACE                   */
#define F_NULL_X_N                     185 /* N  = NULL( X )                 */
#define F_NUM_N                        186 /* N  = NUM                       */
#define F_NUM_A_N                      187 /* N  = NUM( A$ )                 */
#define F_NUM4_X_S                     188 /* S$ = NUM$( X )                 */
#define F_OCT4_X_S                     189 /* S$ = OCT$( X )                 */
#define F_OPEN_A_X_B_N                 190 /* N  = OPEN( A$, X, B$ )         */
#define F_OPEN_A_X_B_Y_N               191 /* N  = OPEN( A$, X, B$, Y )      */
#define F_ORD_A_N                      192 /* N  = ORD( A$ )                 */
#define F_OUT_X_Y_N                    193 /* N  = OUT( X, Y )               */
#define F_PAUSE_X_N                    194 /* N  = PAUSE( X )                */
#define F_PDL_X_N                      195 /* N  = PDL( X )                  */
#define F_PEEK_X_N                     196 /* N  = PEEK( X )                 */
#define F_PI_N                         197 /* N  = PI                        */
#define F_PI_X_N                       198 /* N  = PI( X )                   */
#define F_PIN_X_N                      199 /* N  = PIN( X )                  */
#define F_POKE_X_Y_N                   200 /* N  = POKE( X, Y )              */
#define F_POS_N                        201 /* N  = POS                       */
#define F_POS_X_N                      202 /* N  = POS( X )                  */
#define F_POS_A_B_N                    203 /* N  = POS( A$, B$ )             */
#define F_POS_A_B_X_N                  204 /* N  = POS( A$, B$, X )          */
#define F_PUT_X_N                      205 /* N  = PUT( X )                  */
#define F_PUT_X_Y_N                    206 /* N  = PUT( X, Y )               */
#define F_RAD_N                        207 /* N  = RAD                       */
#define F_RAD_X_N                      208 /* N  = RAD( X )                  */
#define F_RADIAN_N                     209 /* N  = RADIAN                    */
#define F_RAN_N                        210 /* N  = RAN                       */
#define F_RAN_X_N                      211 /* N  = RAN( X )                  */
#define F_RANDOM_N                     212 /* N  = RANDOM                    */
#define F_RANDOM_X_N                   213 /* N  = RANDOM( X )               */
#define F_RANDOMIZE_N                  214 /* N  = RANDOMIZE                 */
#define F_RANDOMIZE_X_N                215 /* N  = RANDOMIZE( X )            */
#define F_REMAINDER_X_Y_N              216 /* N  = REMAINDER( X, Y )         */
#define F_RENAME_A_B_N                 217 /* N  = RENAME( A$, B$ )          */
#define F_REPEAT4_X_A_S                218 /* S$ = REPEAT$( X, A$ )          */
#define F_REPEAT4_X_Y_S                219 /* S$ = REPEAT$( X, Y )           */
#define F_RESET_N                      220 /* N  = RESET                     */
#define F_RIGHT_A_X_S                  221 /* S$ = RIGHT( A$, X )            */
#define F_RIGHT4_A_X_S                 222 /* S$ = RIGHT$( A$, X )           */
#define F_RMDIR_A_N                    223 /* N  = RMDIR( A$ )               */
#define F_RND_N                        224 /* N  = RND                       */
#define F_RND_X_N                      225 /* N  = RND( X )                  */
#define F_ROUND_X_Y_N                  226 /* N  = ROUND( X, Y )             */
#define F_RTRIM4_A_S                   227 /* S$ = RTRIM$( A$ )              */
#define F_SEC_X_N                      228 /* N  = SEC( X )                  */
#define F_SEEK_X_N                     229 /* N  = SEEK( X )                 */
#define F_SEEK_X_Y_N                   230 /* N  = SEEK( X, Y )              */
#define F_SEG_A_X_Y_S                  231 /* S$ = SEG( A$, X, Y )           */
#define F_SEG4_A_X_Y_S                 232 /* S$ = SEG$( A$, X, Y )          */
#define F_SGN_X_N                      233 /* N  = SGN( X )                  */
#define F_SHELL_A_N                    234 /* N  = SHELL( A$ )               */
#define F_SIN_X_N                      235 /* N  = SIN( X )                  */
#define F_SIND_X_N                     236 /* N  = SIND( X )                 */
#define F_SING_X_N                     237 /* N  = SING( X )                 */
#define F_SINH_X_N                     238 /* N  = SINH( X )                 */
#define F_SIZE_A_N                     239 /* N  = SIZE( A$ )                */
#define F_SIZE_N                       240 /* N  = SIZE( ... )               */
#define F_SLEEP_X_N                    241 /* N  = SLEEP( X )                */
#define F_SNH_X_N                      242 /* N  = SNH( X )                  */
#define F_SPA_X_S                      243 /* S$ = SPA( X )                  */
#define F_SPACE_X_S                    244 /* S$ = SPACE( X )                */
#define F_SPACE4_X_S                   245 /* S$ = SPACE$( X )               */
#define F_SPC_X_S                      246 /* S$ = SPC( X )                  */
#define F_SQR_X_N                      247 /* N  = SQR( X )                  */
#define F_SQRT_X_N                     248 /* N  = SQRT( X )                 */
#define F_STR_X_Y_S                    249 /* S$ = STR( X, Y )               */
#define F_STR4_X_S                     250 /* S$ = STR$( X )                 */
#define F_STRING_X_Y_S                 251 /* S$ = STRING( X, Y )            */
#define F_STRING4_X_A_S                252 /* S$ = STRING$( X, A$ )          */
#define F_STRING4_X_Y_S                253 /* S$ = STRING$( X, Y )           */
#define F_STUFF_X_Y_N                  254 /* N  = STUFF( X, Y )             */
#define F_TAB_X_S                      255 /* S$ = TAB( X )                  */
#define F_TAN_X_N                      256 /* N  = TAN( X )                  */
#define F_TAND_X_N                     257 /* N  = TAND( X )                 */
#define F_TANG_X_N                     258 /* N  = TANG( X )                 */
#define F_TANH_X_N                     259 /* N  = TANH( X )                 */
#define F_TI_N                         260 /* N  = TI                        */
#define F_TI4_S                        261 /* S$ = TI$                       */
#define F_TIM_N                        262 /* N  = TIM                       */
#define F_TIM_X_N                      263 /* N  = TIM( X )                  */
#define F_TIME_N                       264 /* N  = TIME                      */
#define F_TIME_X_N                     265 /* N  = TIME( X )                 */
#define F_TIME4_S                      266 /* S$ = TIME$                     */
#define F_TIME4_X_S                    267 /* S$ = TIME$( X )                */
#define F_TIMER_N                      268 /* N  = TIMER                     */
#define F_TOP_N                        269 /* N  = TOP                       */
#define F_TRACE_N                      270 /* N  = TRACE                     */
#define F_TRIM4_A_S                    271 /* S$ = TRIM$( A$ )               */
#define F_TROFF_N                      272 /* N  = TROFF                     */
#define F_TRON_N                       273 /* N  = TRON                      */
#define F_TRUNCATE_X_Y_N               274 /* N  = TRUNCATE( X, Y )          */
#define F_UBOUND_N                     275 /* N  = UBOUND( ... )             */
#define F_UCASE4_A_S                   276 /* S$ = UCASE$( A$ )              */
#define F_UNSAVE_A_N                   277 /* N  = UNSAVE( A$ )              */
#define F_VAL_A_N                      278 /* N  = VAL( A$ )                 */
#define F_VTAB_X_N                     279 /* N  = VTAB( X )                 */
#define F_WAIT_X_N                     280 /* N  = WAIT( X )                 */
#define F_WAIT_X_Y_N                   281 /* N  = WAIT( X, Y )              */
#define F_WAIT_X_Y_Z_N                 282 /* N  = WAIT( X, Y, Z )           */
#define F_WIDTH_X_N                    283 /* N  = WIDTH( X )                */
#define F_WIDTH_X_Y_N                  284 /* N  = WIDTH( X, Y )             */
#define NUM_FUNCTIONS 284









/***************************************************************
  
   bwbasic.h   Part II: Structures
  
***************************************************************/
typedef unsigned long OptionVersionType;  /* OPTION VERSION bitmask, upto 32 BASIC dialects */



/* Typdef structure for strings under Bywater BASIC */

struct StringStructure
{
   BasicStringLengthType length;  /* length of string */
   char                 *sbuffer; /* pointer to string buffer */
};
typedef struct StringStructure StringType;

/* Structure used for all variables under Bywater BASIC */

struct VariableStructure
{
   char             name[ BasicNameLengthMax + 1 ];   /* name */
   BasicNumberType *memnum;                   /* memory for number */
   StringType      *memstr;                   /* memory for string */
   size_t           array_units;              /* total number of units of memory */
   int              LBOUND[ MAX_DIMS ];
   int              UBOUND[ MAX_DIMS ];
   int              array_pos[ MAX_DIMS ];    /* current position in array */
   int              dimensions;               /* number of dimensions, 0 == scalar */
   unsigned char    VariableFlags;
   char             VariableTypeChar;
   struct VariableStructure *next;            /* next variable in chain */
};
typedef struct VariableStructure VariableType;

#define VAR_IS_STRING( v ) ( v->VariableTypeChar == BasicStringSuffix )
/* VariableFlags */
#define VARIABLE_RESERVED_01 0x01  /* reserved for future use */
#define VARIABLE_RESERVED_02 0x02  /* reserved for future use */
#define VARIABLE_RESERVED_04 0x04  /* reserved for future use */
#define VARIABLE_RESERVED_08 0x08  /* reserved for future use */
#if NEW_VIRTUAL
#define VARIABLE_VIRTUAL     0x10  /* VIRTUAL variable: values are stored in disk file */
#else
#define VARIABLE_RESERVED_10 0x10  /* reserved for future use */
#endif /* NEW_VIRTUAL */
#define VARIABLE_DISPLAY     0x20  /* DSP variable: display every assignment */
#define VARIABLE_PRESET      0x40  /* preset variable: CLEAR should not delete */
#define VARIABLE_COMMON      0x80  /* COMMON variable: available to CHAINed program */


/* Structure to represent program lines under Bywater BASIC */

struct LineStructure
{
   struct LineStructure *next;      /* pointer to next line in chain */
   BasicLineNumberType   number;    /* line number */
   char                 *buffer;    /* buffer to hold the line */
   int                   position;  /* current position in line */
   BasicUniqueType       cmdnum;    /* C_... */
   int                   Startpos;  /* start of rest of line read from buffer */
   struct LineStructure *OtherLine; /* pointer to other line in loops */
   int                   Indention;
   int                   IncludeLevel; /* %INCLUDE */
   unsigned char         LineFlags;  /* '*' if executed, ' ' otherwise */
};
typedef struct LineStructure LineType;

/* Structure used for all INTRINSIC functions under Bywater BASIC */

struct FunctionStructure
{
   BasicUniqueType UniqueID;  /* If USER, then LineNumber, else F_... */
   const char     *Syntax;
   const char     *Description;
   char            Name[BasicNameLengthMax + 1];
   unsigned int    ReturnType;      /* MSB: 0x00==INTRINSIC, l->cmdnum; LSB same as VariableType */
   unsigned char   ParameterCount;  /* 0..MAX_FARGS, 255 == VARIANT (...) */
   unsigned long   ParameterTypes;  /* parameter signature, must hold MAX_FARGS bits */
   unsigned long   ParameterTests;  /* parameter checks, must hold 8 nibbles, only first 8 parameters */
   struct FunctionStructure *next;  /* NextPointer */
   OptionVersionType OptionVersionBitmask;   /* OPTION VERSION bitmask */
};
typedef struct FunctionStructure FunctionType;

/* Structure to represent all INTRINSIC commands under Bywater BASIC */

struct CommandStructure
{
   BasicUniqueType   id;  /* C_... */
   const char       *Syntax;
   const char       *Description;
   char              name[BasicNameLengthMax + 1];
   OptionVersionType OptionVersionBitmask;   /* OPTION VERSION bitmask */
};
typedef struct CommandStructure CommandType;

/* Structure to define DEVICE (file) for Bywater BASIC */

struct FileStructure
{
   struct FileStructure * next;
   int             FileNumber;
   int             mode;   /* DEVMODE_ item */
   int             width;  /* width for OUTPUT and APPEND; reclen for RANDOM; not used for INPUT or BINARY */
   int             col; /* current column for OUTPUT and APPEND */
   int             row; /* current row for OUTPUT and APPEND */
   int             EOF_LineNumber; /* CBASIC-II: IF END # filenumber THEN linenumber */
   char            delimit; /* DELIMIT for READ and WRITE */
   char            filename[FILENAME_MAX + 1];  /* filename */
   FILE           *cfp; /* C file pointer for this device */
   char           *buffer; /* pointer to character buffer for RANDOM */
};
typedef struct FileStructure FileType;

struct FieldStructure
{
   /* only used for RANDOM files in dialects with a FIELD command */
   struct FieldStructure * next;
   FileType     * File;
   int            FieldOffset;
   int            FieldLength;
   VariableType * Var;
   int            array_pos[ MAX_DIMS ];    /* current position in array */
};
typedef struct FieldStructure FieldType;

#if NEW_VIRTUAL
struct VirtualStructure
{
   /* only used for VIRTUAL variables in dialects with a DIM # command */
   struct VirtualStructure * next;
   int            FileNumber;  /* note: the file can be opened AFTER the variable is declared */
   size_t         FileOffset;  /* byte offset from the beginning of the file */
   int            FileLength;  /* sizeof( BasicNumberType ) or Fixed String Length */
   VariableType * Variable;    /* the variable */
};
typedef struct VirtualStructure VirtualType;
#endif /* NEW_VIRTUAL */

/* Structure to define expression stack elements under Bywater BASIC : REMOVED */

/* structure for FUNCTION-SUB lookup table element */

struct LookupStructure
{
   char                     *name;
   unsigned char             ParameterCount;  /* 0..MAX_FARGS, 255 == VARIANT (...) */
   unsigned long             ParameterTypes;  /* parameter signature, must hold MAX_FARGS bits */
   LineType                 *line;
   int                       startpos;        /* starting position in line */
   struct LookupStructure   *next;
   VariableType             *local_variable;
   OptionVersionType         OptionVersionBitmask;   /* OPTION VERSION bitmask */
};
typedef struct LookupStructure LookupType;

/* Structure to define EXEC stack elements */

struct StackStructure
{
   LineType            *line;  /* line for execution */
   int                  code;   /* EXEC_... */
   VariableType        *local_variable;   /* local variables of FUNCTION and variable of FOR */
   BasicNumberType      for_step;  /* STEP value of FOR */
   BasicNumberType      for_target;   /* target value of FOR */
   LineType            *LoopTopLine; /* top line of FOR, DO, WHILE, UNTIL */
   BasicLineNumberType  OnErrorGoto; /* ON ERROR GOTO line */
   struct StackStructure * next;
};
typedef struct StackStructure StackType;

/* Structure to define bwBASIC task: REMOVED */

/* Structure to define OPERATORS: REMOVED */

/* Structure to define VERSION information, for OPTION VERSION command */

struct VersionStructure
{
   char           *Name;
   OptionVersionType OptionVersionBitmask;   /* OPTION VERSION bitmask */
   char           *Description;
   unsigned short  Year;
   char           *ID;
   char            OptionCommentChar;   /* Comment, commonly an apostrophe */
   char            OptionStatementChar; /* Statement Seperator, commonly a colon */
   char            OptionPrintChar;     /* Print Statement, commonly question mark */
   char            OptionImageChar;     /* Image Statement, commonly a colon */
   unsigned short  OptionFlags;
   int             OptionBaseValue;      /* Default lower bound for arrays */
   char            OptionDateFormat[32]; /* DATE$ */
   char            OptionTimeFormat[32]; /* TIME$ */
   char            UsingDigit;      /* PRINT USING Digit placeholder, commonly '#' */
   char            UsingComma;      /* PRINT USING Comma, such as thousands and millions, commonly ',' */
   char            UsingPeriod;     /* PRINT USING Period, such as between dollars and cents, commonly '.' */
   char            UsingPlus;       /* PRINT USING Plus  sign, positive value, commonly '+' */
   char            UsingMinus;      /* PRINT USING Minus sign, negative value, commonly '-' */
   char            UsingExrad;      /* PRINT USING Exponential format, commonly '^' */
   char            UsingDollar;     /* PRINT USING Currency symbol, such as dollar, commonly '$' */
   char            UsingFiller;     /* PRINT USING Filler, such as payroll checks, commonly '*' */
   char            UsingLiteral;    /* PRINT USING Literal, commonly '_' */
   char            UsingFirst;      /* PRINT USING First character of a string, commonly '!' */
   char            UsingAll;        /* PRINT USING All of a string, commonly '&' */
   char            UsingLength;     /* PRINT USING Length of a string, commonly '%' */
};
typedef struct VersionStructure VersionType;
#define IS_CHAR( X, Y ) ( (Y) != BasicNulChar && (X) == (Y) )

struct VariantStructure
{
   char                      TypeChar; /* BASIC Suffix ('$' and so on), NUL == unknown */
   BasicNumberType           Number;   /* only when TypeChar != '$' */
   size_t                    Length;   /* only when TypeChar == '$' */
   char *                    Buffer;   /* only when TypeChar == '$' */
};
typedef struct VariantStructure VariantType;

/* 
any routine changing the TypeChar is required to calloc() or free() the Buffer, 
this is checked by is_string_type() and is_number_type() in bwb_exp.c 
*/

#define CLEAR_VARIANT( X ) bwb_memset( X, 0, sizeof( VariantType ) )
#define RELEASE( X ) if( X->Buffer != NULL ) { free( X->Buffer ); X->Buffer = NULL; X->Length = 0; }

/***************************************************************
  
   bwbasic.h   Part III: Global Data
  
***************************************************************/

struct GlobalStruct
{
   jmp_buf         mark;                          /* break_handler() jumps here */
   int             program_run;                   /* has the command-line program been run? */
   VersionType    *CurrentVersion;                /* OPTION VERSION */
   LineType       *ThisLine;                      /* currently executing line */
   int             rescan;                        /* program needs to be rescanned */
   int             bwb_trace;
   int             IsInteractive;
   int             IsCommandLineFile;
   int             MaintainerDebugOn;             /* MAINTAINER DEBUG ON */
   int             LPRINT_NULLS;                  /* LPRINT_FILE_NUMBER */
   int             SCREEN_ROWS;                   /* CONSOLE_FILE_NUMBER */
   int             AutomaticLineNumber;           /* AUTO */
   int             AutomaticLineIncrement;        /* AUTO */
   int             LastInputCount;                /* 0 == none */
   int             LastLineNumber;                /* LIST, LLIST */
   int             last_inp_adv_rval;             /* JBV */
   int             LastFileNumber;                /* 0 == none */
   int             StartTime;                     /* when RUN was executed */
   int             NextValidLineNumber;           /* verify line number order */
   int             stack_level;                   /* stack list count */
   LineType       *data_line;                     /* current DATA line to READ */
   int             data_pos;                      /* position in data_line */
   int             err_number;                    /* ERR */
   LineType       *err_line;                      /* ERL */
   BasicNumberType LastDeterminant;               /* 0 == error */
   FILE           *ExternalInputFile;             /* for --TAPE filename */
   FILE           *MaintainerDebugFile;           /* MAINTAINER DEBUG ON */
   BasicLineNumberType tmr_line;                  /* ON TIMER ... GOSUB ... */
   BasicNumberType tmr_count;                     /* ON TIMER ... GOSUB ... */
   BasicNumberType tmr_expires;                   /* ON TIMER ... GOSUB ... */
   LineType       *ContinueLine;                  /* CONT */
   int             IncludeLevel;                  /* %INCLUDE */
   /* these values are not changed by OPTION VERSION */
   BasicNumberType OptionSleepValue;              /* OPTION SLEEP   value */
   int             OptionIndentValue;             /* OPTION INDENT  value */
   BasicUniqueType OptionTerminalType;            /* OPTION TERMINAL type */
   BasicUniqueType OptionRoundType;               /* OPTION ROUND    type */
   LineType        bwb_start;                     /* starting line marker */
   LineType        bwb_end;                       /* ending line marker */
   LineType        user_line;
   char           *read_line;
   char            EventQueue;                    /* may change to int/long/array */
   VariableType   *var_head;                      /* variable list */
   FunctionType   *fnc_head;                      /* function list */
   LookupType     *fslt_head;                     /* BASIC function-sub-label list */
   StackType      *stack_head;                    /* stack list */
   FileType       *file_head;                     /* file list (does NOT include SYSIN, SYSOUT, or SYSPRN) */
   FieldType      *field_head;                    /* field list */
#if NEW_VIRTUAL
   VirtualType    *virtual_head;                  /* virtual list */
#endif /* NEW_VIRTUAL */

   FileType       *SYSIN;                         /* FileNumber = 0, INPUT, stdin  */
   FileType       *SYSOUT;                        /* FileNumber = 0, PRINT, stdout */
   FileType       *SYSPRN;                        /* FileNumber < 0, LPRINT, stderr */
   int             IsPrinter;                     /* CBASIC-II: LPRINTER sets to TRUE, CONSOLE sets to FALSE */   
   FileType       *CurrentFile;                   /* current file for PRINT or INPUT */
   char           *bwb_ebuf;                      /* buffer for generating error messages */
   char           *UseParameter;                  /* CHAIN ... USE */

   int             DefaultVariableType[ 26 ];     /* VariableTypeChar, A-Z */
#if THE_PRICE_IS_RIGHT
   int             CommandStart[ 26 ];            /* bwb_command->name[0], A-Z */
   int             FunctionStart[ 26 ];           /* bwb_prefuncs->name[0], A-Z */
#endif
   char           *COMMAND5[ 10 ];                /* command line parameters, COMMAND$(0-9) */
   char            progfile[ FILENAME_MAX + 1 ];  /* program filename */
   char            ErrMsg[ 256 ];                 /* ERROR$ */
};
typedef struct GlobalStruct GlobalType;

extern GlobalType * My;


extern CommandType  bwb_cmdtable[ NUM_COMMANDS  ]; /* table of predefined commands  */
extern FunctionType bwb_prefuncs[ NUM_FUNCTIONS ]; /* table of predefined functions */
extern VersionType  bwb_vertable[ NUM_VERSIONS  ]; /* table of predefined versions  */



/***************************************************************
  
   bwbasic.h   Part IV: Function Prototypes
  
***************************************************************/

extern void BasicNumerc ( BasicNumberType Input, char *Output );
extern void break_handler ( void );
extern void break_mes ( int x );
extern int buff_is_eol ( char *buffer, int * position );
extern int buff_peek_array_dimensions ( char *buffer, int *position, int *n_params );
extern int buff_peek_char ( char *buffer, int * position, char find );
extern int buff_peek_word ( char *buffer, int * position, char * find );
extern int buff_read_array_dimensions ( char *buffer, int *position, int *n_params, int params[ ] );
extern int buff_read_element ( char * buffer, int * position, char * element );
extern int buff_read_expression ( char * buffer, int * position, VariantType * X );
extern int buff_read_index_item ( char * buffer, int * position, int Index, int * Value );
extern int buff_read_integer_expression ( char * buffer, int * position, int * Value );
extern int buff_read_label ( char * buffer, int * position, char * label );
extern int buff_read_letter_sequence ( char *buffer, int *position, char *start, char *end );
extern int buff_read_line_number ( char * buffer, int * position, int * linenum );
extern int buff_read_line_sequence ( char *buffer, int * position, int * head, int * tail );
extern VariableType * buff_read_matrix ( char *buffer, int * position );
extern int buff_read_numeric_expression ( char * buffer, int * position, BasicNumberType * Value );
extern VariableType * buff_read_scalar ( char *buffer, int * position );
extern int buff_read_string_expression ( char * buffer, int * position, char ** Value );
extern char buff_read_type_declaration ( char *buffer, int * position );
extern int buff_read_varname ( char *buffer, int * position, char * varname );
extern int buff_skip_char ( char *buffer, int * position, char find );
extern int buff_skip_comma ( char *buffer, int * position );
extern int buff_skip_eol ( char *buffer, int * position );
extern int buff_skip_spaces ( char *buffer, int * position );
extern int buff_skip_word ( char *buffer, int * position, char * find );
extern LineType * bwb_APPEND ( LineType * l );
extern LineType * bwb_AS ( LineType * l );
extern LineType * bwb_AUTO ( LineType * l );
extern LineType * bwb_BACKSPACE ( LineType * l );
extern LineType * bwb_BREAK ( LineType * l );
extern LineType * bwb_BYE ( LineType * l );
extern LineType * bwb_CALL ( LineType * l );
extern LineType * bwb_CASE ( LineType * l );
extern LineType * bwb_CASE_ELSE ( LineType * l );
extern LineType * bwb_CHAIN ( LineType * l );
extern LineType * bwb_CHANGE ( LineType * l );
extern LineType * bwb_CLEAR ( LineType * l );
extern LineType * bwb_CLOAD ( LineType * l );
extern LineType * bwb_CLOAD8 ( LineType * l );
extern LineType * bwb_CLOSE ( LineType * l );
extern LineType * bwb_CLR ( LineType * l );
extern void bwb_clrexec( void );
extern LineType * bwb_CMDS ( LineType * l );
extern LineType * bwb_COMMON ( LineType * l );
extern LineType * bwb_CONSOLE ( LineType * l );
extern LineType * bwb_CONT ( LineType * l );
extern LineType * bwb_CREATE ( LineType * l );
extern LineType * bwb_CSAVE ( LineType * l );
extern LineType * bwb_CSAVE8 ( LineType * l );
extern LineType * bwb_DATA ( LineType * l );
extern void bwb_decexec ( void );
extern LineType * bwb_DEF ( LineType * l );
extern LineType * bwb_DEFBYT ( LineType * l );
extern LineType * bwb_DEFCUR ( LineType * l );
extern LineType * bwb_DEFDBL ( LineType * l );
extern LineType * bwb_DEFINT ( LineType * l );
extern LineType * bwb_DEFLNG ( LineType * l );
extern LineType * bwb_DEFSNG ( LineType * l );
extern LineType * bwb_DEFSTR ( LineType * l );
extern LineType * bwb_DEF_SUB ( LineType * l );
extern LineType * bwb_DELETE ( LineType * l );
extern LineType * bwb_DELIMIT ( LineType * l );
extern LineType * bwb_DIM ( LineType * l );
extern LineType * bwb_DO ( LineType * l );
extern LineType * bwb_DSP ( LineType * l );
extern LineType * bwb_EDIT ( LineType * l );
extern LineType * bwb_ELSE ( LineType * l );
extern LineType * bwb_ELSEIF ( LineType * l );
extern LineType * bwb_END ( LineType * l );
extern LineType * bwb_END_FUNCTION ( LineType * l );
extern LineType * bwb_END_IF ( LineType * l );
extern LineType * bwb_END_SELECT ( LineType * l );
extern LineType * bwb_END_SUB ( LineType * l );
extern LineType * bwb_ERASE ( LineType * l );
extern LineType * bwb_EXCHANGE ( LineType * l );
extern void bwb_execline ( void );
extern LineType * bwb_EXIT ( LineType * l );
extern LineType * bwb_EXIT_DO ( LineType * l );
extern LineType * bwb_EXIT_FOR ( LineType * l );
extern LineType * bwb_EXIT_FUNCTION ( LineType * l );
extern LineType * bwb_EXIT_SUB ( LineType * l );
extern LineType * bwb_EXIT_UNTIL ( LineType * l );
extern LineType * bwb_EXIT_WHILE ( LineType * l );
extern LineType * bwb_FEND ( LineType * l );
extern LineType * bwb_FIELD ( LineType * l );
extern LineType * bwb_FILE ( LineType * l );
extern LineType * bwb_FILES ( LineType * l );
extern int bwb_fload ( char * FileName );
extern LineType * bwb_FNCS ( LineType * l );
extern LineType * bwb_FNEND ( LineType * l );
extern LineType * bwb_FOR ( LineType * l );
extern int bwb_freeline ( LineType * l );
extern LineType * bwb_FUNCTION ( LineType * l );
extern LineType * bwb_GET ( LineType * l );
extern LineType * bwb_GO ( LineType * l );
extern LineType * bwb_GO_SUB ( LineType * l );
extern LineType * bwb_GO_TO ( LineType * l );
extern LineType * bwb_GOODBYE ( LineType * l );
extern LineType * bwb_GOSUB ( LineType * l );
extern LineType * bwb_GOTO ( LineType * l );
extern LineType * bwb_HELP ( LineType * l );
extern LineType * bwb_IF ( LineType * l );
extern LineType * bwb_IF_END ( LineType * l );
extern LineType * bwb_IF_MORE ( LineType * l );
extern LineType * bwb_IF_THEN ( LineType * l );
extern LineType * bwb_IMAGE ( LineType * l );
extern int bwb_incexec ( void );
extern void bwb_init ( void );
extern LineType * bwb_INPUT ( LineType * l );
extern int bwb_interact ( void );
extern int bwb_isalnum ( int C );
extern int bwb_isalpha ( int C );
extern int bwb_iscntrl ( int C );
extern int bwb_isdigit ( int C );
extern int bwb_isgraph ( int C );
extern int bwb_islower ( int C );
extern int bwb_isprint ( int C );
extern int bwb_ispunct ( int C );
extern int bwb_isspace ( int C );
extern int bwb_isupper ( int C );
extern int bwb_isxdigit ( int C );
extern int bwb_is_eof ( FILE * fp );
extern int bwb_ladd ( char *buffer, LineType * p );
extern LineType * bwb_LET ( LineType * line );
extern LineType * bwb_LINE ( LineType * l );
extern LineType * bwb_LIST ( LineType * l );
extern LineType * bwb_LLIST ( LineType * l );
extern LineType * bwb_LOAD ( LineType * l );
extern LineType * bwb_LOOP ( LineType * l );
extern LineType * bwb_LPRINT ( LineType * l );
extern LineType * bwb_LPRINTER ( LineType * l );
extern LineType * bwb_LSET ( LineType * l );
extern void bwb_mainloop ( void );
extern LineType * bwb_MAINTAINER ( LineType * l );
extern LineType * bwb_MARGIN ( LineType * l );
extern LineType * bwb_MAT ( LineType * l );
extern LineType * bwb_MAT_GET ( LineType * l );
extern LineType * bwb_MAT_INPUT ( LineType * l );
extern LineType * bwb_MAT_PRINT ( LineType * l );
extern LineType * bwb_MAT_PUT ( LineType * l );
extern LineType * bwb_MAT_READ ( LineType * l );
extern LineType * bwb_MAT_WRITE ( LineType * l );
extern void * bwb_memchr ( const void *s, int c, size_t n );
extern int bwb_memcmp ( const void *s1, const void *s2, size_t n );
extern void * bwb_memcpy ( void *s1, const void *s2, size_t n );
extern void * bwb_memmove ( void *s1, const void *s2, size_t n );
extern void * bwb_memset ( void *s, int c, size_t n );
extern LineType * bwb_MERGE ( LineType * l );
extern LineType * bwb_MID4 ( LineType * l );
extern LineType * bwb_NAME ( LineType * l );
extern LineType * bwb_NEW ( LineType * l );
extern LineType * bwb_NEXT ( LineType * l );
extern LineType * bwb_OF ( LineType * l );
extern LineType * bwb_OLD ( LineType * l );
extern LineType * bwb_ON ( LineType * l );
extern LineType * bwb_ON_ERROR ( LineType * l );
extern LineType * bwb_ON_ERROR_GOSUB ( LineType * l );
extern LineType * bwb_ON_ERROR_GOTO ( LineType * l );
extern LineType * bwb_ON_ERROR_RESUME ( LineType * l );
extern LineType * bwb_ON_ERROR_RESUME_NEXT ( LineType * l );
extern LineType * bwb_ON_ERROR_RETURN ( LineType * l );
extern LineType * bwb_ON_ERROR_RETURN_NEXT ( LineType * l );
extern LineType * bwb_ON_TIMER ( LineType * l );
extern LineType * bwb_OPEN ( LineType * l );
extern LineType * bwb_OPTION ( LineType * l );
extern LineType * bwb_OPTION_ANGLE ( LineType * l );
extern LineType * bwb_OPTION_ANGLE_DEGREES ( LineType * l );
extern LineType * bwb_OPTION_ANGLE_GRADIANS ( LineType * l );
extern LineType * bwb_OPTION_ANGLE_RADIANS ( LineType * l );
extern LineType * bwb_OPTION_ARITHMETIC ( LineType * l );
extern LineType * bwb_OPTION_ARITHMETIC_DECIMAL ( LineType * l );
extern LineType * bwb_OPTION_ARITHMETIC_FIXED ( LineType * l );
extern LineType * bwb_OPTION_ARITHMETIC_NATIVE ( LineType * l );
extern LineType * bwb_OPTION_BASE ( LineType * l );
extern LineType * bwb_OPTION_BUGS ( LineType * l );
extern LineType * bwb_OPTION_BUGS_OFF ( LineType * l );
extern LineType * bwb_OPTION_BUGS_ON ( LineType * l );
extern LineType * bwb_OPTION_COMMENT ( LineType * l );
extern LineType * bwb_OPTION_COMPARE ( LineType * l );
extern LineType * bwb_OPTION_COMPARE_BINARY ( LineType * l );
extern LineType * bwb_OPTION_COMPARE_DATABASE ( LineType * l );
extern LineType * bwb_OPTION_COMPARE_TEXT ( LineType * l );
extern LineType * bwb_OPTION_COVERAGE ( LineType * l );
extern LineType * bwb_OPTION_COVERAGE_OFF ( LineType * l );
extern LineType * bwb_OPTION_COVERAGE_ON ( LineType * l );
extern LineType * bwb_OPTION_DATE ( LineType * l );
extern LineType * bwb_OPTION_DISABLE ( LineType * l );
extern LineType * bwb_OPTION_DISABLE_COMMAND ( LineType * l );
extern LineType * bwb_OPTION_DISABLE_FUNCTION ( LineType * l );
extern LineType * bwb_OPTION_DISABLE_OPERATOR ( LineType * l );
extern LineType * bwb_OPTION_ENABLE ( LineType * l );
extern LineType * bwb_OPTION_ENABLE_COMMAND ( LineType * l );
extern LineType * bwb_OPTION_ENABLE_FUNCTION ( LineType * l );
extern LineType * bwb_OPTION_ENABLE_OPERATOR ( LineType * l );
extern LineType * bwb_OPTION_ERROR ( LineType * l );
extern LineType * bwb_OPTION_ERROR_GOSUB ( LineType * l );
extern LineType * bwb_OPTION_ERROR_GOTO ( LineType * l );
extern LineType * bwb_OPTION_EXPLICIT ( LineType * l );
extern LineType * bwb_OPTION_IMAGE ( LineType * l );
extern LineType * bwb_OPTION_IMPLICIT ( LineType * l );
extern LineType * bwb_OPTION_INDENT ( LineType * l );
extern LineType * bwb_OPTION_LABELS ( LineType * l );
extern LineType * bwb_OPTION_LABELS_OFF ( LineType * l );
extern LineType * bwb_OPTION_LABELS_ON ( LineType * l );
extern LineType * bwb_OPTION_PRINT ( LineType * l );
extern LineType * bwb_OPTION_ROUND ( LineType * l );
extern LineType * bwb_OPTION_ROUND_BANK ( LineType * l );
extern LineType * bwb_OPTION_ROUND_MATH ( LineType * l );
extern LineType * bwb_OPTION_ROUND_TRUNCATE ( LineType * l );
extern LineType * bwb_OPTION_SLEEP ( LineType * l );
extern LineType * bwb_OPTION_STATEMENT ( LineType * l );
extern LineType * bwb_OPTION_STDERR(LineType * l);
extern LineType * bwb_OPTION_STDIN(LineType * l);
extern LineType * bwb_OPTION_STDOUT(LineType * l);
extern LineType * bwb_OPTION_STRICT ( LineType * l );
extern LineType * bwb_OPTION_STRICT_OFF ( LineType * l );
extern LineType * bwb_OPTION_STRICT_ON ( LineType * l );
extern LineType * bwb_OPTION_TERMINAL ( LineType * l );
extern LineType * bwb_OPTION_TERMINAL_ADM ( LineType * l );
extern LineType * bwb_OPTION_TERMINAL_ANSI ( LineType * l );
extern LineType * bwb_OPTION_TERMINAL_NONE ( LineType * l );
extern LineType * bwb_OPTION_TIME ( LineType * l );
extern LineType * bwb_OPTION_TRACE ( LineType * l );
extern LineType * bwb_OPTION_TRACE_OFF ( LineType * l );
extern LineType * bwb_OPTION_TRACE_ON ( LineType * l );
extern LineType * bwb_OPTION_USING(LineType * l);
extern LineType * bwb_OPTION_VERSION ( LineType * l );
extern LineType * bwb_PAUSE ( LineType * l );
extern LineType * bwb_POP ( LineType * l );
extern LineType * bwb_PRINT ( LineType * l );
extern LineType * bwb_PUT ( LineType * l );
extern LineType * bwb_QUEST ( LineType * l );
extern LineType * bwb_QUIT ( LineType * l );
extern LineType * bwb_READ ( LineType * l );
extern LineType * bwb_RECALL ( LineType * l );
extern LineType * bwb_REM ( LineType * l );
extern LineType * bwb_RENAME ( LineType * l );
extern LineType * bwb_RENUM ( LineType * l );
extern LineType * bwb_RENUMBER ( LineType * l );
extern LineType * bwb_RESET ( LineType * l );
extern LineType * bwb_RESTORE ( LineType * l );
extern LineType * bwb_RESUME ( LineType * l );
extern LineType * bwb_RETURN ( LineType * l );
extern double bwb_rint ( double x );
extern LineType * bwb_RSET ( LineType * l );
extern LineType * bwb_RUN ( LineType * l );
extern LineType * bwb_SAVE ( LineType * l );
extern int bwb_scan ( void );
extern LineType * bwb_SCRATCH ( LineType * l );
extern LineType * bwb_SELECT ( LineType * l );
extern LineType * bwb_SELECT_CASE ( LineType * l );
extern void bwb_single_step ( char * buffer );
extern LineType * bwb_STEP ( LineType * l );
extern LineType * bwb_STOP ( LineType * l );
extern LineType * bwb_STORE ( LineType * l );
extern char * bwb_strcat ( char *s1, const char *s2 );
extern char * bwb_strchr ( const char *s, int c );
extern int bwb_strcmp ( const char *s1, const char *s2 );
extern char * bwb_strcpy ( char *s1, const char *s2 );
extern int bwb_stricmp ( const char *s1, const char *s2 );
extern int bwb_stripcr ( char *s );
extern size_t bwb_strlen ( const char *s );
extern char * bwb_strncat ( char *s1, const char *s2, size_t n );
extern int bwb_strncmp ( const char *s1, const char *s2, size_t n );
extern char * bwb_strncpy ( char *s1, const char *s2, size_t n );
extern int bwb_strnicmp ( const char *s1, const char *s2, size_t n );
extern char * bwb_strrchr ( const char *s, int c );
extern LineType * bwb_SUB ( LineType * l );
extern LineType * bwb_SUBEND ( LineType * l );
extern LineType * bwb_SWAP ( LineType * l );
extern LineType * bwb_SYSTEM ( LineType * l );
extern LineType * bwb_TEXT ( LineType * l );
extern LineType * bwb_THEN ( LineType * l );
extern LineType * bwb_TIMER ( LineType * l );
extern LineType * bwb_TIMER_OFF ( LineType * l );
extern void bwb_Timer_Off ( void );
extern LineType * bwb_TIMER_ON ( LineType * l );
extern void bwb_Timer_On ( void );
extern LineType * bwb_TIMER_STOP ( LineType * l );
extern LineType * bwb_TLOAD ( LineType * l );
extern LineType * bwb_TO ( LineType * l );
extern int bwb_tolower ( int C );
extern int bwb_toupper ( int C );
extern LineType * bwb_TRACE ( LineType * l );
extern LineType * bwb_TRACE_OFF ( LineType * l );
extern LineType * bwb_TRACE_ON ( LineType * l );
extern LineType * bwb_TSAVE ( LineType * l );
extern LineType * bwb_UEND ( LineType * l );
extern LineType * bwb_UNTIL ( LineType * l );
extern LineType * bwb_USE( LineType * l );
extern LineType * bwb_USER_LBL ( LineType * l );
extern LineType * bwb_VARS ( LineType * l );
extern LineType * bwb_vector ( LineType *l );
extern void bwb_Warning ( int ErrorLevel, char *ErrorMessage );
extern void bwb_Warning_Clear ( void );
extern int bwb_Warning_Overflow ( char *Message );
extern int bwb_Warning_Pending ( void );
extern LineType * bwb_WEND ( LineType * l );
extern LineType * bwb_WHILE ( LineType * l );
extern LineType * bwb_WRITE ( LineType * l );
extern LineType * bwb_xlist ( LineType * l, FILE * file );
extern LineType * bwb_xload ( LineType * l );
extern void bwb_xnew ( LineType * l );
extern LineType * bwb_xtxtline ( char *buffer );
extern LineType * bwb_zline ( LineType * l );
extern int bwx_DEBUG ( const char *A );
extern int bwx_input ( char *prompt, char *buffer );
extern int bwx_LPRINT ( char c );
extern int bwx_PRINT ( char c );
extern int bwx_signon ( void );
extern void bwx_STOP ( void );
extern void bwx_terminate ( void );
extern BasicNumberType bwx_TIMER ( BasicNumberType Seconds );
extern void * CALLOC ( size_t nelem, size_t elsize, char *str );
extern int char_is_vartail ( char C );
extern int char_is_vartype ( char C );
extern void CleanLine ( char *buffer );
extern int Determinant ( VariableType * v );
extern void DumpAllCommandHtmlTable ( void );
extern void DumpAllCommandSwitchStatement ( void );
extern void DumpAllCommandSyntax ( void );
extern void DumpAllCommandTableDefinitions ( void );
extern void DumpAllCommandUniqueID ( void );
extern void DumpAllFuctionSyntax ( void );
extern void DumpAllFuctionTableDefinitions ( void );
extern void DumpAllFuctionUniqueID ( void );
extern void DumpAllFunctionHtmlTable ( void );
extern void DumpAllFunctionSwitch ( void );
extern void DumpAllOperatorSyntax( void );
extern void DumpOneCommandSyntax ( int n, int lp );
extern void DumpOneFunctionSyntax ( int n, int lp );
extern void DumpOneOperatorSyntax ( int n, int lp );

extern FileType * find_file_by_name( char * FileName );
extern FileType * find_file_by_number( int FileNumber );
extern FileType * file_new( void );
extern void file_clear( FileType * F );
extern int file_next_number( void );

extern LineType * find_line_number( int number, int IsCache );
extern void FixDescription ( char * Buffer );
extern void fnc_add_deffn ( LookupType * F, char TypeChar );
extern VariableType * fnc_deffn ( int argc, VariableType * argv, int unique_id );
extern FunctionType * fnc_find ( char *name );
extern FunctionType * fnc_find_by_id ( int unique_id );
extern FunctionType * fnc_find_exact ( char *name, int ParameterCount, unsigned long ParameterTypes );
extern int fnc_init ( void );
extern VariableType * fnc_intrinsic ( int argc, VariableType * argv, int unique_id );
extern void FREE ( void *ptr, char *str );
extern int fslt_add ( LineType * l );
extern int fslt_addlocalvar ( LookupType * f, VariableType * v );
extern int fslt_init ( void );
extern void FunctionDefinitionCheck ( FunctionType * f );
extern void FunctionSyntax ( FunctionType * f, char *Syntax );
extern void FunctionUniqueID ( FunctionType * f, char *UniqueID );
extern char * GetKeyword ( LineType * l, char *OutputBuffer, char *Keyword );
extern int GetOnError ( void );
#if FALSE
extern char * GetRestOfLine ( LineType * l, char *OutputBuffer );
#endif /* FALSE */
extern int inp_adv ( char *b, int *c );
extern BasicNumberType inp_numconst ( char *expression, int *ResultCode );
extern int InvertMatrix ( VariableType * vOut, VariableType * vIn );
extern int IsLastKeyword ( LineType * l, char *Keyword );
extern int IsLike ( char * buffer, int * buffer_count, int buffer_Length, char * pattern, int * pattern_count, int pattern_Length );
extern int is_cmd ( char *buffer );
extern int is_empty_filename( char * Buffer );
#if FALSE
extern int is_eol ( char *buffer, int *position );
#endif /* FALSE */
extern int is_let ( char *buffer );
extern int is_ln ( char *buffer );
extern int is_numconst ( char *buffer );
extern int line_is_eol ( LineType * line );
extern int line_peek_array_dimensions ( LineType * line, int *n_params );
extern int line_peek_char ( LineType * line, char find );
extern int line_peek_word ( LineType * line, char * find );
extern int line_read_array_dimensions ( LineType * line, int *n_params, int params[ ] );
extern int line_read_element ( LineType * line, char * element );
extern int line_read_expression ( LineType * line, VariantType * X );
extern int line_read_index_item ( LineType * line, int Index, int * Value );
extern int line_read_integer_expression ( LineType * line, int * Value );
extern int line_read_label ( LineType * line, char * label );
extern int line_read_letter_sequence ( LineType * line, char *start, char *end );
extern int line_read_line_number ( LineType * line, int * linenum );
extern int line_read_line_sequence ( LineType * line, int * head, int * tail );
extern VariableType * line_read_matrix ( LineType * line );
extern int line_read_numeric_expression ( LineType * line, BasicNumberType * Value );
extern VariableType * line_read_scalar ( LineType * line );
extern int line_read_string_expression ( LineType * line, char ** Value );
extern char line_read_type_declaration ( LineType * line );
extern int line_read_varname ( LineType * line, char * varname );
extern int line_skip_char ( LineType * line, char find );
extern int line_skip_comma ( LineType * line );
extern int line_skip_eol ( LineType * line );
extern int line_skip_spaces ( LineType * line );
extern int line_skip_word ( LineType * line, char * find );
extern int line_start ( LineType * l );
extern int ln_asbuf ( LineType * l, char *s );
extern VariableType * mat_find ( char *name );
extern int NumberValueCheck ( unsigned long ParameterTests, BasicNumberType X );
extern void OptionVersionSet ( int i );
extern int prn_iprintf ( char *buffer );
extern int prn_lprintf ( char *buffer );
extern int prn_xprintf ( char *buffer );
extern void ResetConsoleColumn ( void );
extern void SetCurrentFile( FileType * F );
extern void SetOnError ( int LineNumber );
extern void SortAllCommands ( void );
extern void SortAllFunctions ( void );
extern void SortAllOperators ( void );
extern int StringLengthCheck ( unsigned long ParameterTests, int s );
extern int str_btob ( StringType * d, StringType * s );
extern int str_btoc ( char *buffer, StringType * s );
extern StringType * str_cat ( StringType * a, StringType * b );
extern int str_cmp ( StringType * a, StringType * b );
extern int str_ctob ( StringType * s, char *buffer );
extern int str_like ( StringType * a, StringType * b );
extern int str_match( char * A, int A_Length, char *B, int B_Length, int I_Start );
extern int VarTypeIndex ( char C );
extern VariableType * var_chain ( VariableType * argv );
extern void var_CLEAR ( void );
extern int var_delcvars ( void );
extern VariableType * var_find ( char *name, int dimensions, int IsImplicit );
extern VariableType * var_free ( VariableType * v );
extern int var_init ( void );
extern int var_make ( VariableType * v, char type );
extern char var_nametype ( char * name );
extern VariableType * var_new ( char *name, char TypeChar );
extern BasicNumberType VerifyNumeric ( BasicNumberType Value );

extern void field_close_file( FileType * File );
extern void field_free_variable( VariableType * Var );
extern void field_get( FileType * File ); 
extern void field_put( FileType * File );
extern void strupper(char *C);
extern void strlower(char *C);
extern char bwb_file_open( char A, int x, char * B, int y );


#if NEW_VIRTUAL
extern void clear_virtual_by_file( int FileNumber );
#endif /* NEW_VIRTUAL */


int var_get( VariableType * variable, VariantType * variant );
int var_set( VariableType * variable, VariantType * variant );

#define WARN_NEXT_WITHOUT_FOR                bwb_Warning(  1, NULL )
#define WARN_SYNTAX_ERROR                    bwb_Warning(  2, NULL )
#define WARN_RETURN_WITHOUT_GOSUB            bwb_Warning(  3, NULL )
#define WARN_OUT_OF_DATA                     bwb_Warning(  4, NULL )
#define WARN_ILLEGAL_FUNCTION_CALL           bwb_Warning(  5, NULL )
#define WARN_OVERFLOW                        bwb_Warning(  6, NULL )
#define WARN_OUT_OF_MEMORY                   bwb_Warning(  7, NULL )
#define WARN_UNDEFINED_LINE                  bwb_Warning(  8, NULL )
#define WARN_SUBSCRIPT_OUT_OF_RANGE          bwb_Warning(  9, NULL )
#define WARN_REDIMENSION_ARRAY               bwb_Warning( 10, NULL )
#define WARN_DIVISION_BY_ZERO                bwb_Warning( 11, NULL )
#define WARN_ILLEGAL_DIRECT                  bwb_Warning( 12, NULL )
#define WARN_TYPE_MISMATCH                   bwb_Warning( 13, NULL )
#define WARN_OUT_OF_STRING_SPACE             bwb_Warning( 14, NULL )
#define WARN_STRING_TOO_LONG                 bwb_Warning( 15, NULL )
#define WARN_STRING_FORMULA_TOO_COMPLEX      bwb_Warning( 16, NULL )
#define WARN_CANT_CONTINUE                   bwb_Warning( 17, NULL )
#define WARN_UNDEFINED_USER_FUNCTION         bwb_Warning( 18, NULL )
#define WARN_NO_RESUME                       bwb_Warning( 19, NULL )
#define WARN_RESUME_WITHOUT_ERROR            bwb_Warning( 20, NULL )
#define WARN_UNPRINTABLE_ERROR               bwb_Warning( 21, NULL )
#define WARN_MISSING_OPERAND                 bwb_Warning( 22, NULL )
#define WARN_LINE_BUFFER_OVERFLOW            bwb_Warning( 23, NULL )
#define WARN_UNASSIGNED_24                   bwb_Warning( 24, NULL )
#define WARN_UNASSIGNED_25                   bwb_Warning( 25, NULL )
#define WARN_FOR_WITHOUT_NEXT                bwb_Warning( 26, NULL )
#define WARN_UNASSIGNED_27                   bwb_Warning( 27, NULL )
#define WARN_UNASSIGNED_28                   bwb_Warning( 28, NULL )
#define WARN_WHILE_WITHOUT_WEND              bwb_Warning( 29, NULL )
#define WARN_WEND_WITHOUT_WHILE              bwb_Warning( 30, NULL )
#define WARN_EXIT_FUNCTION_WITHOUT_FUNCTION  bwb_Warning( 31, NULL )
#define WARN_END_FUNCTION_WITHOUT_FUNCTION   bwb_Warning( 32, NULL )
#define WARN_EXIT_SUB_WITHOUT_SUB            bwb_Warning( 33, NULL )
#define WARN_END_SUB_WITHOUT_SUB             bwb_Warning( 34, NULL )
#define WARN_EXIT_FOR_WITHOUT_FOR            bwb_Warning( 35, NULL )
#define WARN_FIELD_OVERFLOW                  bwb_Warning( 50, NULL )
#define WARN_INTERNAL_ERROR                  bwb_Warning( 51, NULL )
#define WARN_BAD_FILE_NUMBER                 bwb_Warning( 52, NULL )
#define WARN_FILE_NOT_FOUND                  bwb_Warning( 53, NULL )
#define WARN_BAD_FILE_MODE                   bwb_Warning( 54, NULL )
#define WARN_FILE_ALREADY_OPEN               bwb_Warning( 55, NULL )
#define WARN_UNASSIGNED_56                   bwb_Warning( 56, NULL )
#define WARN_DISK_IO_ERROR                   bwb_Warning( 57, NULL )
#define WARN_FILE_ALREADY_EXISTS             bwb_Warning( 58, NULL )
#define WARN_UNASSIGNED_59                   bwb_Warning( 59, NULL )
#define WARN_UNASSIGNED_60                   bwb_Warning( 60, NULL )
#define WARN_DISK_FULL                       bwb_Warning( 61, NULL )
#define WARN_INPUT_PAST_END                  bwb_Warning( 62, NULL )
#define WARN_BAD_RECORD_NUMBER               bwb_Warning( 63, NULL )
#define WARN_BAD_FILE_NAME                   bwb_Warning( 64, NULL )
#define WARN_UNASSIGNED_65                   bwb_Warning( 65, NULL )
#define WARN_DIRECT_STATEMENT_IN_FILE        bwb_Warning( 66, NULL )
#define WARN_TOO_MANY_FILES                  bwb_Warning( 67, NULL )
#define WARN_UNASSIGNED_68                   bwb_Warning( 68, NULL )
#define WARN_VARIABLE_NOT_DECLARED           bwb_Warning( 70, NULL )
#define WARN_ADVANCED_FEATURE                bwb_Warning( 73, NULL )





/***************************************************************
  
   bwbasic.h   Part V: Other Prototypes
  
***************************************************************/
#if HAVE_UNIX_GCC
/* sleep() is an intrinsic C functions in my environment using -ansi */
#else          /* ! HAVE_UNIX_GCC */
extern unsigned int sleep( unsigned int X );
#endif            /* ! HAVE_UNIX_GCC */

/* EOF */
