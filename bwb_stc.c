/***************************************************************
  
        bwb_stc.c       Commands Related to Structured Programming
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




/* prototypes */

static int fslt_clear(void);
static int scan_readargs(LookupType * f, LineType * l);

/***************************************************************
  
   FUNCTION:       fslt_init()
  
   DESCRIPTION:    This function initializes the FUNCTION-SUB
         lookup table.
  
***************************************************************/

int
fslt_init( void )
{
   bwx_DEBUG(__FUNCTION__);

   My->fslt_head = NULL;
   return TRUE;
}

/***************************************************************
  
        FUNCTION:       bwb_scan()
  
        DESCRIPTION: This function scans all lines of the
            program in memory and creates a FUNCTION-
            SUB lookup table (fslt) for the program.
  
***************************************************************/

static int ErrorMessage( LineType *current )
{
   char tbuf[64];
   switch (current->cmdnum)
   {
   case C_FOR:
      bwb_strcpy( tbuf, "FOR without NEXT" );
      break;
   case C_EXIT_FOR:
      bwb_strcpy( tbuf, "EXIT FOR without FOR" );
      break;
   case C_NEXT:
      bwb_strcpy( tbuf, "NEXT without FOR" );
      break;
   case C_DO:
      bwb_strcpy( tbuf, "DO without LOOP" );
      break;
   case C_EXIT_DO:
      bwb_strcpy( tbuf, "EXIT DO without DO" );
      break;
   case C_LOOP:
      bwb_strcpy( tbuf, "LOOP without DO" );
      break;
   case C_UNTIL:
      bwb_strcpy( tbuf, "UNTIL without UEND" );
      break;
   case C_EXIT_UNTIL:
      bwb_strcpy( tbuf, "EXIT UNTIL without UNTIL" );
      break;
   case C_UEND:
      bwb_strcpy( tbuf, "UEND without UNTIL" );
      break;
   case C_WHILE:
      bwb_strcpy( tbuf, "WHILE without WEND" );
      break;
   case C_EXIT_WHILE:
      bwb_strcpy( tbuf, "EXIT WHILE without WHILE" );
      break;
   case C_WEND:
      bwb_strcpy( tbuf, "WEND without WHILE" );
      break;
   case C_SUB:
      bwb_strcpy( tbuf, "SUB without END SUB" );
      break;
   case C_EXIT_SUB:
      bwb_strcpy( tbuf, "EXIT SUB without SUB" );
      break;
   case C_END_SUB:
      bwb_strcpy( tbuf, "END SUB without SUB" );
      break;
   case C_FUNCTION:
      bwb_strcpy( tbuf, "FUNCTION without END FUNCTION" );
      break;
   case C_EXIT_FUNCTION:
      bwb_strcpy( tbuf, "EXIT FUNCTION without FUNCTION" );
      break;
   case C_END_FUNCTION:
      bwb_strcpy( tbuf, "END FUNCTION without FUNCTION" );
      break;
   case C_IF_THEN:
      bwb_strcpy( tbuf, "IF THEN without END IF" );
      break;
   case C_ELSEIF:
      bwb_strcpy( tbuf, "ELSEIF without IF THEN" );
      break;
   case C_ELSE:
      bwb_strcpy( tbuf, "ELSE without IF THEN" );
      break;
   case C_END_IF:
      bwb_strcpy( tbuf, "END IF without IF THEN" );
      break;
   case C_SELECT_CASE:
      bwb_strcpy( tbuf, "SELECT CASE without END SELECT" );
      break;
   case C_CASE:
      bwb_strcpy( tbuf, "CASE without SELECT CASE" );
      break;
   case C_CASE_ELSE:
      bwb_strcpy( tbuf, "CASE ELSE without SELECT CASE" );
      break;
   case C_END_SELECT:
      bwb_strcpy( tbuf, "END SELECT without SELECT CASE" );
      break;
   default:
      bwb_strcpy( tbuf, "UNKNOWN COMMAND" );
      break;
   }
   sprintf( My->bwb_ebuf, "%s: %d %s\n", tbuf, current->number, current->buffer );
   prn_xprintf( My->bwb_ebuf );
   return FALSE;
}

