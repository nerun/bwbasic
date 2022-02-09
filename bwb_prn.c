/***************************************************************
  
        bwb_prn.c       Print and Error-Handling Commands
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

/*
We try to allow as many legacy PRINT USING formats as reasonable.
Many legacy PRINT USING formats are incompatible with one another.
For example:
1) some use '%' for strings, others use '%' for numbers, others consider '%' as a lieral.
2) some count a leading or traling signs in the width, while others do not.
3) when a value requires more digits than the assigned width:
   a) some truncate the displayed value to the width, 
   b) some expand the width, 
   c) some print a number of '%' or '*', and 
   d) some halt processing.
There is no perfect solution that will work for all possible dialects.
*/


#define PrintUsingNumberDigit    My->CurrentVersion->UsingDigit   /* Digit placeholder, usually '#' */
#define PrintUsingNumberComma    My->CurrentVersion->UsingComma   /* Comma, such as thousands, usually ',' */
#define PrintUsingNumberPeriod   My->CurrentVersion->UsingPeriod  /* Period, such as dollars and cents, usually '.' */
#define PrintUsingNumberPlus     My->CurrentVersion->UsingPlus    /* Plus  sign, positive value, usually '+' */
#define PrintUsingNumberMinus    My->CurrentVersion->UsingMinus   /* Minus sign, negative value, usually '-' */
#define PrintUsingNumberExponent My->CurrentVersion->UsingExrad   /* Exponential format, usually '^' */
#define PrintUsingNumberDollar   My->CurrentVersion->UsingDollar  /* Currency symbol, usually '$' */
#define PrintUsingNumberFiller   My->CurrentVersion->UsingFiller  /* Print filler, such as checks, usually '*' */
#define PrintUsingLiteral        My->CurrentVersion->UsingLiteral /* The next char is a literal, usually '_' */
#define PrintUsingStringFirst    My->CurrentVersion->UsingFirst   /* The first character of the string, usually '!' */
#define PrintUsingStringAll      My->CurrentVersion->UsingAll     /* Print the entire string, usually '&' */
#define PrintUsingStringLength   My->CurrentVersion->UsingLength  /* Print a substring, usually '%' */





/* Prototypes for functions visible only to this file */


static int get_prnfmt(char *buffer, int *position, VariantType *e);
static int xputc(char c);
static int xxputc(char c);
static int xxxputc(char c);
static int bwb_xprint(LineType * l);
static int prn_xxprintf(char *buffer);


int is_empty_filename( char * Buffer )
{
   while( *Buffer == ' ' )
   {
      Buffer++;
   }
   if( *Buffer == BasicNulChar )
   {
      return TRUE;
   }
   return FALSE;
}


FileType * find_file_by_name( char * FileName )
{
   FileType * F;

   if( is_empty_filename( FileName ) )
   {
      /* the rules for Console and Printer vary by command */
      return NULL;
   }
   /* search the list of OPEN files */
   for( F = My->file_head; F != NULL; F = F->next )
   {
      if( F->mode != DEVMODE_CLOSED )
      {
         if( bwb_stricmp( F->filename, FileName ) == 0 )
         {
            /* FOUND */
            return F;
         }
      }
   }
   /* NOT FOUND */
   return NULL;
}


FileType * find_file_by_number( int FileNumber )
{
   FileType * F;
   /* handle MAGIC file numbers */
   if( FileNumber <= 0 )
   {
      /* the rules for Console and Printer vary by command */
      return NULL;
   }
   /* search the list of OPEN files */
   for( F = My->file_head; F != NULL; F = F->next )
   {
      if( F->mode != DEVMODE_CLOSED )
      {
         if( F->FileNumber == FileNumber )
         {
            /* FOUND */
            return F;
         }
      }
   }
   /* NOT FOUND */
   return NULL;
}


FileType * file_new( void )
{
   /* search for an empty slot.  If not found, add a new slot. */
   FileType * F;
   
   for( F = My->file_head; F != NULL; F = F->next )
   {
      if( F->mode == DEVMODE_CLOSED )
      {
         /* FOUND */
         return F;
      }
   }
   /* NOT FOUND */
   F = calloc( 1, sizeof( FileType ) );
   F->next = My->file_head;
   My->file_head = F;
   return F;
}


void file_clear( FileType * F )
{
   /* clean up a file slot that is no longer needed */

#if NEW_VIRTUAL
   clear_virtual_by_file( F->FileNumber );
#endif /* NEW_VIRTUAL */
   F->FileNumber = 0;
   F->mode = DEVMODE_CLOSED;   /* DEVMODE_ item */
   F->width = 0;  /* width for OUTPUT and APPEND; reclen for RANDOM; not used for INPUT or BINARY */
   F->col = 0; /* current column for OUTPUT and APPEND */
   F->row = 0; /* current row for OUTPUT and APPEND */
   F->EOF_LineNumber = 0; /* CBASIC-II: IF END # filenumber THEN linenumber */
   F->delimit = BasicNulChar; /* DELIMIT for READ and WRITE */
   F->filename[0] = BasicNulChar;  /* filename */
   if( F->cfp != NULL )
   {
      fclose( F->cfp ); /* F->cfp != NULL */
      F->cfp = NULL;
   }
   if( F->buffer != NULL ) /* pointer to character buffer for RANDOM */
   {
      free( F->buffer );
      F->buffer = NULL;
   }

}

int file_next_number( void )
{
   int        FileNumber;
   FileType * F;
   FileNumber = 0;
   for (F = My->file_head; F != NULL; F = F->next)
   {
      if (F->mode != DEVMODE_CLOSED)
      {
         if( F->FileNumber > FileNumber )
         {
            FileNumber = F->FileNumber;
         }
      }
   }
   /* 'FileNumber' is the highest FileNumber that is currently open */
   FileNumber++;
   return FileNumber;
}         



/***************************************************************
  
      FUNCTION:       bwx_putc()
  
   DESCRIPTION:    This function outputs a single character
         to the default output device.
  
***************************************************************/

int
bwx_PRINT(char c)
{
   bwx_DEBUG(__FUNCTION__);

   /* send character to console */
   return fputc(c, My->SYSOUT->cfp);
}

int
bwx_LPRINT(char c)
{
   bwx_DEBUG(__FUNCTION__);

   /* send character to printer */
   return fputc(c, My->SYSPRN->cfp);
}

int
prn_lprintf(char *buffer)
{
   bwx_DEBUG(__FUNCTION__);

   while (*buffer != BasicNulChar)
   {
      bwx_LPRINT(*buffer);
      buffer++;
   }
   return 0;
}

int
prn_xprintf(char *buffer)
{
   /* Catch-22: an error has occurred before the devicce table is loaded */
   int             n;
   bwx_DEBUG(__FUNCTION__);
   
   
   n = My->SYSOUT->width;

   if (n > 0)
   {
      int i;
      i = 0;
      while (*buffer)
      {
         fputc(*buffer, My->SYSOUT->cfp);
         buffer++;
         if (*buffer == '\n')
         {
            i = 0;
         }
         i++;
         if (i >= n)
         {
            fputc('\n', My->SYSOUT->cfp);
            i = 0;
         }
      }
   }
   else
   {
      /* raw */
      while (*buffer)
      {
         fputc(*buffer, My->SYSOUT->cfp);
         buffer++;
      }
   }
   fflush(My->SYSOUT->cfp);
   return 0;
}

