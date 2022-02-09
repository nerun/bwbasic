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
/*---------------------------------------------------------------*/



#include "bwbasic.h"



/* declarations of functions visible to this file only */

static int
cnd_thenels(char *buffer, int position, int *then, int *els);
static int
cnd_tostep(char *buffer, int position, int *to, int *step);
static int
case_eval(struct exp_ese * expression, struct exp_ese * minval, struct exp_ese * maxval);
static int
FindTopLineOnStack(struct bwb_line * l);
static int
FindBottomLineOnStack(struct bwb_line * l);
static struct bwb_line *
FindExitLineOnStack(struct bwb_line * l);
static struct bwb_line *
find_NextTestInCode(struct bwb_line * l);


/*  FIXME: DELETE should remove DEF FN, FUNCTIONS, and SUBROUTINES in the deleted line range. */
/*  FIXME: add Variable Range checking, based upon type characters  $, @, %, &, !, # */
/*  FIXME: check EOF, LOF, LOC, SEEK, such as QB45 */
/*  FIXME: centralize Variable/Function/Command name character checking, so we can configure it with OPTION VERSION */


/* 
--------------------------------------------------------------------------------------------
                               FUNCTION - END FUNCTION
 --------------------------------------------------------------------------------------------
*/


struct bwb_line *
bwb_FUNCTION(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* check current exec level */
   if (CURTASK exsc == 0)
   {
      /* skip over the entire function definition */
      return l->OtherLine->next; /* line after END SUB */
   }
   /* we are being executed via fnc_deffn() */

   /* if this is the first time at this SUB statement, note it */

   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
   {

      bwb_incexec();
      CURTASK         excs[CURTASK exsc].LoopTopLine = l;

      /* find the END SUB statement */

      CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);;

      if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
      {
         /* NOT FOUND */
         bwb_error("FUNCTION without END FUNCTION");
         return bwb_zline(l);
      }
   }
   adv_eos(l->buffer, &(l->position));
   return bwb_zline(l);
}


struct bwb_line *
bwb_EXIT_FUNCTION(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);

   next_line = FindExitLineOnStack(l);
   if (next_line == NULL)
   {
      bwb_error("EXIT FUNCTION without FUNCTION");
      return bwb_zline(l);
   }
   {
      /* EXIT FUNCTION */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
#if 0
   /* set the next line in the exec stack */
   next_line->position = 0;
   bwb_setexec(next_line, 0, EXEC_FUNCTION);
   return next_line;
#endif
}


struct bwb_line *
bwb_END_FUNCTION(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* check integrity of SUB commmand */

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("END FUNCTION without FUNCTION");
      return bwb_zline(l);
   }
   /* decrement the stack */
   bwb_decexec();

   /* and return next from old line */
   CURTASK         excs[CURTASK exsc].line->next->position = 0;
   return CURTASK excs[CURTASK exsc].line->next;
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
  
   SYNTAX:     SUB subroutine-name
           ...
           [ EXIT SUB ]
           ...
           END SUB
  
***************************************************************/

struct bwb_line *
bwb_SUB(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* check current exec level */
   if (CURTASK exsc == 0)
   {
      /* skip over the entire function definition */
      return l->OtherLine->next; /* line after END SUB */
   }
   /* we are being executed via fnc_deffn() */

   /* if this is the first time at this SUB statement, note it */

   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
   {

      bwb_incexec();
      CURTASK         excs[CURTASK exsc].LoopTopLine = l;

      /* find the END SUB statement */

      CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);;

      if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
      {
         /* NOT FOUND */
         bwb_error("SUB without END SUB");
         return bwb_zline(l);
      }
   }
   adv_eos(l->buffer, &(l->position));
   return bwb_zline(l);
}


/***************************************************************
  
        FUNCTION:       bwb_endsub()
  
        DESCRIPTION: This C function implements the BASIC
         END SUB command, ending a subroutine
         definition.  Because the command END
         can have multiple meanings, this function
         should be called from the bwb_xend()
         function, which should be able to identify
         an END SUB command.
  
   SYNTAX:     END SUB
  
***************************************************************/
struct bwb_line *
bwb_EXIT_SUB(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);


   next_line = FindExitLineOnStack(l);
   if (next_line == NULL)
   {
      bwb_error("EXIT SUB without SUB");
      return bwb_zline(l);
   }
   {
      /* EXIT SUB */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
#if 0
   /* set the next line in the exec stack */
   next_line->position = 0;
   bwb_setexec(next_line, 0, EXEC_FUNCTION);
   return next_line;
#endif
}

struct bwb_line *
bwb_END_SUB(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* check integrity of SUB commmand */

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("END SUB without SUB");
      return bwb_zline(l);
   }
   /* decrement the stack */
   bwb_decexec();

   /* and return next from old line */
   CURTASK         excs[CURTASK exsc].line->next->position = 0;
   return CURTASK excs[CURTASK exsc].line->next;
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
  
      SYNTAX:     IF expression THEN line [ELSE line]
  
***************************************************************/
struct bwb_line *
bwb_IF(struct bwb_line * l)
{
   /* classic IF */
   int             then, els;
   struct exp_ese *e;
   int             tpos;
   char            tbuf[BasicStringLengthMax + 1];
   int             Value;

   bwx_DEBUG(__FUNCTION__);


   /* Call bwb_exp() to evaluate the condition. This should return with
    * position set to the "THEN" statement */

   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   Value = exp_getival(e);


   /* test for "THEN" and "ELSE" statements */

   cnd_thenels(l->buffer, l->position, &then, &els);


   /* evaluate and execute */

   if (Value != 0)
   {
      /* expression is TRUE */
      if (then == FALSE)
      {
         /* syntac error */
         bwb_error(err_syntax);
         return bwb_zline(l);
      }
      else
      {
         /* check for THEN followed by literal line number */
         struct bwb_line *x;

         tpos = then + strlen("THEN") + 1;
         adv_element(l->buffer, &tpos, tbuf);

         /* check for target label */
         x = find_label(tbuf);
         if (x != NULL)
         {
            adv_eos(l->buffer, &(l->position));
            x->position = 0;
            return x;
         }
         /* syntac error */
         bwb_error(err_syntax);
         return bwb_zline(l);
      }

   }
   else
   {
      /* expression is FALSE */

      if (els == FALSE)
      {
         /* optional */
         adv_eos(l->buffer, &(l->position));
         return bwb_zline(l);
      }
      else
      {
         /* check for ELSE followed by literal line number */
         struct bwb_line *x;

         tpos = els + strlen("ELSE") + 1;
         adv_element(l->buffer, &tpos, tbuf);

         /* check for target label */
         x = find_label(tbuf);
         if (x != NULL)
         {
            adv_eos(l->buffer, &(l->position));
            x->position = 0;
            return x;
         }
         /* syntac error */
         bwb_error(err_syntax);
         return bwb_zline(l);
      }
   }

   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       cnd_thenelse()
  
        DESCRIPTION:    This function searches through the
                        <buffer> beginning at point <position>
                        and attempts to find positions of THEN
                        and ELSE statements.
  
***************************************************************/

static int
cnd_thenels(char *buffer, int position, int *then, int *els)
{
   int             loop, t_pos, b_pos, p_word;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   /* set then and els to 0 initially */

   *then = *els = 0;

   /* loop to find words */

   p_word = b_pos = position;
   t_pos = 0;
   tbuf[0] = '\0';
   loop = TRUE;
   while (loop == TRUE)
   {
      char            c;
      c = buffer[b_pos];

      if (c == '\0' || c == OptionCommentChar || c == ' ')
      {

         if (strncasecmp(tbuf, "THEN", (size_t) strlen("THEN")) == 0)
         {
            *then = p_word;
         }
         else
         if (strncasecmp(tbuf, "GOTO", (size_t) strlen("GOTO")) == 0)
         {

            *then = p_word;
         }
         else
         if (strncasecmp(tbuf, "ELSE", (size_t) strlen("ELSE")) == 0)
         {

            *els = p_word;
         }
         /* check for end of the line */

         if (c == '\0' || c == OptionCommentChar)
         {
            return TRUE;
         }
         ++b_pos;
         p_word = b_pos;
         t_pos = 0;
         tbuf[0] = '\0';
      }
      else
      {
         tbuf[t_pos] = c;
         ++b_pos;
         ++t_pos;
         tbuf[t_pos] = '\0';
      }

   }

   return FALSE;

}


/***************************************************************
  
        FUNCTION:       bwb_IF_THEN()
  
        DESCRIPTION:    This function handles the BASIC IF
                        statement, structured flavor.
  
      SYNTAX:     IF expression THEN
                      ...
                      END IF
  
***************************************************************/
struct bwb_line *
bwb_IF_THEN(struct bwb_line * l)
{
   /* structured IF */
   struct exp_ese *e;
   struct bwb_line *else_line;
   struct bwb_line *endif_line;
   int             Value;

   bwx_DEBUG(__FUNCTION__);


   /* if this is the first time at this IF statement, note it */


#if 0
   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
#endif
      if (FindTopLineOnStack(l) == FALSE)
      {

         bwb_incexec();
         CURTASK         excs[CURTASK exsc].LoopTopLine = l;

         /* find the LOOP statement */

         CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);;

         if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
         {
            /* NOT FOUND */
            bwb_error("IF without END IF");
            return bwb_zline(l);
         }
      }
   /* Call bwb_exp() to evaluate the condition. This should return with
    * position set to the "THEN" statement */

/* FIXME: extract the expression before calling bwb_exp() - no COMMANDS should be there */
   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   Value = exp_getival(e);


   /* evaluate the expression */

   if (Value != 0)
   {
      CURTASK         excs[CURTASK exsc].code = EXEC_IFTRUE;   /* IF has been processed */
      /* fall thru to execute this code block */

      adv_eos(l->buffer, &(l->position));
      return bwb_zline(l);
   }
   CURTASK         excs[CURTASK exsc].code = EXEC_IFFALSE;  /* IF has NOT been
                         * processed */

   else_line = find_NextTestInCode(l);
   if (else_line != NULL)
   {
      else_line->position = 0;
      return else_line;
   }
   endif_line = CURTASK excs[CURTASK exsc].LoopBottomLine;
   endif_line->position = 0;
   return endif_line;

}


