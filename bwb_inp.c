/***************************************************************
  
        bwb_inp.c       Input Routines
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





/* Declarations of functions visible to this file only */




static LineType *bwb_xinp(LineType * l, FILE * f, char delimit);
static int       inp_str( /* LineType * l, */ char *buffer, char *var_list, int *position, int IsFake);
static int       inp_const(char *m_buffer, char *s_buffer, int *position);
static int       inp_assign(char *b, VariableType * v, int IsFake, int IsInput);
static int       read_data(VariableType *v);




/* ResultCode */
#define RESULT_OK 0
#define RESULT_UNKNOWN 1
#define RESULT_TYPE_MMISMATCH 2
#define RESULT_ARITHMETIC_OVERFLOW 3




/***************************************************************
  
      FUNCTION:       bwx_input()
  
   DESCRIPTION:    This function outputs the string pointed
         to by 'prompt', then inputs a character
         string.
  
***************************************************************/

int bwb_is_eof( FILE * fp )
{
   /* 
   Have you ever wondered why C file I/O is slow?   Here is the reason:
   feof() is not set until after a file read error occurs; sad but true. 
   In order to determine whether you are at the end-of-file,
   you have to call both ftell() and fseek() twice,
   which effectively trashes any I/O cache scheme.  
   */
   if( fp != NULL )
   {
      long current;
      long total;
   
      current = ftell( fp );
      fseek( fp, 0, SEEK_END );
      total = ftell( fp ) ;
      if( total == current )
      {
         /* EOF */
         return TRUE;
      }
      else
      {
         /* NOT EOF */
         fseek( fp, current, SEEK_SET );
         return FALSE;
      }
   }
   /* a closed file is always EOF */
   return TRUE;
}


static void clean_cr_lf( char * buffer )
{
   /* 
   some compilers remove CR, but not LF.
   some compilers remove LF, but not CR.
   some compilers remove CR/LF but not LF/CR.
   some compilers remove either CR or LF.
   some compilers remove first CR or LF, but not second LF or CR.
   */
   
    char *E;
    E = bwb_strchr(buffer, '\r');
    if( E != NULL )
    {
       *E = BasicNulChar;
    }
    E = bwb_strchr(buffer, '\n');
    if( E != NULL )
    {
       *E = BasicNulChar;
    }
}

int
bwx_input(char *prompt, char *buffer)
{

   bwx_DEBUG(__FUNCTION__);

   prn_xprintf(prompt);

   fflush( My->SYSOUT->cfp );

   /* for automated testing, TAPE command */
   if (My->IsCommandLineFile == TRUE)
   {
      if ( My->ExternalInputFile != NULL )
      {
         if( fgets(buffer, BasicStringLengthMax, My->ExternalInputFile) == NULL
         ||  feof( My->ExternalInputFile )
         )
         {
            /* stop reading from external file once all INPUT lines have been read */
            fclose(My->ExternalInputFile); /* My->ExternalInputFile != NULL */
            My->ExternalInputFile = NULL;
         }
         else
         {
            fputs( buffer, My->SYSOUT->cfp );
            fflush(My->SYSOUT->cfp);
            clean_cr_lf( buffer );
            ResetConsoleColumn();
            return TRUE;
         }
      }
   }
   fgets(buffer, BasicStringLengthMax, My->SYSIN->cfp);
   clean_cr_lf( buffer );
   ResetConsoleColumn();
   return TRUE;
}