static void CleanNumericString(char *prnbuf, int RemoveDot)
{
   /* remove trailing zeroes */
   char           *E;
   char           *D;

   bwx_DEBUG(__FUNCTION__);

   E = bwb_strchr(prnbuf, 'E');
   if (E == NULL)
   {
      E = bwb_strchr(prnbuf, 'e');
   }
   if (E)
   {
      /* SCIENTIFIC == SCALED notation */
      /* trim leading zeroes in exponent */
      char           *F;
      char           *G;
      F = E;
      while (bwb_isalpha(*F))
      {
         F++;
      }
      while (*F == '+' || *F == '-')
      {
         /* skip sign */
         F++;
      }
      G = F;
      while (*G == '0' || *G == ' ')
      {
         /* skip leading zeroes or spaces */
         G++;
      }
      if (G > F)
      {
         bwb_strcpy(F, G);
      }
      G = NULL;   /* no longer valid */
      *E = BasicNulChar;  /* for bwb_strlen()  */
   }
   D = bwb_strchr(prnbuf, '.');
   if (D)
   {
      int             N = bwb_strlen(D);
      if (N > 1)
      {
         int             M;
         N--;
         M = N;
         while (D[N] == '0')
         {
            /* remove trailing zeroes */
            D[N] = '_';
            N--;
         }
         if (RemoveDot)
         {
            if (E)
            {
               /* SCIENTIFIC == SCALED notation */
               /* do NOT remove '.' */
            }
            else
            {
               /* NORMAL  == UNSCALED notation */
               /* remove trailing '.' */
               /* this will only occur for integer
                * values */
               while (D[N] == '.')
               {
                  /* _###. POSITIVE INTEGER */
                  /* -###. NEGATIVE INTEGER */
                  D[N] = '_';
                  N--;
               }
            }
         }
         if (N < M)
         {
            if (E)
            {
               /* SCIENTIFIC == SCALED notation */
               *E = 'E';
               E = NULL;
            }
            N++;
            /* if INTEGER, then N == 0, else N > 0 */
            M++;
            /* if SCIENTIFIC, then  *M == 'E' else *M == BasicNulChar */
            bwb_strcpy(&(D[N]), &(D[M]));
         }
      }
   }
   if (E)
   {
      /* SCIENTIFIC == SCALED notation */
      *E = 'E';
      E = NULL;
   }
   if (prnbuf[1] == '0' && prnbuf[2] == '.')
   {
      /* _0.### POSITIVE FRACTION ==> _.### */
      /* -0.### NEGATIVE FRACTION ==> -.### */
      bwb_strcpy(&(prnbuf[1]), &(prnbuf[2]));
   }
   if (prnbuf[1] == '.' && prnbuf[2] == 'E')
   {
      /* _.E POSITIVE ZERO ==> _0 */
      /* -.E NEGATIVE ZERO ==> _0 */
      bwb_strcpy(prnbuf, " 0");
   }
}

static int SignificantDigits(char *Buffer)
{
   int             NumDigits;
   char           *P;

   bwx_DEBUG(__FUNCTION__);


   /* determine the number of significant digits */
   NumDigits = 0;
   P = Buffer;
   while (*P)
   {
      if (bwb_isalpha(*P))
      {
         /* 'E', 'e', and so on. */
         break;
      }
      if (bwb_isdigit(*P))
      {
         NumDigits++;
      }
      P++;
   }
   return NumDigits;
}

void
BasicNumerc(BasicNumberType Input, char *Output)
{
   /*
   ********************************************************************************
   
   This is essentially sprintf( Output, "%g", Input ), 
   except the rules for selecting between "%e", "%f", and "%d" are different.

   The C rules depend upon the value of the exponent.
   The BASIC rules depend upon the number of significant digits.

   The results of this routine have been verified by the NBS2 test suite, so   
   THINK VERY CAREFULLY BEFORE MAKING ANY CHANGES TO THIS ROUTINE.   
   
   ********************************************************************************
   */

   char           *E;

   bwx_DEBUG(__FUNCTION__);

   /* print in scientific form first, to determine exponent and significant digits */
   sprintf(Output, "% 1.*E", SIGNIFICANT_DIGITS - 1, Input);
   E = bwb_strchr(Output, 'E');
   if (E == NULL)
   {
      E = bwb_strchr(Output, 'e');
   }
   if (E)
   {
      /* valid */
      int             Exponent;
      int             NumDigits;
      int             DisplayDigits;
      int             zz;
      char           *F;   /* pointer to the exponent's value */
      F = E;
      while (bwb_isalpha(*F))
      {
         F++;
      }
      Exponent = atoi(F);
      CleanNumericString(Output, 0);
      NumDigits = SignificantDigits(Output);
      DisplayDigits = MIN( NumDigits, SIGNIFICANT_DIGITS );
      zz = MAX(Exponent,DisplayDigits - Exponent - 2);
      if (zz >= SIGNIFICANT_DIGITS)
      {
         /* SCIENTIFIC */
         sprintf(Output, "%# 1.*E", DisplayDigits - 1, Input);
      }
      else
      if (Input == (int) Input)
      {
         /* INTEGER */
         sprintf(Output, "% *d", DisplayDigits, (int) Input);
      }
      else
      {
         /* FLOAT */
         int             N;
         /* number of digits before the '.' */
         int             M;
         /* number of digits after  the '.' */
         N = Exponent + 1;
         if (N < 0)
         {
            N = 0;
         }
         M = SIGNIFICANT_DIGITS - N;
         if (M < 0)
         {
            M = 0;
         }
         
         sprintf(Output, "%# *.*f", N, M, Input);
      }
      CleanNumericString(Output, 0);
   }
   else
   {
      /* ERROR, NAN, INFINITY, ETC. */
   }
   return;
}



LineType *
bwb_LPRINT(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   My->CurrentFile = My->SYSPRN;
   bwb_xprint(l);
   return bwb_zline(l);
}


/***************************************************************
  
        FUNCTION:       bwb_print()
  
        DESCRIPTION:    This function implements the BASIC PRINT
                        command.
  
   SYNTAX:     PRINT [# device-number,][USING format-string$;] expressions...
  
***************************************************************/

LineType *
bwb_QUEST(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   return bwb_PRINT(l);
}

static int bwb_print_at(LineType * l)
{
   int             position;
   int             r;
   int             c;
   

   position = 0;
   r = 0;
   c = 0;

   if( line_read_integer_expression(l, &position) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return FALSE;
   }
   

   if ( line_skip_comma(l))
   {
      /* OK */
   }
   else
   {
      WARN_SYNTAX_ERROR;
      return FALSE;
   }

   if( position < 0 )
   {
      WARN_SYNTAX_ERROR;
      return FALSE;
   }
   if( My->CurrentFile->width <= 0 )
   {
      WARN_SYNTAX_ERROR;
      return FALSE;
   }
   if( My->SCREEN_ROWS <= 0 )
   {
      WARN_SYNTAX_ERROR;
      return FALSE;
   }
   r = position / My->CurrentFile->width;
   c = position - r * My->CurrentFile->width;
   while( r >= My->SCREEN_ROWS )
   {
      r -= My->SCREEN_ROWS;
   }
   r++;
   c++;

   switch (My->OptionTerminalType)
   {
   case C_OPTION_TERMINAL_NONE:
      break;
   case C_OPTION_TERMINAL_ADM:
      fprintf(My->CurrentFile->cfp, "%c=%c%c", 27, r + 32, c + 32);
      break;
   case C_OPTION_TERMINAL_ANSI:
      fprintf(My->CurrentFile->cfp, "%c[%d;%dH", 27, r, c);
      break;
   default:
      WARN_SYNTAX_ERROR;
      return FALSE;
      /* break; */
   }      
   My->CurrentFile->row = r;
   My->CurrentFile->col = c;
   return TRUE;
}

static int bwb_print_num(LineType * l)
{
   int             UserFileNumber;


   if( line_read_integer_expression(l, &UserFileNumber) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return FALSE;
   }

   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      /* 
      CBASIC-II: SERIAL & RANDOM file writes
      PRINT # file_number                 ; expression [, expression] ' SERIAL write
      PRINT # file_number , record_number ; expression [, expression] ' RANDOM write
      */

      if( UserFileNumber <= 0 )
      {
         WARN_BAD_FILE_NUMBER;
         return FALSE;
      }
      /* normal file */
      My->CurrentFile = find_file_by_number( UserFileNumber );
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return FALSE;
      }


      if( line_skip_char( l, ',' ) )
      {
         /* 
         PRINT # file_number , record_number ; expression [, expression] ' RANDOM write
         */
         /* get the RecordNumber */
         int RecordNumber;
         
         if( (My->CurrentFile->mode & DEVMODE_RANDOM) == 0 )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( My->CurrentFile->width <= 0 )
         {
            WARN_FIELD_OVERFLOW;
            return FALSE;
         }
         if( line_read_integer_expression( l, &RecordNumber ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return FALSE;
         }
         if( RecordNumber <= 0 )
         {
            WARN_BAD_RECORD_NUMBER;
            return FALSE;
         }
         RecordNumber--; /* BASIC to C */
         /* if( TRUE ) */
         {
            long offset;
            offset = RecordNumber;
            offset *= My->CurrentFile->width;
            fseek( My->CurrentFile->cfp, offset, SEEK_SET );
         }
      }
      if( line_is_eol( l ) )
      {
         /* PRINT # filenum          */
         /* PRINT # filenum , recnum */
      }
      else
      if( line_skip_char( l, ';' ) )
      {
         /* PRINT # filenum          ; */
         /* PRINT # filenum , recnum ; */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return FALSE;
      }
      return TRUE;
   }
   /* 
   SERIAL file writes:
   PRINT # file_number   
   PRINT # file_number [, expression]
   */
   if( UserFileNumber < 0 )
   {
      My->CurrentFile = My->SYSPRN;
   }
   else
   if( UserFileNumber == 0 )
   {
      My->CurrentFile = My->SYSOUT;
   }
   else
   {
      /* normal file */
      My->CurrentFile = find_file_by_number( UserFileNumber );
   }
   if( My->CurrentFile == NULL )
   {
      WARN_BAD_FILE_NUMBER;
      return FALSE;
   }
   if( ( My->CurrentFile->mode & DEVMODE_WRITE ) == 0 )
   {
      WARN_BAD_FILE_NUMBER;
      return FALSE;
   }
   if( line_is_eol( l ) )
   {
      /* PRINT # 2 */
   }
   else
   if( line_skip_comma( l ) )
   {
      /* PRINT # 2 , ... */
   }
   else
   {
      WARN_SYNTAX_ERROR;
      return FALSE;
   }
   return TRUE;
}

