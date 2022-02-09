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
/*---------------------------------------------------------------*/


#include "bwbasic.h"



char           *bwb_ebuf = NULL;  /* error buffer */
static char    *read_line = NULL;
int             bwb_trace = FALSE;
FILE           *errfdevice = NULL;   /* output device for error messages */


int             bwb_curtask = 0;/* current task */

struct bwb_variable *ed = NULL;   /* BWB.EDITOR$ variable */
struct bwb_variable *fi = NULL;   /* BWB.FILES$ variable */
struct bwb_variable *pr = NULL;   /* BWB.PROMPT$ variable */
struct bwb_variable *im = NULL;   /* BWB.IMPLEMENTATION$ variable */
struct bwb_variable *co = NULL;   /* BWB.COLORS variable */

char            progfile[FILENAME_MAX + 1];  /* program filename */
int             rescan = TRUE;   /* program needs to be rescanned */
int             number = 0;   /* current line number */
struct bwb_line *bwb_l = NULL;    /* current line pointer */
struct bwb_line bwb_start; /* starting line marker */
struct bwb_line user_line;
struct bwb_line bwb_end;   /* ending line marker */
struct bwb_line *data_line = NULL;   /* current line to read data */
int             data_pos = 0; /* position in data_line */
struct bwb_variable var_start;   /* variable list start marker */
struct bwb_variable var_end;  /* variable list end marker */
struct bwb_function fnc_start;   /* function list start marker */
struct bwb_function fnc_end;  /* function list end marker */
struct fslte    fslt_start;   /* function-sub-label lookup table start
             * marker */
struct fslte    fslt_end;  /* function-sub-label lookup table end marker */
int             exsc = -1; /* EXEC stack counter */
int             expsc = 0; /* expression stack counter */
struct exse    *excs = NULL;      /* EXEC stack */
struct exp_ese *exps = NULL;      /* Expression stack */
int             IsInteractive = TRUE;
int             IsCommandLineFile = FALSE;
struct dev_element *dev_table = NULL;  /* table of devices */
OptionVersionType OptionVersion = 0;   /* VersionBitmask for compatibility
                * via OPTION VERSION ... */
unsigned short  OptionFlags = 0x0000;
char            OptionCommentChar = '\0'; /* ', ! */
char            OptionStatementChar = '\0'; /* :, \ */
char            OptionDateFormat[81];
char            OptionTimeFormat[81];
int             OptionIndentValue = 2;
int             OptionTerminalType = C_OPTION_TERMINAL_NONE;
int             LPRINT_COLUMN = 1;  /* LPRINT_FILE_NUMBER */
int             LPRINT_WIDTH = 80;  /* LPRINT_FILE_NUMBER */
int             LPRINT_NULLS = 0;   /* LPRINT_FILE_NUMBER */
struct          bwb_line * err_line = NULL; /* line in which error occurred */
int             err_number = 0;  /* number of last error */
int             stopped_line = 0;   /* line number of the last executed
                * STOP command */
int             dim_base = 0; /* OPTION BASE ... */

/* Error messages used more than once */

char            err_openfile[] = ERR_OPENFILE;
char            err_getmem[] = ERR_GETMEM;
char            err_noln[] = ERR_NOLN;
char            err_nofn[] = ERR_NOFN;
char            err_lnnotfound[] = ERR_LNNOTFOUND;
char            err_incomplete[] = ERR_INCOMPLETE;
char            err_valoorange[] = ERR_VALOORANGE;
char            err_syntax[] = ERR_SYNTAX;
char            err_devnum[] = ERR_DEVNUM;
char            err_dev[] = ERR_DEV;
char            err_opsys[] = ERR_OPSYS;
char            err_argstr[] = ERR_ARGSTR;
char            err_defchar[] = ERR_DEFCHAR;
char            err_mismatch[] = ERR_MISMATCH;
char            err_dimnotarray[] = ERR_DIMNOTARRAY;
char            err_retnogosub[] = ERR_RETNOGOSUB;
char            err_od[] = ERR_OD;
char            err_overflow[] = ERR_OVERFLOW;
char            err_nf[] = ERR_NF;
char            err_uf[] = ERR_UF;
char            err_dbz[] = ERR_DBZ;
char            err_redim[] = ERR_REDIM;
char            err_obdim[] = ERR_OBDIM;
char            err_uc[] = ERR_UC;
char            err_noprogfile[] = ERR_NOPROGFILE;



int             NextValidLineNumber = BasicLineNumberMin;   /* Used only to verify
                         * BASIC files are
                         * loaded in ascending
                         * line number order */

BasicLineNumberType tmr_gotol = 0;
BasicNumberType tmr_count = 0;
BasicNumberType tmr_expires = 0;

static FILE    *LPRINTER = NULL;
static char     EventQueue = 0x00;   /* may change to int/long/array */


#if AUTOMATED_REGRESSION
/* for automated testing */
FILE           *ExternalInputFile = NULL;
#endif            /* AUTOMATED_REGRESSION */

int             MaintainerDebugOn = FALSE;
FILE           *MaintainerDebugFile = NULL;

int             DefaultVariableType[26];  /* bwb_variable->type, A-Z */