/***************************************************************
  
        FUNCTION:       bwb_else()
  
        DESCRIPTION:    This function handles the BASIC ELSE
                        statement.
  
      SYNTAX:     ELSE
  
***************************************************************/

struct bwb_line *
bwb_ELSE(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);


   /* there are two legitamate ways to get here: 1. a fall thru from an
    * upper block, code == EXEC_IFTRUE we should jump to the END IF 2. a
    * jump from from an upper block, code == EXEC_IFFALSE we should
    * execute our code block */

   next_line = FindExitLineOnStack(l); /* END IF */
   if (next_line == NULL)
   {
      bwb_error("ELSE without IF");
      return bwb_zline(l);
   }
   switch (CURTASK excs[CURTASK exsc].code)
   {
   case EXEC_IFTRUE: /* IF has been processed */
      /* jump to the END IF */
      next_line->position = 0;
      return next_line;
      break;
   case EXEC_IFFALSE:
      CURTASK excs[CURTASK exsc].code = EXEC_IFTRUE;  /* IF has been processed */
      /* fall thru to execute this code block */
      break;
   default:
      /* the BASIC program has jumped into the middle of a
       * structured command */
      bwb_error("ELSE without IF");
      break;
   }
   return bwb_zline(l);
}




/***************************************************************
  
        FUNCTION:       bwb_elseif()
  
        DESCRIPTION:    This function handles the BASIC ELSEIF
                        statement.
  
   SYNTAX:     ELSEIF
  
***************************************************************/

struct bwb_line *
bwb_ELSEIF(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);

   /* there are two legitamate ways to get here: 1. a fall thru from an
    * upper block, code == EXEC_IFTRUE we should jump to the END IF 2. a
    * jump from from an upper block, code == EXEC_IFFALSE we should
    * execute our code block */

   next_line = FindExitLineOnStack(l); /* END IF */
   if (next_line == NULL)
   {
      bwb_error("ELSEIF without IF");
      return bwb_zline(l);
   }
   switch (CURTASK excs[CURTASK exsc].code)
   {
   case EXEC_IFTRUE: /* IF has been processed */
      /* jump to the END IF */
      next_line->position = 0;
      return next_line;
      break;
   case EXEC_IFFALSE:
      /* execute our code block (maybe) */
      {
         struct bwb_line *else_line;
         struct exp_ese *e;
         /* Call bwb_exp() to evaluate the condition. This
          * should return with position set to the "THEN"
          * statement */

         e = bwb_exp(l->buffer, FALSE, &(l->position));
         if (ERROR_PENDING)
         {
            return bwb_zline(l);
         }
         if (exp_getival(e) != FALSE)
         {
            /* condition is TRUE: proceed to the next
             * line */
            CURTASK         excs[CURTASK exsc].code = EXEC_IFTRUE;   /* IF has been processed */
            /* fall thru to execute this code block */

            adv_eos(l->buffer, &(l->position));
            return bwb_zline(l);

         }
         /* condition is FALSE */

         /* proceed to next ELSE line if there is one */
         else_line = find_NextTestInCode(l); /* ELSEIF or ELSE */
         if (else_line != NULL)
         {
            else_line->position = 0;
            return else_line;
         }
         /* no more ELSExx lines */
         /* jump to the END IF */
         next_line->position = 0;
         return next_line;
      }
      break;
   default:
      /* the BASIC program has jumped into the middle of a
       * structured command */
      bwb_error("ELSEIF without IF");
      break;
   }
   return bwb_zline(l);
}



/***************************************************************
  
        FUNCTION:       bwb_endif()
  
        DESCRIPTION:    This function handles the BASIC END IF
                        statement.
  
   SYNTAX:     END IF
  
***************************************************************/

struct bwb_line *
bwb_END_IF(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);


   /* check integrity of IF commmand */

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("END IF without IF");
      return bwb_zline(l);
   }
   switch (CURTASK excs[CURTASK exsc].code)
   {
   case EXEC_IFTRUE: /* IF has been processed */
      break;
   case EXEC_IFFALSE:
      break;
   default:
      /* the BASIC program has jumped into the middle of a
       * structured command */
      bwb_error("ENDIF without IF");
   }

   /* remove IF from the stack (NEW) */
   bwb_decexec();
   bwb_setexec(l->next, 0, CURTASK excs[CURTASK exsc].code);

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
  
   SYNTAX:     SELECT CASE expression
  
***************************************************************/

struct bwb_line *
bwb_SELECT(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   bwb_error("SELECT without CASE");
   return bwb_zline(l);
}


