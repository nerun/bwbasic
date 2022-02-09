/***************************************************************
  
        bwb_dio.c       Device Input/Output Routines
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




/***************************************************************
  
        FUNCTION:       bwb_open()
  
   DESCRIPTION:    This function implements the BASIC OPEN
         command to open a stream for device input/output.
  
        SYNTAX: 1. OPEN "I"|"O"|"R"|"A", [#]n, filename [,rlen]
                2. OPEN filename [FOR INPUT|OUTPUT|APPEND|RANDOM|BINARY] AS [#]n [LEN=n]
  
***************************************************************/

extern char bwb_file_open( char A, int x, char * B, int y )
{
   /* OPEN "I"|"O"|"R"|"A", [#]n, filename [,rlen] */
   /* P1STR|P2NUM|P3STR|P4NUM */
   /* P1BYT|P2DEV|P3BYT|P4INT */

   int             mode;
   FileType * F;
   char IsError = BasicNulChar;
   
   mode = bwb_toupper( A );
   switch (mode)
   {
   case 'I':
   case 'O':
   case 'A':
   case 'B':
   case 'R':
      break;
   default:
      mode = DEVMODE_CLOSED;
      break;
   }
   F = find_file_by_number( x );
   if( F == NULL )
   {
      F = file_new();
   }
   /**/
   if( F == NULL )
   {
      IsError = 'M';
   }
   else
   if (mode == DEVMODE_CLOSED)
   {
      IsError = 'A';
   }
   else
   if( F == My->SYSIN )
   {
      IsError = 'X';
   }
   else
   if( F == My->SYSOUT )
   {
      IsError = 'X';
   }
   else
   if( F == My->SYSPRN )
   {
      IsError = 'X';
   }
   else
   if ( F->mode != DEVMODE_CLOSED)
   {
      IsError = 'X';
   }
   else
   if (y < 0)
   {
      IsError = 'Y';
   }
   else
   if (y == 0 && mode == 'R')
   {
      IsError = 'Y';
   }
   else
   {
      FILE           *fp = NULL;
      char           *buffer = NULL;
      switch (mode)
      {
      case 'I':
         mode = DEVMODE_INPUT;
         fp = fopen(B, "r");
         y = 0;
         break;
      case 'O':
         mode = DEVMODE_OUTPUT;
         fp = fopen(B, "w");
         y = 0;
         break;
      case 'A':
         mode = DEVMODE_APPEND;
         fp = fopen(B, "a");
         y = 0;
         break;
      case 'B':
         mode = DEVMODE_BINARY;
         fp = fopen(B, "r+");
         if (fp == NULL)
         {
            fp = fopen(B, "w");
            if( fp != NULL )
            {
               fclose(fp); /* fp != NULL */
               fp = fopen(B, "r+");
            }
         }
         y = 0;
         break;
      case 'R':
         mode = DEVMODE_RANDOM;
         fp = fopen(B, "r+");
         if (fp == NULL)
         {
            fp = fopen(B, "w");
            if( fp != NULL )
            {
               fclose(fp); /* fp != NULL */
               fp = fopen(B, "r+");
            }
         }
         if (fp != NULL)
         {
            buffer = CALLOC(y, 1, "F_OPEN_A_X_B_Y_N");
         }
         break;
      }
      if (fp == NULL)
      {
         /* bad file name */
         IsError = 'B';
      }
      else
      if (mode == DEVMODE_RANDOM && buffer == NULL)
      {
         /* bad length */
         IsError = 'Y';
      }
      else
      {
         F->FileNumber = x;
         F->mode = mode;
         F->cfp = fp;
         F->width = y;
         /* WIDTH == RDCLEN */ 
         F->col = 1;
         F->row = 1;
         F->delimit = ',';
         F->buffer = buffer;
         bwb_strcpy(F->filename, B);
         if (mode == DEVMODE_APPEND)
         {
            fseek(fp, 0, SEEK_END);
         }
         else
         if (mode == DEVMODE_RANDOM)
         {
            bwb_memset(buffer, ' ', y); /* flush */
         }
      }
   }
   /* OK */
   return IsError;
}