LineType *
bwb_PRINT(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSOUT;
   line_skip_spaces(l);
   if ( line_skip_char(l,'@'))
   {
      /* PRINT @ position, ... */
      if( bwb_print_at(l) == FALSE )
      {
         return bwb_zline(l);
      }
   }
   else
   if ( line_skip_word( l, "AT")  )
   {
      /* PRINT AT position, ... */
      if( bwb_print_at(l) == FALSE )
      {
         return bwb_zline(l);
      }
   }
   else
   if ( line_skip_char(l,BasicFileNumberPrefix))
   {
      /* PRINT # file, ... */
      if( bwb_print_num(l) == FALSE )
      {
         return bwb_zline(l);
      }
   }
   bwb_xprint(l);
   if( My->CurrentFile == My->SYSOUT )
   {
      /* FOR I = 1 TO 1000: PRINT "."; : NEXT I : PRINT */
      fflush(My->SYSOUT->cfp);
   }
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_xprint()
  
   DESCRIPTION:    This function implements the PRINT
         command, utilizing a specified file our
         output device.
  
***************************************************************/

static int buff_read_using( char * buffer, int * position, char * format_string )
{
   int p;
   
   p = *position;
   
   buff_skip_spaces( buffer, &p );
   if( buff_skip_word( buffer, &p, "USING" ) )
   {
      buff_skip_spaces( buffer, &p );
      if( bwb_isdigit( buffer[p] ) )
      {
          /* PRINT USING ### */
          int LineNumber;
          LineType *x = NULL;
          char *C;
          char *F;

           if( buff_read_line_number(buffer, &p, &LineNumber) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return FALSE;
            }
           /* check for target label */
           x = find_line_number( LineNumber, TRUE );
           if (x == NULL)
           {
               WARN_UNDEFINED_LINE;
               return FALSE;
           }
           /* line exists */
           if( x->cmdnum != C_IMAGE )
           {
               WARN_UNDEFINED_LINE;
               return FALSE;
           }
           /* line contains IMAGE command */
          C = x->buffer;
          C += x->Startpos;
          F = format_string;
          /* look for leading quote in IMAGE "..." */
          while( *C == ' ' )
          {
             C++;
          }
          if( *C == BasicQuoteChar )
          {
             /* QUOTED */
              /* skip leading quote */
              C++;
              while( *C != BasicQuoteChar && *C != BasicNulChar )
              {
                  /* copy format string, but not the trailing quote */
                  *F = *C;
                  C++;
                  F++;
              }
              /* skip trailing quote */
          }
          else
          {
             /* UNQUOTED */
              while( *C != BasicNulChar )
              {
                  /* copy format string verbatim */
                  *F = *C;
                  C++;
                  F++;
              }
          }
          /* terminate format string */
          *F = BasicNulChar;
          
          buff_skip_spaces(buffer, &p);
          if ( buff_skip_comma(buffer, &p ) == FALSE)
          {
             WARN_SYNTAX_ERROR;
             return FALSE;
          }
          buff_skip_spaces(buffer, &p);
      }
      else
      {
         {
            char * Value = NULL;
            
            if( buff_read_string_expression( buffer, &p, &Value ) == FALSE )
            {
               WARN_SYNTAX_ERROR; /* HERE-Here-here BUG? */
               return FALSE;
            }
            if( Value == NULL )
            {
               WARN_SYNTAX_ERROR;
               return FALSE;
            }
            bwb_strcpy( format_string, Value );
            free( Value );
         }
         buff_skip_spaces(buffer, &p);
         if ( buff_skip_comma(buffer, &p) == FALSE)
         {
            WARN_SYNTAX_ERROR;
            return FALSE;
         }
         buff_skip_spaces(buffer, &p);
      }
      *position = p;
      return TRUE;
   }
   return FALSE;
}

static int bwb_xprint(LineType * l)
{
   int Success = FALSE;
   VariantType e;       /* no leaks */
   VariantType *E = &e; /* no leaks */
   static int      fs_pos;
   int             OutputCR;
   char    format_string[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);
   CLEAR_VARIANT( E );


   /* Detect USING Here */

   format_string[0] = BasicNulChar;
   fs_pos = 0;

   /* get "USING" in format_string */
   if( buff_read_using( l->buffer, &(l->position), format_string ) == TRUE )
   {
       fs_pos = 0;
   }
   
   /* if no arguments, simply print CR and return */


   /* LOOP THROUGH PRINT ELEMENTS */

   OutputCR = TRUE;

   /* 1980 PRINT  , , ,"A" */
   line_skip_spaces(l);
   while( line_is_eol(l) == FALSE )
   {
      /* 1980 PRINT  , , ,"A" */
      if ( line_skip_char( l, ',' /* comma-specific */ ) )
      {
         /* tab over */
         OutputCR = FALSE;
         if( format_string[0] == BasicNulChar )
         {
            /* Tab only if there's no format specification! (JBV) */

            if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) && My->CurrentFile->FileNumber > 0 )
            {
               /* CBASIC-II: files use commas between values */
               xputc(',');
            }
            else
            {
               xputc('\t');
            }
         }
         if( buff_read_using( l->buffer, &(l->position), format_string ) == TRUE )
         {
             fs_pos = 0;
            OutputCR = TRUE;
         }
      }
      else
      if ( line_skip_char( l, ';' /* semicolon-specific */ ) )
      {
         /* concatenate strings */
         OutputCR = FALSE;
         if( buff_read_using( l->buffer, &(l->position), format_string ) == TRUE )
         {
             fs_pos = 0;
            OutputCR = TRUE;
         }
         if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) && My->CurrentFile->FileNumber > 0 )
         {
            /* CBASIC-II: files cannot use semicolon */
            WARN_SYNTAX_ERROR;
            goto EXIT;
         }
      }
      else
      {
         /* resolve the string */
         OutputCR = TRUE;
         if( line_read_expression( l, E ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            goto EXIT;
         }
         if( bwb_Warning_Pending() /* Keep This */ )
         {
            /* 
            this might look odd... 
            but we want to abort printing on the first warning.
            The expression list could include a function with side-effects,
            so any warning should immediately halt further evaluation.
            */
            goto EXIT;
         }
         if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) && My->CurrentFile->FileNumber > 0 )
         {
            /* CBASIC-II: files have quoted strings */
            if( E->TypeChar == BasicStringSuffix )
            {
               xputc('\"');
            }
         }
         if( get_prnfmt(format_string, &fs_pos, E ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            goto EXIT;
         }
         if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) && My->CurrentFile->FileNumber > 0 )
         {
            /* CBASIC-II: files have quoted strings */
            if( E->TypeChar == BasicStringSuffix )
            {
               xputc('\"');
            }
         }
         RELEASE( E );
      }
      line_skip_spaces(l);
   }        /* end of loop through print elements */

   if (OutputCR == TRUE)
   {
      /* did not end with ',' or ';' */
      xputc('\n');
   }
   Success = TRUE;
EXIT:
   RELEASE( E );
   return Success;   
}