struct bwb_line *
bwb_SELECT_CASE(struct bwb_line * l)
{
   struct exp_ese *e;

   bwx_DEBUG(__FUNCTION__);


   /* if this is the first time at this SELECT CASE statement, note it */

#if 0
   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
#endif
      if (FindTopLineOnStack(l) == FALSE)
      {

         bwb_incexec();
         CURTASK         excs[CURTASK exsc].LoopTopLine = l;

         /* find the LOOP statement */

         CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);

         if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
         {
            /* NOT FOUND */
            bwb_error("SELECT CASE without END SELECT");
            return bwb_zline(l);
         }
      }
   /* increment the level and set to EXEC_SELFALSE */

   CURTASK         excs[CURTASK exsc].code = EXEC_SELFALSE;


   /* evaluate the expression at this level */

   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (e->type == STRING)
   {
      CURTASK         excs[CURTASK exsc].expression.type = STRING;
      str_btob(&(CURTASK excs[CURTASK exsc].expression.sval),
          &(e->sval));
   }
   else
   {
      CURTASK         excs[CURTASK exsc].expression.type = NUMBER;
      CURTASK         excs[CURTASK exsc].expression.nval
      = exp_getnval(e);
   }

   /* return */

   adv_eos(l->buffer, &(l->position));

   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_case()
  
        DESCRIPTION:    This C function handles the BASIC CASE
                        statement.
  
   SYNTAX:     CASE constant | IF partial-expression | ELSE
  
***************************************************************/

struct bwb_line *
bwb_CASE_ELSE(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);

   /* there are two legitamate ways to get here: 1. a fall thru from an
    * upper block, code == EXEC_SELTRUE we should jump to the END SELECT
    * 2. a jump from from an upper block, code == EXEC_SELFALSE we
    * should execute our code block */

   next_line = FindExitLineOnStack(l); /* END SELECT */
   if (next_line == NULL)
   {
      bwb_error("CASE ELSE without SELECT CASE");
      return bwb_zline(l);
   }
   switch (CURTASK excs[CURTASK exsc].code)
   {
   case EXEC_SELTRUE:   /* SELECT has been processed */
      /* jump to the END SELECT */
      next_line->position = 0;
      return next_line;
      break;
   case EXEC_SELFALSE:
      CURTASK excs[CURTASK exsc].code = EXEC_SELTRUE; /* SELECT has been
                         * processed */
      /* fall thru to execute this code block */
      break;
   default:
      /* the BASIC program has jumped into the middle of a
       * structured command */
      bwb_error("CASE ELSE without SELECT CASE");
      break;
   }
   return bwb_zline(l);
}



struct bwb_line *
bwb_CASE(struct bwb_line * l) /* TODO - FIXME */
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);


   /* there are two legitamate ways to get here: 1. a fall thru from an
    * upper block, code == EXEC_SELTRUE we should jump to the END SELECT
    * 2. a jump from from an upper block, code == EXEC_SELFALSE we
    * should execute our code block */

   next_line = FindExitLineOnStack(l); /* END SELECT */
   if (next_line == NULL)
   {
      bwb_error("CASE without SELECT CASE");
      return bwb_zline(l);
   }
   switch (CURTASK excs[CURTASK exsc].code)
   {
   case EXEC_SELTRUE:   /* SELECT has been processed */
      /* jump to the END SELECT */
      next_line->position = 0;
      return next_line;
      break;
   case EXEC_SELFALSE:
      /* fall thru to execute this code block (maybe) */
      {
         struct exp_ese  minvalue;
         struct exp_ese *minval;
         struct bwb_line *case_line;
         int             IsExpression;

         IsExpression = FALSE;

         minval = bwb_exp(l->buffer, FALSE, &(l->position));
         if (ERROR_PENDING)
         {
            return bwb_zline(l);
         }
         memcpy(&minvalue, minval, sizeof(struct exp_ese));
         minval = &minvalue;

         /* check for string value */

         if (minvalue.type == STRING)
         {  /* STRING */
            bstring        *a;
            bstring        *b;
            a = &(CURTASK excs[CURTASK exsc].expression.sval);
            b = &(minvalue.sval);
            if (str_cmp(a, b) == 0)
            {
               IsExpression = TRUE;
            }
         }
         /* STRING */
         else
         {  /* NUMBER */
            struct exp_ese *maxval;
            maxval = minval;

            /* not a string; advance */

            adv_ws(l->buffer, &(l->position));

            /* check for TO */

            if (is_eol(l->buffer, &(l->position)) != TRUE)
            {
               char            tbuf[BasicStringLengthMax + 1];

               /* find the TO statement */

               adv_element(l->buffer, &(l->position), tbuf);
               if (strcasecmp(tbuf, "TO") != 0)
               {
                  sprintf(bwb_ebuf, "CASE has inexplicable code following expression");
                  bwb_error(bwb_ebuf);
                  adv_eos(l->buffer, &(l->position));
                  return bwb_zline(l);
               }
               /* now evaluate the MAX expression */

               maxval = bwb_exp(l->buffer, FALSE, &(l->position));
               if (ERROR_PENDING)
               {
                  return bwb_zline(l);
               }
            }
            /* evaluate the expression */

            if (case_eval(&(CURTASK excs[CURTASK exsc].expression), minval, maxval) == TRUE)
            {
               IsExpression = TRUE;
            }
         }  /* NUMBER */

         if (IsExpression == TRUE)
         {
            /* condition is TRUE: proceed to the next
             * line */
            CURTASK         excs[CURTASK exsc].code = EXEC_SELTRUE;  /* SELECT has been
                                  * processed */
            /* fall thru to execute this code block */

            adv_eos(l->buffer, &(l->position));
            return bwb_zline(l);
         }
         /* evaluation returns a FALSE value; find next CASExx
          * statement */
         case_line = find_NextTestInCode(l); /* CASE or CASE IF or
                         * CASE ELSE */
         if (case_line != NULL)
         {
            case_line->position = 0;
            return case_line;
         }
         /* no more CASExx lines */
         /* jump to the END SELECT */
         next_line->position = 0;
         return next_line;
      }
      break;
   default:
      /* the BASIC program has jumped into the middle of a
       * structured command */
      bwb_error("CASE without SELECT CASE");
      break;
   }
   return bwb_zline(l);
}




/***************************************************************
  
        FUNCTION:       bwb_caseif()
  
        DESCRIPTION:    This C function handles the BASIC CASE IF
                        statement.
  
***************************************************************/

struct bwb_line *
bwb_CASE_IS(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   return bwb_CASE_IF(l);
}

struct bwb_line *
bwb_CASE_IF(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);


   /* there are two legitamate ways to get here: 1. a fall thru from an
    * upper block, code == EXEC_SELTRUE we should jump to the END SELECT
    * 2. a jump from from an upper block, code == EXEC_SELFALSE we
    * should execute our code block */

   next_line = FindExitLineOnStack(l); /* END SELECT */
   if (next_line == NULL)
   {
      bwb_error("CASE IF without SELECT CASE");
      return bwb_zline(l);
   }
   switch (CURTASK excs[CURTASK exsc].code)
   {
   case EXEC_SELTRUE:   /* IF has been processed */
      /* jump to the END IF */
      next_line->position = 0;
      return next_line;
      break;
   case EXEC_SELFALSE:
      /* execute our code block (maybe) */
      {
         char            tbuf[BasicStringLengthMax + 1];
         int             position;
         struct exp_ese *r;
         struct bwb_line *case_line;

         if (CURTASK excs[CURTASK exsc].expression.type == NUMBER)
         {
            sprintf(tbuf, BasicNumberPrintFormat " %s",
             CURTASK excs[CURTASK exsc].expression.nval,
               &(l->buffer[l->position]));
         }
         else
         {
            bwb_error(err_mismatch);
            adv_eos(l->buffer, &(l->position));
            return bwb_zline(l);
         }

         position = 0;
         r = bwb_exp(tbuf, FALSE, &position);
         if (ERROR_PENDING)
         {
            return bwb_zline(l);
         }
         if (r->nval == TRUE)
         {
            /* condition is TRUE: proceed to the next
             * line */
            CURTASK         excs[CURTASK exsc].code = EXEC_SELTRUE;  /* SELECT has been
                                  * processed */
            /* fall thru to execute this code block */

            adv_eos(l->buffer, &(l->position));
            return bwb_zline(l);
         }
         /* condition is FALSE */

         /* proceed to next CASE line if there is one */
         case_line = find_NextTestInCode(l); /* CASE IF or CASE ELSE */
         if (case_line != NULL)
         {
            case_line->position = 0;
            return case_line;
         }
         /* no more CASExx lines */
         /* jump to the END SELECT */
         next_line->position = 0;
         return next_line;
      }
      break;
   default:
      /* the BASIC program has jumped into the middle of a
       * structured command */
      bwb_error("CASE IF without SELECT CASE");
      break;
   }
   return bwb_zline(l);
}



