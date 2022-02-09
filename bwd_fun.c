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
/*---------------------------------------------------------------*/



/* FUNCTION TABLE */

#include "bwbasic.h"

struct bwb_function bwb_prefuncs[NUM_FUNCTIONS] =
{
 {
  F_ABS_X_N,         /* UniqueID */
  "N  = ABS( X )",      /* Syntax */
  "The absolute value of X.", /* Description */
  "ABS",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_ACOS_X_N,        /* UniqueID */
  "N  = ACOS( X )",     /* Syntax */
  "The arccosine of X in radians, where 0 <= ACOS(X) <= PI.  X s"
  "hall be in the range -1 <= X <= 1.",   /* Description */
  "ACOS",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_ANGLE_X_Y_N,     /* UniqueID */
  "N  = ANGLE( X, Y )",    /* Syntax */
  "The angle in radians between the positive x-axis and the vect"
  "or joining the origin to the point with coordinates (X, Y), w"
  "here -PI < ANGLE(X,Y) <= PI.  X and Y must not both be 0.  No"
  "te that the counterclockwise is positive, e.g., ANGLE(1,1) = "
  "45 degrees.",     /* Description */
  "ANGLE",        /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1ANY | P2ANY,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_ARGC_N,       /* UniqueID */
  "N  = ARGC",       /* Syntax */
  "The number of parameters passed to a FUNCTION or SUB.  If not"
  " in a FUNCTION or SUB, returns -1.",   /* Description */
  "ARGC",         /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_ARGT_X_S,        /* UniqueID */
  "S$ = ARGT$( X )",    /* Syntax */
  "The value of the X'th parameter to a FUNCTION or SUB; '$' or "
  "'#'.  X in [1,ARGC].",  /* Description */
  "ARGT$",        /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1INT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_ARGV_X_N,        /* UniqueID */
  "N  = ARGV( X )",     /* Syntax */
  "The value of the X'th parameter to a FUNCTION or SUB.  X in ["
  "1,ARGC] and ARGT$( X ) = '#'.",  /* Description */
  "ARGV",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1INT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_ARGV_X_S,        /* UniqueID */
  "S$ = ARGV$( X )",    /* Syntax */
  "The value of the X'th parameter to a FUNCTION or SUB.  X in ["
  "1,ARGC] and ARGT$( X ) = '$'.",  /* Description */
  "ARGV$",        /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1INT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_ASC_A_N,         /* UniqueID */
  "N  = ASC( A$ )",     /* Syntax */
  "The ASCII code for the first letter in A$.", /* Description */
  "ASC",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_ASIN_X_N,        /* UniqueID */
  "N  = ASIN( X )",     /* Syntax */
  "The arcsine of X in radians, where -PI/2 <= ASIN(X) <= PI/2; "
  "X shall be in the range -1 <= X <= 1.",   /* Description */
  "ASIN",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_ATN_X_N,         /* UniqueID */
  "N  = ATN( X )",      /* Syntax */
  "The arctangent of X in radians, i.e. the angle whose tangent "
  "is X, where -PI/2 < ATN(X) < PI/2.",   /* Description */
  "ATN",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CCUR_X_N,        /* UniqueID */
  "N  = CCUR( X )",     /* Syntax */
  "The currency (64-bit) integer value of X.",  /* Description */
  "CCUR",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1CUR,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_CDBL_X_N,        /* UniqueID */
  "N  = CDBL( X )",     /* Syntax */
  "The double-precision value of X.",  /* Description */
  "CDBL",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DBL,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CEIL_X_N,        /* UniqueID */
  "N  = CEIL( X )",     /* Syntax */
  "The smallest integer not less than X.",   /* Description */
  "CEIL",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_CHDIR_A_N,       /* UniqueID */
  "N  = CHDIR( A$ )",      /* Syntax */
  "Changes to the directory named to A$.",   /* Description */
  "CHDIR",        /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CHR_X_S,         /* UniqueID */
  "S$ = CHR$( X )",     /* Syntax */
  "The one-character string with the character corresponding to "
  "the ASCII code X.",     /* Description */
  "CHR$",         /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CINT_X_N,        /* UniqueID */
  "N  = CINT( X )",     /* Syntax */
  "The short (16-bit) integer value of X.",  /* Description */
  "CINT",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1INT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CLNG_X_N,        /* UniqueID */
  "N  = CLNG( X )",     /* Syntax */
  "The long (32-bit) integer value of X.",   /* Description */
  "CLNG",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1LNG,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CLOSE_X_N,       /* UniqueID */
  "N  = CLOSE( X )",    /* Syntax */
  "Close file number X.",  /* Description */
  "CLOSE",        /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DEV,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CLS_N,        /* UniqueID */
  "N  = CLS",        /* Syntax */
  "Clears the screen. Cursor is positioned at row 1, column 1.",  /* Description */
  "CLS",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_COLOR_X_Y_N,     /* UniqueID */
  "N  = COLOR( X, Y )",    /* Syntax */
  "Sets the foreground text color to X, and the background text "
  "color to Y.",     /* Description */
  "COLOR",        /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1BYT | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_COS_X_N,         /* UniqueID */
  "N  = COS( X )",      /* Syntax */
  "The cosine of X, where X is in radians.", /* Description */
  "COS",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_COSH_X_N,        /* UniqueID */
  "N  = COSH( X )",     /* Syntax */
  "The hyperbolic cosine of X.",/* Description */
  "COSH",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_COT_X_N,         /* UniqueID */
  "N  = COT( X )",      /* Syntax */
  "The cotangent of X, where X is in radians.", /* Description */
  "COT",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_CSC_X_N,         /* UniqueID */
  "N  = CSC( X )",      /* Syntax */
  "The cosecant of X, where X is in radians.",  /* Description */
  "CSC",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_CSNG_X_N,        /* UniqueID */
  "N  = CSNG( X )",     /* Syntax */
  "The single-precision value of X.",  /* Description */
  "CSNG",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1FLT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CVC_A_N,         /* UniqueID */
  "N  = CVC( A$ )",     /* Syntax */
  "The currency (64-bit) integer value in A$, which was created "
  "by MKC$.",        /* Description */
  "CVC",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1CUR,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_CVD_A_N,         /* UniqueID */
  "N  = CVD( A$ )",     /* Syntax */
  "The double-precision value in A$, which was created by MKD$.", /* Description */
  "CVD",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1DBL,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CVI_A_N,         /* UniqueID */
  "N  = CVI( A$ )",     /* Syntax */
  "The short (16-bit) integer value in A$, which was created by "
  "MKI$.",        /* Description */
  "CVI",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1INT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CVL_A_N,         /* UniqueID */
  "N  = CVL( A$ )",     /* Syntax */
  "The long (32-bit) integer value in A$, which was created by M"
  "KL$.",         /* Description */
  "CVL",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1LNG,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_CVS_A_N,         /* UniqueID */
  "N  = CVS( A$ )",     /* Syntax */
  "The single-precision value in A$, which was created by MKS$.", /* Description */
  "CVS",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1FLT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_DATE_N,       /* UniqueID */
  "N  = DATE",       /* Syntax */
  "The current date in decimal form YYYDDD, where YYY are the nu"
  "mber of years since 1900 and DDD is the ordinal number of the"
  " current day of the year; e.g., the value of DATE on May 9, 1"
  "977 was 77129.  If there is no calendar available, then the v"
  "alue of DATE shall be -1.",   /* Description */
  "DATE",         /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_DATE_S,       /* UniqueID */
  "S$ = DATE$",         /* Syntax */
  "BASIC-80: The current date based on the computer's internal c"
  "lock as a string in the form 'MM-DD-YYYY'.ECMA-116: The date "
  "in the string representation 'YYYYMMDD' according to ISO 2014"
  ".  For example, the value of DATE$ on May 9, 1977 was '197705"
  "09'.  If there is not calendar available, then the value of D"
  "ATE$ shall be '00000000'.",   /* Description */
  "DATE$",        /* Name */
  STRING,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_DEF_FN_N,        /* UniqueID */
  "N  = DEF FN",     /* Syntax */
  "User Function.",     /* Description */
  "DEF FN",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_DEG_X_N,         /* UniqueID */
  "N  = DEG( X )",      /* Syntax */
  "The number of degrees in X radians.",  /* Description */
  "DEG",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_ENVIRON_A_N,     /* UniqueID */
  "N  = ENVIRON( A$ )",    /* Syntax */
  "Sends the environment variable expression contained in A$ to "
  "the host operating system.  A$ must contain the  '=' characte"
  "r.",           /* Description */
  "ENVIRON",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_ENVIRON_A_S,     /* UniqueID */
  "S$ = ENVIRON$( A$ )",   /* Syntax */
  "The value of the environment variable named A$.",  /* Description */
  "ENVIRON$",        /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_EOF_X_N,         /* UniqueID */
  "N  = EOF( X )",      /* Syntax */
  "If device number X is at the end-of-file, then -1, otherwise "
  "0.",           /* Description */
  "EOF",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DEV,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_EPS_X_N,         /* UniqueID */
  "N  = EPS( X )",      /* Syntax */
  "The maximum of (X-X1,X2-X, sigma) where X1 and X2 are the pre"
  "decessor and successor of X and signma is the smallest positi"
  "ve value representable.  If X has no predecessor the X1=X and"
  " if X has no successor the X2=X.  Note EPS(0) is the smallest"
  " positive number representable by the implementation, and is "
  "therefor implementation-defined.  Note also that EPS may prod"
  "uce different results for different arithmetic options (see O"
  "PTION ARITHMETIC).",    /* Description */
  "EPS",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_ERL_N,        /* UniqueID */
  "N  = ERL",        /* Syntax */
  "The line number of the most recent error.",  /* Description */
  "ERL",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_ERR_N,        /* UniqueID */
  "N  = ERR",        /* Syntax */
  "The error number of the most recent error.", /* Description */
  "ERR",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_ERR_S,        /* UniqueID */
  "S$ = ERR$",       /* Syntax */
  "The last error message.",  /* Description */
  "ERR$",         /* Name */
  STRING,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_ERROR_X_N,       /* UniqueID */
  "N  = ERROR( X )",    /* Syntax */
  "Simulate the error number in X.",   /* Description */
  "ERROR",        /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_ERROR_X_A_N,     /* UniqueID */
  "N  = ERROR( X, A$ )",   /* Syntax */
  "Simulate the error number in X, with a custom message in A$.", /* Description */
  "ERROR",        /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2STR,     /* ParameterTypes */
  P1BYT | P2ANY,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_EXP_X_N,         /* UniqueID */
  "N  = EXP( X )",      /* Syntax */
  "The exponential value of X, i.e., the value of the base of na"
  "tural logarithms (e = 2.71828) raised to the power of X;  if "
  "EXP(X) is less that machine infinitesimal, then its value sha"
  "ll be replaced with zero.",   /* Description */
  "EXP",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_FILEATTR_X_Y_N,     /* UniqueID */
  "N  = FILEATTR( X, Y )", /* Syntax */
  "For file X, if Y = 1 then returns open mode, otherwise return"
  "s zero.",         /* Description */
  "FILEATTR",        /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1DEV | P2INT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_FILES_N,         /* UniqueID */
  "N  = FILES",         /* Syntax */
  "Displays all the file names.",   /* Description */
  "FILES",        /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_FILES_A_N,       /* UniqueID */
  "N  = FILES( A$ )",      /* Syntax */
  "Displays all the file names matching A$.",   /* Description */
  "FILES",        /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_FIX_X_N,         /* UniqueID */
  "N  = FIX( X )",      /* Syntax */
  "The truncated integer, part of X. FIX (X) is equivalent to SG"
  "N(X)*INT(ABS(X)). The major difference between FIX and INT is"
  " that FIX does not return the next lower number for negative "
  "X.",           /* Description */
  "FIX",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_FP_X_N,       /* UniqueID */
  "N  = FP( X )",    /* Syntax */
  "The fractional part of X, i.e. X - IP(X).",  /* Description */
  "FP",           /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_FRE_N,        /* UniqueID */
  "N  = FRE",        /* Syntax */
  "The number of bytes of available memory.  This function is pr"
  "ovided for backward compatibility and it always returns a fix"
  "ed value of 32000.",    /* Description */
  "FRE",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_FRE_X_N,         /* UniqueID */
  "N  = FRE( X )",      /* Syntax */
  "The number of bytes of available memory.  This function is pr"
  "ovided for backward compatibility and it always returns a fix"
  "ed value of 32000.  The value of X is ignored.",   /* Description */
  "FRE",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_FRE_A_N,         /* UniqueID */
  "N  = FRE( A$ )",     /* Syntax */
  "The number of bytes of available memory.  This function is pr"
  "ovided for backward compatibility and it always returns a fix"
  "ed value of 32000.The value of A$ is ignored.", /* Description */
  "FRE",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_FREEFILE_N,         /* UniqueID */
  "N  = FREEFILE",      /* Syntax */
  "The next available file number.",   /* Description */
  "FREEFILE",        /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_GET_X_N,         /* UniqueID */
  "N  = GET( X )",      /* Syntax */
  "For file X, gets the next available record.",   /* Description */
  "GET",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DEV,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_GET_X_Y_N,       /* UniqueID */
  "N  = GET( X, Y )",      /* Syntax */
  "For file X, gets record number Y.  The first record number is"
  " 1.",       /* Description */
  "GET",       /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1DEV | P2INT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_HEX_X_S,         /* UniqueID */
  "S$ = HEX$( X )",     /* Syntax */
  "The the hexadecimal (base 16) representation of X.",  /* Description */
  "HEX$",         /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1INT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_INKEY_S,         /* UniqueID */
  "S$ = INKEY$",     /* Syntax */
  "The keypress, if available. If a keypress is not available, t"
  "hen immediately returns a null string ('').  If not supported"
  " by the platform, then always returns an empty string, so use"
  " INPUT$(1) instead.",   /* Description */
  "INKEY$",       /* Name */
  STRING,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_INP_X_N,         /* UniqueID */
  "N  = INP( X )",      /* Syntax */
  "The value read from machine port X.  Not supported on all pla"
  "tforms.",         /* Description */
  "INP",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_INPUT_X_S,       /* UniqueID */
  "S$ = INPUT$( X )",      /* Syntax */
  "The string of X characters, read from the terminal.", /* Description */
  "INPUT$",       /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1LEN,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_INPUT_X_Y_S,     /* UniqueID */
  "S$ = INPUT$( X, Y )",   /* Syntax */
  "The string of X characters, read from file Y.", /* Description */
  "INPUT$",       /* Name */
  STRING,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1LEN | P2DEV,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_INSTR_A_B_N,     /* UniqueID */
  "N  = INSTR( A$, B$ )",  /* Syntax */
  "The position at which B$ occurs in A$, beginning at position "
  "1.",           /* Description */
  "INSTR",        /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1STR | P2STR,     /* ParameterTypes */
  P1ANY | P2ANY,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_INSTR_X_A_B_N,      /* UniqueID */
  "N  = INSTR( X, A$, B$ )",  /* Syntax */
  "The position at which B$ occurs in A$, beginning at position "
  "X.",           /* Description */
  "INSTR",        /* Name */
  NUMBER,         /* ReturnType */
  3,           /* ParameterCount */
  P1NUM | P2STR | P3STR,   /* ParameterTypes */
  P1POS | P2ANY | P3ANY,   /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_INT_X_N,         /* UniqueID */
  "N  = INT( X )",      /* Syntax */
  "The largest integer not greater than X; e.g. INT(1.3) = 1 and"
  " INT(-1.3) = 2.",    /* Description */
  "INT",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_IP_X_N,       /* UniqueID */
  "N  = IP( X )",    /* Syntax */
  "The integer part of X, i.e., SGN(X)*INT(ABS(X)).", /* Description */
  "IP",           /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_KILL_A_N,        /* UniqueID */
  "N  = KILL( A$ )",    /* Syntax */
  "Removes the file named in A$.",  /* Description */
  "KILL",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LCASE_A_S,       /* UniqueID */
  "S$ = LCASE$( A$ )",     /* Syntax */
  "The string of characters from the value associatedwith A$ by "
  "replacing each upper-case-letter in the string by its lower-c"
  "ase version.",    /* Description */
  "LCASE$",       /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LEFT_A_X_S,         /* UniqueID */
  "S$ = LEFT$( A$, X )",   /* Syntax */
  "The X left-most characters of A$, beginning from postion 1.",  /* Description */
  "LEFT$",        /* Name */
  STRING,         /* ReturnType */
  2,           /* ParameterCount */
  P1STR | P2NUM,     /* ParameterTypes */
  P1ANY | P2LEN,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LEN_A_N,         /* UniqueID */
  "N  = LEN( A$ )",     /* Syntax */
  "The length of A$.",     /* Description */
  "LEN",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LOC_X_N,         /* UniqueID */
  "N  = LOC( X )",      /* Syntax */
  "The location of file X; the next record that GET or PUT state"
  "ments will use.",    /* Description */
  "LOC",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DEV,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LOCATE_X_Y_N,    /* UniqueID */
  "N  = LOCATE( X, Y )",   /* Syntax */
  "Locates the cursor to row X, column Y.",  /* Description */
  "LOCATE",       /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1BYT | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LOF_X_N,         /* UniqueID */
  "N  = LOF( X )",      /* Syntax */
  "The length of file X.", /* Description */
  "LOF",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DEV,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LOG_X_N,         /* UniqueID */
  "N  = LOG( X )",      /* Syntax */
  "The natural logarithm of X; X shall be greater than zero.", /* Description */
  "LOG",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1GTZ,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LOG10_X_N,       /* UniqueID */
  "N  = LOG10( X )",    /* Syntax */
  "The common logarithm of X; X shall be greater than zero.",  /* Description */
  "LOG10",        /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1GTZ,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_LOG2_X_N,        /* UniqueID */
  "N  = LOG2( X )",     /* Syntax */
  "The base 2 logarithm of X; X shall be greater than zero.",  /* Description */
  "LOG2",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1GTZ,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_LPOS_N,       /* UniqueID */
  "N  = LPOS",       /* Syntax */
  "The current cursor position in the line for the printer.",  /* Description */
  "LPOS",         /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LTRIM_A_S,       /* UniqueID */
  "S$ = LTRIM$( A$ )",     /* Syntax */
  "The string of characters resulting from the value associated "
  "with A$ by deleting all leading space characters.",   /* Description */
  "LTRIM$",       /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_LWIDTH_X_N,         /* UniqueID */
  "N  = LWIDTH( X )",      /* Syntax */
  "For printer, sets the line width to X. Zero means no wrapping"
  " will occur.",    /* Description */
  "LWIDTH",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_MAX_X_Y_N,       /* UniqueID */
  "N  = MAX( X, Y )",      /* Syntax */
  "The smaller (algebraically) of the parameters.",   /* Description */
  "MAX",       /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1ANY | P2ANY,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_MAXBYT_N,        /* UniqueID */
  "N  = MAXBYT",     /* Syntax */
  "The largest finite positive number representable as a BYTE; i"
  "mplementation-defined.",   /* Description */
  "MAXBYT",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MAXCUR_N,        /* UniqueID */
  "N  = MAXCUR",     /* Syntax */
  "The largest finite positive number representable as a CURRENC"
  "Y; implementation-defined.",  /* Description */
  "MAXCUR",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MAXDBL_N,        /* UniqueID */
  "N  = MAXDBL",     /* Syntax */
  "The largest finite positive number representable as a DOUBLE;"
  " implementation-defined.", /* Description */
  "MAXDBL",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MAXDEV_N,        /* UniqueID */
  "N  = MAXDEV",     /* Syntax */
  "The largest finite positive number useable as a FILE NUMBER; "
  "implementation-defined.",  /* Description */
  "MAXDEV",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MAXINT_N,        /* UniqueID */
  "N  = MAXINT",     /* Syntax */
  "The largest finite positive number representable as an INTEGE"
  "R; implementation-defined.",  /* Description */
  "MAXINT",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MAXLEN_A_N,         /* UniqueID */
  "N  = MAXLEN( A$ )",     /* Syntax */
  "The maximum length associated with the simple-string-variable"
  " A$.",         /* Description */
  "MAXLEN",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_MAXLNG_N,        /* UniqueID */
  "N  = MAXLNG",     /* Syntax */
  "The largest finite positive number representable as a LONG; i"
  "mplementation-defined.",   /* Description */
  "MAXLNG",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MAXLVL_N,        /* UniqueID */
  "N  = MAXLVL",     /* Syntax */
  "The maximum stack level; implementation-defined.", /* Description */
  "MAXLVL",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MAXNUM_N,        /* UniqueID */
  "N  = MAXNUM",     /* Syntax */
  "The largest finite positive number representable and manipula"
  "ble by the implementation; implementation-defined.  MAXNUM ma"
  "y represent diffent number for different arithmetic options ("
  "see OPTION ARITHMETIC).",  /* Description */
  "MAXNUM",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_MAXSNG_N,        /* UniqueID */
  "N  = MAXSNG",     /* Syntax */
  "The largest finite positive number representable as a SINGLE;"
  " implementation-defined.", /* Description */
  "MAXSNG",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MID_A_X_S,       /* UniqueID */
  "S$ = MID$( A$, X )",    /* Syntax */
  "The characters of A$, starting from postion X.",   /* Description */
  "MID$",         /* Name */
  STRING,         /* ReturnType */
  2,           /* ParameterCount */
  P1STR | P2NUM,     /* ParameterTypes */
  P1ANY | P2POS,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_MID_A_X_Y_S,     /* UniqueID */
  "S$ = MID$( A$, X, Y )", /* Syntax */
  "The Y characters of A$, starting from postion X.", /* Description */
  "MID$",         /* Name */
  STRING,         /* ReturnType */
  3,           /* ParameterCount */
  P1STR | P2NUM | P3NUM,   /* ParameterTypes */
  P1ANY | P2POS | P3LEN,   /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_MIN_X_Y_N,       /* UniqueID */
  "N  = MIN( X, Y )",      /* Syntax */
  "The smaller (algebraically) of the parameters.",   /* Description */
  "MIN",       /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1ANY | P2ANY,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_MINBYT_N,        /* UniqueID */
  "N  = MINBYT",     /* Syntax */
  "The largest finite negative number representable as a BYTE; i"
  "mplementation-defined.",   /* Description */
  "MINBYT",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MINCUR_N,        /* UniqueID */
  "N  = MINCUR",     /* Syntax */
  "The largest finite negative number representable as a CURRENC"
  "Y; implementation-defined.",  /* Description */
  "MINCUR",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MINDBL_N,        /* UniqueID */
  "N  = MINDBL",     /* Syntax */
  "The largest finite negative number representable as a DOUBLE;"
  " implementation-defined.", /* Description */
  "MINDBL",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MINDEV_N,        /* UniqueID */
  "N  = MINDEV",     /* Syntax */
  "The largest finite negative number useable as a FILE NUMBER; "
  "implementation-defined.",  /* Description */
  "MINDEV",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MININT_N,        /* UniqueID */
  "N  = MININT",     /* Syntax */
  "The largest finite negative number representable as an INTEGE"
  "R; implementation-defined.",  /* Description */
  "MININT",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MINLNG_N,        /* UniqueID */
  "N  = MINLNG",     /* Syntax */
  "The largest finite negative number representable as a LONG; i"
  "mplementation-defined.",   /* Description */
  "MINLNG",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MINNUM_N,        /* UniqueID */
  "N  = MINNUM",     /* Syntax */
  "The largest finite negative number representable and manipula"
  "ble by the implementation; implementation-defined.  MINNUM ma"
  "y represent diffent number for different arithmetic options ("
  "see OPTION ARITHMETIC).",  /* Description */
  "MINNUM",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MINSNG_N,        /* UniqueID */
  "N  = MINSNG",     /* Syntax */
  "The largest finite negative number representable as a SINGLE;"
  " implementation-defined.", /* Description */
  "MINSNG",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MKC_X_S,         /* UniqueID */
  "S$ = MKC$( X )",     /* Syntax */
  "The internal representation of the currency (64-bit) integer "
  "X as a string.",     /* Description */
  "MKC$",         /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1CUR,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14          /* OptionVersionBitmask */
  },
 {
  F_MKD_X_S,         /* UniqueID */
  "S$ = MKD$( X )",     /* Syntax */
  "The internal representation of X as a string.", /* Description */
  "MKD$",         /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DBL,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_MKDIR_A_N,       /* UniqueID */
  "N  = MKDIR( A$ )",      /* Syntax */
  "Makes the directory named in A$.",  /* Description */
  "MKDIR",        /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_MKI_X_S,         /* UniqueID */
  "S$ = MKI$( X )",     /* Syntax */
  "The internal representation of the short (16-bit) integer X a"
  "s a string.",     /* Description */
  "MKI$",         /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1INT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_MKL_X_S,         /* UniqueID */
  "S$ = MKL$( X )",     /* Syntax */
  "The internal representation of the long (32-bit) integer X as"
  " a string.",         /* Description */
  "MKL$",         /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1LNG,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_MKS_X_S,         /* UniqueID */
  "S$ = MKS$( X )",     /* Syntax */
  "The internal representation of X as a string.", /* Description */
  "MKS$",         /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1FLT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_MOD_X_Y_N,       /* UniqueID */
  "N  = MOD( X, Y )",      /* Syntax */
  "X modulo Y, i.e., X-Y*INT(X/Y).  Y shall not equal zero.",  /* Description */
  "MOD",       /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1ANY | P2NEZ,     /* ParameterTests */
  NULL,           /* NextPointer */
  E86          /* OptionVersionBitmask */
  },
 {
  F_NAME_A_B_N,         /* UniqueID */
  "N  = NAME( A$, B$ )",   /* Syntax */
  "Rename the file named A$ into B$.", /* Description */
  "NAME",         /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1STR | P2STR,     /* ParameterTypes */
  P1BYT | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_NULL_X_N,        /* UniqueID */
  "N  = NULL( X )",     /* Syntax */
  "Appends X null characters after each line printed by LPRINT o"
  "r LLIST.",        /* Description */
  "NULL",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_OCT_X_S,         /* UniqueID */
  "S$ = OCT$( X )",     /* Syntax */
  "The the octal (base 8) representation of X.",   /* Description */
  "OCT$",         /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1INT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_OPEN_A_X_B_N,    /* UniqueID */
  "N  = OPEN( A$, X, B$ )",   /* Syntax */
  "Open file number X.  A$ is the mode: I, O, A, R.  B$ is the f"
  "ile name.  Default the record length.",   /* Description */
  "OPEN",         /* Name */
  NUMBER,         /* ReturnType */
  3,           /* ParameterCount */
  P1STR | P2NUM | P3STR,   /* ParameterTypes */
  P1BYT | P2DEV | P3BYT,   /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_OPEN_A_X_B_Y_N,     /* UniqueID */
  "N  = OPEN( A$, X, B$, Y )",   /* Syntax */
  "Open file number X.  A$ is the mode: I, O, A, R.  B$ is the f"
  "ile name.  Y is the record length.",   /* Description */
  "OPEN",         /* Name */
  NUMBER,         /* ReturnType */
  4,           /* ParameterCount */
  P1STR | P2NUM | P3STR | P4NUM,/* ParameterTypes */
  P1BYT | P2DEV | P3BYT | P4INT,/* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_ORD_A_N,         /* UniqueID */
  "N  = ORD( A$ )",     /* Syntax */
  "The ordinal position of the character named by the string ass"
  "ociated with A$ in the collating sequence of ASCII character "
  "set, where the first member of the character set is in positi"
  "on zero.  The acceptable values for the standard character se"
  "t are shown in Table 1.",  /* Description */
  "ORD",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_OUT_X_Y_N,       /* UniqueID */
  "N  = OUT( X, Y )",      /* Syntax */
  "Sends Y to hardware port X.  Not supported on all platforms.", /* Description */
  "OUT",       /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1INT | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_PEEK_X_N,        /* UniqueID */
  "N  = PEEK( X )",     /* Syntax */
  "The value read from hardware address X.  Not supported on all"
  " platforms.",     /* Description */
  "PEEK",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1LNG,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_PI_N,         /* UniqueID */
  "N  = PI",         /* Syntax */
  "The constant 3.14159 which is the ratio of the circumference "
  "of a circle to its diameter.",   /* Description */
  "PI",           /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_POKE_X_Y_N,         /* UniqueID */
  "N  = POKE( X, Y )",     /* Syntax */
  "Sends Y to hardware address X.  Not supported on all platform"
  "s.",           /* Description */
  "POKE",         /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1LNG | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_POS_N,        /* UniqueID */
  "N  = POS",        /* Syntax */
  "The current cursor position in the line.",   /* Description */
  "POS",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_POS_X_N,         /* UniqueID */
  "N  = POS( X )",      /* Syntax */
  "The current cursor position in the line for file X.", /* Description */
  "POS",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DEV,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_POS_A_B_N,       /* UniqueID */
  "N  = POS( A$, B$ )",    /* Syntax */
  "The character position, within the value assocated with A$, o"
  "f the first character of the first occurence of the value ass"
  "ociated with B$, starting at the first character of A$.  If t"
  "here is not such occurence, then the value returned is zero.", /* Description */
  "POS",       /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1STR | P2STR,     /* ParameterTypes */
  P1ANY | P2ANY,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_POS_A_B_X_N,     /* UniqueID */
  "N  = POS( A$, B$, X )", /* Syntax */
  "The character position, within the value assocated with A$, o"
  "f the first character of the first occurence of the value ass"
  "ociated with B$, starting at the Xth character of A$.  If the"
  "re is not such occurence, then the value returned is zero.",   /* Description */
  "POS",       /* Name */
  NUMBER,         /* ReturnType */
  3,           /* ParameterCount */
  P1STR | P2STR | P3NUM,   /* ParameterTypes */
  P1ANY | P2ANY | P3POS,   /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_PUT_X_N,         /* UniqueID */
  "N  = PUT( X )",      /* Syntax */
  "For file X, puts the next available record.",   /* Description */
  "PUT",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DEV,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_PUT_X_Y_N,       /* UniqueID */
  "N  = PUT( X, Y )",      /* Syntax */
  "For file X, puts record number Y.  The first record number is"
  " 1.",       /* Description */
  "PUT",       /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1DEV | P2INT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_RAD_X_N,         /* UniqueID */
  "N  = RAD( X )",      /* Syntax */
  "The number of radians in X degrees.",  /* Description */
  "RAD",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_RANDOMIZE_N,     /* UniqueID */
  "N  = RANDOMIZE",     /* Syntax */
  "Seeds the pseudo-random number generator with TIME.", /* Description */
  "RANDOMIZE",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_RANDOMIZE_X_N,      /* UniqueID */
  "N  = RANDOMIZE( X )",   /* Syntax */
  "Seeds the pseudo-random number generator with X.", /* Description */
  "RANDOMIZE",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_REMAINDER_X_Y_N,    /* UniqueID */
  "N  = REMAINDER( X, Y )",   /* Syntax */
  "The remainder function, i.e., X-Y*IP(X/Y).  Y shall not equal"
  " zero.",       /* Description */
  "REMAINDER",       /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1ANY | P2NEZ,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_REPEAT_X_Y_S,    /* UniqueID */
  "S$ = REPEAT$( X, Y )",  /* Syntax */
  "The string consisting of X copies of CHR$(Y); X > 0.",   /* Description */
  "REPEAT$",         /* Name */
  STRING,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1LEN | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_REPEAT_X_A_S,    /* UniqueID */
  "S$ = REPEAT$( X, A$ )", /* Syntax */
  "The string consisting of X copies of A$; X > 0.",  /* Description */
  "REPEAT$",         /* Name */
  STRING,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2STR,     /* ParameterTypes */
  P1LEN | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_RESET_N,         /* UniqueID */
  "N  = RESET",         /* Syntax */
  "Close all open files.", /* Description */
  "RESET",        /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_RIGHT_A_X_S,     /* UniqueID */
  "S$ = RIGHT$( A$, X )",  /* Syntax */
  "The right-most X characters of A$.",   /* Description */
  "RIGHT$",       /* Name */
  STRING,         /* ReturnType */
  2,           /* ParameterCount */
  P1STR | P2NUM,     /* ParameterTypes */
  P1ANY | P2LEN,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_RMDIR_A_N,       /* UniqueID */
  "N  = RMDIR( A$ )",      /* Syntax */
  "Removes the directory named in A$.",   /* Description */
  "RMDIR",        /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_RND_N,        /* UniqueID */
  "N  = RND",        /* Syntax */
  "The next pseudo-random number in an implementation-defined se"
  "quence of pseudo-random numbers uniformly distributed in the "
  "range 0 <= RND < 1.",   /* Description */
  "RND",       /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_RND_X_N,         /* UniqueID */
  "N  = RND( X )",      /* Syntax */
  "Returns a pseudorandom number in the range [0,1].  The value "
  "of X is ignored.",      /* Description */
  "RND",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_ROUND_X_Y_N,     /* UniqueID */
  "N  = ROUND( X, Y )",    /* Syntax */
  "The value of X rounded to Y decimal digits to the right of th"
  "e decimal point (or -Y digits to the left if Y < 0); i.e., IN"
  "T(X*10^Y+.5)/10^Y.  Y must be in [-32,32].", /* Description */
  "ROUND",        /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1ANY | P2INT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_RTRIM_A_S,       /* UniqueID */
  "S$ = RTRIM$( A$ )",     /* Syntax */
  "The string of characters resulting from the value associated "
  "with A$ by deleting all trailing space characters.",  /* Description */
  "RTRIM$",       /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_SEC_X_N,         /* UniqueID */
  "N  = SEC( X )",      /* Syntax */
  "The secant of X, where X is in radians.", /* Description */
  "SEC",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_SEEK_X_N,        /* UniqueID */
  "N  = SEEK( X )",     /* Syntax */
  "The location of file X; the next record that GET or PUT state"
  "ments will use.",    /* Description */
  "SEEK",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1DEV,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_SEEK_X_Y_N,         /* UniqueID */
  "N  = SEEK( X, Y )",     /* Syntax */
  "For file X, move to record number Y; the first record number "
  "is 1.",        /* Description */
  "SEEK",         /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1DEV | P2INT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_SGN_X_N,         /* UniqueID */
  "N  = SGN( X )",      /* Syntax */
  "The sign of X: -1 if X < 0, 0 if X = 0, and +1 if X > 0.",  /* Description */
  "SGN",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_SHELL_A_N,       /* UniqueID */
  "N  = SHELL( A$ )",      /* Syntax */
  "The exit code resulting from the execution of an operating sy"
  "stem command.",      /* Description */
  "SHELL",        /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_SIN_X_N,         /* UniqueID */
  "N  = SIN( X )",      /* Syntax */
  "The sine of X, where X is in radians.",   /* Description */
  "SIN",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_SINH_X_N,        /* UniqueID */
  "N  = SINH( X )",     /* Syntax */
  "The hyperbolic sine of X.",   /* Description */
  "SINH",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_SPACE_X_S,       /* UniqueID */
  "S$ = SPACE$( X )",      /* Syntax */
  "The string of X blank spaces.",  /* Description */
  "SPACE$",       /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1LEN,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_SPC_X_S,         /* UniqueID */
  "S$ = SPC( X )",      /* Syntax */
  "The string of X blank spaces.  Only for use within the PRINT "
  "command.",        /* Description */
  "SPC",       /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_SQR_X_N,         /* UniqueID */
  "N  = SQR( X )",      /* Syntax */
  "The non-negative square root of X;  X shall be non-negative.", /* Description */
  "SQR",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1GEZ,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_STR_X_S,         /* UniqueID */
  "S$ = STR$( X )",     /* Syntax */
  "The string generated by the print-statement as the numeric-re"
  "presentation of the value associated with X.",  /* Description */
  "STR$",         /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_STRING_X_A_S,    /* UniqueID */
  "S$ = STRING$( X, A$ )", /* Syntax */
  "The string X bytes long consisting of the first character of "
  "A$.",       /* Description */
  "STRING$",         /* Name */
  STRING,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2STR,     /* ParameterTypes */
  P1LEN | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_STRING_X_Y_S,    /* UniqueID */
  "S$ = STRING$( X, Y )",  /* Syntax */
  "The string X bytes long consisting of CHR$(Y).",   /* Description */
  "STRING$",         /* Name */
  STRING,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1LEN | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_TAB_X_S,         /* UniqueID */
  "S$ = TAB( X )",      /* Syntax */
  "The string required to advance to column X.  Only for use wit"
  "hin the PRINT command.",   /* Description */
  "TAB",       /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_TAN_X_N,         /* UniqueID */
  "N  = TAN( X )",      /* Syntax */
  "The tangent of X, where X is in radians.",   /* Description */
  "TAN",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_TANH_X_N,        /* UniqueID */
  "N  = TANH( X )",     /* Syntax */
  "The hyperbolic tangent of X.",   /* Description */
  "TANH",         /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_TIME_N,       /* UniqueID */
  "N  = TIME",       /* Syntax */
  "The time elapsed since the previous midnight, expressed in se"
  "conds; e.g., the value of TIME at 11:15 AM is 40500.  If ther"
  "e is no clock available, then the value of TIME shall be -1. "
  " The value of TIME at midnight shall be zero (not 86400).", /* Description */
  "TIME",         /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_TIME_S,       /* UniqueID */
  "S$ = TIME$",         /* Syntax */
  "The time of day in 24-hour notation according to ISO 3307.  F"
  "or example, the value of TIME$ at 11:15 AM is '11:15:00'.  If"
  " there is no clock available, then the value of TIME$ shall b"
  "e '99:99:99'.  The value of TIME$ at midnight is '00:00:00'.", /* Description */
  "TIME$",        /* Name */
  STRING,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_TIMER_N,         /* UniqueID */
  "N  = TIMER",         /* Syntax */
  "The time in the system clock in seconds elapsed since midnigh"
  "t.",           /* Description */
  "TIMER",        /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_TRIM_A_S,        /* UniqueID */
  "S$ = TRIM$( A$ )",      /* Syntax */
  "The string resulting from removing both leading and trailing "
  "spaces from A$.",    /* Description */
  "TRIM$",        /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_TROFF_N,         /* UniqueID */
  "N  = TROFF",         /* Syntax */
  "Turn tracing OFF",      /* Description */
  "TROFF",        /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_TRON_N,       /* UniqueID */
  "N  = TRON",       /* Syntax */
  "Turn tracing ON",    /* Description */
  "TRON",         /* Name */
  NUMBER,         /* ReturnType */
  0,           /* ParameterCount */
  PNONE,       /* ParameterTypes */
  PNONE,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_TRUNCATE_X_Y_N,     /* UniqueID */
  "N  = TRUNCATE( X, Y )", /* Syntax */
  "The value of X truncated to Y decimal digits to the right of "
  "the decimal point (or -Y digits to the left if Y < 0); i.e., "
  "IP(X*10^Y)/10^Y.  Y in [-32,32].",  /* Description */
  "TRUNCATE",        /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1ANY | P2INT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  F_UCASE_A_S,       /* UniqueID */
  "S$ = UCASE$( A$ )",     /* Syntax */
  "The string of characters resulting from the value associated "
  "with A$ by replacing each lower-case-letter in the string by "
  "its upper-case version.",  /* Description */
  "UCASE$",       /* Name */
  STRING,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1ANY,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_VAL_A_N,         /* UniqueID */
  "N  = VAL( A$ )",     /* Syntax */
  "The value of the numeric-constant associated with A$, if the "
  "string associated with A$ is a numeric-constant.  Leading and"
  " trailing spaces in the string are ignored.  If the evaluatio"
  "n of the numeric-constant would result in a value which cause"
  "s an underflow, then the value returned shall be zero.  For e"
  "xample, VAL(' 123.5 ' ) = 123.5, VAL('2.E-99') could be zero,"
  " and VAL('MCMXVII') causes an exception.",   /* Description */
  "VAL",       /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1STR,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_WAIT_X_Y_N,         /* UniqueID */
  "N  = WAIT( X, Y )",     /* Syntax */
  "Waits for the value of (INP(X) AND Y) to become nonzero.  Not"
  " supported on all platforms.",   /* Description */
  "WAIT",         /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1INT | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_WAIT_X_Y_Z_N,    /* UniqueID */
  "N  = WAIT( X, Y, Z )",  /* Syntax */
  "Waits for the value of ((INP(X) XOR Z) AND Y) to become nonze"
  "ro.  Not supported on all platforms.", /* Description */
  "WAIT",         /* Name */
  NUMBER,         /* ReturnType */
  3,           /* ParameterCount */
  P1NUM | P2NUM | P3NUM,   /* ParameterTypes */
  P1INT | P2BYT | P3BYT,   /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_WIDTH_X_N,       /* UniqueID */
  "N  = WIDTH( X )",    /* Syntax */
  "For console, sets the line width to X. Zero means no wrapping"
  " will occur.",    /* Description */
  "WIDTH",        /* Name */
  NUMBER,         /* ReturnType */
  1,           /* ParameterCount */
  P1NUM,       /* ParameterTypes */
  P1BYT,       /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  F_WIDTH_X_Y_N,     /* UniqueID */
  "N  = WIDTH( X, Y )",    /* Syntax */
  "For file X, sets the line width to Y.  Zero means no wrapping"
  " will occur.",    /* Description */
  "WIDTH",        /* Name */
  NUMBER,         /* ReturnType */
  2,           /* ParameterCount */
  P1NUM | P2NUM,     /* ParameterTypes */
  P1DEV | P2BYT,     /* ParameterTests */
  NULL,           /* NextPointer */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
};



/* EOF */