LineType *
bwb_BACKSPACE(LineType * l)
{

   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSIN;

   if ( line_skip_char(l,BasicFileNumberPrefix) )
   {
      /* BACKSPACE # filenum */
      int             FileNumber;

      if( line_read_integer_expression(l, &FileNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      } 
      if( FileNumber < 0 )
      {
         /* "BACKSPACE # -1" is silently ignored */
         return bwb_zline(l);
      }   
      if( FileNumber == 0 )
      {
         /* "BACKSPACE # 0" is silently ignored */
         return bwb_zline(l);
      }   
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if ((My->CurrentFile->mode & DEVMODE_READ) == 0)
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      /* not for the console */
      /* if( TRUE ) */
      {
         FILE * f;
         long Offset;
         int DelimiterCount;
         int InQuote;
         int C;

         f = My->CurrentFile->cfp;
         Offset = ftell( f );
         Offset--;
         DelimiterCount = 0;
         InQuote = FALSE;
         
         AGAIN:
         if( Offset <= 0 )
         {
            goto DONE;
         }
         fseek( f, Offset, SEEK_SET );
         C = fgetc( f );
         
         if( InQuote )
         {
            if( C == BasicQuoteChar )
            {
               InQuote = FALSE;
            }
            Offset--;
            goto AGAIN;
         }
         
         if( C == BasicQuoteChar )
         {
            InQuote = TRUE;
            Offset--;
            goto AGAIN;
         }
         
         
         if( C == ',' )
         {
            DelimiterCount++;
            if( DelimiterCount > 1 )
            {
               Offset++;
               goto DONE;
            }
            Offset--;
            goto AGAIN;
         }
         
         if( C == '\n' )
         {
            DelimiterCount++;
            if( DelimiterCount > 1 )
            {
               Offset++;
               goto DONE;
            }
            Offset--;
            if( Offset <= 0 )
            {
               goto DONE;
            }
            fseek( f, Offset, SEEK_SET );
            C = fgetc( f );
            if( C == '\r' )
            {
               Offset--;
            }
            goto AGAIN;
         }
         
         if( C == '\r' )
         {
            DelimiterCount++;
            if( DelimiterCount > 1 )
            {
               Offset++;
               goto DONE;
            }
            Offset--;
            if( Offset <= 0 )
            {
               goto DONE;
            }
            fseek( f, Offset, SEEK_SET );
            C = fgetc( f );
            if( C == '\n' )
            {
               Offset--;
            }
            goto AGAIN;
         }
         
         Offset--;
         goto AGAIN;
         
         DONE:
         if( Offset < 0 )
         {
            Offset = 0;
         }
         fseek( f, Offset, SEEK_SET );
      }
   }
   /* BACKSPACE for console is silently ignored */
   return bwb_zline(l);
}




/***************************************************************
  
        FUNCTION:       bwb_read()
  
        DESCRIPTION:    This function implements the BASIC READ
                        statement.
  
        SYNTAX:         READ variable[, variable...]
  
***************************************************************/

LineType *
bwb_READ(LineType * l)
{

   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSIN;

   if ( line_skip_char(l,BasicFileNumberPrefix) )
   {
      /* READ # filenum, varlist */
      int             FileNumber;

      if( line_read_integer_expression(l, &FileNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }

      if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
      {
         /* 
         CBASIC-II: SERIAL & RANDOM file reads
         READ  # file_number                 ; [ scalar_variable   [ , ... ] ] ' SERIAL
         READ  # file_number , record_number ; [ scalar_variable   [ , ... ] ] ' RANDOM
         */
         if( FileNumber <= 0 )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         /* normal file */
         My->CurrentFile = find_file_by_number( FileNumber );
         if( My->CurrentFile == NULL )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if( line_skip_char( l, ',' )  /* comma-specific */ )
         {
            /* 
            READ # file_number , record_number ; scalar [, scalar] ' RANDOM read
            */
            /* get the RecordNumber */
            int RecordNumber;
            
            if( (My->CurrentFile->mode & DEVMODE_RANDOM) == 0 )
            {
               WARN_BAD_FILE_MODE;
               return bwb_zline(l);
            }
            if( My->CurrentFile->width <= 0 )
            {
               WARN_FIELD_OVERFLOW;
               return bwb_zline(l);
            }
            if( line_read_integer_expression( l, &RecordNumber ) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline(l);
            }
            if( RecordNumber <= 0 )
            {
               WARN_BAD_RECORD_NUMBER;
               return bwb_zline(l);
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
            /* READ # filenum          */
            /* READ # filenum , recnum */
         }
         else
         if( line_skip_char( l, ';' ) )
         {
            /* READ # filenum          ; */
            /* READ # filenum , recnum ; */
         }
         else
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         /* input is not from #0, so branch to bwb_xinp() */
         return bwb_xinp(l, My->CurrentFile->cfp, My->CurrentFile->delimit);
      }
      /* 
      SERIAL file reads:
      READ # file_number   
      READ # file_number [, scalar]
      */
      if ( line_skip_comma(l) )
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( FileNumber < 0 )
      {
         /* "READ # -1" is an error */
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( FileNumber > 0 )
      {
         /* normal file */
         My->CurrentFile = find_file_by_number( FileNumber );
         if( My->CurrentFile == NULL )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if ((My->CurrentFile->mode & DEVMODE_READ) == 0)
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         /* input is not from #0, so branch to bwb_xinp() */
         return bwb_xinp(l, My->CurrentFile->cfp, My->CurrentFile->delimit);
      }
      /* "READ # 0, varlist" is the same as "READ varlist" */
   }
   /* READ varlist */
   do
   {
      VariableType *v;

      /* get a variable */
      if( (v = line_read_scalar( l )) == NULL )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      /* READ data into the variable */
      if( read_data(v) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }
   while( line_skip_comma(l) );
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_data()
  
        DESCRIPTION:    This function implements the BASIC DATA
                        statement, although at the point at which
                        DATA statements are encountered, no
                        processing is done.  All actual processing
                        of DATA statements is accomplished by READ
                        (bwb_read()).
  
        SYNTAX:         DATA constant[, constant]...
  
***************************************************************/

LineType *
bwb_DATA(LineType * l)
{

   bwx_DEBUG(__FUNCTION__);

   line_skip_eol(l);

   return bwb_zline(l);
}






/***************************************************************
  
        FUNCTION:       bwb_restore()
  
        DESCRIPTION:    This function implements the BASIC RESTORE
                        statement.
  
        SYNTAX:         RESTORE [line number]
  
***************************************************************/

LineType *
bwb_RESET(LineType * l)
{
   /* RESET filename$ [, ...] */      
   VariantType E;
   VariantType *e = &E;  /* no leaks */

   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSIN;

   do
   {
      line_skip_spaces(l);
      if( line_read_expression( l, e ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( e->TypeChar == BasicStringSuffix )
      {
         /* STRING */
         /* RESET filename$ ... */
         My->CurrentFile = find_file_by_name( e->Buffer );
      }
      else
      {
         /* NUMBER -- file must already be OPEN */
         /* RESET filenumber ... */
         My->CurrentFile = find_file_by_number( (int) bwb_rint( e->Number ) );
      }  
      RELEASE( e );  
      if( My->CurrentFile == NULL )
      {
         /* file not OPEN */
         /* silently ignored */
      }
      else
      if( My->CurrentFile == My->SYSIN )
      {
         /* silently ignored */
      }
      else
      if( My->CurrentFile == My->SYSOUT )
      {
         /* silently ignored */
      }
      else
      if( My->CurrentFile == My->SYSPRN )
      {
         /* silently ignored */
      }
      else
      {
         /* normal file is OPEN */
         My->CurrentFile->width = 0;
         My->CurrentFile->col = 1;
         My->CurrentFile->row = 1;
         My->CurrentFile->delimit = ',';
         fseek( My->CurrentFile->cfp, 0, SEEK_SET );
      }
   }
   while( line_skip_comma(l) );
   return bwb_zline(l);
}

LineType *
bwb_CLOSE(LineType * l)
{
   /* CLOSE filename$ [, ...] */      
   VariantType E;
   VariantType *e = &E;  /* no leaks */

   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSIN;

   do
   {
      line_skip_spaces(l);
      if( line_read_expression( l, e ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( e->TypeChar == BasicStringSuffix )
      {
         /* STRING */
         /* CLOSE filename$ ... */
         My->CurrentFile = find_file_by_name( e->Buffer );
      }
      else
      {
         /* NUMBER -- file must already be OPEN */
         /* CLOSE filenumber ... */
         My->CurrentFile = find_file_by_number(  (int) bwb_rint( e->Number ) );
      }  
      RELEASE( e );  
      if( My->CurrentFile == NULL )
      {
         /* file not OPEN */
         /* silently ignored */
      }
      else
      if( My->CurrentFile == My->SYSIN )
      {
         /* silently ignored */
      }
      else
      if( My->CurrentFile == My->SYSOUT )
      {
         /* silently ignored */
      }
      else
      if( My->CurrentFile == My->SYSPRN )
      {
         /* silently ignored */
      }
      else
      {
         /* normal file is OPEN */
         file_clear( My->CurrentFile );
      }
   }
   while( line_skip_comma(l) );
   return bwb_zline(l);
}


LineType *
bwb_RESTORE(LineType * l)
{
   int LineNumber;
   LineType *x;

   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSIN;

   if( My->CurrentVersion->OptionVersionBitmask & ( I70 ) )
   {
      /* RESTORE [comment] */      
      line_skip_eol(l);
      My->data_line =  My->bwb_start.next;
      My->data_pos =  My->data_line->Startpos;
      return bwb_zline(l);
   }


   /* get the first element beyond the starting position */

   if( line_skip_char(l, BasicFileNumberPrefix ) )
   {
      /* RESTORE # X */
      int FileNumber;

      if( line_read_integer_expression( l, &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( FileNumber < 0 )
      {
         /* "RESTORE # -1" is silently ignored */
         return bwb_zline(l);
      }
      if( FileNumber > 0 )
      {
         /* normal file */
         My->CurrentFile = find_file_by_number( FileNumber );
         if( My->CurrentFile == NULL )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if( My->CurrentFile->mode != DEVMODE_CLOSED )
         {
            if( My->CurrentFile->cfp != NULL )
            {
               fclose( My->CurrentFile->cfp ); /* My->CurrentFile->cfp != NULL */
            }
            if( My->CurrentFile->buffer != NULL )
            {
               FREE( My->CurrentFile->buffer, "bwb_FILES" );
            }
         }
         My->CurrentFile->width = 0;
         My->CurrentFile->col = 1;
         My->CurrentFile->row = 1;
         My->CurrentFile->delimit = ',';
         My->CurrentFile->buffer = NULL;
         My->CurrentFile->mode = DEVMODE_CLOSED;
         if( bwb_strcmp( My->CurrentFile->filename, "*" ) != 0 )
         {
            if( (My->CurrentFile->cfp = fopen( My->CurrentFile->filename, "r" )) == NULL )
            {
               WARN_BAD_FILE_NAME;
               return bwb_zline(l);
            }
            My->CurrentFile->mode = DEVMODE_INPUT;
         }
         /* OK */
         return bwb_zline(l);
      }
      /* "RESTORE # 0" is the same as "RESTORE"  */
   }
   if( line_is_eol(l) == TRUE )
   {
      /* RESTORE */
      My->data_line =  My->bwb_start.next;
      My->data_pos =  My->data_line->Startpos;
      return bwb_zline(l);
   }
   /* RESTORE linenumber */
   if( line_read_integer_expression(l, &LineNumber) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* check for target label */
   x = find_line_number( LineNumber, TRUE );
   if (x != NULL)
   {
      /* reassign  My->data_line */
      My->data_line = x;
      My->data_pos = x->Startpos;
      return bwb_zline(l);
   }
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_input()
  
        DESCRIPTION:    This function implements the BASIC INPUT
                        statement.
  
        SYNTAX:         INPUT [;][prompt$;]variable[$,variable]...
                        INPUT#n variable[$,variable]...
  
***************************************************************/

LineType *
bwb_GET(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
   {
      /* GET filename$ , scalar [, ...] */
      VariantType E;
      VariantType *e = &E;  
   
   
      My->CurrentFile = My->SYSIN;
   
      line_skip_spaces(l);
      if( line_read_expression( l, e ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( e->TypeChar == BasicStringSuffix )
      {
         /* STRING */
         /* GET filename$ ... */
         if( is_empty_filename( e->Buffer ) )
         {
            /* "GET # 0" is an error */
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         My->CurrentFile = find_file_by_name( e->Buffer );
         if( My->CurrentFile == NULL )
         {
            /* implicitly OPEN for reading */
            My->CurrentFile = file_new();
            My->CurrentFile->cfp = fopen(e->Buffer, "r");
            if( My->CurrentFile->cfp == NULL )
            {
               /* bad file name */
               WARN_BAD_FILE_NUMBER;
               return bwb_zline(l);
            }
            My->CurrentFile->FileNumber = file_next_number();
            My->CurrentFile->mode = DEVMODE_INPUT;
            My->CurrentFile->width = 0;
            /* WIDTH == RECLEN */
            My->CurrentFile->col = 1;
            My->CurrentFile->row = 1;
            My->CurrentFile->delimit = ',';
            My->CurrentFile->buffer = NULL;
            bwb_strcpy(My->CurrentFile->filename, e->Buffer);
         }
      }
      else
      {
         /* NUMBER -- file must already be OPEN */
         /* GET filenumber ... */
         if( e->Number < 0 )
         {
            /* "GET # -1" is an error */
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if( e->Number == 0 )
         {
            /* "GET # 0" is an error */
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         My->CurrentFile = find_file_by_number( (int) bwb_rint( e->Number ) );
         if( My->CurrentFile == NULL )
         {
            /* file not OPEN */
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
      }  
      RELEASE( e );  
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if (( My->CurrentFile->mode & DEVMODE_READ) == 0)
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if ( line_skip_comma(l) )
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      return bwb_xinp(l, My->CurrentFile->cfp, My->CurrentFile->delimit);
   }
   else
   if( My->CurrentVersion->OptionVersionBitmask & ( D71 ) )
   {
      /* GET # file_number [ , RECORD record_number ] */
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
         /* GET # file_number */
      }
      else
      {
         /* GET # file_number , RECORD record_number */
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
      /* if( TRUE ) */
      {
         int i;
         for (i = 0; i < My->CurrentFile->width; i++)
         {
            int c;
            c = fgetc( My->CurrentFile->cfp );
            if( /* EOF */ c < 0 )
            {
               c = BasicNulChar;
            }
            My->CurrentFile->buffer[i] = c;
         }
      }
      field_get( My->CurrentFile );
      /* OK */
      return bwb_zline( l );
   }
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_INPUT(LineType * l)
{
   int             is_prompt;
   int             suppress_qm;
   char            tbuf[BasicStringLengthMax + 1];
   char            pstring[BasicStringLengthMax + 1];
   int             Loop;
   int             LastPosition;

   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSIN;

   pstring[0] = BasicNulChar;

   if ( line_skip_char(l,BasicFileNumberPrefix) )
   {
      /* INPUT # X */
      int FileNumber;

      if( line_read_integer_expression(l, &FileNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if ( line_skip_comma(l) )
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( FileNumber < 0 )
      {
         /* "INPUT # -1" is an error */
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( FileNumber > 0 )
      {
         /* normal file */
         My->CurrentFile = find_file_by_number( FileNumber );
         if( My->CurrentFile == NULL )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if ((My->CurrentFile->mode & DEVMODE_READ) == 0)
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         return bwb_xinp(l, My->CurrentFile->cfp, ',' );
      }
      /* "INPUT #0, varlist" is the same as "INPUT varlist"  */
   }
   /* from this point we presume that input is from My->SYSIN */

   /* check for a semicolon or a quotation mark, not in first position:
    * this should indicate a prompt string */

   suppress_qm = is_prompt = FALSE;

   line_skip_spaces(l);

   if( line_skip_comma( l ) )
   {
      /* INPUT ; "prompt" ... */
      /* INPUT , "prompt" ... */
      suppress_qm = TRUE;
   }
   if( line_peek_char( l, BasicQuoteChar ) )
   {
      is_prompt = TRUE;
   }

   /* get prompt string and print it */

   if (is_prompt == TRUE)
   {
      /* get string element */
      inp_const(l->buffer, pstring, &(l->position)); /* bwb_INPUT prompt */

      /* advance past semicolon to beginning of variable */
      /*--------------------------------------------------------*/
      /* Since inp_const was just called and inp_adv is called  */
      /* within that, it will have already noted and passed the */
      /* comma by the time it gets here.  Therefore one must    */
      /* refer instead to the last returned value for inp_adv!  */
      /* (JBV, 10/95)                                           */
      /*--------------------------------------------------------*/
      /* suppress_qm = inp_adv( l->buffer, &( l->position ) ); */
      suppress_qm = My->last_inp_adv_rval;

   }        /* end condition: prompt string */
   /* print out the question mark delimiter unless it has been suppressed */

   if (suppress_qm != TRUE)
   {
      pstring[ BasicStringLengthMax - 2 ] = BasicNulChar;
      bwb_strcat(pstring, "? ");
   }

   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      /* 
      CBASIC-II: INPUT "prompt" ; LINE variable$
      */
      if( line_skip_word( l, "LINE" ) )
      {
         /* INPUT "prompt" ; LINE variable$ */
         VariableType * v;
         
         if( (v = line_read_scalar( l )) == NULL )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline( l );
         }
         if ( VAR_IS_STRING( v ) )
         {
            VariantType variant;
            
            bwx_input(pstring, tbuf);
            bwb_stripcr(tbuf);
            
            variant.TypeChar = '$';
            variant.Buffer = tbuf;
            variant.Length = bwb_strlen( variant.Buffer );
            if( var_set( v, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return bwb_zline( l );
            }
            return bwb_zline( l );
         }
         WARN_TYPE_MISMATCH;
         return bwb_zline( l );
      }
   }


   Loop = TRUE;
   LastPosition = l->position;
   while (Loop == TRUE)
   {
      /* read a line into the input buffer */
      int Result;
      
      bwx_input(pstring, tbuf);
      bwb_stripcr(tbuf);

      /* reset print column to account for LF at end of fgets() */
      ResetConsoleColumn();

      l->position = LastPosition;
      
      Result = inp_str( /* l, */ tbuf, l->buffer, &(l->position), TRUE); /* bwb_INPUT */
      if( Result > 0 ) /* bwb_INPUT */
      {
         /* successful input, FAKE run  */
         l->position = LastPosition;
         Result = inp_str( /* l, */ tbuf, l->buffer, &(l->position), FALSE); /* bwb_INPUT */
         if( Result > 0 ) 
         {
            /* successful input, REAL run  */
            Loop = FALSE;
         }
      }
      else
      if( Result < 0 )
      {
         /* syntax error, FAKE run  */
         Loop = FALSE;
      }
      else
      {
         fputs( "*** Retry INPUT ***\n", My->SYSOUT->cfp );
      }
   }        /* while( Loop == TRUE ) */
   return bwb_zline(l);;


}


static int file_read_value( FILE * f, char delimit, VariableType *v )
{
   char            tbuf[BasicStringLengthMax + 1];
   int             c;   /* character */


         /* advance beyond whitespace or comma in data buffer */
         /* Advance to next line if end of buffer */
         /* advance beyond whitespace in data buffer */
         /* leading whitespace is NOT part of the DATA item */
         do
         {
            c = fgetc(f);
            if (c < 0)
            {
               /* EOF */
               return FALSE;
            }
            if( c == delimit )
            {
               break;
            }
         }
         while ( ! bwb_isgraph(c) );

         /* now at last we have a variable in v that needs to
          * be assigned data from the data_buffer at position
          * My->data_pos.  What remains to be done is to
          * get one single bit of data, a string constant or
          * numerical constant, into the small buffer */

         {

            int             string; /* a quoted string */
            int             s_pos;
            int             loop;


            string = FALSE;
            s_pos = 0;
            loop = TRUE;

            /* build the constant string */
            while (loop == TRUE)
            {

               if (c == delimit)
               {
                  if (string == FALSE)
                  {
                     /* end of argument */
                     loop = FALSE;
                  }
                  else
                  {
                     /* internal comma is
                      * part of the DATA
                      * item */
                     tbuf[s_pos] = c;
                     ++s_pos;
                  }
               }
               else
               if ( ! bwb_isprint(c) )
               {
                  /* END-OF-LINE */
                  loop = FALSE;
               }
               else
               if (c == BasicQuoteChar)
               {
                  /* Once we finish reading a
                   * quoted string,  we want
                   * to continue reading spaces 
                   * until EOL or comma */
                  if (string == TRUE)
                  {
                     string = FALSE;
                  }
                  else
                  {
                     string = TRUE;
                  }
                  tbuf[s_pos] = c;
                  ++s_pos;
               }
               else
               {
                  tbuf[s_pos] = c;
                  ++s_pos;
               }
               if (loop == TRUE)
               {
                  /* read another character */
                  c = fgetc(f);
                  if (c < 0)
                  {
                     /* EOF */
                     return FALSE;
                  }
               }
            }

            if (string == FALSE)
            {
               /* trailing whitespace is NOT part of
                * the DATA item */
               while (s_pos > 0 && tbuf[s_pos - 1] == ' ')
               {
                  s_pos--;
               }
            }
            tbuf[s_pos] = BasicNulChar;
            /* clean-up quoted string */
            if (s_pos > 0 && tbuf[0] == BasicQuoteChar)
            {
               /* not an empty string */
               char           *Q;

               Q = bwb_strrchr(&tbuf[1], BasicQuoteChar);
               if (Q != NULL)
               {
                  *Q = BasicNulChar;
               }
               bwb_strcpy(tbuf, &(tbuf[1]) );
            }
         }
         /* if( TRUE ) */
         {
            VariantType variant;

            variant.TypeChar = v->VariableTypeChar;

            if ( VAR_IS_STRING( v ) )
            {
               variant.Buffer = tbuf;
               variant.Length = bwb_strlen( variant.Buffer );
            }
            else
            {
               /* N = VAL( A$ ) */
               BasicNumberType Value = 0;
               if (tbuf[0] != BasicNulChar)
               {
                  int             ScanResult;
                  ScanResult = sscanf(tbuf, BasicNumberScanFormat, &Value);
                  if (ScanResult != 1)
                  {
                     /* not a number */
                     Value = 0;
                  }
                  else
                  {
                     /* OK */
                  }
               }
               variant.Number = Value;
            }
            if( var_set( v, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return FALSE;
            }
            
         }
         /* OK */
         return TRUE;
}

static LineType * bwb_xinp(LineType * l, FILE * f, char delimit)
{
   /* INPUT # is similar to READ, where each file line is a DATA line */

   int             main_loop;
   VariableType *v;


   bwx_DEBUG(__FUNCTION__);

   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      /* 
      CBASIC-II: READ # filenumber [, recnum ] ; LINE variable$
      */
      if( line_skip_word( l, "LINE" ) )
      {
         /* INPUT "prompt" ; LINE variable$ */
         VariableType * v;
         
         if( (v = line_read_scalar( l )) == NULL )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline( l );
         }
         if ( VAR_IS_STRING( v ) )
         {
            char tbuf[ BasicStringLengthMax + 1 ];
            
            if( fgets( tbuf, BasicStringLengthMax, f ) == NULL || feof( f ) )
            {
               /* IF END # file_number THEN line_number */
               if( My->CurrentFile->EOF_LineNumber > 0 )
               {
                  LineType *x;
                  
                  x = find_line_number( My->CurrentFile->EOF_LineNumber, TRUE ); /* not found in the cache */
                  if (x != NULL)
                  {
                     /* FOUND */
                     line_skip_eol(l);
                     x->position = 0;
                     return x;
                  }
                  /* NOT FOUND */
                  WARN_UNDEFINED_LINE;
                  return bwb_zline(l);            
               }
            }
            bwb_stripcr(tbuf);
            /* if( TRUE ) */
            {
               VariantType variant;
               
               variant.TypeChar = '$';
               variant.Buffer = tbuf;
               variant.Length = bwb_strlen( variant.Buffer );
               if( var_set( v, &variant ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline( l );
               }
            }
            return bwb_zline( l );
         }
         WARN_TYPE_MISMATCH;
         return bwb_zline( l );
      }
   }


   /* Process each variable read from the READ statement */

   main_loop = TRUE;
   while (main_loop == TRUE)
   {
      int             adv_loop;

      /* first check position in l->buffer and advance beyond
       * whitespace */

      adv_loop = TRUE;
      while (adv_loop == TRUE)
      {
         switch (l->buffer[l->position])
         {
         case ',':   /* variable seperator */
         case ' ':   /* whitespace */
            ++l->position;
            break;
         case BasicNulChar:
            adv_loop = FALSE; /* break out of advance
                      * loop */
            main_loop = FALSE;   /* break out of main
                      * loop */
            break;
         default: /* anything else */
            adv_loop = FALSE; /* break out of advance
                      * loop */
            break;
         }
      }


      /* be sure main_loop id still valid after checking the line */

      if (main_loop == TRUE)
      {
         /* Read a variable name */
         if( (v = line_read_scalar( l )) == NULL )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         /* Read a file value */

         if( file_read_value( f, delimit, v ) == FALSE )
         {
            if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) ) 
            {
               /* IF END # file_number THEN line_number */
               if( My->CurrentFile->EOF_LineNumber > 0 )
               {
                  LineType *x;
                  
                  x = find_line_number( My->CurrentFile->EOF_LineNumber, TRUE ); /* not found in the cache */
                  if (x != NULL)
                  {
                     /* FOUND */
                     line_skip_eol(l);
                     x->position = 0;
                     return x;
                  }
                  /* NOT FOUND */
                  WARN_UNDEFINED_LINE;
                  return bwb_zline(l);            
               }
            }
            WARN_INPUT_PAST_END;
            return bwb_zline(l);
         }
         /* OK */

      }     /* end of remainder of main loop */
   }        /* end of main_loop */

   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) && My->CurrentFile->mode & DEVMODE_RANDOM )
   {
      /* 
      CBASIC-II: RANDOM file reads always acccess a complete record
      */
      long ByteOffset;
      
      /* advance to the end-of-record */
      if( My->CurrentFile->width <= 0 )
      {
         WARN_FIELD_OVERFLOW;
         return bwb_zline(l);
      }
      ByteOffset =  ftell( My->CurrentFile->cfp );
      ByteOffset %= My->CurrentFile->width;
      if( ByteOffset != 0 )
      {
         long RecordNumber;
         RecordNumber =  ftell( My->CurrentFile->cfp );
         RecordNumber /= My->CurrentFile->width;
         RecordNumber ++;
         RecordNumber *= My->CurrentFile->width;
         fseek( My->CurrentFile->cfp, RecordNumber, SEEK_SET );
      }
   }

   return bwb_zline(l);

}


/***************************************************************
  
        FUNCTION:       inp_str()
  
        DESCRIPTION:    This function does INPUT processing
                        from a determined string of input
                        data and a determined variable list
                        (both in memory).  This presupposes
         that input has been taken from My->SYSIN,
         not from a disk file or device.
  
***************************************************************/

static int inp_str( /* LineType * l, */ char *input_buffer, char *var_list, int *vl_position, int IsFake)
{
   int             i;
   int             loop;
   char            ttbuf[BasicStringLengthMax + 1];   /* build element */
   int             ReadAllVars;
   int             ReadAllData;

   bwx_DEBUG(__FUNCTION__);

   ReadAllVars = FALSE;
   ReadAllData = FALSE;




   /* Read elements in input_buffer and assign them to variables in var_list */

   i = 0;
   loop = TRUE;
   while (loop == TRUE)
   {
      VariableType *v;
      register int    n;

      n = 0;
      ttbuf[0] = BasicNulChar;

      buff_skip_spaces( input_buffer, &i );
      buff_skip_spaces( var_list, vl_position );

      /* get a variable name from the list */
      if( (v = buff_read_scalar( var_list, vl_position )) == NULL )
      {
         WARN_SYNTAX_ERROR;
         return -1;   /* FATAL */
      }
      /* build string from input buffer in ttbuf */
      if( VAR_IS_STRING( v ) )
      {
         /* STRING */
         if( input_buffer[ i ] == BasicQuoteChar )
         {
            /* QUOTED STRING */
            int q = 0; /* number of quotes */
            i++;
            q++;
            while( input_buffer[ i ] )
            {
               if( input_buffer[ i ] == BasicQuoteChar )
               {
                  i++; /* quote */
                  q++;
                  if( input_buffer[ i ] == BasicQuoteChar )
                  {
                     /* embedded string "...""..." */
                     q++;
                  }
                  else
                  {
                     /* properly terminated string "...xx..." */
                     break;
                  }
               }
               ttbuf[ n ] = input_buffer[ i ];
               n++;
               i++;
            }
            ttbuf[ n ] = BasicNulChar;
            /* process QUOTED response */
            if (My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* INPUT allows empty string */ )
            {
               /* silently ignore */
            }
            else
            {
               /* an ODD number of quotes is an ERROR */
               if( q & 1 )
               {
                  fputs( "*** Type Mismatch ***\n", My->SYSOUT->cfp) ;
                  return FALSE;  /* RETRY */
               }
            }
         }
         else
         {
            /* UNQUOTED STRING */
            while( input_buffer[ i ] )
            {
               if( input_buffer[ i ] == ',' )
               {
                  break;
               }
               ttbuf[ n ] = input_buffer[ i ];
               n++;
               i++;
            }
            ttbuf[ n ] = BasicNulChar;
            /* RTRIM */
            while( n > 0 && ttbuf[ n - 1 ] == ' ' )
            {
               ttbuf[ n - 1 ] = BasicNulChar;
               n--;
            }
            /* process EMPTY response */
            if (My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* INPUT allows empty string */ )
            {
               /* silently ignore */
            }
            else
            {
               /* an EMPTY response is an ERROR */
               if (ttbuf[0] == BasicNulChar)
               {
                  fputs( "*** Type Mismatch ***\n", My->SYSOUT->cfp );
                  return FALSE;  /* RETRY */
               }
            }
            /* process UNQUOTED response */
            if (My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* INPUT allows unquoted strings */ )
            {
               /* silently ignore */
            }
            else
            {
               /* if was NOT quoted, then the only valid chars are ' ', '+', '-', '.', digit, letter */
               char           *P;
               P = ttbuf;
               while (*P != BasicNulChar)
               {
                  char            C;
      
                  C = *P;
                  P++;
                  /* switch */
                  if (C == ' ' || C == '+' || C == '-' || C == '.' || bwb_isdigit(C) || bwb_isalpha(C))
                  {
                     /* OK */
                  }
                  else
                  {
                     /* ERROR */
                     fputs( "*** Type Mismatch ***\n", My->SYSOUT->cfp );
                     return FALSE;  /* RETRY */
                  }
               }
            }
         }
      }
      else
      {
         /* NUMBER */
         while( input_buffer[ i ] )
         {
            if( input_buffer[ i ] == ',' )
            {
               break;
            }
            ttbuf[ n ] = input_buffer[ i ];
            n++;
            i++;
         }
         ttbuf[ n ] = BasicNulChar;
         /* RTRIM */
         while( n > 0 && ttbuf[ n - 1 ] == ' ' )
         {
            ttbuf[ n - 1 ] = BasicNulChar;
            n--;
         }
         /* process EMPTY response */
         if (My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* INPUT allows empty string */ )
         {
            /* silently ignore */
         }
         else
         {
            /* an EMPTY response is an ERROR */
            if (ttbuf[0] == BasicNulChar)
            {
               fputs( "*** Type Mismatch ***\n", My->SYSOUT->cfp );
               return FALSE;  /* RETRY */
            }
         }
      }


      /* perform type-specific assignment */
      {
         int ResultCode;
         
         ResultCode = inp_assign(ttbuf, v, IsFake, TRUE); /* inp_str, bwb_INPUT */
         switch (ResultCode)
         {
         case RESULT_OK:
            break;
         case RESULT_UNKNOWN:
            WARN_INTERNAL_ERROR;
            return -1;  /* FATAL */
            /* break; */
         case RESULT_TYPE_MMISMATCH:
            fputs("*** Type Mismatch ***\n", My->SYSOUT->cfp );
            return FALSE;  /* RETRY */
            /* break; */
         case RESULT_ARITHMETIC_OVERFLOW:
            fputs("*** Overflow ***\n", My->SYSOUT->cfp );
            return FALSE;  /* RETRY */
            /* break; */
         }
      }
      /* OK */


      /* check for commas in variable list and advance */
      buff_skip_spaces(var_list, vl_position);
      switch (var_list[*vl_position])
      {
      case BasicNulChar:
         loop = FALSE;
         ReadAllVars = TRUE;
         break;
      case ',':
         ++(*vl_position);
         break;
      }
      buff_skip_spaces(var_list, vl_position);

      /* check for commas in input list and advance */
      buff_skip_spaces(input_buffer, &i);
      switch (input_buffer[i])
      {
      case BasicNulChar:
         loop = FALSE;
         ReadAllData = TRUE;
         break;
      case ',':
         ++i;
         break;
      }
      buff_skip_spaces(input_buffer, &i);

   }

   /* return */
   if (ReadAllVars == TRUE && ReadAllData == TRUE)
   {
      return 1; /* SUCCESS */
   }

   /* READ/DATA mismatch */
   fputs("*** Count Mismatch ***\n", My->SYSOUT->cfp );
   return FALSE;  /* RETRY */
}

/***************************************************************
  
        FUNCTION:       inp_assign()
  
        DESCRIPTION:    This function assigns the value of a
                        numerical or string constant to a
                        variable.
  
  
***************************************************************/

static int inp_assign(char *b, VariableType * v, int IsFake, int IsInput)
{
   VariantType variant;

   bwx_DEBUG(__FUNCTION__);

   variant.TypeChar = v->VariableTypeChar;

   if( VAR_IS_STRING( v ) )
   {
      /* STRING */
      if (IsFake == TRUE)
      {
      }
      else
      {
         variant.Buffer = b;
         variant.Length = bwb_strlen( variant.Buffer );
         if( var_set( v, &variant ) == FALSE )
         {
            WARN_VARIABLE_NOT_DECLARED;
            return RESULT_UNKNOWN;
         }
      }
      
      if( My->CurrentVersion->OptionVersionBitmask & ( E78 ) && IsInput == TRUE )
      {
         if (bwb_strchr(b, BasicQuoteChar) != NULL)
         {
            /* ECMA-55 forbids embedded quotes on INPUT */
            return RESULT_TYPE_MMISMATCH;
         }
      }
      return RESULT_OK;
   }
   /* NUMBER */
   if (b[0] == BasicNulChar)
   {
      /* empty input value */
      if (IsFake == TRUE)
      {
      }
      else
      {
         variant.Number = 0.0;
         if( var_set( v, &variant ) == FALSE )
         {
            WARN_VARIABLE_NOT_DECLARED;
            return RESULT_UNKNOWN;
         }
      }
      if (My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* INPUT empty numeric is zero */ )
      {
         return RESULT_OK;
      }
      return RESULT_TYPE_MMISMATCH;
   }
   else
   {
      int ResultCode;
      if (IsFake == TRUE)
      {
         inp_numconst(b, &ResultCode);
      }
      else
      {
         variant.Number = inp_numconst(b, &ResultCode);
         if( var_set( v, &variant ) == FALSE )
         {
            WARN_VARIABLE_NOT_DECLARED;
            return RESULT_UNKNOWN;
         }
      }
      return ResultCode;
   }

   return RESULT_UNKNOWN;

}

/***************************************************************
  
        FUNCTION:       inp_adv()
  
        DESCRIPTION:    This function advances the string pointer
                        past whitespace and the item delimiter
                        (comma).
  
***************************************************************/

int
inp_adv(char *b, int *c)
{
   int             rval;

   bwx_DEBUG(__FUNCTION__);

   rval = FALSE;

   while (TRUE)
   {
      switch (b[*c])
      {
      case ' ':   /* whitespace */
      case ';':   /* semicolon, end of prompt string */
         ++*c;
         break;
      case ',':   /* comma, variable delimiter */
         rval = TRUE;
         ++*c;
         break;
      case BasicNulChar:  /* end of line */
         rval = TRUE;
         My->last_inp_adv_rval = rval;  /* JBV */
         return rval;
      default:
         My->last_inp_adv_rval = rval;  /* JBV */
         return rval;
      }
   }
   /* return 0; */ /* never reached */
}

/***************************************************************
  
        FUNCTION:       inp_const()
  
        DESCRIPTION:    This function reads a numerical or string
                        constant from <m_buffer> into <s_buffer>,
                        incrementing <position> appropriately.
  
***************************************************************/

static int inp_const(char *m_buffer, char *s_buffer, int *position)
{
   int             string;
   int             s_pos;
   int             loop;

   bwx_DEBUG(__FUNCTION__);

   /* leading whitespace is NOT part of the DATA item */
   buff_skip_spaces(m_buffer,position);


   /* first detect string constant */
   string = FALSE;
   if ( buff_skip_char( m_buffer,position, BasicQuoteChar) )
   {
      string = TRUE;
   }

   /* build the constant string */

   s_pos = 0;
   s_buffer[s_pos] = BasicNulChar;


   loop = TRUE;

   while (loop == TRUE)
   {
      switch (m_buffer[*position])
      {
      case BasicNulChar:  /* end of string */
         return TRUE;
         /* internal whitespace is part of the DATA item */
      case ',':   /* or end of argument */
         if (string == FALSE)
         {
            /* trailing whitespace is NOT part of the DATA item */
            while (s_pos > 0 && s_buffer[s_pos - 1] == ' ')
            {
               s_pos--;
               s_buffer[s_pos] = BasicNulChar;
            }
            return TRUE;
         }
         else
         {
            s_buffer[s_pos] = m_buffer[*position];
            ++(*position);
            ++s_pos;
            s_buffer[s_pos] = BasicNulChar;
         }
         break;
      case BasicQuoteChar:
         /* quote character */
         if (string == TRUE)
         {
            /* same as the starting quote character; examples are "..." and '...' */
            ++(*position); /* advance beyond quotation mark */
            if( My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* INPUT quotes */ )
            {
               if( m_buffer[*position] == BasicQuoteChar )
               {
                  /* embedded string "...""..." */
                  s_buffer[s_pos] = m_buffer[*position];
                  ++(*position);
                  ++s_pos;
                  s_buffer[s_pos] = BasicNulChar;
                  break;
               }
            }
            /* properly terminated "...xx..." */
            inp_adv(m_buffer, position);
            return TRUE;
            
         }
         else
         {
            WARN_TYPE_MISMATCH;
            return FALSE;
         }
      default:
         s_buffer[s_pos] = m_buffer[*position];
         ++(*position);
         ++s_pos;
         s_buffer[s_pos] = BasicNulChar;
         break;
      }

   }

   return FALSE;

}


/***************************************************************
  
        FUNCTION:       bwb_LINE()
  
        DESCRIPTION:    This function implements the BASIC LINE
                        INPUT statement.
  
   SYNTAX:         LINE INPUT [[#] device-number,]["prompt string";] string-variable$
  
***************************************************************/

LineType *
bwb_LINE(LineType * l)
{
   int             FileNumber;
   VariableType *v;
   FILE           *inp_device;
   char            tbuf[BasicStringLengthMax + 1];
   char            pstring[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   /* assign default values */

   My->CurrentFile = My->SYSIN;

   inp_device = My->SYSIN->cfp;

   pstring[0] = BasicNulChar;

   /* advance to first element (INPUT statement) */
   if( line_skip_word(l, "INPUT") == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   line_skip_spaces(l);

   /* check for semicolon in first position */

   if (line_skip_comma(l))
   {
      line_skip_spaces(l);
   }
   else
   if ( line_skip_char(l, BasicFileNumberPrefix) )
   {

      if( line_read_integer_expression(l, &FileNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( FileNumber < 0 )
      {
         /* "LINE INPUT # -1" is an error */
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( FileNumber > 0 )
      {
         /* normal file */
         My->CurrentFile = find_file_by_number( FileNumber );
         if( My->CurrentFile == NULL )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if ((My->CurrentFile->mode & DEVMODE_READ) == 0)
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if (My->CurrentFile->cfp == NULL)
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         inp_device = My->CurrentFile->cfp;
      }
   }
   /* check for comma */

   if ( line_skip_comma(l) )
   {
      line_skip_spaces(l);
   }
   /* check for quotation mark indicating prompt */

   if ( line_peek_char(l,BasicQuoteChar))
   {
      inp_const(l->buffer, pstring, &(l->position)); /* bwb_LINE prompt */
   }

   /* read the variable for assignment */
   if( (v = line_read_scalar( l )) == NULL )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if ( VAR_IS_STRING( v ) )
   {
      /* OK */
   }
   else
   {
      /* ERROR */
      WARN_TYPE_MISMATCH;
      return bwb_zline(l);
   }
   /* read a line of text into the bufffer */

   if (inp_device == My->SYSIN->cfp)
   {
      bwx_input(pstring, tbuf);
   }
   else
   {
      fgets(tbuf, BasicStringLengthMax, inp_device);
   }
   bwb_stripcr(tbuf);
   /* if( TRUE ) */
   {
      VariantType variant;
      
      variant.TypeChar = '$';
      variant.Buffer = tbuf;
      variant.Length = bwb_strlen( variant.Buffer );
      if( var_set( v, &variant ) == FALSE )
      {
         WARN_VARIABLE_NOT_DECLARED;
         return bwb_zline( l );
      }
   }
   return bwb_zline(l);
}


/***************************************************************
  
        FUNCTION: inp_numconst()
  
        DESCRIPTION: This function interprets a numerical
         constant.  Added by JBV 10/95
  
***************************************************************/

BasicNumberType
inp_numconst(char *expression, int *ResultCode)
{
   int             base;   /* numerical base for the constant */
   BasicNumberType mantissa;   
   int             exponent;  /* exponent for floating point number */
   int             man_start; /* starting point of mantissa */
   int             s_pos;  /* position in build string */
   int             build_loop;
   int             need_pm;
   unsigned int    u;
   int             IsRounded = FALSE;

   /* Expression stack stuff */
   /* char type; */
   BasicNumberType nval;
   char            string[BasicStringLengthMax + 1];
   int             pos_adv;

   /* initialize the variable if necessary */

   bwx_DEBUG(__FUNCTION__);

   mantissa = 0;


   need_pm = FALSE;
   nval = 0;

   /* check the first character(s) to determine numerical base and
    * starting point of the mantissa */

   switch (expression[0])
   {
   case '-':
   case '+':
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
   case '.':
      base = 10;  /* decimal constant */
      man_start = 0; /* starts at position 0 */
      need_pm = FALSE;
      break;
   case '&':      /* hex or octal constant */
      if (bwb_toupper(expression[1] == 'H') )
      {
         base = 16;  /* hexadecimal constant */
         man_start = 2; /* starts at position 2 */
      }
      else
      {
         base = 8;   /* octal constant */
         if (bwb_toupper(expression[1] == 'O') )
         {
            man_start = 2; /* starts at position 2 */
         }
         else
         {
            man_start = 1; /* starts at position 1 */
         }
      }
      break;
   default:
      *ResultCode = RESULT_TYPE_MMISMATCH;
      return 0;
   }

   /* now build the mantissa according to the numerical base */

   switch (base)
   {

   case 10:    /* decimal constant */

      /* initialize counters */

      pos_adv = man_start;
      /* type = NUMBER; */
      string[0] = BasicNulChar;
      s_pos = 0;
      exponent = 0;
      build_loop = TRUE;

      /* loop to build the string */

      while (build_loop == TRUE)
      {
         switch (expression[pos_adv])
         {
         case '-':   /* prefixed plus or minus */
         case '+':

            /* in the first position, a plus or minus
             * sign can be added to the beginning of the
             * string to be scanned */

            if (pos_adv == man_start)
            {
               string[s_pos] = expression[pos_adv];
               ++pos_adv;  /* advance to next
                      * character */
               ++s_pos;
               string[s_pos] = BasicNulChar;
            }
            /* but in any other position, the plus or
             * minus sign must be taken as an operator
             * and thus as terminating the string to be
             * scanned */

            else
            {
               build_loop = FALSE;
            }
            break;
         case '.':   /* note at least single precision */
         case '0':   /* or ordinary digit */
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            string[s_pos] = expression[pos_adv];
            ++pos_adv;  /* advance to next character */
            ++s_pos;
            string[s_pos] = BasicNulChar;
            break;

         case BasicCurrencySuffix:
         case BasicLongSuffix:   
         case BasicIntegerSuffix:
         case BasicByteSuffix:
            IsRounded = TRUE;
            /* fall thru */
         case BasicDoubleSuffix:
         case BasicSingleSuffix:
            if( My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* TypeChars in constants */ )
            {
               ++pos_adv;  /* advance to next character */
               /* type = NUMBER; */
               exponent = FALSE;
               build_loop = FALSE;
            }
            else
            {
               *ResultCode = RESULT_TYPE_MMISMATCH;
               return 0;
            }
            break;

         case 'D':   /* exponential, double precision */
         case 'd':
            if( My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* 'D' is exponential */ )
            {
               ++pos_adv;  /* advance to next character */
               /* type = NUMBER; */
               exponent = TRUE;
               build_loop = FALSE;
            }
            else
            {
               *ResultCode = RESULT_TYPE_MMISMATCH;
               return 0;
            }
            break;

         case 'E':   /* exponential, single precision */
         case 'e':
            ++pos_adv;  /* advance to next character */
            /* type = NUMBER; */
            exponent = TRUE;
            build_loop = FALSE;
            break;

         case BasicNulChar:
            build_loop = FALSE;
            break;

         default:
            /* not numeric */
            /* ERROR messages are displayed by the
             * calling routine */
            *ResultCode = RESULT_TYPE_MMISMATCH;
            return 0;
            /* break; */
         }

      }

      /* assign the value to the mantissa variable */
      sscanf(string, BasicNumberScanFormat, &mantissa);


      /* test if integer bounds have been exceeded */

      /* read the exponent if there is one */

      if (exponent == TRUE)
      {

         /* allow a plus or minus once at the beginning */

         need_pm = TRUE;

         /* initialize counters */

         string[0] = BasicNulChar;
         s_pos = 0;
         build_loop = TRUE;

         /* loop to build the string */

         while (build_loop == TRUE)
         {
            switch (expression[pos_adv])
            {
            case '-':   /* prefixed plus or minus */
            case '+':

               if (need_pm == TRUE) /* only allow once */
               {
                  string[s_pos] = expression[pos_adv];
                  ++pos_adv;  /* advance to next
                         * character */
                  ++s_pos;
                  string[s_pos] = BasicNulChar;
               }
               else
               {
                  build_loop = FALSE;
               }
               break;

            case '0':   /* or ordinary digit */
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':

               string[s_pos] = expression[pos_adv];
               ++pos_adv;  /* advance to next
                      * character */
               ++s_pos;
               string[s_pos] = BasicNulChar;
               need_pm = FALSE;
               break;

            case BasicCurrencySuffix:
            case BasicLongSuffix:   
            case BasicIntegerSuffix:
            case BasicByteSuffix:
               IsRounded = TRUE;
               /* fall-thru */
            case BasicDoubleSuffix:
            case BasicSingleSuffix:
               if( My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* TypeChars in constants */ )
               {
                  ++pos_adv;  /* advance to next character */
               }
               build_loop = FALSE;
               break;

            default: /* anything else, terminate */
               build_loop = FALSE;
               break;
            }

         }  /* end of build loop for exponent */

         /* assign the value to the user variable */

         sscanf(string, BasicNumberScanFormat, &nval);


      }     /* end of exponent search */
      if (nval == 0)
      {
         nval = mantissa;
      }
      else
      {
         nval = mantissa * pow(10.0, nval);
      }
      if( My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* TypeChars in constants */ )
      {
         /* 1.2% == 1 */
         if( IsRounded )
         {
            nval = bwb_rint( nval );  
         }
      }
      break;

   case 8:     /* octal constant */

      /* initialize counters */

      pos_adv = man_start;
      /* type = NUMBER; */
      string[0] = BasicNulChar;
      s_pos = 0;
      exponent = 0;
      build_loop = TRUE;

      /* loop to build the string */

      while (build_loop == TRUE)
      {
         switch (expression[pos_adv])
         {
         case '0':   /* or ordinary digit */
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
            string[s_pos] = expression[pos_adv];
            ++pos_adv;  /* advance to next character */
            ++s_pos;
            string[s_pos] = BasicNulChar;
            break;

         case BasicDoubleSuffix:
         case BasicSingleSuffix:
         case BasicCurrencySuffix:
         case BasicLongSuffix:   
         case BasicIntegerSuffix:
         case BasicByteSuffix:
            if( My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* TypeChars in constants */ )
            {
               ++pos_adv;  /* advance to next character */
            }
            build_loop = FALSE;
            break;

         default: /* anything else, terminate */
            build_loop = FALSE;
            break;
         }

      }

      /* now scan the string to determine the number */

      sscanf(string, "%o", &u);
      nval = u;

      break;

   case 16:    /* hexadecimal constant */

      /* initialize counters */

      pos_adv = man_start;
      /* type = NUMBER; */
      string[0] = BasicNulChar;
      s_pos = 0;
      exponent = 0;
      build_loop = TRUE;

      /* loop to build the string */

      while (build_loop == TRUE)
      {
         switch (expression[pos_adv])
         {
         case '0':   /* or ordinary digit */
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
         case 'A':
         case 'a':
         case 'B':
         case 'b':
         case 'C':
         case 'c':
         case 'D':
         case 'd':
         case 'E':
         case 'e':
         case 'F':   /* Don't forget these! (JBV) */
         case 'f':
            string[s_pos] = expression[pos_adv];
            ++pos_adv;  /* advance to next character */
            ++s_pos;
            string[s_pos] = BasicNulChar;
            break;

         case BasicDoubleSuffix:
         case BasicSingleSuffix:
         case BasicCurrencySuffix:
         case BasicLongSuffix:   
         case BasicIntegerSuffix:
         case BasicByteSuffix:
            if( My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* TypeChars in constants */ )
            {
               ++pos_adv;  /* advance to next character */
            }
            build_loop = FALSE;
            break;

         default: /* anything else, terminate */
            build_loop = FALSE;
            break;
         }

      }

      /* now scan the string to determine the number */

      sscanf(string, "%x", &u);
      nval = u;
      break;
   }

   /* check Value */
   if (isnan(nval))
   {
      nval = 0;
      /* ERROR messages are displayed by the calling routine */
      *ResultCode = RESULT_TYPE_MMISMATCH;
   }
   else
   if (isinf(nval))
   {
      if (nval < 0)
      {
         nval = -(DBL_MAX);   /* FLT_MAX */
      }
      else
      {
         nval = (DBL_MAX); /* FLT_MAX */
      }
      /* ERROR messages are displayed by the calling routine */
      *ResultCode = RESULT_ARITHMETIC_OVERFLOW;
   }
   else
   {
      *ResultCode = RESULT_OK;
   }

   return nval;

}

static int read_data(VariableType *v)
{
   int             ResultCode;
   char            tbuf[BasicStringLengthMax + 1];

     /* advance beyond whitespace or comma in data buffer */
     inp_adv( My->data_line->buffer, &My->data_pos );

     /* Advance to next line if end of buffer */
     if ( buff_is_eol( My->data_line->buffer, &My->data_pos ) )
     {
        /* end of buffer */
        My->data_line =  My->data_line->next;
        My->data_pos =  My->data_line->Startpos;
     }
     while ( My->data_line->cmdnum != C_DATA)
     {
        if ( My->data_line == &My->bwb_end )
        {
           /* halt */
           WARN_OUT_OF_DATA;
           return FALSE;
        }
        My->data_line =  My->data_line->next;
        My->data_pos =  My->data_line->Startpos;
     }

     /* leading whitespace is NOT part of the DATA item */
     buff_skip_spaces(  My->data_line->buffer, &My->data_pos );

     if ( buff_is_eol( My->data_line->buffer, &My->data_pos ) )
     {
        /* end of buffer */
        WARN_OUT_OF_DATA;
        return FALSE;
     }


     /* now at last we have a variable in v that needs to
      * be assigned data from the data_buffer at position
      * My->data_pos. What remains to be done is to
      * get one single bit of data, a string constant or
      * numerical constant, into the small buffer */

     if( buff_peek_char(  My->data_line->buffer, &My->data_pos, BasicQuoteChar) )
     {
        if ( VAR_IS_STRING( v ) )
        {
           /* OK */
        }
        else
        {
           /* ERROR */
           WARN_TYPE_MISMATCH;
           return FALSE;
        }
     }
     inp_const(My->data_line->buffer, tbuf, &My->data_pos); /* read_data , bwb_READ , bwb_MAT_READ */
     if (My->CurrentVersion->OptionFlags & OPTION_COVERAGE_ON)
     {
        /* this line has been READ */
        My->data_line->LineFlags |= LINE_EXECUTED;
     }

     /* finally assign the data to the variable */
     ResultCode = inp_assign(tbuf, v, FALSE, FALSE);  /* read_data , bwb_READ , bwb_MAT_READ */
     switch (ResultCode)
     {
     case RESULT_OK:
        break;
     case RESULT_UNKNOWN:
        WARN_INTERNAL_ERROR;
        return FALSE;
        /* break; */
     case RESULT_TYPE_MMISMATCH:
        WARN_TYPE_MISMATCH;
        return FALSE;
        /* break; */
     case RESULT_ARITHMETIC_OVERFLOW:
        bwb_Warning_Overflow("*** Arithmetic Overflow ***");
        break;
     }
     /* OK */
     return TRUE;
}

static LineType * file_read_matrix( LineType * l )
{
   /* MAT READ arrayname [;|,] */
   /* Array must be 1, 2 or 3 dimensions */
   /* Array may be either NUMBER or STRING */

   VariableType *v;

   bwx_DEBUG(__FUNCTION__);

   My->LastInputCount = 0;   

   line_skip_spaces( l );
   
   while( bwb_isalpha( l->buffer[l->position] ) )
   {
       My->LastInputCount = 0;   
       if( (v = line_read_matrix( l )) == NULL)
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

       /* both arrays are of the same size */
       /* allow user to use either item seperator */
       if( line_skip_comma(l) )
       {
           /* force printing col-by-col */
       }
       else
       {
           /* force concatenating the columns */
       }

       /* READ array */
       switch( v->dimensions )
       {
       case 1:
           {
               /*
               OPTION BASE 0
               DIM A(5)
               ...
               MAT READ A 
               ...
               FOR I = 0 TO 5
                 READ A(I)
               NEXT I
               ...
               */
               for( v->array_pos[0] = v->LBOUND[0]; v->array_pos[0] <= v->UBOUND[0]; v->array_pos[0]++ )
               {
                  if ( My->CurrentFile == My->SYSIN)
                  {
                      if( read_data(v) == FALSE )
                      {
                         WARN_SUBSCRIPT_OUT_OF_RANGE;
                         return bwb_zline(l);
                      }
                  }
                  else
                  {
                     if( file_read_value( My->CurrentFile->cfp, My->CurrentFile->delimit, v  ) == FALSE )
                     {
                        WARN_INPUT_PAST_END;
                        return bwb_zline(l);
                     }
                  }
                  /* OK */
                  My->LastInputCount++;   
               }
           }
           break;
       case 2:
           {
               /*
               OPTION BASE 0
               DIM B(2,3)
               ...
               MAT READ B 
               ...
               FOR I = 0 TO 2
                   FOR J = 0 TO 3
                       READ B(I,J)
                   NEXT J
                   PRINT
               NEXT I
               ...
               */
               for( v->array_pos[0] = v->LBOUND[0]; v->array_pos[0] <= v->UBOUND[0]; v->array_pos[0]++ )
               {
               for( v->array_pos[1] = v->LBOUND[1]; v->array_pos[1] <= v->UBOUND[1]; v->array_pos[1]++ )
               {
                  if ( My->CurrentFile == My->SYSIN)
                  {
                      if( read_data(v) == FALSE )
                      {
                         WARN_SUBSCRIPT_OUT_OF_RANGE;
                         return bwb_zline(l);
                      }
                  }
                  else
                  {
                     if( file_read_value( My->CurrentFile->cfp, My->CurrentFile->delimit, v  ) == FALSE )
                     {
                        WARN_INPUT_PAST_END;
                        return bwb_zline(l);
                     }
                  }
                  /* OK */
                  My->LastInputCount++;   
               }
               }
           }
           break;
       case 3:
           {
               /*
               OPTION BASE 0
               DIM C(2,3,4)
               ...
               MAT READ C 
               ...
               FOR I = 0 TO 2
                   FOR J = 0 TO 3
                       FOR K = 0 TO 4
                           READ C(I,J,K)
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
                  if ( My->CurrentFile == My->SYSIN)
                  {
                      if( read_data(v) == FALSE )
                      {
                         WARN_SUBSCRIPT_OUT_OF_RANGE;
                         return bwb_zline(l);
                      }
                  }
                  else
                  {
                     if( file_read_value( My->CurrentFile->cfp, My->CurrentFile->delimit, v  ) == FALSE )
                     {
                        WARN_INPUT_PAST_END;
                        return bwb_zline(l);
                     }
                  }
                  /* OK */
                  My->LastInputCount++;   
               }
               }
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

LineType *
bwb_MAT_GET(LineType * l)
{
   /* MAT GET filename$ , matrix [, ...] */
   VariantType E;
   VariantType *e = &E;  

   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSIN;

   line_skip_spaces(l);
   if( line_read_expression( l, e ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( e->TypeChar == BasicStringSuffix )
   {
      /* STRING */
      /* MAT GET filename$ ... */
      if( is_empty_filename( e->Buffer ) )
      {
         /* "MAT GET # 0" is an error */
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      My->CurrentFile = find_file_by_name( e->Buffer );
      if( My->CurrentFile == NULL )
      {
         /* implicitly OPEN for reading */
         My->CurrentFile = file_new();
         My->CurrentFile->cfp = fopen(e->Buffer, "r");
         if( My->CurrentFile->cfp == NULL )
         {
            /* bad file name */
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         My->CurrentFile->FileNumber = file_next_number();
         My->CurrentFile->mode = DEVMODE_INPUT;
         My->CurrentFile->width = 0;
         /* WIDTH == RECLEN */
         My->CurrentFile->col = 1;
         My->CurrentFile->row = 1;
         My->CurrentFile->delimit = ',';
         My->CurrentFile->buffer = NULL;
         bwb_strcpy(My->CurrentFile->filename, e->Buffer);
      }
   }
   else
   {
      /* NUMBER -- file must already be OPEN */
      /* GET filenumber ... */
      if( e->Number < 0 )
      {
         /* "MAT GET # -1" is an error */
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( e->Number == 0 )
      {
         /* "MAT GET # 0" is an error */
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      /* normal file */
      My->CurrentFile = find_file_by_number( (int) bwb_rint( e->Number ) );
      if( My->CurrentFile == NULL )
      {
         /* file not OPEN */
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
   }  
   RELEASE( e );  
   if( My->CurrentFile == NULL )
   {
      WARN_BAD_FILE_NUMBER;
      return bwb_zline(l);
   }
   if (( My->CurrentFile->mode & DEVMODE_READ) == 0)
   {
      WARN_BAD_FILE_NUMBER;
      return bwb_zline(l);
   }
   if ( line_skip_comma(l) )
   {
      /* OK */
   }
   else
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   return file_read_matrix( l );
}


LineType *
bwb_MAT_READ(LineType * l)
{
   /* MAT READ arrayname [;|,] */
   /* Array must be 1, 2 or 3 dimensions */
   /* Array may be either NUMBER or STRING */


   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSIN;

   My->LastInputCount = 0;   

   if ( line_skip_char(l,BasicFileNumberPrefix) )
   {
      /* MAT READ # filenum, varlist */
      int FileNumber;

      if( line_read_integer_expression(l, &FileNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      
      if ( line_skip_comma(l) )
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if ( My->CurrentFile != My->SYSIN)
      {
         if ((My->CurrentFile->mode & DEVMODE_READ) == 0)
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if (My->CurrentFile->cfp == NULL)
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
      }
      /* "MAT READ # 0, varlist" is the same as "MAT READ varlist" */
      line_skip_spaces(l);
   }
   return file_read_matrix( l );
}


static int input_data(VariableType *v, char *Buffer)
{
    char *C;
    char c;
    int ResultCode;
    
    if( Buffer[0] == BasicNulChar )
    {
        /* Get more data */
        bwx_input("?", Buffer );
        if( Buffer[0] == BasicNulChar )
        {
            return FALSE;
        }
    }
    /* process data */

    /* data seperator is an unquoted comma (,) */

    C = Buffer;
    while( *C != BasicNulChar && *C != ',' )
    {
        if( *C == BasicQuoteChar )
        {
            /* skip leading quote */
            C++;
            while( *C != BasicNulChar && *C != BasicQuoteChar )
            {
                /* skip string constant */
                C++;
            }
            if( *C == BasicQuoteChar )
            {
                /* skip trailing quote */
                C++;
            }
        }
        else
        {
            C++;
        }
    }
    c = *C; /* either a comma (,) or a NUL (0) */
    *C = BasicNulChar;
    CleanLine( Buffer );
    if( Buffer[0] == BasicQuoteChar )
    {
        /* remove quotes */
        char *E;
        
        E = Buffer;
        E++;
        E = bwb_strchr(E, BasicQuoteChar);
        if( E != NULL )
        {
            *E = BasicNulChar;
        }
        E = Buffer;
        E++;
        bwb_strcpy(Buffer,E);
    }
   ResultCode = inp_assign(Buffer, v, FALSE, FALSE);  /* input_data , bwb_MAT_INPUT */
   switch (ResultCode)
   {
   case RESULT_OK:
      break;
   case RESULT_UNKNOWN:
      WARN_INTERNAL_ERROR;
      return FALSE;
      /* break; */
   case RESULT_TYPE_MMISMATCH:
      fputs("*** Type Mismatch ***\n", My->SYSOUT->cfp );
      return FALSE;
      /* break; */
   case RESULT_ARITHMETIC_OVERFLOW:
      fputs("*** Overflow ***\n", My->SYSOUT->cfp );
      return FALSE;
      /* break; */
   }
   /* OK */
   if( c == BasicNulChar )
   {
       /* we have consumed the entire buffer */
       Buffer[0] = BasicNulChar;
   }
   else
   if( c == ',' )
   {
       /* shift the buffer left, just past the comma (,) */
       C++;
       bwb_strcpy(Buffer,C);
   }
   else
   {
      WARN_INTERNAL_ERROR;
      return FALSE;
   }
   
   return TRUE;
}

LineType *
bwb_MAT_INPUT(LineType * l)
{
   /* MAT INPUT arrayname [;|,] */
   /* Array must be 1, 2 or 3 dimensions */
   /* Array may be either NUMBER or STRING */
   
   VariableType *v;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);
   
   My->CurrentFile = My->SYSIN;

   My->LastInputCount = 0;   

   if ( line_skip_char(l,BasicFileNumberPrefix) )
   {
      /* MAT INPUT # filenum, varlist */
      int FileNumber;

      if( line_read_integer_expression(l, &FileNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      
      if ( line_skip_comma(l) )
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if ( My->CurrentFile != My->SYSIN)
      {
         if ((My->CurrentFile->mode & DEVMODE_READ) == 0)
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if (My->CurrentFile->cfp == NULL)
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
      }
      /* "MAT INPUT # 0, varlist" is the same as "MAT INPUT varlist" */
      line_skip_spaces(l);
   }


   while( bwb_isalpha( l->buffer[l->position] ) )
   {
       My->LastInputCount = 0;   
       if( (v = line_read_matrix( l )) == NULL)
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

       /* allow user to use either item seperator */
       if( line_skip_comma(l) )
       {
           /* force printing col-by-col */
       }
       else
       {
           /* force concatenating the columns */
       }

       /* INPUT array */
       tbuf[0] = BasicNulChar;
       switch( v->dimensions )
       {
       case 1:
           {
               /*
               OPTION BASE 0
               DIM A(5)
               ...
               MAT INPUT A 
               ...
               FOR I = 0 TO 5
                 INPUT A(I)
               NEXT I
               ...
               */
               My->LastInputCount = 0;   
               for( v->array_pos[0] = v->LBOUND[0]; v->array_pos[0] <= v->UBOUND[0]; v->array_pos[0]++ )
               {
                  if ( My->CurrentFile == My->SYSIN)
                  {
                      if( input_data(v,tbuf) == FALSE )
                      {
                         /*
                         WARN_INPUT_PAST_END;
                         */
                         return bwb_zline(l);
                      }
                  }
                  else
                  {
                     if( file_read_value( My->CurrentFile->cfp, ',', v  ) == FALSE )
                     {
                        WARN_INPUT_PAST_END;
                        return bwb_zline(l);
                     }
                  }
                  /* OK */
                  My->LastInputCount++;   
               }
           }
           break;
       case 2:
           {
               /*
               OPTION BASE 0
               DIM B(2,3)
               ...
               MAT INPUT B 
               ...
               FOR I = 0 TO 2
                   FOR J = 0 TO 3
                       INPUT B(I,J)
                   NEXT J
                   PRINT
               NEXT I
               ...
               */
               My->LastInputCount = 0;   
               for( v->array_pos[0] = v->LBOUND[0]; v->array_pos[0] <= v->UBOUND[0]; v->array_pos[0]++ )
               {
               for( v->array_pos[1] = v->LBOUND[1]; v->array_pos[1] <= v->UBOUND[1]; v->array_pos[1]++ )
               {
                  if ( My->CurrentFile == My->SYSIN)
                  {
                      if( input_data(v,tbuf) == FALSE )
                      {
                         /*
                         WARN_INPUT_PAST_END;
                         */
                         return bwb_zline(l);
                      }
                  }
                  else
                  {
                     if( file_read_value( My->CurrentFile->cfp, ',', v  ) == FALSE )
                     {
                        WARN_INPUT_PAST_END;
                        return bwb_zline(l);
                     }
                  }
                  /* OK */
                  My->LastInputCount++;   
               }
               }
           }
           break;
       case 3:
           {
               /*
               OPTION BASE 0
               DIM C(2,3,4)
               ...
               MAT INPUT C 
               ...
               FOR I = 0 TO 2
                   FOR J = 0 TO 3
                       FOR K = 0 TO 4
                           INPUT C(I,J,K)
                       NEXT K
                       PRINT
                   NEXT J
                   PRINT
               NEXT I
               ...
               */
               My->LastInputCount = 0;   
               for( v->array_pos[0] = v->LBOUND[0]; v->array_pos[0] <= v->UBOUND[0]; v->array_pos[0]++ )
               {
               for( v->array_pos[1] = v->LBOUND[1]; v->array_pos[1] <= v->UBOUND[1]; v->array_pos[1]++ )
               {
               for( v->array_pos[2] = v->LBOUND[2]; v->array_pos[2] <= v->UBOUND[2]; v->array_pos[2]++ )
               {
                  if ( My->CurrentFile == My->SYSIN)
                  {
                      if( input_data(v,tbuf) == FALSE )
                      {
                         /*
                         WARN_INPUT_PAST_END;
                         */
                         return bwb_zline(l);
                      }
                  }
                  else
                  {
                     if( file_read_value( My->CurrentFile->cfp, ',', v  ) == FALSE )
                     {
                        WARN_INPUT_PAST_END;
                        return bwb_zline(l);
                     }
                  }
                  /* OK */
                  My->LastInputCount++;   
               }
               }
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

/* EOF */