/***************************************************************
  
        FUNCTION:       case_eval()
  
        DESCRIPTION:    This function evaluates a case statement
         by comparing minimum and maximum values
         with a set expression. It returns either
         TRUE or FALSE
  
***************************************************************/

static int
case_eval(struct exp_ese * expression, struct exp_ese * minval,
     struct exp_ese * maxval)
{
   bwx_DEBUG(__FUNCTION__);

   /* string value */

   if (expression->type == STRING)
   {
      bwb_error(err_mismatch);
      return FALSE;
   }
   /* numerical value */


   if ((expression->nval >= minval->nval)
       && (expression->nval <= maxval->nval))
   {
      return TRUE;
   }
   return FALSE;

}

/***************************************************************
  
        FUNCTION:       bwb_endselect()
  
        DESCRIPTION:    This function handles the BASIC END
                        SELECT statement.
  
   SYNTAX:     END SELECT
  
***************************************************************/

struct bwb_line *
bwb_END_SELECT(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

   /* check integrity of SELECT CASE commmand */

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("END SELECT without SELECT CASE");
      return bwb_zline(l);
   }
   switch (CURTASK excs[CURTASK exsc].code)
   {
   case EXEC_SELTRUE:   /* SELECT has been processed */
      break;
   case EXEC_SELFALSE:
      break;
   default:
      /* the BASIC program has jumped into the middle of a
       * structured command */
      bwb_error("END SELECT without SELECT CASE");
   }


   /* remove SELECT from the stack */
   bwb_decexec();
   bwb_setexec(l->next, 0, CURTASK excs[CURTASK exsc].code);


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
         DO statement, when DO is not followed by
         an argument.  It is called by bwb_do() in
         bwb_cmd.c.
  
   SYNTAX:     DO ' forever
  
***************************************************************/

struct bwb_line *
bwb_DO(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* if this is the first time at this DO statement, note it */

#if 0
   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
#endif
      if (FindTopLineOnStack(l) == FALSE)
      {

         bwb_incexec();
         CURTASK         excs[CURTASK exsc].LoopTopLine = l;

         /* find the LOOP statement */

         CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);;

         if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
         {
            /* NOT FOUND */
            bwb_error("DO without LOOP");
            return bwb_zline(l);
         }
      }
   bwb_setexec(l, l->position, EXEC_DO);
   return bwb_zline(l);
}

/***************************************************************
  
   FUNCTION:       bwb_DO_UNTIL()
  
   DESCRIPTION:    This C function implements the ANSI BASIC
         DO UNTIL statement, when DO is not followed by
         an argument.  It is called by bwb_do() in
         bwb_cmd.c.
  
   SYNTAX:     DO  UNTIL  expression ' exit when X <> 0
  
***************************************************************/

struct bwb_line *
bwb_DO_UNTIL(struct bwb_line * l)
{
   struct exp_ese *e;

   bwx_DEBUG(__FUNCTION__);
   /* if this is the first time at this WHILE statement, note it */

#if 0
   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
#endif
      if (FindTopLineOnStack(l) == FALSE)
      {

         bwb_incexec();
         CURTASK         excs[CURTASK exsc].LoopTopLine = l;

         /* find the UEND statement (or LOOP statement) */
         CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);


         if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
         {
            /* NOT FOUND */
            bwb_error("DO without LOOP");
            return bwb_zline(l);
         }
      }
   /*----------------------------------------------------*/
   /* Expression evaluation was at the top of bwb_while, */
   /* and the init portion was performed only if TRUE.   */
   /* The init routine should be performed regardless of */
   /* expression value, else a segmentation fault can    */
   /* occur! (JBV)                                       */
   /*----------------------------------------------------*/

   /* call bwb_exp() to interpret the expression */

   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (exp_getival(e) != FALSE)
   {
      /* EXIT DO */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
   /* DO UNTIL ... */
   bwb_setexec(l, l->position, EXEC_DO);
   return bwb_zline(l);

}

/***************************************************************
  
   FUNCTION:       bwb_DO_WHILE()
  
   DESCRIPTION:    This C function implements the ANSI BASIC
         DO statement, when DO is not followed by
         an argument.  It is called by bwb_do() in
         bwb_cmd.c.
  
   SYNTAX:     DO WHILE expression ' exit when X = 0
  
***************************************************************/

struct bwb_line *
bwb_DO_WHILE(struct bwb_line * l)
{
   struct exp_ese *e;

   bwx_DEBUG(__FUNCTION__);
   /* if this is the first time at this WHILE statement, note it */

#if 0
   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
#endif
      if (FindTopLineOnStack(l) == FALSE)
      {

         bwb_incexec();
         CURTASK         excs[CURTASK exsc].LoopTopLine = l;

         /* find the LOOP statement */
         CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);

         if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
         {
            /* NOT FOUND */
            bwb_error("DO without LOOP");
            return bwb_zline(l);
         }
      }
   /*----------------------------------------------------*/
   /* Expression evaluation was at the top of bwb_while, */
   /* and the init portion was performed only if TRUE.   */
   /* The init routine should be performed regardless of */
   /* expression value, else a segmentation fault can    */
   /* occur! (JBV)                                       */
   /*----------------------------------------------------*/

   /* call bwb_exp() to interpret the expression */

   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (exp_getival(e) == FALSE)
   {
      /* EXIT DO */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
   /* DO WHILE ... */
   bwb_setexec(l, l->position, EXEC_DO);
   return bwb_zline(l);


}

/***************************************************************
  
   FUNCTION:       bwb_EXIT_DO()
  
   DESCRIPTION:    This function handles the BASIC EXIT
         DO statement.  This is a structured
         programming command compatible with ANSI
         BASIC. It is called from the bwb_exit()
         subroutine.
  
***************************************************************/

struct bwb_line *
bwb_EXIT_DO(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);


   next_line = FindExitLineOnStack(l);
   if (next_line == NULL)
   {
      bwb_error("EXIT DO without DO");
      return bwb_zline(l);
   }
   {
      /* EXIT DO */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
#if 0   
   next_line = next_line->next;

   /* set the next line in the exec stack */
   next_line->position = 0;
   bwb_setexec(next_line, 0, EXEC_DO);
   return next_line;
#endif
}


/***************************************************************
  
        FUNCTION:       bwb_LOOP()
  
   DESCRIPTION:    This C function implements the ANSI BASIC
         LOOP statement.
  
   SYNTAX:     LOOP ' forever
  
***************************************************************/

struct bwb_line *
bwb_LOOP(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

   /* check integrity of DO loop */

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("LOOP without DO");
      return bwb_zline(l);
   }
   /* reset to the top of the current DO loop */


   CURTASK         excs[CURTASK exsc].LoopTopLine->position = 0;
   bwb_setexec(CURTASK excs[CURTASK exsc].LoopTopLine, 0, EXEC_DO);

   return CURTASK excs[CURTASK exsc].LoopTopLine;

}



/***************************************************************
  
   FUNCTION:       bwb_LOOP_UNTIL()
  
   DESCRIPTION:    This C function implements the ANSI BASIC
         LOOP UNTIL statement and is called by
         bwb_loop().
  
   SYNTAX:     LOOP UNTIL expression ' exit when X <> 0
  
***************************************************************/