/***************************************************************
  
        FUNCTION:       get_prnfmt()
  
   DESCRIPTION:    This function gets the PRINT USING
         format string, returning a structure
         to the format.
  
***************************************************************/
static int num_prnfmt(char *buffer, int *position, VariantType *e)
{
   /*
   Format a NUMBER.
   'buffer' points to the beginning of a PRINT USING format string, such as "###.##".
   'position' is the current offset in 'buffer'.
   'e' is the current expression to print.
   */
   int  width     =   0;
   int  precision =   0;
   int  exponent  =   0;
   char HeadChar  = ' ';
   char FillChar  = ' ';
   char CurrChar  = ' ';
   char ComaChar  = ' ';
   char TailChar  = ' ';
   int  p;
   char tbuf[BasicStringLengthMax + 1];
   
   p = *position;

   while( IS_CHAR(buffer[p], PrintUsingNumberPlus) || IS_CHAR( buffer[p], PrintUsingNumberMinus ) )
   {
       HeadChar = buffer[p];
       width++;
       p++;
   }
   while( IS_CHAR(buffer[p], PrintUsingNumberFiller) || IS_CHAR(buffer[p], PrintUsingNumberDollar ) )
   {
      if( IS_CHAR( buffer[p], PrintUsingNumberFiller ) )
      {
         FillChar = PrintUsingNumberFiller;
      }
      else
      if( IS_CHAR( buffer[p], PrintUsingNumberDollar ) )
      {
         CurrChar = PrintUsingNumberDollar;
      }
      width++;
      p++;
   }
   while( IS_CHAR(buffer[p], PrintUsingNumberDigit) || IS_CHAR(buffer[p], PrintUsingNumberComma ) )
   {
      if( IS_CHAR( buffer[p], PrintUsingNumberComma ) )
      {
         ComaChar = PrintUsingNumberComma;
      }
      width++;
      p++;
   }
   if( IS_CHAR( buffer[p], PrintUsingNumberPeriod ) )
   {
      while( IS_CHAR( buffer[p], PrintUsingNumberPeriod ) )
      {
         width++;
         p++;
      }
      while( IS_CHAR( buffer[p], PrintUsingNumberDigit ) )
      {
         precision++;
         width++;
         p++;
      }
   }
   while( IS_CHAR( buffer[p], PrintUsingNumberExponent ) )
   {
      exponent++;
      precision++;
      width++;
      p++;
   }
   while( IS_CHAR( buffer[p], PrintUsingNumberPlus ) || IS_CHAR( buffer[p], PrintUsingNumberMinus ) )
   {
      TailChar = buffer[p];
      width++;
      p++;
   }
   /* format the number */


   /* displaying both a Heading and a Trailing sign is NOT supported */
   if( TailChar == ' ' )
   {
      /* do nothing */
   }
   else
   if( IS_CHAR( TailChar, PrintUsingNumberPlus ) || IS_CHAR( TailChar, PrintUsingNumberMinus ) )
   {
      /* force the sign to be printed, so we can move it */
      HeadChar = TailChar;
   }
   else
   {
      WARN_INTERNAL_ERROR;
      return FALSE;
   }


   if( HeadChar == ' ' )
   {
      /* only display a '-' sign */
      if( exponent > 0 )
      {
         sprintf(tbuf, "%*.*e", width, precision, e->Number);
      }
      else
      {
         sprintf(tbuf, "%*.*f", width, precision, e->Number);
      }
   }
   else
   if( IS_CHAR( HeadChar, PrintUsingNumberPlus ) || IS_CHAR( HeadChar, PrintUsingNumberMinus ) )
   {
      /* force a leading sign '+' or '-' */
      if( exponent > 0 )
      {
         sprintf(tbuf, "%+*.*e", width, precision, e->Number);
      }
      else
      {
         sprintf(tbuf, "%+*.*f", width, precision, e->Number);
      }
   }
   else
   {
      WARN_INTERNAL_ERROR;
      return FALSE;
   }
   
   if( TailChar == ' ' )
   {
      /* do nothing */
   }
   else
   if( IS_CHAR( TailChar, PrintUsingNumberPlus ) || IS_CHAR( TailChar, PrintUsingNumberMinus ) )
   {
      /* move sign '+' or '-' to end */
      int i;
      int n;

      n = bwb_strlen(tbuf);

      for (i = 0; i < n; i++)
      {
         if( tbuf[i] != ' ' )
         {
            if( IS_CHAR( tbuf[i], PrintUsingNumberPlus ) )
            {
               tbuf[i] = ' ';
               if( IS_CHAR( TailChar, PrintUsingNumberPlus ) )
               {
                  /* TailChar of '+' does print a '+' */
                  bwb_strcat(tbuf,"+");
               }
               else
               if( IS_CHAR( TailChar, PrintUsingNumberMinus ) )
               {
                  /* TailChar of '-' does NOT print a '+' */
                  bwb_strcat(tbuf," ");
               }
            }
            else
            if( IS_CHAR( tbuf[i], PrintUsingNumberMinus ) )
            {
               tbuf[i] = ' ';
               bwb_strcat(tbuf,"-");
            }
            break;
         }
      }
      if( tbuf[0] == ' ' )
      {
         n = bwb_strlen(tbuf); 
         /* n > 0 */
         for( i = 1; i < n; i++ )
         {
            tbuf[i-1] = tbuf[i];
         }
         tbuf[n-1] = BasicNulChar;
      }
   }
   else
   {
      WARN_INTERNAL_ERROR;
      return FALSE;
   }


   if( CurrChar == ' ' )
   {
      /* do nothing */
   }
   else
   if( IS_CHAR( CurrChar, PrintUsingNumberDollar ) )
   {
      int i;
      int n;

      n = bwb_strlen(tbuf);

      for (i = 0; i < n; i++)
      {
         if (tbuf[i] != ' ')
         {
            if (i > 0)
            {
               if (bwb_isdigit(tbuf[i]))
               {
                  tbuf[i - 1] = CurrChar;
               }
               else
               {
                  /* sign char */
                  tbuf[i - 1] = tbuf[i];
                  tbuf[i] = CurrChar;
               }
            }
            break;
         }
      }
   }
   else
   {
      WARN_INTERNAL_ERROR;
      return FALSE;
   }

   if( FillChar == ' ' )
   {
      /* do nothing */
   }
   else
   if( IS_CHAR( FillChar, PrintUsingNumberFiller ) )
   {
      int i;
      int n;

      n = bwb_strlen(tbuf);

      for (i = 0; i < n; i++)
      {
         if (tbuf[i] != ' ')
         {
            break;
         }
         tbuf[i] = PrintUsingNumberFiller;
      }
   }
   else
   {
      WARN_INTERNAL_ERROR;
      return FALSE;
   }

   if( ComaChar == ' ' )
   {
      prn_xxprintf(tbuf);
   }
   else
   if( IS_CHAR( ComaChar, PrintUsingNumberComma ) )
   {
      int  dig_pos = -1;
      int  dec_pos = -1;
      int  i;
      int  n;
      int  commas;

      n = bwb_strlen(tbuf);

      for (i = 0; i < n; i++)
      {
        if ((bwb_isdigit(tbuf[i]) != 0) && (dig_pos == -1))
        {
           dig_pos = i;
        }
        if ((tbuf[i] == PrintUsingNumberPeriod) && (dec_pos == -1))
        {
           dec_pos = i;
        }
        if ((dig_pos != -1) && (dec_pos != -1))
        {
           break;
        }
      }
      if (dig_pos == -1)
      {
         dec_pos = n;
      }
      if (dec_pos == -1)
      {
         dec_pos = n;
      }
      /* count the number of commas */
      commas = 0;
      for (i = 0; i < n; ++i)
      {
         if (((dec_pos - i) % 3 == 0) && (i > dig_pos) && (i < dec_pos))
         {
            commas++;
         }
      }
      /* now, actually print */
      for( i = 0; i < n; i++ )
      {
         if( i < commas && IS_CHAR( tbuf[i], FillChar ) )
         {
            /* 
            Ignore the same number of leading spaces as there are commas.
            While not perfect for all possible cases, 
            it is usually good enough for practical purposes.
            */
         }
         else
         {
            if (((dec_pos - i) % 3 == 0) && (i > dig_pos) && (i < dec_pos))
            {
              xxputc(PrintUsingNumberComma);
            }
            xxputc(tbuf[i]);
         }
      }
   }
   else
   {
      WARN_INTERNAL_ERROR;
      return FALSE;
   }
   *position = p;
   return TRUE;
}