static LineType * find_BottomLineInCode(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   if( l == NULL )
   {
      return NULL;
   }
   while( l->OtherLine != NULL )
   {
      switch( l->cmdnum )
      {
      case C_NEXT:
      case C_LOOP:
      case C_UEND:
      case C_WEND:
      case C_END_SUB:
      case C_END_FUNCTION:
      case C_END_IF:
      case C_END_SELECT:
         return l;
      }
      l = l->OtherLine;
   }
   /* l->OtherLine == NULL */
   return l;
}   

static int MissingBottomLine( LineType * current, int cmdnum )
{
   LineType * BottomLineInCode;

   BottomLineInCode = find_BottomLineInCode( current );
   if( BottomLineInCode == NULL )
   {
      return TRUE;
   }
   if( BottomLineInCode->cmdnum != cmdnum )
   {
      return TRUE;
   }
   return FALSE;
}

static LineType * FindParentCommand( int cmdnum, unsigned int Indention, LineType *Previous[ /* EXECLEVELS + 1 */ ] )
{
   if( Indention > 0 )
   {
      unsigned int i;
      for( i = Indention - 1; /* i >= 0 */; i-- )
      {
         if( Previous[ i ]->cmdnum  == cmdnum )
         {
            /* FOUND */
            return Previous[ i ]; /* EXIT_FOR->OtherLine == FOR */
         }
         if( i == 0 )
         {
            /* NOT FOUND */
         }
      }
   }
   /* NOT FOUND */
   return NULL;
}