struct bwb_line *
bwb_LOOP_UNTIL(struct bwb_line * l)
{
   struct exp_ese *e;

   bwx_DEBUG(__FUNCTION__);

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("LOOP without DO");
      return bwb_zline(l);
   }
   /* call bwb_exp() to interpret the expression */

   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (exp_getival(e) != FALSE)
   {
      /* EXIT DO */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
   /* loop around to DO again */


   CURTASK         excs[CURTASK exsc].LoopTopLine->position = 0;
   bwb_setexec(CURTASK excs[CURTASK exsc].LoopTopLine, 0, EXEC_DO);

   return CURTASK excs[CURTASK exsc].LoopTopLine;


}

/***************************************************************
  
   FUNCTION:       bwb_LOOP_WHILE()
  
   DESCRIPTION:    This C function implements the BASIC
         LOOP WHILE statement and is called by
         bwb_loop().
  
   SYNTAX:     LOOP WHILE expression ' exit when X = 0
  
***************************************************************/

struct bwb_line *
bwb_LOOP_WHILE(struct bwb_line * l)
{
   struct exp_ese *e;

   bwx_DEBUG(__FUNCTION__);

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("LOOP without DO");
      return bwb_zline(l);
   }
   /* call bwb_exp() to interpret the expression */

   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (exp_getival(e) == FALSE)
   {
      /* EXIT DO */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
   /* execute DO-LOOP again */


   CURTASK         excs[CURTASK exsc].LoopTopLine->position = 0;
   bwb_setexec(CURTASK excs[CURTASK exsc].LoopTopLine, 0, EXEC_DO);

   return CURTASK excs[CURTASK exsc].LoopTopLine;

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
  
   SYNTAX:     WHILE expression
                        ...
                        WEND
   SYNTAX:     WHILE expression ' exit when X = 0
  
  
***************************************************************/
struct bwb_line *
bwb_WHILE(struct bwb_line * l)
{
   struct exp_ese *e;

   bwx_DEBUG(__FUNCTION__);
   /* if this is the first time at this WHILE statement, note it */

#if 0
   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
#endif
      if (FindTopLineOnStack(l) == FALSE)
      {

         bwb_incexec();
         CURTASK         excs[CURTASK exsc].LoopTopLine = l;

         /* find the LOOP statement */
         CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);

         if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
         {
            /* NOT FOUND */
            bwb_error("WHILE without WEND");
            return bwb_zline(l);
         }
      }
   /*----------------------------------------------------*/
   /* Expression evaluation was at the top of bwb_while, */
   /* and the init portion was performed only if TRUE.   */
   /* The init routine should be performed regardless of */
   /* expression value, else a segmentation fault can    */
   /* occur! (JBV)                                       */
   /*----------------------------------------------------*/

   /* call bwb_exp() to interpret the expression */

   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (exp_getival(e) == FALSE)
   {
      /* EXIT WHILE */
      struct bwb_line *r;
      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
   /* WHILE ... */
   bwb_setexec(l, l->position, EXEC_WHILE);
   return bwb_zline(l);

}

/***************************************************************
  
   FUNCTION:       bwb_exitwhile()
  
   DESCRIPTION:    This function handles the BASIC EXIT
         WHILE statement.  This is a structured
         programming command compatible with ANSI
         BASIC. It is called from the bwb_exit()
         subroutine.
  
***************************************************************/

struct bwb_line *
bwb_EXIT_WHILE(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);

   /* Check the integrity of the WHILE statement */
   next_line = FindExitLineOnStack(l);
   if (next_line == NULL)
   {
      bwb_error("EXIT WHILE without WHILE");
      return bwb_zline(l);
   }
   {
      /* EXIT WHILE */
      struct bwb_line *r;
      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
#if 0   
   next_line = next_line->next;

   /* set the next line in the exec stack */
   next_line->position = 0;
   bwb_setexec(next_line, 0, EXEC_WHILE);
   return next_line;
#endif
}


/***************************************************************
  
        FUNCTION:       bwb_wend()
  
        DESCRIPTION:    This function handles the BASIC WEND
         statement and the LOOP statement ending
         a DO WHILE loop.
  
   SYNTAX:     WEND
         LOOP
  
***************************************************************/

struct bwb_line *
bwb_WEND(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);
   /* check integrity of WHILE loop */

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("WEND without WHILE");
      return bwb_zline(l);
   }
   /* reset to the top of the current WHILE loop */


   CURTASK         excs[CURTASK exsc].LoopTopLine->position = 0;
   bwb_setexec(CURTASK excs[CURTASK exsc].LoopTopLine, 0, EXEC_WHILE);

   return CURTASK excs[CURTASK exsc].LoopTopLine;

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
  
   SYNTAX:     UNTIL expression ' exit when X <> 0
                          ...
                        UEND
                       
  
***************************************************************/
struct bwb_line *
bwb_UNTIL(struct bwb_line * l)
{
   struct exp_ese *e;

   bwx_DEBUG(__FUNCTION__);
   /* if this is the first time at this WHILE statement, note it */

#if 0
   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
#endif
      if (FindTopLineOnStack(l) == FALSE)
      {

         bwb_incexec();
         CURTASK         excs[CURTASK exsc].LoopTopLine = l;

         /* find the UEND statement (or LOOP statement) */
         CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);


         if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
         {
            /* NOT FOUND */
            bwb_error("UNTIL without UEND");
            return bwb_zline(l);
         }
      }
   /*----------------------------------------------------*/
   /* Expression evaluation was at the top of bwb_while, */
   /* and the init portion was performed only if TRUE.   */
   /* The init routine should be performed regardless of */
   /* expression value, else a segmentation fault can    */
   /* occur! (JBV)                                       */
   /*----------------------------------------------------*/

   /* call bwb_exp() to interpret the expression */

   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (exp_getival(e) != FALSE)
   {
      /* EXIT UNTIL */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
   /* UNTIL ... */
   bwb_setexec(l, l->position, EXEC_UNTIL);
   return bwb_zline(l);

}


/***************************************************************
  
   FUNCTION:       bwb_exituntil()
  
   DESCRIPTION:    This function handles the BASIC EXIT
         UNTIL statement.  This is a structured
         programming command compatible with ANSI
         BASIC. It is called from the bwb_exit()
         subroutine.
  
***************************************************************/

struct bwb_line *
bwb_EXIT_UNTIL(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);

   /* Check the integrity of the UNTIL statement */

   next_line = FindExitLineOnStack(l);
   if (next_line == NULL)
   {
      bwb_error("EXIT UNTIL without UNTIL");
      return bwb_zline(l);
   }
   {
      /* EXIT UNTIL */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
#if 0
   next_line = next_line->next;

   /* set the next line in the exec stack */
   next_line->position = 0;
   bwb_setexec(next_line, 0, EXEC_UNTIL);
   return next_line;
#endif
}


/***************************************************************
  
        FUNCTION:       bwb_uend()
  
        DESCRIPTION:    This function handles the BASIC UEND
         statement and the LOOP statement ending
         a DO UNTIL loop.
  
   SYNTAX:     UEND
  
***************************************************************/

struct bwb_line *
bwb_UEND(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);
   /* check integrity of UNTIL loop */

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("UEND without UNTIL");
      return bwb_zline(l);
   }
   /* reset to the top of the current UNTIL loop */

   CURTASK         excs[CURTASK exsc].LoopTopLine->position = 0;
   bwb_setexec(CURTASK excs[CURTASK exsc].LoopTopLine, 0, EXEC_UNTIL);

   return CURTASK excs[CURTASK exsc].LoopTopLine;

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