LineType *
bwb_OPEN(LineType * l)
{
   int FileNumber = 0;
   char FileMode = 'R';
   int RecordLength = 0;
   VariantType t;
   VariantType *T = &t;
   char IsError;

   CLEAR_VARIANT( T );
   
   bwx_DEBUG(__FUNCTION__);

   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      /* 
      SYNTAX: OPEN filename$ [ RECL reclen ] AS filenumber [ BUFF ignored ] [ RECS ignored ] 
      */
      /* 
      --------------------------------------------- 
      FILE NAME
      --------------------------------------------- 
      */
      if( line_read_expression( l, T ) == FALSE )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      if( T->TypeChar != BasicStringSuffix )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      if( T->Length == 0 )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      /* 
      --------------------------------------------- 
      RECORD LENGTH
      --------------------------------------------- 
      */
      if( line_skip_word( l, "RECL" ) )
      {
         if( line_read_integer_expression( l , &RecordLength ) == FALSE )
         {
            WARN_FIELD_OVERFLOW;
            return bwb_zline( l );
         }
         if( RecordLength <= 0 )
         {
            WARN_FIELD_OVERFLOW;
            return bwb_zline( l );
         }
      }
      /* 
      --------------------------------------------- 
      FILE NUMBER
      --------------------------------------------- 
      */
      if( line_skip_word( l, "AS"   ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline( l );
      }
      if( line_skip_char( l, BasicFileNumberPrefix ) == FALSE )
      {
         /* OPTIONAL */
      }
      if( line_read_integer_expression( l , &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      if( FileNumber <= 0 )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      /* 
      --------------------------------------------- 
      IGNORED
      --------------------------------------------- 
      */
      /* if( TRUE ) */
      {
         int Ignored = 0;
         /*
         these are all parsed but ignored
         */

         if( line_skip_word( l, "BUFF" ) )
         {
            if( line_read_integer_expression( l, &Ignored ) == FALSE )
            {
               WARN_ILLEGAL_FUNCTION_CALL;
               return bwb_zline(l);
            }
            if( Ignored <= 0 )
            {
               WARN_ILLEGAL_FUNCTION_CALL;
               return bwb_zline(l);
            }
         }
         if( line_skip_word( l, "RECS" ) )
         {
            if( line_read_integer_expression( l, &Ignored ) == FALSE )
            {
               WARN_ILLEGAL_FUNCTION_CALL;
               return bwb_zline(l);
            }
            if( Ignored <= 0 )
            {
               WARN_ILLEGAL_FUNCTION_CALL;
               return bwb_zline(l);
            }
         }
      }
      /* 
      --------------------------------------------- 
      FILE MODE 
      --------------------------------------------- 
      */
      FileMode = 'I';
      /* 
      --------------------------------------------- 
      DO IT
      --------------------------------------------- 
      */
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         My->CurrentFile = file_new();
         My->CurrentFile->FileNumber = FileNumber;
      }
      bwb_strcpy( My->CurrentFile->filename, T->Buffer );
      strupper( My->CurrentFile->filename ); /* CBASIC-II: file names are always upper case */      
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
      /* open EXISTING text file for update (reading and writing) */
      
      if( (My->CurrentFile->cfp = fopen( My->CurrentFile->filename, "r+" )) == NULL )
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
               RELEASE( T );
               return x;
            }
            /* NOT FOUND */
            WARN_UNDEFINED_LINE;
            return bwb_zline(l);            
         }
         WARN_BAD_FILE_NAME;
         return bwb_zline(l);
      }
      if( RecordLength > 0 )
      {
         My->CurrentFile->width = RecordLength;
         My->CurrentFile->mode = DEVMODE_RANDOM;
      }
      else
      {
         My->CurrentFile->mode = DEVMODE_INPUT | DEVMODE_OUTPUT;
      }
      /* OK */
   }
   else
   if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
   {
      /* 
      SYNTAX: OPEN filenumber, filename$, INPUT | OUTPUT
      */
      /* 
      --------------------------------------------- 
      FILE NUMBER
      --------------------------------------------- 
      */
      if( line_skip_char( l, BasicFileNumberPrefix ) == FALSE )
      {
         /* OPTIONAL */
      }
      if( line_read_integer_expression( l , &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      if( FileNumber <= 0 )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      if( line_skip_comma( l ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      /* 
      --------------------------------------------- 
      FILE NAME
      --------------------------------------------- 
      */
      if( line_read_expression( l, T ) == FALSE )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      if( T->TypeChar != BasicStringSuffix )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      if( T->Length == 0 )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      if( line_skip_comma( l ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      /* 
      --------------------------------------------- 
      FILE MODE 
      --------------------------------------------- 
      */
      /* if( TRUE ) */
      {
         if( line_skip_word( l, "INPUT" ) )
         {
            FileMode = 'I';
         }
         else
         if( line_skip_word( l, "OUTPUT" ) )
         {
            FileMode = 'O';
         }
         else
         if( line_skip_word( l, "APPEND" ) )
         {
            FileMode = 'A';
         }
         else
         {
            WARN_BAD_FILE_MODE;
            return bwb_zline( l );
         }
      }
      /* 
      --------------------------------------------- 
      RECORD LENGTH
      --------------------------------------------- 
      */
      if( FileMode == 'R' )
      {
         RecordLength = 128;
      }
      else
      {
         RecordLength = 0;
      }
      /* 
      --------------------------------------------- 
      DO IT
      --------------------------------------------- 
      */
      IsError = bwb_file_open( FileMode, FileNumber, T->Buffer, RecordLength );
      switch( IsError )
      {
      case BasicNulChar:
         /* OK */
         break;
      case 'A':
         WARN_BAD_FILE_MODE;
         return bwb_zline( l );
         break;
      case 'X':
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
         break;
      case 'B':
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
         break;
      case 'Y':
         WARN_FIELD_OVERFLOW;
         return bwb_zline( l );
         break;
      case 'M':
         WARN_OUT_OF_MEMORY;
         return bwb_zline( l );
         break;
      default:
         WARN_INTERNAL_ERROR;
         return bwb_zline( l );
         break;
      }
      /* OK */
   }
   else
   if( My->CurrentVersion->OptionVersionBitmask & ( D71 ) )
   {
      /* 
      SYNTAX: OPEN filename$ [FOR mode] AS filenumber [ ,RECORDSIZE ignored ] [ ,CLUSTERSIZE ignored ] [ ,MODE ignored ]
      */
      /* 
      --------------------------------------------- 
      FILE NAME
      --------------------------------------------- 
      */
      if( line_read_expression( l, T ) == FALSE )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      if( T->TypeChar != BasicStringSuffix )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      if( T->Length == 0 )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      /* 
      --------------------------------------------- 
      FILE MODE
      --------------------------------------------- 
      */
      if( line_skip_word( l, "FOR" ) )
      {
         if( line_skip_word( l, "INPUT" ) )
         {
            FileMode = 'I';
         }
         else
         if( line_skip_word( l, "OUTPUT" ) )
         {
            FileMode = 'O';
         }
         else
         if( line_skip_word( l, "APPEND" ) )
         {
            FileMode = 'A';
         }
         else
         if( line_skip_word( l, "RANDOM" ) )
         {
            FileMode = 'R';
         }
         else
         if( line_skip_word( l, "BINARY" ) )
         {
            FileMode = 'B';
         }
         else
         {
            WARN_BAD_FILE_MODE;
            return bwb_zline( l );
         }
      }
      /* 
      --------------------------------------------- 
      FILE NUMBER
      --------------------------------------------- 
      */
      if( line_skip_word( l, "AS"   ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline( l );
      }
      if( line_skip_word( l, "FILE" ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline( l );
      }
      if( line_skip_char( l, BasicFileNumberPrefix ) == FALSE )
      {
         /* OPTIONAL */
      }
      if( line_read_integer_expression( l , &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      if( FileNumber <= 0 )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      /* 
      --------------------------------------------- 
      RECORD LENGTH
      --------------------------------------------- 
      */
      if( FileMode == 'R' )
      {
         RecordLength = 512;
      }
      else
      {
         RecordLength = 0;
      }
      while( line_is_eol( l ) == FALSE )
      {
         int Ignored = 0;
         /*
         these are all parsed but ignored
         */
         if( line_skip_comma( l ) )
         {
            /* OK */
         }
         else
         if( line_skip_word( l , "RECORDSIZE" ) )
         {
            if( line_read_integer_expression( l , &Ignored ) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline( l );
            }
         }
         else
         if( line_skip_word( l , "CLUSTERSIZE" ) )
         {
            if( line_read_integer_expression( l , &Ignored ) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline( l );
            }
         }
         else
         if( line_skip_word( l , "FILESIZE" ) )
         {
            if( line_read_integer_expression( l , &Ignored ) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline( l );
            }
         }
         else
         if( line_skip_word( l , "MODE" ) )
         {
            if( line_read_integer_expression( l , &Ignored ) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline( l );
            }
         }
         else
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline( l );
         }
      }
      /* 
      --------------------------------------------- 
      DO IT
      --------------------------------------------- 
      */
      IsError = bwb_file_open( FileMode, FileNumber, T->Buffer, RecordLength );
      switch( IsError )
      {
      case BasicNulChar:
         /* OK */
         break;
      case 'A':
         WARN_BAD_FILE_MODE;
         return bwb_zline( l );
         break;
      case 'X':
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
         break;
      case 'B':
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
         break;
      case 'Y':
         WARN_FIELD_OVERFLOW;
         return bwb_zline( l );
         break;
      case 'M':
         WARN_OUT_OF_MEMORY;
         return bwb_zline( l );
         break;
      default:
         WARN_INTERNAL_ERROR;
         return bwb_zline( l );
         break;
      }
      /* OK */
   }
   else
   {
      /* OPEN filename$ [FOR mode] AS filenumber [LEN reclen] */
      /* 
      --------------------------------------------- 
      FILE NAME
      --------------------------------------------- 
      */
      if( line_read_expression( l, T ) == FALSE )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      if( T->TypeChar != BasicStringSuffix )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      if( T->Length == 0 )
      {
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
      }
      /* 
      --------------------------------------------- 
      FILE MODE
      --------------------------------------------- 
      */
      if( line_skip_word( l, "FOR" ) )
      {
         if( line_skip_word( l, "INPUT" ) )
         {
            FileMode = 'I';
         }
         else
         if( line_skip_word( l, "OUTPUT" ) )
         {
            FileMode = 'O';
         }
         else
         if( line_skip_word( l, "APPEND" ) )
         {
            FileMode = 'A';
         }
         else
         if( line_skip_word( l, "RANDOM" ) )
         {
            FileMode = 'R';
         }
         else
         if( line_skip_word( l, "BINARY" ) )
         {
            FileMode = 'B';
         }
         else
         {
            WARN_BAD_FILE_MODE;
            return bwb_zline( l );
         }
      }
      /* 
      --------------------------------------------- 
      FILE NUMBER
      --------------------------------------------- 
      */
      if( line_skip_word( l, "AS"   ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline( l );
      }
      if( line_skip_char( l, BasicFileNumberPrefix ) == FALSE )
      {
         /* OPTIONAL */
      }
      if( line_read_integer_expression( l , &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      if( FileNumber <= 0 )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
      }
      /* 
      --------------------------------------------- 
      RECORD LENGTH
      --------------------------------------------- 
      */
      if( FileMode == 'R' )
      {
         RecordLength = 128;
      }
      else
      {
         RecordLength = 0;
      }
      if( line_skip_word( l, "LEN" ) )
      {
         if( line_read_integer_expression( l , &RecordLength ) == FALSE )
         {
           WARN_FIELD_OVERFLOW;
            return bwb_zline( l );
         }
         if( RecordLength <= 0 )
         {
            WARN_FIELD_OVERFLOW;
            return bwb_zline( l );
         }
      }
      /* 
      --------------------------------------------- 
      DO IT
      --------------------------------------------- 
      */
      IsError = bwb_file_open( FileMode, FileNumber, T->Buffer, RecordLength );
      switch( IsError )
      {
      case BasicNulChar:
         /* OK */
         break;
      case 'A':
         WARN_BAD_FILE_MODE;
         return bwb_zline( l );
         break;
      case 'X':
         WARN_BAD_FILE_NUMBER;
         return bwb_zline( l );
         break;
      case 'B':
         WARN_BAD_FILE_NAME;
         return bwb_zline( l );
         break;
      case 'Y':
         WARN_FIELD_OVERFLOW;
         return bwb_zline( l );
         break;
      case 'M':
         WARN_OUT_OF_MEMORY;
         return bwb_zline( l );
         break;
      default:
         WARN_INTERNAL_ERROR;
         return bwb_zline( l );
         break;
      }
      /* OK */
   }
   RELEASE( T );
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_name()
  
   DESCRIPTION:    This function implements the BASIC NAME
         command to rename a disk file.
  
   SYNTAX:         NAME old_filename AS new_filename
  
***************************************************************/


LineType *
bwb_NAME(LineType * l)
{
   int             r;
   char            atbuf[BasicStringLengthMax + 1];
   char            btbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   /* get the first argument in atbuf */

   {
      char * Value = NULL;
      
      if( line_read_string_expression( l, &Value ) == FALSE )
      {
#if FSLSE
         if (ERROR_PENDING)
         {
            return bwb_zline(l);
         }
#endif
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( Value == NULL )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      bwb_strcpy( atbuf, Value );
      free( Value );
   }
   /* skip required word */
   if( line_skip_word(l, "AS") == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   /* get the second argument in btbuf */
   {
      char * Value = NULL;
      
      if( line_read_string_expression( l, &Value ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( Value == NULL )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      bwb_strcpy( btbuf, Value );
      free( Value );
   }
   /* try to rename the file */
   r = rename(atbuf, btbuf);

   /* detect error */
   if (r != 0)
   {
      WARN_BAD_FILE_NAME;
   }
   return bwb_zline(l);
}


/* EOF */
