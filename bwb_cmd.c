/***************************************************************
  
        bwb_cmd.c       Miscellaneous Commands
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



static int xl_line(FILE * file, LineType * l);
static void bwb_new( void );
static void ProcessEscapeChars(const char *Input, char *Output);
static VariableType * find_variable_by_type( char * name, int dimensions, char vartype );


/* 
--------------------------------------------------------------------------------------------
                               REM
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_REM( LineType * L )
{
   /* 
   SYNTAX:     REM comment
   */
   /*
   This line holds BASIC comments.
   */
   bwx_DEBUG(__FUNCTION__);
   line_skip_eol( L );
   return bwb_zline( L );
}

/* 
--------------------------------------------------------------------------------------------
                               IMAGE
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_IMAGE( LineType * L )
{
   /* 
   SYNTAX:     IMAGE print-using-format
   */
   bwx_DEBUG(__FUNCTION__);
   line_skip_eol( L );
   return bwb_zline( L );
}

/* 
--------------------------------------------------------------------------------------------
                               LET
--------------------------------------------------------------------------------------------
*/


LineType *
bwb_LET( LineType * L )
{
   /*
   SYNTAX:     LET variable = expression
   */
   VariableType *v;
   VariantType   x;      /* no leaks */
   VariantType * X = &x; /* no leaks */
   CLEAR_VARIANT( X );

   bwx_DEBUG(__FUNCTION__);
      
   /* read the list of variables */
   do
   {
      if( (v = line_read_scalar( L )) == NULL )
      {
         WARN_VARIABLE_NOT_DECLARED;
         goto EXIT;
      }
   }
   while( line_skip_comma( L ) );

   /* skip the equal sign */
   if( line_skip_char( L, '=' ) )
   {
      /* OK */
   }
   else
   if( line_skip_word( L, "EQ" ) )
   {
      /* OK */
   }
   else
   if( line_skip_word( L, ".EQ." ) )
   {
      /* OK */
   }
   else
   {
      WARN_SYNTAX_ERROR;
      goto EXIT;
   }

   /* evaluate the expression */
   if( line_read_expression( L, X ) )
   {
      /* save the value */
      if( line_is_eol( L ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         goto EXIT;
      }
      L->position = L->Startpos;

      /* for each variable, assign the value */
      do
      {
         /* read a variable */
         if( (v = line_read_scalar( L )) == NULL )
         {
            WARN_VARIABLE_NOT_DECLARED;
            goto EXIT;
         }
         if( var_set( v, X ) == FALSE )
         {
            WARN_TYPE_MISMATCH;
            goto EXIT;
         }
         RELEASE( X );
      }
      while( line_skip_comma( L ) ); 
      line_skip_eol( L );
   }
   else
   {
      WARN_SYNTAX_ERROR;
   }
EXIT:   
   RELEASE( X );
   return bwb_zline( L );
}

/* 
--------------------------------------------------------------------------------------------
                               GO
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_GO( LineType * L )
{
   bwx_DEBUG(__FUNCTION__);
   WARN_SYNTAX_ERROR;
   return bwb_zline( L );
}

LineType *
bwb_THEN( LineType * L )
{
   bwx_DEBUG(__FUNCTION__);
   WARN_SYNTAX_ERROR;
   return bwb_zline( L );
}

LineType *
bwb_TO( LineType * L )
{
   bwx_DEBUG(__FUNCTION__);
   WARN_SYNTAX_ERROR;
   return bwb_zline( L );
}

LineType *
bwb_STEP( LineType * L )
{
   bwx_DEBUG(__FUNCTION__);
   WARN_SYNTAX_ERROR;
   return bwb_zline( L );
}

LineType *
bwb_OF( LineType * L )
{
   bwx_DEBUG(__FUNCTION__);
   WARN_SYNTAX_ERROR;
   return bwb_zline( L );
}

LineType *
bwb_AS( LineType * L )
{
   bwx_DEBUG(__FUNCTION__);
   WARN_SYNTAX_ERROR;
   return bwb_zline( L );
}


/* 
--------------------------------------------------------------------------------------------
                               AUTO
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_AUTO( LineType * L )
{
   /*
   SYNTAX:     AUTO
   SYNTAX:     AUTO start
   SYNTAX:     AUTO start , increment
   */
   bwx_DEBUG(__FUNCTION__);

   My->AutomaticLineNumber = 0;
   My->AutomaticLineIncrement = 0;
   
   if( line_is_eol( L ) )
   {
      /* AUTO                   */
      My->AutomaticLineNumber = 10;
      My->AutomaticLineIncrement = 10;
      return bwb_zline( L );
   }
   if( line_read_line_number( L, &My->AutomaticLineNumber ) )
   {
      /* AUTO ### ... */
      if( My->AutomaticLineNumber < BasicLineNumberMin || My->AutomaticLineNumber > BasicLineNumberMax )
      {
         WARN_UNDEFINED_LINE;
         return bwb_zline( L );
      }
      if( line_is_eol( L ) )
      {
         /* AUTO start             */
         My->AutomaticLineIncrement = 10;
         return bwb_zline( L );
      }
      else
      if( line_skip_comma( L ) )
      {
         /* AUTO ### ,  ... */
         if( line_read_line_number( L, &My->AutomaticLineIncrement ) )
         {
            /* AUTO start , increment */
            if( My->AutomaticLineIncrement < BasicLineNumberMin || My->AutomaticLineIncrement > BasicLineNumberMax )
            {
               WARN_UNDEFINED_LINE;
               return bwb_zline( L );
            }
            return bwb_zline( L );
         }
      }
   }
   My->AutomaticLineNumber = 0;
   My->AutomaticLineIncrement = 0;
   WARN_SYNTAX_ERROR;
   return bwb_zline( L );
}

/* 
--------------------------------------------------------------------------------------------
                               BREAK
--------------------------------------------------------------------------------------------
*/


LineType *
bwb_BREAK(LineType * l)
{
   /*
   SYNTAX:     BREAK
   SYNTAX:     BREAK line [,...]
   SYNTAX:     BREAK line - line
   */
   bwx_DEBUG(__FUNCTION__);

   if( line_is_eol( l ) )
   {
      /* BREAK */
      /* remove all line breaks */
      LineType *x;
      for (x = My->bwb_start.next; x != &My->bwb_end; x = x->next) 
      {
          x->LineFlags &= ~LINE_BREAK;
      }
      return bwb_zline(l);
   }
   else
   {
      do
      {
         int head;
         int tail;
         
         if( line_read_line_sequence(l, &head, &tail) )
         {
            /* BREAK 's' - 'e' */
            LineType *x;
            if( head < BasicLineNumberMin || head > BasicLineNumberMax )
            {
               WARN_UNDEFINED_LINE;
               return bwb_zline(l);
            }
            if( tail < BasicLineNumberMin || tail > BasicLineNumberMax )
            {
               WARN_UNDEFINED_LINE;
               return bwb_zline(l);
            }
            if( head > tail )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline(l);
            }
            /* valid range */
            /* now go through and list appropriate lines */
            for (x = My->bwb_start.next; x != &My->bwb_end; x = x->next)
            {
               if ( head <= x->number && x->number <= tail)
               {
                  if( x->LineFlags & LINE_NUMBERED )
                  {
                     x->LineFlags |= LINE_BREAK;
                  }
               }
            }
         }
         else
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }
      while( line_skip_comma( l ) );
   }
   return bwb_zline(l);
}


/* 
--------------------------------------------------------------------------------------------
                               DSP
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_DSP(LineType * l)
{
   /*
   SYNTAX:     DSP
   SYNTAX:     DSP variablename [,...]
   */
   VariableType *v;
   bwx_DEBUG(__FUNCTION__);


   if( line_is_eol( l ) )
   {
      /* DSP */
      /* remove all variable displays */
      for (v = My->var_head; v != NULL; v = v->next)
      {
         v->VariableFlags &= ~VARIABLE_DISPLAY; /* bwb_DSP() */
      }
      return bwb_zline(l);
   }
   /* DSP variablename [,...] */
   do
   {
      char varname[BasicNameLengthMax + 1];
      
      if( line_read_varname( l, varname )  )
      {
         /* mark the variable */
         for (v = My->var_head; v != NULL; v = v->next)
         {
            if (bwb_stricmp(v->name, varname) == 0)
            {
               v->VariableFlags |= VARIABLE_DISPLAY; /* bwb_DSP() */
            }
         }
      }
   }
   while( line_skip_comma( l ) );
   return bwb_zline(l);
}