struct bwb_line *
bwb_FOR(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

#if 0
   if (CURTASK excs[CURTASK exsc].LoopTopLine != l)
#endif
      if (FindTopLineOnStack(l) == FALSE)
      {

         bwb_incexec();
         CURTASK         excs[CURTASK exsc].LoopTopLine = l;

         /* find the NEXT statement */

         CURTASK         excs[CURTASK exsc].LoopBottomLine = find_BottomLineInCode(l);

         if (CURTASK excs[CURTASK exsc].LoopBottomLine == NULL)
         {
            /* NOT FOUND */
            bwb_error("FOR without NEXT");
            return bwb_zline(l);
         }
      }
   /* -------------------------------------------------------------------
    * ------------------------------- */

   if (TRUE)
   {
      register int    n;
      int             e, loop;
      int             to, step, p;
      BasicNumberType for_step;  /* STEP value for FOR */
      BasicNumberType for_target;   /* target value for FOR */
      struct exp_ese *exp;
      struct bwb_variable *v;
      char            tbuf[BasicStringLengthMax + 1];
      int             VarStart;  /* position in l->buffer */
      int             VarLength;
      int             InitStart;
      int             InitLength;
      int             TermStart;
      int             TermLength;
      int             StepStart;
      int             StepLength;
      BasicNumberType Value;



      /* VarStart = 0; */
      /* VarLength = 0; */
      InitStart = 0;
      InitLength = 0;
      TermStart = 0;
      TermLength = 0;
      StepStart = 0;
      StepLength = 0;

      /* get the variable name */
      VarStart = l->position;
      exp_getvfname(&(l->buffer[l->position]), tbuf);
      VarLength = strlen(tbuf);
      l->position += VarLength;


      /* at this point one should find an equals sign ('=') */

      adv_ws(l->buffer, &(l->position));

      if (l->buffer[l->position] != '=')
      {
         sprintf(bwb_ebuf, "in bwb_for(): failed to find equals sign, buf <%s>",
            &(l->buffer[l->position]));
         bwb_error(bwb_ebuf);
         return bwb_zline(l);
      }
      else
      {
         ++(l->position);
      }

      /* Find the TO and STEP statements */

      cnd_tostep(l->buffer, l->position, &to, &step);

      /* if there is no TO statement, then an error has ocurred */

      if (to < 1)
      {
         bwb_error("FOR without TO");
         return bwb_zline(l);
      }
      /* copy initial value to buffer and evaluate it */

      InitStart = l->position;
      tbuf[0] = '\0';
      p = 0;
      for (n = l->position; n < to; ++n)
      {
         tbuf[p] = l->buffer[n];
         ++p;
         ++l->position;
         tbuf[p] = '\0';
      }
      InitLength = p;

      /* copy target value to small buffer and evaluate it */

      tbuf[0] = '\0';
      p = 0;
      l->position = to + 2;
      if (step < 1)
      {
         e = strlen(l->buffer);
      }
      else
      {
         e = step - 1;
      }

      loop = TRUE;
      n = l->position;


      TermStart = l->position;


      while (loop == TRUE)
      {
         tbuf[p] = l->buffer[n];
         ++p;
         ++l->position;
         tbuf[p] = '\0';

         if (n >= e)
         {
            loop = FALSE;
         }
         ++n;

         if (l->buffer[n] == '\0' || l->buffer[n] == OptionCommentChar)
         {
            loop = FALSE;
         }
      }
      TermLength = p;


      /* If there is a STEP statement, copy it to a buffer and
       * evaluate it */

      if (step > 1)
      {
         tbuf[0] = '\0';
         p = 0;
         l->position = step + 4;

         StepStart = l->position;


         for (n = l->position; n < (int) strlen(l->buffer); ++n)
         {
            tbuf[p] = l->buffer[n];
            ++p;
            ++l->position;
            tbuf[p] = '\0';
         }
         StepLength = p;
      }
      if (OptionFlags & OPTION_BUGS_ON)
      {
         /* Create Variable */
         if (VarLength < 1)
         {
            bwb_error(err_syntax);
            return bwb_zline(l);
         }
         strncpy(tbuf, &l->buffer[VarStart], VarLength);
         tbuf[VarLength] = '\0';
         v = var_find(tbuf);

         /* Init */
         if (InitLength < 1)
         {
            bwb_error(err_syntax);
            return bwb_zline(l);
         }
         strncpy(tbuf, &l->buffer[InitStart], InitLength);
         tbuf[InitLength] = '\0';
         p = 0;
         exp = bwb_exp(tbuf, FALSE, &p);
         if (ERROR_PENDING)
         {
            return bwb_zline(l);
         }
         Value = exp_getnval(exp);

         /* Assign Variable */
         var_setnval(v, Value);

         /* Limit */
         if (TermLength < 1)
         {
            bwb_error(err_syntax);
            return bwb_zline(l);
         }
         strncpy(tbuf, &l->buffer[TermStart], TermLength);
         tbuf[TermLength] = '\0';
         p = 0;
         exp = bwb_exp(tbuf, FALSE, &p);
         if (ERROR_PENDING)
         {
            return bwb_zline(l);
         }
         for_target = exp_getnval(exp);

         /* Step */
         if (StepLength > 0)
         {
            strncpy(tbuf, &l->buffer[StepStart], StepLength);
            tbuf[StepLength] = '\0';
            p = 0;
            exp = bwb_exp(tbuf, FALSE, &p);
            if (ERROR_PENDING)
            {
               return bwb_zline(l);
            }
            for_step = exp_getnval(exp);
         }
         else
         {
            for_step = 1.0;
         }
      }
      else
      {
         /* Limit */
         if (TermLength < 1)
         {
            bwb_error(err_syntax);
            return bwb_zline(l);
         }
         strncpy(tbuf, &l->buffer[TermStart], TermLength);
         tbuf[TermLength] = '\0';
         p = 0;
         exp = bwb_exp(tbuf, FALSE, &p);
         if (ERROR_PENDING)
         {
            return bwb_zline(l);
         }
         for_target = exp_getnval(exp);

         /* Step */
         if (StepLength > 0)
         {
            strncpy(tbuf, &l->buffer[StepStart], StepLength);
            tbuf[StepLength] = '\0';
            p = 0;
            exp = bwb_exp(tbuf, FALSE, &p);
            if (ERROR_PENDING)
            {
               return bwb_zline(l);
            }
            for_step = exp_getnval(exp);
         }
         else
         {
            for_step = 1.0;
         }

         /* Init */
         if (InitLength < 1)
         {
            bwb_error(err_syntax);
            return bwb_zline(l);
         }
         strncpy(tbuf, &l->buffer[InitStart], InitLength);
         tbuf[InitLength] = '\0';
         p = 0;
         exp = bwb_exp(tbuf, FALSE, &p);
         if (ERROR_PENDING)
         {
            return bwb_zline(l);
         }
         Value = exp_getnval(exp);


         /* Create Variable */
         if (VarLength < 1)
         {
            bwb_error(err_syntax);
            return bwb_zline(l);
         }
         strncpy(tbuf, &l->buffer[VarStart], VarLength);
         tbuf[VarLength] = '\0';
         v = var_find(tbuf);

         /* Assign Variable */
         var_setnval(v, Value);

      }

      CURTASK         excs[CURTASK exsc].local_variable = v;
      CURTASK         excs[CURTASK exsc].for_step = for_step;
      CURTASK         excs[CURTASK exsc].for_target = for_target;
   }
   /* -------------------------------------------------------------------
    * ------------------------------ */


   bwb_setexec(l, l->position, EXEC_FOR);


   /* 800 FOR I1=9 TO I1 STEP I1 ' I1=-2 */
   if (TRUE)
   {
      int             IsExit;
      BasicNumberType Value;
      BasicNumberType Target;
      BasicNumberType Step;

      IsExit = FALSE;

      Value = var_getnval(CURTASK excs[CURTASK exsc].local_variable);
      Target = CURTASK excs[CURTASK exsc].for_target;
      Step = CURTASK excs[CURTASK exsc].for_step;

      if (Step > 0)
      {
         /* POSITIVE */
         if (Value > Target)
         {
            IsExit = TRUE;
         }
      }
      else
      {
         /* NEGATIVE */
         if (Value < Target)
         {
            IsExit = TRUE;
         }
      }
      if (IsExit == TRUE)
      {
         /* EXIT FOR */


         struct bwb_line *next_line;

         next_line = CURTASK excs[CURTASK exsc].LoopBottomLine->next;

         bwb_decexec();

         /* set the next line in the exec stack */

         next_line->position = 0;
         /* bwb_setexec( next_line, 0, EXEC_NORM ); *//* WRONG
          * (JBV) */
         bwb_setexec(next_line, 0, CURTASK excs[CURTASK exsc].code); /* JBV */
         return next_line;

      }
   }
   /* proceed with processing */
   return bwb_zline(l);
}



