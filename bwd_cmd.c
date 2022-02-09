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
/* Version 3.10 by Howard Wulf, AF5NE                            */
/*                                                               */
/*---------------------------------------------------------------*/



/* COMMAND TABLE */

#include "bwbasic.h"

CommandType bwb_cmdtable[ NUM_COMMANDS ] =
{
{
   C_QUEST,  /* UniqueID */
   "? expressions...",  /* Syntax */
   "This is just a shortcut for PRINT.",  /* Description */
   "?",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_APPEND,  /* UniqueID */
   "APPEND filename$ | APPEND # filenumber",  /* Syntax */
   "APPEND filename$ Merges the BASIC program in file-name into t"
"he current BASIC program; lines in filename$ replace any matc"
"hing lines in the current program.  APPEND # positions filenu"
"mber at EOF and sets the file to writing; filenumber <= 0 is "
"ignored.",  /* Description */
   "APPEND",  /* Name */
   B15 | D64 | G65 | G67 | HB2  /* OptionVersionBitmask */
},
{
   C_AS,  /* UniqueID */
   "AS string-variable$ [, ...]",  /* Syntax */
   "AS by itself is a Syntax Error.  Use FIELD ... AS instead.",  /* Description */
   "AS",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_AUTO,  /* UniqueID */
   "AUTO [start [, increment]]",  /* Syntax */
   "Automatic line numbering for manual program entry.  If the li"
"ne already exists, then an asterisk is displayed and pressing"
" ENTER leaves the line as-is.  If the line does not exist, th"
"en an asterisk is not displayed and pressing ENTER terminates"
" AUTO mode.  Regardless of the line's existance, entering the"
" command MAN will terminate AUTO mode.  AUTO mode is also ter"
"minated by any ERROR or by pressing control-C.",  /* Description */
   "AUTO",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88  /* OptionVersionBitmask */
},
{
   C_BACKSPACE,  /* UniqueID */
   "BACKSPACE # X",  /* Syntax */
   "Points the file to the previous item.",  /* Description */
   "BACKSPACE",  /* Name */
   D64 | G65 | G67  /* OptionVersionBitmask */
},
{
   C_BREAK,  /* UniqueID */
   "BREAK line [, ...]]",  /* Syntax */
   "Diagnostic command to stop execution at the specified line(s)"
".  BREAK only applies to user-numbered lines.  For multi-stat"
"ement lines, BREAK only applies to the first statement.  BREA"
"K effectively inserts a hidden STOP command immediately after"
" the line number.  Once a BREAK occurrs on a specified line, "
"it is automatically removed.  To remove all existing BREAKs, "
"execute BREAK without any line numbers.",  /* Description */
   "BREAK",  /* Name */
   B15 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_BYE,  /* UniqueID */
   "BYE",  /* Syntax */
   "Exits to the operating system.",  /* Description */
   "BYE",  /* Name */
   B15 | D64 | G65 | G67 | D71 | HB2  /* OptionVersionBitmask */
},
{
   C_CALL,  /* UniqueID */
   "CALL subroutine-name( [parameter [, ...] ] )",  /* Syntax */
   "Calls a subroutine that was defined by SUB and END SUB.",  /* Description */
   "CALL",  /* Name */
   B15 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_CASE,  /* UniqueID */
   "CASE constant [TO constant] | CASE IF partial-expression | CA"
"SE IS partial-expression",  /* Syntax */
   "Introduces an element of a SELECT CASE statement.  Multiple t"
"ests must be seperated a comma.  For example:  CASE 1, 2 TO 3"
", IS > 4, IF < 5",  /* Description */
   "CASE",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_CASE_ELSE,  /* UniqueID */
   "CASE ELSE",  /* Syntax */
   "Introduces a default SELECT CASE element.",  /* Description */
   "CASE ELSE",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_CHAIN,  /* UniqueID */
   "CHAIN filename$ [, linenumber]",  /* Syntax */
   "Load and execute another BASIC program, without clearing comm"
"on variables.  For System/370, the syntax is CHAIN filename$,"
"parameter$.",  /* Description */
   "CHAIN",  /* Name */
   B15 | G65 | G67 | I70 | D71 | I73 | C77 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_CHANGE,  /* UniqueID */
   "CHANGE A$ TO X | CHANGE X TO A$",  /* Syntax */
   "Changes a string to a numeric array or a numeric array to a s"
"tring.",  /* Description */
   "CHANGE",  /* Name */
   B15 | D64 | G65 | G67 | D71  /* OptionVersionBitmask */
},
{
   C_CLEAR,  /* UniqueID */
   "CLEAR",  /* Syntax */
   "Sets all numeric variables to 0, and all string variables to "
"empty strings.",  /* Description */
   "CLEAR",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_CLOAD,  /* UniqueID */
   "CLOAD [file-name$]",  /* Syntax */
   "Loads an ASCII BASIC program into memory.",  /* Description */
   "CLOAD",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85  /* OptionVersionBitmask */
},
{
   C_CLOAD8,  /* UniqueID */
   "CLOAD* ArrayName",  /* Syntax */
   "Loads a numeric array from a file saved using CSAVE*.",  /* Description */
   "CLOAD*",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85  /* OptionVersionBitmask */
},
{
   C_CLOSE,  /* UniqueID */
   "CLOSE filename$ [, ...]",  /* Syntax */
   "Closes an input or output file.",  /* Description */
   "CLOSE",  /* Name */
   I70 | D71 | I73 | C77  /* OptionVersionBitmask */
},
{
   C_CLR,  /* UniqueID */
   "CLR",  /* Syntax */
   "Sets all numeric variables to 0, and all string variables to "
"empty strings.",  /* Description */
   "CLR",  /* Name */
   HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_CMDS,  /* UniqueID */
   "CMDS",  /* Syntax */
   "Prints a list of all implemented BASIC commands.",  /* Description */
   "CMDS",  /* Name */
   B15  /* OptionVersionBitmask */
},
{
   C_COMMON,  /* UniqueID */
   "COMMON variable [, ...]",  /* Syntax */
   "Designates variables to be passed to a CHAINed program.",  /* Description */
   "COMMON",  /* Name */
   B15 | D71 | C77 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_CONSOLE,  /* UniqueID */
   "CONSOLE [WIDTH width]",  /* Syntax */
   "Directs the output of PRINT commands to the console (stdout),"
" and optionally changes the console width.",  /* Description */
   "CONSOLE",  /* Name */
   C77  /* OptionVersionBitmask */
},
{
   C_CONT,  /* UniqueID */
   "CONT",  /* Syntax */
   "Continue a BASIC program after a STOP has been executed.  Pro"
"gram resumes at the line after the STOP.",  /* Description */
   "CONT",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_CREATE,  /* UniqueID */
   "CREATE filename$ [ RECL reclen ] AS filenum [ BUFF number ] ["
" RECS size ]",  /* Syntax */
   "Creates the file and opens it.  If the file already exists, t"
"hen it is ereased.  If the file cannot be created, then an er"
"ror occurs.",  /* Description */
   "CREATE",  /* Name */
   C77  /* OptionVersionBitmask */
},
{
   C_CSAVE,  /* UniqueID */
   "CSAVE [file-name$]",  /* Syntax */
   "Saves the current program into the file file-name$ in ASCII f"
"ormat.",  /* Description */
   "CSAVE",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85  /* OptionVersionBitmask */
},
{
   C_CSAVE8,  /* UniqueID */
   "CSAVE* ArrayName",  /* Syntax */
   "Saves a numeric array into a file for later loading by CLOAD*"
".",  /* Description */
   "CSAVE*",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85  /* OptionVersionBitmask */
},
{
   C_DATA,  /* UniqueID */
   "DATA constant [, ...]",  /* Syntax */
   "Stores numeric and string constants to be accessed by READ.",  /* Description */
   "DATA",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DEF,  /* UniqueID */
   "DEF FNname[(arg...)] [= expression]",  /* Syntax */
   "Defines a user-written function.  Single line DEF functions u"
"se '= expression'.  Multiple line DEF functions do not use '="
" expression' and must end with FNEND.",  /* Description */
   "DEF",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DEF_SUB,  /* UniqueID */
   "DEF SUB",  /* Syntax */
   "An internal command that allows the BASIC program to execute "
"any function as though it were a command.  You do not directl"
"y call this command.  If your program has a line  beginning w"
"ith a functio name, then DEF SUB is used to execute the funct"
"ion and ignore the result.",  /* Description */
   "DEF SUB",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DEFBYT,  /* UniqueID */
   "DEFBYT letter[-letter] [, ...]",  /* Syntax */
   "Declares variables with single-letter names as numeric variab"
"les.  TODO: enforce the range of [MINBYT,MAXBYT].",  /* Description */
   "DEFBYT",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DEFCUR,  /* UniqueID */
   "DEFCUR letter[-letter] [, ...]",  /* Syntax */
   "Declares variables with single-letter names as numeric variab"
"les.  TODO: enforce the range of [MINCUR,MAXCUR].",  /* Description */
   "DEFCUR",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DEFDBL,  /* UniqueID */
   "DEFDBL letter[-letter] [, ...]",  /* Syntax */
   "Declares variables with single-letter names as numeric variab"
"les.  TODO: enforce the range of [MINDBL,MAXDBL].",  /* Description */
   "DEFDBL",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DEFINT,  /* UniqueID */
   "DEFINT  letter[-letter] [, ...]",  /* Syntax */
   "Declares variables with single-letter names as numeric variab"
"les.  TODO: enforce the range of [MININT,MAXINT].",  /* Description */
   "DEFINT",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DEFLNG,  /* UniqueID */
   "DEFLNG letter[-letter] [, ...]",  /* Syntax */
   "Declares variables with single-letter names as numeric variab"
"les.  TODO: enforce the range of [MINLNG,MAXLNG].",  /* Description */
   "DEFLNG",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DEFSNG,  /* UniqueID */
   "DEFSNG letter[-letter] [, ...]",  /* Syntax */
   "Declares variables with single-letter names as numeric variab"
"les.  TODO: enforce the range of [MINSNG,MAXSNG].",  /* Description */
   "DEFSNG",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DEFSTR,  /* UniqueID */
   "DEFSTR letter[-letter] [, ...]",  /* Syntax */
   "Declares variables with single-letter names as string variabl"
"es.  TODO: enforce the range of [MINLEN,MAXLEN].",  /* Description */
   "DEFSTR",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DELETE,  /* UniqueID */
   "DELETE line [- line]",  /* Syntax */
   "Deletes program lines indicated by the argument(s).  All prog"
"ram lines have a number, which is visible with the LIST comma"
"nd.  If line numbers are not provided, they are assigned begi"
"nning with 1.  Deleting a non-existing line does not cause an"
" error.  For CBASIC-II, the syntax is DELETE filenumber.",  /* Description */
   "DELETE",  /* Name */
   B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DELIMIT,  /* UniqueID */
   "DELIMIT # X, A$",  /* Syntax */
   "Sets the delimiter for READ # and WRITE #.",  /* Description */
   "DELIMIT",  /* Name */
   G65 | G67  /* OptionVersionBitmask */
},
{
   C_DIM,  /* UniqueID */
   "DIM [ # filenum , ] variable( elements [, ...]) [, ...]",  /* Syntax */
   "Specifies the dimensions of an array variables.",  /* Description */
   "DIM",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DO,  /* UniqueID */
   "DO [UNTIL|WHILE expression]",  /* Syntax */
   "Top of a DO - LOOP structure.  DO: If the loop is not termina"
"ted by EXIT DO or LOOP UNTIL or LOOP WHILE, then it will loop"
" forever.  DO UNTIL: exits when non-zero.  DO WHILE: exits wh"
"en zero.",  /* Description */
   "DO",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_DSP,  /* UniqueID */
   "DSP variable [, ...]]",  /* Syntax */
   "Diagnostic command to display the value every time the variab"
"le is assigned.  To remove all existing DSPs, execute DSP wit"
"hout any variables.",  /* Description */
   "DSP",  /* Name */
   B15 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_EDIT,  /* UniqueID */
   "EDIT",  /* Syntax */
   "implementation defined.",  /* Description */
   "EDIT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_ELSE,  /* UniqueID */
   "ELSE",  /* Syntax */
   "Introduces a default condition in a multi-line IF statement.",  /* Description */
   "ELSE",  /* Name */
   B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_ELSEIF,  /* UniqueID */
   "ELSEIF",  /* Syntax */
   "Introduces a secondary condition in a multi-line IF statement"
".",  /* Description */
   "ELSEIF",  /* Name */
   B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_END,  /* UniqueID */
   "END",  /* Syntax */
   "Terminates program execution. If the BASIC program was execut"
"ed from the operating system level, then control returns to t"
"he operating system, oterwise control reuturns to the BASIC p"
"rompt.",  /* Description */
   "END",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_END_FUNCTION,  /* UniqueID */
   "END FUNCTION",  /* Syntax */
   "Specifies the last line of a multi-line FUNCTION definition.",  /* Description */
   "END FUNCTION",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_END_IF,  /* UniqueID */
   "END IF",  /* Syntax */
   "Specifies the last line of a multi-line IF definition.",  /* Description */
   "END IF",  /* Name */
   B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_END_SELECT,  /* UniqueID */
   "END SELECT",  /* Syntax */
   "Specifies the last line of a multi-line SELECT CASE definitio"
"n.",  /* Description */
   "END SELECT",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_END_SUB,  /* UniqueID */
   "END SUB",  /* Syntax */
   "Specifies the last line of a multi-line SUB definition.",  /* Description */
   "END SUB",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_ERASE,  /* UniqueID */
   "ERASE variable [, ...]",  /* Syntax */
   "Eliminates arrayed variables from a program.",  /* Description */
   "ERASE",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_EXCHANGE,  /* UniqueID */
   "EXCHANGE variable, variable",  /* Syntax */
   "Swaps the values of two variables. Both variables must be of "
"the same type.",  /* Description */
   "EXCHANGE",  /* Name */
   B15 | HB2  /* OptionVersionBitmask */
},
{
   C_EXIT,  /* UniqueID */
   "EXIT",  /* Syntax */
   "EXIT by itself is a Syntax Error.  Use EXIT DO, EXIT FOR, EXI"
"T FUNCTION, EXIT SUB, EXIT UNTIL, or EXIT WHILE instead.",  /* Description */
   "EXIT",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_EXIT_DO,  /* UniqueID */
   "EXIT DO",  /* Syntax */
   "Immediately exits the inner-most DO-LOOP strucure.",  /* Description */
   "EXIT DO",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_EXIT_FOR,  /* UniqueID */
   "EXIT FOR",  /* Syntax */
   "Immediately exits the inner-most FOR-NEXT strucure.",  /* Description */
   "EXIT FOR",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_EXIT_FUNCTION,  /* UniqueID */
   "EXIT FUNCTION",  /* Syntax */
   "Immediately exits the inner-most multi-line FUNCTION strucure"
".",  /* Description */
   "EXIT FUNCTION",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_EXIT_SUB,  /* UniqueID */
   "EXIT SUB",  /* Syntax */
   "Immediately exits the inner-most multi-line SUB strucure.",  /* Description */
   "EXIT SUB",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_EXIT_UNTIL,  /* UniqueID */
   "EXIT UNTIL",  /* Syntax */
   "Immediately exits the inner-most UNTIL-UEND strucure.",  /* Description */
   "EXIT UNTIL",  /* Name */
   B15 | E86  /* OptionVersionBitmask */
},
{
   C_EXIT_WHILE,  /* UniqueID */
   "EXIT WHILE",  /* Syntax */
   "Immediately exits the inner-most WHILE-END strucure.",  /* Description */
   "EXIT WHILE",  /* Name */
   B15 | E86  /* OptionVersionBitmask */
},
{
   C_FEND,  /* UniqueID */
   "FEND",  /* Syntax */
   "Specifies the last line of a multi-line DEF function.",  /* Description */
   "FEND",  /* Name */
   C77  /* OptionVersionBitmask */
},
{
   C_FIELD,  /* UniqueID */
   "FIELD [#] device-number, number AS string-variable$ [, ...]",  /* Syntax */
   "Assigns 'number' bytes in the buffer of random file 'device-n"
"umber' to the variable 'string-variable$'.  GET will automati"
"cally update the variable, and PUT will automatically use the"
" variable.",  /* Description */
   "FIELD",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_FILE,  /* UniqueID */
   "FILE # X, A$",  /* Syntax */
   "If A$ is '*' then closes file # X.  If A$ is not '*' then ope"
"ns the file named A$ in READ mode.  For CBASIC-II, the syntax"
"is FILE filename$ [( reclen )].",  /* Description */
   "FILE",  /* Name */
   D64 | G65 | G67 | C77  /* OptionVersionBitmask */
},
{
   C_FILES,  /* UniqueID */
   "FILES A$[, ...]",  /* Syntax */
   "If A$ is not '*' opens the file named A$ in READ mode.  The f"
"irst filename of the first FILES command is assocated with fi"
"le number 1.  Note that multiple FILES commands accumulate.",  /* Description */
   "FILES",  /* Name */
   D64 | G65 | G67  /* OptionVersionBitmask */
},
{
   C_FNCS,  /* UniqueID */
   "FNCS",  /* Syntax */
   "Prints a list of all pre-defined BASIC functions.",  /* Description */
   "FNCS",  /* Name */
   B15  /* OptionVersionBitmask */
},
{
   C_FNEND,  /* UniqueID */
   "FNEND",  /* Syntax */
   "Specifies the last line of a multi-line DEF function.",  /* Description */
   "FNEND",  /* Name */
   B15 | G67 | I70 | D71 | I73 | HB1 | HB2 | T80  /* OptionVersionBitmask */
},
{
   C_FOR,  /* UniqueID */
   "FOR variable = start TO finish [STEP increment]",  /* Syntax */
   "Top of a FOR - NEXT structure.  The loop will continue a fixe"
"d number of times, which is determined by the values of start"
", finish, and increment.",  /* Description */
   "FOR",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_FUNCTION,  /* UniqueID */
   "FUNCTION [ ( parameter [, ...  ] ) ]",  /* Syntax */
   "Top line of a multi-line FUNCTION definition.  The variable n"
"ames specified are local to the FUNCTION definition, and are "
"initialized BYVAL when the function is invoked by another rou"
"tine.",  /* Description */
   "FUNCTION",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_GET,  /* UniqueID */
   "GET filename$ , scalar [, ...]",  /* Syntax */
   "Gets one (or more) values from a file.",  /* Description */
   "GET",  /* Name */
   I70 | D71 | I73  /* OptionVersionBitmask */
},
{
   C_GO,  /* UniqueID */
   "GO",  /* Syntax */
   "GO by itself is a syntax error.  Use GOTO or GOSUB instead.",  /* Description */
   "GO",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_GO_SUB,  /* UniqueID */
   "GO SUB line",  /* Syntax */
   "Initiates a subroutine call to the line specified.  The subro"
"utine must end with RETURN.  The 'line' may be a number or a "
"label.",  /* Description */
   "GO SUB",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_GO_TO,  /* UniqueID */
   "GO TO line",  /* Syntax */
   "Branches program execution to the specified line.  The 'line'"
" may be a number or a label.",  /* Description */
   "GO TO",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_GOODBYE,  /* UniqueID */
   "GOODBYE",  /* Syntax */
   "Exits to the operating system.",  /* Description */
   "GOODBYE",  /* Name */
   B15 | D64 | G65 | G67 | D71 | HB2  /* OptionVersionBitmask */
},
{
   C_GOSUB,  /* UniqueID */
   "GOSUB line",  /* Syntax */
   "Initiates a subroutine call to the line specified.  The subro"
"utine must end with RETURN.  The 'line' may be a number or a "
"label.",  /* Description */
   "GOSUB",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_GOTO,  /* UniqueID */
   "GOTO line",  /* Syntax */
   "Branches program execution to the specified line.  The 'line'"
" may be a number or a label.",  /* Description */
   "GOTO",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_HELP,  /* UniqueID */
   "HELP name",  /* Syntax */
   "Provides help on the specified 'name' which is a command name"
" or function name.",  /* Description */
   "HELP",  /* Name */
   B15  /* OptionVersionBitmask */
},
{
   C_IF,  /* UniqueID */
   "IF expression THEN line1 [ELSE line2]",  /* Syntax */
   "Single line standard IF command.  If the value of expression "
"is non-zero, then branh to line1.  If the value of expression"
" is zero and ELSE is provided, then branch to line2.  Otherwi"
"se continue to the next line.  LABELS are not allowed.",  /* Description */
   "IF",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_IF_END,  /* UniqueID */
   "IF END # filenum THEN line1 [ELSE line2]",  /* Syntax */
   "Single line standard IF command.  If the file is at EOF , the"
"n branch to line1.  If the file is not at EOF and ELSE is pro"
"vided, then branch to line2.  Otherwise continue to the next "
"line.  LABELS are not allowed.",  /* Description */
   "IF END",  /* Name */
   D64 | G65 | G67 | C77  /* OptionVersionBitmask */
},
{
   C_IF_MORE,  /* UniqueID */
   "IF MORE # filenum THEN line1 [ELSE line2]",  /* Syntax */
   "Single line standard IF command.  If the file is not at EOF ,"
" then branch to line1.  If the file is at EOF and ELSE is pro"
"vided, then branch to line2.  Otherwise continue to the next "
"line.  LABELS are not allowed.",  /* Description */
   "IF MORE",  /* Name */
   D64 | G65 | G67  /* OptionVersionBitmask */
},
{
   C_IF_THEN,  /* UniqueID */
   "IF expression THEN",  /* Syntax */
   "Top of a multi-line IF - END IF structure.  If the value of e"
"xpression is non-zero, then the program lines upto the next E"
"LSE or ELSE IF command are executed, otherwise the program br"
"anches to the next ELSE or ELSE IF command.",  /* Description */
   "IF THEN",  /* Name */
   B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_IMAGE,  /* UniqueID */
   "IMAGE \"format string\" ",  /* Syntax */
   "Provides format string for PRINT USING line-number.",  /* Description */
   "IMAGE",  /* Name */
   I70 | I73 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_INPUT,  /* UniqueID */
   "INPUT [# device-number]|[;][\"prompt string\";] variable [, ..."
"]s",  /* Syntax */
   "Reads input from the terminal or the file specified by device"
"-number.",  /* Description */
   "INPUT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_LET,  /* UniqueID */
   "[LET] variable = expression",  /* Syntax */
   "Assigns the value of expression to variable.  The 'LET' keywo"
"rd is optional.",  /* Description */
   "LET",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_LINE,  /* UniqueID */
   "LINE INPUT [[#] device-number,][\"prompt string\";] string-vari"
"able$",  /* Syntax */
   "Reads entire line from the keyboard or a file into string-var"
"iable$.",  /* Description */
   "LINE",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_LIST,  /* UniqueID */
   "LIST line1 [- line2]",  /* Syntax */
   "Lists BASIC program lines from 'line1' to 'line2' to stdout.",  /* Description */
   "LIST",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_LLIST,  /* UniqueID */
   "LLIST line1 [- line2]",  /* Syntax */
   "Lists BASIC program lines from 'line1' to 'line2' to the defa"
"ult printer on stderr.",  /* Description */
   "LLIST",  /* Name */
   B15 | D71 | C77 | HB2 | T80 | M80 | M85 | M88  /* OptionVersionBitmask */
},
{
   C_LOAD,  /* UniqueID */
   "LOAD [file-name$]",  /* Syntax */
   "Loads an ASCII BASIC program into memory.",  /* Description */
   "LOAD",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_LOOP,  /* UniqueID */
   "LOOP [UNTIL|WHILE expression]",  /* Syntax */
   "Bottom of a DO - LOOP structure.  LOOP:  IF the loop is not t"
"erminated by EXIT DO or DO UNTIL or DO WHILE, then it will lo"
"op forever.  LOOP UNTIL:  exits when expression is nonz-zero."
"  LOOP WHILE:  exits when expression is zero.",  /* Description */
   "LOOP",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_LPRINT,  /* UniqueID */
   "LPRINT [USING format-string$;] expressions...",  /* Syntax */
   "Send output to the default printer(stderr).",  /* Description */
   "LPRINT",  /* Name */
   B15 | I70 | D71 | I73 | E78 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_LPRINTER,  /* UniqueID */
   "LPRINTER [WIDTH width]",  /* Syntax */
   "Directs the output of PRINT commands to the printer (stderr),"
" and optionally changes the printer width.",  /* Description */
   "LPRINTER",  /* Name */
   C77  /* OptionVersionBitmask */
},
{
   C_LSET,  /* UniqueID */
   "LSET string-variable$ = expression",  /* Syntax */
   "Left-aligns the value of expression into string-variable$.  I"
"f the length of the value is too short, then it is padded on "
"the right with spaces.  If the length of the value is too lon"
"g, then it is truncated on the right.  This is only for use w"
"ith variables assigned to a random access buffer with FIELD c"
"ommand.",  /* Description */
   "LSET",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_MAINTAINER,  /* UniqueID */
   "MAINTAINER",  /* Syntax */
   "This command is reserved for use by the Bywater BASIC maintai"
"ner and does whatever the maintainer wants it to do.  It is n"
"ot for the BASIC programmer.",  /* Description */
   "MAINTAINER",  /* Name */
   B15  /* OptionVersionBitmask */
},
{
   C_MARGIN,  /* UniqueID */
   "MARGIN # filenumber, width",  /* Syntax */
   "Sets the file margin for writing; filenumber <= 0 is ignored."
"",  /* Description */
   "MARGIN",  /* Name */
   B15 | D64 | G65 | G67 | HB2  /* OptionVersionBitmask */
},
{
   C_MAT,  /* UniqueID */
   "MAT arrayname = expression",  /* Syntax */
   "Matrix operations:  MAT A = CON, MAT A = IDN, MAT A = ZER, MA"
"T A = INV B, MAT A = TRN B, MAT A = (k) * B, MAT A = B, MAT A"
" = B + C, MAT A = B - C, MAT A = B * C. ",  /* Description */
   "MAT",  /* Name */
   D64 | G65 | G67 | I70 | D71 | I73 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_MAT_GET,  /* UniqueID */
   "MAT GET filename$, arrayname",  /* Syntax */
   "Matrix get.",  /* Description */
   "MAT GET",  /* Name */
   G65 | G67 | I70 | I73  /* OptionVersionBitmask */
},
{
   C_MAT_INPUT,  /* UniqueID */
   "MAT INPUT arrayname",  /* Syntax */
   "Matrix input.",  /* Description */
   "MAT INPUT",  /* Name */
   D64 | G65 | G67 | I70 | D71 | I73 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_MAT_PRINT,  /* UniqueID */
   "MAT PRINT arrayname",  /* Syntax */
   "Matrix print.",  /* Description */
   "MAT PRINT",  /* Name */
   D64 | G65 | G67 | I70 | D71 | I73 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_MAT_PUT,  /* UniqueID */
   "MAT PUT filename$, arrayname",  /* Syntax */
   "Matrix put.",  /* Description */
   "MAT PUT",  /* Name */
   G65 | G67 | I70 | I73  /* OptionVersionBitmask */
},
{
   C_MAT_READ,  /* UniqueID */
   "MAT READ arrayname",  /* Syntax */
   "Matrix read.",  /* Description */
   "MAT READ",  /* Name */
   D64 | G65 | G67 | I70 | D71 | I73 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_MAT_WRITE,  /* UniqueID */
   "MAT WRITE arrayname",  /* Syntax */
   "Matrix write.",  /* Description */
   "MAT WRITE",  /* Name */
   D64 | G65 | G67 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_MERGE,  /* UniqueID */
   "MERGE file-name",  /* Syntax */
   "Merges the BASIC program in file-name into the current BASIC "
"program.  Lines in file-name replace any matching lines in th"
"e current program.",  /* Description */
   "MERGE",  /* Name */
   B15 | D71 | C77 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_MID4,  /* UniqueID */
   "MID$( variable$, start [, count ] ) = expression",  /* Syntax */
   "Replaces a subtring of variable$ with expression.",  /* Description */
   "MID$",  /* Name */
   B15 | D71 | C77 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_NAME,  /* UniqueID */
   "NAME old_file_name AS new_file_name",  /* Syntax */
   "Changes the name of an existing file.",  /* Description */
   "NAME",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_NEW,  /* UniqueID */
   "NEW",  /* Syntax */
   "Deletes the program in memory and clears all variables.",  /* Description */
   "NEW",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_NEXT,  /* UniqueID */
   "NEXT [variable]",  /* Syntax */
   "The bottom line of a FOR - NEXT structure.",  /* Description */
   "NEXT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OF,  /* UniqueID */
   "OF line, ...",  /* Syntax */
   "OF by itself is a Syntax Error.  Use GOTO ... OF instead.",  /* Description */
   "OF",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OLD,  /* UniqueID */
   "OLD [file-name$]",  /* Syntax */
   "Loads an ASCII BASIC program into memory.",  /* Description */
   "OLD",  /* Name */
   D64 | G65 | G67 | D71  /* OptionVersionBitmask */
},
{
   C_ON,  /* UniqueID */
   "ON expression GOTO | GOSUB line [, ...]",  /* Syntax */
   "Branches (GOTO) or calls (GOSUB) based on the rounded value o"
"f variable.",  /* Description */
   "ON",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_ON_ERROR,  /* UniqueID */
   "ON ERROR ...",  /* Syntax */
   "ON ERROR  by itself is a syntax error.  Use ON ERROR GOSUB or"
" ON ERROR GOTO instead.",  /* Description */
   "ON ERROR",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_ON_ERROR_GOSUB,  /* UniqueID */
   "ON ERROR GOSUB errline",  /* Syntax */
   "When a trappable error occurs, execute 'GOSUB errline'.  The "
"error handler must terminate with a RETURN command.  If the l"
"ine number is 0 (zerp), then use the default error handler.  "
"Valid when OPTION ERROR GOSUB.",  /* Description */
   "ON ERROR GOSUB",  /* Name */
   B15  /* OptionVersionBitmask */
},
{
   C_ON_ERROR_GOTO,  /* UniqueID */
   "ON ERROR GOTO errline",  /* Syntax */
   "When a trappable error occurs, execute 'GOTO errline'.  The e"
"rror handler must terminate with a RESUME command.  If the li"
"ne number is 0 (zerp), then use the default error handler.  V"
"alid when OPTION ERROR GOTO.",  /* Description */
   "ON ERROR GOTO",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_ON_ERROR_RESUME,  /* UniqueID */
   "ON ERROR RESUME ...",  /* Syntax */
   "ON ERROR RESUME by itself is a syntax error.  Use ON ERROR RE"
"SUME NEXT instead.",  /* Description */
   "ON ERROR RESUME",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_ON_ERROR_RESUME_NEXT,  /* UniqueID */
   "ON ERROR RESUME NEXT",  /* Syntax */
   "When a trappable error occurs, execution continues with the n"
"ext line.  Valid when OPTION ERROR GOTO.",  /* Description */
   "ON ERROR RESUME NEXT",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_ON_ERROR_RETURN,  /* UniqueID */
   "ON ERROR RETURN ...",  /* Syntax */
   "ON ERROR RETURN by itself is a syntax error.  Use ON ERROR RE"
"TURN NEXT instead.",  /* Description */
   "ON ERROR RETURN",  /* Name */
   B15  /* OptionVersionBitmask */
},
{
   C_ON_ERROR_RETURN_NEXT,  /* UniqueID */
   "ON ERROR RETURN NEXT",  /* Syntax */
   "When a trappable error occurs, execution continues with the n"
"ext line.  Valid when OPTION ERROR GOSUB.",  /* Description */
   "ON ERROR RETURN NEXT",  /* Name */
   B15  /* OptionVersionBitmask */
},
{
   C_ON_TIMER,  /* UniqueID */
   "ON TIMER count GOSUB line",  /* Syntax */
   "Specifies a line (or label) to gosub when count seconds have "
"elaspsed after TIMER ON is executed.  The interrupt routine s"
"hould end with a RETURN command.  Timer events only occur in "
"running BASIC programs.  The resolution of the clock is imple"
"mentation defined.",  /* Description */
   "ON TIMER",  /* Name */
   B15 | M88  /* OptionVersionBitmask */
},
{
   C_OPEN,  /* UniqueID */
   "OPEN file-name [FOR INPUT|OUTPUT|APPEND|BINARY|RANDOM] AS [#]"
"device-number [LEN = record-length]",  /* Syntax */
   "Opens a file for use.  INPUT, OUTPUT, and APPEND are for text"
" files.  BINARY is for binary files.  RANDOM is for structure"
"d binary files and requires LEN.  Files opened for RANDOM use"
" GET and PUT.  For CBASIC-II, this opens an existing file and"
" the syntax is OPEN filename$ [ RECL reclen ] AS filenum [ BU"
"FF number ] [ RECS size ].",  /* Description */
   "OPEN",  /* Name */
   B15 | I70 | D71 | I73 | C77 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION,  /* UniqueID */
   "OPTION ...",  /* Syntax */
   "OPTION by itself is a syntax error.  Use one of the many OPTI"
"ON commands instead.",  /* Description */
   "OPTION",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ANGLE,  /* UniqueID */
   "OPTION ANGLE ...",  /* Syntax */
   "OPTION ANGLE by itself is a syntax error.  Use OPTION ANGLE D"
"EGREES, OPTION ANGLE GRADIANS, or OPTION ANGLE RADIANS instea"
"d.",  /* Description */
   "OPTION ANGLE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ANGLE_DEGREES,  /* UniqueID */
   "OPTION ANGLE DEGREES",  /* Syntax */
   "Configures these math functions to accept and return angles i"
"n degrees:  ACOS, ACS, ANGLE, ARCSIN, ASIN, ASN, ARCTAN, ATN,"
" ATAN, COS, COT, CSC, SEC, SIN and TAN.",  /* Description */
   "OPTION ANGLE DEGREES",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ANGLE_GRADIANS,  /* UniqueID */
   "OPTION ANGLE GRADIANS",  /* Syntax */
   "Configures these math functions to accept and return angles i"
"n gradians:  ACOS, ANGLE, ASIN, ASN, ATN, ATAN, COS, COT, CSC"
", SEC, SIN and TAN.",  /* Description */
   "OPTION ANGLE GRADIANS",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ANGLE_RADIANS,  /* UniqueID */
   "OPTION ANGLE RADIANS",  /* Syntax */
   "Configures these math functions to accept and return angles i"
"n radians:  ACOS, ANGLE, ASIN, ASN, ATN, ATAN, COS, COT, CSC,"
" SEC, SIN and TAN.",  /* Description */
   "OPTION ANGLE RADIANS",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ARITHMETIC,  /* UniqueID */
   "OPTION ARITHMETIC",  /* Syntax */
   "OPTION ARITHMETIC by itself is a syntax error.Use OPTION ARIT"
"HMETIC DECIMAL, OPTION ARITHMETIC FIXED, or OPTION ARITHMETIC"
" NATIVE instead.",  /* Description */
   "OPTION ARITHMETIC",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ARITHMETIC_DECIMAL,  /* UniqueID */
   "OPTION ARITHMETIC DECIMAL",  /* Syntax */
   "Currently has no effect.",  /* Description */
   "OPTION ARITHMETIC DECIMAL",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ARITHMETIC_FIXED,  /* UniqueID */
   "OPTION ARITHMETIC FIXED",  /* Syntax */
   "Currently has no effect.",  /* Description */
   "OPTION ARITHMETIC FIXED",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ARITHMETIC_NATIVE,  /* UniqueID */
   "OPTION ARITHMETIC NATIVE",  /* Syntax */
   "Currently has no effect.",  /* Description */
   "OPTION ARITHMETIC NATIVE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_BASE,  /* UniqueID */
   "OPTION BASE X",  /* Syntax */
   "Sets the lowest array subscript to X.",  /* Description */
   "OPTION BASE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_BUGS,  /* UniqueID */
   "OPTION BUGS",  /* Syntax */
   "OPTION BUGS by itself is a syntax error.  Use OPTION BUGS OFF"
" or OPTION BUGS ON instead.",  /* Description */
   "OPTION BUGS",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_BUGS_OFF,  /* UniqueID */
   "OPTION BUGS OFF",  /* Syntax */
   "Disables bugs commonly found in BASIC-80 derivatives and clon"
"es.",  /* Description */
   "OPTION BUGS OFF",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_BUGS_ON,  /* UniqueID */
   "OPTION BUGS ON",  /* Syntax */
   "Enables bugs commonly found in BASIC-80 derivatives and clone"
"s.",  /* Description */
   "OPTION BUGS ON",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_COMMENT,  /* UniqueID */
   "OPTION COMMENT char",  /* Syntax */
   "Sets the comment character.",  /* Description */
   "OPTION COMMENT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_COMPARE,  /* UniqueID */
   "OPTION COMPARE",  /* Syntax */
   "OPTION COMPARE by itself is a syntax error.  Use OPTION COMPA"
"RE BINARY, OPTION COMPARE DATABASE, or OPTION COMPARE TEXT in"
"stead.",  /* Description */
   "OPTION COMPARE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_COMPARE_BINARY,  /* UniqueID */
   "OPTION COMPARE BINARY",  /* Syntax */
   "Causes string comparisons to be case-sensitive.",  /* Description */
   "OPTION COMPARE BINARY",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_COMPARE_DATABASE,  /* UniqueID */
   "OPTION COMPARE DATABASE",  /* Syntax */
   "Causes string comparisons to be case-insensitive.",  /* Description */
   "OPTION COMPARE DATABASE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_COMPARE_TEXT,  /* UniqueID */
   "OPTION COMPARE TEXT",  /* Syntax */
   "Causes string comparisons to be case-insensitive.",  /* Description */
   "OPTION COMPARE TEXT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_COVERAGE,  /* UniqueID */
   "OPTION COVERAGE",  /* Syntax */
   "OPTION COVERAGE by itself is a syntax error.Use OPTION COVERA"
"GE OFF or OPTION COVERAGE ON instead.",  /* Description */
   "OPTION COVERAGE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_COVERAGE_OFF,  /* UniqueID */
   "OPTION COVERAGE OFF",  /* Syntax */
   "Disables BASIC code coverage recording, displayed using the L"
"IST command.",  /* Description */
   "OPTION COVERAGE OFF",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_COVERAGE_ON,  /* UniqueID */
   "OPTION COVERAGE ON",  /* Syntax */
   "Enables BASIC code coverage recording, displayed using the LI"
"ST command.",  /* Description */
   "OPTION COVERAGE ON",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_DATE,  /* UniqueID */
   "OPTION DATE format$",  /* Syntax */
   "Sets the date format string used by C strftime() for DATE$.",  /* Description */
   "OPTION DATE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_DISABLE,  /* UniqueID */
   "OPTION DISABLE",  /* Syntax */
   "OPTION DISABLE by itself is a syntax error.  Use OPTION DISAB"
"LE COMMAND, OPTION DISABLE FUNCTION, or OPTION DISABLE OPERAT"
"OR instead",  /* Description */
   "OPTION DISABLE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_DISABLE_COMMAND,  /* UniqueID */
   "OPTION DISABLE COMMAND name",  /* Syntax */
   "Disables the specified BASIC command.",  /* Description */
   "OPTION DISABLE COMMAND",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_DISABLE_FUNCTION,  /* UniqueID */
   "OPTION DISABLE FUNCTION name",  /* Syntax */
   "Disables the specified BASIC function.",  /* Description */
   "OPTION DISABLE FUNCTION",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_DISABLE_OPERATOR,  /* UniqueID */
   "OPTION DISABLE OPERATOR name",  /* Syntax */
   "Disables the specified BASIC operator.",  /* Description */
   "OPTION DISABLE OPERATOR",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ENABLE,  /* UniqueID */
   "OPTION ENABLE",  /* Syntax */
   "OPTION ENABLE by itself is a syntax error.  Use OPTION ENABLE"
" COMMAND, OPTION ENABLE FUNCTION, or OPTION ENABLE OPERATOR i"
"nstead.",  /* Description */
   "OPTION ENABLE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ENABLE_COMMAND,  /* UniqueID */
   "OPTION ENABLE COMMAND name",  /* Syntax */
   "Enables the specified BASIC command.",  /* Description */
   "OPTION ENABLE COMMAND",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ENABLE_FUNCTION,  /* UniqueID */
   "OPTION ENABLE FUNCTION name",  /* Syntax */
   "Enables the specified BASIC function.",  /* Description */
   "OPTION ENABLE FUNCTION",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ENABLE_OPERATOR,  /* UniqueID */
   "OPTION ENABLE OPERATOR name",  /* Syntax */
   "Enables the specified BASIC operator.",  /* Description */
   "OPTION ENABLE OPERATOR",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ERROR,  /* UniqueID */
   "OPTION ERROR",  /* Syntax */
   "OPTION ERROR by itself is a syntax error.  Use OPTION ERROR G"
"OSUB or OPTION ERROR GOTO instead.",  /* Description */
   "OPTION ERROR",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ERROR_GOSUB,  /* UniqueID */
   "OPTION ERROR GOSUB",  /* Syntax */
   "When an error occurs, GOSUB to the error handler.  The error "
"handler exits with RETURN.",  /* Description */
   "OPTION ERROR GOSUB",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ERROR_GOTO,  /* UniqueID */
   "OPTION ERROR GOTO",  /* Syntax */
   "When an error occurs, GOTO to the error handler.  The error h"
"andler exits with RESUME.",  /* Description */
   "OPTION ERROR GOTO",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_EXPLICIT,  /* UniqueID */
   "OPTION EXPLICIT",  /* Syntax */
   "All variables must be declared using DIM.",  /* Description */
   "OPTION EXPLICIT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_IMAGE,  /* UniqueID */
   "OPTION IMAGE char$",  /* Syntax */
   "Sets the shortcut IMAGE character.",  /* Description */
   "OPTION IMAGE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_IMPLICIT,  /* UniqueID */
   "OPTION IMPLICIT",  /* Syntax */
   "Variables need not be declared using DIM, provided arrays hav"
"e no more that 10 elements.  This is the opposite of OPTION E"
"XPLICIT, and is the default for all versions of BASIC.",  /* Description */
   "OPTION IMPLICIT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_INDENT,  /* UniqueID */
   "OPTION INDENT number",  /* Syntax */
   "Sets indention level for LIST.  Zero means no indention.  Def"
"ault is 2.",  /* Description */
   "OPTION INDENT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_LABELS,  /* UniqueID */
   "OPTION LABELS",  /* Syntax */
   "OPTION LABELS by itself is a syntax error.  Use OPTION LABELS"
" OFF or OPTION LABELS ON instead.",  /* Description */
   "OPTION LABELS",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_LABELS_OFF,  /* UniqueID */
   "OPTION LABELS OFF",  /* Syntax */
   "Disables text labels.",  /* Description */
   "OPTION LABELS OFF",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_LABELS_ON,  /* UniqueID */
   "OPTION LABELS ON",  /* Syntax */
   "Enables text labels.",  /* Description */
   "OPTION LABELS ON",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_PRINT,  /* UniqueID */
   "OPTION PRINT char$",  /* Syntax */
   "Sets the shortcut PRINT character.",  /* Description */
   "OPTION PRINT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ROUND,  /* UniqueID */
   "OPTION ROUND",  /* Syntax */
   "OPTION ROUND by itself is a syntax error.  Use OPTION ROUND B"
"ANK, OPTION ROUND MATH, or OPTION ROUND TRUNCATE instead.",  /* Description */
   "OPTION ROUND",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ROUND_BANK,  /* UniqueID */
   "OPTION ROUND BANK",  /* Syntax */
   "Round using the Banker's rule.",  /* Description */
   "OPTION ROUND BANK",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ROUND_MATH,  /* UniqueID */
   "OPTION ROUND MATH",  /* Syntax */
   "Round using mathematical rules.",  /* Description */
   "OPTION ROUND MATH",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_ROUND_TRUNCATE,  /* UniqueID */
   "OPTION ROUND TRUNCATE",  /* Syntax */
   "Round using truncation.",  /* Description */
   "OPTION ROUND TRUNCATE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_SLEEP,  /* UniqueID */
   "OPTION SLEEP number",  /* Syntax */
   "Sets multiplier for SLEEP and WAIT.  Zero means no waiting.  "
"Default is 1.",  /* Description */
   "OPTION SLEEP",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_STATEMENT,  /* UniqueID */
   "OPTION STATEMENT char$",  /* Syntax */
   "Sets the statement seperator character.",  /* Description */
   "OPTION STATEMENT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_STDERR,  /* UniqueID */
   "OPTION STDERR filename$",  /* Syntax */
   "Sets the file used for STDERR, which is used by LPRINT comman"
"ds.",  /* Description */
   "OPTION STDERR",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_STDIN,  /* UniqueID */
   "OPTION STDIN filename$",  /* Syntax */
   "Sets the file used for STDIN, which is used by INPUT commands"
".",  /* Description */
   "OPTION STDIN",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_STDOUT,  /* UniqueID */
   "OPTION STDOUT filename$",  /* Syntax */
   "Sets the file used for STDOUT, which is used by PRINT command"
"s.",  /* Description */
   "OPTION STDOUT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_STRICT,  /* UniqueID */
   "OPTION STRICT",  /* Syntax */
   "OPTION STRICT by itself is a syntax error.  Use OPTION STRICT"
" OFF or OPTION STRICT ON instead.",  /* Description */
   "OPTION STRICT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_STRICT_OFF,  /* UniqueID */
   "OPTION STRICT OFF",  /* Syntax */
   "Disables checking for implicit array creation without using t"
"he DIM command.",  /* Description */
   "OPTION STRICT OFF",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_STRICT_ON,  /* UniqueID */
   "OPTION STRICT ON",  /* Syntax */
   "Enables checking for implicit array creation without using th"
"e DIM command.",  /* Description */
   "OPTION STRICT ON",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_TERMINAL,  /* UniqueID */
   "OPTION TERMINAL",  /* Syntax */
   "OPTION TERMINAL by itself is a syntax error.  Use OPTION TERM"
"INAL ADM, OPTION TERMINAL ANSI, or OPTION TERMINAL NONE inste"
"ad.",  /* Description */
   "OPTION TERMINAL",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_TERMINAL_ADM,  /* UniqueID */
   "OPTION TERMINAL ADM",  /* Syntax */
   "Enables ADM-3A terminal control codes for CLS, COLOR, and LOC"
"ATE.",  /* Description */
   "OPTION TERMINAL ADM",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_TERMINAL_ANSI,  /* UniqueID */
   "OPTION TERMINAL ANSI",  /* Syntax */
   "Enables ANSI terminal control codes for CLS, COLOR, and LOCAT"
"E.",  /* Description */
   "OPTION TERMINAL ANSI",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_TERMINAL_NONE,  /* UniqueID */
   "OPTION TERMINAL NONE",  /* Syntax */
   "Disables terminal control codes for CLS, COLOR, and LOCATE.",  /* Description */
   "OPTION TERMINAL NONE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_TIME,  /* UniqueID */
   "OPTION TIME format$",  /* Syntax */
   "Sets the time format string used by C strftime() for TIME$.",  /* Description */
   "OPTION TIME",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_TRACE,  /* UniqueID */
   "OPTION TRACE",  /* Syntax */
   "OPTION TRACE by itself is a syntax error.  Use OPTION TRACE O"
"FF or OPTION TRACE ON instead.",  /* Description */
   "OPTION TRACE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_TRACE_OFF,  /* UniqueID */
   "OPTION TRACE OFF",  /* Syntax */
   "Disables displaying a stack trace when an ERROR occurs.",  /* Description */
   "OPTION TRACE OFF",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_TRACE_ON,  /* UniqueID */
   "OPTION TRACE ON",  /* Syntax */
   "Enables displaying a stack trace when an ERROR occurs.",  /* Description */
   "OPTION TRACE ON",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_USING,  /* UniqueID */
   "OPTION USING string$",  /* Syntax */
   "Specifies the magic characters for the PRINT USING command.  "
"A common value for string$ is '#,.+-^$*_!&%'.  Please examine"
" the source code for bwb_OPTION_USING for details regarding t"
"he meaning of each character position.",  /* Description */
   "OPTION USING",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_OPTION_VERSION,  /* UniqueID */
   "OPTION VERSION [version]",  /* Syntax */
   "Selects a combination of OPTION settings and enables certain "
"commands and functions.  If no version is specified, displays"
" a list of the available versions.",  /* Description */
   "OPTION VERSION",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_PAUSE,  /* UniqueID */
   "PAUSE [comment]",  /* Syntax */
   "Pauses processing until the ENTER key is presseed.",  /* Description */
   "PAUSE",  /* Name */
   I70 | I73  /* OptionVersionBitmask */
},
{
   C_POP,  /* UniqueID */
   "POP",  /* Syntax */
   "Pops one GOSUB from the return stack.",  /* Description */
   "POP",  /* Name */
   B15 | HB2  /* OptionVersionBitmask */
},
{
   C_PRINT,  /* UniqueID */
   "PRINT [# device-number,][USING format$;] expressions ...",  /* Syntax */
   "Sends output to a file.  #0 is the console(stdout).  #-1 goes"
" to the printer(stderr).",  /* Description */
   "PRINT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_PUT,  /* UniqueID */
   "PUT filename$ , value [, ...]",  /* Syntax */
   "Puts one (or more) values into a file.",  /* Description */
   "PUT",  /* Name */
   I70 | D71 | I73  /* OptionVersionBitmask */
},
{
   C_QUIT,  /* UniqueID */
   "QUIT",  /* Syntax */
   "Exits to the operating system.",  /* Description */
   "QUIT",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | E86  /* OptionVersionBitmask */
},
{
   C_READ,  /* UniqueID */
   "READ variable [, ...]",  /* Syntax */
   "Reads values from DATA statements.",  /* Description */
   "READ",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_RECALL,  /* UniqueID */
   "RECALL ArrayName",  /* Syntax */
   "Loads a numeric array from a file saved using STORE.",  /* Description */
   "RECALL",  /* Name */
   B15 | HB2  /* OptionVersionBitmask */
},
{
   C_REM,  /* UniqueID */
   "REM ...",  /* Syntax */
   "Remark.",  /* Description */
   "REM",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_RENAME,  /* UniqueID */
   "RENAME [file-name$]",  /* Syntax */
   "Changes the file name which will be used by SAVE.  Does not s"
"ave the file.",  /* Description */
   "RENAME",  /* Name */
   D64 | G65 | G67 | D71  /* OptionVersionBitmask */
},
{
   C_RENUM,  /* UniqueID */
   "RENUM",  /* Syntax */
   "Implementation defined.",  /* Description */
   "RENUM",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_RENUMBER,  /* UniqueID */
   "RENUMBER",  /* Syntax */
   "Implementation defined.",  /* Description */
   "RENUMBER",  /* Name */
   HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_RESET,  /* UniqueID */
   "RESET filename$ [, ...]",  /* Syntax */
   "Positions an input or output file to the bcginning.",  /* Description */
   "RESET",  /* Name */
   I70 | I73  /* OptionVersionBitmask */
},
{
   C_RESTORE,  /* UniqueID */
   "RESTORE [line]",  /* Syntax */
   "Resets the line used for the next READ statement.  'line' may"
" be either a number or a label.",  /* Description */
   "RESTORE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_RESUME,  /* UniqueID */
   "RESUME [ 0 | line | NEXT ]",  /* Syntax */
   "Used in an error handler to specify the next line to execute."
"  RESUME and RESUME 0 branch to ERL.  RESUME NEXT branches to"
" the line after ERL.  RESUME line branches to the specified l"
"ine.  'line' may be either a number or a label.",  /* Description */
   "RESUME",  /* Name */
   B15 | D71 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_RETURN,  /* UniqueID */
   "RETURN",  /* Syntax */
   "Concludes a subroutine called by GOSUB.",  /* Description */
   "RETURN",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_RSET,  /* UniqueID */
   "RSET string-variable$ = expression",  /* Syntax */
   "Right-aligns the value of expression into string-variable$.  "
"If the length of the value is too short, then it is padded on"
" the left with spaces.  If the length of the value is too lon"
"g, then it is truncated on the right.  This is only for use w"
"ith variables assigned to a random access buffer with FIELD c"
"ommand.",  /* Description */
   "RSET",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_RUN,  /* UniqueID */
   "RUN [line | file-name$]",  /* Syntax */
   "RUN executes the program in memory from the start.  RUN line "
"exexecutes the program in memory beginning at 'line'.  RUN fi"
"le-name$ loads a new BAASIC program and executes the program "
"from the start.",  /* Description */
   "RUN",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_SAVE,  /* UniqueID */
   "SAVE [file-name$]",  /* Syntax */
   "Saves the current program into the file file-name$ in ASCII f"
"ormat.",  /* Description */
   "SAVE",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_SCRATCH,  /* UniqueID */
   "SCRATCH # X",  /* Syntax */
   "Sets the file mode to writing.",  /* Description */
   "SCRATCH",  /* Name */
   G65 | G67 | HB2  /* OptionVersionBitmask */
},
{
   C_SELECT,  /* UniqueID */
   "SELECT",  /* Syntax */
   "SELECT by itself is a syntax error.  Use SELECT CASE instead."
"",  /* Description */
   "SELECT",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_SELECT_CASE,  /* UniqueID */
   "SELECT CASE expression",  /* Syntax */
   "Introduces a multi-line conditional selection statement.",  /* Description */
   "SELECT CASE",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_STEP,  /* UniqueID */
   "STEP",  /* Syntax */
   "STEP by itself is a syntax error.  Use FOR ... STEP instead.",  /* Description */
   "STEP",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_STOP,  /* UniqueID */
   "STOP",  /* Syntax */
   "Interrupts program execution and displays the line number of "
"the STOP command.  For use when debugging BASIC programs.  Wh"
"ether STOP issues a SIGINT signal is implementation defined.",  /* Description */
   "STOP",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_STORE,  /* UniqueID */
   "STORE ArrayName",  /* Syntax */
   "Saves a numeric array into a file for later loading by RECALL"
".",  /* Description */
   "STORE",  /* Name */
   B15 | HB2  /* OptionVersionBitmask */
},
{
   C_SUB,  /* UniqueID */
   "SUB name [ ( parameter [,...] ) ]",  /* Syntax */
   "Top line of a multi-line SUB definition.  The variable names "
"specified are local to the SUB definition, and are initialize"
"d BYVAL when the subroutine is invoked by another routine.",  /* Description */
   "SUB",  /* Name */
   B15 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_SUBEND,  /* UniqueID */
   "SUBEND",  /* Syntax */
   "Specifies the last line of a multi-line SUB definition.  Same"
" as END SUB.",  /* Description */
   "SUBEND",  /* Name */
   B15 | HB2  /* OptionVersionBitmask */
},
{
   C_SWAP,  /* UniqueID */
   "SWAP variable, variable",  /* Syntax */
   "Swaps the values of two variables. Both variables must be of "
"the same type.",  /* Description */
   "SWAP",  /* Name */
   B15 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_SYSTEM,  /* UniqueID */
   "SYSTEM",  /* Syntax */
   "Exits to the operating system.",  /* Description */
   "SYSTEM",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_TEXT,  /* UniqueID */
   "TEXT letter[-letter] [, ...]",  /* Syntax */
   "Declares variables with single-letter names as string variabl"
"es.  Same as DEFSTR.  TODO: enforce the range of [MINLEN,MAXL"
"EN].",  /* Description */
   "TEXT",  /* Name */
   HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_THEN,  /* UniqueID */
   "THEN line1",  /* Syntax */
   "THEN by itself is a syntax error.  Use IF ... THEN line1 inst"
"ead.",  /* Description */
   "THEN",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_TIMER,  /* UniqueID */
   "TIMER",  /* Syntax */
   "TIMER by itself is a syntax error.  Use TIMER OFF, TIMER ON, "
"or TIMER STOP instead.",  /* Description */
   "TIMER",  /* Name */
   B15 | M88  /* OptionVersionBitmask */
},
{
   C_TIMER_OFF,  /* UniqueID */
   "TIMER OFF",  /* Syntax */
   "TIMER OFF terminates the timer interrupt.",  /* Description */
   "TIMER OFF",  /* Name */
   B15 | M88  /* OptionVersionBitmask */
},
{
   C_TIMER_ON,  /* UniqueID */
   "TIMER ON",  /* Syntax */
   "TIMER ON enables the timer interrupt.  When the specified sec"
"onds have elapsed, TIMER STOP is internaly executed before th"
"e interrupt is taken.  TIMER ON should be executed just befor"
"e the RETURN command if you want the interrupt to occur again"
".",  /* Description */
   "TIMER ON",  /* Name */
   B15 | M88  /* OptionVersionBitmask */
},
{
   C_TIMER_STOP,  /* UniqueID */
   "TIMER STOP",  /* Syntax */
   "TIMER STOP disables the interrupt, but the count continues.",  /* Description */
   "TIMER STOP",  /* Name */
   B15 | M88  /* OptionVersionBitmask */
},
{
   C_TLOAD,  /* UniqueID */
   "TLOAD [file-name$]",  /* Syntax */
   "Loads an ASCII BASIC program into memory.",  /* Description */
   "TLOAD",  /* Name */
   B15 | HB2  /* OptionVersionBitmask */
},
{
   C_TO,  /* UniqueID */
   "TO",  /* Syntax */
   "TO by itself is a syntax error.  Use FOR ... TO instead.",  /* Description */
   "TO",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_TRACE,  /* UniqueID */
   "TRACE ON | OFF",  /* Syntax */
   "TRACE enables tracing.",  /* Description */
   "TRACE",  /* Name */
   G67 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_TRACE_OFF,  /* UniqueID */
   "TRACE OFF",  /* Syntax */
   "Disables tracing.",  /* Description */
   "TRACE OFF",  /* Name */
   G67 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_TRACE_ON,  /* UniqueID */
   "TRACE ON",  /* Syntax */
   "TRACE ON enables tracing.",  /* Description */
   "TRACE ON",  /* Name */
   G67 | HB1 | HB2  /* OptionVersionBitmask */
},
{
   C_TSAVE,  /* UniqueID */
   "TSAVE [file-name$]",  /* Syntax */
   "Saves the current program into the file file-name$ in ASCII f"
"ormat.",  /* Description */
   "TSAVE",  /* Name */
   B15 | HB2  /* OptionVersionBitmask */
},
{
   C_UEND,  /* UniqueID */
   "UEND",  /* Syntax */
   "Bottom of a UNTIL - UEND structure.",  /* Description */
   "UEND",  /* Name */
   B15 | HB2  /* OptionVersionBitmask */
},
{
   C_UNTIL,  /* UniqueID */
   "UNTIL expression",  /* Syntax */
   "Top of a UNTIL - UEND structure.  If the expression is zero, "
"then the loop is terminated.",  /* Description */
   "UNTIL",  /* Name */
   B15 | HB2  /* OptionVersionBitmask */
},
{
   C_USE,  /* UniqueID */
   "USE parameter$",  /* Syntax */
   "Receives the value from the calling CHAINing program.",  /* Description */
   "USE",  /* Name */
   I70 | I73  /* OptionVersionBitmask */
},
{
   C_USER_LBL,  /* UniqueID */
   "USER LBL",  /* Syntax */
   "An internal command that allows the BASIC program to find a u"
"ser label.  You do not directly call this command.  If your p"
"rogram has a line with a label, then USER LBL is used to mark"
" the line.",  /* Description */
   "USER LBL",  /* Name */
   B15 | D64 | G65 | G67 | I70 | D71 | I73 | C77 | E78 | HB1 | HB2 | T80 | M80 | M85 | E86 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_VARS,  /* UniqueID */
   "VARS",  /* Syntax */
   "Prints a list of all global variables.",  /* Description */
   "VARS",  /* Name */
   B15  /* OptionVersionBitmask */
},
{
   C_WEND,  /* UniqueID */
   "WEND",  /* Syntax */
   "Bottom of a WHILE - WEND structure.",  /* Description */
   "WEND",  /* Name */
   B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_WHILE,  /* UniqueID */
   "WHILE",  /* Syntax */
   "Top of a WHILE - WEND structure.  If the expression is non-ze"
"ro, then the loop is terminated.",  /* Description */
   "WHILE",  /* Name */
   B15 | D71 | C77 | HB1 | HB2 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
{
   C_WRITE,  /* UniqueID */
   "WRITE [# device-number,] element [, .... ]",  /* Syntax */
   "Outputs variables to the screen or to a file.",  /* Description */
   "WRITE",  /* Name */
   B15 | G65 | G67 | D71 | T80 | M80 | M85 | M88 | VBX  /* OptionVersionBitmask */
},
};


/* EOF */