static int str_prnfmt(char *buffer, int *position, VariantType *e)
{
   /*
   Format a STRING.
   'buffer' points to the beginning of a PRINT USING format string, such as "###.##".
   'position' is the current offset in 'buffer'.
   'e' is the current expression to print.
   */
   int p;
   char tbuf[BasicStringLengthMax + 1];
   
   p = *position;
   
   if( e->TypeChar == BasicStringSuffix )
   {
      bwb_strcpy( tbuf, e->Buffer );
   }
   else
   {
      BasicNumerc( e->Number, tbuf );
   }
   
   if( IS_CHAR( buffer[ p ], PrintUsingStringFirst ) )
   {
      /* print first character only */
      int i = 0;

      if( tbuf[i] == BasicNulChar )
      {
         xxputc(' ');
      }
      else
      {
         xxputc(tbuf[i]);
         i++;
      }
      p++;
   }
   else
   if( IS_CHAR( buffer[ p ], PrintUsingStringAll ) )
   {
      /* print entire string */
      p++;
      prn_xxprintf(tbuf);
   }
   else
   if( IS_CHAR( buffer[ p ], PrintUsingStringLength ) )
   {
      /* print N characters or spaces */
      int i = 0;

      if( tbuf[i] == BasicNulChar )
      {
         xxputc(' ');
      }
      else
      {
         xxputc(tbuf[i]);
         i++;
      }
      p++;

      while( buffer[p] != BasicNulChar && buffer[p] != PrintUsingStringLength )
      {
         if( tbuf[i] == BasicNulChar )
         {
            xxputc(' ');
         }
         else
         {
            xxputc(tbuf[i]);
            i++;
         }
         p++;
      }
      if( buffer[p] == PrintUsingStringLength )
      {
         if( tbuf[i] == BasicNulChar )
         {
            xxputc(' ');
         }
         else
         {
            xxputc(tbuf[i]);
            i++;
         }
         p++;
      }
   }
   *position = p;
   return TRUE;
}

static int is_magic_string( char * buffer )
{
   /* 
   for the character string pointed to 'buffer':
   return TRUE if it is a MagicString sequence,
   return FALSE otherwise.
   */
   char *P;

   /* "!" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingStringFirst ) )
   {
      return TRUE;
   }

   /* "&" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingStringAll ) )
   {
      return TRUE;
   }

   /* "%...%" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingStringLength ) )
   {
      return TRUE;
   }

   return FALSE;
}

static int is_magic_number( char * buffer )
{
   /* 
   for the character string pointed to 'buffer':
   return TRUE if it is a MagicNumber sequence,
   return FALSE otherwise.
   */
   char *P;

   /* "+**" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingNumberPlus ) )
   {
      P++;
      if( IS_CHAR( *P, PrintUsingNumberFiller ) )
      {
         P++;
         if( IS_CHAR( *P, PrintUsingNumberFiller ) )
         {
            /* "+**" */
            return TRUE;
         }
      }
   }

   /* "+$$" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingNumberPlus ) )
   {
      P++;
      if( IS_CHAR( *P, PrintUsingNumberDollar ) )
      {
         P++;
         if( IS_CHAR( *P, PrintUsingNumberDollar ) )
         {
            /* "+$$" */
            return TRUE;
         }
      }
   }

   /* "+#" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingNumberPlus ) )
   {
      P++;
      if( IS_CHAR( *P, PrintUsingNumberDigit ) )
      {
         /* "+#" */
         return TRUE;
      }
   }

   /* "-**" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingNumberMinus ) )
   {
      P++;
      if( IS_CHAR( *P, PrintUsingNumberFiller ) )
      {
         P++;
         if( IS_CHAR( *P, PrintUsingNumberFiller ) )
         {
            /* "-**" */
            return TRUE;
         }
      }
   }

   /* "-$$" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingNumberMinus ) )
   {
      P++;
      if( IS_CHAR( *P, PrintUsingNumberDollar ) )
      {
         P++;
         if( IS_CHAR( *P, PrintUsingNumberDollar ) )
         {
            /* "-$$" */
            return TRUE;
         }
      }
   }

   /* "-#" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingNumberMinus ) )
   {
      P++;
      if( IS_CHAR( *P, PrintUsingNumberDigit ) )
      {
         /* "-#" */
         return TRUE;
      }
   }

   /* "**" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingNumberFiller ) )
   {
      P++;
      if( IS_CHAR( *P, PrintUsingNumberFiller ) )
      {
         /* "**" */
         return TRUE;
      }
   }
   
   /* "$$" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingNumberDollar ) )
   {
      P++;
      if( IS_CHAR( *P, PrintUsingNumberDollar ) )
      {
         /* "$$" */
         return TRUE;
      }
   }

   /* "#" */
   P = buffer;
   if( IS_CHAR( *P, PrintUsingNumberDigit ) )
   {
      /* "#" */
      return TRUE;
   }

   return FALSE;
}

static int get_prnfmt(char *buffer, int *position, VariantType *e)
{
   /*
   Format an EXPRESSION.
   'buffer' points to the beginning of a PRINT USING format string, such as "###.##".
   'position' is the current offset in 'buffer'.
   'e' is the current expression to print.
   */
   


   int p;
   int  IsLoop = TRUE;
   int  IsUsed = FALSE;

   bwx_DEBUG(__FUNCTION__);


   p = *position;
   if( p < 0 )
   {
      p = 0;
   }
   else
   if( p > 0 )
   {
      if( buffer[p] == BasicNulChar )
      {
         p = 0;
      }
   }
   while( IsLoop == TRUE )
   {
      if( buffer[p] == BasicNulChar )
      {
         IsLoop = FALSE;
      }
      else
      {
         int IsLiteral = TRUE;
         
         
         if( IsLiteral == TRUE )
         {
            char * S;

            S = buffer;
            S += p;

            if( is_magic_string( S ) )
            {
               /* MagicString Value */
               if( IsUsed == TRUE )
               {
                  IsLoop = FALSE;
               }
               else
               if( e->TypeChar == BasicStringSuffix )
               {
                  str_prnfmt( buffer, &p, e );
                  IsUsed = TRUE;
               }
               else
               {
                  IsLoop = FALSE;
               }
               IsLiteral = FALSE;
            }
         }
         if( IsLiteral == TRUE )
         {
            char * S;

            S = buffer;
            S += p;

            if( is_magic_number( S ) )
            {
               /* MagicNumber Value */
               if( IsUsed == TRUE )
               {
                  IsLoop = FALSE;
               }
               else
               if( e->TypeChar == BasicStringSuffix )
               {
                  IsLoop = FALSE;
               }
               else
               {
                  num_prnfmt( buffer, &p, e );
                  IsUsed = TRUE;
               }
               IsLiteral = FALSE;
            }
         }
         if( IsLiteral == TRUE )
         {
            if( PrintUsingLiteral != BasicNulChar && buffer[p] == PrintUsingLiteral )
            {
               /* print next character as literal */
               p++;
               if( buffer[p] == BasicNulChar )
               {
                 /* PRINT USING "_" */
                 xxputc(' ');
               }
               else
               {
                 xxputc(buffer[p]);
                 p++;
               }
            }
            else
            {
                 xxputc(buffer[p]);
                 p++;
            }
         }
         
      }
   }

   if( IsUsed == FALSE )
   {

      if( e->TypeChar == BasicStringSuffix )
      {
         /* PRINT USING "";A$    */
         /* PRINT USING "ABC";A$ */
         prn_iprintf(e->Buffer);
      }
      else
      {
         /* PRINT USING "";X     */
         /* PRINT USING "ABC";X  */
         /* [space]number[space]   POSITIVE or ZERO
          * [minus]number[space]   NEGATIVE */
         char tbuf[ 32 ];

         BasicNumerc(e->Number, tbuf);

         if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) && My->CurrentFile->FileNumber > 0 )
         {
            /* CBASIC-II: numbers in files do NOT have leading or trailing spaces */
            char * P;
            P = tbuf;
            while( *P == ' ' )
            {
               P++;
            }
            prn_iprintf( P );
         }
         else
         {
            prn_iprintf(tbuf);
            xputc(' ');
         }
      }
   }
   *position = p;
   return TRUE;
}

/***************************************************************
  
        FUNCTION:       prn_iprintf()
  
   DESCRIPTION:    This function outputs a null-terminated
         string to a specified file or output
         device.
  
***************************************************************/