/***************************************************************
  
   FUNCTION:       bwb_exitfor()
  
   DESCRIPTION:    This function handles the BASIC EXIT
         FOR statement.  This is a structured
         programming command compatible with ANSI
         BASIC. It is called from the bwb_exit()
         subroutine.
  
   SYNTAX:     EXIT FOR
         
***************************************************************/

struct bwb_line *
bwb_EXIT_FOR(struct bwb_line * l)
{
   struct bwb_line *next_line;

   bwx_DEBUG(__FUNCTION__);
   /* Check the integrity of the FOR statement */

   next_line = FindExitLineOnStack(l);
   if (next_line == NULL)
   {
      bwb_error("EXIT FOR without FOR");
      return bwb_zline(l);
   }
   {
      /* EXIT FOR */
      struct bwb_line *r;

      CURTASK         excs[CURTASK exsc].LoopTopLine = NULL;
      r = CURTASK excs[CURTASK exsc].LoopBottomLine->next;
      bwb_setexec(r, 0, CURTASK excs[CURTASK exsc - 1].code);
      r->position = 0;
      bwb_decexec();
      return r;
   }
#if 0
   next_line = next_line->next;

   /* set the next line in the exec stack */
   next_line->position = 0;
   bwb_setexec(next_line, 0, EXEC_FOR);
   return next_line;
#endif
}




/***************************************************************
  
        FUNCTION:       bwb_next()
  
        DESCRIPTION:    This function handles the BASIC NEXT
                        statement.
  
   SYNTAX:     NEXT counter
  
***************************************************************/


struct bwb_line *
bwb_NEXT(struct bwb_line * l)
{
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   if (FindBottomLineOnStack(l) == FALSE)
   {
      /* NOT FOUND */
      bwb_error("NEXT without FOR");
      return bwb_zline(l);
   }
   /* read the argument, if there is one */

   exp_getvfname(&(l->buffer[l->position]), tbuf);

   if (strlen(tbuf) != 0)
   {
      /* NEXT variablename */
      struct bwb_variable *v;


      v = var_find(tbuf);
      l->position += strlen(tbuf);

      /* decrement or increment the value */

      if (CURTASK excs[CURTASK exsc].local_variable != v)
      {
         /* not found */
         bwb_error("NEXT without FOR");
         return bwb_zline(l);
      }
   }
   else
   {
      /* NEXT */

      if (CURTASK excs[CURTASK exsc].local_variable == NULL)
      {
         /* not found */
         bwb_error("NEXT without FOR");
         return bwb_zline(l);
      }
   }




   var_setnval(CURTASK excs[CURTASK exsc].local_variable,
          var_getnval(CURTASK excs[CURTASK exsc].local_variable)
          + CURTASK excs[CURTASK exsc].for_step);

   /* check for completion of the loop */

   if (CURTASK excs[CURTASK exsc].for_step > 0) /* if step is positive */
   {

      if (var_getnval(CURTASK excs[CURTASK exsc].local_variable)
          > CURTASK excs[CURTASK exsc].for_target)
      {
         bwb_decexec();
         bwb_setexec(l->next, 0, CURTASK excs[CURTASK exsc].code);
         return bwb_zline(l);
      }
   }
   else        /* if step is negative */
   {
      if (var_getnval(CURTASK excs[CURTASK exsc].local_variable)
          < CURTASK excs[CURTASK exsc].for_target)
      {
         bwb_decexec();
         bwb_setexec(l->next, 0, CURTASK excs[CURTASK exsc].code);
         return bwb_zline(l);
      }
   }

   /* Target not reached: return to the top of the FOR loop */

   bwb_setexec(CURTASK excs[CURTASK exsc].LoopTopLine,
      CURTASK excs[CURTASK exsc].LoopTopLine->position, EXEC_FOR);


   return CURTASK excs[CURTASK exsc].LoopTopLine;

}



/***************************************************************
  
   FUNCTION:   cnd_tostep()
  
   DESCRIPTION:   This function searches through the
         <buffer> beginning at point <position>
         and attempts to find positions of TO
         and STEP statements.
  
***************************************************************/

static int
cnd_tostep(char *buffer, int position, int *to, int *step)
{
   int             loop, t_pos, b_pos, p_word;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);
   /* set then and els to FALSE initially */

   *to = *step = FALSE;

   /* loop to find words */

   p_word = b_pos = position;
   t_pos = 0;
   tbuf[0] = '\0';
   loop = TRUE;
   while (loop == TRUE)
   {

      if (buffer[b_pos] == '\0' || buffer[b_pos] == OptionCommentChar)
      {
         /* end of string */
         return TRUE;
      }
      switch (buffer[b_pos])
      {
      case ' ':   /* whitespace = end of word */
         if (strncasecmp(tbuf, "TO", (size_t) strlen("TO")) == 0)
         {
            *to = p_word;
         }
         else
         if (strncasecmp(tbuf, "STEP", (size_t) strlen("STEP")) == 0)
         {
            *step = p_word;
         }
         ++b_pos;
         p_word = b_pos;
         t_pos = 0;
         tbuf[0] = '\0';
         break;

      default:
         tbuf[t_pos] = buffer[b_pos];
         ++b_pos;
         ++t_pos;
         tbuf[t_pos] = '\0';
         break;
      }

   }

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       var_setnval()
  
   DESCRIPTION:    This function sets the value of numerical
         variable v to the value of i.
  
***************************************************************/

int
var_setnval(struct bwb_variable * v, BasicNumberType i)
{
   bwx_DEBUG(__FUNCTION__);

   switch (v->type)
   {
   case NUMBER:
      *var_findnval(v, v->array_pos) = i;
      break;
   default:
      sprintf(bwb_ebuf, "in var_setnval(): variable <%s> is not a number",
         v->name);
      bwb_error(bwb_ebuf);
   }

   /* successful assignment */

   return TRUE;

}

static
int
FindTopLineOnStack(struct bwb_line * l)
{
   /* since we are the top of the loop, we MIGHT be on the stack */
   int             i;

   bwx_DEBUG(__FUNCTION__);

   for (i = CURTASK exsc; i >= 0; i--)
   {
      struct bwb_line *current;

      current = CURTASK excs[i].LoopTopLine;
      if (current != NULL)
      {
         if (current == l)
         {
            /* FOUND */
            while (CURTASK exsc > i)
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
            /* NOT FOUND */
            return FALSE;
            break;
         }
      }
   }

   /* NOT FOUND */
   return FALSE;
}

