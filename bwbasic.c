/***************************************************************
  
        bwbasic.c       Main Program File
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

GlobalType * My = NULL;

static char * DefaultErrorMessage[ 80 ] = 
{
   /* 00 */ ""
,  /* 01 */ "NEXT without FOR"
,  /* 02 */ "Syntax error"
,  /* 03 */ "RETURN without GOSUB"
,  /* 04 */ "Out of DATA"
,  /* 05 */ "Illegal function call"
,  /* 06 */ "Overflow"
,  /* 07 */ "Out of memory"
,  /* 08 */ "Undefined line"
,  /* 09 */ "Subscript out of range"
,  /* 10 */ "Redimensioned array"
,  /* 11 */ "Division by zero"
,  /* 12 */ "Illegal direct"
,  /* 13 */ "Type mismatch"
,  /* 14 */ "Out of string space"
,  /* 15 */ "String too long"
,  /* 16 */ "String formula too complex"
,  /* 17 */ "Can't continue"
,  /* 18 */ "Undefined user function"
,  /* 19 */ "No RESUME"
,  /* 20 */ "RESUME without error"
,  /* 21 */ "Unprintable error"
,  /* 22 */ "Missing operand"
,  /* 23 */ "Line buffer overflow"
,  /* 24 */ ""
,  /* 25 */ ""
,  /* 26 */ "FOR without NEXT"
,  /* 27 */ ""
,  /* 28 */ ""
,  /* 29 */ "WHILE without WEND"
,  /* 30 */ "WEND without WHILE"
,  /* 31 */ "EXIT FUNCTION without FUNCTION"
,  /* 32 */ "END FUNCTION without FUNCTION"
,  /* 33 */ "EXIT SUB without SUB"
,  /* 34 */ "END SUB without SUB"
,  /* 35 */ "EXIT FOR without FOR"
,  /* 36 */ ""
,  /* 37 */ ""
,  /* 38 */ ""
,  /* 39 */ ""
,  /* 40 */ ""
,  /* 41 */ ""
,  /* 42 */ ""
,  /* 43 */ ""
,  /* 44 */ ""
,  /* 45 */ ""
,  /* 46 */ ""
,  /* 47 */ ""
,  /* 48 */ ""
,  /* 49 */ ""
,  /* 50 */ "Field overflow"
,  /* 51 */ "Internal error"
,  /* 52 */ "Bad file number"
,  /* 53 */ "File not found"
,  /* 54 */ "Bad file mode"
,  /* 55 */ "File already open"
,  /* 56 */ ""
,  /* 57 */ "Disk I/O error"
,  /* 58 */ "File already exists"
,  /* 59 */ ""
,  /* 60 */ ""
,  /* 61 */ "Disk full"
,  /* 62 */ "Input past end"
,  /* 63 */ "Bad record number"
,  /* 64 */ "Bad file name"
,  /* 65 */ ""
,  /* 66 */ "Direct statement in file"
,  /* 67 */ "Too many files"
,  /* 68 */ ""
,  /* 69 */ ""
,  /* 70 */ "Variable Not Declared"
,  /* 71 */ ""
,  /* 72 */ ""
,  /* 73 */ "Advanced Feature"
,  /* 74 */ ""
,  /* 75 */ ""
,  /* 76 */ ""
,  /* 77 */ ""
,  /* 78 */ ""
,  /* 79 */ ""
};


static char    *Banner[] = {
             "########  ##    ## ##      ##    ###    ######## ######## ########            ",
             "##     ##  ##  ##  ##  ##  ##   ## ##      ##    ##       ##     ##           ",
             "##     ##   ####   ##  ##  ##  ##   ##     ##    ##       ##     ##           ",
             "########     ##    ##  ##  ## ##     ##    ##    ######   ########            ",
             "##     ##    ##    ##  ##  ## #########    ##    ##       ##   ##             ",
             "##     ##    ##    ##  ##  ## ##     ##    ##    ##       ##    ##            ",
             "########     ##     ###  ###  ##     ##    ##    ######## ##     ##           ",
             "                                                                              ",
             "                                                                              ",
             "                                    ########     ###     ######  ####  ###### ",
             "                                    ##     ##   ## ##   ##    ##  ##  ##    ##",
             "                                    ##     ##  ##   ##  ##        ##  ##      ",
             "                                    ########  ##     ##  ######   ##  ##      ",
             "                                    ##     ## #########       ##  ##  ##      ",
             "                                    ##     ## ##     ## ##    ##  ##  ##    ##",
             "                                    ########  ##     ##  ######  ####  ###### ",
             "                                                                              ",
             "Bywater BASIC Interpreter, version 3.10                                       ",
             "Copyright (c) 1993, Ted A. Campbell                                           ",
             "Copyright (c) 1995-1997, Jon B. Volkoff                                       ",
             "Copyright (c) 2014-2016, Howard Wulf, AF5NE                                   ",
             "                                                                              ",
             NULL
};


int
bwx_DEBUG(const char *A)
{
   /* 
   ** -- RECURSION WARNING -- 
   ** -- DO NOT SEND IT TO BACK TO BWBASIC --  
   ** This is DEBUG output.  
   ** Send it to a file, OutputDebugString, or fputs() 
   */
   if (My->MaintainerDebugOn)
   {
      if (My->MaintainerDebugFile == NULL)
      {
         My->MaintainerDebugFile = fopen(DEBUGFILENAME, "w");
      }
      if (My->MaintainerDebugFile != NULL)
      {
         fputs(A, My->MaintainerDebugFile);
         fputs("\n", My->MaintainerDebugFile);
         fflush(My->MaintainerDebugFile);
      }
      else
      {
         /* unable to open DEBUG file, so send the message to My->SYSOUT->cfp */
         if( My->SYSOUT != NULL )
         {
            if( My->SYSOUT->cfp != NULL )
            {
               fputs( A, My->SYSOUT->cfp );
               fputs( "\n", My->SYSOUT->cfp );
               fflush( My->SYSOUT->cfp );
            }
         }
      }
   }
   return 0;
}


/***************************************************************
  
      FUNCTION:       bwx_terminate()
  
   DESCRIPTION:    This function terminates program execution.
  
***************************************************************/

void
bwx_terminate(void)
{
   bwx_DEBUG(__FUNCTION__);
   exit(0);
}



/***************************************************************
  
   FUNCTION:       break_handler()
  
   DESCRIPTION:    This function is called by break_mes()
         and handles program interruption by break
         (or by the STOP command).
  
***************************************************************/

void
break_handler(void)
{
   bwx_DEBUG(__FUNCTION__);

   My->AutomaticLineNumber = 0;
   My->AutomaticLineIncrement = 0;

   if (My->IsInteractive)
   {
      /* INTERACTIVE: terminate program */

      /* reset all stack counters */
      bwb_clrexec();
      SetOnError(0);

      My->err_number = -1; /* in break_handler() */


      /* reset the break handler */
      signal(SIGINT, break_mes);

      longjmp(My->mark, -1);


      return;
   }
   /* NOT INTERACTIVE:  terminate immediately */
   bwx_terminate();
}

/***************************************************************
  
   FUNCTION:       break_mes()
  
   DESCRIPTION:    This function is called (a) by a SIGINT
         signal or (b) by bwb_STOP via bwx_STOP.
         It prints an error message then calls
         break_handler() to terminate the program.
  
***************************************************************/

void
break_mes(int x /* Parameter 'x' is never used */ )
{
   /* break_mes() is FATAL */
   bwx_DEBUG(__FUNCTION__);
   if (My->err_number < 0)  /* do not make a bad situation worse */
   {
      /* an error has already ben reported */
   }
   else
   {
      prn_xprintf("\n");
      if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
      {
         if( is_empty_filename( My->progfile ) == FALSE )
         {
            prn_xprintf("FILE:");
            prn_xprintf(My->progfile);
            prn_xprintf(", ");
         }
      }
      prn_xprintf("Program interrupted at line");
      if( My->ThisLine != NULL )
      {
         if ( BasicLineNumberMin <= My->ThisLine->number &&  My->ThisLine->number <= BasicLineNumberMax )
         {
            char            tbuf[33];
            sprintf(tbuf, "%d", My->ThisLine->number);
            prn_xprintf(" ");
            prn_xprintf(tbuf);
         }
      }
      prn_xprintf("\n");
   }
   break_handler();
}

void
bwx_STOP(void)
{
   bwx_DEBUG(__FUNCTION__);
   break_mes(0);
}


/***************************************************************
  
      FUNCTION:       bwx_signon()
  
   DESCRIPTION:    This function prints out the sign-on
         message for bwBASIC.
  
***************************************************************/


int
bwx_signon(void)
{

   /* NOT called if a file is provided on the command line */
   int             i;
   bwx_DEBUG(__FUNCTION__);
   for (i = 0; Banner[i] != NULL; i++)
   {
      prn_xprintf(Banner[i]);
      prn_xprintf("\n");
   }

   return TRUE;

}



BasicNumberType
bwx_TIMER(BasicNumberType Seconds)
{
   /* Return a number representing Seconds in the future.  Seconds >= 0.
    * Seconds may be non-integer, such as 0.001 or 86399.999. The
    * maximum allowed Seconds is DBL_MAX. This is used two ways: 1) in
    * bwb_TIMER(), the ON TIMER count (>0)  is used to determine the
    * expiration time. In this case, simply return what the value will
    * be in the future. Note that ON TIMER enforces Seconds > (1 /
    * CLOCKS_PER_SEC) 2) in bwb_execline(), zero (=0) is used to
    * determine the current time and compare it to #1. In this case,
    * simply return what the value is now. Both the resolution of the
    * timer, and frequency of update, are implementation defined. */
   bwx_DEBUG(__FUNCTION__);
   if (Seconds < 0)
   {
      WARN_INTERNAL_ERROR;
      return 0;
   }
   else
   {
      BasicNumberType Result;
      Result = clock();
      Result /= CLOCKS_PER_SEC;
      Result += Seconds;
      return Result;
   }
}

void
CleanLine(char *buffer)
{
   /* cleanup the line, so it is easier to parse */
   char           *newbuffer;

   bwx_DEBUG(__FUNCTION__);


   if( buffer == NULL )
   {
      /* do nothing */
      return;
   }
   if( buffer[0] == BasicNulChar )
   {
      /* do nothing */
      return;
   }
      
   /* remove CR/LF */
   newbuffer = bwb_strchr( buffer, '\r' );
   if( newbuffer != NULL )
   {
      *newbuffer = BasicNulChar;
   }
   newbuffer = bwb_strchr( buffer, '\n' );
   if( newbuffer != NULL )
   {
      *newbuffer = BasicNulChar;
   }
   
   /* remove ALL embedded control characters */
   /* if you want a control character, then use CHR$ */
   newbuffer = buffer;
   while (*newbuffer != BasicNulChar)
   {
      if( bwb_isprint( *newbuffer ) )
      {
         /* OK */
      }
      else
      {
         *newbuffer = ' ';
      }
      newbuffer++;
   }
   /* LTRIM$ */
   newbuffer = buffer;
   if (*newbuffer != BasicNulChar)
   {
      /* not an empty line, so remove one (or more) leading spaces */
      while (*newbuffer == ' ')
      {
         newbuffer++;
      }
      if (newbuffer > buffer)
      {
         bwb_strcpy(buffer, newbuffer);
      }
   }
   /* RTRIM$ */
   newbuffer = buffer;
   if (*newbuffer != BasicNulChar)
   {
      /* not an empty line, so remove one (or more) trailing spaces */
      char           *E;

      E = bwb_strchr(newbuffer, BasicNulChar);
      E--;
      while (E >= newbuffer && *E == ' ')
      {
         *E = BasicNulChar;
         E--;
      }
   }
}


/***************************************************************
  
   FUNCTION:       bwb_init()
  
   DESCRIPTION:    This function initializes bwBASIC.
  
***************************************************************/

