/***************************************************************
  
        bwb_cnd.c       Conditional Expressions and Commands
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
--------------------------------------------------------------------------------------------
                                STATIC UTILITY DECLARATIONS
--------------------------------------------------------------------------------------------
*/

static int FindTopLineOnStack(LineType * l);
static LineType * bwb__if_file(LineType * l, int ThenValue );
static LineType * bwb_then_else(LineType * l, int Value);
static int IsTypeMismatch( char L, char R );
static int for_limit_check( BasicNumberType Value, BasicNumberType Target, BasicNumberType Step );


/* 
--------------------------------------------------------------------------------------------
                               EXIT
--------------------------------------------------------------------------------------------
*/

LineType *
bwb_EXIT(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                               SELECT
--------------------------------------------------------------------------------------------
*/
LineType *
bwb_SELECT(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}


/* 
--------------------------------------------------------------------------------------------
                               FUNCTION - END FUNCTION
--------------------------------------------------------------------------------------------
*/

/***************************************************************
  
        FUNCTION:       bwb_FUNCTION()
  
   DESCRIPTION:    This function implements the BASIC
         FUNCTION command, introducing a named
         function.
  
   SYNTAX: FUNCTION subroutine-name
              ...
              [ EXIT FUNCTION ]
              ...
           END FUNCTION
  
***************************************************************/

LineType *
bwb_FUNCTION(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* check current exec level */
   if (My->stack_head->next == NULL)
   {
      /* skip over the entire function definition */
      l = l->OtherLine; /* line of END SUB */
      l = l->next; /* line after END SUB */
      l->position = 0;
      return l;
   }

   /* we are being executed via fnc_deffn() */

   /* if this is the first time at this SUB statement, note it */
   if (My->stack_head->LoopTopLine != l)
   {
      if( bwb_incexec() )
      {
         /* OK */
         My->stack_head->LoopTopLine = l;
      }
      else
      {
         /* ERROR */
         WARN_OUT_OF_MEMORY;
         return &My->bwb_end;
      }
   }
   line_skip_eol(l);
   return bwb_zline(l);
}


LineType *
bwb_EXIT_FUNCTION(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* check integrity of SUB commmand */
   if( FindTopLineOnStack(l->OtherLine) )
   {
      /* FOUND */
      LineType *r;
      bwb_decexec();
      r = l->OtherLine; /* line of FUNCTION */
      r = r->OtherLine; /* line of END FUNCTION */
      r = r->next; /* line after END FUNCTION */
      r->position = 0;
      return r;
   }
   /* NOT FOUND */
   WARN_EXIT_FUNCTION_WITHOUT_FUNCTION;
   return bwb_zline(l);
}

LineType *
bwb_END_FUNCTION(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* check integrity of SUB commmand */
   if (FindTopLineOnStack(l->OtherLine) == FALSE)
   {
      /* NOT FOUND */
      WARN_END_FUNCTION_WITHOUT_FUNCTION;
      return bwb_zline(l);
   }
   /* decrement the stack */
   bwb_decexec();

   /* and return next from old line */
   My->stack_head->line->next->position = 0;
   return My->stack_head->line->next;
}

LineType *
bwb_FNEND(LineType * l)
{
   return bwb_END_FUNCTION( l );
}

LineType *
bwb_FEND(LineType * l)
{
   return bwb_END_FUNCTION( l );
}


/* 
--------------------------------------------------------------------------------------------
                               SUB - END SUB
--------------------------------------------------------------------------------------------
*/

/***************************************************************
  
        FUNCTION:       bwb_sub()
  
   DESCRIPTION:    This function implements the BASIC
         SUB command, introducing a named
         subroutine.
  
   SYNTAX: SUB subroutine-name
              ...
              [ EXIT SUB ]
              ...
           END SUB
  
***************************************************************/

LineType *
bwb_SUB(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* check current exec level */
   if (My->stack_head->next == NULL)
   {
      /* skip over the entire function definition */
      l = l->OtherLine; /* line of END SUB */
      l = l->next; /* line after END SUB */
      l->position = 0;
      return l;
   }
   /* we are being executed via fnc_deffn() */

   /* if this is the first time at this SUB statement, note it */
   if (My->stack_head->LoopTopLine != l)
   {
      if( bwb_incexec() )
      {
         /* OK */
         My->stack_head->LoopTopLine = l;
      }
      else
      {
         /* ERROR */
         WARN_OUT_OF_MEMORY;
         return &My->bwb_end;
      }
   }
   line_skip_eol(l);
   return bwb_zline(l);
}

LineType *
bwb_EXIT_SUB(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* check integrity of SUB commmand */
   if( FindTopLineOnStack(l->OtherLine) )
   {
      /* FOUND */
      LineType *r;
      bwb_decexec();
      r = l->OtherLine; /* line of FUNCTION */
      r = r->OtherLine; /* line of END FUNCTION */
      r = r->next; /* line after END FUNCTION */
      r->position = 0;
      return r;
   }
   /* NOT FOUND */
   WARN_EXIT_SUB_WITHOUT_SUB;
   return bwb_zline(l);
}

LineType *
bwb_END_SUB(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* check integrity of SUB commmand */
   if (FindTopLineOnStack(l->OtherLine) == FALSE)
   {
      /* NOT FOUND */
      WARN_END_SUB_WITHOUT_SUB;
      return bwb_zline(l);
   }
   /* decrement the stack */
   bwb_decexec();

   /* and return next from old line */
   My->stack_head->line->next->position = 0;
   return My->stack_head->line->next;
}

LineType *
bwb_SUBEND(LineType * l)
{
   return bwb_END_SUB( l );
}


/* 
--------------------------------------------------------------------------------------------
                                IF - END IF
--------------------------------------------------------------------------------------------
*/


/***************************************************************
  
        FUNCTION:       bwb_IF()
  
        DESCRIPTION:    This function handles the BASIC IF
                        statement, standard flavor.
  standard
      SYNTAX:     IF expression  THEN line [ELSE line]
                  IF END  # file THEN line [ELSE line]
                  IF MORE # file THEN line [ELSE line]
  
***************************************************************/
LineType *
bwb_IF(LineType * l)
{
   /* classic IF */
   /* IF expression THEN 100          */
   /* IF expression THEN 100 ELSE 200 */
   int             Value;

   bwx_DEBUG(__FUNCTION__);


   if( line_read_integer_expression(l, &Value) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   return bwb_then_else(l, Value);
}

LineType *
bwb_IF_END(LineType * l)
{
   /* IF END #1 THEN 100          */
   if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
   {
      /* sets a linenumber to branch to on EOF */
      int  FileNumber = 0;
      int  LineNumber = 0;


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
      if( line_skip_word( l, "THEN") == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_read_integer_expression( l, &LineNumber ) == FALSE )
      {
         WARN_UNDEFINED_LINE;
         return bwb_zline(l);
      }
      if( LineNumber < 0 )
      {
         WARN_UNDEFINED_LINE;
         return bwb_zline(l);
      }
      /* now, we are ready to create the file */      
      My->CurrentFile = find_file_by_number( FileNumber );
      if( My->CurrentFile == NULL )
      {
         My->CurrentFile = file_new();
         My->CurrentFile->FileNumber = FileNumber;
      }
      My->CurrentFile->EOF_LineNumber = LineNumber;
      return bwb_zline(l);
   }   
   return bwb__if_file( l, TRUE );
}

LineType *
bwb_IF_MORE(LineType * l)
{
   /* IF MORE #1 THEN 100          */
   return bwb__if_file( l, FALSE );
}





/***************************************************************
  
        FUNCTION:       bwb_IF_THEN()
  
        DESCRIPTION:    This function handles the BASIC IF
                        statement, structured flavor.
  
      SYNTAX:     IF expression THEN
                     ...
                  ELSEIF expression
                     ...
                  ELSE
                     ...
                  END IF
  
***************************************************************/
LineType *
bwb_IF_THEN(LineType * l)
{
   /* structured IF */
   LineType *else_line;
   int             Value;

   bwx_DEBUG(__FUNCTION__);
   
   /* evaluate the expression */
   if( line_read_integer_expression(l, &Value) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if (Value)
   {
      /* expression is TRUE */
      l->next->position = 0;
      return l->next;
   }

   /* 
   RESUME knows we iterate thru the various ELSEIF commands, and restarts at the IF THEN command.
   RESUME NEXT knows we iterate thru the various ELSEIF commands, and restarts at the END IF command.
   */

   for( else_line = l->OtherLine; else_line->cmdnum == C_ELSEIF; else_line = else_line->OtherLine )
   {
      else_line->position = else_line->Startpos;

      /* evaluate the expression */
      if( line_read_integer_expression(else_line, &Value) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if (Value)
      {
         /* expression is TRUE */
         else_line->next->position = 0;
         return else_line->next;
      }
   }
   /* ELSE or END IF */
   else_line->next->position = 0;
   return else_line->next;   
}

LineType *
bwb_ELSEIF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   for( l = l->OtherLine; l->OtherLine != NULL; l = l->OtherLine );
   l = l->next; /* line after END IF */
   l->position = 0;
   return l; 
}

LineType *
bwb_ELSE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   
   for( l = l->OtherLine; l->OtherLine != NULL; l = l->OtherLine );
   l = l->next; /* line after END IF */
   l->position = 0;
   return l; 
}

LineType *
bwb_END_IF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                                 SELECT CASE - END SELECT
--------------------------------------------------------------------------------------------
*/


/***************************************************************
  
        FUNCTION:       bwb_select()
  
        DESCRIPTION:    This C function handles the BASIC SELECT
                        statement.
  
   SYNTAX:     SELECT CASE expression           ' examples:
               CASE value                       ' CASE 5
               CASE min TO max                  ' CASE 1 TO 10
               CASE IF relationaloperator value ' CASE IF > 5
               CASE IS relationaloperator value ' CASE IS > 5
               CASE ELSE
               END SELECT
  
***************************************************************/



LineType *
bwb_SELECT_CASE(LineType * l)
{
   VariantType selectvalue;
   VariantType *e = &selectvalue;
   LineType *else_line;

   bwx_DEBUG(__FUNCTION__);

   /* evaluate the expression */
   if( line_read_expression( l, e ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   /* 
   RESUME knows we iterate thru the various CASE commands, and restarts at the SELECT CASE command.
   RESUME NEXT knows we iterate thru the various CASE commands, and restarts at the END SELECT command.
   */
   for( else_line = l->OtherLine; else_line->cmdnum == C_CASE; else_line = else_line->OtherLine )
   {
      else_line->position = else_line->Startpos;
      do
      {
         /* evaluate the expression */
         if( line_skip_word( else_line, "IF" ) || line_skip_word( else_line, "IS" ) )
         {
            /* CASE IS < 10    */
            /* CASE IF < "DEF" */
            /* CASE IS > 7     */
            /* CASE IS > "ABC" */
         
            char            tbuf[BasicStringLengthMax + 1];
            int             position;
            VariantType casevalue;
            VariantType *r = &casevalue;

            if( e->TypeChar == BasicStringSuffix )
            {
               /* STRING */
               bwb_strcpy( tbuf, e->Buffer );
            }
            else
            {
               /* NUMBER */
               sprintf(tbuf, BasicNumberPrintFormat, e->Number );
            }
            bwb_strcat( tbuf, " " );
            bwb_strcat( tbuf, &(else_line->buffer[else_line->position]));
            position = 0;
            if( buff_read_expression( tbuf, &position, r) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline(l);
            }
            if( r->TypeChar == BasicStringSuffix )
            {
               RELEASE( r );
               WARN_TYPE_MISMATCH;
               return bwb_zline(l);
            }
            if (r->Number)
            {
               /* expression is TRUE */
               else_line->next->position = 0;
               return else_line->next;
            }
            /* condition is FALSE */
            /* proceed to next CASE line if there is one */
         }
         else
         {
            /* CASE 7 */
            /* CASE 7 TO 10 */
            /* CASE "ABC" */
            /* CASE "ABC" TO "DEF" */
            VariantType minvalue;
            VariantType * minval = &minvalue;

            /* evaluate the MIN expression */   
            if( line_read_expression( else_line, minval ) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline(l);
            }
            if( IsTypeMismatch(e->TypeChar, minval->TypeChar) )
            {
               RELEASE( minval );
               WARN_TYPE_MISMATCH;
               return bwb_zline(l);
            }
            if( line_skip_word( else_line, "TO" ) )
            {
               /* CASE 7 TO 10 */
               /* CASE "ABC" TO "DEF" */
               VariantType maxvalue;
               VariantType * maxval = &maxvalue;

               /* evaluate the MAX expression */
               if( line_read_expression( else_line, maxval ) == FALSE )
               {
                  WARN_SYNTAX_ERROR;
                  return bwb_zline(l);
               }
               if( IsTypeMismatch(e->TypeChar, maxval->TypeChar) )
               {
                  RELEASE( maxval );
                  WARN_TYPE_MISMATCH;
                  return bwb_zline(l);
               }
               if (e->TypeChar == BasicStringSuffix)
               {
                  /* STRING */  
                  if ( bwb_strcmp( e->Buffer, minval->Buffer ) >= 0
                  &&   bwb_strcmp( e->Buffer, maxval->Buffer ) <= 0)
                  {
                     /* expression is TRUE */
                     else_line->next->position = 0;
                     return else_line->next;
                  }
               }
               else
               {
                  /* NUMBER */
                  if( e->Number >= minval->Number
                  &&  e->Number <= maxval->Number )
                  {
                     /* expression is TRUE */
                     else_line->next->position = 0;
                     return else_line->next;
                  }
               }
            }
            else
            {
               /* CASE 7 */
               /* CASE "ABC" */
               if (e->TypeChar == BasicStringSuffix)
               {
                  /* STRING */  
                  if (bwb_strcmp( e->Buffer, minval->Buffer ) == 0)
                  {
                     /* expression is TRUE */
                     else_line->next->position = 0;
                     return else_line->next;
                  }
               }
               else
               {
                  /* NUMBER */
                  if( e->Number == minval->Number )
                  {
                     /* expression is TRUE */
                     else_line->next->position = 0;
                     return else_line->next;
                  }
               }
            }
            /* condition is FALSE */
            /* proceed to next CASE line if there is one */
         }
      }
      while( line_skip_comma( else_line ) );
   }
   /* CASE_ELSE or END_SELECT */
   else_line->next->position = 0;
   return else_line->next;   
}

LineType *
bwb_CASE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   for( l = l->OtherLine; l->OtherLine != NULL; l = l->OtherLine );
   l = l->next; /* line after END SELECT */
   l->position = 0;
   return l; 
}

LineType *
bwb_CASE_ELSE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   for( l = l->OtherLine; l->OtherLine != NULL; l = l->OtherLine );
   l = l->next; /* line after END SELECT */
   l->position = 0;
   return l; 
}


LineType *
bwb_END_SELECT(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   return bwb_zline(l);
}

/* 
--------------------------------------------------------------------------------------------
                                DO - LOOP
--------------------------------------------------------------------------------------------
*/

/***************************************************************
  
   FUNCTION:       bwb_DO()
  
   DESCRIPTION:    This C function implements the ANSI BASIC
         DO statement.
  
   SYNTAX:     DO [UNTIL|WHILE condition]
                  ...
                  [EXIT DO]
                  ...
               LOOP [UNTIL|WHILE condition]
  
***************************************************************/

LineType *
bwb_DO(LineType * l)
{
   LineType *r;
   int Value;
   bwx_DEBUG(__FUNCTION__);
   /* DO ' forever */
   /* DO UNTIL ' exits when != 0 */
   /* DO WHILE ' exits when == 0 */

   do
   {
      /* evaluate the expression */
      if( line_is_eol( l ) )
      {
         break; /* exit 'do' */
      }
      else
      if( line_skip_word( l, "UNTIL" ) )
      {
         /* DO UNTIL */
         if( line_read_integer_expression(l, &Value) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }

         if (Value != 0)
         {
            /* EXIT DO */
            r = l->OtherLine; /* line of LOOP */
            r = r->next;      /* line after LOOP */
            r->position = 0;
            return r;
         }
      }
      else
      if( line_skip_word( l, "WHILE" ) )
      {
         /* DO WHILE */
         if( line_read_integer_expression(l, &Value) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }

         if (Value == 0)
         {
            /* EXIT DO */
            r = l->OtherLine; /* line of LOOP */
            r = r->next;      /* line after LOOP */
            r->position = 0;
            return r;
         }
      }
      
   }
   while( line_skip_comma( l ) );

   return bwb_zline(l);
}


LineType *
bwb_EXIT_DO(LineType * l)
{
   LineType *r;
   bwx_DEBUG(__FUNCTION__);
   /* EXIT DO */
   r = l->OtherLine; /* line of DO */
   r = r->OtherLine; /* line of LOOP */
   r = r->next;      /* line after LOOP */
   r->position = 0;
   return r;
}


LineType *
bwb_LOOP(LineType * l)
{
   LineType *r;
   int Value;
   bwx_DEBUG(__FUNCTION__);
   
   /* LOOP ' forever */
   /* LOOP UNTIL ' exits when != 0 */
   /* LOOP WHILE ' exits when == 0 */

   do
   {
      /* evaluate the expression */
      if( line_is_eol( l ) )
      {
         break; /* exit 'do' */
      }
      else
      if( line_skip_word( l, "UNTIL" ) )
      {
         /* LOOP UNTIL */
         if( line_read_integer_expression(l, &Value) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }

         if (Value != 0)
         {
            /* EXIT DO */
            return bwb_zline(l);
         }
      }
      else
      if( line_skip_word( l, "WHILE" ) )
      {
         /* LOOP WHILE */
         if( line_read_integer_expression(l, &Value) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }

         if (Value == 0)
         {
            /* EXIT DO */
            return bwb_zline(l);
         }
      }
      
   }
   while( line_skip_comma( l ) );

   /* loop around to DO again */
   r = l->OtherLine; /* line of DO */
   r->position = 0;
   return r;
}



/* 
--------------------------------------------------------------------------------------------
                                WHILE - WEND
--------------------------------------------------------------------------------------------
*/




/***************************************************************
  
        FUNCTION:       bwb_WHILE()
  
        DESCRIPTION:    This function handles the BASIC
                        WHILE statement.
  
   SYNTAX:     WHILE expression ' exits when == 0
                 ...
                 [EXIT WHILE]
                 ...
               WEND
  
  
***************************************************************/
LineType *
bwb_WHILE(LineType * l)
{
   int Value;

   LineType * r;
   bwx_DEBUG(__FUNCTION__);

   if( line_read_integer_expression(l, &Value) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if ( Value == 0 )
   {
      /* EXIT WHILE */
      r = l->OtherLine; /* line of WEND */
      r = r->next;      /* line after WEND */
      r->position = 0;
      return r;
   }
   return bwb_zline(l);
}


LineType *
bwb_EXIT_WHILE(LineType * l)
{
   LineType *r;
   bwx_DEBUG(__FUNCTION__);
   /* EXIT WHILE */
   r = l->OtherLine; /* line of WHILE */
   r = r->OtherLine; /* line of WEND */
   r = r->next;      /* line after WEND */
   r->position = 0;
   return r;
}

LineType *
bwb_WEND(LineType * l)
{
   LineType *r;
   bwx_DEBUG(__FUNCTION__);
   r = l->OtherLine; /* line of WHILE */
   r->position = 0;
   return r;
}


/* 
--------------------------------------------------------------------------------------------
                                UNTIL - UEND
--------------------------------------------------------------------------------------------
*/







/***************************************************************
  
        FUNCTION:       bwb_UNTIL()
  
        DESCRIPTION:    This function handles the BASIC 
                         UNTIL statement.
  
   SYNTAX:     UNTIL expression ' exits when != 0
                 ...
                 [EXIT UNTIL]
                 ...
               UEND
                       
  
***************************************************************/
LineType *
bwb_UNTIL(LineType * l)
{
   int Value;

   LineType * r;
   bwx_DEBUG(__FUNCTION__);

   if( line_read_integer_expression(l, &Value) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if ( Value != 0 )
   {
      /* EXIT UNTIL */
      r = l->OtherLine; /* line of UEND */
      r = r->next;      /* line after UEND */
      r->position = 0;
      return r;
   }
   return bwb_zline(l);
}


LineType *
bwb_EXIT_UNTIL(LineType * l)
{
   LineType *r;
   bwx_DEBUG(__FUNCTION__);
   /* EXIT UNTIL */
   r = l->OtherLine; /* line of UNTIL */
   r = r->OtherLine; /* line of UEND */
   r = r->next;      /* line after UEND */
   r->position = 0;
   return r;
}


LineType *
bwb_UEND(LineType * l)
{
   LineType *r;
   bwx_DEBUG(__FUNCTION__);
   r = l->OtherLine; /* line of UNTIL */
   r->position = 0;
   return r;
}


/* 
--------------------------------------------------------------------------------------------
                                FOR - NEXT
--------------------------------------------------------------------------------------------
*/



/***************************************************************
  
        FUNCTION:       bwb_for()
  
        DESCRIPTION:    This function handles the BASIC FOR
                        statement.
  
   SYNTAX:     FOR counter = start TO finish [STEP increment]
                 ...
                 [EXIT FOR]
                 ...
               NEXT [counter]
  
NOTE:    This is controlled by the OptionVersion bitmask.
  
    The order of expression evaluation and variable creation varies.
    For example:
        FUNCTION FNA( Y )
            PRINT "Y="; Y
            FNA = Y
        END FUNCTION
        FOR X = FNA(3) TO FNA(1) STEP FNA(2)
        NEXT X
    ANSI/ECMA;
        Y= 1
        Y= 2
        Y= 3
        X is created (if it does not exist)
        X is assigned the value of 3
    MICROSOFT;
        X is created (if it does not exist)
        Y= 3
        X is assigned the value of 3
        Y= 1
        Y= 2
  
  
ECMA-55: Section 13.4
       ...
       The action of the for-statement and the next-statement is de-
       fined in terms of other statements, as follows:
  
              FOR v = initial-value TO limit STEP increment
              (block)
              NEXT v
  
       is equivalent to:
  
              LET own1 = limit
              LET own2 = increment
              LET v = initial-value
       line1  IF (v-own1) * SGN (own2) > 0 THEN line2
              (block)
              LET v = v + own2
              GOTO line1
       line2  REM continued in sequence
       ...
  
***************************************************************/


LineType *
bwb_FOR(LineType * l)
{
   LineType *r;
   VariableType *v;
   BasicNumberType Value;
   BasicNumberType Target;
   BasicNumberType Step;
   VariantType variant;
   bwx_DEBUG(__FUNCTION__);
   

   /* if this is the first time at this FOR statement, note it */
   if (FindTopLineOnStack(l) == FALSE)
   {
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
   }

   /* INITIALIZE */
   
   
   if( (v = line_read_scalar( l )) == NULL )
   {
      WARN_VARIABLE_NOT_DECLARED;
      return bwb_zline(l);
   }
   if( v->dimensions > 0 )
   {
      WARN_TYPE_MISMATCH;
      return bwb_zline(l);
   }
   if( v->VariableTypeChar == BasicStringSuffix )
   {
      WARN_TYPE_MISMATCH;
      return bwb_zline(l);
   }
   if( line_skip_char( l, '=' ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_numeric_expression( l, &Value) == FALSE )      
   {
      WARN_ILLEGAL_FUNCTION_CALL;
      return bwb_zline(l);
   }
   if (My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* FOR X = ... */ )
   {
      /* Assign Variable */
      variant.TypeChar = v->VariableTypeChar;
      variant.Number = Value;
      if( var_set( v, &variant ) == FALSE )
      {
         WARN_VARIABLE_NOT_DECLARED;
         return bwb_zline( l );
      }
   }
   else
   {
      /* assigned below */
   }
   if( line_skip_word( l, "TO" ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_numeric_expression( l, &Target) == FALSE )      
   {
      WARN_ILLEGAL_FUNCTION_CALL;
      return bwb_zline(l);
   }
   if( line_skip_word( l, "STEP" )  )
   {
      if( line_read_numeric_expression( l, &Step) == FALSE )      
      {
         WARN_ILLEGAL_FUNCTION_CALL;
         return bwb_zline(l);
      }
   }
   else
   {
      Step = 1;
   }
   if (My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* FOR X = ... */ )
   {
      /* assigned above */
   }
   else
   {
      /* Assign Variable */
      variant.TypeChar = v->VariableTypeChar;
      variant.Number = Value;
      if( var_set( v, &variant ) == FALSE )
      {
         WARN_VARIABLE_NOT_DECLARED;
         return bwb_zline( l );
      }
   }

   /* CHECK */
   if ( for_limit_check( Value, Target, Step ) )
   {
      /* EXIT FOR */
      bwb_decexec();

      r = l->OtherLine; /* line of NEXT */
      r = r->next;      /* line after NEXT */
      r->position = 0;
      return r;
   }

   /* we will loop at least once */
   My->stack_head->line = l;
   My->stack_head->code = EXEC_FOR;
   My->stack_head->local_variable = v;
   My->stack_head->for_step = Step;
   My->stack_head->for_target = Target;
   My->stack_head->LoopTopLine = l;
   My->stack_head->OnErrorGoto = 0;
   /* proceed with processing */
   return bwb_zline(l);
}


LineType *
bwb_EXIT_FOR(LineType * l)
{
   LineType *r;
   bwx_DEBUG(__FUNCTION__);

   if (FindTopLineOnStack(l->OtherLine) == FALSE)
   {
      WARN_EXIT_FOR_WITHOUT_FOR;
      return bwb_zline(l);
   }
   My->stack_head->code = EXEC_FOR;
   bwb_decexec();

   r = l->OtherLine; /* line of FOR */
   r = r->OtherLine; /* line of NEXT */
   r = r->next;      /* line after NEXT */
   r->position = 0;
   return r;
}


LineType *
bwb_NEXT(LineType * l)
{
   LineType *r;
   VariableType *v;
   BasicNumberType Value;
   BasicNumberType Target;
   BasicNumberType Step;
   bwx_DEBUG(__FUNCTION__);
   

   if (FindTopLineOnStack(l->OtherLine) == FALSE)
   {
      WARN_NEXT_WITHOUT_FOR;
      return bwb_zline(l);
   }
   My->stack_head->code = EXEC_FOR;
   
   /* INCREMENT */
   v      = My->stack_head->local_variable;
   Target = My->stack_head->for_target;
   Step   = My->stack_head->for_step;

   /* if( TRUE ) */
   {
      VariantType variant;
      if( var_get( v, &variant ) == FALSE )
      {
         WARN_NEXT_WITHOUT_FOR;
         return bwb_zline(l);
      }
      if( variant.TypeChar == '$' )
      {
         WARN_NEXT_WITHOUT_FOR;
         return bwb_zline(l);
      }
      variant.Number += Step;
      Value = variant.Number;
      if( var_set( v, &variant ) == FALSE )
      {
         WARN_NEXT_WITHOUT_FOR;
         return bwb_zline(l);
      }
   }
   
   /* CHECK */
   if ( for_limit_check( Value, Target, Step ) )
   {
      /* EXIT FOR */
      bwb_decexec();
      return bwb_zline(l);
   }
   /* proceed with processing */
   r = l->OtherLine; /* line of FOR */
#if 0 
   /* 
   This example causes a Syntax Error:
   100 FOR I = 1 TO 1000:NEXT
   The error is actually caused by execline().
   Note that the example is a delay loop.
   Only NEXT has this issue, because it jumps to TOP->next.
   All other loop structures jump to either TOP or BOTTOM->next.
   */
   r = r->next;      /* line after FOR */
   r->position = 0;
#endif
   line_skip_eol(r);
   return r;
}


/* 
--------------------------------------------------------------------------------------------
                                STATIC UTILITY ROUTINES
--------------------------------------------------------------------------------------------
*/


static int FindTopLineOnStack(LineType * l)
{
   /* since we are at the top of a loop, we MIGHT be on the stack */
   StackType * stack_item;

   bwx_DEBUG(__FUNCTION__);

   for ( stack_item = My->stack_head; stack_item != NULL; stack_item = stack_item->next )
   {
      LineType *current;

      current = stack_item->LoopTopLine;
      if (current != NULL)
      {
         if (current == l)
         {
            /* FOUND */
            while (My->stack_head != stack_item)
            {
               bwb_decexec();
            }
            /* we are now the top item on the stack */
            return TRUE;
         }
         /* do NOT cross a function/sub boundary */
         switch (current->cmdnum)
         {
         case C_FUNCTION:
         case C_SUB:
         case C_GOSUB:
            /* NOT FOUND */
            return FALSE;
            /* break; */
         }
      }
   }
   /* NOT FOUND */
   return FALSE;
}

static LineType * bwb__if_file(LineType * l, int ThenValue )
{
   /* IF END  # filenumber THEN linenumber */
   /* IF MORE # filenumber THEN linenumber */
   int             Value;
   int FileNumber;

   bwx_DEBUG(__FUNCTION__);


   if( line_skip_char(l,BasicFileNumberPrefix) )
   {
      /* IF END # */
      FileType * F;

      if( line_read_integer_expression( l, &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( FileNumber < 0 )
      {
         /* Printer is NOT EOF */
         Value = FALSE;
      }
      else
      if( FileNumber == 0 )
      {
         /* Console is NOT EOF */
         Value = FALSE;
      }
      else
      {
         /* normal file */
         F = find_file_by_number( FileNumber );
         if( F == NULL )
         {
            WARN_BAD_FILE_NUMBER;
            return bwb_zline(l);
         }
         /* if( TRUE ) */
         {
            /* actual file -- are we at the end? */
            FILE           *fp;
            long             current;
            long             total;
            fp = F->cfp;
            current = ftell(fp);
            fseek(fp, 0, SEEK_END);
            total = ftell(fp);
            if (total == current)
            {
               /* EOF */
               Value = TRUE;
            }
            else
            {
               /* NOT EOF */
               Value = FALSE;
               fseek(fp, current, SEEK_SET);
            }
         }   
      }   
   }
   else
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   
   if (Value == ThenValue)
   {
      /* expression is TRUE, take THEN path */
      return bwb_then_else(l, TRUE);
   }
   /* expression is FALSE, take ELSE path */
   return bwb_then_else(l, FALSE);
}

static LineType * bwb_then_else(LineType * l, int Value)
{
   /* 
   ... THEN 100 
   ... THEN 100 ELSE 200 
   
   The deciding expression has already been parsed and evaluated.
   If Value != 0, then we want to take the THEN path.
   If Value == 0, then we want to take the ELSE path.
   */

   int             LineNumber;
   LineType *x;

   if( line_skip_comma(l) )
   {
      /* OK */
   }
   else
   {
      /* OPTIONAL */
   }

   if( line_skip_word( l, "THEN" ) )
   {
      /* OK */
   }
   else
   if( line_skip_word( l, "GOTO" ) )
   {
      /* OK */
   }
   else
   {
      /* REQUIRED */
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   /* read THEN's LineNumber */
   if( line_read_integer_expression(l, &LineNumber) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   if (Value == 0)
   {
      /* expression is FALSE, take ELSE path */
      if( line_is_eol( l ) )
      {
         /* OPTIONAL */
         return bwb_zline(l);
      }
      
      if( line_skip_comma(l) )
      {
         /* OK */
      }
      else
      {
         /* OPTIONAL */
      }

      if( line_skip_word( l, "ELSE" ) )
      {
         /* OK */
      }
      else
      {
         /* REQUIRED */
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }

      if( line_read_integer_expression(l, &LineNumber) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }

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
      x = find_line_number( LineNumber, TRUE );
   }
   if (x != NULL)
   {
      line_skip_eol(l);
      x->position = 0;
#if THE_PRICE_IS_RIGHT
      l->OtherLine = x; /* save in cache */
#endif /* THE_PRICE_IS_RIGHT */
      return x;
   }
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);

}

static int IsTypeMismatch( char L, char R )
{
   if( L == BasicStringSuffix && R == BasicStringSuffix )
   {
      /* both STRING */
      return FALSE;
   }
   if( L != BasicStringSuffix && R != BasicStringSuffix )
   {
      /* both NUMBER */
      return FALSE;
   }
   /* TYPE MISMATCH */
   return TRUE;
}

static int for_limit_check( BasicNumberType Value, BasicNumberType Target, BasicNumberType Step )
{
   if (Step > 0)
   {
      /* POSITIVE */
      if (Value > Target)
      {
         /* FOR I = 3 TO 2 STEP 1 */
         return TRUE;
      }
   }
   else
   {
      /* NEGATIVE */
      if (Value < Target)
      {
         /* FOR I = -3 TO -2 STEP -1 */
         return TRUE;
      }
   }
   return FALSE;
}

/* EOF */