static
int
FindBottomLineOnStack(struct bwb_line * l)
{
   /* since we are the bottom of the loop, we MUST be on the stack */

   bwx_DEBUG(__FUNCTION__);

   while (CURTASK exsc >= 0)
   {
      if (CURTASK excs[CURTASK exsc].LoopBottomLine == l)
      {
         /* FOUND */
         return TRUE;
      }
      bwb_decexec();
   }

   /* NOT FOUND */
   return FALSE;
}

static struct bwb_line *
FindExitLineOnStack(struct bwb_line * l)
{
   /* we are an EXIT ... command, find the bottom line on the stack */

   bwx_DEBUG(__FUNCTION__);
   while (CURTASK exsc >= 0)
   {
      struct bwb_line *current;

      current = CURTASK excs[CURTASK exsc].LoopBottomLine;
      if (current != NULL)
      {
         switch (l->cmdnum)
         {
         case C_EXIT_DO:
            /* DO - LOOP */
            switch (current->cmdnum)
            {
            case C_LOOP:
            case C_LOOP_UNTIL:
            case C_LOOP_WHILE:
               return current;
               break;
            }
            break;
         case C_EXIT_WHILE:
            /* WHILE - WEND */
            switch (current->cmdnum)
            {
            case C_WEND:
               return current;
               break;
            }
            break;
         case C_EXIT_UNTIL:
            /* UNTIL - UEND */
            switch (current->cmdnum)
            {
            case C_UEND:
               return current;
               break;
            }
            break;
         case C_EXIT_FOR:
            /* FOR - NEXT */
            switch (current->cmdnum)
            {
            case C_NEXT:
               return current;
               break;
            }
            break;
         case C_EXIT_SUB:
            /* SUB - END SUB */
            switch (current->cmdnum)
            {
            case C_END_SUB:
               return current;
               break;
            }
            break;
         case C_EXIT_FUNCTION:
            /* FUNCTION - END FUNCTION */
            switch (current->cmdnum)
            {
            case C_END_FUNCTION:
               return current;
               break;
            }
            break;
         case C_ELSEIF:
         case C_ELSE:
            /* IF - END IF */
            switch (current->cmdnum)
            {
            case C_END_IF:
               return current;
               break;
            }
            break;
         case C_CASE_IF:
         case C_CASE_IS:
         case C_CASE:
         case C_CASE_ELSE:
            /* SELECT CASE - END SELECT */
            switch (current->cmdnum)
            {
            case C_END_SELECT:
               return current;
               break;
            }
            break;
         }
      }
      bwb_decexec();
   }
   /* NOT FOUND */
   return FALSE;
}


static struct bwb_line *
scan_BottomLineInCode(struct bwb_line * l)
{
   /* we are the TopLine.  Let's find our matching BottomLine */

   struct bwb_line *current;
   register int    w_level;

   bwx_DEBUG(__FUNCTION__);

   switch (l->cmdnum)
   {
   case C_DEF:
   case C_USER_LBL:
      /* just the single line */
      return l;
      break;
   }

   /* multiple lines */
   w_level = 1;
   for (current = l->next; current != &CURTASK bwb_end; current = current->next)
   {
      switch (l->cmdnum)
      {
      case C_DO:
      case C_DO_WHILE:
      case C_DO_UNTIL:
         /* DO - LOOP */
         switch (current->cmdnum)
         {
         case C_DO:
         case C_DO_WHILE:
         case C_DO_UNTIL:
            ++w_level;
            break;
         case C_LOOP:
         case C_LOOP_UNTIL:
         case C_LOOP_WHILE:
            --w_level;
            if (w_level == 0)
            {
               return current;
            }
            break;
         }
         break;
      case C_WHILE:
         /* WHILE - WEND */
         switch (current->cmdnum)
         {
         case C_WHILE:
            ++w_level;
            break;
         case C_WEND:
            --w_level;
            if (w_level == 0)
            {
               return current;
            }
            break;
         }
         break;
      case C_UNTIL:
         /* UNTIL - UEND */
         switch (current->cmdnum)
         {
         case C_UNTIL:
            ++w_level;
            break;
         case C_UEND:
            --w_level;
            if (w_level == 0)
            {
               return current;
            }
            break;
         }
         break;
      case C_FOR:
         /* FOR - NEXT */
         switch (current->cmdnum)
         {
         case C_FOR:
            ++w_level;
            break;
         case C_NEXT:
            --w_level;
            if (w_level == 0)
            {
               return current;
            }
            break;
         }
         break;
      case C_IF_THEN:
         /* IF - END IF */
         switch (current->cmdnum)
         {
         case C_IF_THEN:
            ++w_level;
            break;
         case C_END_IF:
            --w_level;
            if (w_level == 0)
            {
               return current;
            }
            break;
         }
         break;
      case C_SELECT_CASE:
         /* SELECT CASE - END SELECT */
         switch (current->cmdnum)
         {
         case C_SELECT_CASE:
            ++w_level;
            break;
         case C_END_SELECT:
            --w_level;
            if (w_level == 0)
            {
               return current;
            }
            break;
         }
         break;
      case C_SUB:
         /* SUB - END SUB */
         switch (current->cmdnum)
         {
         case C_SUB:
            ++w_level;
            break;
         case C_END_SUB:
            --w_level;
            if (w_level == 0)
            {
               return current;
            }
            break;
         }
         break;
      case C_FUNCTION:
         /* SUB - END SUB */
         switch (current->cmdnum)
         {
         case C_FUNCTION:
            ++w_level;
            break;
         case C_END_FUNCTION:
            --w_level;
            if (w_level == 0)
            {
               return current;
            }
            break;
         }
         break;
      default:
         return NULL;
         break;
      }
   }
   /* NOT FOUND */
   return NULL;
}


struct bwb_line *
find_BottomLineInCode(struct bwb_line * l)
{
   /* we only want to scan once */
   /* bwb_scan() sets l->OtherLine to NULL to ensure MERGE and DELETE
    * work correctly */
   bwx_DEBUG(__FUNCTION__);

   if (l->OtherLine == NULL)
   {
      /* we do not yet know */
      l->OtherLine = scan_BottomLineInCode(l);
      if (l->OtherLine != NULL)
      {
         /* FOUND */
         /* mark the bottom line to point to us, so we can use
          * this info in DELETE */
         l->OtherLine->OtherLine = l;
      }
   }
   return l->OtherLine;
}

static struct bwb_line *
find_NextTestInCode(struct bwb_line * l)
{
   struct bwb_line *current;
   register int    w_level;

   bwx_DEBUG(__FUNCTION__);

   w_level = 1;
   for (current = l->next; current != &CURTASK bwb_end; current = current->next)
   {
      switch (l->cmdnum)
      {
      case C_IF_THEN:
      case C_ELSEIF:
         switch (current->cmdnum)
         {
         case C_IF_THEN:
            ++w_level;
            break;
         case C_ELSEIF:
         case C_ELSE:
            /* we must report only the first ELSE we
             * encounter at level 1 */
            if (w_level == 1)
            {
               return current;
            }
            break;
         case C_END_IF:
            --w_level;
            if (w_level == 0)
            {
               return NULL;
            }
            break;
         }
         break;
      case C_CASE:
      case C_CASE_IF:
      case C_CASE_IS:
         switch (current->cmdnum)
         {
         case C_SELECT_CASE:
            ++w_level;
            break;
         case C_CASE:
         case C_CASE_IF:
         case C_CASE_IS:
         case C_CASE_ELSE:
            /* we must report only the first CASE we
             * encounter at level 1 */
            if (w_level == 1)
            {
               return current;
            }
            break;
         case C_END_SELECT:
            --w_level;
            if (w_level == 0)
            {
               return NULL;
            }
         }
         break;
      default:
         return NULL;
         break;
      }

   }
   return NULL;
}

/* EOF */