void
bwb_init( void )
{
   register int    n;
   static char     start_buf[] = "\0";
   static char     end_buf[] = "\0";
   bwx_DEBUG(__FUNCTION__);


   My->IsCommandLineFile = FALSE;
   My->ExternalInputFile = NULL; /* for automated testing, TAPE command */
   My->IsPrinter = FALSE; /* CBASIC-II: CONSOLE and LPRINTER commands */

    bwb_strcpy( My->progfile, "" );
    
    bwb_memset( &My->bwb_start, 0, sizeof(LineType) );
    bwb_memset( &My->bwb_end  , 0, sizeof(LineType) );
    bwb_memset( &My->user_line, 0, sizeof(LineType) );


   My->bwb_start.number = BasicLineNumberMin - 1;
   My->bwb_start.next = &My->bwb_end;
   My->bwb_start.position = 0;
   My->bwb_start.buffer = start_buf;

   My->bwb_end.number = BasicLineNumberMax + 1;
   My->bwb_end.next = &My->bwb_end;
   My->bwb_end.position = 0;
   My->bwb_end.buffer = end_buf;

   My->user_line.number = BasicLineNumberMin - 1;
   My->user_line.next = &My->bwb_end;
   My->user_line.position = 0;
   My->user_line.buffer = NULL;

   My->data_line = &My->bwb_start;
   My->data_pos = 0;

   My->stack_head = NULL;
   My->stack_level = 0;

   My->field_head = NULL;

#if NEW_VIRTUAL
   My->virtual_head = NULL;
#endif /* NEW_VIRTUAL */
   My->ThisLine = &My->bwb_start;

    
   SortAllCommands();
   SortAllFunctions();
   SortAllOperators();


   /* Memory allocation */
   if ((My->bwb_ebuf = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_init")) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return;
   }
   if ((My->read_line = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_init")) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return;
   }
   if ((My->SYSIN = CALLOC(1, sizeof(FileType), "bwb_init")) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return;
   }
   if ((My->SYSOUT = CALLOC(1, sizeof(FileType), "bwb_init")) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return;
   }
   if ((My->SYSPRN = CALLOC(1, sizeof(FileType), "bwb_init")) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return;
   }

   /* initialize tables of variables, functions */
   var_init();
   fnc_init();
   fslt_init();
   OptionVersionSet(0);

   My->SYSIN->mode = DEVMODE_INPUT;
   My->SYSIN->width = 80;
   My->SYSIN->col = 1;
   My->SYSIN->row = 1;
   My->SYSIN->delimit = ',';
   My->SYSIN->cfp = stdin;

   My->SYSOUT->mode = DEVMODE_OUTPUT;
   My->SYSOUT->width = 80;
   My->SYSOUT->col = 1;
   My->SYSOUT->row = 1;
   My->SYSOUT->delimit = ',';
   My->SYSOUT->cfp = stdout;

   My->SYSPRN->mode = DEVMODE_OUTPUT;
   My->SYSPRN->width = 80;
   My->SYSPRN->col = 1;
   My->SYSPRN->row = 1;
   My->SYSPRN->delimit = ',';
   My->SYSPRN->cfp = stderr;


   My->LPRINT_NULLS = 0;
   My->SCREEN_ROWS = 24;

   /* OPEN #0 is an ERROR. */
   /* CLOSE #0 is an ERROR. */
   /* WIDTH #0, 80 is the same as WIDTH 80. */
   /* LPRINT and LLIST are sent to bwx_LPRINT() */

   /* default variable type */
   for (n = 0; n < 26; n++)
   {
      My->DefaultVariableType[n] = BasicDoubleSuffix;
   }
   /* default COMMAND$(0-9) */   
   for( n = 0; n < 10; n++ )
   {
      My->COMMAND5[n] = NULL;
   } 
}


void strupper(char *C)
{
   if (C == NULL)
   {
      return;
   }
   while (*C)
   {
      *C = bwb_toupper(*C);
      C++;
   }
}

void strlower(char *C)
{
   if (C == NULL)
   {
      return;
   }
   while (*C)
   {
      *C = bwb_tolower(*C);
      C++;
   }
}

/***************************************************************
  
        FUNCTION:       main()
  
        DESCRIPTION:    As in any C program, main() is the basic
                        function from which the rest of the
                        program is called. Some environments,
         however, provide their own main() functions
         (Microsoft Windows (tm) is an example).
         In these cases, the following code will
         have to be included in the initialization
         function that is called by the environment.
  
***************************************************************/

void
bwb_single_step( char * buffer )
{
   bwx_DEBUG(__FUNCTION__);

   CleanLine(buffer);
   if ( buffer[0] == BasicNulChar )
   {
      /* empty -- do nothing */
   }
   else
   if (is_ln(buffer) == FALSE)
   {
      /* If there is no line number, then execute the line as received */
      /* RUN */
      bwb_Warning_Clear();
      SetOnError(0);
      bwb_xtxtline(buffer);
   }
   else
   if (is_numconst(buffer) == TRUE)
   {
      /*-----------------------------------------------------------------*/
      /* Another possibility: if buffer is a numeric constant,           */
      /* then delete the indicated line number (JBV)                     */
      /*-----------------------------------------------------------------*/
      /* DELETE ... */
      int             LineNumber;

      LineNumber = atoi(buffer);
      bwb_Warning_Clear();
      SetOnError(0);
      sprintf(buffer, "delete %d", LineNumber);
      bwb_xtxtline(buffer);
   }
   else
   {
      /* If there is a line number, then add it to the BASIC program */
      /* 100 REM */
      bwb_ladd(buffer, &My->bwb_start);
   }

   while( My->stack_head != NULL )
   {
      bwb_execline();
   }
}


static void execute_profile( char * FileName /*, int IsRequired */  )
{
   FILE           *profile;

   My->NextValidLineNumber = BasicLineNumberMin;   
   profile = fopen( FileName, "r" );
   if ( profile == NULL )
   {
      /* NOT FOUND */
      /* OPTIONAL */
      return;
   }
   /* FOUND */
   if (My->IsInteractive)
   {
      /* set a buffer for jump: program execution returns to this point in
       * case of a jump (error, interrupt, or finish program) */
      My->program_run = 0;
      signal(SIGINT, break_mes);
      setjmp(My->mark);
      if (My->program_run > 0)
      {
         /* error in PROFILE */
         exit( 1 );
      }
      My->program_run++;
   }

   /* 
   The profile only exists to allow 
   setting the BWB... variables 
   and executing OPTION ... commands
   No other use is supported. 
   */
   {
      int             Loop;
   
      Loop = TRUE;
      if (feof(profile))
      {
         Loop = FALSE;
      }
      while (Loop == TRUE)
      {
         My->read_line[0] = BasicNulChar;
         fgets(My->read_line, BasicStringLengthMax, profile);
   
         /* be sure that this is not EOF with a NULL line */
         if (feof(profile))
         {
            Loop = FALSE;
         }
         else
         {
            bwb_single_step(My->read_line);
         }
      }
      fclose( profile ); /* profile != NULL */
      profile = NULL;

   }
}

static void mark_preset_variables( void )
{
   /* mark all existing variables as preset */
   /* this includes the BWB.* variables and all variables declared in any PROFILE */
   VariableType *v;

   for (v = My->var_head; v != NULL; v = v->next)
   {
      v->VariableFlags |= VARIABLE_PRESET;
      v->VariableFlags |= VARIABLE_COMMON;   
   }
}


static void execute_program( char * FileName )
{
   My->NextValidLineNumber = BasicLineNumberMin;   
   My->IsCommandLineFile = TRUE;
   if( bwb_fload( FileName ) == FALSE )
   {
      sprintf(My->bwb_ebuf, "Failed to open file %s\n", FileName); fputs( My->bwb_ebuf, My->SYSOUT->cfp );
      /* the file load has failed, so do NOT run the program */
      exit(1);
   }
   if (My->err_number < 0 /* file load failed */ )
   {
      /* the file load has failed, so do NOT run the program */
      exit(1);
   }
   bwb_RUN(&My->bwb_start);
}

int
main(int argc, char **argv)
{
   int i;

   My = calloc( 1, sizeof( GlobalType ) );
   if( My == NULL )
   {
      puts( "My == NULL" );
      return 1;
   }
   My->CurrentVersion = &bwb_vertable[ 0 ]; 
   My->IsInteractive = TRUE;
   My->OptionSleepValue = 1;
   My->OptionIndentValue = 2;
   My->OptionTerminalType = C_OPTION_TERMINAL_NONE;
   My->OptionRoundType = C_OPTION_ROUND_BANK;
   My->NextValidLineNumber = BasicLineNumberMin;
   My->IncludeLevel = 0; /* %INCLUDE */
   
   bwx_DEBUG(__FUNCTION__);

   bwb_init();
   if (My->err_number < 0 /* bwb_init() failed */ )
   {
      /* FATAL */
      return 1;
   }
   /* Signon message banner */
   if (argc < 2)
   {
      /* no parameters */
      if (My->IsInteractive)
      {
         bwx_signon();
      }
      else
      {
         /* if INTERACTIVE is FALSE, then we must have a program file */
         fputs( "Program file not specified\n", My->SYSOUT->cfp );
         return 1;
      }
   }

   /* initialize preset variables */
   if (My->IsInteractive)
   {
      /* set a buffer for jump: program execution returns to this point in
       * case of a jump (error, interrupt, or finish program) */
      My->program_run = 0;
      signal(SIGINT, break_mes);
      setjmp(My->mark);
      if (My->program_run > 0)
      {
         /* error in profile */
         return 1;
      }
      My->program_run++;
   }
   
   sprintf( My->read_line, "LET %s = \"%s\"", DEFVNAME_EDITOR, DEF_EDITOR );
   bwb_single_step(My->read_line);

   sprintf( My->read_line, "LET %s = \"%s\"", DEFVNAME_FILES, DEF_FILES );
   bwb_single_step(My->read_line);

   sprintf( My->read_line, "LET %s = \"%s\"", DEFVNAME_IMPL, IMP_IDSTRING );
   bwb_single_step(My->read_line);

   sprintf( My->read_line, "LET %s = \"%s\"", DEFVNAME_PROMPT, PROMPT );
   bwb_single_step(My->read_line);

   sprintf( My->read_line, "LET %s = \"%s\"", DEFVNAME_RENUM, DEF_RENUM );
   bwb_single_step(My->read_line);

   sprintf( My->read_line, "LET %s = %d", DEFVNAME_COLORS, DEF_COLORS );
   bwb_single_step(My->read_line);


   /* check to see if there is a program file: but do this only the first time around! */


#if PROFILE
   execute_profile( PROFILENAME );
#endif


   for( i = 1; i < argc; i++ )
   {
      /* 
      SYNTAX:    bwbasic
      SYNTAX:    bwbasic --tape TapefileName.txt
      SYNTAX:    bwbasic --profile ProfileName.txt
      SYNTAX:    bwbasic program.bas
      SYNTAX:    bwbasic --tape tapefile.inp --profile profile.bas program.bas 
      */
      if( bwb_stricmp( argv[i], "--tape" ) == 0 
      ||  bwb_stricmp( argv[i], "-t"     ) == 0 
      ||  bwb_stricmp( argv[i], "/tape"  ) == 0 
      ||  bwb_stricmp( argv[i], "/t"     ) == 0 
      )
      {
         i++;
         if( i < argc )
         {
            /* --tape filename */
            My->ExternalInputFile = fopen(argv[i], "r");
         }
      }
      else
      if( bwb_stricmp( argv[i], "--profile" ) == 0 
      ||  bwb_stricmp( argv[i], "-p"        ) == 0 
      ||  bwb_stricmp( argv[i], "/profile"  ) == 0 
      ||  bwb_stricmp( argv[i], "/p"        ) == 0 
      )
      {
         i++;
         if( i < argc )
         {
            /* --profile filename */
            execute_profile( argv[i] );
         }
      }
      else
      {
         /* program */
         mark_preset_variables();
         {
            int j = i;
            int n = 0;
            for( n = 0; n < 10 && j < argc; n++, j++ )
            {
               My->COMMAND5[n] = argv[j];
            } 
         }
         execute_program( argv[i] );
         break;
      }
   }   

   if (My->IsInteractive)
   {
      /* set a buffer for jump: program execution returns to this point in
       * case of a jump (error, interrupt, or finish program) */
      My->program_run = 0;
      signal(SIGINT, break_mes);
      setjmp(My->mark);
      if (My->program_run > 0)
      {
         /* error in console mode */
      }
      My->program_run++;
   }

   /* main program loop */
   My->NextValidLineNumber = BasicLineNumberMin;   
   mark_preset_variables();
   while (!feof(My->SYSIN->cfp)) /* condition !feof( My->SYSIN->cfp ) added in v1.11 */
   {
      bwb_mainloop();
   }

   bwx_terminate();  /* allow ^D (Unix) exit with grace */

   return 0;
}



/***************************************************************
  
   FUNCTION:       bwb_interact()
  
   DESCRIPTION:   This function gets a line from the user
         and processes it.
  
***************************************************************/

int
bwb_interact(void)
{
   char LineExists = ' ';
   char prompt[BasicStringLengthMax + 1]; 

   bwx_DEBUG(__FUNCTION__);

   /* take input from keyboard */
   if( My->AutomaticLineNumber > 0 && My->AutomaticLineIncrement > 0 )
   {
      /* AUTO 100, 10 */
      LineType * l;
      for (l = &My->bwb_start; l != &My->bwb_end; l = l->next)
      {
         if( l->number == My->AutomaticLineNumber )
         {
            /* FOUND */
            LineExists = '*';
            break;
         }
         else
         if( l->number > My->AutomaticLineNumber )
         {
            /* NOT FOUND */
            LineExists = ' ';
            break;
         }
      }
      sprintf(prompt,"%d%c", My->AutomaticLineNumber, LineExists );
   }
   else
   {
      VariableType *v; 

      if( (v = var_find(DEFVNAME_PROMPT,0,FALSE)) == NULL )
      {
         bwb_strcpy(prompt, PROMPT);
      }
      else
      {
         VariantType variant;

         if( var_get( v, &variant ) == FALSE )
         {
            bwb_strcpy(prompt, PROMPT);
         }
         else
         {
            if( variant.TypeChar == '$' )
            {
               bwb_strcpy( prompt, variant.Buffer );
               RELEASE( (&variant) );
            }
            else
            {
               bwb_strcpy(prompt, PROMPT);
            }
         }
      }
   }
   bwx_input(prompt, My->read_line);
   CleanLine(My->read_line);   /* JBV */

   My->NextValidLineNumber = BasicLineNumberMin;


   if( My->AutomaticLineNumber > 0 && My->AutomaticLineIncrement > 0 )
   {
       if( My->read_line[0] != BasicNulChar )
       {
           /* non-empty response */
           if( bwb_stricmp( My->read_line, "MAN" ) == 0 )
           {
               /* MAN terminates AUTO mode */
               My->AutomaticLineNumber = 0;
               My->AutomaticLineIncrement = 0;
           }
           else
           {
               /* overwrite any existing line */
               char tbuf[BasicStringLengthMax + 1];
        
               sprintf(tbuf,"%d ", My->AutomaticLineNumber );
               bwb_strcat( tbuf, My->read_line );
               bwb_ladd(tbuf, &My->bwb_start);
               My->AutomaticLineNumber += My->AutomaticLineIncrement;
           }
       }
       else
       {
           /* empty response */
           if( LineExists == '*' )
           {
               /*
               An empty response with an existing line,
               causes AUTO to continue with the next line,
               leaving the current line intact.
               */
               My->AutomaticLineNumber += My->AutomaticLineIncrement;
           }
           else
           {
               /* 
               An empty response with a non-existing line,
               causes AUTO to terminate.
               */
               My->AutomaticLineNumber = 0;
               My->AutomaticLineIncrement = 0;
           }
       }
   }
   else
   if (is_ln(My->read_line) == FALSE)
   {
      /* If there is no line number, execute the line as received */
      /* RUN */
      bwb_Warning_Clear();
      SetOnError(0);
      bwb_xtxtline(My->read_line);
   }
   else
   if (is_numconst(My->read_line) == TRUE)
   {
      /*-----------------------------------------------------------------*/
      /* Another possibility: if My->read_line is a numeric constant,
       * delete */
      /* the indicated line number (JBV)                                 */
      /*-----------------------------------------------------------------*/
      /* DELETE ... */
      int             LineNumber;
      LineNumber = atoi(My->read_line);
      bwb_Warning_Clear();
      SetOnError(0);
      sprintf(My->read_line, "delete %d", LineNumber);
      bwb_xtxtline(My->read_line);
   }
   else
   {
      /* If there is a line number, add the line to the file in memory */
      bwb_ladd(My->read_line, &My->bwb_start);
   }

   return TRUE;

}




/***************************************************************
  
      FUNCTION:       bwb_fload()
  
      DESCRIPTION:   This function loads a BASIC program
         file into memory given a FILE pointer.
  
***************************************************************/

static void FixQuotes( char * buffer )
{
   /* fix unbalanced quotes */
   /* 'buffer' shall be declared "char buffer[BasicStringLengthMax + 1]". */
   int p;
   int QuoteCount = 0;
   
   p = 0;
   while( buffer[p] != BasicNulChar )
   {
      if( buffer[p] == BasicQuoteChar )
      {
         QuoteCount++;
      }
      p++;
      if( p > BasicStringLengthMax )
      {
         p = BasicStringLengthMax;
         buffer[p] = BasicNulChar;
      }
   }
   if( QuoteCount & 1 )
   {
      /* odd == missing trailing quote */
      if( p > BasicStringLengthMax )
      {
         p = BasicStringLengthMax;
      }
      buffer[p] = BasicQuoteChar;
      p++;
      buffer[p] = BasicNulChar;
   }
}

extern int bwb_fload ( char * FileName )
{
   FILE * file = NULL;
   int             Loop;
   bwx_DEBUG(__FUNCTION__);

   /*
   Just in case you are wondering...
   Although this handles the most common cases, it does not handle all possible cases.
   The correct solution is to provide the actual filename (with extension),
   as it exists in the operating system.
   */
   file = NULL;
   if( FileName == NULL /* My->progfile */ )
   {
      /* the FileName is already in My->progfile */
      if (My->progfile[0] == BasicNulChar)
      {
         return FALSE;
      }
      if (file == NULL)
      {
         /* AS-IS */
         file = fopen(My->progfile, "r");
      }
      if (file == NULL)
      {
         /* UPPERCASE */
         strupper(My->progfile);
         file = fopen(My->progfile, "r");
      }
      if (file == NULL)
      {
         /* LOWERCASE */
         strlower(My->progfile);
         file = fopen(My->progfile, "r");
      }
      /* do NOT add extensions to existing filenames */
      if (file == NULL)
      {
         /* NOT FOUND */
         return FALSE;
      }
   }
   else
   {
      if (FileName[0] == BasicNulChar)
      {
         return FALSE;
      }
      if( My->IncludeLevel > 0 )
      {
         /* %INCLUDE filename */
         if (file == NULL)
         {
            /* AS-IS */
            file = fopen( FileName, "r");
         }
         if (file == NULL)
         {
            /* UPPERCASE */
            strupper(FileName);
            file = fopen( FileName, "r");
         }
         if (file == NULL)
         {
            /* LOWERCASE */
            strlower(FileName);
            file = fopen( FileName, "r");
         }
         /* do NOT add extensions to %INCLUDE filenames */
      }
      else
      {
         if (file == NULL)
         {
            /* AS-IS */
            bwb_strcpy(My->progfile, FileName);
            file = fopen(My->progfile, "r");
         }
         if (file == NULL)
         {
            /* UPPERCASE */
            bwb_strcpy(My->progfile, FileName);
            strupper(My->progfile);
            file = fopen(My->progfile, "r");
         }
         if (file == NULL)
         {
            /* LOWERCASE */
            bwb_strcpy(My->progfile, FileName);
            strlower(My->progfile);
            file = fopen(My->progfile, "r");
         }
         if (file == NULL)
         {
            /* UPPERCASE + EXTENSION */
            bwb_strcpy(My->progfile, FileName);
            bwb_strcat(My->progfile, ".BAS");
            strupper(My->progfile);
            file = fopen(My->progfile, "r");
         }
         if (file == NULL)
         {
            /* LOWERCASE + EXTENSION */
            bwb_strcpy(My->progfile, FileName);
            bwb_strcat(My->progfile, ".bas");
            strlower(My->progfile);
            file = fopen(My->progfile, "r");
         }
      }
      if (file == NULL)
      {
         /* NOT FOUND */
         return FALSE;
      }
   }
   My->NextValidLineNumber = BasicLineNumberMin;
   Loop = TRUE;
   if (feof(file))
   {
      Loop = FALSE;
   }
   while (Loop == TRUE)
   {
      My->read_line[0] = BasicNulChar;
      fgets(My->read_line, BasicStringLengthMax, file);
      if (file == My->SYSIN->cfp)
      {
         ResetConsoleColumn();
      }
      /* be sure that this is not EOF with a NULL line */
      if (feof(file))
      {
         Loop = FALSE;
      }
      else
      {
         CleanLine(My->read_line);
         if ( My->read_line[0] != BasicNulChar )
         {
            if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
            {
               /* SYNTAX: %INCLUDE literal.file.name */

               const char Magic_Word[] = "%INCLUDE";
               int Magic_Length = bwb_strlen( Magic_Word );
               if( bwb_strnicmp( My->read_line, Magic_Word, Magic_Length ) == 0 )
               {
                  int Result;
                  int p = Magic_Length;
                  char varname[ BasicNameLengthMax + 1 ];
                  
                  if( buff_read_varname( My->read_line, &p, varname ) == FALSE )
                  {
                     fputs( "Did not find filename after %INCLUDE\n", My->SYSOUT->cfp );
                     fputs( My->read_line, My->SYSOUT->cfp );
                     return FALSE;
                  }
                  if( buff_is_eol( My->read_line, &p ) == FALSE )
                  {
                     fputs( "Found garbage after %INCLUDE\n", My->SYSOUT->cfp );
                     fputs( My->read_line, My->SYSOUT->cfp );
                     return FALSE;
                  }
                  My->IncludeLevel++; /* %INCLUDE */
                  Result = bwb_fload( varname );
                  My->IncludeLevel--; /* %INCLUDE */
                  if( Result == FALSE )
                  {
                     fputs( "Failed to load file\n", My->SYSOUT->cfp );
                     fputs( My->read_line, My->SYSOUT->cfp );
                     return FALSE;
                  }
               }
               else
               {
                  bwb_ladd(My->read_line, &My->bwb_start);
               }
            }
            else
            {
               bwb_ladd(My->read_line, &My->bwb_start);
            }
         }
      }
   }

   /* close file stream */

   fclose(file); /* file != NULL */

   My->NextValidLineNumber = BasicLineNumberMin;

   return TRUE;
}


static char * FindClassicStatementEnd(char *C)
{
   /* 
    * find the end of the current statement
    */
   
   bwx_DEBUG(__FUNCTION__);


   if( My->CurrentVersion->OptionStatementChar == BasicNulChar && My->CurrentVersion->OptionCommentChar == BasicNulChar )
   {
      /* DO NOTHING: Multi-statment lines are not possible */
      return NULL;
   }
   /* skip line number */
   while (bwb_isdigit(*C))
   {
      C++;
   }
   /* skip spaces */
   while (*C == ' ')
   {
      C++;
   }
   if( IS_CHAR( *C, My->CurrentVersion->OptionCommentChar ) )
   {
      /* The entire line is a comment */
      return NULL;
   }
   if (bwb_strnicmp(C, "REM", 3) == 0)
   {
      /* The entire line is a comment */
      return NULL;
   }
   if( (My->CurrentVersion->OptionFlags & OPTION_LABELS_ON) && (My->CurrentVersion->OptionStatementChar != BasicNulChar) )
   {
      /* determine if this line is a LABEL */
      int p;
      char label[BasicNameLengthMax + 1];

      p = 0;
      if( buff_read_label( C, &p, label ) )
      {
         buff_skip_spaces( C, &p );
         if( buff_skip_char( C, &p, My->CurrentVersion->OptionStatementChar ) )
         {
            if( buff_is_eol( C, &p ) )
            {
               /* The entire line is a label */
               /* LABEL : \0 */
               return NULL;
            }
         }
      }
   }
   /* not a special case, so split on the first unquoted OptionCommentChar or OptionStatementChar */
   while (*C != BasicNulChar)
   {
      if (*C == BasicQuoteChar)
      {
         /* skip leading quote */
         C++;
         while (*C != BasicQuoteChar && *C != BasicNulChar)
         {
            /* skip string constant */
            C++;
         }
         if (*C == BasicQuoteChar)
         {
            /* skip trailing quote */
            C++;
         }
      }
      else
      if ( IS_CHAR( *C, My->CurrentVersion->OptionCommentChar ) /* ', ! */ )
      {
         /* FOUND */
         return C;
      }
      else
      if ( IS_CHAR( *C, My->CurrentVersion->OptionStatementChar ) /* :, \ */ )
      {
         /* FOUND */
         return C;
      }
      else
      {
         C++;
      }
   }
   /* NOT FOUND */
   return NULL;
}


static void ImportClassicIfThenElse(char *InBuffer)
{
/*
**
** Determine the type of IF command:
**
** a) STANDARD: 
**    IF x THEN line ELSE line
**
** b) CLASSIC:
**    IF x THEN stmt(s) ELSE stmt(s)
**
** c) STRUCTURED:
**    IF x THEN
**       stmts
**    ELSE
**      stmts
**    END IF
**
** The STANDARD and STRUCTURED forms
** are natively supported.
**
** The CLASSIC form is converted to
** the STRUCTURED form.
**
*/

#define NULLCHAR     BasicNulChar
#define QUOTECHAR    BasicQuoteChar
#define COLONCHAR    My->CurrentVersion->OptionStatementChar
#define SPACECHAR    ' '
   int             i;

   int             nIF = 0;
   int             nTHEN = 0;
   int             nELSE = 0;
   int             nENDIF = 0;

#define NO_COMMAND    0
#define IF_COMMAND    1
#define THEN_COMMAND  2
#define ELSE_COMMAND  3
#define ENDIF_COMMAND 4
   int             LastCommand = NO_COMMAND;

   char            REM[] = "REM ";
   char            IF[] = "IF ";
   char            THEN[] = "THEN ";
   char            THEN2[] = "THEN";
   char            ELSE[] = "ELSE ";
   char            ENDIF[] = "END IF";
   char            GOTO[] = "GOTO ";
   char            DATA[] = "DATA ";
   char            CASE[] = "CASE ";
   char            OutBuffer[BasicStringLengthMax + 1];
   char           *Input = InBuffer;
   char           *Output = OutBuffer;
   char            LastChar = COLONCHAR;

   int             REM_len = bwb_strlen(REM);
   int             IF_len = bwb_strlen(IF);
   int             THEN_len = bwb_strlen(THEN);
   int             THEN2_len = bwb_strlen(THEN2);
   int             ELSE_len = bwb_strlen(ELSE);
   int             ENDIF_len = bwb_strlen(ENDIF);
   int             GOTO_len = bwb_strlen(GOTO);
   int             DATA_len = bwb_strlen(DATA);
   int             CASE_len = bwb_strlen(CASE);

#define OUTPUTCHAR( c ) { *Output = c; Output++; }
#define COPYCHAR  { LastChar = *Input; *Output = *Input; Output++; Input++; }
#define COPY_LINENUMBER  while( bwb_isdigit( *Input )       ) COPYCHAR;
#define COPY_SPACES      while( *Input == SPACECHAR     ) COPYCHAR;
#define COPY_IF          for( i = 0; i < IF_len; i++    ) COPYCHAR;
#define COPY_THEN        for( i = 0; i < THEN_len; i++  ) COPYCHAR;
#define COPY_THEN2       for( i = 0; i < THEN2_len; i++ ) COPYCHAR;
#define COPY_ELSE        for( i = 0; i < ELSE_len; i++  ) COPYCHAR;
#define COPY_ENDIF       for( i = 0; i < ENDIF_len; i++ ) COPYCHAR;
#define FORCE_ENDIF      for( i = 0; i < ENDIF_len; i++ ) OUTPUTCHAR( ENDIF[ i ] );
#define FORCE_GOTO       for( i = 0; i < GOTO_len; i++  ) OUTPUTCHAR( GOTO[ i ] );
#define FORCE_COLON if( LastChar != COLONCHAR ) OUTPUTCHAR( COLONCHAR );

   bwx_DEBUG(__FUNCTION__);

   if( My->CurrentVersion->OptionStatementChar == BasicNulChar )
   {
      /* DO NOTHING: All IFs must be STANDARD or STRUCTURED */
      return;
   }


   COPY_LINENUMBER;
   COPY_SPACES;
   LastChar = COLONCHAR;


   while (*Input != NULLCHAR)
   {
      if (*Input == My->CurrentVersion->OptionCommentChar)
      {
         /* comment */
         break;
      }
      else
      if (*Input == QUOTECHAR)
      {
         /* string constant */
         COPYCHAR;
         while (*Input != QUOTECHAR && *Input != NULLCHAR)
         {
            COPYCHAR;
         }
         if (*Input == QUOTECHAR)
         {
            COPYCHAR;
         }
         COPY_SPACES;
      }
      else
      if (bwb_isalnum(LastChar))
      {
         /* can NOT be the start of a command */
         COPYCHAR;
      }
      else
      if (!bwb_isalpha(*Input))
      {
         /* can NOT be the start of a command */
         COPYCHAR;
      }
      else
      if (bwb_strnicmp(Input, REM, REM_len) == 0)
      {
         break;
      }
      else
      if (bwb_strnicmp(Input, DATA, DATA_len) == 0)
      {
         /* DATA ... */
         break;
      }
      else
      if (bwb_strnicmp(Input, CASE, CASE_len) == 0)
      {
         /* CASE ... */
         break;
      }
      else
      if (bwb_strnicmp(Input, IF, IF_len) == 0)
      {
         /* IF */
         LastCommand = IF_COMMAND;
         nIF++;
         COPY_IF;
         COPY_SPACES;
      }
      else
      if (bwb_strnicmp(Input, GOTO, GOTO_len) == 0 && nIF > nTHEN)
      {
         /* I F 
          *
          * x GO T O 
          *
          * li n e 
          *
          * EL S E 
          *
          * li n e */
         LastCommand = THEN_COMMAND;
         nTHEN++;
         COPY_THEN;
         COPY_SPACES;
         COPY_LINENUMBER;
         COPY_SPACES;
         if (bwb_strnicmp(Input, ELSE, ELSE_len) == 0)
         {
            /* E L S E 
             *
             * Li n e N u m b e r */
            COPY_ELSE;
            COPY_SPACES;
            COPY_LINENUMBER;
            COPY_SPACES;
         }
         /* I S 
          *
          * ST A N D A R D , 
          *
          * NO T 
          *
          * CL A S S I C */
         nENDIF++;
         LastCommand = ENDIF_COMMAND;
      }
      else
      if (bwb_strnicmp(Input, THEN, THEN_len) == 0)
      {
         /* T H E N */
         LastCommand = THEN_COMMAND;
         nTHEN++;
         COPY_THEN;
         COPY_SPACES;
         if (bwb_isdigit(*Input))
         {
            /* * 
             *
             * a  * ) 
             *
             * ST A N D A R D : *   
             *
             * IF 
             * x TH E N 
             *
             * li n e 
             *
             * EL S E 
             *
             * li n e * 
             *
             *
             */
            char           *SavedInput;
            char           *SavedOutput;
            SavedInput = Input;
            SavedOutput = Output;

            COPY_LINENUMBER;
            COPY_SPACES;
            if (bwb_strnicmp(Input, ELSE, ELSE_len) == 0)
            {
               /* E L S E 
                *
                * Li n e N u m b e r */
               COPY_ELSE;
               COPY_SPACES;
               if (bwb_isdigit(*Input))
               {
                  COPY_LINENUMBER;
                  COPY_SPACES;
                  /* I S 
                   *
                   * ST A N D A R D , 
                   *
                   * NO T 
                   *
                   * CL A S S I C */
                  nENDIF++;
                  LastCommand = ENDIF_COMMAND;
               }
               else
               {
                  /* I F 
                   *
                   * x TH E N 
                   *
                   * li n e 
                   *
                   * EL S E 
                   *
                   * st m t s */
                  Input = SavedInput;
                  Output = SavedOutput;
                  FORCE_COLON;
                  FORCE_GOTO;
                  COPY_LINENUMBER;
                  COPY_SPACES;
               }
            }
            else
            {
               /* I S 
                *
                * ST A N D A R D , 
                *
                * NO T 
                *
                * CL A S S I C */
               nENDIF++;
               LastCommand = ENDIF_COMMAND;
            }
         }
         else
         if (*Input == My->CurrentVersion->OptionCommentChar || *Input == NULLCHAR)
         {
            /* I S 
             *
             * ST R U C T U R E D , 
             *
             * NO T 
             *
             * CL A S S I C */
            nENDIF++;
            LastCommand = ENDIF_COMMAND;
         }
         else
         {
            /* * 
             *
             * b  * ) 
             *
             * CL A S S I C : *   
             *
             * IF 
             * x TH E N 
             *
             * st m t ( s ) 
             *
             * EL S E 
             *
             * st m t ( s ) * 
             *
             *
             */
         }
         FORCE_COLON;
      }
      else
      if (bwb_strnicmp(Input, THEN2, THEN2_len) == 0)
      {
         /* t r a i l i n g 
          *
          * TH E N ? */
         char           *PeekInput;

         PeekInput = Input;
         PeekInput += THEN2_len;
         while( *PeekInput == ' ' )
         {
             PeekInput++;
         }
         if (*PeekInput == My->CurrentVersion->OptionCommentChar || *PeekInput == NULLCHAR)
         {
            /* I S 
             *
             * ST R U C T U R E D , 
             *
             * NO T 
             *
             * CL A S S I C */
            nTHEN++;
            COPY_THEN2;
            nENDIF++;
            LastCommand = ENDIF_COMMAND;
            FORCE_COLON;
         }
         else
         {
            /* T H E N x y z , 
             *
             * TH E N 1 2 3 */
         }
      }
      else
      if (bwb_strnicmp(Input, ELSE, ELSE_len) == 0)
      {
         /* E L S E */
         if (LastCommand == ELSE_COMMAND)
         {
            /* w e 
             *
             * ne e d 
             *
             * an 
             * EN D I F 
             *
             * he r e */
            FORCE_COLON;
            FORCE_ENDIF;
            FORCE_COLON;
            nENDIF++;
         }
         LastCommand = ELSE_COMMAND;
         nELSE++;
         FORCE_COLON;
         COPY_ELSE;
         FORCE_COLON;
         COPY_SPACES;
         if (bwb_isdigit(*Input))
         {
            /* I F 
             *
             * x TH E N 
             *
             * st m t ( s ) 
             *
             * EL S E 
             *
             * Li n e N u m b e r */
            FORCE_GOTO;
            COPY_LINENUMBER;
            COPY_SPACES;
         }
         FORCE_COLON;
      }
      else
      if (bwb_strnicmp(Input, ENDIF, ENDIF_len) == 0)
      {
         /* E N D 
          *
          * IF */
         LastCommand = ENDIF_COMMAND;
         nENDIF++;
         COPY_ENDIF;
         FORCE_COLON;
      }
      else
      {
         /* w a s 
          *
          * NO T 
          *
          * th e 
          *
          * st a r t 
          *
          * of 
          * a co m m a n d */
         COPYCHAR;
      }
   }
   /* end of input */
   if (nENDIF < nIF)
   {
      while (nENDIF < nIF)
      {
         /* we need trailing ENDIF's */
         nENDIF++;
         FORCE_COLON;
         FORCE_ENDIF;
      }
   }
   /* fixup trailing REMark command */
   if (bwb_strnicmp(Input, REM, REM_len) == 0)
   {
      /* REMark */
      /* 100 A=1 REMark */
      /* 100 A=1:REMark */
      /* 100 A=1'REMark */
      FORCE_COLON;
   }
   /* copy the comments */
   while (*Input != NULLCHAR)
   {
      COPYCHAR;
      /* cppcheck: (style) Variable 'LastChar' is assigned a value that is never used. */
   }
   OUTPUTCHAR(NULLCHAR);
   bwb_strcpy(InBuffer, OutBuffer);
}


/***************************************************************
  
        FUNCTION:       bwb_ladd()
  
        DESCRIPTION:    This function adds a new line (in the
                        buffer) to the program in memory.
  
***************************************************************/

int
bwb_ladd(char *buffer, LineType * p)
{
   LineType *l;
   LineType *previous;
   char           *newbuffer;

   char           *NextStatement;
   char           *ThisStatement;
   int             Replace = TRUE;
   char            BreakChar = BasicNulChar;


   bwx_DEBUG(__FUNCTION__);
   

   CleanLine(buffer);
   if ( buffer[0] == BasicNulChar )
   {
      return FALSE;
   }
   /* AUTO-FIX UNBALANCED QUOTES */
   FixQuotes(buffer);
   /*
   from here, the line WILL be added so the user can EDIT it,  
   we just complain and refuse to run if any error is detected.
   */

   My->rescan = TRUE;   /* program needs to be scanned again */

   if( IS_CHAR( *buffer, My->CurrentVersion->OptionStatementChar ) )
   {
      /* part of a multi-statement line */
   }
   else
   if( IS_CHAR( *buffer, My->CurrentVersion->OptionCommentChar ) )
   {
      /* part of a multi-statement line */
   }
   else
   {
      ImportClassicIfThenElse(buffer);
   }
   ThisStatement = buffer;
   NextStatement = NULL;
   BreakChar = BasicNulChar;

   do
   {
      if( BreakChar == BasicNulChar )
      {
          /* first pass thru the do{} while loop, do nothing */
      }
      else
      if( IS_CHAR( BreakChar, My->CurrentVersion->OptionCommentChar ) )
      {
          /* ThisStatment will turn out to be the last */
          *ThisStatement = My->CurrentVersion->OptionCommentChar;
      }
      else
      if( IS_CHAR( BreakChar, My->CurrentVersion->OptionStatementChar ) )
      {
          /* we are NOT the last statement, skip over the OptionStatementChar */
          ThisStatement++;
      }
      else
      {
         /* Internal Error */
      }
      
      if( BreakChar == BasicNulChar && IS_CHAR( *buffer, My->CurrentVersion->OptionStatementChar ) )
      {
         /* first pass thru and line begins with colon */
         /* part of a multi-statement line */
         NextStatement = NULL;
         if( My->NextValidLineNumber > 1 )
         {
            My->NextValidLineNumber--;
         }
         Replace = FALSE;
      }
      else
      if( BreakChar == BasicNulChar  && IS_CHAR( *buffer, My->CurrentVersion->OptionCommentChar ) )
      {
         /* first pass thru and line begins with apostrophe */
         /* part of a multi-statement line */
         NextStatement = NULL;
         if( My->NextValidLineNumber > 1 )
         {
            My->NextValidLineNumber--;
         }
         Replace = FALSE;
      }
      else
      {
         NextStatement = FindClassicStatementEnd(ThisStatement);
      }
      
      if (NextStatement == NULL)
      {
         /* we are the last statement */
      }
      else
      {
         /* another statement follows */
         BreakChar = *NextStatement;
         *NextStatement = BasicNulChar;
      }
      CleanLine(ThisStatement);
      if ( ThisStatement[0] != BasicNulChar )
      {

         /* get memory for this line */

         /* Revised to CALLOC pass-thru call by JBV */
         if ((l = (LineType *) CALLOC((size_t) 1, sizeof(LineType), "bwb_ladd")) == NULL)
         {
            WARN_OUT_OF_MEMORY;
            return FALSE;
         }
         /* this line has not been executed or numbered */
         l->LineFlags = 0;
         l->IncludeLevel = My->IncludeLevel; /* %INCLUDE */

         /* get the first element and test for a line number */
         l->position = 0;

         /* set line number in line structure */

         /* ALL lines will have a line number.  If a line
          * number is not provided, then the next available
          * line number is assigned.  */
         newbuffer = ThisStatement;
         l->number = My->NextValidLineNumber;

         if( buff_read_line_number(newbuffer, &(l->position), &l->number) )
         {
            if (l->number < My->NextValidLineNumber)
            {
               /* ERROR */
               sprintf(My->bwb_ebuf, "%d < %d - LINE OUT OF ORDER: %s\n", l->number, My->NextValidLineNumber, buffer);
               fputs( My->bwb_ebuf, My->SYSOUT->cfp );
               My->err_number = -1;  /* in bwb_ladd(), LINE OUT OF ORDER */
               l->number = My->NextValidLineNumber; /* sane default */
            }
            else
            if (l->number < BasicLineNumberMin || l->number > BasicLineNumberMax)
            {
               /* ERROR */
               sprintf(My->bwb_ebuf, "INVALID LINE NUMBER: %s\n", buffer);
               fputs( My->bwb_ebuf, My->SYSOUT->cfp );
               My->err_number = -1;  /* in bwb_ladd(), INVALID LINE NUMBER */
               l->number = My->NextValidLineNumber; /* sane default */
            }
            else
            {
               /* OK */
               My->NextValidLineNumber = l->number;
               l->LineFlags |= LINE_NUMBERED; /* line was manually numbered */
            }
            /* A SPACE IS REQUIRED AFTER THE LINE NUMBER
             * -- NO EXCEPTIONS */
            if (newbuffer[l->position] != ' ')
            {
               /* ERROR */
               sprintf(My->bwb_ebuf, "MISSING SPACE AFTER LINE NUMBER: %s\n", buffer);
               fputs( My->bwb_ebuf, My->SYSOUT->cfp );
               My->err_number = -1;  /* in bwb_ladd(), MISSING SPACE AFTER LINE NUMBER */
            }
            /* the buffer does NOT contain the line
             * number */
            newbuffer += l->position;
         }
         /* the next valid line number is this line number plus one */

         CleanLine(newbuffer);
         if (*newbuffer != BasicNulChar)
         {
            /* not an empty line, so it must start with a
             * BASIC COMMAND or VARIABLE */
            if( *newbuffer == My->CurrentVersion->OptionStatementChar )
            {
               /* this is a multi-statement line */
               newbuffer++;
               while( *newbuffer == ' ' )
               {
                  newbuffer++;
               }
            }
            if (bwb_isalpha(*newbuffer))
            {
               /* OK */
            }
            else
            if (*newbuffer == My->CurrentVersion->OptionCommentChar)
            {
               /* OK */
            }
            else
            if (*newbuffer == My->CurrentVersion->OptionPrintChar)
            {
               /* OK */
            }
            else
            if (*newbuffer == My->CurrentVersion->OptionImageChar)
            {
               /* OK */
            }
            else
            {
               /* ERROR */
               sprintf(My->bwb_ebuf, "ILLEGAL CHARACTER AFTER LINE NUMBER: %s\n", newbuffer);
               fputs( My->bwb_ebuf, My->SYSOUT->cfp );
               My->err_number = -1;  /* in bwb_ladd(), ILLEGAL CHARACTER AFTER LINE NUMBER */
            }
         }
         if (ln_asbuf(l, newbuffer) == FALSE)
         {
            /* ERROR */
            sprintf(My->bwb_ebuf, "INTERNAL ERROR, ln_asbuf() == FALSE\n");
            fputs( My->bwb_ebuf, My->SYSOUT->cfp );
            My->err_number = -1;  /* in bwb_ladd(), INTERNAL ERROR, ln_asbuf() == FALSE */
         }
         if (l->cmdnum > 0)
         {
            /* OK */
         }
         else
         {
            /* ERROR */
            sprintf(My->bwb_ebuf, "ILLEGAL COMMAND AFTER LINE NUMBER: %d %s\n", l->number, l->buffer);
            fputs( My->bwb_ebuf, My->SYSOUT->cfp );
            My->err_number = -1;  /* in bwb_ladd(), ILLEGAL COMMAND AFTER LINE NUMBER */
         }
         for (previous = p; previous != &My->bwb_end; previous = previous->next)
         {
            if (previous->number == l->number)
            {
               if (Replace == TRUE)
               {
                  /* REPLACE 'previous' */
                  while (previous->number == l->number)
                  {
                     LineType * z;
                     
                     z = previous;
                     previous = previous->next;
                     bwb_freeline( z );
                  }
                  l->next = previous;
                  p->next = l;
                  
               }
               else
               {
                  /* APPEND after 'previous' */
                  while (previous->next->number == l->number)
                  {
                     previous = previous->next;
                  }
                  l->next = previous->next;
                  previous->next = l;
               }
               break;
            }
            else
            if (previous->number < l->number && l->number < previous->next->number)
            {
               /* INSERT BETWEEN 'previous' AND
                * 'previous->next' */
               l->next = previous->next;
               previous->next = l;
               break;
            }
            p = previous;
         }

      }
      /* another statement may follow */
      ThisStatement = NextStatement;
      Replace = FALSE;

   }
   while (ThisStatement != NULL);

   My->NextValidLineNumber++;

   return TRUE;

}

/***************************************************************
  
        FUNCTION:       bwb_xtxtline()
  
        DESCRIPTION:    This function executes a text line, i.e.,
                        places it in memory and then relinquishes
                        control.
  
***************************************************************/

LineType *
bwb_xtxtline(char *buffer)
{
   bwx_DEBUG(__FUNCTION__);


   /* remove old program from memory */
   bwb_xnew(&My->user_line);

   /* advance past whitespace */
   while (*buffer == ' ')
   {
      buffer++;
   }
   if (*buffer == BasicNulChar)
   {
      return &My->bwb_end;
   }
   bwb_ladd(buffer, &My->user_line);


   /* execute the line as BASIC command line */
   if( bwb_incexec() )
   {
      My->stack_head->line = My->user_line.next; /* and set current line in it */
      My->stack_head->code = EXEC_NORM;   
      return My->user_line.next;
   }
   /* ERROR */
   return &My->bwb_end;
}

/***************************************************************
  
        FUNCTION:       bwb_incexec()
  
        DESCRIPTION:    This function increments the EXEC
         stack counter.
  
***************************************************************/

int
bwb_incexec(void)
{
   StackType * stack_item;
   bwx_DEBUG(__FUNCTION__);
   if( My->stack_level >= EXECLEVELS )
   {
      WARN_OUT_OF_MEMORY;
      return FALSE;
   }
   if ((stack_item = calloc(1, sizeof(StackType))) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return FALSE;
   }
   stack_item->line = My->ThisLine;
   stack_item->LoopTopLine = NULL;
   stack_item->local_variable = NULL;
   stack_item->OnErrorGoto = 0;
   stack_item->next = My->stack_head;
   My->stack_head = stack_item;
   My->stack_level++;
   return TRUE;
}

/***************************************************************
  
        FUNCTION:       bwb_decexec()
  
        DESCRIPTION:    This function decrements the EXEC
         stack counter.
  
***************************************************************/
void bwb_clrexec( void )
{
   bwx_DEBUG(__FUNCTION__);
   while(  My->stack_head != NULL )
   {
      bwb_decexec();
   }
}

void
bwb_decexec(void)
{
   StackType * stack_item;
   bwx_DEBUG(__FUNCTION__);

   if( My->stack_head == NULL )
   {
      WARN_RETURN_WITHOUT_GOSUB;
      return;
   }
   stack_item = My->stack_head;
   My->stack_head = stack_item->next;
   free( stack_item );
   My->stack_level--;
}

/***************************************************************
  
        FUNCTION:       bwb_mainloop()
  
        DESCRIPTION:    This C function performs one iteration
                        of the interpreter. In a non-preemptive
                        scheduler, this function should be called
                        by the scheduler, not by bwBASIC code.
  
***************************************************************/

void
bwb_mainloop(void)
{
   bwx_DEBUG(__FUNCTION__);

   if( My->stack_head != NULL )
   {
      /* BASIC program running */
      bwb_execline();   /* execute one line of program */
      return;
   }
   /* BASIC program completed */
   
   if( My->ExternalInputFile != NULL )
   {
      /* for automated testing, TAPE command */
      if( bwb_is_eof( My->ExternalInputFile ) == FALSE )
      {
         /* TAPE command is active */
         bwb_interact();   /* get user interaction */
         return;
      }
   }
   /* TAPE command inactive or completed */
   
   if (My->IsCommandLineFile == TRUE)
   {
      /* BASIC program was started from command line */
      bwx_terminate();
      return;
   }
   /* BASIC program was not started from command line */

   if (My->IsInteractive)
   {
      /* interactive */
      bwb_interact();   /* get user interaction */
      return;
   }
   /* non-interactive */

   bwx_terminate();
}

/***************************************************************
  
        FUNCTION:       bwb_execline()
  
        DESCRIPTION:    This function executes a single line of
                        a program in memory. This function is
         called by bwb_mainloop().
  
***************************************************************/

/*
ON statement (event trapping)
ON { COM(n) | KEY(n) | PEN | PLAY(n) | STRIG(n) | TIMER (x) } GOSUB line_number
ON ERROR GOTO line_number
  
*/
#define EVENT_ERROR 0x01
#define EVENT_COM   0X02
#define EVENT_KEY   0X04
#define EVENT_PEN   0X08
#define EVENT_PLAY  0X10
#define EVENT_STRIG 0X20
#define EVENT_TIMER 0X40
#define EVENT_USER  0X80

static void SetEvent(char EventType)
{
   bwx_DEBUG(__FUNCTION__);
   My->EventQueue |= EventType;
}
static void ClrEvent(char EventType)
{
   bwx_DEBUG(__FUNCTION__);
   My->EventQueue &= ~EventType;
}
static int HasEvent(char EventType)
{
   bwx_DEBUG(__FUNCTION__);
   return My->EventQueue & EventType;
}
void
bwb_Timer_On(void)
{
   bwx_DEBUG(__FUNCTION__);
   SetEvent(EVENT_TIMER);
}
void
bwb_Timer_Off(void)
{
   bwx_DEBUG(__FUNCTION__);
   ClrEvent(EVENT_TIMER);
}

void
bwb_Warning( int ErrorLevel, char *ErrorMessage )
{
   bwx_DEBUG(__FUNCTION__);
   if( ErrorLevel == 0 )
   {
      /* clear any existing error */
      My->err_number = 0;          /* ERR */
      My->err_line = NULL;         /* ERL */
      bwb_strcpy(My->ErrMsg, "");  /* ERROR$ */
      ClrEvent(EVENT_ERROR);
   }
   else
   {
      if( HasEvent(EVENT_ERROR) == 0 ) /* no errors pending */
      {
         /* only keep the first pending error to occur */
         if( ErrorMessage == NULL )
         {
            /* use the default error message */
            if( ErrorLevel > 0 && ErrorLevel < 80 )
            {
               ErrorMessage = DefaultErrorMessage[ ErrorLevel ];
            }
            else
            {
               ErrorMessage = DefaultErrorMessage[ 0 ];
            }
         }
         else
         {
            /* use a specified error message */
            if( bwb_strlen(ErrorMessage) > 255 )
            {
               ErrorMessage[ 255 ] = BasicNulChar;
            }
         }
         My->err_number = ErrorLevel;          /* ERR */
         if( My->stack_head != NULL )
         {
            My->err_line = My->stack_head->line;  /* ERL */
         }
         bwb_strcpy(My->ErrMsg, ErrorMessage); /* ERROR$ */
         SetEvent(EVENT_ERROR);
      }
   }
}
int
bwb_Warning_Pending(void)
{
   bwx_DEBUG(__FUNCTION__);
   return HasEvent(EVENT_ERROR);
}
void
bwb_Warning_Clear(void)
{
   bwx_DEBUG(__FUNCTION__);
   bwb_Warning(0, "");  /* clear */
}
int
bwb_Warning_Overflow(char *Message)
{
   /* Numeric overflow, String overflow, SPC() and TAB() */
   /* OVERFLOW is different.  A default value can be used. */
   /* If ON ERROR GOTO 0, then display message and continue (returns FALSE), otherwise ERROR 6 (returns TRUE) */
   bwx_DEBUG(__FUNCTION__);
   if (GetOnError() == 0)
   {
      /* user error handler NOT active */
#if TRUE
      fputs( Message, My->SYSOUT->cfp );
      fputs( "\n", My->SYSOUT->cfp );
#else
      sprintf( My->bwb_ebuf, "LINE %d: %s\n", My->ThisLine->number, Message );
      fputs( My->bwb_ebuf, My->SYSOUT->cfp );
#endif
      return FALSE;
   }
   /* user error handler IS active */
   bwb_Warning(6, Message);   /* Overflow */
   return TRUE;
}
void
bwb_execline(void)
{
   LineType *r, *l;

   bwx_DEBUG(__FUNCTION__);

   l = My->stack_head->line;

   /* if the line is &My->bwb_end, then break out of EXEC loops */
   if (l == &My->bwb_end || My->err_number < 0)
   {
      bwb_clrexec();
      return;
   }
   /* Check for wacko line numbers  */

   My->ThisLine = l;

   /* Print line number if trace is on */

   if (My->bwb_trace == TRUE)
   {
      if (l->number > 0)
      {
         sprintf(My->bwb_ebuf, "[ %d %s ]", l->number, l->buffer);
         prn_xprintf(My->bwb_ebuf);
      }
   }
   l->position = l->Startpos;

   /* if there is a BASIC command in the line, execute it here */


   if (l->cmdnum > 0)
   {
      /* OK */
   }
   else
   {
      WARN_ILLEGAL_DIRECT;
      l->cmdnum = C_REM;
   }
   /* l->cmdnum > 0 */
   
   if( l->LineFlags & LINE_BREAK )
   {
      /* BREAK line */
      l->LineFlags &= ~LINE_BREAK;
      My->ContinueLine = l;
      bwx_STOP();
      return;
   }


   /* execute the command vector */
   /* advance beyond whitespace */

   line_skip_spaces(l);

   if (My->CurrentVersion->OptionFlags & (OPTION_COVERAGE_ON) )
   {
      /* * We do this here * so "END" and "STOP" * are marked */
      if (l->cmdnum == C_DATA)
      {
         /* DATA lines are marked when they are READ */
      }
      else
      {
         /* this line was executed */
         l->LineFlags |= LINE_EXECUTED;
      }
   }

   r = bwb_vector(l);
   if( r == NULL )
   {
      WARN_INTERNAL_ERROR;
      return;
   }

   if (My->err_number < 0)  /* in bwb_execline, FATAL ERROR PENDING */
   {
      /* FATAL */
      bwb_clrexec();
      return;
   }
   if( bwb_Warning_Pending() /* Keep This */ )
   {
      /* we are probably not at the end-of-the-line */
   }
   else
   if (r == l)
   {
      /* we should be at the end-of-the-line */
      if ( line_is_eol(l) )
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return;
      }
   }
   else
   {
      /* we are probably not at the end-of-the-line */
   }

   if (HasEvent(EVENT_ERROR))
   {
      /* a NON-FATAL ERROR has occurred. ERR, ERL, and ERROR$ were
       * already set using bwb_warning(ERR,ERROR$) */
      int             err_gotol;
      ClrEvent(EVENT_ERROR);
      err_gotol = GetOnError();
      if (l->number == 0)
      {
         /* BASIC PROMPT */
         /* For example: 10 ON ERROR GOTO 20 ERROR 1 */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      else
      if (My->err_line == NULL)
      {
         /* BASIC PROMPT */
         /* For example: 10 ON ERROR GOTO 20 ERROR 1 */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      else
      if (My->err_line->number == 0)
      {
         /* BASIC PROMPT */
         /* For example: 10 ON ERROR GOTO 20 ERROR 1 */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      else
      if (err_gotol == -1)
      {
         /* ON ERROR RESUME NEXT */
         r->next->position = 0;
         My->stack_head->line = r->next;
         return;
      }
      else
      if (err_gotol == 0)
      {
         /* ON ERROR GOTO 0 */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      else
      if (err_gotol == My->err_line->number)
      {
         /* RECURSION */
         /* For example: 10 ON ERROR GOTO 20 20 ERROR 1 */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      else
      {
         /* USER ERROR HANDLER SPECIFIED */
         /* find the user-specified error handler and jump there */
         LineType *x;

         for (x = &My->bwb_start; x != &My->bwb_end; x = x->next)
         {
            if (x->number == err_gotol)
            {
               /* FOUND */
               if( My->CurrentVersion->OptionFlags & ( OPTION_ERROR_GOSUB ) )
               {
                   /* OPTION ERROR GOSUB */
                   if( bwb_incexec() )
                   {
                      x->position = 0;
                      My->stack_head->line = x;
                      My->stack_head->code = EXEC_GOSUB;
                   }
                   else
                   {
                      /* ERROR */
                      My->stack_head->line = &My->bwb_end;
                   }
               }
               else
               {
                   /* OPTION ERROR GOTO */
                   x->position = 0;  /* start of line */
                   My->stack_head->line = x;
               }
               return;
            }
         }
         /* NOT FOUND */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      /* DEFAULT ERROR HANDLER */
      /* FATAL */
      bwx_DEBUG(My->ErrMsg); /* My->MaintainerDebugOn */
      prn_xprintf("\n");

      if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
      {
         if( My->progfile[ 0 ] != BasicNulChar )
         {
            prn_xprintf("FILE:");
            prn_xprintf(My->progfile);
            prn_xprintf(", ");
         }
      }
      prn_xprintf("ERROR in line");
      if( My->ThisLine != NULL )
      {
         if ( BasicLineNumberMin <= My->ThisLine->number &&  My->ThisLine->number <= BasicLineNumberMax )
         {
            /* BASIC program line */
            char            tbuf[32];
            sprintf(tbuf, "%d", My->ThisLine->number);
            prn_xprintf(" ");
            prn_xprintf(tbuf);
         }
      }
      prn_xprintf(": ");
      prn_xprintf(My->ErrMsg);
      prn_xprintf("\n");

      if (My->CurrentVersion->OptionFlags & ( OPTION_TRACE_ON ) )
      {
         /* 
         ** Dump the BASIC stack trace when a FATAL error occurs.
         ** First line is the TOP of the stack.
         ** Last line is the BOTTOM of the stack.
         */
         StackType *stack_item;
         prn_xprintf("\nSTACK TRACE:\n");
         for( stack_item = My->stack_head; stack_item != NULL; stack_item = stack_item->next )
         {
            LineType *l;

            l = stack_item->line;
            if (l != NULL)
            {
               if ( BasicLineNumberMin <= l->number &&  l->number <= BasicLineNumberMax )
               {
                  /* BASIC program line */
                  char            LineNumber[32];
                  sprintf(LineNumber, "%d:", l->number);
                  prn_xprintf(LineNumber);
                  if (l->buffer != NULL)
                  {
                     prn_xprintf(l->buffer);
                  }
                  prn_xprintf("\n");
               }
            }
         }
      }

      My->AutomaticLineNumber = 0;
      My->AutomaticLineIncrement = 0;
   
      if (My->IsInteractive)
      {
         /* INTERACTIVE: terminate program */
   
         /* reset all stack counters */
         bwb_clrexec();
         SetOnError(0);
   
         My->err_number = -1; /* in break_handler() */
   
   
         /* reset the break handler */
         signal(SIGINT, break_mes);
   
   
         return;
      }
      /* NOT INTERACTIVE:  terminate immediately */
      bwx_terminate();
      return; /* never reached */
   }
   if (l->number > 0)
   {
      /* These events only occur in running programs */

      if (HasEvent(EVENT_TIMER))
      {
         /* TIMER ON */
         if (My->tmr_count > 0)
         {

            if (bwx_TIMER(0) > My->tmr_expires)
            {
               ClrEvent(EVENT_TIMER);
               if (My->tmr_line > 0)
               {
                  /* ON TIMER( My->tmr_count ) GOSUB My->tmr_line */
                  LineType *x;

                  for (x = &My->bwb_start; x != &My->bwb_end; x = x->next)
                  {
                     if (x->number == My->tmr_line)
                     {
                        /* FOUND */
                        /* save current stack level */
                        My->stack_head->line = r; 
                        /* increment exec stack */
                        if( bwb_incexec() )
                        {
                           /* set the new position to x and return x */
                           x->position = 0;
                           My->stack_head->line = x;
                           My->stack_head->code = EXEC_GOSUB;
                        }
                        else
                        {
                           /* ERROR */
                           My->stack_head->line = &My->bwb_end;
                        }
                        return;
                     }
                  }
                  /* NOT FOUND */
               }
            }
         }
      }
   }
   /* check for end of line: if so, advance to next line and return */
   if (r == l)
   {
      /* advance to the next line */
      l->next->position = 0;
      r = l->next;
   }
   else
   if ( line_is_eol( r ) )
   {
      /* we could be at the end-of-the-line, added for RETURN */
      /* advance to the next line */
      r->next->position = 0;
      r = r->next;
   }
   /* else reset with the value in r */
   My->stack_head->line = r;

}

/***************************************************************
  
        FUNCTION:       ln_asbuf()
  
        DESCRIPTION:    This function allocates memory and copies
         a null-terminated string to a line buffer.
  
***************************************************************/

int
ln_asbuf(LineType * l, char *s)
{
   bwx_DEBUG(__FUNCTION__);

   if (l->buffer != NULL)
   {
      /* Revised to FREE pass-thru call by JBV */
      FREE(l->buffer, "ln_asbuf");
      l->buffer = NULL; /* JBV */
   }
   /* Revised to CALLOC pass-thru call by JBV */
   if ((l->buffer = CALLOC(bwb_strlen(s) + 2, sizeof(char), "ln_asbuf")) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return FALSE;
   }
   /* copy the whole line to the line structure buffer */

   bwb_strcpy(l->buffer, s);


   /* strip CR from the buffer */

   CleanLine(l->buffer);


   /* determine command */
   line_start(l);

   return TRUE;

}


/***************************************************************
  
   FUNCTION:       is_ln()
  
   DESCRIPTION:    This function determines whether a program
         line (in buffer) begins with a line number.
  
***************************************************************/

int
is_ln(char *buffer)
{
   int             position;
   bwx_DEBUG(__FUNCTION__);

   position = 0;
   buff_skip_spaces(buffer, &position);
   if( bwb_isdigit(buffer[position]) )
   {
      return TRUE;
   }
   return FALSE;
}


/***************************************************************
  
   FUNCTION:       CALLOC()
  
   DESCRIPTION:    Pass-thru function to calloc() for debugging
         purposes.  Added by JBV 10/95
  
***************************************************************/

void           *
CALLOC(size_t nelem, size_t elsize, char *str)
{
   void           *ptr;

   bwx_DEBUG(__FUNCTION__);

   ptr = calloc(nelem, elsize);

   return ptr;
}

/***************************************************************
  
   FUNCTION:       FREE()
  
   DESCRIPTION:    Pass-thru function to free() for debugging
         purposes.  Added by JBV 10/95
  
***************************************************************/

void
FREE(void *ptr, char *str)
{
   bwx_DEBUG(__FUNCTION__);

   free(ptr);

}

/* SWITCH */
LineType *
bwb_vector( LineType *l )
{
   LineType *r;
   bwx_DEBUG(__FUNCTION__);
   bwx_DEBUG( l->buffer ); /* My->MaintainerDebugOn */
   switch( l->cmdnum )
   {
   case C_QUEST:
      r = bwb_QUEST( l );
      break;
   case C_APPEND:
      r = bwb_APPEND( l );
      break;
   case C_AS:
      r = bwb_AS( l );
      break;
   case C_AUTO:
      r = bwb_AUTO( l );
      break;
   case C_BACKSPACE:
      r = bwb_BACKSPACE( l );
      break;
   case C_BREAK:
      r = bwb_BREAK( l );
      break;
   case C_BYE:
      r = bwb_BYE( l );
      break;
   case C_CALL:
      r = bwb_CALL( l );
      break;
   case C_CASE:
      r = bwb_CASE( l );
      break;
   case C_CASE_ELSE:
      r = bwb_CASE_ELSE( l );
      break;
   case C_CHAIN:
      r = bwb_CHAIN( l );
      break;
   case C_CHANGE:
      r = bwb_CHANGE( l );
      break;
   case C_CLEAR:
      r = bwb_CLEAR( l );
      break;
   case C_CLOAD:
      r = bwb_CLOAD( l );
      break;
   case C_CLOAD8:
      r = bwb_CLOAD8( l );
      break;
   case C_CLOSE:
      r = bwb_CLOSE( l );
      break;
   case C_CLR:
      r = bwb_CLR( l );
      break;
   case C_CMDS:
      r = bwb_CMDS( l );
      break;
   case C_COMMON:
      r = bwb_COMMON( l );
      break;
   case C_CONSOLE:
      r = bwb_CONSOLE( l );
      break;
   case C_CONT:
      r = bwb_CONT( l );
      break;
   case C_CREATE:
      r = bwb_CREATE( l );
      break;
   case C_CSAVE:
      r = bwb_CSAVE( l );
      break;
   case C_CSAVE8:
      r = bwb_CSAVE8( l );
      break;
   case C_DATA:
      r = bwb_DATA( l );
      break;
   case C_DEF:
      r = bwb_DEF( l );
      break;
   case C_DEF_SUB:
      r = bwb_DEF_SUB( l );
      break;
   case C_DEFBYT:
      r = bwb_DEFBYT( l );
      break;
   case C_DEFCUR:
      r = bwb_DEFCUR( l );
      break;
   case C_DEFDBL:
      r = bwb_DEFDBL( l );
      break;
   case C_DEFINT:
      r = bwb_DEFINT( l );
      break;
   case C_DEFLNG:
      r = bwb_DEFLNG( l );
      break;
   case C_DEFSNG:
      r = bwb_DEFSNG( l );
      break;
   case C_DEFSTR:
      r = bwb_DEFSTR( l );
      break;
   case C_DELETE:
      r = bwb_DELETE( l );
      break;
   case C_DELIMIT:
      r = bwb_DELIMIT( l );
      break;
   case C_DIM:
      r = bwb_DIM( l );
      break;
   case C_DO:
      r = bwb_DO( l );
      break;
   case C_DSP:
      r = bwb_DSP( l );
      break;
   case C_EDIT:
      r = bwb_EDIT( l );
      break;
   case C_ELSE:
      r = bwb_ELSE( l );
      break;
   case C_ELSEIF:
      r = bwb_ELSEIF( l );
      break;
   case C_END:
      r = bwb_END( l );
      break;
   case C_END_FUNCTION:
      r = bwb_END_FUNCTION( l );
      break;
   case C_END_IF:
      r = bwb_END_IF( l );
      break;
   case C_END_SELECT:
      r = bwb_END_SELECT( l );
      break;
   case C_END_SUB:
      r = bwb_END_SUB( l );
      break;
   case C_ERASE:
      r = bwb_ERASE( l );
      break;
   case C_EXCHANGE:
      r = bwb_EXCHANGE( l );
      break;
   case C_EXIT:
      r = bwb_EXIT( l );
      break;
   case C_EXIT_DO:
      r = bwb_EXIT_DO( l );
      break;
   case C_EXIT_FOR:
      r = bwb_EXIT_FOR( l );
      break;
   case C_EXIT_FUNCTION:
      r = bwb_EXIT_FUNCTION( l );
      break;
   case C_EXIT_SUB:
      r = bwb_EXIT_SUB( l );
      break;
   case C_EXIT_UNTIL:
      r = bwb_EXIT_UNTIL( l );
      break;
   case C_EXIT_WHILE:
      r = bwb_EXIT_WHILE( l );
      break;
   case C_FEND:
      r = bwb_FEND( l );
      break;
   case C_FIELD:
      r = bwb_FIELD( l );
      break;
   case C_FILE:
      r = bwb_FILE( l );
      break;
   case C_FILES:
      r = bwb_FILES( l );
      break;
   case C_FNCS:
      r = bwb_FNCS( l );
      break;
   case C_FNEND:
      r = bwb_FNEND( l );
      break;
   case C_FOR:
      r = bwb_FOR( l );
      break;
   case C_FUNCTION:
      r = bwb_FUNCTION( l );
      break;
   case C_GET:
      r = bwb_GET( l );
      break;
   case C_GO:
      r = bwb_GO( l );
      break;
   case C_GO_SUB:
      r = bwb_GO_SUB( l );
      break;
   case C_GO_TO:
      r = bwb_GO_TO( l );
      break;
   case C_GOODBYE:
      r = bwb_GOODBYE( l );
      break;
   case C_GOSUB:
      r = bwb_GOSUB( l );
      break;
   case C_GOTO:
      r = bwb_GOTO( l );
      break;
   case C_HELP:
      r = bwb_HELP( l );
      break;
   case C_IF:
      r = bwb_IF( l );
      break;
   case C_IF_END:
      r = bwb_IF_END( l );
      break;
   case C_IF_MORE:
      r = bwb_IF_MORE( l );
      break;
   case C_IF_THEN:
      r = bwb_IF_THEN( l );
      break;
   case C_IMAGE:
      r = bwb_IMAGE( l );
      break;
   case C_INPUT:
      r = bwb_INPUT( l );
      break;
   case C_LET:
      r = bwb_LET( l );
      break;
   case C_LINE:
      r = bwb_LINE( l );
      break;
   case C_LIST:
      r = bwb_LIST( l );
      break;
   case C_LLIST:
      r = bwb_LLIST( l );
      break;
   case C_LOAD:
      r = bwb_LOAD( l );
      break;
   case C_LOOP:
      r = bwb_LOOP( l );
      break;
   case C_LPRINT:
      r = bwb_LPRINT( l );
      break;
   case C_LPRINTER:
      r = bwb_LPRINTER( l );
      break;
   case C_LSET:
      r = bwb_LSET( l );
      break;
   case C_MAINTAINER:
      r = bwb_MAINTAINER( l );
      break;
   case C_MARGIN:
      r = bwb_MARGIN( l );
      break;
   case C_MAT:
      r = bwb_MAT( l );
      break;
   case C_MAT_GET:
      r = bwb_MAT_GET( l );
      break;
   case C_MAT_INPUT:
      r = bwb_MAT_INPUT( l );
      break;
   case C_MAT_PRINT:
      r = bwb_MAT_PRINT( l );
      break;
   case C_MAT_PUT:
      r = bwb_MAT_PUT( l );
      break;
   case C_MAT_READ:
      r = bwb_MAT_READ( l );
      break;
   case C_MAT_WRITE:
      r = bwb_MAT_WRITE( l );
      break;
   case C_MERGE:
      r = bwb_MERGE( l );
      break;
   case C_MID4:
      r = bwb_MID4( l );
      break;
   case C_NAME:
      r = bwb_NAME( l );
      break;
   case C_NEW:
      r = bwb_NEW( l );
      break;
   case C_NEXT:
      r = bwb_NEXT( l );
      break;
   case C_OF:
      r = bwb_OF( l );
      break;
   case C_OLD:
      r = bwb_OLD( l );
      break;
   case C_ON:
      r = bwb_ON( l );
      break;
   case C_ON_ERROR:
      r = bwb_ON_ERROR( l );
      break;
   case C_ON_ERROR_GOSUB:
      r = bwb_ON_ERROR_GOSUB( l );
      break;
   case C_ON_ERROR_GOTO:
      r = bwb_ON_ERROR_GOTO( l );
      break;
   case C_ON_ERROR_RESUME:
      r = bwb_ON_ERROR_RESUME( l );
      break;
   case C_ON_ERROR_RESUME_NEXT:
      r = bwb_ON_ERROR_RESUME_NEXT( l );
      break;
   case C_ON_ERROR_RETURN:
      r = bwb_ON_ERROR_RETURN( l );
      break;
   case C_ON_ERROR_RETURN_NEXT:
      r = bwb_ON_ERROR_RETURN_NEXT( l );
      break;
   case C_ON_TIMER:
      r = bwb_ON_TIMER( l );
      break;
   case C_OPEN:
      r = bwb_OPEN( l );
      break;
   case C_OPTION:
      r = bwb_OPTION( l );
      break;
   case C_OPTION_ANGLE:
      r = bwb_OPTION_ANGLE( l );
      break;
   case C_OPTION_ANGLE_DEGREES:
      r = bwb_OPTION_ANGLE_DEGREES( l );
      break;
   case C_OPTION_ANGLE_GRADIANS:
      r = bwb_OPTION_ANGLE_GRADIANS( l );
      break;
   case C_OPTION_ANGLE_RADIANS:
      r = bwb_OPTION_ANGLE_RADIANS( l );
      break;
   case C_OPTION_ARITHMETIC:
      r = bwb_OPTION_ARITHMETIC( l );
      break;
   case C_OPTION_ARITHMETIC_DECIMAL:
      r = bwb_OPTION_ARITHMETIC_DECIMAL( l );
      break;
   case C_OPTION_ARITHMETIC_FIXED:
      r = bwb_OPTION_ARITHMETIC_FIXED( l );
      break;
   case C_OPTION_ARITHMETIC_NATIVE:
      r = bwb_OPTION_ARITHMETIC_NATIVE( l );
      break;
   case C_OPTION_BASE:
      r = bwb_OPTION_BASE( l );
      break;
   case C_OPTION_BUGS:
      r = bwb_OPTION_BUGS( l );
      break;
   case C_OPTION_BUGS_OFF:
      r = bwb_OPTION_BUGS_OFF( l );
      break;
   case C_OPTION_BUGS_ON:
      r = bwb_OPTION_BUGS_ON( l );
      break;
   case C_OPTION_COMMENT:
      r = bwb_OPTION_COMMENT( l );
      break;
   case C_OPTION_COMPARE:
      r = bwb_OPTION_COMPARE( l );
      break;
   case C_OPTION_COMPARE_BINARY:
      r = bwb_OPTION_COMPARE_BINARY( l );
      break;
   case C_OPTION_COMPARE_DATABASE:
      r = bwb_OPTION_COMPARE_DATABASE( l );
      break;
   case C_OPTION_COMPARE_TEXT:
      r = bwb_OPTION_COMPARE_TEXT( l );
      break;
   case C_OPTION_COVERAGE:
      r = bwb_OPTION_COVERAGE( l );
      break;
   case C_OPTION_COVERAGE_OFF:
      r = bwb_OPTION_COVERAGE_OFF( l );
      break;
   case C_OPTION_COVERAGE_ON:
      r = bwb_OPTION_COVERAGE_ON( l );
      break;
   case C_OPTION_DATE:
      r = bwb_OPTION_DATE( l );
      break;
   case C_OPTION_DISABLE:
      r = bwb_OPTION_DISABLE( l );
      break;
   case C_OPTION_DISABLE_COMMAND:
      r = bwb_OPTION_DISABLE_COMMAND( l );
      break;
   case C_OPTION_DISABLE_FUNCTION:
      r = bwb_OPTION_DISABLE_FUNCTION( l );
      break;
   case C_OPTION_DISABLE_OPERATOR:
      r = bwb_OPTION_DISABLE_OPERATOR( l );
      break;
   case C_OPTION_ENABLE:
      r = bwb_OPTION_ENABLE( l );
      break;
   case C_OPTION_ENABLE_COMMAND:
      r = bwb_OPTION_ENABLE_COMMAND( l );
      break;
   case C_OPTION_ENABLE_FUNCTION:
      r = bwb_OPTION_ENABLE_FUNCTION( l );
      break;
   case C_OPTION_ENABLE_OPERATOR:
      r = bwb_OPTION_ENABLE_OPERATOR( l );
      break;
   case C_OPTION_ERROR:
      r = bwb_OPTION_ERROR( l );
      break;
   case C_OPTION_ERROR_GOSUB:
      r = bwb_OPTION_ERROR_GOSUB( l );
      break;
   case C_OPTION_ERROR_GOTO:
      r = bwb_OPTION_ERROR_GOTO( l );
      break;
   case C_OPTION_EXPLICIT:
      r = bwb_OPTION_EXPLICIT( l );
      break;
   case C_OPTION_IMAGE:
      r = bwb_OPTION_IMAGE( l );
      break;
   case C_OPTION_IMPLICIT:
      r = bwb_OPTION_IMPLICIT( l );
      break;
   case C_OPTION_INDENT:
      r = bwb_OPTION_INDENT( l );
      break;
   case C_OPTION_LABELS:
      r = bwb_OPTION_LABELS( l );
      break;
   case C_OPTION_LABELS_OFF:
      r = bwb_OPTION_LABELS_OFF( l );
      break;
   case C_OPTION_LABELS_ON:
      r = bwb_OPTION_LABELS_ON( l );
      break;
   case C_OPTION_PRINT:
      r = bwb_OPTION_PRINT( l );
      break;
   case C_OPTION_ROUND:
      r = bwb_OPTION_ROUND( l );
      break;
   case C_OPTION_ROUND_BANK:
      r = bwb_OPTION_ROUND_BANK( l );
      break;
   case C_OPTION_ROUND_MATH:
      r = bwb_OPTION_ROUND_MATH( l );
      break;
   case C_OPTION_ROUND_TRUNCATE:
      r = bwb_OPTION_ROUND_TRUNCATE( l );
      break;
   case C_OPTION_SLEEP:
      r = bwb_OPTION_SLEEP( l );
      break;
   case C_OPTION_STATEMENT:
      r = bwb_OPTION_STATEMENT( l );
      break;
   case C_OPTION_STDERR:
      r = bwb_OPTION_STDERR( l );
      break;
   case C_OPTION_STDIN:
      r = bwb_OPTION_STDIN( l );
      break;
   case C_OPTION_STDOUT:
      r = bwb_OPTION_STDOUT( l );
      break;
   case C_OPTION_STRICT:
      r = bwb_OPTION_STRICT( l );
      break;
   case C_OPTION_STRICT_OFF:
      r = bwb_OPTION_STRICT_OFF( l );
      break;
   case C_OPTION_STRICT_ON:
      r = bwb_OPTION_STRICT_ON( l );
      break;
   case C_OPTION_TERMINAL:
      r = bwb_OPTION_TERMINAL( l );
      break;
   case C_OPTION_TERMINAL_ADM:
      r = bwb_OPTION_TERMINAL_ADM( l );
      break;
   case C_OPTION_TERMINAL_ANSI:
      r = bwb_OPTION_TERMINAL_ANSI( l );
      break;
   case C_OPTION_TERMINAL_NONE:
      r = bwb_OPTION_TERMINAL_NONE( l );
      break;
   case C_OPTION_TIME:
      r = bwb_OPTION_TIME( l );
      break;
   case C_OPTION_TRACE:
      r = bwb_OPTION_TRACE( l );
      break;
   case C_OPTION_TRACE_OFF:
      r = bwb_OPTION_TRACE_OFF( l );
      break;
   case C_OPTION_TRACE_ON:
      r = bwb_OPTION_TRACE_ON( l );
      break;
   case C_OPTION_USING:
      r = bwb_OPTION_USING( l );
      break;
   case C_OPTION_VERSION:
      r = bwb_OPTION_VERSION( l );
      break;
   case C_PAUSE:
      r = bwb_PAUSE( l );
      break;
   case C_POP:
      r = bwb_POP( l );
      break;
   case C_PRINT:
      r = bwb_PRINT( l );
      break;
   case C_PUT:
      r = bwb_PUT( l );
      break;
   case C_QUIT:
      r = bwb_QUIT( l );
      break;
   case C_READ:
      r = bwb_READ( l );
      break;
   case C_RECALL:
      r = bwb_RECALL( l );
      break;
   case C_REM:
      r = bwb_REM( l );
      break;
   case C_RENAME:
      r = bwb_RENAME( l );
      break;
   case C_RENUM:
      r = bwb_RENUM( l );
      break;
   case C_RENUMBER:
      r = bwb_RENUMBER( l );
      break;
   case C_RESET:
      r = bwb_RESET( l );
      break;
   case C_RESTORE:
      r = bwb_RESTORE( l );
      break;
   case C_RESUME:
      r = bwb_RESUME( l );
      break;
   case C_RETURN:
      r = bwb_RETURN( l );
      break;
   case C_RSET:
      r = bwb_RSET( l );
      break;
   case C_RUN:
      r = bwb_RUN( l );
      break;
   case C_SAVE:
      r = bwb_SAVE( l );
      break;
   case C_SCRATCH:
      r = bwb_SCRATCH( l );
      break;
   case C_SELECT:
      r = bwb_SELECT( l );
      break;
   case C_SELECT_CASE:
      r = bwb_SELECT_CASE( l );
      break;
   case C_STEP:
      r = bwb_STEP( l );
      break;
   case C_STOP:
      r = bwb_STOP( l );
      break;
   case C_STORE:
      r = bwb_STORE( l );
      break;
   case C_SUB:
      r = bwb_SUB( l );
      break;
   case C_SUBEND:
      r = bwb_SUBEND( l );
      break;
   case C_SWAP:
      r = bwb_SWAP( l );
      break;
   case C_SYSTEM:
      r = bwb_SYSTEM( l );
      break;
   case C_TEXT:
      r = bwb_TEXT( l );
      break;
   case C_THEN:
      r = bwb_THEN( l );
      break;
   case C_TIMER:
      r = bwb_TIMER( l );
      break;
   case C_TIMER_OFF:
      r = bwb_TIMER_OFF( l );
      break;
   case C_TIMER_ON:
      r = bwb_TIMER_ON( l );
      break;
   case C_TIMER_STOP:
      r = bwb_TIMER_STOP( l );
      break;
   case C_TLOAD:
      r = bwb_TLOAD( l );
      break;
   case C_TO:
      r = bwb_TO( l );
      break;
   case C_TRACE:
      r = bwb_TRACE( l );
      break;
   case C_TRACE_OFF:
      r = bwb_TRACE_OFF( l );
      break;
   case C_TRACE_ON:
      r = bwb_TRACE_ON( l );
      break;
   case C_TSAVE:
      r = bwb_TSAVE( l );
      break;
   case C_UEND:
      r = bwb_UEND( l );
      break;
   case C_UNTIL:
      r = bwb_UNTIL( l );
      break;
   case C_USE:
      r = bwb_USE( l );
      break;
   case C_USER_LBL:
      r = bwb_USER_LBL( l );
      break;
   case C_VARS:
      r = bwb_VARS( l );
      break;
   case C_WEND:
      r = bwb_WEND( l );
      break;
   case C_WHILE:
      r = bwb_WHILE( l );
      break;
   case C_WRITE:
      r = bwb_WRITE( l );
      break;
   default:
      WARN_INTERNAL_ERROR;
      r = l;
      break;
   }
   return r;
}

/* EOF */
