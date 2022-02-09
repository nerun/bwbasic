/***************************************************************
  
        bwd_cmd.c       Command Table
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

/* COMMAND TABLE */

#include "bwbasic.h"

struct bwb_command bwb_cmdtable[NUM_COMMANDS] =
{
 {
  C_QUEST,        /* UniqueID */
  "? expressions...",      /* Syntax */
  "This is just a shortcut for PRINT.",   /* Description */
  "?",            /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_CALL,         /* UniqueID */
  "CALL subroutine-name( [parameter [, ...] ] )",  /* Syntax */
  "Calls a subroutine that was defined by SUB and END SUB.",   /* Description */
  "CALL",         /* Name */
  B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_CASE,         /* UniqueID */
  "CASE constant",      /* Syntax */
  "Introduces an element of a SELECT CASE statement.",   /* Description */
  "CASE",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_CASE_ELSE,       /* UniqueID */
  "CASE ELSE",       /* Syntax */
  "Introduces a default SELECT CASE element.",  /* Description */
  "CASE ELSE",       /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_CASE_IF,         /* UniqueID */
  "CASE IF partial-expression",  /* Syntax */
  "Introduces a conditional SELECT CASE element.", /* Description */
  "CASE IF",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_CASE_IS,         /* UniqueID */
  "CASE IS",         /* Syntax */
  "Same as CASE IF.",      /* Description */
  "CASE IS",         /* Name */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  C_CHAIN,        /* UniqueID */
  "CHAIN filename$",    /* Syntax */
  "Load and execute another BASIC program, without clearing vari"
  "ables.",       /* Description */
  "CHAIN",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_CHANGE,       /* UniqueID */
  "CHANGE A$ TO X | CHANGE X TO A$",   /* Syntax */
  "Changes a string to a numeric array or a numeric array to a s"
  "tring.",       /* Description */
  "CHANGE",       /* Name */
  B14 | D64       /* OptionVersionBitmask */
  },
 {
  C_CLEAR,        /* UniqueID */
  "CLEAR",        /* Syntax */
  "Sets all numeric variables to 0, and all string variables to "
  "empty strings.",     /* Description */
  "CLEAR",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_CLOAD,        /* UniqueID */
  "CLOAD [file-name$]",    /* Syntax */
  "Loads an ASCII BASIC program into memory.",  /* Description */
  "CLOAD",        /* Name */
  B14 | M80 | T83 | T84 | M85 /* OptionVersionBitmask */
  },
 {
  C_CLOAD_,       /* UniqueID */
  "CLOAD* ArrayName",      /* Syntax */
  "Loads a numeric array from a file saved using CSAVE*.",  /* Description */
  "CLOAD*",       /* Name */
  B14 | M80 | T83 | T84 | M85 /* OptionVersionBitmask */
  },
 {
  C_CMDS,         /* UniqueID */
  "CMDS",         /* Syntax */
  "Prints a list of all implemented BASIC commands.", /* Description */
  "CMDS",         /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_COMMON,       /* UniqueID */
  "COMMON variable [, ...]",  /* Syntax */
  "Designates variables to be passed to a CHAINed program.",   /* Description */
  "COMMON",       /* Name */
  B14 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_CONT,         /* UniqueID */
  "CONT",         /* Syntax */
  "Continue a BASIC program after a STOP has been executed.  Pro"
  "gram resumes at the line after the STOP.",   /* Description */
  "CONT",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_CSAVE,        /* UniqueID */
  "CSAVE [file-name$]",    /* Syntax */
  "Saves the current program into the file file-name$ in ASCII f"
  "ormat.",       /* Description */
  "CSAVE",        /* Name */
  B14 | M80 | T83 | T84 | M85 /* OptionVersionBitmask */
  },
 {
  C_CSAVE_,       /* UniqueID */
  "CSAVE* ArrayName",      /* Syntax */
  "Saves a numeric array into a file.",   /* Description */
  "CSAVE*",       /* Name */
  B14 | M80 | T83 | T84 | M85 /* OptionVersionBitmask */
  },
 {
  C_DATA,         /* UniqueID */
  "DATA constant [, ...]",  /* Syntax */
  "Stores numeric and string constants to be accessed by READ.",  /* Description */
  "DATA",         /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DEF,       /* UniqueID */
  "DEF FNname(arg...)] = expression",  /* Syntax */
  "Defines a single line user-written function.  For multiple li"
  "ne functions use the FUNCTION command.",  /* Description */
  "DEF",       /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DEF_SUB,         /* UniqueID */
  "DEF SUB",         /* Syntax */
  "An internal command that allows the BASIC program to execute "
  "any function as though it were a command.  You do not directl"
  "y call this command.  If your program has a line  beginning w"
  "ith a functio name, then DEF SUB is used to execute the funct"
  "ion and ignore the result.",  /* Description */
  "DEF SUB",         /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DEFDBL,       /* UniqueID */
  "DEFDBL letter[-letter] [, ...]", /* Syntax */
  "Declares variables with single-letter names as numeric variab"
  "les.  TODO: enforce the rnage of [MINDBL,MAXDBL].",   /* Description */
  "DEFDBL",       /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DEFINT,       /* UniqueID */
  "DEFINT  letter[-letter] [, ...]",   /* Syntax */
  "Declares variables with single-letter names as numeric variab"
  "les.  TODO: enforce the rnage of [MININT,MAXINT].",   /* Description */
  "DEFINT",       /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DEFSNG,       /* UniqueID */
  "DEFSNG letter[-letter] [, ...]", /* Syntax */
  "Declares variables with single-letter names as numeric variab"
  "les.  TODO: enforce the rnage of [MINSNG,MAXSNG].",   /* Description */
  "DEFSNG",       /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DEFSTR,       /* UniqueID */
  "DEFSTR letter[-letter] [, ...]", /* Syntax */
  "Declares variables with single-letter names as string variabl"
  "es.  TODO: enforce the range of [MINLEN,MAXLEN].", /* Description */
  "DEFSTR",       /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DELETE,       /* UniqueID */
  "DELETE line [- line]",  /* Syntax */
  "Deletes program lines indicated by the argument(s).  All prog"
  "ram lines have a number, which is visible with the LIST comma"
  "nd.  If line numbers are not provided, they are assigned begi"
  "nning with 1.",      /* Description */
  "DELETE",       /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DIM,       /* UniqueID */
  "DIM variable( elements [, ...]) [, ...]", /* Syntax */
  "Specifies the dimensions of an array variables.",  /* Description */
  "DIM",       /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DO,           /* UniqueID */
  "DO",           /* Syntax */
  "Top of a DO - LOOP structure.  If the loop is not terminated "
  "by EXIT DO or LOOP UNTIL or LOOP WHILE, then it will loop for"
  "ever.",        /* Description */
  "DO",           /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DO_UNTIL,        /* UniqueID */
  "DO UNTIL expression",   /* Syntax */
  "Top of a DO - LOOP structure.  If the expression is zero, the"
  "n the loop is terminated.",   /* Description */
  "DO UNTIL",        /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_DO_WHILE,        /* UniqueID */
  "DO WHILE expression",   /* Syntax */
  "Top of a DO - LOOP structure.  If the expression is non-zero,"
  " then the loop is terminated.",  /* Description */
  "DO WHILE",        /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_EDIT,         /* UniqueID */
  "EDIT",         /* Syntax */
  "implementation defined.",  /* Description */
  "EDIT",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_ELSE,         /* UniqueID */
  "ELSE",         /* Syntax */
  "Introduces a default condition in a multi-line IF statement.", /* Description */
  "ELSE",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_ELSEIF,       /* UniqueID */
  "ELSEIF",       /* Syntax */
  "Introduces a secondary condition in a multi-line IF statement"
  ".",            /* Description */
  "ELSEIF",       /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_END,       /* UniqueID */
  "END",       /* Syntax */
  "Terminates program execution. If the BASIC program was execut"
  "ed from the operating system level, then control returns to t"
  "he operating system, oterwise control reuturns to the BASIC p"
  "rompt.",       /* Description */
  "END",       /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_END_FUNCTION,    /* UniqueID */
  "END FUNCTION",    /* Syntax */
  "Specifies the last line of a multi-line FUNCTION definition.", /* Description */
  "END FUNCTION",    /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_END_IF,       /* UniqueID */
  "END IF",       /* Syntax */
  "Specifies the last line of a multi-line IF definition.", /* Description */
  "END IF",       /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_END_SELECT,         /* UniqueID */
  "END SELECT",         /* Syntax */
  "Specifies the last line of a multi-line SELECT CASE definitio"
  "n.",           /* Description */
  "END SELECT",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_END_SUB,         /* UniqueID */
  "END SUB",         /* Syntax */
  "Specifies the last line of a multi-line SUB definition.",   /* Description */
  "END SUB",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_ERASE,        /* UniqueID */
  "ERASE variable [, ...]",   /* Syntax */
  "Eliminates arrayed variables from a program.",  /* Description */
  "ERASE",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_EXIT,         /* UniqueID */
  "EXIT",         /* Syntax */
  "EXIT by itself is a syntax error.  Use EXIT DO, EXIT FOR, EXI"
  "T FUNCTION, EXIT SUB, EXIT UNTIL, or EXIT WHILE.", /* Description */
  "EXIT",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_EXIT_DO,         /* UniqueID */
  "EXIT DO",         /* Syntax */
  "Immediately exits the inner-most DO-LOOP strucure.",  /* Description */
  "EXIT DO",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_EXIT_FOR,        /* UniqueID */
  "EXIT FOR",        /* Syntax */
  "Immediately exits the inner-most FOR-NEXT strucure.", /* Description */
  "EXIT FOR",        /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_EXIT_FUNCTION,      /* UniqueID */
  "EXIT FUNCTION",      /* Syntax */
  "Immediately exits the inner-most multi-line FUNCTION strucure"
  ".",            /* Description */
  "EXIT FUNCTION",      /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_EXIT_SUB,        /* UniqueID */
  "EXIT SUB",        /* Syntax */
  "Immediately exits the inner-most multi-line SUB strucure.", /* Description */
  "EXIT SUB",        /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_EXIT_UNTIL,         /* UniqueID */
  "EXIT UNTIL",         /* Syntax */
  "Immediately exits the inner-most UNTIL-UEND strucure.",  /* Description */
  "EXIT UNTIL",         /* Name */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  C_EXIT_WHILE,         /* UniqueID */
  "EXIT WHILE",         /* Syntax */
  "Immediately exits the inner-most WHILE-END strucure.",   /* Description */
  "EXIT WHILE",         /* Name */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  C_FIELD,        /* UniqueID */
  "FIELD [#] device-number, number AS string-variable$ [, ...]",  /* Syntax */
  "Assigns 'number' bytes in the buffer of random file 'device-n"
  "umber' to the variable 'string-variable$'.  GET will automati"
  "cally update the variable, and PUT will automatically use the"
  " variable.",         /* Description */
  "FIELD",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_FNCS,         /* UniqueID */
  "FNCS",         /* Syntax */
  "Prints a list of all pre-defined BASIC functions.",   /* Description */
  "FNCS",         /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_FOR,       /* UniqueID */
  "FOR variable = start TO finish [STEP increment]",  /* Syntax */
  "Top of a FOR - NEXT structure.  The loop will continue a fixe"
  "d number of times, which is determined by the values of start"
  ", finish, and increment.", /* Description */
  "FOR",       /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_FUNCTION,        /* UniqueID */
  "FUNCTION [ ( parameter [, ...  ] ) ]", /* Syntax */
  "Top line of a multi-line FUNCTION definition.  The variable n"
  "ames specified are local to the FUNCTION definition, and are "
  "initialized BYVAL when the function is invoked by another rou"
  "tine.",        /* Description */
  "FUNCTION",        /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_GO,           /* UniqueID */
  "GO",           /* Syntax */
  "GO by itself is a syntax error.  Use GOTO or GOSUB instead.",  /* Description */
  "GO",           /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_GOSUB,        /* UniqueID */
  "GOSUB line",         /* Syntax */
  "Initiates a subroutine call to the line specified.  The subro"
  "utine must end with RETURN.  The 'line' may be a number or a "
  "label.",       /* Description */
  "GOSUB",        /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_GOTO,         /* UniqueID */
  "GOTO line",       /* Syntax */
  "Branches program execution to the specified line.  The 'line'"
  " may be a number or a label.",   /* Description */
  "GOTO",         /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_HELP,         /* UniqueID */
  "HELP name",       /* Syntax */
  "Provides help on the specified 'name' which is a command name"
  " or function name.",    /* Description */
  "HELP",         /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_IF,           /* UniqueID */
  "IF expression THEN line1 [ELSE line2]",   /* Syntax */
  "Single line standard IF command.  If the value of expression "
  "is non-zero, then branh to line1.  If the value of expression"
  " is zero and ELSE is provided, then branch to line2.  If the "
  "value of expression is zero and ELSE is not provided, continu"
  "e to the next line.  LABELS are not allowed.",   /* Description */
  "IF",           /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_IF_THEN,         /* UniqueID */
  "IF expression THEN",    /* Syntax */
  "Top of a multi-line IF - END IF structure.  If the value of e"
  "xpression is non-zero, then the program lines upto the next E"
  "LSE or ELSE IF command are executed, otherwise the program br"
  "anches to the next ELSE or ELSE IF command.",   /* Description */
  "IF THEN",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_INPUT,        /* UniqueID */
  "INPUT [# device-number]|[;][\"prompt string\";] variable [, ...]"
  "s",            /* Syntax */
  "Reads input from the terminal or athe file specified by devic"
  "e-number.",       /* Description */
  "INPUT",        /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_LET,       /* UniqueID */
  "[LET] variable = expression",/* Syntax */
  "Assigns the value of expression to variable.  The 'LET' keywo"
  "rd is optional.",    /* Description */
  "LET",       /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_LINE,         /* UniqueID */
  "LINE INPUT [[#] device-number,][\"prompt string\";] string-vari"
  "able$",        /* Syntax */
  "Reads entire line from the keyboard or a file into string-var"
  "iable$.",         /* Description */
  "LINE",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_LIST,         /* UniqueID */
  "LIST line1 [- line2]",  /* Syntax */
  "Lists BASIC program lines from 'line1' to 'line2'",   /* Description */
  "LIST",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_LOAD,         /* UniqueID */
  "LOAD [file-name$]",     /* Syntax */
  "Loads an ASCII BASIC program into memory.",  /* Description */
  "LOAD",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_LOOP,         /* UniqueID */
  "LOOP",         /* Syntax */
  "Bottom of a DO - LOOP structure.  IF the loop is not terminat"
  "ed by EXIT DO or DO UNTIL or DO WHILE, then it will loop fore"
  "ver.",         /* Description */
  "LOOP",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_LOOP_UNTIL,         /* UniqueID */
  "LOOP UNTIL",         /* Syntax */
  "Bottom of a DO - LOOP structure.  If the expression is zero, "
  "then the loop is terminated.",   /* Description */
  "LOOP UNTIL",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_LOOP_WHILE,         /* UniqueID */
  "LOOP WHILE",         /* Syntax */
  "Bottom of a DO - LOOP structure.  If the expression is non-ze"
  "ro, then the loop is terminated.",  /* Description */
  "LOOP WHILE",         /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_LPRINT,       /* UniqueID */
  "LPRINT [USING format-string$;] expressions...", /* Syntax */
  "Send output to the default printer which is implementation de"
  "fined.",       /* Description */
  "LPRINT",       /* Name */
  B14 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_LSET,         /* UniqueID */
  "LSET string-variable$ = expression",   /* Syntax */
  "Left-aligns the value of expression into string-variable$.  I"
  "f the length of the value is too short, then it is padded on "
  "the right with spaces.  If the length of the value is too lon"
  "g, then it is truncated on the right.  This is only for use w"
  "ith variables assigned to a random access buffer with FIELD c"
  "ommand.",         /* Description */
  "LSET",         /* Name */
  B14 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_MAINTAINER,         /* UniqueID */
  "MAINTAINER",         /* Syntax */
  "This command is reserved for use by the Bywater BASIC maintai"
  "ner and does whatever the maintainer wants it to do.  It is n"
  "ot for the BASIC programmer.",   /* Description */
  "MAINTAINER",         /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_MERGE,        /* UniqueID */
  "MERGE file-name",    /* Syntax */
  "Merges the BASIC program in file-name into the current BASIC "
  "program.  Lines in file-name replace any matching lines in th"
  "e current program.",    /* Description */
  "MERGE",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_MID_,         /* UniqueID */
  "MID$( variable$, start [, count ] ) = expression", /* Syntax */
  "Replaces a subtring of variable$ with expression.",   /* Description */
  "MID$",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_NAME,         /* UniqueID */
  "NAME old-file-name AS new-file-name",  /* Syntax */
  "Changes the name of an existing file.",   /* Description */
  "NAME",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_NEW,       /* UniqueID */
  "NEW",       /* Syntax */
  "Deletes the program in memory and clears all variables.",   /* Description */
  "NEW",       /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_NEXT,         /* UniqueID */
  "NEXT [variable]",    /* Syntax */
  "The bottom line of a FOR - NEXT structure.", /* Description */
  "NEXT",         /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_ON,           /* UniqueID */
  "ON expression GOTO | GOSUB line [, ...]", /* Syntax */
  "Branches (GOTO) or calls (GOSUB) based on the rounded value o"
  "f variable.",     /* Description */
  "ON",           /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_ON_ERROR_GOTO,        /* UniqueID */
  "ON ERROR GOTO errline", /* Syntax */
  "When a trappable error occurs, execute 'GOTO errline'.  "
  "The error handler must terminate with a RESUME command.  "
  "If the line number is 0 (zerp), then use the default error handler.  "
  "Valid when OPTION ERROR GOTO.", /* Description */
  "ON ERROR GOTO",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_ON_ERROR_GOSUB,        /* UniqueID */
  "ON ERROR GOSUB errline", /* Syntax */
  "When a trappable error occurs, execute 'GOSUB errline'.  "
  "The error handler must terminate with a RETURN command.  "
  "If the line number is 0 (zerp), then use the default error handler.  "
  "Valid when OPTION ERROR GOSUB.", /* Description */
  "ON ERROR GOSUB",        /* Name */
  B14 /* OptionVersionBitmask */
  },
 {
  C_ON_ERROR_RESUME_NEXT,        /* UniqueID */
  "ON ERROR RESUME NEXT", /* Syntax */
  "When a trappable error occurs, execution continues with the next line.  "
  "Valid when OPTION ERROR GOTO.", /* Description */
  "ON ERROR RESUME NEXT",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_ON_ERROR_RETURN_NEXT,        /* UniqueID */
  "ON ERROR RETURN NEXT", /* Syntax */
  "When a trappable error occurs, execution continues with the next line.  "
  "Valid when OPTION ERROR GOSUB.", /* Description */
  "ON ERROR RETURN NEXT",        /* Name */
  B14 /* OptionVersionBitmask */
  },
 {
  C_ON_TIMER,        /* UniqueID */
  "ON TIMER count GOSUB line",   /* Syntax */
  "Specifies a line (or label) to gosub when count seconds have "
  "elaspsed after TIMER ON is executed.  The interrupt routine s"
  "hould end with a RETURN command.  Timer events only occur in "
  "running BASIC programs.  The resolution of the clock is imple"
  "mentation defined.",    /* Description */
  "ON TIMER",        /* Name */
  B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_OPEN,         /* UniqueID */
  "OPEN file-name [FOR INPUT|OUTPUT|APPEND|BINARY|RANDOM] AS [#]"
  "device-number [LEN = record-length]",  /* Syntax */
  "Opens a file for use.  INPUT, OUTPUT, and APPEND are for text"
  " files.  BINARY is for binary files.  RANDOM is for structure"
  "d binary files and requires LEN.  Files opened for RANDOM use"
  " GET and PUT.",      /* Description */
  "OPEN",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_OPTION,       /* UniqueID */
  "OPTION ...",         /* Syntax */
  "OPTION by itself is a syntax error.",  /* Description */
  "OPTION",       /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_OPTION_ANGLE_DEGREES,  /* UniqueID */
  "OPTION ANGLE DEGREES",  /* Syntax */
  "Configures the math functions to accept and return angles in "
  "degrees.",        /* Description */
  "OPTION ANGLE DEGREES",  /* Name */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  C_OPTION_ANGLE_RADIANS,  /* UniqueID */
  "OPTION ANGLE RADIANS",  /* Syntax */
  "Configures the math functions to accept and return angles in "
  "radians.",        /* Description */
  "OPTION ANGLE RADIANS",  /* Name */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  C_OPTION_ARITHMETIC_DECIMAL,   /* UniqueID */
  "OPTION ARITHMETIC DECIMAL",   /* Syntax */
  "Currently has no effect.", /* Description */
  "OPTION ARITHMETIC DECIMAL",   /* Name */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  C_OPTION_ARITHMETIC_FIXED,  /* UniqueID */
  "OPTION ARITHMETIC FIXED",  /* Syntax */
  "Currently has no effect.", /* Description */
  "OPTION ARITHMETIC FIXED",  /* Name */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  C_OPTION_ARITHMETIC_NATIVE, /* UniqueID */
  "OPTION ARITHMETIC NATIVE", /* Syntax */
  "Currently has no effect.", /* Description */
  "OPTION ARITHMETIC NATIVE", /* Name */
  B14 | E86       /* OptionVersionBitmask */
  },
 {
  C_OPTION_BASE_0,      /* UniqueID */
  "OPTION BASE 0",      /* Syntax */
  "Sets the lowest array subscript to 0.",   /* Description */
  "OPTION BASE 0",      /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_OPTION_BASE_1,      /* UniqueID */
  "OPTION BASE 1",      /* Syntax */
  "Sets the lowest array subscript to 1.",   /* Description */
  "OPTION BASE 1",      /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_OPTION_BUGS_OFF,    /* UniqueID */
  "OPTION BUGS OFF",    /* Syntax */
  "Disables bugs commonly found in BASIC-80 derivatives and clon"
  "es.",       /* Description */
  "OPTION BUGS OFF",    /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_BUGS_ON,     /* UniqueID */
  "OPTION BUGS ON",     /* Syntax */
  "Enables bugs commonly found in BASIC-80 derivatives and clone"
  "s.",           /* Description */
  "OPTION BUGS ON",     /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_COMMENT, /* UniqueID */
  "OPTION COMMENT char", /* Syntax */
  "Sets the comment character.",  /* Description */
  "OPTION COMMENT", /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_COMPARE_BINARY, /* UniqueID */
  "OPTION COMPARE BINARY", /* Syntax */
  "Causes string comparisons to be case-sensitive.",  /* Description */
  "OPTION COMPARE BINARY", /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_COMPARE_DATABASE,   /* UniqueID */
  "OPTION COMPARE DATABASE",   /* Syntax */
  "Causes string comparisons to be case-insensitive.",   /* Description */
  "OPTION COMPARE DATABASE",   /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_COMPARE_TEXT,   /* UniqueID */
  "OPTION COMPARE TEXT",   /* Syntax */
  "Causes string comparisons to be case-insensitive.",   /* Description */
  "OPTION COMPARE TEXT",   /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_COVERAGE_OFF,   /* UniqueID */
  "OPTION COVERAGE OFF",   /* Syntax */
  "Disables BASIC code coverage recording, displayed using the L"
  "IST command.",    /* Description */
  "OPTION COVERAGE OFF",   /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_COVERAGE_ON,    /* UniqueID */
  "OPTION COVERAGE ON",    /* Syntax */
  "Enables BASIC code coverage recording, displayed using the LI"
  "ST command.",     /* Description */
  "OPTION COVERAGE ON",    /* Name */
  B14          /* OptionVersionBitmask */
  },  
 {
  C_OPTION_DATE, /* UniqueID */
  "OPTION DATE format", /* Syntax */
  "Sets the date format string used by C strftime() for DATE$.",  /* Description */
  "OPTION DATE", /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_DISABLE_COMMAND,      /* UniqueID */
  "OPTION DISABLE COMMAND name",  /* Syntax */
  "Disables the specified BASIC command.",  /* Description */
  "OPTION DISABLE COMMAND",      /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_DISABLE_FUNCTION,      /* UniqueID */
  "OPTION DISABLE FUNCTION name",  /* Syntax */
  "Disables the specified BASIC function.",  /* Description */
  "OPTION DISABLE FUNCTION",      /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_DISABLE_OPERATOR,      /* UniqueID */
  "OPTION DISABLE OPERATOR name",  /* Syntax */
  "Disables the specified BASIC operator.",  /* Description */
  "OPTION DISABLE OPERATOR",      /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_ENABLE_COMMAND,      /* UniqueID */
  "OPTION ENABLE COMMAND name",  /* Syntax */
  "Enables the specified BASIC command.",  /* Description */
  "OPTION ENABLE COMMAND",      /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_ENABLE_FUNCTION,      /* UniqueID */
  "OPTION ENABLE FUNCTION name",  /* Syntax */
  "Enables the specified BASIC function.",  /* Description */
  "OPTION ENABLE FUNCTION",      /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_ENABLE_OPERATOR,      /* UniqueID */
  "OPTION ENABLE OPERATOR name",  /* Syntax */
  "Enables the specified BASIC operator.",  /* Description */
  "OPTION ENABLE OPERATOR",      /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_ERROR_GOSUB,  /* UniqueID */
  "OPTION ERROR GOSUB",  /* Syntax */
  "When an error occurs, GOSUB to the error handler.  "
  "The error handler exits with RETURN.", /* Description */
  "OPTION ERROR GOSUB",  /* Name */
  B14       /* OptionVersionBitmask */
  },
 {
  C_OPTION_ERROR_GOTO,  /* UniqueID */
  "OPTION ERROR GOTO",  /* Syntax */
  "When an error occurs, GOTO to the error handler.  "
  "The error handler exits with RESUME.", /* Description */
  "OPTION ERROR GOTO",  /* Name */
  B14       /* OptionVersionBitmask */
  },
 {
  C_OPTION_INDENT,      /* UniqueID */
  "OPTION INDENT number",  /* Syntax */
  "Sets indention level for LIST.  Zero means no indention.  Def"
  "ault is 2.",         /* Description */
  "OPTION INDENT",      /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_LABELS_OFF,    /* UniqueID */
  "OPTION LABELS OFF",    /* Syntax */
  "Disables text labels.", /* Description */
  "OPTION LABELS OFF",    /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_LABELS_ON,     /* UniqueID */
  "OPTION LABELS ON",     /* Syntax */
  "Enables text labels.", /* Description */
  "OPTION LABELS ON",     /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_STATEMENT, /* UniqueID */
  "OPTION STATEMENT char", /* Syntax */
  "Sets the statement seperator character.",  /* Description */
  "OPTION STATEMENT", /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_STRICT_OFF,     /* UniqueID */
  "OPTION STRICT OFF",     /* Syntax */
  "Disables checking for implicit array creation without using t"
  "he DIM command.",    /* Description */
  "OPTION STRICT OFF",     /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_STRICT_ON,      /* UniqueID */
  "OPTION STRICT ON",      /* Syntax */
  "Enables checking for implicit array creation without using th"
  "e DIM command.",     /* Description */
  "OPTION STRICT ON",      /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_TERMINAL_ADM_3A,   /* UniqueID */
  "OPTION TERMINAL ADM-3A",   /* Syntax */
  "Enables ADM-3A terminal control codes for CLS, COLOR, and LOC"
  "ATE.",         /* Description */
  "OPTION TERMINAL ADM-3A",   /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_TERMINAL_ANSI,  /* UniqueID */
  "OPTION TERMINAL ANSI",  /* Syntax */
  "Enables ANSI terminal control codes for CLS, COLOR, and LOCAT"
  "E.",           /* Description */
  "OPTION TERMINAL ANSI",  /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_TERMINAL_NONE,  /* UniqueID */
  "OPTION TERMINAL NONE",  /* Syntax */
  "Disables terminal control codes for CLS, COLOR, and LOCATE.",  /* Description */
  "OPTION TERMINAL NONE",  /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_TIME, /* UniqueID */
  "OPTION TIME format", /* Syntax */
  "Sets the time format string used by C strftime() for TIME$.",  /* Description */
  "OPTION TIME", /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_TRACE_OFF,      /* UniqueID */
  "OPTION TRACE OFF",      /* Syntax */
  "Disables displaying a stack trace when an ERROR occurs.",   /* Description */
  "OPTION TRACE OFF",      /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_TRACE_ON,    /* UniqueID */
  "OPTION TRACE ON",    /* Syntax */
  "Enables displaying a stack trace when an ERROR occurs.", /* Description */
  "OPTION TRACE ON",    /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_OPTION_VERSION,     /* UniqueID */
  "OPTION VERSION [version] ",   /* Syntax */
  "Selects a combination of OPTION settings and enables certain "
  "commands and functions.  If no version is specified, displays"
  " a list of the available versions.",   /* Description */
  "OPTION VERSION",     /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_PRINT,        /* UniqueID */
  "PRINT [# device-number,][USING format-string$;] expressions.."
  ".",            /* Syntax */
  "Sends output to the console or a file.",  /* Description */
  "PRINT",        /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_QUIT,         /* UniqueID */
  "QUIT",         /* Syntax */
  "Exits to the operating system.", /* Description */
  "QUIT",         /* Name */
  B14 | D64 | E78 | E86    /* OptionVersionBitmask */
  },
 {
  C_READ,         /* UniqueID */
  "READ variable [, ...]", /* Syntax */
  "Reads values from DATA statements.",   /* Description */
  "READ",         /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_REM,       /* UniqueID */
  "REM ...",         /* Syntax */
  "Remark.",         /* Description */
  "REM",       /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_RENUM,        /* UniqueID */
  "RENUM",        /* Syntax */
  "Implementation defined.",  /* Description */
  "RENUM",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_RESTORE,         /* UniqueID */
  "RESTORE [line]",     /* Syntax */
  "Resets the line used for the next READ statement.  'line' may"
  " be either a number or a label.",   /* Description */
  "RESTORE",         /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_RESUME,       /* UniqueID */
  "RESUME [ 0 | line | NEXT ]",  /* Syntax */
  "Used in an error handler to specify the next line to execute."
  "  RESUME and RESUME 0 branch to ERL.  RESUME NEXT branches to"
  " the line after ERL.  RESUME line branches to the specified l"
  "ine.  'line' may be either a number or a label.",  /* Description */
  "RESUME",       /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_RETURN,       /* UniqueID */
  "RETURN",       /* Syntax */
  "Concludes a subroutine called by GOSUB.", /* Description */
  "RETURN",       /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_RSET,         /* UniqueID */
  "RSET string-variable$ = expression",   /* Syntax */
  "Right-aligns the value of expression into string-variable$.  "
  "If the length of the value is too short, then it is padded on"
  " the left with spaces.  If the length of the value is too lon"
  "g, then it is truncated on the right.  This is only for use w"
  "ith variables assigned to a random access buffer with FIELD c"
  "ommand.",         /* Description */
  "RSET",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_RUN,       /* UniqueID */
  "RUN [line | file-name$]",  /* Syntax */
  "RUN executes the program in memory from the start.  RUN line "
  "exexecutes the program in memory beginning at 'line'.  RUN fi"
  "le-name$ loads a new BAASIC program and executes the program "
  "from the start.",    /* Description */
  "RUN",       /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_SAVE,         /* UniqueID */
  "SAVE [file-name$]",     /* Syntax */
  "Saves the current program into the file file-name$ in ASCII f"
  "ormat.",       /* Description */
  "SAVE",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_SELECT,       /* UniqueID */
  "SELECT",       /* Syntax */
  "SELECT by itself is a syntax error.  Use SELECT CASE instead."
  "",          /* Description */
  "SELECT",       /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_SELECT_CASE,     /* UniqueID */
  "SELECT CASE expression",   /* Syntax */
  "Introduces a multi-line conditional selection statement.",  /* Description */
  "SELECT CASE",     /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_STOP,         /* UniqueID */
  "STOP",         /* Syntax */
  "Interrupts program execution and displays the line number of "
  "the STOP command.  For use when debugging BASIC programs.  Wh"
  "ether STOP issues a SIGINT signal is implementation defined.", /* Description */
  "STOP",         /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_SUB,       /* UniqueID */
  "SUB name [ ( parameter [,...] ) ]", /* Syntax */
  "Top line of a multi-line SUB definition.  The variable names "
  "specified are local to the SUB definition, and are initialize"
  "d BYVAL when the subroutine is invoked by another routine.",   /* Description */
  "SUB",       /* Name */
  B14 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_SWAP,         /* UniqueID */
  "SWAP variable, variable",  /* Syntax */
  "Swaps the values of two variables. Both variables must be of "
  "the same type.",     /* Description */
  "SWAP",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_SYSTEM,       /* UniqueID */
  "SYSTEM",       /* Syntax */
  "Exits to the operating system.", /* Description */
  "SYSTEM",       /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_TIMER,        /* UniqueID */
  "TIMER", /* Syntax */
  "TIMER by itself is a syntax error.  "
  "Use TIMER OFF, TIMER ON, or TIMER STOP instead.",   /* Description */
  "TIMER",        /* Name */
  B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_TIMER_OFF,        /* UniqueID */
  "TIMER OFF", /* Syntax */
  "TIMER OFF terminates the timer interrupt.",   /* Description */
  "TIMER OFF",        /* Name */
  B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_TIMER_ON,        /* UniqueID */
  "TIMER ON", /* Syntax */
  "TIMER ON enables the timer interrupt.  When the specified seconds have elapsed, TIMER STOP "
  "is internaly executed before the interrupt is taken.  TIMER ON"
  " should be executed just before the RETURN command if you wan"
  "t the interrupt to occur again.",   /* Description */
  "TIMER ON",        /* Name */
  B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_TIMER_STOP,        /* UniqueID */
  "TIMER STOP", /* Syntax */
  "TIMER STOP disables the interrupt, but the count continues.",   /* Description */
  "TIMER STOP",        /* Name */
  B14 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_UEND,         /* UniqueID */
  "UEND",         /* Syntax */
  "Bottom of a UNTIL - UEND structure.",  /* Description */
  "UEND",         /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_UNTIL,        /* UniqueID */
  "UNTIL expression",      /* Syntax */
  "Top of a UNTIL - UEND structure.  If the expression is zero, "
  "then the loop is terminated.",   /* Description */
  "UNTIL",        /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_USER_LBL,        /* UniqueID */
  "USER LBL",        /* Syntax */
  "An internal command that allows the BASIC program to find a u"
  "ser label.  You do not directly call this command.  If your p"
  "rogram has a line with a label, then USER LBL is used to mark"
  " the line.",         /* Description */
  "USER LBL",        /* Name */
  B14 | D64 | E78 | M80 | T83 | T84 | M85 | E86 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_VARS,         /* UniqueID */
  "VARS",         /* Syntax */
  "Prints a list of all global variables.",  /* Description */
  "VARS",         /* Name */
  B14          /* OptionVersionBitmask */
  },
 {
  C_WEND,         /* UniqueID */
  "WEND",         /* Syntax */
  "Bottom of a WHILE - WEND structure.",  /* Description */
  "WEND",         /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_WHILE,        /* UniqueID */
  "WHILE",        /* Syntax */
  "Top of a WHILE - WEND structure.  If the expression is non-ze"
  "ro, then the loop is terminated.",  /* Description */
  "WHILE",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
 {
  C_WRITE,        /* UniqueID */
  "WRITE [# device-number,] element [, .... ]", /* Syntax */
  "Outputs variables to the screen or to a file.", /* Description */
  "WRITE",        /* Name */
  B14 | M80 | T83 | T84 | M85 | M88 | M90 | M91 | M92 | M93 | M95 | M97 | M98 /* OptionVersionBitmask */
  },
};


/* EOF */