int
prn_iprintf(char *buffer)
{
   int             n;

   bwx_DEBUG(__FUNCTION__);

   if (My->CurrentFile->width == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      while (*buffer)
      {
         xxxputc(*buffer);
         buffer++;
      }
      return 0;
   }
   /* check to see if width will be exceeded */
   n = My->CurrentFile->col + bwb_strlen(buffer) - 1;
   if (n > My->CurrentFile->width)
   {
      xputc('\n');
   }
   /* output the string */
   while (*buffer)
   {
      xputc(*buffer);
      buffer++;
   }
   return 0;
}

/***************************************************************
  
        FUNCTION:       prn_xxprintf()
  
   DESCRIPTION:    This function outputs a null-terminated
         string to a specified file or output
         device without expanding tabs.
         Added by JBV 10/95
  
***************************************************************/

static int prn_xxprintf(char *buffer)
{
   int             n;

   bwx_DEBUG(__FUNCTION__);

   if (My->CurrentFile->width == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      while (*buffer)
      {
         xxxputc(*buffer);
         buffer++;
      }
      return 0;
   }
   /* check to see if width will be exceeded */
   n = My->CurrentFile->col + bwb_strlen(buffer) - 1;
   if (n > My->CurrentFile->width)
   {
      xxputc('\n');
   }
   /* output the string */
   while (*buffer)
   {
      xxputc(*buffer);
      buffer++;
   }
   return 0;
}

/***************************************************************
  
        FUNCTION:       xputc()
  
   DESCRIPTION:    This function outputs a character to a
         specified file or output device, expanding
         TABbed output approriately.
  
***************************************************************/

static int xputc(char c)
{
   static char     CHR_pending = FALSE;

   bwx_DEBUG(__FUNCTION__);

   if (My->CurrentFile->width == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      xxxputc(c);
      return 0;
   }
   /* check for pending SPC */
   if (CHR_pending == PRN_SPC)
   {
      /* 190 PRINT SPC(A);"X" ' A = 0...255 */
      int             i;
      for (i = 0; i < c; i++)
      {
         xxputc(' ');
      }
      CHR_pending = FALSE;
      return TRUE;
   }
   /* check for pending TAB */
   if (CHR_pending == PRN_TAB)
   {
      /* WIDTH 80 */
      while (c > My->CurrentFile->width)
      {
         /* If n is greater than the margin m, then n is
          * reduced by an integral multiple of m so that it is
          * in the range 1 <= n <= m; */
         c -= My->CurrentFile->width;
      }
      /* 190 PRINT TAB(A);"X" ' A = 0 */
      if (c == 0)
      {
         /* use the value of one */
         c = 1;
         /* continue processing */
      }
      if ((int) c < My->CurrentFile->col)
      {
         xxputc('\n');
      }
      while (My->CurrentFile->col < (int) c)
      {
         xxputc(' ');
      }
      CHR_pending = FALSE;
      return TRUE;
   }
   /* check for specific output options */
   switch (c)
   {
   case PRN_SPC:
   case PRN_TAB:
      CHR_pending = c;
      break;
   case '\t':
      {
         int             LastZoneColumn;
         LastZoneColumn = 1;
         while (LastZoneColumn < My->CurrentFile->width)
         {
            LastZoneColumn += ZONE_WIDTH;
         }
         LastZoneColumn -= ZONE_WIDTH;

         if (My->CurrentFile->col >= LastZoneColumn)
         {
            /* advance to a new line */
            xxputc('\n');
         }
         else
         {
            /* advance to the next print zone */
            if ((My->CurrentFile->col % ZONE_WIDTH) == 1)
            {
               xxputc(' ');
            }
            while ((My->CurrentFile->col % ZONE_WIDTH) != 1)
            {
               xxputc(' ');
            }
         }
      }
      break;
   default:
      xxputc(c);
      break;
   }
   return 0;
}

/***************************************************************
  
        FUNCTION:       xxputc()
  
   DESCRIPTION:    This function outputs a character to a
         specified file or output device, checking
         to be sure the PRINT width is within
         the bounds specified for that device.
  
***************************************************************/

static int xxputc(char c)
{
   bwx_DEBUG(__FUNCTION__);

   if (My->CurrentFile->width == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      xxxputc(c);
      return 0;
   }
   /* check to see if width has been exceeded */
   if (c != '\n')
   {
      /* REM this should print one line, not two lines WIDTH 80
       * PRINT SPACE$( 80 ) */
      if (My->CurrentFile->col > My->CurrentFile->width)
      {
         xxxputc('\n'); /* output LF */
      }
   }
   /* output the character */
   xxxputc(c);

   return 0;
}

/***************************************************************
  
   FUNCTION:       xxxputc()
  
   DESCRIPTION:    This function sends a character to a
         specified file or output device.
  
***************************************************************/


static int xxxputc(char c)
{
   bwx_DEBUG(__FUNCTION__);

   if (My->CurrentFile == My->SYSPRN)
   {
      bwx_LPRINT(c);
      if (c == '\n' && My->SYSPRN->width > 0 && My->LPRINT_NULLS > 0)
      {
         int             i;
         for (i = 0; i < My->LPRINT_NULLS; i++)
         {
            bwx_LPRINT(0);
         }
      }
   }
   else
   {
      if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) && c == '\n' )
      {
         if( My->CurrentFile->mode & DEVMODE_RANDOM && My->CurrentFile->width > 0 )
         {
            /* CBASIC-II: RANDOM files are padded on the right with spaces */
            while( My->CurrentFile->col < My->CurrentFile->width )
            {
               fputc(' ', My->CurrentFile->cfp);
               My->CurrentFile->col++;
            }
         }
      }
      fputc(c, My->CurrentFile->cfp);
   }
   /* update current column position */
   if (My->CurrentFile->width == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      My->CurrentFile->col = 1;
      My->CurrentFile->row = 1;
   }
   else
   if (c == '\n')
   {
      My->CurrentFile->col = 1;
      My->CurrentFile->row ++;
   }
   else
   {
      My->CurrentFile->col ++;
   }
   return 0;
}


void 
ResetConsoleColumn( void )
{
   bwx_DEBUG(__FUNCTION__);

   My->SYSOUT->col = 1;
}


LineType *
bwb_PUT(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
   {
      /* PUT filename$ , value [, ...] */
      VariantType e;       /* no leaks */
      VariantType *E = &e; /* no leaks */

      CLEAR_VARIANT( E );
      line_skip_spaces(l);
      if( line_read_expression( l, E ) == FALSE )
      {
         goto EXIT;
      }
      if( E->TypeChar == BasicStringSuffix )
      {
         /* STRING */
         /* PUT filename$ ... */
         if( is_empty_filename( E->Buffer ) )
         {
            /* "PUT # 0" is an error */
            WARN_BAD_FILE_NUMBER;
            goto EXIT;
         }
         My->CurrentFile = find_file_by_name( E->Buffer );
         if( My->CurrentFile == NULL )
         {
            /* implicitly OPEN for writing */
            My->CurrentFile = file_new();
            My->CurrentFile->cfp = fopen(E->Buffer, "w");
            if( My->CurrentFile->cfp == NULL )
            {
               /* bad file name */
               WARN_BAD_FILE_NUMBER;
               goto EXIT;
            }
            My->CurrentFile->FileNumber = file_next_number();
            My->CurrentFile->mode = DEVMODE_OUTPUT;
            My->CurrentFile->width = 0;
            /* WIDTH == RECLEN */
            My->CurrentFile->col = 1;
            My->CurrentFile->row = 1;
            My->CurrentFile->delimit = ',';
            My->CurrentFile->buffer = NULL;
            bwb_strcpy(My->CurrentFile->filename, E->Buffer);
         }
      }
      else
      {
         /* NUMBER -- file must already be OPEN */
         /* PUT filenumber ... */
         if( E->Number < 0 )
         {
            /* "PUT # -1" is an error */
            WARN_BAD_FILE_NUMBER;
            goto EXIT;
         }
         if( E->Number == 0 )
         {
            /* "PUT # 0" is an error */
            WARN_BAD_FILE_NUMBER;
            goto EXIT;
         }
         /* normal file */
         My->CurrentFile = find_file_by_number( (int) bwb_rint( E->Number ) );
         if( My->CurrentFile == NULL )
         {
            /* file not OPEN */
            WARN_BAD_FILE_NUMBER;
            goto EXIT;
         }
      }  
      RELEASE( E );  
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         goto EXIT;
      }
      if (( My->CurrentFile->mode & DEVMODE_WRITE) == 0)
      {
         WARN_BAD_FILE_NUMBER;
         goto EXIT;
      }
      if( line_is_eol(l) )
      {
         /* PUT F$ */
         /* PUT #1 */
         xputc('\n');
         goto EXIT;
      }
      else
      if (line_skip_comma(l))
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         goto EXIT;
      }
   
      /* loop through elements */
   
      while( ! line_is_eol(l) )
      {
         while (line_skip_comma(l))
         {
            /* PUT F$, ,,,A,,,B,,, */
            /* PUT #1, ,,,A,,,B,,, */
            xputc( My->CurrentFile->delimit );
         }
   
         if ( ! line_is_eol(l) )
         {
            /* print this item */

            /* get the next element */
            line_skip_spaces(l);
            if( line_read_expression( l, E ) == FALSE )
            {
               goto EXIT;
            }
            if( E->TypeChar == BasicStringSuffix )
            {
               /* STRING */
               xputc(BasicQuoteChar);
               prn_iprintf(E->Buffer);
               xputc(BasicQuoteChar);
            }
            else
            {
               /* NUMBER */
               char tbuf[ 32 ];
               BasicNumerc(E->Number, tbuf);
               prn_iprintf(tbuf);
            }  
            RELEASE( E );   
         }
      }
      /* print LF */
      xputc('\n');
      /* OK */