int
bwb_scan(void)
{
   /*
   STATIC ANALYSIS
   
   
   Background.
   This routine began as a way to record the line numbers associated with the cmdnum of FUNCTION, SUB, or LABEL.
   
   Pretty-printing.
   Indention was added for pretty-printing by LIST, based upon the cmdnum (Indetion++, Indention--).
   
   Mismatched structured commands.
   When reviewing a properly indented listing, mismatched structured commands are easier to visually indentify 
   (FOR without NEXT and so on), so Previous[] was added to record the previous cmdnum at a given Indention.
   Comparing Current->cmdnum with Previous->cmdnum allows mismatched structured commands to be detected.

   Reduce stack usage for structured loops.
   OtherLine, which was previously determined at runtime for loops, could now be determined during the scan.
   Previously all loops used the stack so the EXIT command could find the loop's bottom line.
   The EXIT commands could now look in Previous[] to determine their loop's top line and follow that to the loop's bottom line.
   As a result, now the FOR loops use the stack, but all other loops do not.
   
   Reduce stack usaage for structured IF/SELECT.
   Previuosly the structured IF/SELECT command used the stack to hold the results of comparisons and intermediate values.
   OtherLine is now used to link these commands to their next occurring command.
   As a result, the path thru the structure is now chosen at the IF/SELECT command, and the stack is no longer used.
   The RESUME command knows about this linkage, so a simple "RESUME" jumps to the "IF THEN" or "SELECT CASE"
   and "RESUME NEXT" jumps to the "END IF" or "END SELECT".

   Caching for unstructured commands.
   OtherLine was not previously used for any purpose for the unstructured GOTO, GOSUB, IF and ON commands.
   It is now used to cache the line last executed by these commands to reduce the time required to find the target line.
   The cache reduces execution time because the target line is usually (but not always) the same.
   For the simple commands "GOTO 100", "GOSUB 100" and "IF x THEN 100", the cache will always succeed.
   For the command "IF x THEN 100 ELSE 200", the cache will succeed for the last taken path.
   Because programs are typically written so one path is more likely, the cache usually succeeds.
   For the "ON x GOTO ..." and "ON x GOSUB ...", the cache succeeds when the result of the test expression repeats, such as:
      FOR I = 1 TO 100
         ON INT(I/10) GOSUB ...
      NEXT I
   In this example, the cache will succeed 90 times and fail 10 times.
   
   Checking FOR/NEXT variable names.
   If a variable name is provided for a NEXT command, then it is compared against the variable name of the matching FOR command.
   This detects the following kind of mismatch:
      FOR I = ...
      NEXT J
      
   Linking DATA commands.
   OtherLine was not previously used for any purpose for the DATA commands.
   It is now used to point to the next DATA command after the current DATA command.
   This reduces the time required for READ to find the next DATA command.
   The last DATA command has an OtherLine of NULL.
   RESTORE knows about the DATA command linkage and uses this information to find the specified DATA command.
   READ knows about the DATA command linkage and uses this information to find the next DATA command.
   
   */

   /*
   OtherLine is used for different purposes depending upon the command.
   
   For structured IF_THEN and SELECT_CASE, OtherLine is used to form a one-way list:
      IF_THEN->OtherLine == next occuring ELSE_IF, ELSE, END_IF
      ELSE_IF->Otherline == next occuring ELSE_IF, ELSE, END_IF
      ELSE->OtherLine    == END_IF
      END_IF->OtherLine  == NULL


   For the structured loops, OtherLine is uses as a circular list:
      WHILE->OtherLine      == WEND
      EXIT_WHILE->OtherLine == WHILE
      WEND->OtherLine       == WHILE
   
   For unstructured commands, OtherLine is used as a one-entry cache.  
   It contains a pointer to the Most Recently Used line returned by the command:
   
      GOTO->OtherLine       == MRU target line
      GOSUB->OtherLine      == MRU target line
      IF->OtherLine         == MRU target line
      ON->OtherLine         == MRU target line

   
   For other commands, OtherLine is not used.

   */

   LineType *current;
   unsigned int Indention;
   LineType *Previous[ EXECLEVELS + 1 ]; /* previous part of structured command */

   bwx_DEBUG(__FUNCTION__);


   if ( My->rescan != TRUE)
   {
      /* program is clean, no scan required */
      return TRUE;
   }
   /* program needs to be scanned again, because a line was added or deleted */

   /* reset these whenever a SCAN occurs */
   My->data_line = My->bwb_start.next;
   My->data_pos = My->data_line->Startpos;
   My->err_line = NULL;
   My->ContinueLine = NULL;


   /* first run through the FUNCTION - SUB loopkup table and free any existing memory */

   fslt_clear();



   for( Indention = 0; Indention <= EXECLEVELS; Indention++)
   {
      Previous[ Indention ] = NULL;
   }
   Indention = 0;

   for (current =  My->bwb_start.next; current != &My->bwb_end; current = current->next)
   {
      current->OtherLine = NULL;


      switch (current->cmdnum)
      {
      case C_DEF:
      case C_FUNCTION:
      case C_SUB:
      case C_USER_LBL:
         fslt_add(current);
      }

      /* verify the 'current' command is consistent with a 'previous' command at a lower indention */
      switch (current->cmdnum)
      {
      case C_EXIT_FOR:
         current->OtherLine = FindParentCommand( C_FOR, Indention, Previous ); /* EXIT_FOR->OtherLine == FOR */
         if( current->OtherLine == NULL )
         {
            return ErrorMessage( current );
         }
         break;
      case C_EXIT_WHILE:
         current->OtherLine = FindParentCommand( C_WHILE, Indention, Previous ); /* EXIT_WHILE->OtherLine == WHILE */
         if( current->OtherLine == NULL )
         {
            return ErrorMessage( current );
         }
         break;
      case C_EXIT_UNTIL:
         current->OtherLine = FindParentCommand( C_UNTIL, Indention, Previous ); /* EXIT_UNTIL->OtherLine == UNTIL */
         if( current->OtherLine == NULL )
         {
            return ErrorMessage( current );
         }
         break;
      case C_EXIT_FUNCTION:
         current->OtherLine = FindParentCommand( C_FUNCTION, Indention, Previous ); /* EXIT_FUNCTION->OtherLine == FUNCTION */
         if( current->OtherLine == NULL )
         {
            return ErrorMessage( current );
         }
         break;
      case C_EXIT_SUB:
         current->OtherLine = FindParentCommand( C_SUB, Indention, Previous ); /* EXIT_SUB->OtherLine == SUB */
         if( current->OtherLine == NULL )
         {
            return ErrorMessage( current );
         }
         break;
      case C_EXIT_DO:
         current->OtherLine = FindParentCommand( C_DO, Indention, Previous ); /* EXIT_DO->OtherLine == DO */
         if( current->OtherLine == NULL )
         {
            return ErrorMessage( current );
         }
         break;
      }
      

      /* verify the 'current' command is consistent with a 'previous' command at the same indention */
      switch (current->cmdnum)
      {
      case C_NEXT:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_FOR:
            /* if( TRUE ) */
            {
               /* compare the 'NEXT' variable with the 'FOR' variable */
               current->position = current->Startpos;
               Previous[ Indention ]->position = Previous[ Indention ]->Startpos;
               if( line_is_eol( current ) )
               {
                  /* NEXT */
                  /* there is no variable to compare */
               }
               else
               {
                  /* NEXT variable */
                  char NextVarName[ BasicNameLengthMax + 1 ];
                  char ForVarName[ BasicNameLengthMax + 1 ];
   
                  if( line_read_varname( current, NextVarName ) == FALSE)
                  {
                     return ErrorMessage( current );
                  }
                  if( line_read_varname( Previous[ Indention ], ForVarName ) == FALSE)
                  {
                     return ErrorMessage( current );
                  }
                  if( bwb_stricmp( ForVarName, NextVarName ) != 0 )
                  {
                     return ErrorMessage( current );
                  }
               }
               /* MATCHED */
               current->Startpos = current->position;
               Previous[ Indention ]->position = Previous[ Indention ]->Startpos;
            }
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* FOR->OtherLine = NEXT */
            current->OtherLine = Previous[ Indention ]; /* NEXT->OtherLine = FOR */
            Previous[ Indention ] = current; /* last command at this level = NEXT */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_LOOP:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_DO:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* DO->OtherLine = LOOP */
            current->OtherLine = Previous[ Indention ]; /* LOOP->OtherLine = DO */
            Previous[ Indention ] = current; /* last command at this level = LOOP */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_UEND:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_UNTIL:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* UNTIL->OtherLine = UEND */
            current->OtherLine = Previous[ Indention ]; /* UEND->OtherLine = UNTIL */
            Previous[ Indention ] = current; /* last command at this level = UEND */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_WEND:
         if( Indention == 0 )
         {
            sprintf( My->bwb_ebuf, "Unmatched command %d %s", current->number, current->buffer );
            prn_xprintf( My->bwb_ebuf );
            return FALSE;
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_WHILE:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* WHILE->OtherLine = WEND */
            current->OtherLine = Previous[ Indention ]; /* WEND->OtherLine = WHILE */
            Previous[ Indention ] = current; /* last command at this level = WEND */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_END_SUB:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_SUB:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* SUB->OtherLine = END_SUB */
            current->OtherLine = Previous[ Indention ]; /* END_SUB->OtherLine = SUB */
            Previous[ Indention ] = current; /* last command at this level = END_SUB */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_END_FUNCTION:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_FUNCTION:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* FUNCTION->OtherLine = END_FUNCTION */
            current->OtherLine = Previous[ Indention ]; /* END_FUNCTION->OtherLine = FUNCTION */
            Previous[ Indention ] = current; /* last command at this level = END_FUNCTION */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_ELSEIF:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_IF_THEN:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* IF_THEN->OtherLine = ELSEIF */
            Previous[ Indention ] = current; /* last command at this level = ELSEIF */
            break;
         case C_ELSEIF:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* ELSEIF->OtherLine = ELSEIF */
            Previous[ Indention ] = current; /* last command at this level = ELSEIF */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_ELSE:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_IF_THEN:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* IF_THEN->OtherLine = ELSE */
            Previous[ Indention ] = current; /* last command at this level = ELSE */
            break;
         case C_ELSEIF:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* ELSEIF->OtherLine = ELSE */
            Previous[ Indention ] = current; /* last command at this level = ELSE */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_END_IF:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_IF_THEN:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* IF_THEN->OtherLine = END_IF */
            Previous[ Indention ] = current; /* last command at this level = END_IF */
            break;
         case C_ELSEIF:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* ELSEIF->OtherLine = END_IF */
            Previous[ Indention ] = current; /* last command at this level = END_IF */
            break;
         case C_ELSE:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* ELSE->OtherLine = END_IF */
            Previous[ Indention ] = current; /* last command at this level = END_IF */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_CASE:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_SELECT_CASE:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* C_SELECT_CASE->OtherLine = C_CASE */
            Previous[ Indention ] = current; /* last command at this level = C_CASE */
            break;
         case C_CASE:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* CASE->OtherLine = C_CASE */
            Previous[ Indention ] = current; /* last command at this level = C_CASE */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_CASE_ELSE:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_CASE:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* CASE->OtherLine = C_CASE_ELSE */
            Previous[ Indention ] = current; /* last command at this level = C_CASE_ELSE */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      case C_END_SELECT:
         if( Indention == 0 )
         {
            return ErrorMessage( current );
         }
         Indention--;
         switch( Previous[ Indention ]->cmdnum )
         {
         case C_CASE:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* CASE->OtherLine = END_SELECT */
            Previous[ Indention ] = current; /* last command at this level = END_SELECT */
            break;
         case C_CASE_ELSE:
            /* OK */
            Previous[ Indention ]->OtherLine = current; /* CASE_ELSE->OtherLine = END_SELECT */
            Previous[ Indention ] = current; /* last command at this level = END_SELECT */
            break;
         default:
            return ErrorMessage( current );
         }
         break;
      }
      /* OK */

      current->Indention = Indention;

      /* record the 'current' command as the 'previous' command at this indention */
      switch (current->cmdnum)
      {
      case C_FUNCTION:
      case C_SUB:
         /* this 'command' can NOT be inside the structure of another 'command' */
         if( Indention > 0 )
         {
            return ErrorMessage( Previous[ Indention - 1 ] );
         }
      case C_FOR:
      case C_DO:
      case C_UNTIL:
      case C_WHILE:
      case C_IF_THEN:
      case C_SELECT_CASE:
         if( Previous[ Indention ] != NULL )
         {
            /* we are NOT the first command at this indention level */
            /* verify the 'previous' command at this level was properly closed */
            switch( Previous[ Indention ]->cmdnum )
            {
            case C_FOR:
            case C_DO:
            case C_UNTIL:
            case C_WHILE:
            case C_FUNCTION:
            case C_SUB:
            case C_IF_THEN:
            case C_ELSEIF:
            case C_ELSE:
            case C_SELECT_CASE:
            case C_CASE:
            case C_CASE_ELSE:
               /* there is an existing unclosed structure */
               return ErrorMessage( Previous[ Indention ] );
            }
         }
         Previous[ Indention ] = current;
         Indention++;
         if( Indention == EXECLEVELS )
         {
            prn_xprintf( "Program is nested too deep" );
            return FALSE;
         }
         Previous[ Indention ] = NULL;
         break;
      case C_ELSEIF:
      case C_ELSE:
      case C_CASE:
      case C_CASE_ELSE:
         /* 
         Previous[ Indention ] was already checked and assigned above, just indent.
         */
         Indention++;
         if( Indention == EXECLEVELS )
         {
            prn_xprintf( "Program is nested too deep" );
            return FALSE;
         }
         Previous[ Indention ] = NULL;
         break;
      }
   }

   if( Indention > 0 )
   {
      return ErrorMessage( Previous[ Indention - 1 ] );
   }

   /* verify the OtherLine chain terminates correctly; we should find the bottom command */
   for (current =  My->bwb_start.next; current != &My->bwb_end; current = current->next)
   {
      switch(current->cmdnum)
      {
      case C_FOR:
      case C_EXIT_FOR:
         if( MissingBottomLine( current, C_NEXT ) )
         {
            return ErrorMessage( current );
         }
         break;
      case C_DO:
      case C_EXIT_DO:
         if( MissingBottomLine( current, C_LOOP ) )
         {
            return ErrorMessage( current );
         }
         break;
      case C_UNTIL:
      case C_EXIT_UNTIL:
         if( MissingBottomLine( current, C_UEND ) )
         {
            return ErrorMessage( current );
         }
         break;
      case C_WHILE:
      case C_EXIT_WHILE:
         if( MissingBottomLine( current, C_WEND ) )
         {
            return ErrorMessage( current );
         }
         break;
      case C_FUNCTION:
      case C_EXIT_FUNCTION:
         if( MissingBottomLine( current, C_END_FUNCTION ) )
         {
            return ErrorMessage( current );
         }
         break;
      case C_SUB:
      case C_EXIT_SUB:
         if( MissingBottomLine( current, C_END_SUB ) )
         {
            return ErrorMessage( current );
         }
         break;
      case C_IF_THEN:
         if( MissingBottomLine( current, C_END_IF ) )
         {
            return ErrorMessage( current );
         }
         break;
      case C_SELECT_CASE:
         if( MissingBottomLine( current, C_END_SELECT ) )
         {
            return ErrorMessage( current );
         }
         break;
      }
   }


   /* return */

   My->rescan = FALSE;
   return TRUE;

}

/***************************************************************
  
        FUNCTION:       fslt_clear()
  
        DESCRIPTION: This C function clears all existing memory
            in the FUNCTION-SUB lookup table.
  
***************************************************************/

static int fslt_clear(void)
{
   LookupType   *current;

   bwx_DEBUG(__FUNCTION__);

   /* run through table and clear memory */
   for (current = My->fslt_head; current != NULL; )
   {
      LookupType   *next;

      next = current->next;

      /* check for local variables and free them */
      if( current->local_variable != NULL )
      {
         var_free( current->local_variable );
         current->local_variable = NULL;
      }

      /* Revised to FREE pass-thru calls by JBV */
      if (current->name != NULL)
      {
         FREE(current->name, "fslt_clear");  /* JBV */
         current->name = NULL;   /* JBV */
      }
      FREE(current, "fslt_clear");
      current = next;
   }
   My->fslt_head = NULL;
   return TRUE;
}


/***************************************************************
  
        FUNCTION:       fslt_add()
  
        DESCRIPTION: This C function adds an entry to the
            FUNCTION-SUB lookup table.
  
***************************************************************/

int
fslt_add(LineType * l /* , int *position , int code */ )
{
   char           *name;
   LookupType   *f;
   char            TypeChar;
   char            varname[BasicNameLengthMax + 1];
   
   bwx_DEBUG(__FUNCTION__);
   
   /* get the element for name */
   switch (l->cmdnum)
   {
   case C_DEF:
   case C_FUNCTION:
   case C_SUB:
      l->position = l->Startpos;
      if( line_read_varname(l, varname) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return FALSE;
      }
      break;
   case C_USER_LBL:
      l->position = 0;
      if( line_read_label(l, varname) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return FALSE;
      }
      l->position = l->Startpos;
      break;
   default:
      WARN_SYNTAX_ERROR;
      return FALSE;
   }   
   line_skip_spaces( l );

   /* get memory for name buffer */
   /* Revised to CALLOC pass-thru call by JBV */
   if ((name = CALLOC(1, bwb_strlen(varname) + 1, "fslt_add")) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return FALSE;
   }
   bwb_strcpy(name, varname);

   /* get memory for fslt structure */
   if ((f = CALLOC(1, sizeof(LookupType), "fslt_add")) == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return FALSE;
   }
   /* fill in structure */

   f->line = l;
   f->name = name;
   f->local_variable = NULL;
   f->ParameterCount = 0;  /* 0..32, 255 == VARIAMT */
   f->ParameterTypes = 0;  /* bit 0 is first parameter */
   f->startpos = l->position;




   /* read arguments */
   switch (l->cmdnum)
   {
   case C_DEF:
   case C_FUNCTION:
   case C_SUB:
      TypeChar = var_nametype( varname );
      if ( line_peek_char( l, BasicLeftParenChar) )
      {
         if( scan_readargs(f, l) )
         {
            f->startpos = l->position;
         }
      }
      /* determine function type */
      if( TypeChar == BasicNulChar )
      {
         /* function has no explicit type char */
         TypeChar = line_read_type_declaration(l);
         if( TypeChar == BasicNulChar )
         {
            /* function has no declared type */
            TypeChar = My->DefaultVariableType[VarTypeIndex(varname[0])];
         }
      }
      break;
   case C_USER_LBL:
      TypeChar = BasicLongSuffix;
      break;
   default:
      WARN_SYNTAX_ERROR;
      return FALSE;
   }   
   
   /* establish linkages */
   f->next = My->fslt_head;
   My->fslt_head = f;

   fnc_add_deffn(f, TypeChar);
   return TRUE;
}

/***************************************************************
  
        FUNCTION:       scan_readargs()
  
        DESCRIPTION: This C function reads arguments (variable
            names for an entry added to the FUNCTION-
         SUB lookup table.
  
***************************************************************/

static int scan_readargs(LookupType * f, LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   f->ParameterCount = 0;  /* 0..254, 255 == ... */
   f->ParameterTypes = 0;  /* bit 0 is first parameter */

   /* we should be at begin paren */
   if ( line_skip_char(l, BasicLeftParenChar) == FALSE)
   {
      WARN_SYNTAX_ERROR;
      return FALSE;
   }
   if( line_skip_char(l, BasicRightParenChar) )
   {
      /* end of NO argument list */
      /* FUNCTION ABC() */
      return TRUE;
   }
   if (line_skip_word(l, "...") )
   {
      /* FUNCTION FRED( ... ) */
      if ( line_skip_char( l, BasicRightParenChar))
      {
         f->ParameterCount = 0xFF;  /* VARIANT */
         f->ParameterTypes = 0;
         return TRUE;
      }
      WARN_SYNTAX_ERROR;
      return FALSE;
   }

   /* loop through looking for arguments */
   do
   {
      VariableType *v;
      char TypeChar;
      char varname[BasicNameLengthMax + 1];

      /* presume beginning of argument == variable name */
      if( line_read_varname(l, varname ) == FALSE)
      {
         WARN_SYNTAX_ERROR;
         return FALSE;
      }
      /* determine variable type */
      TypeChar = var_nametype( varname );
      if( TypeChar == BasicNulChar )
      {
         /* variable has no explicit type char */
         TypeChar = line_read_type_declaration(l);
         if( TypeChar == BasicNulChar )
         {
            /* variable has no declared type */
            TypeChar = My->DefaultVariableType[VarTypeIndex(varname[0])];
         }
      }

      /* initialize the variable and add it to local chain */
      v = var_new(varname, TypeChar );
      fslt_addlocalvar(f, v);
      if ( VAR_IS_STRING( v ) )
      {
         f->ParameterTypes |= (1 << f->ParameterCount);
      }
      f->ParameterCount++; /* 0..32, 255 == VARIANT */
      if (f->ParameterCount > 32)
      {
         /* should have been declared VARIANT */
         WARN_SYNTAX_ERROR;
         return FALSE;
      }
   }
   while( line_skip_comma(l) );

   if( line_skip_char(l, BasicRightParenChar) )
   {
      /* end of argument list */
      return TRUE;
   }

   /* FUNCTION ABC( A$, B$, */
   WARN_SYNTAX_ERROR;
   return FALSE;
}



/***************************************************************
  
   FUNCTION:       fslt_addlocalvar()
  
   DESCRIPTION:    This function adds a local variable
         to the FUNCTION-SUB lookup table at
         a specific level.
  
***************************************************************/

int
fslt_addlocalvar(LookupType * f, VariableType * v)
{
   bwx_DEBUG(__FUNCTION__);

   /* find end of local chain */

   if (f->local_variable == NULL)
   {
      f->local_variable = v;
   }
   else
   {
      VariableType *p;
      VariableType *c;

      p = f->local_variable;
      for (c = f->local_variable->next; c != NULL; c = c->next)
      {
         p = c;
      }
      p->next = v;
   }

   v->next = NULL;


   return TRUE;

}



LineType *
bwb_USER_LBL(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   line_skip_eol(l);

   return bwb_zline(l);
}



/***************************************************************
  
        FUNCTION:       bwb_def()
  
        DESCRIPTION:    This C function implements the BASIC
                        DEF statement. 
  
   SYNTAX:     DEF FNname(arg...)] = expression
  
   NOTE:    It is not a strict requirement that the
         function name should begin with "FN".
  
***************************************************************/

LineType *
bwb_DEF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* this line will be executed by fnc_deffn() in bwb_fnc.c */
   line_skip_eol(l);

   return bwb_zline(l);
}