/* 
--------------------------------------------------------------------------------------------
                               GOTO
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_GO_TO(LineType * l)
{
   return bwb_GOTO( l );
}

LineType *
bwb_GOTO(LineType * l)
{
   /*
   SYNTAX:     GOTO line                     ' standard GOTO
   SYNTAX:     GOTO expression               ' calculated GOTO
   SYNTAX:     GOTO expression OF line,...   ' indexed GOTO, same as ON expression GOTO line,...
   */
   int Value = 0;
   int LineNumber = 0;
   LineType *x;
   bwx_DEBUG(__FUNCTION__);
   
   if( line_is_eol( l ) )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_integer_expression( l, &Value ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   if( line_is_eol( l ) )
   {
      /* GOTO linenumber */
      /* 'Value' is the line number */
      LineNumber = Value;
   }
   else
   if( line_skip_word( l, "OF" ) )
   {
      /* GOTO expression OF line, ... */
      /* 'Value' is an index into a list of line numbers */
      if( line_read_index_item( l, Value, &LineNumber ) )
      {
         /* found 'LineNumber' */
      }
      else
      if( My->CurrentVersion->OptionFlags & (OPTION_BUGS_ON) /* GOTO X OF ... */ )
      {
         /* silently fall-thru to the following line */
         line_skip_eol(l);
         return bwb_zline(l);
      }
      else
      {
         /* ERROR */
         WARN_UNDEFINED_LINE;
         return bwb_zline(l);
      }
   }
   else
   if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
   {
      /* GOTO line [,...] ON expression */
      while( line_skip_comma( l ) )
      {
         if( line_read_integer_expression( l, &Value ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }
      if( line_skip_word( l, "ON" ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_read_integer_expression( l, &Value ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      /* 'Value' is an index into a list of line numbers */
      l->position = l->Startpos;
      if( line_read_index_item( l, Value, &LineNumber ) )
      {
         /* found 'LineNumber' */
      }
      else
      {
         /* silently fall-thru to the following line */
         line_skip_eol(l);
         return bwb_zline(l);
      }
      line_skip_eol(l);
   }
   else
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   if( LineNumber < BasicLineNumberMin || LineNumber > BasicLineNumberMax )
   {
      WARN_UNDEFINED_LINE;
      return bwb_zline(l);
   }
   /* valid range */

   x = NULL;
#if THE_PRICE_IS_RIGHT
   if( l->OtherLine != NULL )
   {
      /* look in the cache */   
      if( l->OtherLine->number == LineNumber )
      {
         x = l->OtherLine; /* found in cache */
      }
   }
#endif /* THE_PRICE_IS_RIGHT */
   if( x == NULL )
   {
      x = find_line_number( LineNumber, TRUE ); /* not found in the cache */
   }
   if (x != NULL)
   {
      /* FOUND */
      line_skip_eol(l);
      x->position = 0;
#if THE_PRICE_IS_RIGHT
      l->OtherLine = x; /* save in cache */
#endif /* THE_PRICE_IS_RIGHT */
      return x;
   }
   /* NOT FOUND */
   WARN_UNDEFINED_LINE;
   return bwb_zline(l);
}


/* 
--------------------------------------------------------------------------------------------
                               GOSUB
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_GO_SUB(LineType * l)
{
   return bwb_GOSUB( l );
}

LineType *
bwb_GOSUB(LineType * l)
{
   /*
   SYNTAX:     GOSUB line                     ' standard GOSUB
   SYNTAX:     GOSUB expression               ' calculated GOSUB
   SYNTAX:     GOSUB expression OF line,...   ' indexed GOSUB, same as ON expression GOSUB line,...
   */
   int Value = 0;
   int LineNumber = 0;
   LineType *x = NULL;
   bwx_DEBUG(__FUNCTION__);
   
   if( line_is_eol( l ) )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_integer_expression( l, &Value ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   if( line_is_eol( l ) )
   {
      /* GOSUB linenumber */
      /* 'Value' is the line number */
      LineNumber = Value;
   }
   else
   if( line_skip_word( l, "OF" ) )
   {
      /* GOSUB linenumber [,...] OF expression */
      /* 'Value' is an index into a list of line numbers */
      if( line_read_index_item( l, Value, &LineNumber ) )
      {
         /* found 'LineNumber' */
      }
      else
      if( My->CurrentVersion->OptionFlags & (OPTION_BUGS_ON) /* GOSUB X OF ... */ )
      {
         /* silently fall-thru to the following line */
         line_skip_eol(l);
         return bwb_zline(l);
      }
      else
      {
         /* ERROR */
         WARN_UNDEFINED_LINE;
         return bwb_zline(l);
      }
   }
   else
   if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
   {
      /* GOSUB line [,...] ON expression */
      while( line_skip_comma( l ) )
      {
         if( line_read_integer_expression( l, &Value ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }
      if( line_skip_word( l, "ON" ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_read_integer_expression( l, &Value ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      /* 'Value' is an index into a list of line numbers */
      l->position = l->Startpos;
      if( line_read_index_item( l, Value, &LineNumber ) )
      {
         /* found 'LineNumber' */
      }
      else
      {
         /* silently fall-thru to the following line */
         line_skip_eol(l);
         return bwb_zline(l);
      }
      line_skip_eol(l);
   }
   else
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   if( LineNumber < BasicLineNumberMin || LineNumber > BasicLineNumberMax )
   {
      WARN_UNDEFINED_LINE;
      return bwb_zline(l);
   }
   /* valid range */

   x = NULL;
#if THE_PRICE_IS_RIGHT
   if( l->OtherLine != NULL )
   {
      /* look in the cache */   
      if( l->OtherLine->number == LineNumber )
      {
         x = l->OtherLine; /* found in cache */
      }
   }
#endif /* THE_PRICE_IS_RIGHT */
   if( x == NULL )
   {
      x = find_line_number( LineNumber, TRUE ); /* not found in the cache */
   }
   if (x != NULL)
   {
      /* FOUND */
      line_skip_eol(l);
      /* save current stack level */
      My->stack_head->line = l;
      /* increment exec stack */
      if( bwb_incexec() )
      {
         /* set the new position to x and return x */
         x->position = 0;
         My->stack_head->line = x;
         My->stack_head->code = EXEC_GOSUB;
#if THE_PRICE_IS_RIGHT
         l->OtherLine = x; /* save in cache */
#endif /* THE_PRICE_IS_RIGHT */
         return x;
      }
      else
      {
         /* ERROR */
         WARN_OUT_OF_MEMORY;
         return &My->bwb_end;
      }
   }
   /* NOT FOUND */
   WARN_UNDEFINED_LINE;
   return bwb_zline(l);
}



/* 
--------------------------------------------------------------------------------------------
                               RETURN
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_RETURN(LineType * l)
{
   /*
   SYNTAX:     RETURN
   */

   bwx_DEBUG(__FUNCTION__);
   
   if( My->CurrentVersion->OptionVersionBitmask & ( I70 ) )
   {
      /* RETURN [comment] */
      line_skip_eol(l);
   }
   
   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      /* CBASIC-II: RETURN exits the first FUNCTION or GOSUB */

      while( My->stack_head->code != EXEC_GOSUB && My->stack_head->code != EXEC_FUNCTION )
      {
         bwb_decexec();
         if( My->stack_head == NULL )
         {
            WARN_RETURN_WITHOUT_GOSUB;
            return bwb_zline(l);
         }
         if (My->stack_head->code == EXEC_NORM)  /* End of the line? */
         {
            WARN_RETURN_WITHOUT_GOSUB;
            return bwb_zline(l);
         }
      }
   }
   else
   {
      /* RETURN exits the first GOSUB */

      while (My->stack_head->code != EXEC_GOSUB)
      {
         bwb_decexec();
         if( My->stack_head == NULL )
         {
            WARN_RETURN_WITHOUT_GOSUB;
            return bwb_zline(l);
         }
         if (My->stack_head->code == EXEC_NORM)  /* End of the line? */
         {
            WARN_RETURN_WITHOUT_GOSUB;
            return bwb_zline(l);
         }
      }
   }
   

   /* decrement the EXEC stack counter */

   bwb_decexec();
   return My->stack_head->line;
}

/* 
--------------------------------------------------------------------------------------------
                               POP
--------------------------------------------------------------------------------------------
*/


LineType *
bwb_POP(LineType * l)
{
   /*
   SYNTAX:     POP
   */
   StackType *stack_item;
   bwx_DEBUG(__FUNCTION__);
   stack_item = My->stack_head;

   while (stack_item->code != EXEC_GOSUB)
   {
      stack_item = stack_item->next;
      if( stack_item == NULL )
      {
         WARN_RETURN_WITHOUT_GOSUB;
         return bwb_zline(l);
      }
      if (stack_item->code == EXEC_NORM)  /* End of the line? */
      {
         WARN_RETURN_WITHOUT_GOSUB;
         return bwb_zline(l);
      }
   }
   /* hide the GOSUB */
   stack_item->code = EXEC_POPPED;
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               ON
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_ON(LineType * l)
{
   /*
   SYNTAX:     ON expression GOTO  line,...    ' expression evaluates to an index
   SYNTAX:     ON expression GOSUB line,...    ' expression evaluates to an index
   */
   int Value = 0;
   int command = 0;
   int LineNumber = 0;
   LineType *x = NULL;
   bwx_DEBUG(__FUNCTION__);
   
   if( line_is_eol( l ) )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_integer_expression( l, &Value ) == FALSE )
   {
      WARN_UNDEFINED_LINE;
      return bwb_zline(l);
   }
   if( line_skip_word( l, "GO" ) )
   {
      if( line_skip_word( l, "TO" ) )
      {
         command = C_GOTO;
      }
      else
      if( line_skip_word( l, "SUB" ) )
      {
         command = C_GOSUB;
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }
   else
   if( line_skip_word( l, "GOTO" ) )
   {
      command = C_GOTO;
   }
   else
   if( line_skip_word( l, "GOSUB" ) )
   {
      command = C_GOSUB;
   }
   else
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   
   /* 'Value' is an index into a list of line numbers */
   if( line_read_index_item( l, Value, &LineNumber ) )
   {
      /* found 'LineNumber' */
   }
   else
   if( My->CurrentVersion->OptionFlags & (OPTION_BUGS_ON) /* ON X GOTO|GOSUB ... */ )
   {
      /* silently fall-thru to the following line */
      line_skip_eol(l);
      return bwb_zline(l);
   }
   else
   {
      /* ERROR */
      WARN_UNDEFINED_LINE;
      return bwb_zline(l);
   }

   if( LineNumber < BasicLineNumberMin || LineNumber > BasicLineNumberMax )
   {
      WARN_UNDEFINED_LINE;
      return bwb_zline(l);
   }
   /* valid range */
   x = NULL;
#if THE_PRICE_IS_RIGHT
   if( l->OtherLine != NULL )
   {
      /* look in the cache */   
      if( l->OtherLine->number == LineNumber )
      {
         x = l->OtherLine; /* found in cache */
      }
   }
#endif /* THE_PRICE_IS_RIGHT */
   if( x == NULL )
   {
      x = find_line_number( LineNumber, TRUE ); /* not found in the cache */
   }
   if (x != NULL)
   {
      /* FOUND */
      if( command == C_GOTO )
      {
         /* ON ... GOTO ... */
         line_skip_eol(l);
         x->position = 0;
#if THE_PRICE_IS_RIGHT
         l->OtherLine = x; /* save in cache */
#endif /* THE_PRICE_IS_RIGHT */
         return x;
      }
      else
      if( command == C_GOSUB )
      {
         /* ON ... GOSUB ... */
         line_skip_eol(l);
         /* save current stack level */
         My->stack_head->line = l;
         /* increment exec stack */
         if( bwb_incexec() )
         {
            /* set the new position to x and return x */
            x->position = 0;
            My->stack_head->line = x;
            My->stack_head->code = EXEC_GOSUB;
#if THE_PRICE_IS_RIGHT
            l->OtherLine = x; /* save in cache */
#endif /* THE_PRICE_IS_RIGHT */
            return x;
         }
         else
         {
            /* ERROR */
            WARN_OUT_OF_MEMORY;
            return &My->bwb_end;
         }
      }
      else
      {
         /* ERROR */
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }
   /* NOT FOUND */
   WARN_UNDEFINED_LINE;
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               PAUSE
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_PAUSE(LineType * l)
{
   /*
   SYNTAX:     PAUSE
   */
   char            tbuf[BasicStringLengthMax + 1];
   char            prompt[32];
   bwx_DEBUG(__FUNCTION__);
   
   if( My->CurrentVersion->OptionVersionBitmask & ( I70 ) )
   {
      /* PAUSE [comment] */
      line_skip_eol(l);
   }
   sprintf( prompt, "PAUSE AT %d\n", l->number );
   bwx_input( prompt, tbuf );
   return bwb_zline(l);
}


/* 
--------------------------------------------------------------------------------------------
                               STOP
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_STOP(LineType * l)
{
   /*
   SYNTAX:     STOP
   */
   bwx_DEBUG(__FUNCTION__);
   
   if( My->CurrentVersion->OptionVersionBitmask & ( I70 ) )
   {
      /* STOP [comment] */
      line_skip_eol(l);
   }
   My->ContinueLine = l->next;
   bwx_STOP();
   return bwb_END(l);
}


/* 
--------------------------------------------------------------------------------------------
                               END
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_END(LineType * l)
{
   /*
   SYNTAX:     END
   */
   bwx_DEBUG(__FUNCTION__);

   if( My->CurrentVersion->OptionVersionBitmask & ( I70 ) )
   {
      /* END [comment] */
      line_skip_eol(l);
   }
   My->ContinueLine = l->next;
   break_handler();
   return &My->bwb_end;
}

/* 
--------------------------------------------------------------------------------------------
                               RUN
--------------------------------------------------------------------------------------------
*/

static
LineType *
bwb_run_filename_or_linenumber( LineType * L )
{
   LineType *current = NULL;
   VariantType   x;      /* no leaks */
   VariantType * X = &x; /* no leaks */
   CLEAR_VARIANT( X );

   if( line_read_expression( L, X ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      goto EXIT;
   }
   if( X->TypeChar == BasicStringSuffix )
   {
      /* RUN "filename" */
      /* RUN A$ */
      if( X->Length == 0 )
      {
         WARN_BAD_FILE_NAME;
         goto EXIT;
      }
      /* open the file and execute it */
      bwb_new(); /* clear memory */
      if( bwb_fload( X->Buffer ) == FALSE )
      {
         WARN_BAD_FILE_NAME;
         goto EXIT;
      }
      if( bwb_scan() == FALSE )
      {
         WARN_CANT_CONTINUE;
         goto EXIT;
      }
      current = My->bwb_start.next;
   }
   else
   {
      /* RUN 100 */
      /* RUN N */
      /* execute the line */
      int  LineNumber;


      LineNumber = (int) bwb_rint( X->Number );
      if( bwb_scan() == FALSE )
      {
         WARN_CANT_CONTINUE;
         goto EXIT;
      }
      if( BasicLineNumberMin <= LineNumber &&  LineNumber <= BasicLineNumberMax )
      {
         current = find_line_number( LineNumber, FALSE );
      }
   }
EXIT:   
   RELEASE( X );
   return current;
}


LineType *
bwb_RUN( LineType * L )
{
   /*
   SYNTAX:     RUN
   SYNTAX:     RUN filename$
   SYNTAX:     RUN linenumber
   */
   LineType *current;

   bwx_DEBUG(__FUNCTION__);


   /* clear the STACK */
   bwb_clrexec();
   if( bwb_incexec() )
   {
      /* OK */
   }
   else
   {
      /* ERROR */
      WARN_OUT_OF_MEMORY;
      return &My->bwb_end;
   }
   

   /* see if there is an element */
   line_skip_spaces( L );
   if ( line_is_eol( L ) )
   {
      /* RUN */

      var_CLEAR();

      /* if( TRUE ) */
      {
         int n;
         for (n = 0; n < 26; n++)
         {
            My->DefaultVariableType[n] = BasicDoubleSuffix;
         }
      }
      if( bwb_scan() == FALSE )
      {
         WARN_CANT_CONTINUE;
         return &My->bwb_end;
      }
      current = My->bwb_start.next;
   }
   else
   {
      /* RUN 100 : RUN "FILENAME.BAS" */
      current = bwb_run_filename_or_linenumber( L );
      if (current == NULL)
      {
         WARN_UNDEFINED_LINE;
         return &My->bwb_end;
      }
   }
   current->position = 0;

   My->stack_head->line = current;
   My->stack_head->code = EXEC_NORM;

   /* RUN */
   bwb_Warning_Clear();
   My->ContinueLine = NULL;
   SetOnError(0);

   /* if( TRUE ) */
   {
      time_t   t;
      struct tm *lt;
      
      time(&t);
      lt = localtime(&t);
      My->StartTime = lt->tm_hour;
      My->StartTime *= 60;
      My->StartTime += lt->tm_min;
      My->StartTime *= 60;
      My->StartTime += lt->tm_sec;
      /* number of seconds since midnight */
   }

   return current;
}

/* 
--------------------------------------------------------------------------------------------
                               CONT
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_CONT(LineType * l)
{
   /*
   SYNTAX:     CONT
   */
   LineType *current = NULL;

   bwx_DEBUG(__FUNCTION__);


   /* see if there is an element */
   if( line_is_eol( l ) )
   {
      /* CONT */
      current = My->ContinueLine;
   }
   else
   {
      /* CONT 100 */
      int LineNumber = 0 ;
      if( line_read_line_number( l, &LineNumber ) )
      {
         current = find_line_number( LineNumber, TRUE );
      }
   }
   

   if (current == NULL || current == &My->bwb_end)
   {
      /* same as "RUN" */
      current = My->bwb_start.next;
   }
   if( bwb_scan() == FALSE )
   {
      WARN_CANT_CONTINUE;
      return bwb_zline(l);
   }

   current->position = 0;
   bwb_clrexec();
   if( bwb_incexec() )
   {
      /* OK */
   }
   else
   {
      /* ERROR */
      WARN_OUT_OF_MEMORY;
      return &My->bwb_end;
   }
   
   My->stack_head->line = current;
   My->stack_head->code = EXEC_NORM;

   /* CONT */
   My->ContinueLine = NULL;
   return current;
}


/* 
--------------------------------------------------------------------------------------------
                               NEW
--------------------------------------------------------------------------------------------
*/

static void bwb_new()
{
   bwx_DEBUG(__FUNCTION__);

   /* clear program in memory */
   bwb_xnew(&My->bwb_start);

   /* clear all variables */
   var_CLEAR();
   /* if( TRUE ) */
   {
      int n;
      for (n = 0; n < 26; n++)
      {
         My->DefaultVariableType[n] = BasicDoubleSuffix;
      }
   }

   /* NEW */
   bwb_Warning_Clear();
   My->ContinueLine = NULL;
   SetOnError(0);
}

LineType *
bwb_NEW(LineType * l)
{
   /*
   SYNTAX:     NEW
   */
   bwx_DEBUG(__FUNCTION__);
   bwb_new();
   if( My->CurrentVersion->OptionVersionBitmask & (D64 | G65 | G67) )
   {
      if( line_is_eol(l) )
      {
         /* NEW */
         /* prompt for the program name */
         bwx_input( "NEW PROBLEM NAME:", My->progfile );
      }
      else
      {
         /* NEW filename$ */
         /* the parameter is the program name */

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
         bwb_strcpy( My->progfile, Value );
         free( Value );
      }
   }
   else
   {
      /* ignore any parameters */
      line_skip_eol(l); 
   }
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               SCRATCH
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_SCRATCH(LineType * l)
{
   /*
   SYNTAX:     SCRATCH              -- same as NEW
   SYNTAX:     SCRATCH # filenumber -- close file and re-open for output
   */
   bwx_DEBUG(__FUNCTION__);
   
   if( line_is_eol(l) )
   {
      /* SCRATCH */
      bwb_new();
      return bwb_zline(l);
   }
   else
   if( line_skip_char(l, BasicFileNumberPrefix ) )
   {
      /* SCRATCH # X */
      int FileNumber;

      if( line_read_integer_expression( l, &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( FileNumber < 0 )
      {
         /* "SCRATCH # -1" is silently ignored */
         return bwb_zline(l);
      }
      if( FileNumber == 0 )
      {
         /* "SCRATCH # 0" is silently ignored */
         return bwb_zline(l);
      }
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
            FREE( My->CurrentFile->buffer, "bwb_SCRATCHS" );
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
         if( (My->CurrentFile->cfp = fopen( My->CurrentFile->filename, "w" )) == NULL )
         {
            WARN_BAD_FILE_NAME;
            return bwb_zline(l);
         }
         My->CurrentFile->mode = DEVMODE_OUTPUT;
      }
      /* OK */
      return bwb_zline(l);
   }
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}


/* 
--------------------------------------------------------------------------------------------
                               BYE
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_BYE(LineType * l)
{
   /*
   SYNTAX:     BYE
   */
   bwx_DEBUG(__FUNCTION__);
   return bwb_SYSTEM(l);
}

/* 
--------------------------------------------------------------------------------------------
                               GOODBYE
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_GOODBYE(LineType * l)
{
   /*
   SYNTAX:     GOODBYE
   */
   bwx_DEBUG(__FUNCTION__);
   return bwb_SYSTEM(l);
}

/* 
--------------------------------------------------------------------------------------------
                               QUIT
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_QUIT(LineType * l)
{
   /*
   SYNTAX:     QUIT
   */
   bwx_DEBUG(__FUNCTION__);
   return bwb_SYSTEM(l);
}

/* 
--------------------------------------------------------------------------------------------
                               SYSTEM
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_SYSTEM(LineType * l)
{
   /*
   SYNTAX:     SYSTEM
   */
   bwx_DEBUG(__FUNCTION__);
   prn_xprintf("\n");
   bwx_terminate();
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               SYSTEM
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_LOAD(LineType * l)
{
   /*
   SYNTAX:     LOAD filename$
   */
   bwx_DEBUG(__FUNCTION__);

   /* clear current contents */
   bwb_new();

   /* call xload function to load program in memory */

   bwb_xload(l);

   return bwb_zline(l);

}

/* 
--------------------------------------------------------------------------------------------
                               OLD
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_OLD(LineType * l)
{
   /*
   SYNTAX:     OLD filename$
   */
   bwx_DEBUG(__FUNCTION__);

   return bwb_LOAD(l);
}

/* 
--------------------------------------------------------------------------------------------
                               RENAME
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_RENAME(LineType * l)
{
   /*
   SYNTAX:     RENAME filename$
   */
   
   bwx_DEBUG(__FUNCTION__);
   if (line_is_eol(l))
   {
      /* RENAME */
      if( My->CurrentVersion->OptionVersionBitmask & (D64 | G65 | G67) )
      {
         /* prompt for the program name */
         bwx_input( "RENAME PROBLEM NAME:", My->progfile );
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }
   else
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
      bwb_strcpy( My->progfile, Value );
      free( Value );
   }
   if (My->progfile[0] == BasicNulChar)
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_xload()
  
        DESCRIPTION: This C function loads a BASIC program
         into memory.
  
***************************************************************/

LineType *
bwb_xload(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* Get an argument for filename */
   line_skip_spaces(l);
   if (line_is_eol(l))
   {
      /* default is the last filename used by LOAD or SAVE */
      if( My->CurrentVersion->OptionVersionBitmask & (D64 | G65 | G67) )
      {
         /* OLD */
         /* prompt for the program name */
         bwx_input( "OLD PROBLEM NAME:", My->progfile );
      }
   }
   else
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
      bwb_strcpy( My->progfile, Value );
      free( Value );
   }
   if (My->progfile[0] == BasicNulChar)
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   if( bwb_fload( NULL /* My->progfile */ ) == FALSE )
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               MAT
--------------------------------------------------------------------------------------------
*/

int
Determinant( VariableType * v )
{
   /* http://easy-learn-c-language.blogspot.com/search/label/Numerical%20Methods */
   /* Numerical Methods: Determinant of nxn matrix using C */

    BasicNumberType  ** matrix;
    BasicNumberType ratio;

    int i;
    int j;
    int k;
    int n;


   My->LastDeterminant = 0; /* default */

   n = v->UBOUND[0] - v->LBOUND[0] + 1;
   
   if( (matrix = (BasicNumberType **) calloc( n, sizeof( BasicNumberType * ) )) == NULL )
   {
      return FALSE;
   }
   for(i = 0; i < n; i++)
   {
      if( (matrix[i] = (BasicNumberType *)calloc( n, sizeof( BasicNumberType ) )) == NULL )
      {
         return FALSE;
      }
   }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            VariantType variant;
            v->array_pos[0] = v->LBOUND[0] + i;
            v->array_pos[1] = v->LBOUND[1] + j;
            if( var_get( v, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return FALSE;
            }
            if( variant.TypeChar == '$' )
            {
               WARN_TYPE_MISMATCH;
               return FALSE;
            }
            matrix[i][j] = variant.Number;
        }
    }

    /* Conversion of matrix to upper triangular */

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            if(j>i)
            {
                if( matrix[i][i] == 0 )
                {
                     /* - Evaluation of an expression results in division
                      * by zero (nonfatal, the recommended recovery
                      * procedure is to supply machine infinity with the
                      * sign of the numerator and continue) 
                      */
                     if( bwb_Warning_Overflow("*** WARNING:  Divide by 0 ***") )
                     {
                        /* ERROR */
                        goto Error;
                     }
                     /* CONTINUE */
                     if( matrix[j][i] < 0 )
                     {
                        ratio = -DBL_MAX;
                     }
                     else
                     {
                        ratio = DBL_MAX;
                     }
                }
                else
                {
                   ratio = matrix[j][i]/matrix[i][i];
                }
                for(k = 0; k < n; k++)
                {
                    matrix[j][k] -= ratio * matrix[i][k];
                }
            }
        }
    }
    

    My->LastDeterminant = 1; /* storage for determinant */

    for(i = 0; i < n; i++)
    {
        BasicNumberType Value;           

        Value = matrix[i][i];
        My->LastDeterminant *= Value;
    }

Error:

   for(i = 0; i < n; i++)
   {
      free( matrix[i] );
   }
   free( matrix );
   
    return TRUE;
}

int
InvertMatrix( VariableType * vOut, VariableType * vIn )
{
   /* http://easy-learn-c-language.blogspot.com/search/label/Numerical%20Methods */
   /* Numerical Methods: Inverse of nxn matrix using C */

    BasicNumberType  ** matrix;
    BasicNumberType ratio;

    int i;
    int j;
    int k;
    int n;


   n = vIn->UBOUND[0] - vIn->LBOUND[0] + 1;
   
   if( (matrix = (BasicNumberType **) calloc( n, sizeof( BasicNumberType * ) )) == NULL )
   {
      return FALSE;
   }
   
   for(i = 0; i < n; i++)
   {
      if( (matrix[i] = (BasicNumberType *)calloc( n+n, sizeof( BasicNumberType ) )) == NULL )
      {
         return FALSE;
      }
   }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            VariantType variant;
            vIn->array_pos[0] = vIn->LBOUND[0] + i;
            vIn->array_pos[1] = vIn->LBOUND[1] + j;
            if( var_get( vIn, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return FALSE;
            }
            if( variant.TypeChar == '$' )
            {
               WARN_TYPE_MISMATCH;
               return FALSE;
            }
            matrix[i][j] = variant.Number;
        }
    }

    for(i = 0; i < n; i++)
    {
        for(j = n; j < 2*n; j++)
        {
            if(i==(j-n))
            {
                matrix[i][j] = 1.0;
            }
            else
            {
                matrix[i][j] = 0.0;
            }
        }
    }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            if(i!=j)
            {
                if( matrix[i][i] == 0 )
                {
                     /* - Evaluation of an expression results in division
                      * by zero (nonfatal, the recommended recovery
                      * procedure is to supply machine infinity with the
                      * sign of the numerator and continue) 
                      */
                     if( bwb_Warning_Overflow("*** WARNING:  Divide by 0 ***") )
                     {
                        /* ERROR */
                        return FALSE;
                     }
                     /* CONTINUE */
                     if( matrix[j][i] < 0 )
                     {
                        ratio = -DBL_MAX;
                     }
                     else
                     {
                        ratio = DBL_MAX;
                     }
                }
                else
                {
                   ratio = matrix[j][i]/matrix[i][i];
                }
                for(k = 0; k < 2*n; k++)
                {
                    matrix[j][k] -= ratio * matrix[i][k];
                }
            }
        }
    }

    for(i = 0; i < n; i++)
    {
       BasicNumberType a;

        a = matrix[i][i];
        if( a == 0 )
        {
            /* - Evaluation of an expression results in division
             * by zero (nonfatal, the recommended recovery
             * procedure is to supply machine infinity with the
             * sign of the numerator and continue) 
             */
            if( bwb_Warning_Overflow("*** WARNING:  Divide by 0 ***") )
            {
               /* ERROR */
               return FALSE;
            }
            /* CONTINUE */
           for(j = 0; j < 2*n; j++)
           {
               if( matrix[i][j] < 0 )
               {
                  matrix[i][j] = -DBL_MAX;
               }
               else
               {
                  matrix[i][j] = DBL_MAX;
               }
           }
        }
        else
        {
           for(j = 0; j < 2*n; j++)
           {
               matrix[i][j] /= a;
           }
        }
    }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            VariantType variant;
            
            vOut->array_pos[0] = vOut->LBOUND[0] + i;
            vOut->array_pos[1] = vOut->LBOUND[0] + j;
            variant.TypeChar = vOut->VariableTypeChar;
            variant.Number = matrix[i][j + n];
            if( var_set( vOut, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return FALSE;
            }
        }
    }

   for(i = 0; i < n; i++)
   {
      free( matrix[i] );
   }
   free( matrix );

    return TRUE;
}

static int
line_read_matrix_redim( LineType * l, VariableType *v_A )
{
   /* get OPTIONAL parameters if the variable is dimensioned */
   line_skip_spaces(l);
   if ( line_peek_char(l,BasicLeftParenChar))
   {
      /* get requested size, which is <= original array size */
      size_t array_units;
      int    n;
      int    n_params;  /* number of parameters */
      int    pp[ MAX_DIMS ];

      for (n = 0; n < MAX_DIMS; n++)
      {
         pp[n] = 0;
      }
      if( line_read_array_dimensions(l, &n_params, pp) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return FALSE;
      }
      /* update array dimensions */
      array_units = 1;
      for (n = 0; n < n_params; n++)
      {
         if( pp[n] < My->CurrentVersion->OptionBaseValue )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return FALSE;
         }
         array_units *= pp[n] - My->CurrentVersion->OptionBaseValue + 1;
      }
      if( array_units > v_A->array_units )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return FALSE;
      }
      v_A->dimensions = n_params;
      for (n = 0; n < v_A->dimensions; n++)
      {
         v_A->LBOUND[n] = My->CurrentVersion->OptionBaseValue;
         v_A->UBOUND[n] = pp[n];
      }
   }
   return TRUE;
}