EXIT:
      RELEASE( E );
      return bwb_zline(l);
   }
   else
   if( My->CurrentVersion->OptionVersionBitmask & ( D71 ) )
   {
      /* PUT # file_number [ , RECORD record_number ] */
      int file_number = 0;
      if( line_skip_char( l, BasicFileNumberPrefix ) == FALSE )
      {
         /* OPTIONAL */
      }
      if( line_read_integer_expression( l, &file_number ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      if( file_number < 1 )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      My->CurrentFile = find_file_by_number( file_number );
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      if( My->CurrentFile->mode != DEVMODE_RANDOM )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      if( My->CurrentFile->width <= 0 )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      if( line_is_eol( l ) )
      {
         /* PUT # file_number */
      }
      else
      {
         /* PUT # file_number , RECORD record_number */
         int record_number = 0;
         long offset = 0;
         if( line_skip_comma( l ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline( l );
         }
         if( line_skip_word( l, "RECORD" ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline( l );
         }
         if( line_read_integer_expression( l, &record_number ) == FALSE )
         {
            WARN_BAD_RECORD_NUMBER;
            return bwb_zline( l );
         }
         if( record_number <= 0 )
         {
            WARN_BAD_RECORD_NUMBER;
            return bwb_zline( l );
         }
         record_number--; /* BASIC to C */
         offset = record_number;
         offset *= My->CurrentFile->width;
         if (fseek(My->CurrentFile->cfp, offset, SEEK_SET) != 0)
         {
            WARN_BAD_RECORD_NUMBER;
            return bwb_zline( l );
         }
      }
      field_put( My->CurrentFile );
      /* if( TRUE ) */
      {
         int i;
         for (i = 0; i < My->CurrentFile->width; i++)
         {
            char c;
            c = My->CurrentFile->buffer[i];
            fputc(c, My->CurrentFile->cfp);
         }
      }
      /* OK */
      return bwb_zline( l );
   }
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}


/***************************************************************
  
        FUNCTION:       bwb_write()
  
   DESCRIPTION:    This C function implements the BASIC WRITE
         command.
  
   SYNTAX:     WRITE [# device-number,] element [, element ]....
  
***************************************************************/


LineType *
bwb_WRITE(LineType * l)
{
   int OutputCR;
   VariantType x;       /* no leaks */
   VariantType *X = &x; /* no leaks */

   bwx_DEBUG(__FUNCTION__);
   CLEAR_VARIANT( X );     
   My->CurrentFile = My->SYSOUT;
   if ( line_skip_char( l, BasicFileNumberPrefix ) )
   {
      int             UserFileNumber;

      if( line_read_integer_expression(l, &UserFileNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }

      /* check the requested device number */
      if( UserFileNumber < 0 )
      {
         My->CurrentFile = My->SYSPRN;
      }
      else
      if( UserFileNumber == 0 )
      {
         My->CurrentFile = My->SYSOUT;
      }
      else
      {
         /* normal file */
         My->CurrentFile = find_file_by_number( UserFileNumber );
      }
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if (( My->CurrentFile->mode & DEVMODE_WRITE) == 0)
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( line_is_eol(l) )
      {
         /* WRITE #1 */
         xputc('\n');
         return bwb_zline(l);
      }
      else
      if (line_skip_comma(l))
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }


   /* loop through elements */
   OutputCR = TRUE;
   line_skip_spaces( l );
   while( line_is_eol(l) == FALSE )
   {
      if( line_skip_comma(l) )
      {
         /* WRITE     ,,,A,,,B,,, */
         /* WRITE #1, ,,,A,,,B,,, */
         OutputCR = FALSE;
         xputc( My->CurrentFile->delimit );
      }
      else
      {
         /* print the expression */
         OutputCR = TRUE;
         if( line_read_expression( l, X ) == FALSE )
         {
            goto EXIT;
         }
         if( bwb_Warning_Pending() /* Keep This */ )
         {
            /* 
            this might look odd... 
            but we want to abort printing on the first error.
            The expression list could include a function with side-effects,
            so any kind of error should immediately halt further evaluation.
            */
            goto EXIT;
         }
         if( X->TypeChar == BasicStringSuffix )
         {
            /* STRING */
            xputc(BasicQuoteChar);
            prn_iprintf(X->Buffer);
            xputc(BasicQuoteChar);
         }
         else
         {
            /* NUMBER */
            char tbuf[ 32 ];
            BasicNumerc(X->Number, tbuf);
            prn_iprintf(tbuf);
         }  
         RELEASE( X );   
      }
      line_skip_spaces( l );
   }
   /* print LF */
   if( OutputCR == TRUE )
   {
      xputc('\n');
   }
EXIT:
   RELEASE( X );      
   return bwb_zline(l);
}

static LineType * file_write_matrix( LineType * l, char delimit )
{
   /* MAT PRINT arrayname [;|,] */
   /* Array must be 1, 2 or 3 dimensions */
   /* Array may be either NUMBER or STRING */
   VariableType *v;
   char            ItemSeperator[2];

   bwx_DEBUG(__FUNCTION__);

   /* get the variable name */
   line_skip_spaces(l);
   while( bwb_isalpha( l->buffer[l->position] ) )
   {
      /* get matrix name */
       if((v = line_read_matrix( l )) == NULL)
       {
          WARN_SUBSCRIPT_OUT_OF_RANGE;
          return bwb_zline(l);
       }

       /* variable MUST be an array of 1, 2 or 3 dimensions */
       if (v->dimensions < 1)
       {
          WARN_SUBSCRIPT_OUT_OF_RANGE;
          return bwb_zline(l);
       }
       if(v->dimensions > 3)
       {
          WARN_SUBSCRIPT_OUT_OF_RANGE;
          return bwb_zline(l);
       }


       /* 
       This may look odd, but MAT PRINT is special.
       The variable seperator AFTER the variable determines how 
       the variable's values are printed.
       The number of dimension determines:
       a) the meaning of comma (,) and semicolon (;)
       b) the default of row-by-row or col-by-col
       */
       /* default the item seperator based upon variable's dimensions */
       ItemSeperator[0] = BasicNulChar;
       ItemSeperator[1] = BasicNulChar;
       switch( v->dimensions )
       {
       case 1:
           /* by default, a one dimension array is printed row-by-row */
           ItemSeperator[0] = '\n';
           break;
       case 2:
           /* by default, a two dimension array is printed col-by-col */
           ItemSeperator[0] = delimit;
           break;
       case 3:
           /* by default, a three dimension array is printed col-by-col */
           ItemSeperator[0] = delimit;
           break;
       }
       /* allow user to assign the item seperator */
       if( line_skip_char(l, ',' /* comma-specific */ ))
       {
           /* force printing col-by-col */
           ItemSeperator[0] = delimit;
       }
       else
       if( line_skip_char(l, ';' /* semicolon-specific */ ))
       {
           /* force concatenating the columns */
           ItemSeperator[0] = BasicNulChar;
       }
       /* print array */
       switch( v->dimensions )
       {
       case 1:
           {
               /*
               OPTION BASE 0
               DIM A(5)
               ...
               MAT PRINT A 
               ...
               FOR I = 0 TO 5
                 PRINT A(I)
               NEXT I
               ...
               */
               for( v->array_pos[0] = v->LBOUND[0]; v->array_pos[0] <= v->UBOUND[0]; v->array_pos[0]++ )
               {
                  char tbuf[BasicStringLengthMax + 1];
                  
                   if( ItemSeperator[0] != BasicNulChar && v->array_pos[0] > v->LBOUND[0] )
                   {
                       prn_iprintf(ItemSeperator);
                   }
                  /* if( TRUE ) */
                  {
                     VariantType variant;
                     if( var_get( v, &variant) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }
                     if( variant.TypeChar == '$' )
                     {
                        bwb_strcpy( tbuf, variant.Buffer );
                     }
                     else
                     {
                        BasicNumerc( variant.Number, tbuf );
                     }
                  }
                   prn_iprintf(tbuf);
               }
               prn_iprintf("\n");
           }
           break;
       case 2:
           {
               /*
               OPTION BASE 0
               DIM B(2,3)
               ...
               MAT PRINT B 
               ...
               FOR I = 0 TO 2
                   FOR J = 0 TO 3
                       PRINT B(I,J),
                   NEXT J
                   PRINT
               NEXT I
               ...
               */
               for( v->array_pos[0] = v->LBOUND[0]; v->array_pos[0] <= v->UBOUND[0]; v->array_pos[0]++ )
               {
               for( v->array_pos[1] = v->LBOUND[1]; v->array_pos[1] <= v->UBOUND[1]; v->array_pos[1]++ )
               {
                  char tbuf[BasicStringLengthMax + 1];

                   if( ItemSeperator[0] != BasicNulChar && v->array_pos[1] > v->LBOUND[1] )
                   {
                       prn_iprintf(ItemSeperator);
                   }
                  /* if( TRUE ) */
                  {
                     VariantType variant;
                     if( var_get( v, &variant) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }
                     if( variant.TypeChar == '$' )
                     {
                        bwb_strcpy( tbuf, variant.Buffer );
                     }
                     else
                     {
                        BasicNumerc( variant.Number, tbuf );
                     }
                  }
                   prn_iprintf(tbuf);
               }
               prn_iprintf("\n");
               }
           }
           break;
       case 3:
           {
               /*
               OPTION BASE 0
               DIM C(2,3,4)
               ...
               MAT PRINT C 
               ...
               FOR I = 0 TO 2
                   FOR J = 0 TO 3
                       FOR K = 0 TO 4
                           PRINT C(I,J,K),
                       NEXT K
                       PRINT
                   NEXT J
                   PRINT
               NEXT I
               ...
               */
               for( v->array_pos[0] = v->LBOUND[0]; v->array_pos[0] <= v->UBOUND[0]; v->array_pos[0]++ )
               {
               for( v->array_pos[1] = v->LBOUND[1]; v->array_pos[1] <= v->UBOUND[1]; v->array_pos[1]++ )
               {
               for( v->array_pos[2] = v->LBOUND[2]; v->array_pos[2] <= v->UBOUND[2]; v->array_pos[2]++ )
               {
                  char tbuf[BasicStringLengthMax + 1];

                   if( ItemSeperator[0] != BasicNulChar && v->array_pos[2] > v->LBOUND[2] )
                   {
                       prn_iprintf(ItemSeperator);
                   }
                  /* if( TRUE ) */
                  {
                     VariantType variant;
                     if( var_get( v, &variant) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }
                     if( variant.TypeChar == '$' )
                     {
                        bwb_strcpy( tbuf, variant.Buffer );
                     }
                     else
                     {
                        BasicNumerc( variant.Number, tbuf );
                     }
                  }
                   prn_iprintf(tbuf);
               }
               prn_iprintf("\n");
               }
               prn_iprintf("\n");
               }
           }
           break;
       }
       /* skip spaces */
       line_skip_spaces(l);
       /* process the next variable, if any  */
   }
   return bwb_zline(l);
}


static LineType * bwb_mat_dump(LineType * l, int IsWrite )
{
   /* MAT PRINT arrayname [;|,] */
   /* Array must be 1, 2 or 3 dimensions */
   /* Array may be either NUMBER or STRING */
   char delimit;

   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSOUT;
   if ( line_skip_char( l, BasicFileNumberPrefix ) )
   {
      int             UserFileNumber;

      if( line_read_integer_expression(l, &UserFileNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }

      /* check the requested device number */
      if( UserFileNumber < 0 )
      {
         My->CurrentFile = My->SYSPRN;
      }
      else
      if( UserFileNumber == 0 )
      {
         My->CurrentFile = My->SYSOUT;
      }
      else
      {
         /* normal file */
         My->CurrentFile = find_file_by_number( UserFileNumber );
      }
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if ((My->CurrentFile->mode & DEVMODE_WRITE) == 0)
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if (line_skip_comma(l))
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }

   if( IsWrite )
   {
      /* MAT WRITE */
      delimit = My->CurrentFile->delimit;
   }
   else
   {
      /* MAT PRINT */
      delimit = '\t'; 
   }
   return file_write_matrix( l, delimit );
}


LineType *
bwb_MAT_PUT(LineType * l)
{
   /* MAT PUT filename$ , matrix [, ...] */

   VariantType x;       /* no leaks */
   VariantType *X = &x; /* no leaks */

   bwx_DEBUG(__FUNCTION__);
   CLEAR_VARIANT( X );

   My->CurrentFile = My->SYSOUT;

   line_skip_spaces(l);
   if( line_read_expression( l, X ) == FALSE )
   {
      goto EXIT;
   }
   if( X->TypeChar == BasicStringSuffix )
   {
      /* STRING */
      /* MAT PUT filename$ ... */
      if( is_empty_filename( X->Buffer ) )
      {
         /* "MAT PUT # 0" is an error */
         WARN_BAD_FILE_NUMBER;
         goto EXIT;
      }
      My->CurrentFile = find_file_by_name( X->Buffer );
      if( My->CurrentFile == NULL )
      {
         /* implicitly OPEN for writing */
         My->CurrentFile = file_new();
         My->CurrentFile->cfp = fopen( X->Buffer, "w" );
         if( My->CurrentFile->cfp == NULL )
         {
            /* bad file name */
            WARN_BAD_FILE_NUMBER;
            goto EXIT;
         }
         My->CurrentFile->FileNumber = file_next_number();
         My->CurrentFile->mode = DEVMODE_OUTPUT;
         My->CurrentFile->width = 0;
         /* WIDTH == RECLEN */
         My->CurrentFile->col = 1;
         My->CurrentFile->row = 1;
         My->CurrentFile->delimit = ',';
         My->CurrentFile->buffer = NULL;
         bwb_strcpy( My->CurrentFile->filename, X->Buffer );
      }
   }
   else
   {
      /* NUMBER -- file must already be OPEN */
      /* PUT filenumber ... */
      if( X->Number < 0 )
      {
         /* "MAT PUT # -1" is an error */
         WARN_BAD_FILE_NUMBER;
         goto EXIT;
      }
      if( X->Number == 0 )
      {
         /* "MAT PUT # 0" is an error */
         WARN_BAD_FILE_NUMBER;
         goto EXIT;
      }
      /* normal file */
      My->CurrentFile = find_file_by_number( (int) bwb_rint( X->Number ) );
      if( My->CurrentFile == NULL )
      {
         /* file not OPEN */
         WARN_BAD_FILE_NUMBER;
         goto EXIT;
      }
   }  
   RELEASE( X );  
   if( My->CurrentFile == NULL )
   {
      WARN_BAD_FILE_NUMBER;
      goto EXIT;
   }
   if (( My->CurrentFile->mode & DEVMODE_WRITE) == 0)
   {
      WARN_BAD_FILE_NUMBER;
      goto EXIT;
   }
   if ( line_skip_comma(l) )
   {
      /* OK */
   }
   else
   {
      WARN_SYNTAX_ERROR;
      goto EXIT;
   }
   return file_write_matrix( l, My->CurrentFile->delimit );
EXIT:
   RELEASE( X );  
   return bwb_zline(l);
}


LineType *
bwb_MAT_WRITE(LineType * l)
{
   return bwb_mat_dump( l, TRUE );
}

LineType *
bwb_MAT_PRINT(LineType * l)
{
   return bwb_mat_dump( l, FALSE );
}



/*  EOF  */