/***************************************************************
  
        FUNCTION:       bwb_call()
  
        DESCRIPTION: This C function implements the BASIC
            CALL subroutine command.
  
   SYNTAX:     CALL subroutine-name( param1, param2 )
  
***************************************************************/

LineType *
bwb_CALL(LineType * l)
{
   VariantType x;        /* no leaks */
   VariantType * X = &x; /* no leaks */

   bwx_DEBUG(__FUNCTION__);
   CLEAR_VARIANT( X );

   /* Call the expression interpreter to evaluate the function */
   if( line_read_expression( l, X ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      goto EXIT;
   }
EXIT:   
   RELEASE( X );
   return bwb_zline(l);
}


/***************************************************************
  
   FUNCTION:   find_label()
  
   DESCRIPTION:   This C function finds a program line that
         begins with the label included in <buffer>.
  
***************************************************************/
LineType *
find_line_number( int number, int IsCache )
{
   /* LABELS are resolved to their line number by the expresson parser */
   if (number > 0)
   {
      LineType *x;
      LineType *StartHere;

      
      /* this is the default */
      StartHere = &My->bwb_start;

#if THE_PRICE_IS_RIGHT  
      if( IsCache )
      {    
         /* 
         This is a brute force search.
         We can reduce the time required by using the various
         existing pointers to start closer to the target.
         Note that these pointers usually have NOTHING to do with the
         line we are searching for.
         We want to use the closest pointer that is BEFORE the target.
         We can NOT use '==' because of multi-statement lines.
         We do NOT require these pointers.  We are only using
         them because they ALREADY exist for other purposes.
         */
         
         LineType *CheckHere;



         /* see if the 'THIS' pointer HAPPENS to be closer */
         CheckHere = My->ThisLine;
         if( CheckHere != NULL )
         {
         
            if( CheckHere->LineFlags & LINE_NUMBERED  && CheckHere->number == number )
            {
               /* exact match */
               return CheckHere;
            }
            if( CheckHere->number > StartHere->number && CheckHere->number < number )
            {
               /* it is closer than where we are now */
               StartHere = CheckHere;
            }
            
         }
         /* see if the 'DATA' pointer HAPPENS to be closer */
         CheckHere = My->data_line;
         if( CheckHere != NULL )
         {
            if( CheckHere->LineFlags & LINE_NUMBERED  && CheckHere->number == number )
            {
               /* exact match */
               return CheckHere;
            }
            if( CheckHere->number > StartHere->number && CheckHere->number < number )
            {
               /* it is closer than where we are now */
               StartHere = CheckHere;
            }
         }
         /* see if the 'ERL' pointer HAPPENS to be closer */
         CheckHere = My->err_line;
         if( CheckHere != NULL )
         {
            if( CheckHere->LineFlags & LINE_NUMBERED  && CheckHere->number == number )
            {
               /* exact match */
               return CheckHere;
            }
            if( CheckHere->number > StartHere->number && CheckHere->number < number )
            {
               /* it is closer than where we are now */
               StartHere = CheckHere;
            }
         }
         /* see if any pointer on the stack HAPPENS to be closer */
         if( My->stack_head != NULL )
         {
            StackType * stack_item;
            for (stack_item = My->stack_head; stack_item != NULL; stack_item = stack_item->next)
            {
               CheckHere = stack_item->line;
               if( CheckHere != NULL )
               {
                  if( CheckHere->LineFlags & LINE_NUMBERED  && CheckHere->number == number )
                  {
                     /* exact match */
                     return CheckHere;
                  }
                  if( CheckHere->number > StartHere->number && CheckHere->number < number )
                  {
                     /* it is closer than where we are now */
                     StartHere = CheckHere;
                  }
               }
            }
         }
      }
#endif
      /* brute force search */
      for (x = StartHere; x != &My->bwb_end; x = x->next)
      {
         if (x->number == number)
         {
            /* FOUND */
            return x;
         }
#if THE_PRICE_IS_RIGHT
         if( IsCache )
         {
            /* see if the 'OtherLine' pointer HAPPENS to be closer */
            LineType *CheckHere;

            CheckHere = x->OtherLine;
            if( CheckHere != NULL )
            {
               if( CheckHere->LineFlags & LINE_NUMBERED  && CheckHere->number == number )
               {
                  /* exact match */
                  return CheckHere;
               }
               if( CheckHere->number > x->number && CheckHere->number < number )
               {
                  /* it is closer than where we are now */
                  x = CheckHere;
               }
            }
         }
#endif
      }
   }
   /* NOT FOUND */
   WARN_UNDEFINED_LINE;
   return NULL;
}





BasicNumberType
VerifyNumeric(BasicNumberType Value)
{
   bwx_DEBUG(__FUNCTION__);
   /* check Value */
   if (isnan(Value))
   {
      /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
      WARN_INTERNAL_ERROR;
      return 0;
   }
   else
   if (isinf(Value))
   {
      /* - Evaluation of an expression results in an overflow
       * (nonfatal, the recommended recovery procedure is to supply
       * machine in- finity with the algebraically correct sign and
       * continue). */
      if (Value < 0)
      {
         Value = -DBL_MAX;
      }
      else
      {
         Value = DBL_MAX;
      }
      if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") )
      {
         /* ERROR */
      }
      /* CONTINUE */
   }
   /* OK */
   return Value;
}

VariableType *
var_chain(VariableType * argv)
{
   /* create a variable chain */
   VariableType *argn;

   bwx_DEBUG(__FUNCTION__);

   if (argv == NULL)
   {
      /* we are the first variable in the chain */
      argn = (VariableType *) CALLOC(1, sizeof(VariableType), "var_chain");
   }
   else
   {
      /* find the end of the chain */
      for (argn = argv; argn->next != NULL; argn = argn->next);
      /* add ourself to the end */
      argn->next = (VariableType *) CALLOC(1, sizeof(VariableType), "var_chain");
      argn = argn->next;
   }
   argn->next = NULL;
   /* return pointer to the variable just created */
   return argn;
}


/* EOF */
