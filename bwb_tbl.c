/***************************************************************
  
   bwb_tbl.c            OPTION VERSION table
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



#include "bwbasic.h"

VersionType bwb_vertable[ NUM_VERSIONS ] =
{
   {
      "BYWATER",           /* Name */
      B15,                 /* OptionVersionBitmask */
      "Bywater BASIC",     /* Description */
      2015,                /* Year */
      "B15",               /* ID */
      '\'',                /* OptionCommentChar */
      ':',                 /* OptionStatementChar */
      '?',                 /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_LABELS_ON | OPTION_BUGS_ON | OPTION_TRACE_ON, /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%Y",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '\\'                 /* UsingLength */
   },
   {
      "DARTMOUTH",         /* Name */
      D64,                 /* OptionVersionBitmask */
      "Dartmouth DTSS BASIC",   /* Description */
      1964,                /* Year */
      "D64",               /* ID */
      '\0',                /* OptionCommentChar */
      '\0',                /* OptionStatementChar */
      '\0',                /* OptionPrintChar */
      ':',                 /* OptionImageChar */
      FALSE,               /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%y",          /* OptionDateFormat */
      "%H:%M",             /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '%'                  /* UsingLength */ 
   },
   {
      "MARK-I"   ,         /* Name */
      G65,                 /* OptionVersionBitmask */
      "GE 265 Mainframe BASIC",  /* Description */
      1965,                /* Year */
      "G65",               /* ID */
      '\0',                /* OptionCommentChar */
      '\0',                /* OptionStatementChar */
      '\0',                /* OptionPrintChar */
      ':',                 /* OptionImageChar */
      FALSE,               /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%y",          /* OptionDateFormat */
      "%H:%M",             /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '%'                  /* UsingLength */
   },
   {
      "MARK-II"  ,         /* Name */
      G67,                 /* OptionVersionBitmask */
      "GE 435 Mainframe BASIC",  /* Description */
      1967,                /* Year */
      "G67",               /* ID */
      '\'',                /* OptionCommentChar */
      '\0',                /* OptionStatementChar */
      '\0',                /* OptionPrintChar */
      ':',                 /* OptionImageChar */
      FALSE,               /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%y",          /* OptionDateFormat */
      "%H:%M",             /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '%'                  /* UsingLength */
   },
   {
      "SYSTEM-360",        /* Name */
      I70,                 /* OptionVersionBitmask */
      "IBM System/360 Mainframe BASIC", /* Description */
      1970,                /* Year */
      "I70",               /* ID */
      '\0',                /* OptionCommentChar */
      '\0',                /* OptionStatementChar */
      '\0',                /* OptionPrintChar */
      ':',                 /* OptionImageChar */
      FALSE,               /* OptionFlags */
      1,                   /* OptionBaseValue */
      "%Y/%m/%d",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      '\0',                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '!' ,                /* UsingExrad */
      '\0',                /* UsingDollar */
      '\0',                /* UsingFiller */
      '\0',                /* UsingLiteral */
      '\0',                /* UsingFirst */
      '\0',                /* UsingAll */
      '\0'                 /* UsingLength */
   },
   {
      "RSTS-11",           /* Name */
      D71,                 /* OptionVersionBitmask */
      "DEC RSTS-11 Minicomputer BASIC-PLUS",  /* Description */
      1971,                /* Year */
      "D71",               /* ID */
      '!',                 /* OptionCommentChar */
      ':' ,                /* OptionStatementChar */
      '?' ,                /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_BUGS_ON,      /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%d-%b-%y",          /* OptionDateFormat */
      "%I:%M %p",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '\\'                 /* UsingLength */
   },
   {
      "SYSTEM-370",        /* Name */
      I73,                 /* OptionVersionBitmask */
      "IBM System/370 Mainframe BASIC", /* Description */
      1973,                /* Year */
      "I73",               /* ID */
      '\0',                /* OptionCommentChar */
      '\0',                /* OptionStatementChar */
      '\0',                /* OptionPrintChar */
      ':',                 /* OptionImageChar */
      FALSE,               /* OptionFlags */
      1,                   /* OptionBaseValue */
      "%Y/%m/%d",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      '\0',                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '|' ,                /* UsingExrad */
      '\0',                /* UsingDollar */
      '\0',                /* UsingFiller */
      '\0',                /* UsingLiteral */
      '\0',                /* UsingFirst */
      '\0',                /* UsingAll */
      '\0'                 /* UsingLength */
   },
   {
      "CBASIC-II",         /* Name */
      C77,                 /* OptionVersionBitmask */
      "CBASIC-II for CP/M",  /* Description */
      1977,                /* Year */
      "C77",               /* ID */
      '\'',                /* OptionCommentChar */
      ':' ,                /* OptionStatementChar */
      '?' ,                /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_BUGS_ON,      /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%Y/%m/%d",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '\\',                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '/'                  /* UsingLength */
   },
   {
      "ECMA-55",           /* Name */
      E78,                 /* OptionVersionBitmask */
      "ANSI Minimal BASIC",/* Description */
      1978,                /* Year */
      "E78",               /* ID */
      '\0',                /* OptionCommentChar */
      '\0',                /* OptionStatementChar */
      '\0',                /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      FALSE,               /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%Y",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '%'                  /* UsingLength */
   },
   {
      "HANDBOOK1",         /* Name */
      HB1,                 /* OptionVersionBitmask */
      "The BASIC Handbook, 1st Edition", /* Description */
      1978,                /* Year */
      "HB1",               /* ID */
      '\'',                /* OptionCommentChar */
      ':',                 /* OptionStatementChar */
      '?',                 /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_BUGS_ON,      /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%Y",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '%'                  /* UsingLength */
   },
   {
      "HANDBOOK2",         /* Name */
      HB2,                 /* OptionVersionBitmask */
      "The BASIC Handbook, 2nd Edition", /* Description */
      1981,                /* Year */
      "HB2",               /* ID */
      '\'',                /* OptionCommentChar */
      ':',                 /* OptionStatementChar */
      '?',                 /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_BUGS_ON,      /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%Y",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '%'                  /* UsingLength */
   },
   {
      "TRS-80",            /* Name */
      T80,                 /* OptionVersionBitmask */
      "TRS-80 Model I/III/4 LBASIC", /* Description */
      1981,                /* Year */
      "T80",               /* ID */
      '\'',                /* OptionCommentChar */
      ':',                 /* OptionStatementChar */
      '?',                 /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_BUGS_ON,      /* OptionFlags */
      0,                   /* OptionBaseValue */
      "",                  /* OptionDateFormat */
      "%m/%d/%y %H:%M:%S", /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '%'                  /* UsingLength */
   },
   {
      "BASIC-80",          /* Name */
      M80,                 /* OptionVersionBitmask */
      "Microsoft BASIC-80 for Xenix",/* Description */
      1982,                /* Year */
      "M80",               /* ID */
      '\'',                /* OptionCommentChar */
      ':',                 /* OptionStatementChar */
      '?',                 /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_BUGS_ON,      /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%Y",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '\\'                 /* UsingLength */
   },
   {
      "GW-BASIC",          /* Name */
      M85,                 /* OptionVersionBitmask */
      "Microsoft GW-BASIC for PC-DOS",/* Description */
      1985,                /* Year */
      "M85",               /* ID */
      '\'',                /* OptionCommentChar */
      ':',                 /* OptionStatementChar */
      '?',                 /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_BUGS_ON,      /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%Y",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '\\'                 /* UsingLength */
   },
   {
      "ECMA-116",          /* Name */
      E86,                 /* OptionVersionBitmask */
      "ANSI Full BASIC",   /* Description */
      1986,                /* Year */
      "E86",               /* ID */
      '!',                 /* OptionCommentChar */
      '\0',                /* OptionStatementChar */
      '\0',                /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_LABELS_ON | OPTION_STRICT_ON, /* OptionFlags */
      1,                   /* OptionBaseValue */
      "%Y-%m-%d",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '%'                  /* UsingLength */
   },
   {
      "QBASIC",            /* Name */
      M88,                 /* OptionVersionBitmask */
      "Microsoft QBASIC for PC-DOS",  /* Description */
      1988,                /* Year */
      "M88",               /* ID */
      '\'',                /* OptionCommentChar */
      ':',                 /* OptionStatementChar */
      '?',                 /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_LABELS_ON | OPTION_BUGS_ON, /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%Y",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '\\'                 /* UsingLength */
   },
   {
      "VBX",               /* Name */
      VBX,                 /* OptionVersionBitmask */
      "Microsoft VBX for Windows",     /* Description */
      1990,                /* Year */
      "VBX",               /* ID */
      '\'',                /* OptionCommentChar */
      ':',                 /* OptionStatementChar */
      '?',                 /* OptionPrintChar */
      '\0',                /* OptionImageChar */
      OPTION_LABELS_ON | OPTION_BUGS_ON, /* OptionFlags */
      0,                   /* OptionBaseValue */
      "%m/%d/%Y",          /* OptionDateFormat */
      "%H:%M:%S",          /* OptionTimeFormat */
      '#' ,                /* UsingDigit */
      ',' ,                /* UsingComma */
      '.' ,                /* UsingPeriod */
      '+' ,                /* UsingPlus */
      '-' ,                /* UsingMinus */
      '^' ,                /* UsingExrad */
      '$' ,                /* UsingDollar */
      '*' ,                /* UsingFiller */
      '_' ,                /* UsingLiteral */
      '!' ,                /* UsingFirst */
      '&' ,                /* UsingAll */
      '\\'                 /* UsingLength */
   },
};

/* EOF */