int
bwx_LPRINT(char a)
{
   bwx_DEBUG(__FUNCTION__);
   /* send character to printer */
   if (LPRINTER == NULL)
   {
      LPRINTER = fopen(LPRINTFILENAME, "w");
   }
   if (LPRINTER != NULL)
   {
      fputc(a, LPRINTER);
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

   if (IsInteractive)
   {
      /* INTERACTIVE: terminate program */

      /* reset all stack counters */
      CURTASK         exsc = -1;
      CURTASK         expsc = 0;
      SetOnError(0);

      err_number = -1;/* in break_handler() */


      /* reset the break handler */
      signal(SIGINT, break_mes);

      longjmp(mark, -1);


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
break_mes(int x)
{
   /* break_mes() is FATAL */
   bwx_DEBUG(__FUNCTION__);
   if (err_number < 0)  /* don't make a bad situation worse */
   {
      /* an error has already ben reported */
   }
   else
   {
      prn_xprintf("\n");
      prn_xprintf(MES_BREAK);
      if (CURTASK number > 0)
      {
         char            tbuf[33];
         sprintf(tbuf, "%d", CURTASK number);
         prn_xprintf(" ");
         prn_xprintf(tbuf);
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
  
   FUNCTION:       bwb_error()
  
   DESCRIPTION:    This function is called to handle errors
         in Bywater.  It displays the error
         message, then calls the break_handler()
         routine.
  
***************************************************************/

int
bwb_error(char *message)
{
   /* bwb_error() is FATAL */
   bwx_DEBUG(__FUNCTION__);
   if (err_number < 0)  /* don't make a bad situation worse */
   {
      /* an error has already ben reported */
   }
   else
   {
      prn_xprintf("\n");
      prn_xprintf(ERROR_HEADER);
      if (CURTASK number > 0)
      {
         char            tbuf[32];
         sprintf(tbuf, "%d", CURTASK number);
         prn_xprintf(" ");
         prn_xprintf(tbuf);
      }
      prn_xprintf(": ");
      prn_xprintf(message);
      prn_xprintf("\n");

      if (OptionFlags & OPTION_TRACE_ON)
      {
         /* * dump BASIC stack trace when FATAL error * First
          * line is the TOP of the stack * Last line is the
          * BOTTOM of the stack */

         int             i;
         prn_xprintf("\nSTACK TRACE:\n");
         for (i = CURTASK exsc; i >= 0; i--)
         {
            struct bwb_line *l;

            l = CURTASK excs[i].line;
            if (l != NULL)
            {
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
   break_handler();
   return FALSE;
}


/***************************************************************
  
      FUNCTION:       bwx_signon()
  
   DESCRIPTION:    This function prints out the sign-on
         message for bwBASIC.
  
***************************************************************/
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
             MES_SIGNON " " VERSION,
             MES_COPYRIGHT_1,
             MES_COPYRIGHT_2,
             MES_COPYRIGHT_3,
             MES_LANGUAGE,
             NULL
};


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

int
bwx_CONSOLE_WIDTH()
{
   /* default CONSOLE width */
   return 80;
}

int
bwx_LPRINT_WIDTH()
{
   /* default PRINTER file width */
   return 80;
}

int
bwx_RANDOM_RECORD_SIZE()
{
   /* default RANDOM record size */
   return 128;
}

int
bwx_DEBUG(const char *A)
{
   /* -- RECURSION WARNING -- DO NOT SEND IT TO BACK TO BWBASIC --  */
   /* This is DEBUG output.  Send it to a file, OutputDebugString, or
    * puts() */
   if (MaintainerDebugOn)
   {
      if (MaintainerDebugFile == NULL)
      {
         MaintainerDebugFile = fopen(DEBUGFILENAME, "w");
      }
      if (MaintainerDebugFile != NULL)
      {
         fputs(A, MaintainerDebugFile);
         fputs("\n", MaintainerDebugFile);
         fflush(MaintainerDebugFile);
      }
      else
      {
         puts(A);
      }
   }
   return 0;
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
      bwb_error("INTERNAL ERROR: bwx_TIMER called with negative value");
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

static void
CleanLine(char *buffer)
{
   char           *newbuffer;

   bwx_DEBUG(__FUNCTION__);
   

   /* cleanup the line, so it's easier to parse  */
   newbuffer = buffer;
   while (*newbuffer != 0)
   {
      if (*newbuffer < ' ')
      {
         /* remove ALL embedded control characters */
         /* if you want a control character, then use CHR$ */
         *newbuffer = ' ';
      }
      newbuffer++;
   }
   /* LTRIM$ */
   newbuffer = buffer;
   if (*newbuffer != 0)
   {
      /* not an empty line, so remove one (or more) leading spaces */
      while (*newbuffer == ' ')
      {
         newbuffer++;
      }
      if (newbuffer > buffer)
      {
         strcpy(buffer, newbuffer);
      }
   }
   /* RTRIM$ */
   newbuffer = buffer;
   if (*newbuffer != 0)
   {
      /* not an empty line, so remove one (or more) trailing spaces */
      char           *E;

      E = strchr(newbuffer, 0);
      E--;
      while (E >= newbuffer && *E == ' ')
      {
         *E = 0;
         E--;
      }
   }
}


/***************************************************************
  
   FUNCTION:       bwb_init()
  
   DESCRIPTION:    This function initializes bwBASIC.
  
***************************************************************/

void
bwb_init(int argc, char **argv)
{
   register int    n;
   static char     start_buf[] = "\0";
   static char     end_buf[] = "\0";
   bwx_DEBUG(__FUNCTION__);

    strcpy( progfile, "" );
    
    memset( &bwb_start, 0, sizeof(struct bwb_line) );
    memset( &user_line, 0, sizeof(struct bwb_line) );
    memset( &bwb_end  , 0, sizeof(struct bwb_line) );
    
    memset( &var_start, 0, sizeof(struct bwb_variable) );
    memset( &var_end  , 0, sizeof(struct bwb_variable) );
    
    memset( &fnc_start, 0, sizeof(struct bwb_function) );
    memset( &fnc_end  , 0, sizeof(struct bwb_function) );
    
    memset( &fslt_start, 0, sizeof(struct fslte) );
    memset( &fslt_end  , 0, sizeof(struct fslte) );

   SortAllCommands();
   SortAllFunctions();


   errfdevice = stderr;

   /* Memory allocation */


   /* expression stack */
   /* Revised to CALLOC pass-thru call by JBV */
   if ((exps = CALLOC(ESTACKSIZE, sizeof(struct exp_ese), "bwb_init")) == NULL)
   {
      bwb_error("in bwb_init(): failed to find memory for exps");
      return;
   }
   /* EXEC stack */

   /* Revised to CALLOC pass-thru call by JBV */
   if ((excs = CALLOC(EXECLEVELS, sizeof(struct exse), "bwb_init")) == NULL)
   {
      bwb_error("in bwb_init(): failed to find memory for excs");
      return;
   }
   /* character buffers */

   /* Revised to CALLOC pass-thru call by JBV */
   if ((bwb_ebuf = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_init")) == NULL)
   {
      bwb_error("in bwb_init(): failed to find memory for bwb_ebuf");
      return;
   }
   /* Revised to CALLOC pass-thru call by JBV */
   if ((read_line = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_init")) == NULL)
   {
      bwb_error("in bwb_init(): failed to find memory for read_line");
      return;
   }
   /* assign memory for the device table */
   /* Revised to CALLOC pass-thru call by JBV */
   if ((dev_table = CALLOC(BasicFileNumberMax + 1 /* console is #0 */ , sizeof(struct dev_element), "bwb_init")) == NULL)
   {
      bwb_error("in bwb_init(): failed to find memory for dev_table");
      return;
   }
   /* initialize tables of variables, functions */

   bwb_start.number = BasicLineNumberMin - 1;
   bwb_start.next = &bwb_end;
   user_line.number = BasicLineNumberMin - 1;
   user_line.next = &bwb_end;
   bwb_end.number = BasicLineNumberMax + 1;
   bwb_end.next = &bwb_end;
   bwb_start.buffer = start_buf;
   user_line.buffer = NULL;
   bwb_end.buffer = end_buf;
   data_line = &bwb_start;
   data_pos = 0;
   exsc = -1;
   expsc = 0;
   bwb_start.position = 0;
   user_line.position = 0;
   bwb_l = &bwb_start;

   var_init(0);
   fnc_init(0);
   fslt_init(0);
   OptionVersionSet(0);



   /* initialize all devices to DEVMODE_CLOSED */

   for (n = 0; n <= BasicFileNumberMax; n++)
   {
      dev_table[n].mode = DEVMODE_CLOSED;
      dev_table[n].width = 0;
      dev_table[n].col = 0;
      dev_table[n].filename[0] = '\0';
      dev_table[n].cfp = NULL;
      dev_table[n].buffer = NULL;
   }
   /* NOTE: File #0 is the console for both INPUT and OUTPUT. */
   if (TRUE)
   {
      dev_table[CONSOLE_FILE_NUMBER].mode = DEVMODE_INPUT | DEVMODE_OUTPUT;
      dev_table[CONSOLE_FILE_NUMBER].width = bwx_CONSOLE_WIDTH();
      dev_table[CONSOLE_FILE_NUMBER].col = 1;
      LPRINT_WIDTH = bwx_LPRINT_WIDTH();
      LPRINT_COLUMN = 1;
      LPRINT_NULLS = 0;
      /* OPEN #0 is an ERROR. */
      /* CLOSE #0 is an ERROR. */
      /* WIDTH #0, 80 is the same as WIDTH 80. */
      /* LPRINT and LLIST are sent to bwx_LPRINT() */
   }
   for (n = 0; n < 26; n++)
   {
      DefaultVariableType[n] = NUMBER;
   }
}

/***************************************************************
  
   FUNCTION:       bwb_interact()
  
   DESCRIPTION:   This function gets a line from the user
         and processes it.
  
***************************************************************/

int
bwb_interact(void)
{
   bwx_DEBUG(__FUNCTION__);


   /* take input from keyboard */

   bwb_gets(read_line);
   CleanLine(read_line);   /* JBV */

   NextValidLineNumber = BasicLineNumberMin;


   if (is_ln(read_line) == FALSE)
   {
      /* If there is no line number, execute the line as received */
      /* RUN */
      bwb_Warning_Clear();
      SetOnError(0);
      bwb_xtxtline(read_line);
   }
   else
   if (is_numconst(read_line) == TRUE)
   {
      /*-----------------------------------------------------------------*/
      /* Another possibility: if read_line is a numeric constant,
       * delete */
      /* the indicated line number (JBV)                                 */
      /*-----------------------------------------------------------------*/
      /* DELETE ... */
      int             LineNumber;
      LineNumber = atoi(read_line);
      bwb_Warning_Clear();
      SetOnError(0);
      sprintf(read_line, "delete %d", LineNumber);
      bwb_xtxtline(read_line);
   }
   else
   {
      /* If there is a line number, add the line to the file in
       * memory */
      bwb_ladd(read_line, &CURTASK bwb_start);
   }

   return TRUE;

}




/***************************************************************
  
      FUNCTION:       bwb_fload()
  
      DESCRIPTION:   This function loads a BASIC program
         file into memory given a FILE pointer.
  
***************************************************************/

int
bwb_fload(FILE * file)
{
   int             Loop;
   bwx_DEBUG(__FUNCTION__);

   NextValidLineNumber = BasicLineNumberMin;
   Loop = TRUE;
   if (feof(file))
   {
      Loop = FALSE;
   }
   while (Loop == TRUE)
   {
      read_line[0] = '\0';
      fgets(read_line, BasicStringLengthMax, file);
      if (file == stdin)
      {
         ResetConsoleColumn();

      }
      CleanLine(read_line);

      /* be sure that this is not EOF with a NULL line */
      if (feof(file))
      {
         Loop = FALSE;
      }
      else
      if (strlen(read_line) > 0)
      {
         bwb_ladd(read_line, &CURTASK bwb_start);
      }
   }

   /* close file stream */

   fclose(file);

   NextValidLineNumber = BasicLineNumberMin;

   return TRUE;
}


static
char           *
FindClassicStatementEnd(char *C)
{
   /* 
    * find an unquoted OptionStatementChar
    * that is NOT in a comment 
    */
   bwx_DEBUG(__FUNCTION__);

#if 0
   if (OptionFlags & OPTION_BUGS_ON)
   {
      /* OK */
   }
   else
   {
      /* ERROR */
      return NULL;
   }
#endif
   if( OptionStatementChar == '\0' )
   {
      /* DO NOTHING: Multi-statment lines are not allowed */
      return NULL;
   }
   /* skip line number */
   while (isdigit(*C))
   {
      C++;
   }
   /* skip spaces */
   while (*C == ' ')
   {
      C++;
   }
   if (strncasecmp(C, "REM", 3) == 0)
   {
      /* REMark */
      return NULL;
   }
   if (OptionFlags & OPTION_LABELS_ON)
   {
      /* determine if this line is a LABEL */
      if (isalpha(*C))
      {
         char           *T = C;
         
         /* LABEL : ' */

         if (OptionFlags & OPTION_BUGS_ON)
         {
             /* labels may be alphanumeric with extra symbols */
             while (isalnum(*T) || *T == '_' || *T == '.')
             {
                T++;
             }
         }
         else
         {
             /* labels may only be alphanumeric */
             while (isalnum(*T))
             {
                T++;
             }
         }
         while (*T == ' ')
         {
            T++;
         }
         if (*T == OptionStatementChar)
         {
            T++;
            while (*T == ' ')
            {
               T++;
            }
            if (*T == OptionCommentChar
                || *T == '\0'
               )
            {
               /* LABEL */
               return NULL;
            }
         }
      }
   }
   while (*C != '\0')
   {
      if (*C == '\"')
      {
         /* skip leading quote */
         C++;
         while (*C != '\"' && *C != '\0')
         {
            /* skip string constant */
            C++;
         }
         if (*C == '\"')
         {
            /* skip trailing quote */
            C++;
         }
      }
      else
      if (*C == OptionCommentChar)
      {
         /* comment */
         return NULL;
      }
      else
      if (*C == OptionStatementChar /* :, \ */ )
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


static void
ImportClassicIfThenElse(char *InBuffer)
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

#define NULLCHAR     '\0'
#define QUOTECHAR    '\"'
#define COLONCHAR    OptionStatementChar
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

   int             REM_len = strlen(REM);
   int             IF_len = strlen(IF);
   int             THEN_len = strlen(THEN);
   int             THEN2_len = strlen(THEN2);
   int             ELSE_len = strlen(ELSE);
   int             ENDIF_len = strlen(ENDIF);
   int             GOTO_len = strlen(GOTO);
   int             DATA_len = strlen(DATA);
   int             CASE_len = strlen(CASE);

#define OUTPUTCHAR( c ) { *Output = c; Output++; }
#define COPYCHAR  { LastChar = *Input; *Output = *Input; Output++; Input++; }
#define COPY_LINENUMBER  while( isdigit( *Input )       ) COPYCHAR;
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

#if 0
   if (OptionFlags & OPTION_LABELS_ON)
   {
      /* DO NOTHING: All IFs must be STANDARD or STRUCTURED */
      return;
   }
#endif
   if( OptionStatementChar == '\0' )
   {
      /* DO NOTHING: All IFs must be STANDARD or STRUCTURED */
      return;
   }


   COPY_LINENUMBER;
   COPY_SPACES;
   LastChar = COLONCHAR;


   while (*Input != NULLCHAR)
   {
      if (*Input == OptionCommentChar)
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
      if (isalnum(LastChar))
      {
         /* can NOT be the start of a command */
         COPYCHAR;
      }
      else
      if (!isalpha(*Input))
      {
         /* can NOT be the start of a command */
         COPYCHAR;
      }
      else
      if (strncasecmp(Input, REM, REM_len) == 0)
      {
         break;
      }
      else
      if (strncasecmp(Input, DATA, DATA_len) == 0)
      {
         /* DATA ... */
         break;
      }
      else
      if (strncasecmp(Input, CASE, CASE_len) == 0)
      {
         /* CASE ... */
         break;
      }
      else
      if (strncasecmp(Input, IF, IF_len) == 0)
      {
         /* IF */
         LastCommand = IF_COMMAND;
         nIF++;
         COPY_IF;
         COPY_SPACES;
      }
      else
      if (strncasecmp(Input, GOTO, GOTO_len) == 0 && nIF > nTHEN)
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
         if (strncasecmp(Input, ELSE, ELSE_len) == 0)
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
      if (strncasecmp(Input, THEN, THEN_len) == 0)
      {
         /* T H E N */
         LastCommand = THEN_COMMAND;
         nTHEN++;
         COPY_THEN;
         COPY_SPACES;
         if (isdigit(*Input))
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
            if (strncasecmp(Input, ELSE, ELSE_len) == 0)
            {
               /* E L S E 
                *
                * Li n e N u m b e r */
               COPY_ELSE;
               COPY_SPACES;
               if (isdigit(*Input))
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
         if (*Input == OptionCommentChar || *Input == NULLCHAR)
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
      if (strncasecmp(Input, THEN2, THEN2_len) == 0)
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
         if (*PeekInput == OptionCommentChar || *PeekInput == NULLCHAR)
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
      if (strncasecmp(Input, ELSE, ELSE_len) == 0)
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
         if (isdigit(*Input))
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
      if (strncasecmp(Input, ENDIF, ENDIF_len) == 0)
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
   if (strncasecmp(Input, REM, REM_len) == 0)
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
   strcpy(InBuffer, OutBuffer);
}


/***************************************************************
  
        FUNCTION:       bwb_ladd()
  
        DESCRIPTION:    This function adds a new line (in the
                        buffer) to the program in memory.
  
***************************************************************/

int
bwb_ladd(char *buffer, struct bwb_line * p)
{
   struct bwb_line *l;
   struct bwb_line *previous;
   char            s_buffer[BasicStringLengthMax + 1];
   char           *newbuffer;

   char           *NextStatement;
   char           *ThisStatement;
   int             Replace = TRUE;


   bwx_DEBUG(__FUNCTION__);
   

   CleanLine(buffer);

   if (strlen(buffer) == 0)
   {
      return FALSE;
   }
   /* * from here, the line WILL be added so the user can EDIT it, * we
    * just complain and refuse to run */

   CURTASK         rescan = TRUE;   /* program needs to be scanned again */

#if 0
   if (OptionFlags & OPTION_BUGS_ON)
#endif
   {
      ImportClassicIfThenElse(buffer);
   }
   ThisStatement = buffer;
   NextStatement = NULL;


   do
   {
#if 0
      if (OptionFlags & OPTION_BUGS_ON)
#endif
      {
         NextStatement = FindClassicStatementEnd(ThisStatement);
         if (NextStatement == NULL)
         {
            /* we are the last part */
         }
         else
         {
            /* another statement follows */
            *NextStatement = '\0';
            NextStatement++;
         }
      }
      CleanLine(ThisStatement);
      if (strlen(ThisStatement) > 0)
      {

         /* get memory for this line */

         /* Revised to CALLOC pass-thru call by JBV */
         if ((l = (struct bwb_line *) CALLOC((size_t) 1, sizeof(struct bwb_line), "bwb_ladd")) == NULL)
         {
            bwb_error("in bwb_ladd(): failed to find memory for new line");
            return FALSE;
         }
         /* this line has not been executed or numbered */
#if 0
         l->Coverage = ' ';   
#endif
         l->LineFlags = 0;

         /* get the first element and test for a line number */

         adv_element(ThisStatement, &(l->position), s_buffer);

         /* set line number in line structure */

#if 0
         CleanLine(ThisStatement);
#endif
         /* ALL lines will have a line number.  If a line
          * number is not provided, then the next available
          * line number is assigned.  */
         newbuffer = ThisStatement;
         l->number = NextValidLineNumber;
         if (is_numconst(s_buffer) == TRUE)
         {
            l->number = atoi(s_buffer);
            if (l->number < NextValidLineNumber)
            {
               /* ERROR */
               sprintf(bwb_ebuf, "%d < %d - LINE OUT OF ORDER: %s", l->number, NextValidLineNumber, buffer);
               puts(bwb_ebuf);
               err_number = -1;  /* in bwb_ladd(), LINE
                         * OUT OF ORDER */
               l->number = NextValidLineNumber; /* sane default */
            }
            else
            if (l->number < BasicLineNumberMin || l->number > BasicLineNumberMax)
            {
               /* ERROR */
               sprintf(bwb_ebuf, "INVALID LINE NUMBER: %s", buffer);
               puts(bwb_ebuf);
               err_number = -1;  /* in bwb_ladd(),
                         * INVALID LINE NUMBER */
               l->number = NextValidLineNumber; /* sane default */
            }
            else
            {
               /* OK */
               NextValidLineNumber = l->number;
               l->LineFlags |= LINE_NUMBERED; /* line was manually numbered */
            }
            /* A SPACE IS REQUIRED AFTER THE LINE NUMBER
             * -- NO EXCEPTIONS */
            if (buffer[l->position] != ' ')
            {
               /* ERROR */
               sprintf(bwb_ebuf, "MISSING SPACE AFTER LINE NUMBER: %s", buffer);
               puts(bwb_ebuf);
               err_number = -1;  /* in bwb_ladd(),
                         * MISSING SPACE AFTER
                         * LINE NUMBER */
            }
            /* the buffer does NOT contain the line
             * number */
            newbuffer += l->position;
         }
         /* the next valid line number is this line number
          * plus one */
#if 0
         NextValidLineNumber++;
#endif

         CleanLine(newbuffer);
         if (*newbuffer != 0)
         {
            /* not an empty line, so it must start with a
             * BASIC COMMAND or VARIABLE */
            if (isalpha(*newbuffer) || *newbuffer == OptionCommentChar || *newbuffer == '?')
            {
               /* OK */
            }
            else
            {
               /* ERROR */
               sprintf(bwb_ebuf, "ILLEGAL CHARACTER AFTER LINE NUMBER: %s", newbuffer);
               puts(bwb_ebuf);
               err_number = -1;  /* in bwb_ladd(),
                         * ILLEGAL CHARACTER
                         * AFTER LINE NUMBER */
            }
         }
         if (ln_asbuf(l, newbuffer) == FALSE)
         {
            /* ERROR */
            sprintf(bwb_ebuf, "in bwb_ladd(): INTERNAL ERROR, ln_asbuf() == FALSE");
            puts(bwb_ebuf);
            err_number = -1;  /* in bwb_ladd(),
                      * INTERNAL ERROR,
                      * ln_asbuf() == FALSE */
         }
         if (l->cmdnum <= 0)
         {
            /* ERROR */
            sprintf(bwb_ebuf, "ILLEGAL COMMAND AFTER LINE NUMBER: %d %s", l->number, l->buffer);
            puts(bwb_ebuf);
            err_number = -1;  /* in bwb_ladd(),
                      * ILLEGAL COMMAND AFTER
                      * LINE NUMBER */
         }
         for (previous = p; previous != &CURTASK bwb_end; previous = previous->next)
         {
            if (previous->number == l->number)
            {
               if (Replace == TRUE)
               {
                  /* REPLACE 'previous' */
                  l->next = previous->next;
                  p->next = l;
                  bwb_freeline(previous);
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
      if (NextStatement == NULL)
      {
         /* we are the last part */
      }
      else
      {
         /* another statement follows */
         ThisStatement = NextStatement;
         Replace = FALSE;
      }

   }
   while (NextStatement != NULL);

   NextValidLineNumber++;

   return TRUE;

}

/***************************************************************
  
        FUNCTION:       bwb_xtxtline()
  
        DESCRIPTION:    This function executes a text line, i.e.,
                        places it in memory and then relinquishes
                        control.
  
***************************************************************/

struct bwb_line *
bwb_xtxtline(char *buffer)
{
   bwx_DEBUG(__FUNCTION__);


   /* remove old program from memory */
   bwb_xnew(&user_line);

   /* advance past whitespace */
   while (*buffer == ' ')
   {
      buffer++;
   }
   if (*buffer == '\0')
   {
      return &CURTASK bwb_end;
   }
   bwb_ladd(buffer, &user_line);


   /* execute the line as BASIC command line */
   bwb_incexec();    /* increment EXEC stack */
   bwb_setexec(user_line.next, 0, EXEC_NORM);   /* and set current line
                      * in it */

   return user_line.next;

}

/***************************************************************
  
        FUNCTION:       bwb_incexec()
  
        DESCRIPTION:    This function increments the EXEC
         stack counter.
  
***************************************************************/

void
bwb_incexec(void)
{
   bwx_DEBUG(__FUNCTION__);
   ++CURTASK exsc;

   if (CURTASK exsc >= EXECLEVELS)
   {
      --CURTASK exsc;
      sprintf(bwb_ebuf, "in bwb_incexec(): incremented EXEC stack past max <%d>",
         EXECLEVELS);
      bwb_error(bwb_ebuf);
      return;
   }
   CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
   CURTASK         excs[CURTASK exsc].LoopBottomLine = NULL;
   CURTASK         excs[CURTASK exsc].local_variable = NULL;
   CURTASK         excs[CURTASK exsc].OnErrorGoto = 0;


}

/***************************************************************
  
        FUNCTION:       bwb_decexec()
  
        DESCRIPTION:    This function decrements the EXEC
         stack counter.
  
***************************************************************/

void
bwb_decexec(void)
{

   /* decrement the exec stack counter */
   bwx_DEBUG(__FUNCTION__);

   --CURTASK exsc;      /* NBS_P164_0840 fixed */

   if (CURTASK exsc < -1)
   {
      CURTASK         exsc = -1;
      sprintf(bwb_ebuf, "in bwb_decexec(): decremented EXEC stack past min <-1>");
      bwb_error(bwb_ebuf);
      return;
   }
}

/***************************************************************
  
        FUNCTION:       bwb_setexec()
  
        DESCRIPTION:    This function sets the line and position
         for the next call to bwb_execline();
  
***************************************************************/

int
bwb_setexec(struct bwb_line * l, int position, int code)
{
   bwx_DEBUG(__FUNCTION__);

   CURTASK         excs[CURTASK exsc].line = l;
   CURTASK         excs[CURTASK exsc].code = code;

   return TRUE;

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

   if (CURTASK exsc > -1)
   {
      bwb_execline();   /* execute one line of program */
   }
   else
   if (IsCommandLineFile == TRUE)
   {
      bwx_terminate();
   }
   else
   if (IsInteractive)
   {
      bwb_interact();   /* get user interaction */
   }
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

static void
SetEvent(char EventType)
{
   bwx_DEBUG(__FUNCTION__);
   EventQueue |= EventType;
}
static void
ClrEvent(char EventType)
{
   bwx_DEBUG(__FUNCTION__);
   EventQueue &= ~EventType;
}
static int
HasEvent(char EventType)
{
   bwx_DEBUG(__FUNCTION__);
   return EventQueue & EventType;
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

char            ErrMsg[256];  /* ERROR$ */
void
bwb_Warning(int ErrorLevel, char *ErrorMessage)
{
   bwx_DEBUG(__FUNCTION__);
   if (ErrorLevel < 0)
   {
      bwb_error("bwb_warning() called with invalid ErrorLevel");
      return;
   }
   if (ErrorMessage == NULL)
   {
      bwb_error("bwb_warning() called with invalid ErrorMessage");
      return;
   }
   if (strlen(ErrorMessage) > 255)
   {
      bwb_error("bwb_warning() called with invalid ErrorMessage");
      return;
   }
   if (ErrorLevel > 0)
   {
      if (err_number == 0) /* no errors pending */
      {
         /* only keep the first pending error to occur */
         err_number = ErrorLevel;   /* ERR */
         err_line = CURTASK excs[CURTASK exsc].line;
         strcpy(ErrMsg, ErrorMessage); /* ERROR$ */
         SetEvent(EVENT_ERROR);
      }
   }
   else
   if (ErrorLevel == 0)
   {
      err_number = 0;   /* ERR */
      err_line = NULL;  /* ERL */
      strcpy(ErrMsg, "");  /* ERROR$ */
      ClrEvent(EVENT_ERROR);
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
void
bwb_Warning_OutOfData(char *Message)
{
   bwx_DEBUG(__FUNCTION__);
   bwb_Warning(4, Message);/* Out of DATA */
}
void
bwb_Warning_InvalidParameter(char *Message)
{
   bwx_DEBUG(__FUNCTION__);
   bwb_Warning(5, Message);/* Illegal function call */
}
void
bwb_Warning_Overflow(char *Message)
{
   /* Numeric overflow, String overflow, SPC() and TAB() */
   /* OVERFLOW is different.  A default value is used. */
   bwx_DEBUG(__FUNCTION__);
   if (GetOnError() == 0)
   {
      /* user error handler NOT active */
      puts(Message);
   }
   else
   {
      /* user error handler IS active */
      bwb_Warning(6, Message);   /* Overflow */
   }
}
void
bwb_Warning_AdvancedFeature(char *Message)
{
   bwx_DEBUG(__FUNCTION__);
   bwb_Warning(73, Message);  /* Advanced Feature */
}


void
bwb_execline(void)
{
   struct bwb_line *r, *l;

   bwx_DEBUG(__FUNCTION__);

   l = CURTASK excs[CURTASK exsc].line;

   /* if the line is &CURTASK bwb_end, then break out of EXEC loops */

   if (l == &CURTASK bwb_end || err_number < 0)
   {
      CURTASK         exsc = -1;
      return;
   }
   /* Check for wacko line numbers  */


   /* Print line number if trace is on */

   if (bwb_trace == TRUE)
   {
      if (l->number > 0)
      {
         sprintf(bwb_ebuf, "[ %d %s ]", l->number, l->buffer);
         prn_xprintf(bwb_ebuf);
      }
   }
   /* Set current line for error/break handling */

   CURTASK         number = l->number;
   CURTASK         bwb_l = l;

   l->position = l->Startpos;

   /* if there is a BASIC command in the line, execute it here */


   if (l->cmdnum <= 0)
   {
      bwb_error(err_syntax);
      return;
   }
   /* l->cmdnum > 0 */


   /* execute the command vector */
   /* advance beyond whitespace */

   adv_ws(l->buffer, &(l->position));

   /* FIXME: use NextLine = l->next, any jumps update NextLine */

   if (OptionFlags & OPTION_COVERAGE_ON)
   {
      /* * We do this here * so "END" and "STOP" * are marked */
      if (l->cmdnum == C_DATA)
      {
         /* * DATA lines are marked * when they are READ */
      }
      else
      {
         /* * this line was executed */
#if 0
         l->Coverage = '*';
#endif         
         l->LineFlags |= LINE_EXECUTED;
      }
   }

   r = bwb_vector(l);

   if (err_number < 0)  /* in bwb_execline, FATAL ERROR PENDING */
   {
      /* FATAL */
      CURTASK         exsc = -1;
      return;
   }
   if (ERROR_PENDING)
   {
      /* we are probably not at the end-of-the-line */
   }
   else
   if (r == l)
   {
      /* we should be at the end-of-the-line */
      if (l->buffer[l->position] != '\0')
      {
         bwb_error(err_syntax);
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
      if (err_line == NULL)
      {
         /* BASIC PROMPT */
         /* For example: 10 ON ERROR GOTO 20 ERROR 1 */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      else
      if (err_line->number == 0)
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
         bwb_setexec(r->next, 0, CURTASK excs[CURTASK exsc].code);
         return;
      }
      else
      if (err_gotol == 0)
      {
         /* ON ERROR GOTO 0 */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      else
      if (err_gotol == err_line->number)
      {
         /* RECURSION */
         /* For example: 10 ON ERROR GOTO 20 20 ERROR 1 */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      else
      {
         /* USER ERROR HANDLER SPECIFIED */
         /* find the user-specified error handler and jump
          * there */
         struct bwb_line *x;

         for (x = &CURTASK bwb_start; x != &CURTASK bwb_end; x = x->next)
         {
            if (x->number == err_gotol)
            {
               /* FOUND */
               if( OptionFlags & OPTION_ERROR_GOSUB )
               {
                   /* OPTION ERROR GOSUB */
                   bwb_incexec();
                   x->position = 0;
                   bwb_setexec(x, 0, EXEC_GOSUB);
               }
               else
               {
                   /* OPTION ERROR GOTO */
                   x->position = 0;  /* start of line */
                   bwb_setexec(x, 0, CURTASK excs[CURTASK exsc].code);
               }
               return;
            }
         }
         /* NOT FOUND */
         /* fall thru to the DEFAULT ERROR HANDLER */
      }
      /* DEFAULT ERROR HANDLER */
      /* FATAL */
      bwb_error(ErrMsg);
      return;
   }
   if (l->number > 0)
   {
      /* These events only occur in running programs */

      if (HasEvent(EVENT_TIMER))
      {
         /* TIMER ON */
         if (tmr_count > 0)
         {

            if (bwx_TIMER(0) > tmr_expires)
            {
               ClrEvent(EVENT_TIMER);
               if (tmr_gotol > 0)
               {
                  /* ON TIMER(...) GOSUB ...
                   * ... */
                  struct bwb_line *x;

                  for (x = &CURTASK bwb_start; x != &CURTASK bwb_end; x = x->next)
                  {
                     if (x->number == tmr_gotol)
                     {
                        /* FOUND */
                        bwb_incexec();
                        x->position = 0;
                        bwb_setexec(x, 0, EXEC_GOSUB);
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
   if (r->buffer[r->position] == '\0')
   {
      /* we could be at the end-of-the-line, added for RETURN */
      /* advance to the next line */
      r->next->position = 0;
      r = r->next;
   }
   /* else reset with the value in r */

   bwb_setexec(r, r->position, CURTASK excs[CURTASK exsc].code);


}

/***************************************************************
  
        FUNCTION:       ln_asbuf()
  
        DESCRIPTION:    This function allocates memory and copies
         a null-terminated string to a line buffer.
  
***************************************************************/

int
ln_asbuf(struct bwb_line * l, char *s)
{
   bwx_DEBUG(__FUNCTION__);

   if (l->buffer != NULL)
   {
      /* Revised to FREE pass-thru call by JBV */
      FREE(l->buffer, "ln_asbuf");
      l->buffer = NULL; /* JBV */
   }
   /* Revised to CALLOC pass-thru call by JBV */
   if ((l->buffer = CALLOC(strlen(s) + 2, sizeof(char), "ln_asbuf"))
       == NULL)
   {
      bwb_error("in ln_asbuf(): failed to find memory for new line");
      return FALSE;
   }
   /* copy the whole line to the line structure buffer */

   strcpy(l->buffer, s);


   /* strip CR from the buffer */

   CleanLine(l->buffer);


   /* determine command */
   line_start(l);

   return TRUE;

}

/***************************************************************
  
        FUNCTION:       bwb_gets()
  
        DESCRIPTION:    This function reads a single line from
                        the specified buffer.
  
***************************************************************/

int
bwb_gets(char *buffer)
{
    struct bwb_variable *v; 
    char Buffer[BasicStringLengthMax + 1]; 

    v = var_find(DEFVNAME_PROMPT); 
    str_btoc(Buffer, var_getsval(v)); 
    bwx_input(Buffer, buffer);
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
   adv_ws(buffer, &position);
   switch (buffer[position])
   {
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      return TRUE;
   default:
      return FALSE;
   }
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
struct bwb_line *
bwb_vector(struct bwb_line * l)
{
   struct bwb_line *r;

   bwx_DEBUG(__FUNCTION__);

   switch (l->cmdnum)
   {
   case C_QUEST:
      r = bwb_QUEST(l);
      break;
   case C_CALL:
      r = bwb_CALL(l);
      break;
   case C_CASE:
      r = bwb_CASE(l);
      break;
   case C_CASE_ELSE:
      r = bwb_CASE_ELSE(l);
      break;
   case C_CASE_IF:
      r = bwb_CASE_IF(l);
      break;
   case C_CASE_IS:
      r = bwb_CASE_IS(l);
      break;
   case C_CHAIN:
      r = bwb_CHAIN(l);
      break;
   case C_CHANGE:
      r = bwb_CHANGE(l);
      break;
   case C_CLEAR:
      r = bwb_CLEAR(l);
      break;
   case C_CLOAD:
      r = bwb_CLOAD(l);
      break;
   case C_CLOAD_:
      r = bwb_CLOAD_(l);
      break;
   case C_CMDS:
      r = bwb_CMDS(l);
      break;
   case C_COMMON:
      r = bwb_COMMON(l);
      break;
   case C_CONT:
      r = bwb_CONT(l);
      break;
   case C_CSAVE:
      r = bwb_CSAVE(l);
      break;
   case C_CSAVE_:
      r = bwb_CSAVE_(l);
      break;
   case C_DATA:
      r = bwb_DATA(l);
      break;
   case C_DEF:
      r = bwb_DEF(l);
      break;
   case C_DEF_SUB:
      r = bwb_DEF_SUB(l);
      break;
   case C_DEFDBL:
      r = bwb_DEFDBL(l);
      break;
   case C_DEFINT:
      r = bwb_DEFINT(l);
      break;
   case C_DEFSNG:
      r = bwb_DEFSNG(l);
      break;
   case C_DEFSTR:
      r = bwb_DEFSTR(l);
      break;
   case C_DELETE:
      r = bwb_DELETE(l);
      break;
   case C_DIM:
      r = bwb_DIM(l);
      break;
   case C_DO:
      r = bwb_DO(l);
      break;
   case C_DO_UNTIL:
      r = bwb_DO_UNTIL(l);
      break;
   case C_DO_WHILE:
      r = bwb_DO_WHILE(l);
      break;
   case C_EDIT:
      r = bwb_EDIT(l);
      break;
   case C_ELSE:
      r = bwb_ELSE(l);
      break;
   case C_ELSEIF:
      r = bwb_ELSEIF(l);
      break;
   case C_END:
      r = bwb_END(l);
      break;
   case C_END_FUNCTION:
      r = bwb_END_FUNCTION(l);
      break;
   case C_END_IF:
      r = bwb_END_IF(l);
      break;
   case C_END_SELECT:
      r = bwb_END_SELECT(l);
      break;
   case C_END_SUB:
      r = bwb_END_SUB(l);
      break;
   case C_ERASE:
      r = bwb_ERASE(l);
      break;
   case C_EXIT:
      r = bwb_EXIT(l);
      break;
   case C_EXIT_DO:
      r = bwb_EXIT_DO(l);
      break;
   case C_EXIT_FOR:
      r = bwb_EXIT_FOR(l);
      break;
   case C_EXIT_FUNCTION:
      r = bwb_EXIT_FUNCTION(l);
      break;
   case C_EXIT_SUB:
      r = bwb_EXIT_SUB(l);
      break;
   case C_EXIT_UNTIL:
      r = bwb_EXIT_UNTIL(l);
      break;
   case C_EXIT_WHILE:
      r = bwb_EXIT_WHILE(l);
      break;
   case C_FIELD:
      r = bwb_FIELD(l);
      break;
   case C_FNCS:
      r = bwb_FNCS(l);
      break;
   case C_FOR:
      r = bwb_FOR(l);
      break;
   case C_FUNCTION:
      r = bwb_FUNCTION(l);
      break;
   case C_GO:
      r = bwb_GO(l);
      break;
   case C_GOSUB:
      r = bwb_GOSUB(l);
      break;
   case C_GOTO:
      r = bwb_GOTO(l);
      break;
   case C_HELP:
      r = bwb_HELP(l);
      break;
   case C_IF:
      r = bwb_IF(l);
      break;
   case C_IF_THEN:
      r = bwb_IF_THEN(l);
      break;
   case C_INPUT:
      r = bwb_INPUT(l);
      break;
   case C_LET:
      r = bwb_LET(l);
      break;
   case C_LINE:
      r = bwb_LINE(l);
      break;
   case C_LIST:
      r = bwb_LIST(l);
      break;
   case C_LOAD:
      r = bwb_LOAD(l);
      break;
   case C_LOOP:
      r = bwb_LOOP(l);
      break;
   case C_LOOP_UNTIL:
      r = bwb_LOOP_UNTIL(l);
      break;
   case C_LOOP_WHILE:
      r = bwb_LOOP_WHILE(l);
      break;
   case C_LPRINT:
      r = bwb_LPRINT(l);
      break;
   case C_LSET:
      r = bwb_LSET(l);
      break;
   case C_MAINTAINER:
      r = bwb_MAINTAINER(l);
      break;
   case C_MERGE:
      r = bwb_MERGE(l);
      break;
   case C_MID_:
      r = bwb_MID_(l);
      break;
   case C_NAME:
      r = bwb_NAME(l);
      break;
   case C_NEW:
      r = bwb_NEW(l);
      break;
   case C_NEXT:
      r = bwb_NEXT(l);
      break;
   case C_ON:
      r = bwb_ON(l);
      break;
   case C_ON_ERROR_GOTO:
      r = bwb_ON_ERROR_GOTO(l);
      break;
   case C_ON_ERROR_GOSUB:
      r = bwb_ON_ERROR_GOSUB(l);
      break;
   case C_ON_ERROR_RESUME_NEXT:
      r = bwb_ON_ERROR_RESUME_NEXT(l);
      break;
   case C_ON_ERROR_RETURN_NEXT:
      r = bwb_ON_ERROR_RETURN_NEXT(l);
      break;
   case C_ON_TIMER:
      r = bwb_ON_TIMER(l);
      break;
   case C_OPEN:
      r = bwb_OPEN(l);
      break;
   case C_OPTION:
      r = bwb_OPTION(l);
      break;
   case C_OPTION_ANGLE_DEGREES:
      r = bwb_OPTION_ANGLE_DEGREES(l);
      break;
   case C_OPTION_ANGLE_RADIANS:
      r = bwb_OPTION_ANGLE_RADIANS(l);
      break;
   case C_OPTION_ARITHMETIC_DECIMAL:
      r = bwb_OPTION_ARITHMETIC_DECIMAL(l);
      break;
   case C_OPTION_ARITHMETIC_FIXED:
      r = bwb_OPTION_ARITHMETIC_FIXED(l);
      break;
   case C_OPTION_ARITHMETIC_NATIVE:
      r = bwb_OPTION_ARITHMETIC_NATIVE(l);
      break;
   case C_OPTION_BASE_0:
      r = bwb_OPTION_BASE_0(l);
      break;
   case C_OPTION_BASE_1:
      r = bwb_OPTION_BASE_1(l);
      break;
   case C_OPTION_BUGS_OFF:
      r = bwb_OPTION_BUGS_OFF(l);
      break;
   case C_OPTION_BUGS_ON:
      r = bwb_OPTION_BUGS_ON(l);
      break;
   case C_OPTION_COMMENT:
      r = bwb_OPTION_COMMENT(l);
      break;
   case C_OPTION_COMPARE_BINARY:
      r = bwb_OPTION_COMPARE_BINARY(l);
      break;
   case C_OPTION_COMPARE_DATABASE:
      r = bwb_OPTION_COMPARE_DATABASE(l);
      break;
   case C_OPTION_COMPARE_TEXT:
      r = bwb_OPTION_COMPARE_TEXT(l);
      break;
   case C_OPTION_COVERAGE_OFF:
      r = bwb_OPTION_COVERAGE_OFF(l);
      break;
   case C_OPTION_COVERAGE_ON:
      r = bwb_OPTION_COVERAGE_ON(l);
      break;
   case C_OPTION_DATE:
      r = bwb_OPTION_DATE(l);
      break;
   case C_OPTION_DISABLE_COMMAND:
      r = bwb_OPTION_DISABLE_COMMAND(l);
      break;
   case C_OPTION_DISABLE_FUNCTION:
      r = bwb_OPTION_DISABLE_FUNCTION(l);
      break;
   case C_OPTION_DISABLE_OPERATOR:
      r = bwb_OPTION_DISABLE_OPERATOR(l);
      break;
   case C_OPTION_ENABLE_COMMAND:
      r = bwb_OPTION_ENABLE_COMMAND(l);
      break;
   case C_OPTION_ENABLE_FUNCTION:
      r = bwb_OPTION_ENABLE_FUNCTION(l);
      break;
   case C_OPTION_ENABLE_OPERATOR:
      r = bwb_OPTION_ENABLE_OPERATOR(l);
      break;
   case C_OPTION_ERROR_GOSUB:
      r = bwb_OPTION_ERROR_GOSUB(l);
      break;
   case C_OPTION_ERROR_GOTO:
      r = bwb_OPTION_ERROR_GOTO(l);
      break;
   case C_OPTION_INDENT:
      r = bwb_OPTION_INDENT(l);
      break;
   case C_OPTION_LABELS_OFF:
      r = bwb_OPTION_LABELS_OFF(l);
      break;
   case C_OPTION_LABELS_ON:
      r = bwb_OPTION_LABELS_ON(l);
      break;
   case C_OPTION_STATEMENT:
      r = bwb_OPTION_STATEMENT(l);
      break;
   case C_OPTION_STRICT_OFF:
      r = bwb_OPTION_STRICT_OFF(l);
      break;
   case C_OPTION_STRICT_ON:
      r = bwb_OPTION_STRICT_ON(l);
      break;
   case C_OPTION_TERMINAL_ADM_3A:
      r = bwb_OPTION_TERMINAL_ADM_3A(l);
      break;
   case C_OPTION_TERMINAL_ANSI:
      r = bwb_OPTION_TERMINAL_ANSI(l);
      break;
   case C_OPTION_TERMINAL_NONE:
      r = bwb_OPTION_TERMINAL_NONE(l);
      break;
   case C_OPTION_TIME:
      r = bwb_OPTION_TIME(l);
      break;
   case C_OPTION_TRACE_OFF:
      r = bwb_OPTION_TRACE_OFF(l);
      break;
   case C_OPTION_TRACE_ON:
      r = bwb_OPTION_TRACE_ON(l);
      break;
   case C_OPTION_VERSION:
      r = bwb_OPTION_VERSION(l);
      break;
   case C_PRINT:
      r = bwb_PRINT(l);
      break;
   case C_QUIT:
      r = bwb_QUIT(l);
      break;
   case C_READ:
      r = bwb_READ(l);
      break;
   case C_REM:
      r = bwb_REM(l);
      break;
   case C_RENUM:
      r = bwb_RENUM(l);
      break;
   case C_RESTORE:
      r = bwb_RESTORE(l);
      break;
   case C_RESUME:
      r = bwb_RESUME(l);
      break;
   case C_RETURN:
      r = bwb_RETURN(l);
      break;
   case C_RSET:
      r = bwb_RSET(l);
      break;
   case C_RUN:
      r = bwb_RUN(l);
      break;
   case C_SAVE:
      r = bwb_SAVE(l);
      break;
   case C_SELECT:
      r = bwb_SELECT(l);
      break;
   case C_SELECT_CASE:
      r = bwb_SELECT_CASE(l);
      break;
   case C_STOP:
      r = bwb_STOP(l);
      break;
   case C_SUB:
      r = bwb_SUB(l);
      break;
   case C_SWAP:
      r = bwb_SWAP(l);
      break;
   case C_SYSTEM:
      r = bwb_SYSTEM(l);
      break;
   case C_TIMER:
      r = bwb_TIMER(l);
      break;
   case C_TIMER_OFF:
      r = bwb_TIMER_OFF(l);
      break;
   case C_TIMER_ON:
      r = bwb_TIMER_ON(l);
      break;
   case C_TIMER_STOP:
      r = bwb_TIMER_STOP(l);
      break;
   case C_UEND:
      r = bwb_UEND(l);
      break;
   case C_UNTIL:
      r = bwb_UNTIL(l);
      break;
   case C_USER_LBL:
      r = bwb_USER_LBL(l);
      break;
   case C_VARS:
      r = bwb_VARS(l);
      break;
   case C_WEND:
      r = bwb_WEND(l);
      break;
   case C_WHILE:
      r = bwb_WHILE(l);
      break;
   case C_WRITE:
      r = bwb_WRITE(l);
      break;
   default:
      sprintf(bwb_ebuf, "in bwb_vector(), INTERNAL ERROR: %d not in switch", l->cmdnum);
      bwb_error(bwb_ebuf);
      r = l;
      break;
   }
   return r;
}

/* EOF */