LineType *
bwb_MAT(LineType * l)
{
   /*
   SYNTAX:     MAT A = CON
   SYNTAX:     MAT A = IDN
   SYNTAX:     MAT A = ZER
   SYNTAX:     MAT A = INV B
   SYNTAX:     MAT A = TRN B
   SYNTAX:     MAT A = (k) * B
   SYNTAX:     MAT A = B
   SYNTAX:     MAT A = B + C
   SYNTAX:     MAT A = B - C
   SYNTAX:     MAT A = B * C
   */
   VariableType *v_A;
   char varname_A[BasicNameLengthMax + 1];
   
   bwx_DEBUG(__FUNCTION__);
   /* just a placeholder for now. this will grow. */

   if( line_read_varname(l, varname_A) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }   
   v_A = mat_find(varname_A);
   if( v_A == NULL)
   {
      WARN_VARIABLE_NOT_DECLARED;
      return bwb_zline(l);
   }
   /* variable MUST be numeric */
   if ( VAR_IS_STRING( v_A ) )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_matrix_redim( l, v_A ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_skip_char(l, '=') == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* MAT A = ... */
   if( line_skip_word(l,"CON") )
   {
      /* MAT A = CON */
      /* MAT A = CON(I) */
      /* MAT A = CON(I,J) */
      /* MAT A = CON(I,J,K) */
      /* OK */
      int i;
      int j;
      int k;

      if( line_read_matrix_redim( l, v_A ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }

      /* both arrays are of the same size */
      switch( v_A->dimensions )
      {
      case 1:
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            VariantType variant;
            
            variant.TypeChar = v_A->VariableTypeChar;
            variant.Number = 1;
            v_A->array_pos[0] = i;
            if( var_set( v_A, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return bwb_zline(l);
            }
         }
         break;
      case 2:
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
            {
               VariantType variant;
               
               variant.TypeChar = v_A->VariableTypeChar;
               variant.Number = 1;
               v_A->array_pos[0] = i;
               v_A->array_pos[1] = j;
               if( var_set( v_A, &variant ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           
            }
         }
         break;
      case 3:
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
            {
               for( k = v_A->LBOUND[2]; k <= v_A->UBOUND[2]; k++ )
               {
                  VariantType variant;
                  
                  variant.TypeChar = v_A->VariableTypeChar;
                  variant.Number = 1;
                  v_A->array_pos[0] = i;
                  v_A->array_pos[1] = j;
                  v_A->array_pos[2] = k;
                  if( var_set( v_A, &variant ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           
               }
            }
         }
         break;
      default:
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }
   else
   if( line_skip_word(l,"IDN") )
   {
      /* MAT A = IDN */
      /* MAT A = IDN(I,J) */
      /* OK */
      int i;
      int j;

      if( line_read_matrix_redim( l, v_A ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }

      if( v_A->dimensions != 2 )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return bwb_zline(l);
      }
      if( v_A->LBOUND[0] != v_A->LBOUND[1] )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return bwb_zline(l);
      }
      if( v_A->UBOUND[0] != v_A->UBOUND[1] )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return bwb_zline(l);
      }
      /* square matrix */
      for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
      {
         for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
         {
            VariantType variant;
            
            variant.TypeChar = v_A->VariableTypeChar;
            if( i == j )
            {
               variant.Number = 1;
            }
            else
            {
               variant.Number = 0;
            }
            v_A->array_pos[0] = i;
            v_A->array_pos[1] = j;
            if( var_set( v_A, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return bwb_zline(l);
            }           
         }
      }
   }
   else
   if( line_skip_word(l,"ZER") )
   {
      /* MAT A = ZER */
      /* MAT A = ZER(I) */
      /* MAT A = ZER(I,J) */
      /* MAT A = ZER(I,J,K) */
      /* OK */
      int i;
      int j;
      int k;

      if( line_read_matrix_redim( l, v_A ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      /* both arrays are of the same size */
      switch( v_A->dimensions )
      {
      case 1:
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            VariantType variant;
            
            variant.TypeChar = v_A->VariableTypeChar;
            variant.Number = 0;
            v_A->array_pos[0] = i;
            if( var_set( v_A, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return bwb_zline(l);
            }           
         }
         break;
      case 2:
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
            {
               VariantType variant;
               
               variant.TypeChar = v_A->VariableTypeChar;
               variant.Number = 0;
               v_A->array_pos[0] = i;
               v_A->array_pos[1] = j;
               if( var_set( v_A, &variant ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           
            }
         }
         break;
      case 3:
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
            {
               for( k = v_A->LBOUND[2]; k <= v_A->UBOUND[2]; k++ )
               {
                  VariantType variant;
                  
                  variant.TypeChar = v_A->VariableTypeChar;
                  variant.Number = 0;
                  v_A->array_pos[0] = i;
                  v_A->array_pos[1] = j;
                  v_A->array_pos[2] = k;
                  if( var_set( v_A, &variant ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           
               }
            }
         }
         break;
      default:
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }
   else
   if( line_skip_word(l,"INV") )
   {
      /* MAT A = INV B */
      /* MAT A = INV( B ) */
      /* OK */
      VariableType *v_B;
      char varname_B[BasicNameLengthMax + 1];
      
      if (v_A->dimensions != 2)
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if ( v_A->LBOUND[0] != v_A->LBOUND[1] || v_A->UBOUND[0] != v_A->UBOUND[1] )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return bwb_zline(l);
      }
      if( line_skip_char(l,BasicLeftParenChar) == FALSE )
      {
         /* optional */
      }
      if( line_read_varname(l, varname_B) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }   
      
      if( (v_B = mat_find(varname_B)) == NULL)
      {
         WARN_VARIABLE_NOT_DECLARED;
         return bwb_zline(l);
      }
      /* variable MUST be numeric */
      if ( VAR_IS_STRING( v_B ) )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_read_matrix_redim( l, v_B ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_skip_char(l,BasicRightParenChar) == FALSE )
      {
         /* optional */
      }
      if (v_B->dimensions != 2)
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if ( v_B->LBOUND[0] != v_B->LBOUND[1] || v_B->UBOUND[0] != v_B->UBOUND[1] )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return bwb_zline(l);
      }
      if ( v_A->LBOUND[0] != v_B->LBOUND[0] || v_A->UBOUND[0] != v_B->UBOUND[0] )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return bwb_zline(l);
      }
      /* square matrix */
      if( Determinant( v_B ) == FALSE )
      {
         WARN_ILLEGAL_FUNCTION_CALL;
         return bwb_zline(l);
      }
      if( My->LastDeterminant == 0 )
      {
         WARN_ILLEGAL_FUNCTION_CALL;
         return bwb_zline(l);
      }
      if( InvertMatrix( v_A, v_B ) == FALSE )
      {
         WARN_ILLEGAL_FUNCTION_CALL;
         return bwb_zline(l);
      }
   }
   else
   if( line_skip_word(l,"TRN") )
   {
      /* MAT A = TRN B */
      /* MAT A = TRN( B ) */
      /* OK */
      int i;
      int j;
      VariableType *v_B;
      char varname_B[BasicNameLengthMax + 1];

      if( v_A->dimensions != 2 )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_skip_char(l,BasicLeftParenChar) == FALSE )
      {
         /* optional */
      }
      if( line_read_varname(l, varname_B) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }   
      if( (v_B = mat_find(varname_B)) == NULL)
      {
         WARN_VARIABLE_NOT_DECLARED;
         return bwb_zline(l);
      }
      /* variable MUST be numeric */
      if ( VAR_IS_STRING( v_B ) )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_read_matrix_redim( l, v_B ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_skip_char(l,BasicRightParenChar) == FALSE )
      {
         /* optional */
      }
      if( v_B->dimensions != 2 )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      /* MxN */
      if( v_A->LBOUND[0] != v_B->LBOUND[1] || v_A->UBOUND[0] != v_B->UBOUND[1] )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( v_A->LBOUND[1] != v_B->LBOUND[0] || v_A->UBOUND[1] != v_B->UBOUND[0] )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      /* transpose matrix */
      for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
      {
         for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
         {
            VariantType variant;
            
            v_B->array_pos[1] = i;
            v_B->array_pos[0] = j;
            if( var_get( v_B, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return bwb_zline(l);
            }           
            v_A->array_pos[0] = i;
            v_A->array_pos[1] = j;
            if( var_set( v_A, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return bwb_zline(l);
            }           
         }
      }
   }
   else
   if( line_peek_char(l,BasicLeftParenChar) ) 
   {
      /* MAT A = (k) * B */
      BasicNumberType Multiplier; 
      VariableType *v_B;
      int i;
      int j;
      int k;
      char *E;
      int p;
      char varname_B[BasicNameLengthMax + 1];
      char tbuf[BasicStringLengthMax + 1];
      
      bwb_strcpy( tbuf, &(l->buffer[l->position]));
      E = bwb_strrchr( tbuf, '*' );
      if( E == NULL )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      *E = BasicNulChar;
      p = 0;
      if( buff_read_numeric_expression(tbuf, &p, &Multiplier) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      l->position += p;
      if( line_skip_char(l,'*') == FALSE ) 
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_read_varname(l, varname_B) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }   
      
      if( (v_B = mat_find(varname_B)) == NULL)
      {
         WARN_VARIABLE_NOT_DECLARED;
         return bwb_zline(l);
      }
      /* variable MUST be numeric */
      if ( VAR_IS_STRING( v_B ) )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_read_matrix_redim( l, v_B ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( v_A->dimensions != v_B->dimensions )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      /* both arrays are of the same size */
      switch( v_A->dimensions )
      {
      case 1:
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            VariantType variant;
            
            v_B->array_pos[0] = i;
            if( var_get( v_B, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return bwb_zline(l);
            }           

            variant.Number *= Multiplier;

            v_A->array_pos[0] = i;
            if( var_set( v_A, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return bwb_zline(l);
            }           
         }
         break;
      case 2:
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
            {
               VariantType variant;
               
               v_B->array_pos[0] = i;
               v_B->array_pos[1] = j;
               if( var_get( v_B, &variant ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           

               variant.Number *= Multiplier;

               v_A->array_pos[0] = i;
               v_A->array_pos[1] = j;
               if( var_set( v_A, &variant ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           
            }
         }
         break;
      case 3:
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
            {
               for( k = v_A->LBOUND[2]; k <= v_A->UBOUND[2]; k++ )
               {
                  VariantType variant;
                  
                  v_B->array_pos[0] = i;
                  v_B->array_pos[1] = j;
                  v_B->array_pos[2] = k;
                  if( var_get( v_B, &variant ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           

                  variant.Number *= Multiplier;

                  v_A->array_pos[0] = i;
                  v_A->array_pos[1] = j;
                  v_A->array_pos[2] = k;
                  if( var_set( v_A, &variant ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           
               }
            }
         }
         break;
      default:
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }
   else
   {
      /* MAT A = B */
      /* MAT A = B + C */
      /* MAT A = B - C */
      /* MAT A = B * C */
      VariableType *v_B;
      char varname_B[BasicNameLengthMax + 1];
      
      if( line_read_varname(l, varname_B) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }   
      
      if( (v_B = mat_find(varname_B)) == NULL)
      {
         WARN_VARIABLE_NOT_DECLARED;
         return bwb_zline(l);
      }
      /* variable MUST be numeric */
      if ( VAR_IS_STRING( v_B ) )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_read_matrix_redim( l, v_B ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_is_eol(l) )
      {
         /* MAT A = B */
         /* OK */
         int i;
         int j;
         int k;
         
         if( v_A->dimensions != v_B->dimensions )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         /* both arrays are of the same size */
         switch( v_A->dimensions )
         {
         case 1:
            for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
            {
               VariantType variant;
               
               v_B->array_pos[0] = i;
               if( var_get( v_B, &variant ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           

               v_A->array_pos[0] = i;
               if( var_set( v_A, &variant ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           
            }
            break;
         case 2:
            for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
            {
               for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
               {
                  VariantType variant;
                  
                  v_B->array_pos[0] = i;
                  v_B->array_pos[1] = j;
                  if( var_get( v_B, &variant ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           

                  v_A->array_pos[0] = i;
                  v_A->array_pos[1] = j;
                  if( var_set( v_A, &variant ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           
               }
            }
            break;
         case 3:
            for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
            {
               for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
               {
                  for( k = v_A->LBOUND[2]; k <= v_A->UBOUND[2]; k++ )
                  {
                     VariantType variant;
                     
                     v_B->array_pos[0] = i;
                     v_B->array_pos[1] = j;
                     v_B->array_pos[2] = k;
                     if( var_get( v_B, &variant ) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }           

                     v_A->array_pos[0] = i;
                     v_A->array_pos[1] = j;
                     v_A->array_pos[2] = k;
                     if( var_set( v_A, &variant ) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }           
                  }
               }
            }
            break;
         default:
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }
      else
      if( line_skip_char(l,'+') ) 
      {
         /* MAT A = B + C */
         /* OK */
         int i;
         int j;
         int k;
         VariableType *v_C;
         char varname_C[BasicNameLengthMax + 1];
         
         if( v_A->dimensions != v_B->dimensions )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         /* both arrays are of the same size */

         if( line_read_varname(l, varname_C) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }   
         
         if( (v_C = mat_find(varname_C)) == NULL)
         {
            WARN_VARIABLE_NOT_DECLARED;
            return bwb_zline(l);
         }
         /* variable MUST be numeric */
         if ( VAR_IS_STRING( v_C ) )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         if( line_read_matrix_redim( l, v_C ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         if( v_B->dimensions != v_C->dimensions )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         /* both arrays are of the same size */
         switch( v_A->dimensions )
         {
         case 1:
            for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
            {
               VariantType variant_L;
               VariantType variant_R;
               
               v_B->array_pos[0] = i;
               if( var_get( v_B, &variant_L ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           

               v_C->array_pos[0] = i;
               if( var_get( v_C, &variant_R ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           

               variant_L.Number += variant_R.Number;

               v_A->array_pos[0] = i;
               if( var_set( v_A, &variant_L ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           
            }
            break;
         case 2:
            for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
            {
               for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
               {
                  VariantType variant_L;
                  VariantType variant_R;
                  
                  v_B->array_pos[0] = i;
                  v_B->array_pos[1] = j;
                  if( var_get( v_B, &variant_L ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           

                  v_C->array_pos[0] = i;
                  v_C->array_pos[1] = j;
                  if( var_get( v_C, &variant_R ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           

                  variant_L.Number += variant_R.Number;

                  v_A->array_pos[0] = i;
                  v_A->array_pos[1] = j;
                  if( var_set( v_A, &variant_L ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           
               }
            }
            break;
         case 3:
            for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
            {
               for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
               {
                  for( k = v_A->LBOUND[2]; k <= v_A->UBOUND[2]; k++ )
                  {
                     VariantType variant_L;
                     VariantType variant_R;
                     
                     v_B->array_pos[0] = i;
                     v_B->array_pos[1] = j;
                     v_B->array_pos[2] = k;
                     if( var_get( v_B, &variant_L ) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }           

                     v_C->array_pos[0] = i;
                     v_C->array_pos[1] = j;
                     v_C->array_pos[2] = k;
                     if( var_get( v_C, &variant_R ) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }           

                     variant_L.Number += variant_R.Number;

                     v_A->array_pos[0] = i;
                     v_A->array_pos[1] = j;
                     v_A->array_pos[2] = k;
                     if( var_set( v_A, &variant_L ) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }           
                  }
               }
            }
            break;
         default:
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }
      else
      if( line_skip_char(l,'-') ) 
      {
         /* MAT A = B - C */
         /* OK */
         int i;
         int j;
         int k;
         VariableType *v_C;
         char varname_C[BasicNameLengthMax + 1];
         
         if( v_A->dimensions != v_B->dimensions )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         /* both arrays are of the same size */
         
         if( line_read_varname(l, varname_C) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }   
         
         if( (v_C = mat_find(varname_C)) == NULL)
         {
            WARN_VARIABLE_NOT_DECLARED;
            return bwb_zline(l);
         }
         /* variable MUST be numeric */
         if ( VAR_IS_STRING( v_C ) )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         if( line_read_matrix_redim( l, v_C ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         if( v_B->dimensions != v_C->dimensions )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         /* both arrays are of the same dimension */
         switch( v_A->dimensions )
         {
         case 1:
            for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
            {
               VariantType variant_L;
               VariantType variant_R;
               
               v_B->array_pos[0] = i;
               if( var_get( v_B, &variant_L ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           

               v_C->array_pos[0] = i;
               if( var_get( v_C, &variant_R ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           

               variant_L.Number -= variant_R.Number;

               v_A->array_pos[0] = i;
               if( var_set( v_A, &variant_L ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           
            }
            break;
         case 2:
            for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
            {
               for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
               {
                  VariantType variant_L;
                  VariantType variant_R;
                  
                  v_B->array_pos[0] = i;
                  v_B->array_pos[1] = j;
                  if( var_get( v_B, &variant_L ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           

                  v_C->array_pos[0] = i;
                  v_C->array_pos[1] = j;
                  if( var_get( v_C, &variant_R ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           

                  variant_L.Number -= variant_R.Number;

                  v_A->array_pos[0] = i;
                  v_A->array_pos[1] = j;
                  if( var_set( v_A, &variant_L ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           
               }
            }
            break;
         case 3:
            for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
            {
               for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
               {
                  for( k = v_A->LBOUND[2]; k <= v_A->UBOUND[2]; k++ )
                  {
                     VariantType variant_L;
                     VariantType variant_R;

                     v_B->array_pos[0] = i;
                     v_B->array_pos[1] = j;
                     v_B->array_pos[2] = k;
                     if( var_get( v_B, &variant_L ) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }           

                     v_C->array_pos[0] = i;
                     v_C->array_pos[1] = j;
                     v_C->array_pos[2] = k;
                     if( var_get( v_C, &variant_R ) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }           

                     variant_L.Number -= variant_R.Number;

                     v_A->array_pos[0] = i;
                     v_A->array_pos[1] = j;
                     v_A->array_pos[2] = k;
                     if( var_set( v_A, &variant_L ) == FALSE )
                     {
                        WARN_VARIABLE_NOT_DECLARED;
                        return bwb_zline(l);
                     }           
                  }
               }
            }
            break;
         default:
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }
      else
      if( line_skip_char(l,'*') ) 
      {
         /* MAT A = B * C */
         int i;
         int j;
         int k;
         VariableType *v_C;
         char varname_C[BasicNameLengthMax + 1];


         if( v_A->dimensions != 2 )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return bwb_zline(l);
         }
         if( v_B->dimensions != 2 )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return bwb_zline(l);
         }
         if( line_read_varname(l, varname_C) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }   
         if( (v_C = mat_find(varname_C)) == NULL)
         {
            WARN_VARIABLE_NOT_DECLARED;
            return bwb_zline(l);
         }
         /* variable MUST be numeric */
         if ( VAR_IS_STRING( v_C ) ) 
         {
            WARN_TYPE_MISMATCH;
            return bwb_zline(l);
         }
         if( line_read_matrix_redim( l, v_C ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         if( v_C->dimensions != 2 )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return bwb_zline(l);
         }
         if( v_A->LBOUND[0] != v_B->LBOUND[0] )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return bwb_zline(l);
         }
         if( v_A->UBOUND[0] != v_B->UBOUND[0] )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return bwb_zline(l);
         }
         if( v_A->LBOUND[1] != v_C->LBOUND[1] )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return bwb_zline(l);
         }
         if( v_A->UBOUND[1] != v_C->UBOUND[1] )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return bwb_zline(l);
         }
         if( v_B->LBOUND[1] != v_C->LBOUND[0] )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return bwb_zline(l);
         }
         if( v_B->UBOUND[1] != v_C->UBOUND[0] )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return bwb_zline(l);
         }
         for( i = v_A->LBOUND[0]; i <= v_A->UBOUND[0]; i++ )
         {
            for( j = v_A->LBOUND[1]; j <= v_A->UBOUND[1]; j++ )
            {
               VariantType variant_A;
               
               variant_A.TypeChar = v_A->VariableTypeChar;
               variant_A.Number = 0;

               v_A->array_pos[0] = i;
               v_A->array_pos[1] = j;
               if( var_set( v_A, &variant_A ) == FALSE )
               {
                  WARN_VARIABLE_NOT_DECLARED;
                  return bwb_zline(l);
               }           
               for( k = v_C->LBOUND[0]; k <= v_C->UBOUND[0]; k++ )
               {
                  VariantType variant_B;
                  VariantType variant_C;
                  
                  v_A->array_pos[0] = i;
                  v_A->array_pos[1] = j;
                  if( var_get( v_A, &variant_A ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           

                  v_B->array_pos[0] = i;
                  v_B->array_pos[1] = k;
                  if( var_get( v_B, &variant_B ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           

                  v_C->array_pos[0] = k;
                  v_C->array_pos[1] = j;
                  if( var_get( v_C, &variant_C ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           

                  variant_A.Number += variant_B.Number * variant_C.Number;

                  v_A->array_pos[0] = i;
                  v_A->array_pos[1] = j;
                  if( var_set( v_A, &variant_A ) == FALSE )
                  {
                     WARN_VARIABLE_NOT_DECLARED;
                     return bwb_zline(l);
                  }           
               }
            }
         }
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }
   return bwb_zline(l);
}



/* 
--------------------------------------------------------------------------------------------
                               STORE
--------------------------------------------------------------------------------------------
*/


LineType *
bwb_STORE(LineType * l)
{
   /*
   SYNTAX:     STORE NumericArrayName
   */
   bwx_DEBUG(__FUNCTION__);
   return bwb_CSAVE8(l);
}


/* 
--------------------------------------------------------------------------------------------
                               CSAVE*
--------------------------------------------------------------------------------------------
*/

#define CSAVE_VERSION_1 0x20150218L

LineType *
bwb_CSAVE8(LineType * l)
{
   /*
   SYNTAX:     CSAVE* NumericArrayName
   */
   VariableType *v = NULL;
   FILE           *f;
   unsigned long   n;
   size_t          t;
   char varname[BasicNameLengthMax + 1];


   bwx_DEBUG(__FUNCTION__);

   if( line_read_varname(l, varname) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }   
   
   v = mat_find(varname);
   if( v == NULL)
   {
      WARN_VARIABLE_NOT_DECLARED;
      return bwb_zline(l);
   }
   /* variable MUST be numeric */
   if ( VAR_IS_STRING( v ) )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* variable MUST be an array */
   if (v->dimensions == 0)
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_matrix_redim( l, v ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* variable storage is a mess, we bypass that tradition here. */
   t = v->array_units;
   if (t <= 1)
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* open file */
   f = fopen(v->name, "w");
   if (f == NULL)
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* write version number */
   n = CSAVE_VERSION_1;
   fwrite(&n, sizeof(long), 1, f);
   /* write total number of elements */
   fwrite(&t, sizeof(long), 1, f);
   /* write data */
   fwrite(v->memnum, sizeof(BasicNumberType), t, f);
   /* OK */
   fclose(f); /* f != NULL */
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               RECALL
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_RECALL(LineType * l)
{
   /*
   SYNTAX:     RECALL NumericArrayName
   */
   bwx_DEBUG(__FUNCTION__);
   return bwb_CLOAD8(l);
}

/* 
--------------------------------------------------------------------------------------------
                               CLOAD*
--------------------------------------------------------------------------------------------
*/


LineType *
bwb_CLOAD8(LineType * l)
{
   /*
   SYNTAX:     CLOAD* NumericArrayName
   */
   VariableType *v = NULL;
   FILE           *f;
   unsigned long   n;
   size_t          t;
   char varname[BasicNameLengthMax + 1];


   bwx_DEBUG(__FUNCTION__);

   if( line_read_varname(l, varname) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }   
   
   v = mat_find(varname);
   if( v == NULL)
   {
      WARN_VARIABLE_NOT_DECLARED;
      return bwb_zline(l);
   }

   /* variable MUST be numeric */
   if ( VAR_IS_STRING( v ) )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* variable MUST be an array */
   if (v->dimensions == 0)
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_matrix_redim( l, v ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* variable storage is a mess, we bypass that tradition here. */
   t = v->array_units;
   if (t <= 1)
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* open file */
   f = fopen(v->name, "r");
   if (f == NULL)
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   /* read version number */
   n = 0;
   fread(&n, sizeof(long), 1, f);
   if (n != CSAVE_VERSION_1)
   {
      fclose(f); /* f != NULL */
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   /* read total number of elements */
   n = 0;
   fread(&n, sizeof(long), 1, f);
   if (n != t)
   {
      fclose(f); /* f != NULL */
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   /* read data */
   fread(v->memnum, sizeof(BasicNumberType), t, f);
   /* OK */
   fclose(f); /* f != NULL */
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               CSAVE
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_CSAVE(LineType * l)
{
   /*
   SYNTAX:     CSAVE [filename$]
   */
   bwx_DEBUG(__FUNCTION__);
   return bwb_SAVE(l);
}

/* 
--------------------------------------------------------------------------------------------
                               TSAVE
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_TSAVE(LineType * l)
{
   /*
   SYNTAX:     TSAVE [filename$]
   */
   bwx_DEBUG(__FUNCTION__);
   return bwb_SAVE(l);
}

/* 
--------------------------------------------------------------------------------------------
                               CLOAD
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_CLOAD(LineType * l)
{
   /*
   SYNTAX:     CLOAD [filename$]
   */
   bwx_DEBUG(__FUNCTION__);
   return bwb_LOAD(l);
}

/* 
--------------------------------------------------------------------------------------------
                               TLOAD
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_TLOAD(LineType * l)
{
   /*
   SYNTAX:     TLOAD [filename$]
   */
   bwx_DEBUG(__FUNCTION__);
   return bwb_LOAD(l);
}


/* 
--------------------------------------------------------------------------------------------
                               SAVE
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_SAVE(LineType * l)
{
   /*
   SYNTAX:     SAVE [filename$]
   */
   FILE           *outfile;
   bwx_DEBUG(__FUNCTION__);


   /* Get an argument for filename */

   if (line_is_eol(l))
   {
      /* default is the last filename used by LOAD or SAVE */
   }
   else
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
      bwb_strcpy( My->progfile, Value );
      free( Value );
   }
   if( is_empty_filename( My->progfile ) )
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   if ((outfile = fopen(My->progfile, "w")) == NULL)
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   bwb_xlist(l, outfile);
   fclose(outfile); /* outfile != NULL */


   return bwb_zline(l);

}

/* 
--------------------------------------------------------------------------------------------
                               LIST
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_LIST(LineType * l)
{
   /*
   SYNTAX:     LIST
   SYNTAX:     LIST line [,...]
   SYNTAX:     LIST line - line
   */
   bwx_DEBUG(__FUNCTION__);
   
   
   My->OptionIndentValue = 2;

   bwb_xlist(l, My->SYSOUT->cfp);
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               LLIST
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_LLIST(LineType * l)
{
   /*
   SYNTAX:     LLIST
   SYNTAX:     LLIST line [,...]
   SYNTAX:     LLIST line - line
   */
   bwx_DEBUG(__FUNCTION__);

   bwb_xlist(l, My->SYSPRN->cfp);
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_xlist()
  
        DESCRIPTION: This C function lists the program in
         memory to a specified output device.
  
***************************************************************/

LineType *
bwb_xlist(LineType * l, FILE * file)
{
   bwx_DEBUG(__FUNCTION__);

   if( line_is_eol( l ) )
   {
      /* LIST */
      LineType *x;
      /* now go through and list appropriate lines */
      bwb_scan();
      My->LastLineNumber = -1;
      for (x = My->bwb_start.next; x != &My->bwb_end; x = x->next)
      {
         xl_line(file, x);
      }
      fprintf(file, "\n");
   }
   else
   {
      do
      {
         int head;
         int tail;
         
         if( line_read_line_sequence(l, &head, &tail) )
         {
            /* LIST 's' - 'e' */
            LineType *x;
            if( head < BasicLineNumberMin || head > BasicLineNumberMax )
            {
               WARN_UNDEFINED_LINE;
               return bwb_zline(l);
            }
            if( tail < BasicLineNumberMin || tail > BasicLineNumberMax )
            {
               WARN_UNDEFINED_LINE;
               return bwb_zline(l);
            }
            if( head > tail )
            {
               WARN_UNDEFINED_LINE;
               return bwb_zline(l);
            }
            /* valid range */
            /* now go through and list appropriate lines */
            bwb_scan();
            My->LastLineNumber = -1;
            for (x = My->bwb_start.next; x != &My->bwb_end; x = x->next)
            {
               if ( head <= x->number && x->number <= tail)
               {
                  xl_line(file, x);
               }
            }
            fprintf(file, "\n");
         }
         else
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }
      while( line_skip_comma( l ) );
   }
   return bwb_zline(l);
}

/***************************************************************
  
   FUNCTION:       xl_line()
  
   DESCRIPTION:    This function lists a single program
         line to a specified device of file.
         It is called by bwb_xlist() and bwb_SAVE().
  
***************************************************************/

static int xl_line(FILE * file, LineType * l)
{
   char LineExecuted;
   char *C; /* start of comment text */
   char buffer[32]; /* 0...99999 */

   bwx_DEBUG(__FUNCTION__);
   
   /*
   ** The only difference between LIST, LLIST and SAVE is:
   ** LIST and LLIST display an '*' 
   ** when a line has been executed
   ** and OPTION COVERAGE ON is enabled.
   */

   LineExecuted = ' ';
   if (My->CurrentVersion->OptionFlags & (OPTION_COVERAGE_ON) )
   {
      if( l->LineFlags & LINE_EXECUTED )
      {
         if( file == My->SYSOUT->cfp || file == My->SYSPRN->cfp )
         {
            /* LIST */
            /* LLIST */
            LineExecuted = '*';
         }
         else
         {
            /* SAVE */
            /* EDIT implies SAVE */
         }
      }
   }

   C = l->buffer;
   if( l->LineFlags & LINE_NUMBERED )
   {
       /* explicitly numbered */
       sprintf( buffer, "%*d", BasicLineNumberDigits , l->number);
       /* ##### xxx */
   }
   else
   {
      /* implicitly numbered */
      if (My->LastLineNumber == l->number )
      {
          /* multi-statement line */
          if( l->cmdnum == C_REM && IS_CHAR( l->buffer[0], My->CurrentVersion->OptionCommentChar) )
          {
             /* trailing comment */
             sprintf( buffer, "%*s%c", BasicLineNumberDigits - 1, "", My->CurrentVersion->OptionCommentChar );
             C++; /* skip comment char */
             while( *C == ' ' )
             {
                /* skip spaces */
                C++;
             }
             /* ____' xxx */
          }
          else
          if( My->CurrentVersion->OptionStatementChar != BasicNulChar )
          {
             /* all other commands, add a colon */
             sprintf( buffer, "%*s%c", BasicLineNumberDigits - 1, "", My->CurrentVersion->OptionStatementChar );
             /* ____: xxx */
          }
          else
          {
             /*
             The user is trying to list a multi-line statement 
             in a dialect that does NOT support multi-line statements.  
             This could occur when LOADing in one dialect and then SAVEing as another dialect, such as:
                OPTION VERSION BASIC-80
                LOAD "TEST1.BAS"
                   100 REM TEST
                   110 PRINT:PRINT:PRINT
                OPTION VERSION MARK-I
                EDIT
                   100 REM TEST
                   110 PRINT
                       PRINT
                       PRINT
             The only thing we can reasonably do is put spaces for the line number,
             since the user will have to edit the results manually anyways.
             */
             sprintf( buffer, "%*s", BasicLineNumberDigits , "" );
             /* _____ xxx */
          }
      }
      else
      {
         /* single-statement line */
         sprintf( buffer, "%*s", BasicLineNumberDigits, "");
         /* _____ xxx */
      }
   }
   
   fprintf(file, "%s", buffer);
   fprintf(file, "%c", LineExecuted);
   
   /* if( TRUE ) */
   {
      /* %INCLUDE */
      int i;
      for( i = 0; i < l->IncludeLevel; i++ )
      {
            fputc( ' ', file );
      }
   }
   if( My->OptionIndentValue > 0 )
   {
      int i;
      
      for( i = 0; i < l->Indention; i++ )
      {
         int j;
         for( j = 0; j < My->OptionIndentValue; j++ )
         {
            fputc( ' ', file );
         }
      }
   }
   fprintf(file, "%s\n", C);
   
   My->LastLineNumber = l->number;

   return TRUE;
}

/* 
--------------------------------------------------------------------------------------------
                               DELETE
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_DELETE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   if( line_is_eol( l ) )
   {
      /* DELETE */
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   else
   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      /*
      SYNTAX:     DELETE filenum [,...]
      */
      do
      {
         int FileNumber = 0;
         if( line_read_integer_expression( l, &FileNumber ) == FALSE )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if( FileNumber <= 0 )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }

         My->CurrentFile = find_file_by_number( FileNumber );
         if( My->CurrentFile == NULL )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if( My->CurrentFile->mode == DEVMODE_CLOSED )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if( My->CurrentFile->cfp != NULL )
         {
            fclose( My->CurrentFile->cfp ); /* My->CurrentFile->cfp != NULL */
            My->CurrentFile->cfp = NULL;
         }
         if( My->CurrentFile->buffer != NULL )
         {
            FREE( My->CurrentFile->buffer, "bwb_DELETE" );
            My->CurrentFile->buffer = NULL;
         }
         My->CurrentFile->width = 0;
         My->CurrentFile->col = 1;
         My->CurrentFile->row = 1;
         My->CurrentFile->delimit = ',';
         My->CurrentFile->mode = DEVMODE_CLOSED;
         if( My->CurrentFile->filename[ 0 ] == BasicNulChar )
         {
            WARN_BAD_FILE_NAME;
            return bwb_zline(l);
         }
         remove( My->CurrentFile->filename );
         My->CurrentFile->filename[ 0 ] = BasicNulChar;
      }
      while( line_skip_comma( l ) );
      /* OK */      
      return bwb_zline(l);
   }
   else
   {
      /*
      SYNTAX:     DELETE line [,...]
      SYNTAX:     DELETE line - line
      */
      do
      {
         int head;
         int tail;
         
         if( line_read_line_sequence(l, &head, &tail) )
         {
            /* DELETE 's' - 'e' */
            LineType *x;
            LineType *previous;
            if( head < BasicLineNumberMin || head > BasicLineNumberMax )
            {
               WARN_UNDEFINED_LINE;
               return bwb_zline(l);
            }
            if( tail < BasicLineNumberMin || tail > BasicLineNumberMax )
            {
               WARN_UNDEFINED_LINE;
               return bwb_zline(l);
            }
            if( head > tail )
            {
               WARN_UNDEFINED_LINE;
               return bwb_zline(l);
            }
            /* valid range */
            /* now go through and list appropriate lines */
            previous = &My->bwb_start;
            for (x = My->bwb_start.next; x != &My->bwb_end; )
            {
               LineType *next;


               next = x->next;
               if ( x->number < head )
               {
                  previous = x;
               }
               else
               if ( head <= x->number && x->number <= tail)
               {
                  if (x == l)
                  {
                     /* 100 DELETE 100 */
                     WARN_SYNTAX_ERROR;
                     return bwb_zline(l);
                  }
                  bwb_freeline(x);
                  previous->next = next;
               }
               x = next;
            }
         }
         else
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }
      while( line_skip_comma( l ) );
   }
   return bwb_zline(l);
}

#if FALSE /* keep the source to DONUM and DOUNNUM */

/* 
--------------------------------------------------------------------------------------------
                               DONUM
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_donum(LineType * l)
{
   /*
   SYNTAX:     DONUM
   */
   LineType *current;
   register int    lnumber;

   bwx_DEBUG(__FUNCTION__);

   lnumber = 10;
   for (current = My->bwb_start.next; current != &My->bwb_end; current = current->next)
   {
      current->number = lnumber;

      lnumber += 10;
      if (lnumber > BasicLineNumberMax)
      {
         return bwb_zline(l);
      }
   }

   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               DOUNUM
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_dounnum(LineType * l)
{
   /*
   SYNTAX:     DOUNNUM
   */
   LineType *current;

   bwx_DEBUG(__FUNCTION__);

   for (current = My->bwb_start.next; current != &My->bwb_end; current = current->next)
   {
      current->number = 0;
   }

   return bwb_zline(l);
}

#endif            /* FALSE */



/* 
--------------------------------------------------------------------------------------------
                               FILES
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_FILES(LineType * l)
{
   /*
   SYNTAX:     FILES A$ [, ...]
   */
   /* open a list of files in READ mode */
   
   do
   {
      int FileNumber;

      FileNumber = My->LastFileNumber;
      FileNumber++;
      if( FileNumber < 0 )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( FileNumber == 0 )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         My->CurrentFile = file_new();
         My->CurrentFile->FileNumber = FileNumber;
      }
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
         bwb_strcpy( My->CurrentFile->filename, Value );
         free( Value );
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
      My->LastFileNumber = FileNumber;
      /* OK */
   }
   while( line_skip_comma( l ) );
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               FILE
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_FILE(LineType * l)
{
   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      /* 
      CBASIC-II: 
      FILE file_name$                      ' filename$ must be a simple string scalar (no arrays)
      FILE file_name$ ( record_length% )   ' filename$ must be a simple string scalar (no arrays)
      -- if the file exists, 
         then it is used, 
         else it is created.
      -- Does not trigger IF END #
      */
      do
      {
         int FileNumber;
         VariableType * v;
         char varname[BasicNameLengthMax + 1];
   
         if( line_read_varname( l, varname ) == FALSE )
         {
            WARN_BAD_FILE_NAME;
            return bwb_zline(l);
         }
         if( varname[0] == BasicNulChar )
         {
            WARN_BAD_FILE_NAME;
            return bwb_zline(l);
         }
         v = find_variable_by_type( varname, 0, BasicStringSuffix );
         if( v == NULL )
         {
            WARN_VARIABLE_NOT_DECLARED;
            return bwb_zline(l);
         }
         if( VAR_IS_STRING( v ) )
         {
            /* OK */
         }
         else
         {
            WARN_TYPE_MISMATCH;
            return bwb_zline(l);
         }
   
         FileNumber = My->LastFileNumber;
         FileNumber++;
         if( FileNumber < 0 )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         if( FileNumber == 0 )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         My->CurrentFile = find_file_by_number( FileNumber );
         if( My->CurrentFile == NULL )
         {
            My->CurrentFile = file_new();
            My->CurrentFile->FileNumber = FileNumber;
         }
         if( My->CurrentFile->mode != DEVMODE_CLOSED )
         {
            if( My->CurrentFile->cfp != NULL )
            {
               fclose( My->CurrentFile->cfp ); /* My->CurrentFile->cfp != NULL */
            }
            if( My->CurrentFile->buffer != NULL )
            {
               FREE( My->CurrentFile->buffer, "bwb_FILE" );
            }
         }
         My->CurrentFile->width = 0;
         My->CurrentFile->col = 1;
         My->CurrentFile->row = 1;
         My->CurrentFile->delimit = ',';
         My->CurrentFile->buffer = NULL;
         My->CurrentFile->mode = DEVMODE_CLOSED;
         /* OK */
         if( line_skip_char( l, '(' ) )
         {
            /* RANDOM file */
            int RecLen;
            
            if( line_read_integer_expression( l, &RecLen ) == FALSE )
            {
               WARN_FIELD_OVERFLOW;
               return bwb_zline(l);
            }
            if( RecLen <= 0 )
            {
               WARN_FIELD_OVERFLOW;
               return bwb_zline(l);
            }
            if( line_skip_char( l, ')' ) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline(l);
            }
            My->CurrentFile->buffer = CALLOC(RecLen + 1 /* NUL */, 1, "bwb_FILE");
            My->CurrentFile->width = RecLen;
         }

         /* if( TRUE ) */
         {
            VariantType variant;
            if( var_get( v, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return bwb_zline(l);
            }
            if( variant.TypeChar == '$' )
            {
               bwb_strcpy( My->CurrentFile->filename, variant.Buffer );
               RELEASE( (&variant) );
            }
            else
            {
               WARN_TYPE_MISMATCH;
               return bwb_zline(l);
            }
         }
         if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
         {
            strupper( My->CurrentFile->filename ); /* CBASIC-II: file names are always upper case */
         }
         My->CurrentFile->cfp = fopen(My->CurrentFile->filename, "r+");
         if (My->CurrentFile->cfp == NULL)
         {
            My->CurrentFile->cfp = fopen(My->CurrentFile->filename, "w");
            if( My->CurrentFile->cfp != NULL )
            {
               fclose(My->CurrentFile->cfp); /* fp != NULL */
               My->CurrentFile->cfp = fopen(My->CurrentFile->filename, "r+");
            }
         }
         if (My->CurrentFile->cfp == NULL)
         {
            WARN_BAD_FILE_NAME;
            return bwb_zline(l);
         }
         if( My->CurrentFile->width > 0 )
         {
            /* RANDOM file */
            My->CurrentFile->mode = DEVMODE_RANDOM;
         }
         else
         {
            /* SERIAL file */
            My->CurrentFile->mode = DEVMODE_INPUT | DEVMODE_OUTPUT;
         }
         /* OK */
         My->LastFileNumber = FileNumber;
      }
      while( line_skip_comma( l ) );
      /* OK */
      return bwb_zline(l);
   }
   if( line_skip_char(l,BasicFileNumberPrefix) )
   {
      /*
      SYNTAX:     FILE # X, A$
      */
      int FileNumber;

      if( line_read_integer_expression( l, &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( line_skip_comma(l) )
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
         /* "FILE # -1" is an ERROR */
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( FileNumber == 0 )
      {
         /* "FILE # 0" is an ERROR */
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         My->CurrentFile = file_new();
         My->CurrentFile->FileNumber = FileNumber;
      }
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
         bwb_strcpy( My->CurrentFile->filename, Value );
         free( Value );
      }
      if( My->CurrentFile->mode != DEVMODE_CLOSED )
      {
         if( My->CurrentFile->cfp != NULL )
         {
            fclose( My->CurrentFile->cfp ); /* My->CurrentFile->cfp != NULL */
         }
         if( My->CurrentFile->buffer != NULL )
         {
            FREE( My->CurrentFile->buffer, "bwb_FILE" );
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
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               DELIMIT
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_DELIMIT(LineType * l)
{
   /*
   SYNTAX:     DELIMIT # X, A$
   */
   if( line_skip_char(l,BasicFileNumberPrefix) )
   {
      /* DELIMIT # */
      int FileNumber;
      char delimit;

      My->CurrentFile = My->SYSIN;

      if( line_read_integer_expression( l, &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( line_skip_comma(l) )
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
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
         delimit = Value[ 0 ];
         free( Value );
      }
      if( FileNumber < 0 )
      {
         /* "DELIMIT # -1" is the printer */
         My->SYSPRN->delimit = delimit;
         return bwb_zline(l);
      }
      if( FileNumber == 0 )
      {
         /* "DELIMIT # 0" is the console */
         My->SYSIN->delimit = delimit;
         My->SYSOUT->delimit = delimit;
         return bwb_zline(l);
      }
      /* normal file */
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      My->CurrentFile->delimit = delimit;
      /* OK */
      return bwb_zline(l);
   }
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               MARGIN
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_MARGIN(LineType * l)
{
   /*
   SYNTAX:     MARGIN # X, Y
   */
   /* set width for OUTPUT  */
   int FileNumber;
   int Value;
   if( line_skip_char(l,BasicFileNumberPrefix) )
   {
      /* MARGIN # */
      My->CurrentFile = My->SYSIN;

      if( line_read_integer_expression( l, &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( line_skip_comma(l) )
      {
         /* OK */
      }
      else
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_read_integer_expression( l, &Value ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( FileNumber < 0 )
      {
         /* "MARGIN # -1" is the printer */
         My->SYSPRN->width = Value;
         return bwb_zline(l);
      }
      if( FileNumber == 0 )
      {
         /* "MARGIN # 0" is the console */
         My->SYSIN->width = Value;
         My->SYSOUT->width = Value;
         return bwb_zline(l);
      }
      /* normal file */
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      My->CurrentFile->width = Value;
      /* OK */
      return bwb_zline(l);
   }
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               USE
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_USE(LineType * l)
{
   /*
   SYNTAX:     USE parameter$ ' System/370
   */
   VariableType * v;
   
   if( (v = line_read_scalar( l )) == NULL )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( v->VariableTypeChar != BasicStringSuffix )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* OK */
   if( My->UseParameter != NULL )
   {
      VariantType variant;

      variant.TypeChar = BasicStringSuffix;
      variant.Buffer = My->UseParameter;
      variant.Length = strlen( My->UseParameter );
      var_set( v, &variant );
   }
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               CHAIN
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_CHAIN(LineType * l)
{
   /*
   SYNTAX:     CHAIN file-name$ [, linenumber]
   SYNTAX:     CHAIN file-name$ [, parameter$] ' System/370
   */
   /* based upon bwb_xload() */
   int LineNumber;
   LineType * x;

   bwx_DEBUG(__FUNCTION__);

   /* Get an argument for filename */
   if (line_is_eol(l))
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   else
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
      bwb_strcpy( My->progfile, Value );
      free( Value );
   }
   if (My->progfile[0] == BasicNulChar)
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      strupper( My->progfile ); /* CBASIC-II: file names are always upper case */
      if( bwb_strchr( My->progfile, '.' ) == NULL )
      {
         /* The ".BAS" file extension is implied */
         bwb_strcat( My->progfile, ".BAS" );
      }
   }
   /* optional linenumber */
   LineNumber = 0;
   if( line_skip_comma(l) )
   {
      if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
      {
         /* CHAIN filename$, parameter$ */
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
            if( My->UseParameter != NULL )
            {
               free( My->UseParameter );
            }
            My->UseParameter = Value;
         }
      }
      else
      {
         /* CHAIN filename$, linenumber */
         if( line_read_integer_expression( l, &LineNumber ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
         if( LineNumber < BasicLineNumberMin || LineNumber > BasicLineNumberMax )
         {
            WARN_UNDEFINED_LINE;
            return bwb_zline(l);
         }
      }
   }

   /* deallocate all variables except common ones */
   var_delcvars();

   /* remove old program from memory */
   bwb_xnew(&My->bwb_start);

   /* load new program in memory */
   if( bwb_fload( NULL /* My->progfile */ ) == FALSE )
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   x = &My->bwb_start;
   if( LineNumber > 0 )
   {
      /* search for a matching line number */
      while(x->number != LineNumber && x!= &My->bwb_end)
      {
         x=x->next;
      }
      if( x == &My->bwb_end )
      {
         /* NOT FOUND */
         x = &My->bwb_start;
      }
   }

   if( bwb_scan() == FALSE )
   {
      WARN_CANT_CONTINUE;
      return bwb_zline(l);
   }

   /* reset all stack counters */
   bwb_clrexec();
   if( bwb_incexec() )
   {
      /* OK */
   }
   else
   {
      /* ERROR */
      WARN_OUT_OF_MEMORY;
      return &My->bwb_end;
   }
   
   /* run the program */

   /* CHAIN */
   bwb_Warning_Clear();
   My->ContinueLine = NULL;
   SetOnError(0);
   x->position = 0;
   My->stack_head->line = x;
   My->stack_head->code = EXEC_NORM;
   return x;
}

/***************************************************************
  
        FUNCTION:       bwb_merge()
  
        DESCRIPTION: This C function implements the BASIC
         MERGE command, merging command lines from
         a specified file into the program in memory
         without deleting the lines already in memory.
  
      SYNTAX:     MERGE file-name
  
***************************************************************/
LineType *
bwb_APPEND(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   if ( line_skip_char( l, BasicFileNumberPrefix ) )
   {
      /* APPEND # filenumber */
      int             FileNumber;

      if( line_read_integer_expression(l, &FileNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( FileNumber < 0 )
      {
         /* "APPEND # -1" is silently ignored */
         return bwb_zline(l);
      }
      if( FileNumber == 0 )
      {
         /* "APPEND # 0" is silently ignored */
         return bwb_zline(l);
      }
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      /* normal file */
      fseek( My->CurrentFile->cfp, 0, SEEK_END );
      My->CurrentFile->mode = DEVMODE_APPEND;
      /* OK */
      return bwb_zline(l);
   }
   /* APPEND filename$ */
   return bwb_MERGE(l);
}

LineType *
bwb_MERGE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* call xload function to merge program in memory */
   bwb_xload(l);
   return bwb_zline(l);
}



void
SetOnError(int LineNumber)
{
   /* scan the stack looking for a FUNCTION/SUB */
   StackType * stack_item;

   bwx_DEBUG(__FUNCTION__);
   
   if( My->stack_head == NULL )
   {
      bwx_DEBUG( "My->stack_head == NULL" );
      return;
   }

   for( stack_item = My->stack_head; stack_item->next != NULL; stack_item = stack_item->next )
   {
      LineType *current;

      current = stack_item->LoopTopLine;
      if (current != NULL)
      {
         switch (current->cmdnum)
         {
         case C_FUNCTION:
         case C_SUB:
            /* FOUND */
            /* we are in a FUNCTION/SUB, so this is LOCAL */
            stack_item->OnErrorGoto = LineNumber;
            return;
            /* break; */
         }
      }
   }
   /* stack_item->next == NULL */
   /* NOT FOUND */
   /* we are NOT in a FUNCTION/SUB  */
   stack_item->OnErrorGoto = LineNumber;
}


int
GetOnError(void)
{
   /* scan the stack looking for an active "ON ERROR GOTO linenumber" */
   StackType * stack_item;

   bwx_DEBUG(__FUNCTION__);

   for ( stack_item = My->stack_head; stack_item != NULL; stack_item = stack_item->next )
   {
      if( stack_item->OnErrorGoto != 0 )
      {
         /* FOUND */
         return stack_item->OnErrorGoto;
      }
   }
   /* NOT FOUND */
   return 0;
}


/***************************************************************
  
        FUNCTION:       bwb_onerror()
  
        DESCRIPTION: This C function implements the BASIC
            ON ERROR GOSUB command.
  
      SYNTAX:     ON ERROR GOSUB line | label
  
***************************************************************/
LineType *
bwb_ON_ERROR(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_ON_ERROR_GOTO(LineType * l)
{
   /* ON ERROR GOTO line */
   int LineNumber;

   /* get the line number */
   LineNumber = 0;
   if( line_is_eol( l ) )
   {
      /* ON ERROR GOTO */
      SetOnError(0);
      return bwb_zline(l);
   }
   if( line_read_integer_expression( l, &LineNumber ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* ON ERORR GOTO linenumber */
   if( LineNumber == 0 )
   {
      /* ON ERROR GOTO 0 */
      SetOnError(0);
      return bwb_zline(l);
   }
   if( LineNumber < BasicLineNumberMin || LineNumber > BasicLineNumberMax )
   {
      /* ERROR */
      WARN_UNDEFINED_LINE;
      return bwb_zline(l);
   }
   /* OK */
   SetOnError(LineNumber);
   return bwb_zline(l);
}

LineType *
bwb_ON_ERROR_GOSUB(LineType * l)
{
   /* ON ERROR GOSUB line */
   return bwb_ON_ERROR_GOTO(l);
}

LineType *
bwb_ON_ERROR_RESUME(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_ON_ERROR_RESUME_NEXT(LineType * l)
{
   SetOnError(-1);
   return bwb_zline(l);
}

LineType *
bwb_ON_ERROR_RETURN(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_ON_ERROR_RETURN_NEXT(LineType * l)
{
   return bwb_ON_ERROR_RESUME_NEXT(l);
}

LineType *
bwb_ON_TIMER(LineType * l)
{
   /* ON TIMER(...) GOSUB ... */
   BasicNumberType v;
   BasicNumberType minv;
   int LineNumber;


   bwx_DEBUG(__FUNCTION__);

   /* turn off the TIMER - so syntax errors will NOT fire the timer */
   bwb_Timer_Off();
   My->tmr_line = 0;
   My->tmr_count = 0;


   /* get the SECOMDS parameter */
   if( line_read_numeric_expression( l, &v )  == FALSE)
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   
   minv = 1;
   minv /= CLOCKS_PER_SEC;

   if (v < minv)
   {
      /* ERROR */
      WARN_ILLEGAL_FUNCTION_CALL;
      return bwb_zline(l);
   }
      
   /* get the GOSUB keyword */
   if( line_skip_word( l, "GOSUB" ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* ON TIMER(X) GOSUB line */
   if( line_read_integer_expression( l, &LineNumber ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( LineNumber < BasicLineNumberMin || LineNumber > BasicLineNumberMax )
   {
      /* ERROR */
      WARN_UNDEFINED_LINE;
      return bwb_zline(l);
   }
   /* OK */
   My->tmr_line = LineNumber;
   My->tmr_count = v;
   return bwb_zline(l);
}


LineType *
bwb_TIMER(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* turn off the TIMER - so syntax errors will NOT fire the timer */
   bwb_Timer_Off();

   WARN_SYNTAX_ERROR;

   return bwb_zline(l);
}

LineType *
bwb_TIMER_OFF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* turn off the TIMER - so syntax errors will NOT fire the timer */
   bwb_Timer_Off();

   /* TIMER OFF */
   My->tmr_line = 0;
   My->tmr_count = 0;

   return bwb_zline(l);
}

LineType *
bwb_TIMER_ON(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* turn off the TIMER - so syntax errors will NOT fire the timer */
   bwb_Timer_Off();

   /* TIMER ON */
   if (My->tmr_count > 0 && My->tmr_line > 0)
   {
      My->tmr_expires = bwx_TIMER(My->tmr_count);
      bwb_Timer_On();
   }

   return bwb_zline(l);
}

LineType *
bwb_TIMER_STOP(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* turn off the TIMER - so syntax errors will NOT fire the timer */
   bwb_Timer_Off();

   /* TIMER STOP */

   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_resume()
  
        DESCRIPTION: This C function implements the BASIC
            RESUME command.
  
      SYNTAX:     RESUME [ 0 | NEXT | line | label ]
  
***************************************************************/
LineType *
bwb_RESUME(LineType * l)
{
   int LineNumber = 0;
   LineType *x = NULL;

   bwx_DEBUG(__FUNCTION__);

   if (My->err_number == 0) /* RESUME without ERR are FATAL */
   {
      WARN_RESUME_WITHOUT_ERROR;
      return bwb_zline(l);
   }
   if (My->err_line == NULL)
   {
      /* RESUME without ERL are FATAL */
      WARN_RESUME_WITHOUT_ERROR;
      return bwb_zline(l);
   }
   if (My->err_line->number == 0)
   {
      /* RESUME without ERL are FATAL */
      WARN_RESUME_WITHOUT_ERROR;
      return bwb_zline(l);
   }
   /* Get optional argument for RESUME */


   if( line_is_eol(l) )
   {
      /* RESUME */
      /* 
      Execution resumes at the statement which caused the error 
      For structured commands, this is the top line of the structure.
      */
      x = My->err_line;
      bwb_Warning_Clear();
      x->position = 0;
      return x;
   }
   if( line_skip_word(l, "NEXT") )
   {
      /* RESUME NEXT */
      /* 
      Execution resumes at the statement immediately following the one which caused the error. 
      For structured commands, this is the bottom line of the structure.
      */
      switch( My->err_line->cmdnum )
      {
      case C_IF_THEN:
         /* skip to END_IF */
         for( x = My->err_line->OtherLine; x->cmdnum != C_END_IF; x = x->OtherLine );
         break;
      case C_SELECT_CASE:
         /* skip to END_SELECT */
         for( x = My->err_line->OtherLine; x->cmdnum != C_END_SELECT; x = x->OtherLine );
         break;
      default:
         x = My->err_line->next;
      }
      bwb_Warning_Clear();
      x->position = 0;
      return x;
   }
   /* RESUME ### */
   if( line_read_integer_expression( l, &LineNumber ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   if( LineNumber == 0 )
   {
      /* SPECIAL CASE */
      /* RESUME 0 */
      /* Execution resumes at the statement which caused the error */
      x = My->err_line;
      bwb_Warning_Clear();
      x->position = 0;
      return x;
   }
   if( LineNumber < BasicLineNumberMin || LineNumber > BasicLineNumberMax )
   {
      /* ERROR */
      WARN_UNDEFINED_LINE;
      return bwb_zline(l);
   }
   /* VERIFY LINE EXISTS */
   x = find_line_number( LineNumber, TRUE );
   if( x != NULL )
   {
      /* FOUND */
      bwb_Warning_Clear();
      x->position = 0;
      return x;
   }
   /* NOT FOUND */
   WARN_UNDEFINED_LINE;
   return bwb_zline(l);
}





/***************************************************************
  
        FUNCTION:       bwb_xnew()
  
        DESCRIPTION: Clears the program in memory, but does not
         deallocate all variables.
  
***************************************************************/

void
bwb_xnew(LineType * l)
{
   LineType *current, *previous;
   int             wait;


   bwx_DEBUG(__FUNCTION__);

   previous = NULL;  /* JBV */
   wait = TRUE;
   for (current = l->next; current != &My->bwb_end; current = current->next)
   {
      if (wait != TRUE)
      {
         /* Revised to FREE pass-thru call by JBV */
         FREE(previous, "bwb_xnew");
      }
      wait = FALSE;
      previous = current;
   }

   l->next = &My->bwb_end;

}


/***************************************************************
  
        FUNCTION:       bwb_cmds()
  
   DESCRIPTION:    This function implements a CMD command,
         which lists all commands implemented.
         It is not part of a BASIC specification,
         but is used for debugging bwBASIC.
  
   SYNTAX:     CMDS
  
***************************************************************/


LineType *
bwb_CMDS(LineType * l)
{
   register int    n;
   int             t;

   bwx_DEBUG(__FUNCTION__);
   
   My->CurrentFile =  My->SYSOUT;
   prn_iprintf("BWBASIC COMMANDS AVAILABLE:\n");

   /* run through the command table and print comand names */

   t = 0;
   for (n = 0; n < NUM_COMMANDS; ++n)
   {
      prn_iprintf(bwb_cmdtable[n].name);
      if (t < 4)
      {
         prn_iprintf("\t");
         t++;
      }
      else
      {
         prn_iprintf("\n");
         t = 0;
      }
   }
   if (t > 0)
   {
      prn_iprintf("\n");
   }
   return bwb_zline(l);
}

static void FixUp( char * Name )
{
   char * C;
   
   C = Name;
   while (*C != BasicNulChar)
   {
      if( bwb_isalnum(*C) )
      {
         /* OK */
      }
      else
      {
         /* FIX */
         switch( *C )
         {
         case '!': *C = '1'; break;
         case '@': *C = '2'; break;
         case '#': *C = '3'; break;
         case '$': *C = '4'; break;
         case '%': *C = '5'; break;
         case '^': *C = '6'; break;
         case '&': *C = '7'; break;
         case '*': *C = '8'; break;
         case '(': *C = '9'; break;
         case ')': *C = '0'; break;
         default:
            *C = '_';
         }
      }  
      C++;
   }
}


static void CommandUniqueID(int i, char *UniqueID)
{
   bwx_DEBUG(__FUNCTION__);

   bwb_strcpy(UniqueID, "C_");

   if (bwb_stricmp(bwb_cmdtable[i].name, "?") == 0)
   {
      bwb_strcat(UniqueID, "QUEST");
   }
   else
   {
      bwb_strcat(UniqueID, bwb_cmdtable[i].name);
   }
   FixUp( UniqueID );
}

static void CommandVector(int i, char *Vector)
{
   bwx_DEBUG(__FUNCTION__);

   bwb_strcpy(Vector, "bwb_");

   if (bwb_stricmp(bwb_cmdtable[i].name, "?") == 0)
   {
      bwb_strcat(Vector, "QUEST");
   }
   else
   {
      bwb_strcat(Vector, bwb_cmdtable[i].name);
   }
   FixUp( Vector );
}

static void CommandOptionVersion(int n, char *OutputLine)
{
   int             i;
   int             j;

   bwx_DEBUG(__FUNCTION__);

   bwb_strcpy(OutputLine, "");
   j = 0;
   for (i = 0; i < NUM_VERSIONS; i++)
   {
      if (bwb_cmdtable[n].OptionVersionBitmask & bwb_vertable[i].OptionVersionBitmask)
      {
         if (j > 0)
         {
            bwb_strcat(OutputLine, " | ");
         }
         bwb_strcat(OutputLine, bwb_vertable[i].ID);
         j++;
      }
   }
}


void SortAllCommands(void)
{
   /* sort by name */
   static int      IsSorted = FALSE;

   bwx_DEBUG(__FUNCTION__);

   if (IsSorted == FALSE)
   {
      int             i;
      for (i = 0; i < NUM_COMMANDS - 1; i++)
      {
         int             j;
         int             k;
         k = i;
         for (j = i + 1; j < NUM_COMMANDS; j++)
         {
            if (bwb_stricmp(bwb_cmdtable[j].name, bwb_cmdtable[k].name) < 0)
            {
               k = j;
            }
         }
         if (k > i)
         {
            CommandType t;
            bwb_memcpy(&t, &(bwb_cmdtable[i]), sizeof(CommandType));
            bwb_memcpy(&(bwb_cmdtable[i]), &(bwb_cmdtable[k]), sizeof(CommandType));
            bwb_memcpy(&(bwb_cmdtable[k]), &t, sizeof(CommandType));
         }
      }
#if THE_PRICE_IS_RIGHT
      for( i = 0; i < 26; i++ )
      {
         My->CommandStart[ i ] = -1;
      }
      for (i = 0; i < NUM_COMMANDS; i++)
      {
         char C = bwb_cmdtable[i].name[0];
         if( bwb_isalpha( C ) )
         {
            int j;
            j = VarTypeIndex( C );
            if( My->CommandStart[ j ] < 0 )
            {
               /* this is the first command starting with this letter */
               My->CommandStart[ j ] = i;
            }
         }
      }
#endif
      IsSorted = TRUE;
   }
}

void SortAllFunctions(void)
{
   /* sort by name then number of parameters */
   static int      IsSorted = FALSE;

   bwx_DEBUG(__FUNCTION__);

   if (IsSorted == FALSE)
   {
      int             i;
      for (i = 0; i < NUM_FUNCTIONS - 1; i++)
      {
         int             j;
         int             k;
         k = i;
         for (j = i + 1; j < NUM_FUNCTIONS; j++)
         {
            int             n;
            n = bwb_stricmp(bwb_prefuncs[j].Name, bwb_prefuncs[k].Name);
            if (n < 0)
            {
               k = j;
            }
            else
            if (n == 0)
            {
               if (bwb_prefuncs[j].ParameterCount < bwb_prefuncs[k].ParameterCount)
               {
                  k = j;
               }
            }
         }
         if (k > i)
         {
            FunctionType t;
            bwb_memcpy(&t, &(bwb_prefuncs[i]), sizeof(FunctionType));
            bwb_memcpy(&(bwb_prefuncs[i]), &(bwb_prefuncs[k]), sizeof(FunctionType));
            bwb_memcpy(&(bwb_prefuncs[k]), &t, sizeof(FunctionType));
         }
      }
#if THE_PRICE_IS_RIGHT
      for( i = 0; i < 26; i++ )
      {
         My->FunctionStart[ i ] = -1;
      }
      for (i = 0; i < NUM_FUNCTIONS; i++)
      {
         char C = bwb_prefuncs[i].Name[0];
         if( bwb_isalpha( C ) )
         {
            int j;
            j = VarTypeIndex( C );
            if( My->FunctionStart[ j ] < 0 )
            {
               /* this is the first command starting with this letter */
               My->FunctionStart[ j ] = i;
            }
         }
      }
#endif
      IsSorted = TRUE;
   }
}


void DumpAllCommandUniqueID(void)
{
   register int    i;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   prn_lprintf("/* COMMANDS */\n");

   /* run through the command table and print comand #define */

   for (i = 0; i < NUM_COMMANDS; i++)
   {
      char           *Syntax;
      char            UniqueID[BasicNameLengthMax + 1];

      CommandUniqueID(i, UniqueID);
      Syntax = bwb_cmdtable[i].name;
      sprintf(tbuf, "#define %-30s %3d /* %-30s */\n", UniqueID, i + 1, Syntax);
      prn_lprintf(tbuf);
   }
   sprintf(tbuf, "#define NUM_COMMANDS %d\n", i);
   prn_lprintf(tbuf);

}

static void ProcessEscapeChars(const char *Input, char *Output)
{
   int             n;


   bwx_DEBUG(__FUNCTION__);

   n = 0;

   while (*Input != BasicNulChar)
   {
      /* \a  \b  \f  \n  \r  \t  \v \" \\ */
      switch (*Input)
      {
      case '\a':
         *Output = '\\';
         Output++;
         *Output = 'a';
         Output++;
         break;
      case '\b':
         *Output = '\\';
         Output++;
         *Output = 'b';
         Output++;
         break;
      case '\f':
         *Output = '\\';
         Output++;
         *Output = 'f';
         Output++;
         break;
      case '\n':
         *Output = '\\';
         Output++;
         *Output = 'n';
         Output++;
         break;
      case '\r':
         *Output = '\\';
         Output++;
         *Output = 'r';
         Output++;
         break;
      case '\t':
         *Output = '\\';
         Output++;
         *Output = 't';
         Output++;
         break;
      case '\v':
         *Output = '\\';
         Output++;
         *Output = 'n';
         Output++;
         break;
      case '\"':
         *Output = '\\';
         Output++;
         *Output = '"';
         Output++;
         break;
      case '\\':
         *Output = '\\';
         Output++;
         *Output = '\\';
         Output++;
         break;
      default:
         *Output = *Input;
         Output++;
         break;
      }
      Input++;
      *Output = BasicNulChar;
      n++;
      if (n > 60)
      {
         *Output = '\"';
         Output++;
         *Output = '\n';
         Output++;
         *Output = '\"';
         Output++;
         *Output = BasicNulChar;
         n = 0;
      }
   }
}

void DumpAllCommandTableDefinitions(void)
{
   /* generate bwd_cmd.c */

   int             i;

   bwx_DEBUG(__FUNCTION__);

   prn_lprintf("/* COMMAND TABLE */\n\n");
   prn_lprintf("#include \"bwbasic.h\"\n\n");
   prn_lprintf("CommandType bwb_cmdtable[ NUM_COMMANDS ] =\n");
   prn_lprintf("{\n");

   /* run through the command table and print comand #define */

   for (i = 0; i < NUM_COMMANDS; i++)
   {
      char            tbuf[BasicStringLengthMax + 1];


      prn_lprintf("{\n");

      prn_lprintf("   ");
      CommandUniqueID(i, tbuf);
      prn_lprintf(tbuf);
      prn_lprintf(",  /* UniqueID */\n");

      prn_lprintf("   ");
      prn_lprintf("\"");
      ProcessEscapeChars(bwb_cmdtable[i].Syntax, tbuf);
      prn_lprintf(tbuf);
      prn_lprintf("\"");
      prn_lprintf(",  /* Syntax */\n");

      prn_lprintf("   ");
      prn_lprintf("\"");
      ProcessEscapeChars(bwb_cmdtable[i].Description, tbuf);
      prn_lprintf(tbuf);
      prn_lprintf("\"");
      prn_lprintf(",  /* Description */\n");

      prn_lprintf("   ");
      prn_lprintf("\"");
      prn_lprintf(bwb_cmdtable[i].name);
      prn_lprintf("\"");
      prn_lprintf(",  /* Name */\n");

      prn_lprintf("   ");
      CommandOptionVersion(i, tbuf);
      prn_lprintf(tbuf);
      prn_lprintf("  /* OptionVersionBitmask */\n");

      prn_lprintf("},\n");
   }
   prn_lprintf("};\n\n");

}

void DumpAllCommandSwitchStatement(void)
{
   int             i;

   bwx_DEBUG(__FUNCTION__);

   /* run through the command table and print comand #define */
   prn_lprintf("/* SWITCH */\n");
   prn_lprintf("LineType *bwb_vector( LineType *l )\n");
   prn_lprintf("{\n");

   prn_lprintf("   ");
   prn_lprintf("LineType *r;\n");

   prn_lprintf("   ");
   prn_lprintf("switch( l->cmdnum )\n");

   prn_lprintf("   ");
   prn_lprintf("{\n");

   for (i = 0; i < NUM_COMMANDS; i++)
   {
      char            tbuf[BasicNameLengthMax + 1];

      prn_lprintf("   ");
      prn_lprintf("case ");
      CommandUniqueID(i, tbuf);
      prn_lprintf(tbuf);
      prn_lprintf(":\n");

      prn_lprintf("   ");
      prn_lprintf("   ");
      prn_lprintf("r = ");
      CommandVector(i, tbuf);
      prn_lprintf(tbuf);
      prn_lprintf("( l );\n");

      prn_lprintf("   ");
      prn_lprintf("   ");
      prn_lprintf("break;\n");

   }

   prn_lprintf("   ");
   prn_lprintf("default:\n");

   prn_lprintf("   ");
   prn_lprintf("   ");
   prn_lprintf("WARN_INTERNAL_ERROR;\n");

   prn_lprintf("   ");
   prn_lprintf("   ");
   prn_lprintf("r = l;\n");

   prn_lprintf("   ");
   prn_lprintf("   ");
   prn_lprintf("break;\n");


   prn_lprintf("   ");
   prn_lprintf("}\n");

   prn_lprintf("   ");
   prn_lprintf("return r;\n");

   prn_lprintf("}\n");

}

void FixDescription( char * Buffer )
{
   int i;
   char * C;
   
   i = 1;
   C = Buffer;
   while( *C != BasicNulChar )
   {
      if( i > 60 && *C == ' ' )
      {
         *C = '\t';
         i = 0;
      }
      i++;
      C++;
   }
}

void DumpOneCommandSyntax(int n, int lp)
{
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   if (n < 0 || n >= NUM_COMMANDS)
   {
      return;
   }
   /* NAME */
   {
      sprintf(tbuf, "     SYNTAX: %s\n", bwb_cmdtable[n].Syntax);
      if (lp)
      {
         prn_lprintf(tbuf);
      }
      else
      {
         prn_xprintf(tbuf);
      }
   }
   /* DESCRIPTION */
   {
                     
      sprintf(tbuf, "DESCRIPTION: %s\n", bwb_cmdtable[n].Description);
      FixDescription( tbuf );
      {
         char *C;
         
         C = tbuf;
         while( *C != BasicNulChar )
         {
            if( *C == '\t' )
            {
               char tbuf2[32];
               bwb_strcpy( tbuf2, "\n             " );
               if (lp)
               {
                  prn_lprintf(tbuf2);
               }
               else
               {
                  prn_xprintf(tbuf2);
               }
               C++;
               while( *C == ' ' )
               {
                  C++;
               }
            }
            else
            {
               if( lp )
               {
                  bwx_LPRINT( *C );
               }
               else
               {
                  bwx_PRINT( *C );
               }
               C++;
            }
         }
      }
   }
   /* COMPATIBILITY */
   if (lp)
   {
      int             i;
      sprintf(tbuf, "   VERSIONS:\n");
      if (lp)
      {
         prn_lprintf(tbuf);
      }
      else
      {
         prn_xprintf(tbuf);
      }
      for (i = 0; i < NUM_VERSIONS; i++)
      {
         char            X;
         if (bwb_cmdtable[n].OptionVersionBitmask & bwb_vertable[i].OptionVersionBitmask)
         {
            /* SUPPORTED */
            X = 'X';
         }
         else
         {
            /* NOT SUPPORTED */
            X = '_';
         }
         sprintf(tbuf, "             [%c] %s\n", X, bwb_vertable[i].Name);
         if (lp)
         {
            prn_lprintf(tbuf);
         }
         else
         {
            prn_xprintf(tbuf);
         }
      }
   }
}

void DumpAllCommandSyntax(void)
{
   /* for the C maintainer */
   int             n;
   bwx_DEBUG(__FUNCTION__);

   prn_lprintf("============================================================\n");
   prn_lprintf("                    COMMANDS                                \n");
   prn_lprintf("============================================================\n");
   prn_lprintf("\n");
   prn_lprintf("\n");
   for (n = 0; n < NUM_COMMANDS; n++)
   {
      prn_lprintf("------------------------------------------------------------\n");
      DumpOneCommandSyntax(n, TRUE);

   }
   prn_lprintf("------------------------------------------------------------\n");
}


void DumpAllCommandHtmlTable(void)
{
   /* generate bwd_cmd.htm */

   int             i;
   int             j;

   bwx_DEBUG(__FUNCTION__);


   /* LEGEND */
   prn_lprintf("<html><head><title>CMDS</title></head><body>\n");
   prn_lprintf("<h1>LEGEND</h1><br>\n");
   prn_lprintf("<table>\n");

   prn_lprintf("<tr>");
   prn_lprintf("<td>");
   prn_lprintf("<b>");
   prn_lprintf("ID");
   prn_lprintf("</b>");
   prn_lprintf("</td>");
   prn_lprintf("<td>");
   prn_lprintf("<b>");
   prn_lprintf("NAME");
   prn_lprintf("</b>");
   prn_lprintf("</td>");
   prn_lprintf("<td>");
   prn_lprintf("<b>");
   prn_lprintf("DESCRIPTION");
   prn_lprintf("</b>");
   prn_lprintf("</td>");
   prn_lprintf("</tr>\n");

   for (j = 0; j < NUM_VERSIONS; j++)
   {
      prn_lprintf("<tr>");
      prn_lprintf("<td>");
      prn_lprintf(bwb_vertable[j].ID);
      prn_lprintf("</td>");
      prn_lprintf("<td>");
      prn_lprintf(bwb_vertable[j].Name);
      prn_lprintf("</td>");
      prn_lprintf("<td>");
      prn_lprintf(bwb_vertable[j].Description);
      prn_lprintf("</td>");
      prn_lprintf("</tr>\n");
   }
   prn_lprintf("</table>\n");
   prn_lprintf("<hr>\n");


   /* DETAILS */
   prn_lprintf("<h1>DETAILS</h1><br>\n");
   prn_lprintf("<table>\n");

   prn_lprintf("<tr>");
   prn_lprintf("<td>");
   prn_lprintf("<b>");
   prn_lprintf("COMMAND");
   prn_lprintf("</b>");
   prn_lprintf("</td>");
   for (j = 0; j < NUM_VERSIONS; j++)
   {
      prn_lprintf("<td>");
      prn_lprintf("<b>");
      prn_lprintf(bwb_vertable[j].ID);
      prn_lprintf("</b>");
      prn_lprintf("</td>");
   }
   prn_lprintf("</tr>\n");


   /* run through the command table and print comand -vs- OPTION VERSION */

   for (i = 0; i < NUM_COMMANDS; i++)
   {
      prn_lprintf("<tr>");
      prn_lprintf("<td>");
      prn_lprintf( (char *) bwb_cmdtable[i].Syntax);
      prn_lprintf("</td>");

      for (j = 0; j < NUM_VERSIONS; j++)
      {
         prn_lprintf("<td>");
         if (bwb_cmdtable[i].OptionVersionBitmask & bwb_vertable[j].OptionVersionBitmask)
         {
            prn_lprintf("X");
         }
         else
         {
            prn_lprintf(" ");
         }
         prn_lprintf("</td>");
      }
      prn_lprintf("</tr>\n");
   }
   prn_lprintf("</table>\n");
   prn_lprintf("</body></html>\n");
   prn_lprintf("\n");
}

LineType * bwb_HELP(LineType * l)
{
   /* HELP ... */
   int             n;
   char            tbuf[BasicStringLengthMax + 1];
   int             Found;
   char * C;

   bwx_DEBUG(__FUNCTION__);

   Found = FALSE;

   C = l->buffer;
   C += l->position;
   bwb_strcpy( tbuf, C );

   /* RTRIM$ */
   C = tbuf;
   if (*C != 0)
   {
      /* not an empty line, so remove one (or more) trailing spaces */
      char           *E;

      E = bwb_strchr(tbuf, 0);
      E--;
      while (E >= tbuf && *E == ' ')
      {
         *E = 0;
         E--;
      }
   }
   

   /* EXACT match */
   for (n = 0; n < NUM_COMMANDS; n++)
   {
      if (bwb_stricmp(bwb_cmdtable[n].name, tbuf) == 0)
      {
         prn_xprintf("------------------------------------------------------------\n");
         DumpOneCommandSyntax(n, FALSE);
         Found = TRUE;
      }
   }

   for (n = 0; n < NUM_FUNCTIONS; n++)
   {
      if (bwb_stricmp(bwb_prefuncs[n].Name, tbuf) == 0)
      {
         prn_xprintf("------------------------------------------------------------\n");
         DumpOneFunctionSyntax(n, FALSE);
         Found = TRUE;
      }
   }

   if (Found == FALSE)
   {
      /* PARTIAL match */
      int             Length;
      Length = bwb_strlen(tbuf);

      for (n = 0; n < NUM_COMMANDS; n++)
      {
         if (bwb_strnicmp(bwb_cmdtable[n].name, tbuf, Length) == 0)
         {
            if (Found == FALSE)
            {
               prn_xprintf("The following topics are a partial match:\n");
            }
            prn_xprintf(bwb_cmdtable[n].name);
            prn_xprintf("\t");
            Found = TRUE;
         }
      }

      for (n = 0; n < NUM_FUNCTIONS; n++)
      {
         if (bwb_strnicmp(bwb_prefuncs[n].Name, tbuf, Length) == 0)
         {
            if (Found == FALSE)
            {
               prn_xprintf("The following topics are a partial match:\n");
            }
            prn_xprintf(bwb_prefuncs[n].Name);
            prn_xprintf("\t");
            Found = TRUE;
         }
      }
      if (Found == TRUE)
      {
         /* match */
         prn_xprintf("\n");
      }
   }
   if (Found == FALSE)
   {
      /* NO match */
      prn_xprintf("No help found.\n");
   }
   line_skip_eol(l);
   return bwb_zline(l);

}

int NumberValueCheck(unsigned long ParameterTests, BasicNumberType X)
{
   BasicNumberType XR;  /* rounded value */


   bwx_DEBUG(__FUNCTION__);


   /* check for invalid numeric value */
   if (isnan(X))
   {
      /* INTERNAL ERROR */
      return -1;
   }
   /* VALID NUMERIC VALUE */
   XR = bwb_rint(X);
   ParameterTests &= 0x0000000F;
   switch (ParameterTests)
   {
   case P1ERR:
      /* INTERNAL ERROR */
      return -1;
      /* break; */
   case P1ANY:
      if (XR < -DBL_MAX || XR > DBL_MAX)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1BYT:
      if (XR < 0 || XR > UCHAR_MAX)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1INT:
      if (XR < SHRT_MIN || XR > SHRT_MAX)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1LNG:
      if (XR < LONG_MIN || XR > LONG_MAX)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1CUR:
      if (XR < LONG_MIN || XR > LONG_MAX)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1FLT:
      if (X < -FLT_MAX || X > FLT_MAX)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1DBL:
      if (X < -DBL_MAX || X > DBL_MAX)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1DEV:
      /* BasicFileNumberMax must be <= INT16_MAX */
      if (XR < 0 || XR > BasicFileNumberMax)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1LEN:
      /* BasicStringLengthMax must be <= INT16_MAX */
      if (XR < 0 || XR > BasicStringLengthMax)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1POS:
      /* BasicStringLengthMax must be <= INT16_MAX */
      if (XR < 1 || XR > BasicStringLengthMax)
      {
         /* ERROR */
         return -1;
      }
      /* OK */
      return 0;
      /* break; */
   case P1COM:
      /* Number of COMx: ports must be <= INT16_MAX */
      if (XR == 1 || XR == 2 || XR == 3 || XR == 4)
      {
         /* OK */
         return 0;
      }
      break;
   case P1LPT:
      /* Number of LPTx: ports must be <= INT16_MAX */
      if (XR == 0 || XR == 1 || XR == 2 || XR == 3)
      {
         /* OK */
         return 0;
      }
      break;
   case P1GTZ:
      if (X > 0)
      {
         /* OK */
         return 0;
      }
      break;
   case P1GEZ:
      if (X >= 0)
      {
         /* OK */
         return 0;
      }
      break;
   case P1NEZ:
      if (X != 0)
      {
         /* OK */
         return 0;
      }
      break;
   }
   /* ERROR */
   return -1;
}

int StringLengthCheck(unsigned long ParameterTests, int s)
{

   bwx_DEBUG(__FUNCTION__);

   /* check for invalid string length */
   if (s < 0 || s > BasicStringLengthMax)
   {
      /* INTERNAL ERROR */
      return -1;
   }
   /* VALID STRING LENGTH */
   ParameterTests &= 0x0000000F;

   switch (ParameterTests)
   {
   case P1ERR:
      /* INTERNAL ERROR */
      return -1;
      /* break; */
   case P1ANY:
      /* OK */
      return 0;
      /* break; */
   case P1BYT:
      if (s >= sizeof(BasicByteType))
      {
         /* OK */
         return 0;
      }
      break;
   case P1INT:
      if (s >= sizeof(BasicIntegerType))
      {
         /* OK */
         return 0;
      }
      break;
   case P1LNG:
      if (s >= sizeof(BasicLongType))
      {
         /* OK */
         return 0;
      }
      break;
   case P1CUR:
      if (s >= sizeof(BasicCurrencyType))
      {
         /* OK */
         return 0;
      }
      break;
   case P1FLT:
      if (s >= sizeof(BasicSingleType))
      {
         /* OK */
         return 0;
      }
      break;
   case P1DBL:
      if (s >= sizeof(BasicDoubleType))
      {
         /* OK */
         return 0;
      }
      break;
   case P1DEV:
   case P1LEN:
   case P1POS:
   case P1GEZ:
   case P1GTZ:
   case P1NEZ:
      /* ERROR */
      return -1;
      /* break; */
   }
   /* ERROR */
   return -1;
}

void FunctionDefinitionCheck(FunctionType * f)
{
   /* function definition check -- look for obvious errors */
   bwx_DEBUG(__FUNCTION__);


   /* sanity check */
   if (f->ParameterCount == PNONE || f->ParameterCount == 0xFF)
   {
      /* function has NO explicit parameters */
      if (f->ParameterTypes == PNONE)
      {
         /* OK */
      }
      else
      {
         /* oops */
         sprintf(My->bwb_ebuf, "invalid ParameterTypes <%s>\n", f->Name);
         prn_xprintf(My->bwb_ebuf);
      }
      if (f->ParameterTests == PNONE)
      {
         /* OK */
      }
      else
      {
         /* oops */
         sprintf(My->bwb_ebuf, "invalid ParameterTests <%s>\n", f->Name);
         prn_xprintf(My->bwb_ebuf);
      }
   }
   else
   {
      /* function HAS an explicit number of parameters */
      int             i;
      unsigned long   ParameterTests;
      ParameterTests = f->ParameterTests;
      for (i = 0; i < f->ParameterCount; i++)
      {
         /* sanity check this parameter */
         unsigned long   thischeck;
         thischeck = ParameterTests & 0x0000000F;
         /* verify parameter check  */
         if (f->ParameterTypes & (1 << i))
         {
            /* STRING */
            if (thischeck >= P1ANY && thischeck <= P1DBL)
            {
               /* OK */
            }
            else
            {
               /* oops */
               sprintf(My->bwb_ebuf, "invalid ParameterTests <%s> parameter %d\n", f->Name, i + 1);
               prn_xprintf(My->bwb_ebuf);
            }
         }
         else
         {
            /* NUMBER */
            if (thischeck >= P1ANY && thischeck <= P1NEZ)
            {
               /* OK */
            }
            else
            {
               /* oops */
               sprintf(My->bwb_ebuf, "invalid ParameterTests <%s> parameter %d\n", f->Name, i + 1);
               prn_xprintf(My->bwb_ebuf);
            }
         }
         ParameterTests = ParameterTests >> 4;
      }
      if (ParameterTests != 0)
      {
         /* oops */
         sprintf(My->bwb_ebuf, "invalid ParameterTests <%s> parameter %d\n", f->Name, i + 1);
         prn_xprintf(My->bwb_ebuf);
      }
   }
}

void FunctionUniqueID(FunctionType * f, char *UniqueID)
{
   /* generate the function's UniqueID */
   /* manual fixup required for duplicates */
   char            NumVar;
   char            StrVar;

   bwx_DEBUG(__FUNCTION__);


   NumVar = 'X';
   StrVar = 'A';

   /* F_  */
   bwb_strcpy(UniqueID, "F_");
   /* NAME  */
   bwb_strcat(UniqueID, f->Name);
   /* PARAMETERS */
   if (f->ParameterCount == PNONE || f->ParameterCount == 0xFF)
   {
      /* function has NO explicit parameters */
   }
   else
   {
      /* function HAS explicit parameters */
      int             i;
      unsigned long    ParameterTypes;
      /* unsigned long ParameterTests; */
      ParameterTypes = f->ParameterTypes;
      /* ParameterTests = f->ParameterTests; */
      for (i = 0; i < f->ParameterCount; i++)
      {
         char            VarName[BasicNameLengthMax + 1];
         /* sanity check this parameter */
         /* unsigned long thischeck; thischeck =
          * ParameterTests & 0x0000000F; */
         /* verify parameter check  */
         if (ParameterTypes & 1)
         {
            /* STRING */
            sprintf(VarName, "_%c", StrVar);
            StrVar++;
         }
         else
         {
            /* NUMBER */
            sprintf(VarName, "_%c", NumVar);
            NumVar++;
         }
         bwb_strcat(UniqueID, VarName);
         ParameterTypes = ParameterTypes >> 1;
         /* ParameterTests = ParameterTests >> 4; */
      }
   }
   /* RETURN TYPE */
   if (f->ReturnType == BasicStringSuffix)
   {
      bwb_strcat(UniqueID, "_S");
   }
   else
   {
      bwb_strcat(UniqueID, "_N");
   }
   /* fixup illegal characters,  "DEF FN" "BLOAD:", "CLOAD*" */
   FixUp(UniqueID);
}


void FunctionSyntax(FunctionType * f, char *Syntax)
{
   /* generate the function's Syntax */
   char            NumVar;
   char            StrVar;

   bwx_DEBUG(__FUNCTION__);


   NumVar = 'X';
   StrVar = 'A';

   /* RETURN TYPE */
   if (f->ReturnType == BasicStringSuffix)
   {
      bwb_strcpy(Syntax, "S$ = ");
   }
   else
   {
      bwb_strcpy(Syntax, "N  = ");
   }
   /* NAME  */
   bwb_strcat(Syntax, f->Name);
   /* PARAMETERS */
   if (f->ParameterCount == PNONE )
   {
      /* function has NO explicit parameters */
   }
   else
   if (f->ParameterCount == 0xFF)   
   {
      /* function has a variable number of parameters */
      bwb_strcat(Syntax, "( ... )");
   }
   else
   {
      /* function HAS explicit parameters */
      int             i;
      unsigned long ParameterTypes;
      /* unsigned long ParameterTests; */
      ParameterTypes = f->ParameterTypes;
      /* ParameterTests = f->ParameterTests; */

      if (f->ReturnType == BasicStringSuffix)
      {
         bwb_strcat(Syntax, "( ");
      }
      else
      {
         bwb_strcat(Syntax, "( ");
      }

      for (i = 0; i < f->ParameterCount; i++)
      {
         char            VarName[BasicNameLengthMax + 1];
         /* sanity check this parameter */
         /* unsigned long thischeck; thischeck =
          * ParameterTests & 0x0000000F; */
         if (i > 0)
         {
            bwb_strcat(Syntax, ", ");
         }
         /* verify parameter check  */
         if (ParameterTypes & 1)
         {
            /* STRING */
            sprintf(VarName, "%c$", StrVar);
            StrVar++;
         }
         else
         {
            /* NUMBER */
            sprintf(VarName, "%c", NumVar);
            NumVar++;
         }
         bwb_strcat(Syntax, VarName);
         ParameterTypes = ParameterTypes >> 1;
         /* ParameterTests = ParameterTests >> 4; */
      }
      if (f->ReturnType == BasicStringSuffix)
      {
         bwb_strcat(Syntax, " )");
      }
      else
      {
         bwb_strcat(Syntax, " )");
      }
   }
}

void DumpAllFuctionUniqueID(void)
{
   /* for the C maintainer */
   char            tbuf[BasicStringLengthMax + 1];
   int             i;

   bwx_DEBUG(__FUNCTION__);

   prn_lprintf("/* FUNCTIONS */\n");
   for (i = 0; i < NUM_FUNCTIONS; i++)
   {
      char            UniqueID[BasicStringLengthMax + 1];
      char            Syntax[BasicStringLengthMax + 1];

      FunctionUniqueID(&(bwb_prefuncs[i]), UniqueID);
      FunctionSyntax(&(bwb_prefuncs[i]), Syntax);
      sprintf(tbuf, "#define %-30s %3d /* %-30s */\n", UniqueID, i + 1, Syntax);
      prn_lprintf(tbuf);
   }
   sprintf(tbuf, "#define NUM_FUNCTIONS %d\n", i);
   prn_lprintf(tbuf);
}

void DumpAllFunctionSwitch(void)
{
   /* for the C maintainer */
   int             i;

   bwx_DEBUG(__FUNCTION__);

   prn_lprintf("/* SWITCH */\n");
   prn_lprintf("switch( UniqueID )\n");
   prn_lprintf("{\n");
   for (i = 0; i < NUM_FUNCTIONS; i++)
   {
      char            tbuf[BasicStringLengthMax + 1];

      prn_lprintf("case ");
      FunctionUniqueID(&(bwb_prefuncs[i]), tbuf);
      prn_lprintf(tbuf);
      prn_lprintf(":\n");
      prn_lprintf("    break;\n");
   }
   prn_lprintf("}\n");
}

static const char *ParameterRangeID[16] =
{
 "P%dERR",
 "P%dANY",
 "P%dBYT",
 "P%dINT",
 "P%dLNG",
 "P%dCUR",
 "P%dFLT",
 "P%dDBL",
 "P%dDEV",
 "P%dLEN",
 "P%dPOS",
 "P%dCOM",
 "P%dLPT",
 "P%dGTZ",
 "P%dGEZ",
 "P%dNEZ",
};

static const char *NumberVariableRange[16] =
{
  /* P1ERR */ "  PARAMETER: %c  is a number, INTERNAL ERROR",
  /* P1ANY */ "  PARAMETER: %c  is a number",
  /* P1BYT */ "  PARAMETER: %c  is a number, [0,255]",
  /* P1INT */ "  PARAMETER: %c  is a number, [MININT,MAXINT]",
  /* P1LNG */ "  PARAMETER: %c  is a number, [MINLNG,MAXLNG]",
  /* P1CUR */ "  PARAMETER: %c  is a number, [MINCUR,MAXCUR]",
  /* P1FLT */ "  PARAMETER: %c  is a number, [MINFLT,MAXFLT]",
  /* P1DBL */ "  PARAMETER: %c  is a number, [MINDBL,MAXDBL]",
  /* P1DEV */ "  PARAMETER: %c  is a number, [1,MAXDEV]",
  /* P1LEN */ "  PARAMETER: %c  is a number, [0,MAXLEN]",
  /* P1POS */ "  PARAMETER: %c  is a number, [1,MAXLEN]",
  /* P1COM */ "  PARAMETER: %c  is a number, [1,4]",
  /* P1LPT */ "  PARAMETER: %c  is a number, [0,3]",
  /* P1GTZ */ "  PARAMETER: %c  is a number, >  0",
  /* P1GEZ */ "  PARAMETER: %c  is a number, >= 0",
  /* P1NEZ */ "  PARAMETER: %c  is a number, <> 0",
};

static const char *StringVariableRange[16] =
{
  /* P1ERR */ "  PARAMETER: %c$ is a string, INTERNAL ERROR",
  /* P1ANY */ "  PARAMETER: %c$ is a string, LEN >= 0",
  /* P1BYT */ "  PARAMETER: %c$ is a string, LEN >= 1",
  /* P1INT */ "  PARAMETER: %c$ is a string, LEN >= sizeof(INT)",
  /* P1LNG */ "  PARAMETER: %c$ is a string, LEN >= sizeof(LNG)",
  /* P1CUR */ "  PARAMETER: %c$ is a string, LEN >= sizeof(CUR)",
  /* P1FLT */ "  PARAMETER: %c$ is a string, LEN >= sizeof(FLT)",
  /* P1DBL */ "  PARAMETER: %c$ is a string, LEN >= sizeof(DBL)",
  /* P1DEV */ "  PARAMETER: %c$ is a string, RESERVED",
  /* P1LEN */ "  PARAMETER: %c$ is a string, RESERVED",
  /* P1POS */ "  PARAMETER: %c$ is a string, RESERVED",
  /* P1COM */ "  PARAMETER: %c$ is a string, RESERVED",
  /* P1LPT */ "  PARAMETER: %c$ is a string, RESERVED",
  /* P1GTZ */ "  PARAMETER: %c$ is a string, RESERVED",
  /* P1GEZ */ "  PARAMETER: %c$ is a string, RESERVED",
  /* P1NEZ */ "  PARAMETER: %c$ is a string, RESERVED",
};

void DumpAllFuctionTableDefinitions(void)
{
   /* generate bwd_fun.c */
   int             n;

   bwx_DEBUG(__FUNCTION__);


   prn_lprintf("/* FUNCTION TABLE */\n\n");
   prn_lprintf("#include \"bwbasic.h\"\n\n");
   prn_lprintf("FunctionType bwb_prefuncs[ NUM_FUNCTIONS ] =\n");
   prn_lprintf("{\n");
   for (n = 0; n < NUM_FUNCTIONS; n++)
   {
      int             i;
      int             j;
      char            tbuf[BasicStringLengthMax + 1];
      char            UniqueID[BasicStringLengthMax + 1];
      char            Syntax[BasicStringLengthMax + 1];
      FunctionType *f;

      f = &(bwb_prefuncs[n]);

      FunctionUniqueID(f, UniqueID);
      FunctionSyntax(f, Syntax);
      prn_lprintf("{\n");
      sprintf(tbuf, "  %s, /* UniqueID */\n", UniqueID);
      prn_lprintf(tbuf);
      sprintf(tbuf, "  \"%s\", /* Syntax */\n", Syntax);
      prn_lprintf(tbuf);

      prn_lprintf("  ");
      prn_lprintf("\"");
      ProcessEscapeChars(f->Description, tbuf);
      prn_lprintf(tbuf);
      prn_lprintf("\"");
      prn_lprintf(",  /* Description */\n");
      sprintf(tbuf, "  \"%s\", /* Name */\n", f->Name);
      prn_lprintf(tbuf);
      switch(f->ReturnType)
      {
      case BasicDoubleSuffix:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "BasicDoubleSuffix");
         break;
      case BasicSingleSuffix:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "BasicSingleSuffix");
         break;
      case BasicCurrencySuffix:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "BasicCurrencySuffix");
         break;
      case BasicLongSuffix:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "BasicLongSuffix");
         break;
      case BasicIntegerSuffix:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "BasicIntegerSuffix");
         break;
      case BasicByteSuffix:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "BasicByteSuffix");
         break;
      case BasicStringSuffix:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "BasicStringSuffix");
         break;
      default:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "INTERNAL ERROR");
         break;
      }
      prn_lprintf(tbuf);
      sprintf(tbuf, "  %d, /* ParameterCount */\n", f->ParameterCount);
      prn_lprintf(tbuf);
      if (f->ParameterCount == 0 || f->ParameterCount == 0xFF)
      {
         /* function has NO explicit parameters */
         sprintf(tbuf, "  %s, /* ParameterTypes */\n", "PNONE");
         prn_lprintf(tbuf);
         sprintf(tbuf, "  %s, /* ParameterTests */\n", "PNONE");
         prn_lprintf(tbuf);
      }
      else
      {
         /* function has explicit parameters */
         bwb_strcpy(tbuf, "  ");
         for (i = 0; i < f->ParameterCount; i++)
         {
            unsigned long ParameterTypes;
            ParameterTypes = f->ParameterTypes >> i;
            ParameterTypes &= 0x1;
            if (i > 0)
            {
               bwb_strcat(tbuf, " | ");
            }
            if (ParameterTypes)
            {
               sprintf(bwb_strchr(tbuf, BasicNulChar), "P%dSTR", i + 1);
            }
            else
            {
               sprintf(bwb_strchr(tbuf, BasicNulChar), "P%dNUM", i + 1);
            }
         }
         bwb_strcat(tbuf, ", /* ParameterTypes */\n");
         prn_lprintf(tbuf);


         bwb_strcpy(tbuf, "  ");
         for (i = 0; i < f->ParameterCount; i++)
         {
            unsigned long ParameterTests;
            ParameterTests = f->ParameterTests >> (i * 4);
            ParameterTests &= 0xF;

            if (i > 0)
            {
               bwb_strcat(tbuf, " | ");
            }
            sprintf(bwb_strchr(tbuf, 0), ParameterRangeID[ParameterTests], i + 1);
            /* Conversion may lose significant digits */
         }
         bwb_strcat(tbuf, ", /* ParameterTests */\n");
         prn_lprintf(tbuf);
      }
      sprintf(tbuf, "  %s, /* NextPointer */\n", "NULL");
      prn_lprintf(tbuf);
      bwb_strcpy(tbuf, "  ");
      j = 0;
      for (i = 0; i < NUM_VERSIONS; i++)
      {
         if (f->OptionVersionBitmask & bwb_vertable[i].OptionVersionBitmask)
         {
            if (j > 0)
            {
               bwb_strcat(tbuf, " | ");
            }
            bwb_strcat(tbuf, bwb_vertable[i].ID);
            j++;
         }
      }
      bwb_strcat(tbuf, " /* OptionVersionBitmask */\n");
      prn_lprintf(tbuf);
      prn_lprintf("},\n");
   }
   prn_lprintf("};\n\n");
}

void DumpOneFunctionSyntax(int n, int lp)
{
   char            tbuf[BasicStringLengthMax + 1];
   FunctionType *f;

   bwx_DEBUG(__FUNCTION__);

   if (n < 0 || n >= NUM_FUNCTIONS)
   {
      return;
   }
   f = &(bwb_prefuncs[n]);
   /* NAME */
   {
      char            UniqueID[BasicStringLengthMax + 1];
      char            Syntax[BasicStringLengthMax + 1];

      FunctionUniqueID(f, UniqueID);
      FunctionSyntax(f, Syntax);
      sprintf(tbuf, "     SYNTAX: %s\n", Syntax);
      if (lp)
      {
         prn_lprintf(tbuf);
      }
      else
      {
         prn_xprintf(tbuf);
      }
   }
   /* PARAMETERS */
   if (f->ParameterCount == PNONE || f->ParameterCount == 0xFF)   
   {
      /* function has NO explicit parameters */
   }
   else
   {
      /* function HAS explicit parameters */
      int             i;
      unsigned long   ParameterTypes;
      unsigned long   ParameterTests;
      char            NumVar;
      char            StrVar;
      ParameterTypes = f->ParameterTypes;
      ParameterTests = f->ParameterTests;
      NumVar = 'X';
      StrVar = 'A';
      for (i = 0; i < f->ParameterCount; i++)
      {
         /* sanity check this parameter */
         unsigned long   thischeck;
         thischeck = ParameterTests & 0x0000000F;
         /* verify parameter check  */
         if (ParameterTypes & 1)
         {
            /* STRING */
            sprintf(tbuf, StringVariableRange[thischeck], StrVar);
            /* Conversion may lose significant digits */
            StrVar++;
         }
         else
         {
            /* NUMBER */
            sprintf(tbuf, NumberVariableRange[thischeck], NumVar);
            /* Conversion may lose significant digits */
            NumVar++;
         }
         if (lp)
         {
            prn_lprintf(tbuf);
         }
         else
         {
            prn_xprintf(tbuf);
         }
         if (lp)
         {
            prn_lprintf("\n");
         }
         else
         {
            prn_xprintf("\n");
         }
         ParameterTypes = ParameterTypes >> 1;
         ParameterTests = ParameterTests >> 4;
      }
   }
   /* DESCRIPTION */
   {
      sprintf(tbuf, "DESCRIPTION: %s\n", f->Description);
      FixDescription( tbuf );
      {
         char *C;
         
         C = tbuf;
         while( *C != BasicNulChar )
         {
            if( *C == '\t' )
            {
               char tbuf2[32];
               bwb_strcpy( tbuf2, "\n             " );
               if (lp)
               {
                  prn_lprintf(tbuf2);
               }
               else
               {
                  prn_xprintf(tbuf2);
               }
               C++;
               while( *C == ' ' )
               {
                  C++;
               }
            }
            else
            {
               if( lp )
               {
                  bwx_LPRINT( *C );
               }
               else
               {
                  bwx_PRINT( *C );
               }
               C++;
            }
         }
      }
   }
   /* COMPATIBILITY */
   if (lp)
   {
      int             i;
      sprintf(tbuf, "   VERSIONS:\n");
      if (lp)
      {
         prn_lprintf(tbuf);
      }
      else
      {
         prn_xprintf(tbuf);
      }
      for (i = 0; i < NUM_VERSIONS; i++)
      {
         char            X;
         if (f->OptionVersionBitmask & bwb_vertable[i].OptionVersionBitmask)
         {
            /* SUPPORTED */
            X = 'X';
         }
         else
         {
            /* NOT SUPPORTED */
            X = '_';
         }
         sprintf(tbuf, "             [%c] %s\n", X, bwb_vertable[i].Name);
         if (lp)
         {
            prn_lprintf(tbuf);
         }
         else
         {
            prn_xprintf(tbuf);
         }
      }
   }
}

void DumpAllFuctionSyntax(void)
{
   /* for the C maintainer */
   int             n;
   bwx_DEBUG(__FUNCTION__);

   prn_lprintf("============================================================\n");
   prn_lprintf("                    FUNCTIONS                               \n");
   prn_lprintf("============================================================\n");
   prn_lprintf("\n");
   prn_lprintf("\n");
   for (n = 0; n < NUM_FUNCTIONS; n++)
   {
      prn_lprintf("------------------------------------------------------------\n");
      DumpOneFunctionSyntax(n, TRUE);
   }
   prn_lprintf("------------------------------------------------------------\n");
}

void DumpAllFunctionHtmlTable(void)
{
   /* generate bwd_cmd.htm */

   int             i;
   int             j;

   bwx_DEBUG(__FUNCTION__);


   /* LEGEND */
   prn_lprintf("<html><head><title>FNCS</title></head><body>\n");
   prn_lprintf("<h1>LEGEND</h1><br>\n");
   prn_lprintf("<table>\n");

   prn_lprintf("<tr>");
   prn_lprintf("<td>");
   prn_lprintf("<b>");
   prn_lprintf("ID");
   prn_lprintf("</b>");
   prn_lprintf("</td>");
   prn_lprintf("<td>");
   prn_lprintf("<b>");
   prn_lprintf("NAME");
   prn_lprintf("</b>");
   prn_lprintf("</td>");
   prn_lprintf("<td>");
   prn_lprintf("<b>");
   prn_lprintf("DESCRIPTION");
   prn_lprintf("</b>");
   prn_lprintf("</td>");
   prn_lprintf("</tr>\n");

   for (j = 0; j < NUM_VERSIONS; j++)
   {
      prn_lprintf("<tr>");
      prn_lprintf("<td>");
      prn_lprintf(bwb_vertable[j].ID);
      prn_lprintf("</td>");
      prn_lprintf("<td>");
      prn_lprintf(bwb_vertable[j].Name);
      prn_lprintf("</td>");
      prn_lprintf("<td>");
      prn_lprintf(bwb_vertable[j].Description);
      prn_lprintf("</td>");
      prn_lprintf("</tr>\n");
   }
   prn_lprintf("</table>\n");
   prn_lprintf("<hr>\n");


   /* DETAILS */
   prn_lprintf("<h1>DETAILS</h1><br>\n");
   prn_lprintf("<table>\n");

   prn_lprintf("<tr>");
   prn_lprintf("<td>");
   prn_lprintf("<b>");
   prn_lprintf("FUNCTION");
   prn_lprintf("</b>");
   prn_lprintf("</td>");
   for (j = 0; j < NUM_VERSIONS; j++)
   {
      prn_lprintf("<td>");
      prn_lprintf("<b>");
      prn_lprintf(bwb_vertable[j].ID);
      prn_lprintf("</b>");
      prn_lprintf("</td>");
   }
   prn_lprintf("</tr>\n");


   /* run through the command table and print comand -vs- OPTION VERSION */

   for (i = 0; i < NUM_FUNCTIONS; i++)
   {
      prn_lprintf("<tr>");
      prn_lprintf("<td>");
      prn_lprintf( (char *) bwb_prefuncs[i].Syntax);
      prn_lprintf("</td>");

      for (j = 0; j < NUM_VERSIONS; j++)
      {
         prn_lprintf("<td>");
         if (bwb_prefuncs[i].OptionVersionBitmask & bwb_vertable[j].OptionVersionBitmask)
         {
            prn_lprintf("X");
         }
         else
         {
            prn_lprintf(" ");
         }
         prn_lprintf("</td>");
      }
      prn_lprintf("</tr>\n");
   }
   prn_lprintf("</table>\n");
   prn_lprintf("</body></html>\n");
   prn_lprintf("\n");
}

LineType * bwb_DEF_SUB(LineType * l)
{
   /* user is executing a function, such as 100 COS X, as though it were a command */
   /* this applies to both intrinsic functions and user defined functions and subroutines */
   /* no special parsing is required, just add () around the parameters */

   char            Buffer[BasicStringLengthMax + 1];
   int             i;
   char           *C;
   char           *F;
   VariantType   x;      /* no leaks */
   VariantType * X = &x; /* no leaks */
   CLEAR_VARIANT( X );

   bwx_DEBUG(__FUNCTION__);

   i = 0;
   C = l->buffer;
   C += l->position;
   F = Buffer;

   while (*C == ' ')
   {
      /* skip leading spaces before name */
      C++;     
   }
   /* these should not happen */
   if (*C == BasicNulChar)
   {
      WARN_INTERNAL_ERROR;
      goto EXIT;
   }
   if (!bwb_isalpha(*C))
   {
      WARN_INTERNAL_ERROR;
      goto EXIT;
   }
   if (bwb_strlen(C) + 2 > BasicStringLengthMax)
   {
      WARN_INTERNAL_ERROR;
      goto EXIT;
   }
   /* OK */
   while (bwb_isalpha(*C))
   {
      /* copy command name */
      *F = *C;
      F++;
      C++;
   }
   if (*C == BasicStringSuffix)
   {
      /* copy command name */
      *F = *C;
      F++;
      C++;
   }
   while (*C == ' ')
   {
      /* skip trailing spaces after name */
      C++;     
   }
   /* add left paren */
   *F = BasicLeftParenChar;
   F++;
   while (*C != BasicNulChar)
   {
      /* copy parameters */
      if( *C == BasicQuoteChar )
      {
          /* quoted string */
          /* if( TRUE ) */
          {
              /* leading quote */
              *F = *C;
              F++;
              C++;
          }
          while( *C != BasicNulChar && *C != BasicQuoteChar )
          {
              /* copy enclosed string */
              *F = *C;
              F++;
              C++;
          }
          if( *C == BasicQuoteChar )
          {
              /* trailing quote */
              *F = *C;
              F++;
              C++;
          }
          else
          {
             /* add missing trailing quote */
             *F = BasicQuoteChar;
             F++;
          }
      }
      else
      if( *C == ' ' )
      {
          /* skip unquoted spaces */
          C++;
      }
      else
      {
          /* normal char, so copy it */
          *F = *C;
          F++;
          C++;
      }
   }
   /* add right paren */
   *F = BasicRightParenChar;
   F++;
   /* terminate */
   *F = BasicNulChar;
   
   /* Call the expression interpreter to evaluate the function */
   if( buff_read_expression( Buffer, &i, X ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      goto EXIT;
   }
   line_skip_eol(l);
EXIT:
   RELEASE( X );
   return bwb_zline(l);
}



/***************************************************************
  
        FUNCTION:       bwb_zline()
  
        DESCRIPTION: This function is called at the exit from
            Bywater BASIC command functions.  It returns
            a pointer to the current position in the current line.
  
***************************************************************/

LineType * bwb_zline(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* skip trailing spaces */
   line_skip_spaces(l);
   /* usually, but NOT always, we are at the end of the line */
   return l;
}


/***************************************************************
  
        FUNCTION:       fnc_fncs()
  
        DESCRIPTION:    This C function is used for debugging
                        purposes; it prints a list of all defined
                        functions.
  
   SYNTAX:     FNCS
  
***************************************************************/
LineType * bwb_FNCS(LineType * l)
{

   register int    n;
   int             t;

   bwx_DEBUG(__FUNCTION__);

   My->CurrentFile = My->SYSOUT;
   prn_iprintf("BWBASIC FUNCTIONS AVAILABLE:\n");

   /* run through the command table and print comand names */

   t = 0;
   for (n = 0; n < NUM_FUNCTIONS; ++n)
   {
      prn_iprintf(bwb_prefuncs[n].Name);
      if (t < 4)
      {
         prn_iprintf("\t");
         t++;
      }
      else
      {
         prn_iprintf("\n");
         t = 0;
      }
   }
   if (t > 0)
   {
      prn_iprintf("\n");
   }
   return bwb_zline(l);
}


LineType * bwb_MAINTAINER(LineType * l)
{

   int             ShowHelp;
   char varname[BasicNameLengthMax + 1]; /* not actually a variable name, just a small buffer */

   bwx_DEBUG(__FUNCTION__);

   ShowHelp = TRUE;
   if( line_read_varname(l, varname) )
   {
      if (bwb_stricmp(varname, "CMDS") == 0)
      {
         if( line_read_varname(l, varname) )
         {
            if (bwb_stricmp(varname, "HTML") == 0)
            {
               DumpAllCommandHtmlTable();
               ShowHelp = FALSE;
            }
            else
            if (bwb_stricmp(varname, "ID") == 0)
            {
               DumpAllCommandUniqueID();
               ShowHelp = FALSE;
            }
            else
            if (bwb_stricmp(varname, "MANUAL") == 0)
            {
               DumpAllCommandSyntax();
               ShowHelp = FALSE;
            }
            else
            if (bwb_stricmp(varname, "SWITCH") == 0)
            {
               DumpAllCommandSwitchStatement();
               ShowHelp = FALSE;
            }
            else
            if (bwb_stricmp(varname, "TABLE") == 0)
            {
               DumpAllCommandTableDefinitions();
               ShowHelp = FALSE;
            }
         }
      }
      else
      if (bwb_stricmp(varname, "FNCS") == 0)
      {
         if( line_read_varname(l, varname) )
         {
            if (bwb_stricmp(varname, "HTML") == 0)
            {
               DumpAllFunctionHtmlTable();
               ShowHelp = FALSE;
            }
            else
            if (bwb_stricmp(varname, "ID") == 0)
            {
               DumpAllFuctionUniqueID();
               ShowHelp = FALSE;
            }
            else
            if (bwb_stricmp(varname, "MANUAL") == 0)
            {
               DumpAllFuctionSyntax();
               DumpAllOperatorSyntax();
               ShowHelp = FALSE;
            }
            else
            if (bwb_stricmp(varname, "SWITCH") == 0)
            {
               DumpAllFunctionSwitch();
               ShowHelp = FALSE;
            }
            else
            if (bwb_stricmp(varname, "TABLE") == 0)
            {
               DumpAllFuctionTableDefinitions();
               ShowHelp = FALSE;
            }
         }
      }
      else
      if (bwb_stricmp(varname, "DEBUG") == 0)
      {
         if( line_read_varname(l, varname) )
         {
            if (bwb_stricmp(varname, "ON") == 0)
            {
               My->MaintainerDebugOn = TRUE;
               ShowHelp = FALSE;
            }
            else
            if (bwb_stricmp(varname, "OFF") == 0)
            {
               My->MaintainerDebugOn = FALSE;
               ShowHelp = FALSE;
            }
         }
      }
      else
      if (bwb_stricmp(varname, "STACK") == 0)
      {
         /*
         dump the current execution stack, 
         Leftmost is the top, 
         Rigthmost is the bottom. 
         */
         StackType * stack_item;
      
         bwx_DEBUG(__FUNCTION__);
      
         for( stack_item = My->stack_head; stack_item != NULL; stack_item = stack_item->next )
         {
            LineType *l;
   
            l = stack_item->line;
            if (l != NULL)
            {
               char            LineNumber[32];
   
               sprintf(LineNumber, "%d:", l->number);
               prn_xprintf(LineNumber);
            }
         }
         prn_xprintf("\n");
         ShowHelp = FALSE;
      }
   }
   if (ShowHelp == TRUE)
   {
      prn_xprintf("MAINTAINER is a command intended for the C maintainer.\n");
      prn_xprintf("It is used when making global changes to the internal tables.\n");
      prn_xprintf("This command is subject to change without notice, at any time.\n");
      prn_xprintf("Currently, the valid statements are:\n\n");
      prn_xprintf("MAINTAINER CMDS HTML   ' dump COMMAND vs VERSION as HTML table\n");
      prn_xprintf("MAINTAINER CMDS ID     ' dump COMMAND #defines for UniqueID\n");
      prn_xprintf("MAINTAINER CMDS MANUAL ' dump COMMAND manual\n");
      prn_xprintf("MAINTAINER CMDS SWITCH ' dump COMMAND switch(UniqueID)\n");
      prn_xprintf("MAINTAINER CMDS TABLE  ' dump COMMAND table\n");
      prn_xprintf("MAINTAINER FNCS HTML   ' dump FUNCTION vs VERSION as HTML table\n");
      prn_xprintf("MAINTAINER FNCS ID     ' dump FUNCTION #defines for UniqueID\n");
      prn_xprintf("MAINTAINER FNCS MANUAL ' dump FUNCTION manual\n");
      prn_xprintf("MAINTAINER FNCS SWITCH ' dump FUNCTION switch(UniqueID)\n");
      prn_xprintf("MAINTAINER FNCS TABLE  ' dump FUNCTION table\n");
      prn_xprintf("MAINTAINER DEBUG ON    ' enable degug tracing\n");
      prn_xprintf("MAINTAINER DEBUG OFF   ' disable degug tracing\n");
      prn_xprintf("MAINTAINER STACK       ' dump the BASIC stack\n");
   }
   return bwb_zline(l);
}


/***************************************************************
  
        FUNCTION:       fnc_init()
  
        DESCRIPTION:    This command initializes the function
                        linked list, placing all predefined functions
                        in the list.
  
***************************************************************/

int fnc_init( void )
{
   register int    n;
   FunctionType *f;

   bwx_DEBUG(__FUNCTION__);

   My->fnc_head = NULL;
   for (n = 0; n < NUM_FUNCTIONS; n++)
   {
      f = &(bwb_prefuncs[n]);
      FunctionDefinitionCheck(f);
      f->next = My->fnc_head;
      My->fnc_head = f;
   }
   return TRUE;
}

void fnc_add_deffn(LookupType * F, char TypeChar)
{
   FunctionType *f;

   bwx_DEBUG(__FUNCTION__);

   /* get memory for fslt structure */

   if ((f = CALLOC(1, sizeof(FunctionType), "fnc_add_deffn")) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return;
   }
   bwb_strcpy(f->Name, F->name);


   if( TypeChar == BasicNulChar )
   {
      /* default function return type */
      TypeChar = BasicDoubleSuffix;
   }
   f->ReturnType = TypeChar;


   f->ReturnType |= (F->line->cmdnum & 0x00FF) << 8;  /* UniqueID is a line number */
   f->UniqueID = F->line->number;
   f->ParameterCount = F->ParameterCount; /* 0..32, 0xFF == VARIANT */
   f->ParameterTypes = F->ParameterTypes;
   f->OptionVersionBitmask = My->CurrentVersion->OptionVersionBitmask;

   /* establish linkages */

   f->next = My->fnc_head;
   My->fnc_head = f;
}

static LookupType * fslt_find_by_line(int LineNumber)
{
   LookupType   *f;

   bwx_DEBUG(__FUNCTION__);

   /* SEARCH */
   for (f = My->fslt_head; f != NULL; f = f->next)
   {
      if( f->line )
      {
         if (f->line->number == LineNumber)
         {
            /* FOUND */
            return f;
         }
      }
   }
   /* NOT FOUND */
   return NULL;
}

VariableType * fnc_deffn(int argc, VariableType * argv, int unique_id)
{
   /* 
   The generic handler for user defined functions.  
   When called by exp_function(), f->id will be set to the line number of a specific DEF USR.
   */
   LookupType   *f;
   VariableType *v;
   VariableType *argn;
   int             i;
   LineType *call_line;
   StackType *   save_elevel;
   bwx_DEBUG(__FUNCTION__);

   /* initialize the variable if necessary */

   f = fslt_find_by_line(unique_id);

   /* these errors should not occur */
   if (f == NULL)
   {
      WARN_INTERNAL_ERROR;
      return NULL;
   }
   if (argv == NULL)
   {
      WARN_INTERNAL_ERROR;
      return NULL;
   }
   if (f->ParameterCount == 0xFF)
   {
      /* VARIANT */
   }
   else
   if (argc != f->ParameterCount)
   {
      WARN_INTERNAL_ERROR;
      return NULL;
   }
   if (f->ParameterCount == 0xFF)
   {
      /* VARIANT */
      f->local_variable = argv;
   }
   else
   if (argc > 0)
   {
      v = f->local_variable;
      argn = argv;
      for (i = 0; i < argc; i++)
      {
         argn = argn->next;
         if (v == NULL)
         {
            WARN_INTERNAL_ERROR;
            return NULL;
         }
         if (argn == NULL)
         {
            WARN_INTERNAL_ERROR;
            return NULL;
         }
         if ( VAR_IS_STRING( v ) != VAR_IS_STRING( argn ) )
         {
            WARN_INTERNAL_ERROR;
            return NULL;
         }
         /* NEW ... */
         if (v->name[0] != BasicNulChar)
         {
#define PARAM_NUMBER   *argn->memnum
#define PARAM_LENGTH    argn->memstr->length
#define PARAM_BUFFER    argn->memstr->sbuffer
            int             IsError;
            IsError = 0;
            switch (v->VariableTypeChar)
            {
            case BasicStringSuffix:
               IsError = StringLengthCheck(P1ANY, PARAM_LENGTH);
               break;
            case BasicDoubleSuffix:
               IsError = NumberValueCheck(P1DBL, PARAM_NUMBER);
               break;
            case BasicSingleSuffix:
               IsError = NumberValueCheck(P1FLT, PARAM_NUMBER);
               break;
            case BasicCurrencySuffix:
               IsError = NumberValueCheck(P1CUR, PARAM_NUMBER);
               break;
            case BasicLongSuffix:
               IsError = NumberValueCheck(P1LNG, PARAM_NUMBER);
               break;
            case BasicIntegerSuffix:
               IsError = NumberValueCheck(P1INT, PARAM_NUMBER);
               break;
            case BasicByteSuffix:
               IsError = NumberValueCheck(P1BYT, PARAM_NUMBER);
               break;
            default:
               WARN_TYPE_MISMATCH;
               return NULL;
            }
            if (IsError != 0)
            {
               sprintf(My->bwb_ebuf, "ILLEGAL FUUNCTION CALL: %s(%s)", f->name, v->name);
               bwb_Warning(5, My->bwb_ebuf); /* WARN_ILLEGAL_FUNCTION_CALL */
               return argv;
            }
         }
         /* ...NEW */
         v = v->next;
      }
   }
   /* OK */
   call_line = f->line; /* line to call for function */
   call_line->position = f->startpos;

   if (call_line->cmdnum == C_DEF)
   {
      line_skip_spaces(call_line);
      if ( line_skip_char(call_line, '=') == FALSE )
      {
         WARN_INTERNAL_ERROR;
         return NULL;
      }
   }
/* PUSH STACK */
 
   save_elevel = My->stack_head;
   if( bwb_incexec() )
   {
      /* OK */
   }
   else
   {
      /* ERROR */
      WARN_OUT_OF_MEMORY;
      return NULL;
   }
   My->stack_head->line = call_line;
   My->stack_head->code = EXEC_FUNCTION;


   /* create variable chain */
   if (f->ParameterCount == 0xFF)
   {
      /* VARIANT */
   }
   else
   if (argc > 0)
   {
      VariableType *source = NULL; /* source variable */
      source = f->local_variable;
      argn = argv;
      for (i = 0; i < argc; i++)
      {
         argn = argn->next;
         /* copy the name */
         bwb_strcpy(argn->name, source->name);
         
         if ( VAR_IS_STRING( source ) )
         {
         }
         else
         {
            int            IsError;
            double         Value;
            VariantType    variant;
            
            if( var_get( argn, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return NULL;
            }
            if( variant.TypeChar == '$' )
            {
               WARN_TYPE_MISMATCH;
               return NULL;
            }
            Value = variant.Number;
            IsError = 0;
            switch (source->VariableTypeChar)
            {
            case BasicStringSuffix:
               break;
            case BasicDoubleSuffix:
               IsError = NumberValueCheck(P1DBL, Value);
               break;
            case BasicSingleSuffix:
               IsError = NumberValueCheck(P1FLT, Value);
               break;
            case BasicCurrencySuffix:
               IsError = NumberValueCheck(P1CUR, Value);
               Value = bwb_rint( Value );
               break;
            case BasicLongSuffix:
               IsError = NumberValueCheck(P1LNG, Value);
               Value = bwb_rint( Value );
               break;
            case BasicIntegerSuffix:
               IsError = NumberValueCheck(P1INT, Value);
               Value = bwb_rint( Value );
               break;
            case BasicByteSuffix:
               IsError = NumberValueCheck(P1BYT, Value);
               Value = bwb_rint( Value );
               break;
            default:
               WARN_TYPE_MISMATCH;
               return NULL;
            }
            if (IsError != 0)
            {
               sprintf(My->bwb_ebuf, "ILLEGAL FUUNCTION CALL: %s(%s)", f->name, source->name);
               bwb_Warning(5, My->bwb_ebuf); /* WARN_ILLEGAL_FUNCTION_CALL */
               return argv;
            }
            variant.Number = Value;
            if( var_set( argn, &variant ) == FALSE )
            {
               WARN_VARIABLE_NOT_DECLARED;
               return NULL;
            }
         }
         source = source->next;
      }
   }
   if (call_line->cmdnum == C_DEF)
   {
      VariantType   x;      /* no leaks */
      VariantType * X = &x; /* no leaks */
      CLEAR_VARIANT( X );


      /* the function return variable is hidden */
      My->stack_head->local_variable = argv->next;
      /* var_islocal() uses the LoopTopLine to find local variables */
      My->stack_head->LoopTopLine = call_line; /* FUNCTION, SUB */

      /* evaluate the expression */
      if( line_read_expression( call_line, X ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         goto EXIT;
      }
      
      /* save the value */
      switch( X->TypeChar )
      {
      case BasicStringSuffix  :
         if( argv->VariableTypeChar != BasicStringSuffix )
         {
            WARN_TYPE_MISMATCH;
            goto EXIT;
         }
         /* OK */
         {
            StringType t;
            StringType * b;
   
            b = argv->memstr;
            t.sbuffer = X->Buffer;
            t.length  = X->Length;
            str_btob(b, &t);
         }
         break;
      case BasicDoubleSuffix  :
      case BasicSingleSuffix  :
      case BasicCurrencySuffix:
      case BasicLongSuffix    :
      case BasicIntegerSuffix :
      case BasicByteSuffix    :
         /* OK */
         {
            /* both numbers */
            int             IsError = 0;
            double          Value;
      
            Value = X->Number;
            Value = VerifyNumeric(Value);
      
            switch (argv->VariableTypeChar)
            {
            case BasicStringSuffix:
               WARN_TYPE_MISMATCH;
               goto EXIT;
               /* break; */
            case BasicDoubleSuffix:
               IsError = NumberValueCheck(P1DBL,Value);
               break;
            case BasicSingleSuffix:
               IsError = NumberValueCheck(P1FLT,Value);
               break;
            case BasicCurrencySuffix:
               IsError = NumberValueCheck(P1CUR,Value);
               Value = bwb_rint( Value );
               break;
            case BasicLongSuffix:
               IsError = NumberValueCheck(P1LNG,Value);
               Value = bwb_rint( Value );
               break;
            case BasicIntegerSuffix:
               IsError = NumberValueCheck(P1INT,Value);
               Value = bwb_rint( Value );
               break;
            case BasicByteSuffix:
               IsError = NumberValueCheck(P1BYT,Value);
               Value = bwb_rint( Value );
               break;
            default:
               WARN_TYPE_MISMATCH;
               goto EXIT;
               /* break; */
            }
            if (IsError != 0)
            {
               if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") )
               {
                  /* ERROR */
                  goto EXIT;
               }
               /* CONTINUE */
            }
            /* assign Value */
            *argv->memnum = Value;
         }
         break;
      default                 :
         WARN_TYPE_MISMATCH;
         goto EXIT;
         /* break; */
      }
EXIT:
      RELEASE( X );


      /* break variable chain */
      My->stack_head->local_variable = NULL;



/* POP STACK */
      bwb_decexec();

   }
   else
   {
      /* the function return variable is visible */
      My->stack_head->local_variable = argv;
      /* var_islocal() uses the LoopTopLine to find local variables */
      My->stack_head->LoopTopLine = call_line; /* FUNCTION, SUB */
      /* execute until function returns */
      while (My->stack_head != save_elevel)
      {
         bwb_execline();
      }
   }

   if (f->ParameterCount == 0xFF)
   {
      /* VARIANT */
      f->local_variable = NULL;
   }

   if (argv->name[0] != BasicNulChar)
   {
#define RESULT_NUMBER  *argv->memnum
#define RESULT_BUFFER   argv->memstr->sbuffer
#define RESULT_LENGTH   argv->memstr->length
      int             IsError;

      IsError = 0;
      switch ( argv->VariableTypeChar ) 
      {
      case BasicStringSuffix:
         IsError = StringLengthCheck(P1ANY, RESULT_LENGTH);
         break;
      case BasicDoubleSuffix:
         IsError = NumberValueCheck(P1DBL, RESULT_NUMBER);
         break;
      case BasicSingleSuffix:
         IsError = NumberValueCheck(P1FLT, RESULT_NUMBER);
         break;
      case BasicCurrencySuffix:
         IsError = NumberValueCheck(P1CUR, RESULT_NUMBER);
         break;
      case BasicLongSuffix:
         IsError = NumberValueCheck(P1LNG, RESULT_NUMBER);
         break;
      case BasicIntegerSuffix:
         IsError = NumberValueCheck(P1INT, RESULT_NUMBER);
         break;
      case BasicByteSuffix:
         IsError = NumberValueCheck(P1BYT, RESULT_NUMBER);
         break;
      default:
         /* no check */
         break;
      }
      if (IsError != 0)
      {
         if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") )
         {
            /* ERROR */
         }
         /* CONTINUE */
      }
   }
   /* ...NEW */


   return argv;
}

/***************************************************************
  
        FUNCTION:       fnc_find()
  
        DESCRIPTION:    This C function attempts to locate
                        a BASIC function with the specified name.
                        If successful, it returns a pointer to
                        the C structure for the BASIC function,
                        if not successful, it returns NULL.
  
***************************************************************/

FunctionType * fnc_find(char *name)
{
#if THE_PRICE_IS_RIGHT
   char C;
   bwx_DEBUG(__FUNCTION__);

   C = name[0];
   if( bwb_isalpha( C ) )
   {
      FunctionType *f;
      int i;

      /* search USER functions */
      for (f = My->fnc_head; f != NULL && (f->ReturnType & 0xFF00) != 0; f = f->next)
      {
         if (My->CurrentVersion->OptionVersionBitmask & f->OptionVersionBitmask)
         {
            if (bwb_stricmp(f->Name, name) == 0)
            {
               return f;
            }
         }
      }

      /* search INTRINSIC functions */
      /* start with the closest function, without going over */
      i = My->FunctionStart[ VarTypeIndex( C ) ]; 
      /* first function starting with this letter, assigned in SortAllFunctions() */
      if( i < 0 )
      {
         /* NOT FOUND */
         return NULL;
      }
      for(; i < NUM_FUNCTIONS; i++ )
      {
         f = &bwb_prefuncs[ i ];
         if (My->CurrentVersion->OptionVersionBitmask & f->OptionVersionBitmask)
         {
            int result;
            
            result = bwb_stricmp(f->Name, name);

            if( result == 0 )
            {
               /* FOUND */
               return f;
            }
            if( result > 0 /* found > searched */ )
            {
               /* NOT FOUND */
               return NULL;
            }
         }
      }
   }
   /* NOT FOUND */
   return NULL;
#else   
   FunctionType *f;
   bwx_DEBUG(__FUNCTION__);

   if (name[0] == BasicNulChar)
   {
      return NULL;
   }

   for (f = My->fnc_head; f != NULL; f = f->next)
   {
      if (My->CurrentVersion->OptionVersionBitmask & f->OptionVersionBitmask)
      {
         if (bwb_stricmp(f->Name, name) == 0)
         {
            return f;
         }
      }
   }
   /* search has failed: return NULL */
   return NULL;
#endif
}

FunctionType * fnc_find_exact(char *name, int ParameterCount, unsigned long ParameterTypes)
{
#if THE_PRICE_IS_RIGHT
   char C;
   bwx_DEBUG(__FUNCTION__);

   C = name[0];
   if( bwb_isalpha( C ) )
   {
      FunctionType *f;
      int i;

      /* search USER functions */
      for (f = My->fnc_head; f != NULL && (f->ReturnType & 0xFF00) != 0; f = f->next)
      {
         if (My->CurrentVersion->OptionVersionBitmask & f->OptionVersionBitmask)
         {
            if (f->ParameterCount == ParameterCount)
            {
               if (f->ParameterTypes == ParameterTypes)
               {
                  if (bwb_stricmp(f->Name, name) == 0)
                  {
                     return f;
                  }
               }
            }
         }
      }

      /* search INTRINSIC functions */
      /* start with the closest function, without going over */
      i = My->FunctionStart[ VarTypeIndex( C ) ]; 
      /* first function starting with this letter, assigned in SortAllFunctions() */
      if( i < 0 )
      {
         /* NOT FOUND */
         return NULL;
      }
      for(; i < NUM_FUNCTIONS; i++ )
      {
         f = &bwb_prefuncs[ i ];
         if (My->CurrentVersion->OptionVersionBitmask & f->OptionVersionBitmask)
         {
            if (f->ParameterCount == ParameterCount)
            {
               if (f->ParameterTypes == ParameterTypes)
               {
                  int result;
                  
                  result = bwb_stricmp(f->Name, name);
      
                  if( result == 0 )
                  {
                     /* FOUND */
                     return f;
                  }
                  if( result > 0 /* found > searched */ )
                  {
                     /* NOT FOUND */
                     return NULL;
                  }
               }
            }
         }
      }
   }
   /* NOT FOUND */
   return NULL;
#else
   FunctionType *f;

   bwx_DEBUG(__FUNCTION__);

   for (f = My->fnc_head; f != NULL; f = f->next)
   {
      if (My->CurrentVersion->OptionVersionBitmask & f->OptionVersionBitmask)
      {
         if (f->ParameterCount == ParameterCount)
         {
            if (f->ParameterTypes == ParameterTypes)
            {
               if (bwb_stricmp(f->Name, name) == 0)
               {
                  return f;
               }
            }
         }
      }
   }
   /* search has failed: return NULL */
   return NULL;
#endif
}

FunctionType * fnc_find_by_id(int unique_id)
{
   /* NOTE: This should ONLY search the INTRINSIC functions, not USER functions  */
   int             i;

   bwx_DEBUG(__FUNCTION__);

   for (i = 0; i < NUM_FUNCTIONS; i++)
   {
      if (bwb_prefuncs[i].UniqueID == unique_id)
      {
         /* FOUND */
         return &(bwb_prefuncs[i]);
      }
   }
   /* NOT FOUND */
   return NULL;
}

static VariableType * find_variable_by_type( char * name, int dimensions, char vartype )
{
   VariableType *v = NULL;
   
   v = var_find( name, dimensions, FALSE );
   if( v )
   {
      if ( VAR_IS_STRING( v ) )
      {
         if( vartype == BasicStringSuffix )
         {
            /* found */
            return v;
         }
      }
      else
      {
         if( vartype != BasicStringSuffix )
         {
            /* found */
            return v;
         }
      }
   }
   /* not found */
   return NULL;
}

/* 
--------------------------------------------------------------------------------------------
                               CHANGE
--------------------------------------------------------------------------------------------
*/

LineType * bwb_CHANGE(LineType * l)
{
   /* SYNTAX: CHANGE A$ TO X */
   /* SYNTAX: CHANGE X TO A$ */

   char            varname[BasicNameLengthMax + 1];
   VariableType *v = NULL;
   VariableType *A = NULL;
   VariableType *X = NULL;
   int             IsStringToArray = FALSE;


   bwx_DEBUG(__FUNCTION__);
   /* get 1st variable */
   if( line_read_varname(l, varname) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   v = find_variable_by_type( varname, 0, BasicStringSuffix );
   if( v )
   {
      /* STRING to ARRAY */
      A = v;
      IsStringToArray = TRUE;
   }
   else
   {
      /* ARRAY to STRING */
      v = find_variable_by_type( varname, 1, BasicDoubleSuffix );
      if( v )
      {
         X = v;
         IsStringToArray = FALSE;
      }
   }
   if( v == NULL )
   {
      WARN_VARIABLE_NOT_DECLARED;
      return bwb_zline(l);
   }
   
   /* get "TO" */
   if( line_skip_word( l, "TO" ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* get 2nd variable */
   line_skip_spaces(l);
   if( line_read_varname(l, varname) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }   

   if (IsStringToArray)
   {
      /* STRING to ARRAY */
      v = find_variable_by_type( varname, 1, BasicDoubleSuffix );
      if( v == NULL )
      {
         v = var_find( varname, 1, TRUE );
      }
      if( v )
      {
         X = v;
      }
   }
   else
   {
      /* ARRAY to STRING */
      v = find_variable_by_type( varname, 0, BasicStringSuffix );
      if( v == NULL )
      {
         v = var_find( varname, 0, TRUE );
      }
      if( v )
      {
         A = v;
      }
   }

   if( v == NULL )
   {
      WARN_VARIABLE_NOT_DECLARED;
      return bwb_zline(l);
   }

   if (IsStringToArray)
   {
      /* CHANGE A$ TO X */
      int             i;
      int             n;
      char           *a;
      BasicNumberType *x;
      unsigned long   t;

      if (A->memstr == NULL)
      {
         WARN_INTERNAL_ERROR;
         return bwb_zline(l);
      }
      if (A->memstr->sbuffer == NULL)
      {
         WARN_INTERNAL_ERROR;
         return bwb_zline(l);
      }
      /* variable storage is a mess, we bypass that tradition here. */
      t = 1;
      for (n = 0; n < X->dimensions; n++)
      {
         t *= X->UBOUND[n] - X->LBOUND[n] + 1;
      }
      if (t <= A->memstr->length)
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return bwb_zline(l);
      }
      n = A->memstr->length;
      a = A->memstr->sbuffer;
      x = X->memnum;
      *x = n;
      x++;
      for (i = 0; i < n; i++)
      {
         char            C;
         BasicNumberType V;

         C = *a;
         V = C;
         *x = V;
         x++;
         a++;
      }
   }
   else
   {
      /* CHANGE X TO A$ */
      int              i;
      int              n;
      char            *a;
      BasicNumberType *x;
      unsigned long    t;

      if (A->memstr == NULL)
      {
         A->memstr = calloc( 1, sizeof( StringType ) );
         if (A->memstr == NULL)
         {
            WARN_INTERNAL_ERROR;
            return bwb_zline(l);
         }
         A->memstr->sbuffer = calloc( 1, BasicStringLengthMax + 1 );
         A->memstr->length = 0;
      }
      if (A->memstr->sbuffer == NULL)
      {
         A->memstr->sbuffer = calloc( 1, BasicStringLengthMax + 1 );
         if (A->memstr->sbuffer == NULL)
         {
            WARN_INTERNAL_ERROR;
            return bwb_zline(l);
         }
         A->memstr->length = 0;
      }
      /* variable storage is a mess, we bypass that tradition here. */
      t = 1;
      for (n = 0; n < X->dimensions; n++)
      {
         t *= X->UBOUND[n] - X->LBOUND[n] + 1;
      }
      if (t <= 1)
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return bwb_zline(l);
      }
      if (t > BasicStringLengthMax)
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return bwb_zline(l);
      }
      /* n = A->memstr->length; */
      a = A->memstr->sbuffer;
      x = X->memnum;
      n = (int) bwb_rint( *x );
      A->memstr->length = n;
      x++;
      for (i = 0; i < n; i++)
      {
         char            C;
         BasicNumberType V;

         V = *x;
         C = V;
         *a = C;
         x++;
         a++;
      }
   }
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               CONSOLE
--------------------------------------------------------------------------------------------
*/

LineType * bwb_CONSOLE(LineType * l)
{
   /* SYNTAX: CONSOLE */
   /* SYNTAX: CONSOLE WIDTH width */

   if( My->IsPrinter == TRUE )
   {
      FileType * T;
      
      T = My->SYSOUT;
      My->SYSOUT = My->SYSPRN;
      My->SYSPRN = T;
   
      My->IsPrinter = FALSE;
   }
   /* 
   In general, this is the result:
   My->SYSOUT->cfp == stdout
   My->SYSPRN->cfp == stderr
   */
   if( line_skip_word( l, "WIDTH" ) )
   {
      int width = 0;
      
      if( line_read_integer_expression( l, &width ) == FALSE )
      {
         WARN_ILLEGAL_FUNCTION_CALL;
         return bwb_zline(l);
      }
      if( width < 0 )
      {
         WARN_ILLEGAL_FUNCTION_CALL;
         return bwb_zline(l);
      }
      My->SYSOUT->width = width;
   }
   
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               LPRINTER
--------------------------------------------------------------------------------------------
*/

LineType * bwb_LPRINTER(LineType * l)
{
   /* SYNTAX: LPRINTER */
   /* SYNTAX: LPRINTER WIDTH width */

   if( My->IsPrinter == FALSE )
   {
      FileType * T;
      
      T = My->SYSOUT;
      My->SYSOUT = My->SYSPRN;
      My->SYSPRN = T;
   
      My->IsPrinter = TRUE;
   }
   /*
   In general, this is the result:
   My->SYSOUT->cfp == stderr
   My->SYSPRN->cfp == stdout
   */
   if( line_skip_word( l, "WIDTH" ) )
   {
      int width = 0;
      
      if( line_read_integer_expression( l, &width ) == FALSE )
      {
         WARN_ILLEGAL_FUNCTION_CALL;
         return bwb_zline(l);
      }
      if( width < 0 )
      {
         WARN_ILLEGAL_FUNCTION_CALL;
         return bwb_zline(l);
      }
      My->SYSOUT->width = width;
   }
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               CREATE
--------------------------------------------------------------------------------------------
*/

LineType * bwb_CREATE(LineType * l)
{
   /* SYNTAX: CREATE filename$ [ RECL reclen ] AS filenum [ BUFF number ] [ RECS size ] */
   int  FileNumber = 0;
   int  width = 0;   
   int  buffnum = 0;
   int  recsnum = 0;
   char filename[ BasicStringLengthMax + 1 ];

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
      bwb_strcpy( filename, Value );
      free( Value );
   }
   if( filename[ 0 ] == BasicNulChar )
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      strupper( filename ); /* CBASIC-II: file names are always upper case */
   }
   if( line_skip_word( l, "RECL" ) )
   {
      if( line_read_integer_expression( l, &width ) == FALSE )
      {
         WARN_FIELD_OVERFLOW;
         return bwb_zline(l);
      }
      if( width <= 0 )
      {
         WARN_FIELD_OVERFLOW;
         return bwb_zline(l);
      }
   }
   if( line_skip_word( l, "AS") == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_integer_expression( l, &FileNumber ) == FALSE )
   {
      WARN_BAD_FILE_NUMBER;
      return bwb_zline(l);
   }
   if( FileNumber <= 0 )
   {
      WARN_BAD_FILE_NUMBER;
      return bwb_zline(l);
   }
   if( line_skip_word( l, "BUFF" ) )
   {
      if( line_read_integer_expression( l, &buffnum ) == FALSE )
      {
         WARN_FIELD_OVERFLOW;
         return bwb_zline(l);
      }
      if( buffnum <= 0 )
      {
         WARN_FIELD_OVERFLOW;
         return bwb_zline(l);
      }
   }
   if( line_skip_word( l, "RECS" ) )
   {
      if( line_read_integer_expression( l, &recsnum ) == FALSE )
      {
         WARN_FIELD_OVERFLOW;
         return bwb_zline(l);
      }
      if( recsnum <= 0 )
      {
         WARN_FIELD_OVERFLOW;
         return bwb_zline(l);
      }
   }
   /* now, we are ready to create the file */      
   My->CurrentFile = find_file_by_number( FileNumber );
   if( My->CurrentFile == NULL )
   {
      My->CurrentFile = file_new();
      My->CurrentFile->FileNumber = FileNumber;
   }
   bwb_strcpy( My->CurrentFile->filename, filename );
   if( My->CurrentFile->mode != DEVMODE_CLOSED )
   {
      if( My->CurrentFile->cfp != NULL )
      {
         fclose( My->CurrentFile->cfp ); /* My->CurrentFile->cfp != NULL */
      }
      if( My->CurrentFile->buffer != NULL )
      {
         FREE( My->CurrentFile->buffer, "bwb_CREATE" );
      }
   }
   My->CurrentFile->width = 0;
   My->CurrentFile->col = 1;
   My->CurrentFile->row = 1;
   My->CurrentFile->delimit = ',';
   My->CurrentFile->buffer = NULL;
   My->CurrentFile->mode = DEVMODE_CLOSED;
   /* truncate to zero length or create text file for update  (reading and writing) */
   if( (My->CurrentFile->cfp = fopen( My->CurrentFile->filename, "w+" )) == NULL )
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   if( width > 0 )
   {
      My->CurrentFile->width = width;
      My->CurrentFile->mode = DEVMODE_RANDOM;
   }
   else
   {
      My->CurrentFile->mode = DEVMODE_INPUT | DEVMODE_OUTPUT;
   }
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               EOF
--------------------------------------------------------------------------------------------
*/



/* EOF */
