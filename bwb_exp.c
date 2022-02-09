/****************************************************************
  
        bwb_exp.c       Expression Parser
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




/***************************************************************
  
        FUNCTION: bwb_exp()
  
        DESCRIPTION: This is the function by which the expression
         parser is called.
  
***************************************************************/

struct exp_ese *
bwb_exp(char *expression, int assignment, int *position)
{
   struct exp_ese *rval;   /* return value */
   int             entry_level, main_loop, err_condition;
   int             r;   /* return value from functions */
   register int    c;   /* quick counter */

   bwx_DEBUG(__FUNCTION__);


   /* save the entry level of the expression stack in order to check it
    * at the end of this function */

   entry_level = CURTASK expsc;
   err_condition = FALSE;

   /* advance past whitespace */

   adv_ws(expression, position);

   /* increment the expression stack counter to get a new level */

   inc_esc();

   /* check to be sure there is a legitimate expression and set initial
    * parameters for the main loop */

   if (is_eol(expression, position) == TRUE)
   {
      main_loop = FALSE;   /* break out of loop */
   }
   else
   {
      main_loop = TRUE;
      CURTASK         exps[CURTASK expsc].pos_adv = 0;
   }




   /* main parsing loop */

   while (main_loop == TRUE)
   {
      char           *e;   /* pointer to current string */

      /* set variable <e> to the start of the expression */

      e = &(expression[*position]);


      /* detect the operation required at this level */

      CURTASK         exps[CURTASK expsc].operation = exp_findop(e);

      /* perform actions specific to the operation */

      switch (CURTASK exps[CURTASK expsc].operation)
      {
      case OP_ERROR:
         main_loop = FALSE;
         err_condition = TRUE;
         break;

      case OP_TERMINATE:   /* terminate at THEN, ELSE, TO */
      case OP_STRJOIN:  /* string join or tab */
      case OP_STRTAB:
         main_loop = FALSE;
         err_condition = FALSE;
         dec_esc();
         break;

      case OP_ADD:   /* in the case of any numerical operation, */
      case OP_SUBTRACT:
      case OP_MULTIPLY:
      case OP_DIVIDE:
      case OP_MODULUS:
      case OP_EXPONENT:
      case OP_INTDIVISION:
      case OP_GREATERTHAN:
      case OP_LESSTHAN:
      case OP_GTEQ:
      case OP_LTEQ:
      case OP_NOTEQUAL:
      case OP_NOT:
      case OP_AND:
      case OP_OR:
      case OP_XOR:
      case OP_IMPLIES:
      case OP_EQUIV:
      case OP_NEGATION: /* JBV */
      case OP_POSATION:


         CURTASK exps[CURTASK expsc].pos_adv = -1; /* set to strange number */

         /* cycle through operator table to find match */

         for (c = 0; c < NUM_OPERATORS; ++c)
         {
            if (exp_ops[c].operation == CURTASK exps[CURTASK expsc].operation)
            {
               CURTASK         exps[CURTASK expsc].pos_adv = strlen(exp_ops[c].symbol);
            }
         }

         if (CURTASK exps[CURTASK expsc].pos_adv == -1)  /* was a match found? */
         {
            CURTASK         exps[CURTASK expsc].pos_adv = 0;   /* no -- set to 0 */
         }
         break;   /* and move on */

      case OP_EQUALS:


         if (assignment == TRUE)
         {
            CURTASK         exps[CURTASK expsc].operation = OP_ASSIGN;
         }
         CURTASK         exps[CURTASK expsc].pos_adv = 1;
         break;

      case PARENTHESIS:
         r = exp_paren(e);
         break;

      case CONST_STRING:
         r = exp_strconst(e);
         break;

      case CONST_NUMERICAL:
         r = exp_numconst(e);
         break;

      case FUNCTION:


         r = exp_function(e);
         break;


      case VARIABLE:
         r = exp_variable(e);
         break;

      default:
         err_condition = TRUE;
         main_loop = FALSE;
         sprintf(bwb_ebuf, "in bwb_exp.c:bwb_exp(): unidentified operation (%d).",
            CURTASK exps[CURTASK expsc].operation);
         bwb_error(bwb_ebuf);
         break;
      }

      /* increment *position counter based on previous actions */

      *position += CURTASK exps[CURTASK expsc].pos_adv;
      CURTASK         exps[CURTASK expsc].pos_adv = 0;   /* reset advance counter */


      /* check for end of string */

      if (is_eol(expression, position) == TRUE)
      {
         main_loop = FALSE;   /* break out of loop */
      }
      /* get a new stack level before looping */

      if (main_loop == TRUE)
      {
         r = inc_esc();
      }
      /* check for error return */

      if (r == OP_ERROR)
      {
         main_loop = FALSE;
         err_condition = TRUE;
      }
      else
      {
         r = TRUE;
      }

   }        /* end of main parsing loop */


   /* check error condition */

   if (err_condition == TRUE)
   {


      /* decrement the expression stack counter until it matches
       * entry_level */

      while (CURTASK expsc > entry_level)
      {
         dec_esc();
      }

      bwb_error(err_syntax);
      return NULL;
   }
   /* no error; normal exit from function */

   else
   {

      /* are any more operations needed? if we are still at entry
       * level, then they are not */

      /* try operations */

      exp_operation(entry_level);

      /* see what is on top of the stack */

      if (CURTASK expsc > (entry_level + 1))
      {
         switch (CURTASK exps[CURTASK expsc].operation)
         {
         case OP_STRJOIN:
            if (CURTASK expsc != (entry_level + 2))
            {
               sprintf(bwb_ebuf, "in bwb_exp(): OP_STRJOIN in wrong position.");
               bwb_error(bwb_ebuf);
            }
            break;
         default:
            sprintf(bwb_ebuf, "in bwb_exp(): incomplete expression.");
            bwb_error(bwb_ebuf);
            break;
         }

         /* decrement the expression stack counter */


         dec_esc();

      }
      /* assign rvar to the variable for the current level */

      rval = &(CURTASK exps[CURTASK expsc]);

      /* decrement the expression stack counter */

      dec_esc();

      /* check the current level before exit */
      if (entry_level != CURTASK expsc)
      {
         sprintf(bwb_ebuf, "in bwb_exp(): exit stack level (%d) does not match entry stack level (%d)",
            CURTASK expsc, entry_level);
         bwb_error(bwb_ebuf);
      }
   }

   /* return a pointer to the last stack level */

   return rval;

}

/***************************************************************
  
   FUNCTION:       exp_findop()
  
   DESCRIPTION:    This function reads the expression to find
         what operation is required at its stack level.
  
***************************************************************/

int
exp_findop(char *expression)
{
   register int    c;   /* character counter */
   int             rval;   /* return value */
   char            cbuf[BasicStringLengthMax + 1]; /* capitalized
                      * expression */
   char            nbuf[BasicStringLengthMax + 1]; /* non-capitalized
                      * expression */
   int             position;  /* position in the expression */
   int             adv_loop;  /* control loop to build expression */
   int             LastPosition;


   bwx_DEBUG(__FUNCTION__);


   /* set return value to OP_NULL initially */

   rval = OP_NULL;

   /* assign local pointer to expression to begin reading */

   position = 0;

   /* advance to the first significant character */

   adv_ws(expression, &position);


   /* we now have the first significant character and can begin parsing */

   /* check the first character for an indication of a parenthetical
    * expression, a string constant, or a numerical constant that begins
    * with a digit (numerical constants beginning with a plus or minus
    * sign or hex/octal/binary constants will have to be detected by
    * exp_isnc() */

   switch (expression[position])
   {
   case '=':
      /* look-ahead one character */
      switch( expression[position + 1] )
      {
      case '>':
          /* => */
          rval = OP_GTEQ;
          break;
      case '<':
          /* =< */
          rval = OP_LTEQ;
          break;
      default:
          rval = OP_EQUALS;
          break;
      }
      break;
   case '<':
      /* look-ahead one character */
      switch( expression[position + 1] )
      {
      case '>':
          /* <> */
          rval = OP_NOTEQUAL;
          break;
      case '=':
          /* <= */
          rval = OP_LTEQ;
          break;
      default:
          rval = OP_LESSTHAN;
          break;
      }
      break;
   case '>':
      /* look-ahead one character */
      switch( expression[position + 1] )
      {
      case '=':
          /* >= */
          rval = OP_GTEQ;
          break;
      case '<':
          /* >< */
          rval = OP_NOTEQUAL;
          break;
      default:
          rval = OP_GREATERTHAN;
          break;
      }
      break;
   case '\"':     /* this should indicate a string constant */
      rval = CONST_STRING;
      break;
   case '(':      /* this will indicate a simple parenthetical
             * expression */
      rval = PARENTHESIS;
      break;
   case ')':      /* end of argument list? */
      rval = OP_TERMINATE;
      break;

   case '0':      /* these will indicate a numerical constant */
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
   case '&':      /* designator for hex or octal constant */
      rval = CONST_NUMERICAL;
      break;
   }


   if (rval != OP_NULL)
   {
      return rval;
   }
   /* String constants, numerical constants, open parentheses, and the
    * plus and minus operators have been checked at this point; but if
    * the return value is still OP_NULL, other possibilities must be
    * checked, namely, other operators, function names, and variable
    * names.  The function adv_element cannot be used here because it
    * will stop, e.g., with certain operators and not include them in
    * the returned element. */

   /* get a character string to be interpreted */

   adv_loop = TRUE;
   cbuf[0] = '\0';
   nbuf[0] = '\0';
   c = 0;
   while (adv_loop == TRUE)
   {


      switch (expression[position])
      {
      case ' ':   /* whitespace */
      case '\0':  /* end of string */
      case '(':   /* parenthesis terminating function name */
         adv_loop = FALSE;
         break;
      default:
         nbuf[c] = cbuf[c] = expression[position];
         ++c;
         nbuf[c] = cbuf[c] = '\0';
         ++position;
         break;
      }


      LastPosition = position;


      if (c > BasicStringLengthMax)
      {
         adv_loop = FALSE;
      }
   }


   /* check for numerical constant */

   rval = exp_isnc(cbuf);

   if (rval != OP_NULL)
   {
      return rval;
   }
   /* check for other operators */

   rval = exp_isop(cbuf);

   if (rval != OP_NULL)
   {
      return rval;
   }
   exp_getvfname(cbuf, nbuf);
   if (strlen(nbuf) == 0)
   {
      return OP_ERROR;
   }
   strcpy(CURTASK exps[CURTASK expsc].string, nbuf);


   /* User definedfunctions are now processed by fnc_deffn */

   if (rval != OP_NULL)
   {
      return rval;
   }
   /* check for function name */

   rval = exp_isfn(nbuf);

   if (rval != OP_NULL)
   {
      return rval;
   }
   /* check for a BASIC command, esp. to catch THEN or ELSE */

   rval = exp_iscmd(nbuf);

   if (rval != OP_NULL)
   {
      return rval;
   }
   /* last: check for variable name, and assign it if there is not
    * already one */

   /* OPTION STRICT OFF to disables Implicit DIM */
   /* This is only for ECMA-55.  ECMA-116 requires arrays to be
    * dimensioned prior to being used. */
   if (strlen(nbuf) == strlen(cbuf))
   {
      if (bwb_isvar(nbuf) == FALSE)
      {
         /* not an existing variable */
         while (expression[LastPosition] == ' ')
         {
            LastPosition++;
         }
         if (expression[LastPosition] == '(')
         {
            /* MUST be a dynamically created array, LET
             * ... A(10) ... ' variable "A" has NOT been
             * dimensioned */
            int             NumDimensions;

            NumDimensions = DetermineNumberOfDimensions(expression, LastPosition);
            if (NumDimensions < 1)
            {
               bwb_error(err_incomplete);
               return OP_ERROR;
            }
            if (ImplicitDim(nbuf, NumDimensions) != TRUE)
            {
               bwb_error(err_syntax);
               return OP_ERROR;
            }
         }  /* if( expression[ LastPosition ] == '(' ) */
      }     /* if( bwb_isvar( nbuf ) == FALSE ) */
   }        /* if( strlen( nbuf ) == strlen( cbuf ) ) */
   rval = exp_isvn(nbuf);

   if (rval != OP_NULL)
   {
      return rval;
   }
   /* return the value assigned (or OP_ERROR if none assigned) */

   return OP_ERROR;

}


/***************************************************************
  
   FUNCTION:       exp_isnc()
  
   DESCRIPTION:    This function reads the expression to find
         if a numerical constant is present at this
         point.
  
***************************************************************/

int
exp_isnc(char *expression)
{

   bwx_DEBUG(__FUNCTION__);

   switch (expression[0])
   {
   case '0':      /* these will indicate a numerical constant */
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
   case '&':      /* indicator for hex or octal constant */
      return CONST_NUMERICAL;



   case '-':
      switch (CURTASK exps[CURTASK expsc - 1].operation)
      {
      case VARIABLE: /* external variable pointed to by xvar */
         if (TRUE)
         {
            struct bwb_variable *v;
            v = CURTASK exps[CURTASK expsc - 1].xvar;
            if (v->IsInDim > 0)
            {
               return OP_NEGATION;
            }
         }
      case NUMBER:   /* number held as internal variable in uvar */
      case CONST_STRING:   /* string constant */
      case CONST_NUMERICAL:   /* numerical constant */
         /* might be subtraction */
         return OP_NULL;
      }
      /* any arithmetic operator: +, -, *, /, \, MOD */
      /* any logical operator: AND, OR, XOR, IMP, EQV, NOT */
      /* any comparison operator: <, <=, =, >=, > */
      /* assignment: LET A = - .... */
      /* parenthesis: LET A = ... (- .... */
      /* command: PRINT - ....  */
      return OP_NEGATION;

   case '+':
   case BasicFileNumberPrefix:   /* EOF( #1 ) */
      switch (CURTASK exps[CURTASK expsc - 1].operation)
      {
      case VARIABLE: /* external variable pointed to by xvar */
         if (TRUE)
         {
            struct bwb_variable *v;
            v = CURTASK exps[CURTASK expsc - 1].xvar;
            if (v->IsInDim > 0)
            {
               return OP_POSATION;
            }
         }
      case NUMBER:   /* number held as internal variable in uvar */
      case CONST_STRING:   /* string constant */
      case CONST_NUMERICAL:   /* numerical constant */
         /* might be addition */
         return OP_NULL;
      }
      /* any arithmetic operator: +, -, *, /, \, MOD */
      /* any logical operator: AND, OR, XOR, IMP, EQV, NOT */
      /* any comparison operator: <, <=, =, >=, > */
      /* assignment: LET A = + .... */
      /* parenthesis: LET A = ... (+ .... */
      /* command: PRINT + ....  */
      return OP_POSATION;


   default:
      return OP_NULL;
   }

}

/***************************************************************
  
   FUNCTION:       exp_isop()
  
   DESCRIPTION:    This function reads the expression to find
         if a logical or mathematical operation is
         required at this point.
  
        This function presupposes that a numerical constant with
        affixed plus or minus sign has been ruled out.
  
***************************************************************/

int
exp_isop(char *expression)
{

   /* Problem: The following lines are valid BASIC: LET note = 0 LET
    * not1 = 0 LET andy = 0 LET and2 = 0 LET oreo = 0 LET or3 = 0 LET
    * xorigin = 0 LET xor4 = 0 LET imply = 0 LET impl5 = 0 LET equality
    * = 0 LET equ6 = 0 LET mode = 0 LET mod7 = 0 However, the
    * interpreter displays the message: ERROR: in exp_getnval():
    * operation <0> is not a number */
   /* Reference:  ECMA-116, Page 15, Section 4.1.2, Last Paragraph All
    * keywords in a program, when used as such, shall be preceded and
    * followed by some character other than a letter, digit, underline
    * or dollar-sign.  A keyword may also be followed by an end-of-line */
   int             i;
   int             j;
   int             k;
   int             Found;

   register int    c;   /* counter */

   bwx_DEBUG(__FUNCTION__);

   /* compare the initial characters of the string with the table of
    * operators */

   for (c = 0; c < NUM_OPERATORS; ++c)
   {
      if (OptionVersion & exp_ops[c].OptionVersionBitmask)
         if (strncasecmp(expression, exp_ops[c].symbol,
               (size_t) strlen(exp_ops[c].symbol)) == 0)
         {
            /* Problem: The following lines are valid
             * BASIC: LET note = 0 LET not1 = 0 LET andy
             * = 0 LET and2 = 0 LET oreo = 0 LET or3 = 0
             * LET xorigin = 0 LET xor4 = 0 LET imply = 0
             * LET impl5 = 0 LET equality = 0 LET equ6 =
             * 0 LET mode = 0 LET mod7 = 0 
             *
             * However, the interpreter displays the
             * message: ERROR: in exp_getnval():
             * operation <0> is not a number */
            /* Reference:  ECMA-116, Page 15, Section
             * 4.1.2, Last Paragraph All keywords in a
             * program, when used as such, shall be
             * preceded and followed by some character
             * other than a letter, digit, underline or
             * dollar-sign.  A keyword may also be
             * followed by an end-of-line */
            Found = TRUE;
            if (isalpha(exp_ops[c].symbol[0]))
            {
               /* NOT, AND, OR, XOR, IMP, EQV, MOD */
               i = strlen(exp_ops[c].symbol);
               j = strlen(expression);
               if (j > i)
               {
                  /* the expression does not
                   * end with the end of the
                   * operator */
                  k = expression[i];
                  /* character AFTER the
                   * operator */
                  if (isalpha(k))
                  {
                     /* NOTe, ANDy, OReo,
                      * XORe, IMPly,
                      * EQUals, MODe */
                     Found = FALSE;
                  }
                  else
                  if (isdigit(k))
                  {
                     /* NOT1, AND2, OR3,
                      * XOR4, IMP5, EQU6,
                      * MOD7 */
                     Found = FALSE;
                  }
                  else
                  if (k == BasicStringSuffix)
                  {
                     /* N O T $ , 
                      *
                      * AN D $ , 
                      *
                      * OR $ , 
                      *
                      * XO R $ , 
                      *
                      * IM P $ , 
                      *
                      * EQ U $ , 
                      *
                      * MO D $ */
                     Found = FALSE;
                  }
                  else
                  if( OptionFlags & OPTION_BUGS_ON )
                  {
                      if (k == '_' || k == '.' )
                      {
                         /* NOT _, AND _, OR_
                          * , XOR _, IMP _,
                          * EQU _, MOD _ */
                         Found = FALSE;
                      }
                  }
                  else
                  {
                     /* N O T , 
                      *
                      * AN D , 
                      *
                      * OR , 
                      *
                      * XO R , 
                      *
                      * IM P , 
                      *
                      * EQ U , 
                      *
                      * MO D */
                  }
               }
            }
            if (Found == TRUE)
            {
               return exp_ops[c].operation;
            }
         }
   }

   /* search failed; return OP_NULL */

   return OP_NULL;

}

/***************************************************************
  
   FUNCTION:       exp_iscmd()
  
   DESCRIPTION:    This function reads the expression to find
         if a BASIC command name is present; if so,
         it returns OP_TERMINATE to terminate expression
         parsing.  This is critical, for example, in
         parsing a conditional following IF where THEN,
         ELSE, and other BASIC commands may follow.
  
***************************************************************/

int
exp_iscmd(char *expression)
{
   register int    n;

   bwx_DEBUG(__FUNCTION__);

/* FIXME: extract the expression before calling bwb_exp() - no COMMANDS should be there */
/* This should be a Syntax Error */

   /* first check for likely statements */
   if (strcasecmp(expression, "THEN") == 0)
   {
      /* IF ... THEN  */
      return OP_TERMINATE;
   }
   if (strcasecmp(expression, "TO") == 0)
   {
      /* FOR X =... TO ... STEP  */
      return OP_TERMINATE;
   }
   if (strcasecmp(expression, "STEP") == 0)
   {
      /* FOR X =... TO ... STEP  */
      return OP_TERMINATE;
   }
   if (strcasecmp(expression, "GO") == 0)
   {
      /* ON ... GO  */
      return OP_TERMINATE;
   }
   if (strcasecmp(expression, "GOTO") == 0)
   {
      /* ON ... GOTO  */
      return OP_TERMINATE;
   }
   if (strcasecmp(expression, "GOSUB") == 0)
   {
      /* ON ... GOSUB  */
      return OP_TERMINATE;
   }
   /* run through the command table and search for a match */




   n = is_cmd(expression);
   if (n > 0)
   {
      /* FIXME: this should return OP_ERROR, since we found an
       * UNEXPECTED command */

      return OP_TERMINATE;
   }
   /* search failed, return NULL */

   return OP_NULL;

}


/***************************************************************
  
   FUNCTION:       exp_isfn()
  
   DESCRIPTION:    This function reads the expression to find
         if a function name is present at this point.
  
***************************************************************/

int
exp_isfn(char *expression)
{
   /* if we are in a USER DEFINED FUNCTION - END FUNCTION block, the
    * function name is a local variable */
   int             i;


   bwx_DEBUG(__FUNCTION__);

   for (i = CURTASK exsc; i >= 0; i--)
   {
      if (CURTASK excs[i].LoopTopLine != NULL)
      {
         if (CURTASK excs[i].LoopTopLine->cmdnum == C_FUNCTION)
         {
            if (CURTASK excs[i].local_variable != NULL)
            {
               if (strcasecmp(CURTASK excs[i].local_variable->name, expression) == 0)
               {
                  return VARIABLE;
               }
            }
         }
      }
   }

   if (fnc_find(expression) != NULL)
   {
      return FUNCTION;
   }
   return OP_NULL;

}

/***************************************************************
  
   FUNCTION:       exp_isvn()
  
   DESCRIPTION:    This function reads the expression to find
         if a variable name at this point.
  
***************************************************************/

int
exp_isvn(char *expression)
{
   bwx_DEBUG(__FUNCTION__);


   if (var_find(expression) != NULL)
   {
      return VARIABLE;
   }
   return OP_NULL;

}

/***************************************************************
  
   FUNCTION:       exp_getvfname()
  
   DESCRIPTION:    This function reads the expression to find
         a variable or function name at this point.
  
***************************************************************/

int
exp_getvfname(char *source, char *destination)
{
#if 0
   int             s_pos, d_pos; /* source, destination positions */

   bwx_DEBUG(__FUNCTION__);

   s_pos = d_pos = 0;
   destination[0] = '\0';
   while (source[s_pos] != '\0')
   {

      /* all alphabetical characters are acceptable */

      if (isalpha(source[s_pos]) != 0)

      {
         destination[d_pos] = source[s_pos];

         ++d_pos;
         ++s_pos;
         destination[d_pos] = '\0';
      }
      /* numerical characters are acceptable but not in the first
       * position */

      else
      if ((isdigit(source[s_pos]) != 0) && (d_pos != 0))
      {
         destination[d_pos] = source[s_pos];
         ++d_pos;
         ++s_pos;
         destination[d_pos] = '\0';
      }
      /* other characters will have to be tried on their own merits */

      else
      {
         switch (source[s_pos])
         {

         case '.':   /* tolerated non-alphabetical
                * characters */
         case '_':
            destination[d_pos] = source[s_pos];
            ++d_pos;
            ++s_pos;
            destination[d_pos] = '\0';
            break;

         case STRING:   /* terminating characters */
         case BasicDoubleSuffix: /* Microsoft-type
                      * precision indicator;
                      * ignored but
                      * terminates */
         case BasicSingleSuffix: /* Microsoft-type
                      * precision indicator;
                      * ignored but
                      * terminates */
         case BasicCurrencySuffix:  /* Microsoft-type
                      * precision indicator;
                      * ignored but
                      * terminates */
         case BasicLongSuffix:   /* Microsoft-type precision
                   * indicator; ignored but
                   * terminates */
         case BasicIntegerSuffix:   /* Microsoft-type
                      * precision indicator;
                      * ignored but
                      * terminates */

            destination[d_pos] = source[s_pos];
            ++d_pos;
            ++s_pos;
            destination[d_pos] = '\0';

            return TRUE;

         case '(':   /* begin function/sub name */
            return TRUE;

         default: /* anything else is non-tolerated */
            return FALSE;
         }
      }
   }


   return TRUE;      /* exit after coming to the end */
#endif
   int position = 0;
    
   bwb_getvarname( source, destination, &position);
   return TRUE;      /* exit after coming to the end */

}

/***************************************************************
  
   FUNCTION:       exp_validarg()
  
   DESCRIPTION:    This function reads the expression to
         determine whether it is a valid argument (to be
         read recursively by bwb_exp() and passed to a
         function.
  
***************************************************************/

int
exp_validarg(char *expression)
{
   register int    c;

   bwx_DEBUG(__FUNCTION__);

   c = 0;
   while (TRUE)
   {
      switch (expression[c])
      {
      case ' ':
         ++c;
         break;
      case '\0':
         return FALSE;
      default:
         return TRUE;
      }
   }

}

/***************************************************************
  
        FUNCTION:    exp_getnval()
  
   DESCRIPTION:    This function returns the numerical value
         contain in the expression-stack element
         pointed to by 'e'.
  
***************************************************************/

double
exp_getnval(struct exp_ese * e)
{
   BasicNumberType Value;

   bwx_DEBUG(__FUNCTION__);

   Value = 0;

   if (e->type == NUMBER)
   {
      switch (e->operation)
      {
      case VARIABLE:
         Value = (*var_findnval(e->xvar, e->array_pos));
         break;
      case NUMBER:
         Value = e->nval;
         break;
      default:
         bwb_error(err_syntax);
      }
   }
   else
   {
      bwb_error(err_mismatch);
   }
   Value = VerifyNumeric(Value);
   return Value;

}

int
exp_getival(struct exp_ese * e)
{
   double          XR;
   XR = rint(exp_getnval(e));
   if (XR < INT_MIN)
   {
      bwb_Warning_Overflow("*** Arithmetic Overflow ***");
      XR = INT_MIN;
   }
   else
   if (XR > INT_MAX)
   {
      bwb_Warning_Overflow("*** Arithmetic Overflow ***");
      XR = INT_MAX;
   }
   return (int) XR;
}

/***************************************************************
  
   FUNCTION:       exp_getsval()
  
   DESCRIPTION:    This function returns a pointer to the
         BASIC string structure pointed to by
         expression-stack element 'e'.
  
***************************************************************/

bstring        *
exp_getsval(struct exp_ese * e)
{
   static bstring  b;

   bwx_DEBUG(__FUNCTION__);

   b.rab = FALSE;

   /* return based on operation type */

   switch (e->operation)
   {
   case CONST_STRING:
   case OP_STRJOIN:
      return &(e->sval);
   case VARIABLE:
      switch (e->type)
      {
      case STRING:
         return var_findsval(e->xvar, e->array_pos);
      case NUMBER:
         sprintf(bwb_ebuf, BasicNumberPrintFormat " ", exp_getnval(e));
         str_ctob(&b, bwb_ebuf);
         return &b;
      default:
         sprintf(bwb_ebuf, "in exp_getsval(): type <%c> inappropriate for NUMBER",
            e->type);
         bwb_error(bwb_ebuf);
         return NULL;
      }
      break;

   case NUMBER:
      switch (e->type)
      {
      case NUMBER:
         sprintf(bwb_ebuf, BasicNumberPrintFormat " ", exp_getnval(e));
         str_ctob(&b, bwb_ebuf);
         return &b;
      default:
         sprintf(bwb_ebuf, "in exp_getsval(): type <%c> inappropriate for NUMBER",
            e->type);
         bwb_error(bwb_ebuf);
         return NULL;
      }
      break;
   default:
      sprintf(bwb_ebuf, "in exp_getsval(): operation <%d> inappropriate",
         e->operation);
      bwb_error(bwb_ebuf);
      return NULL;
   }

   /* this point may not be reached */

   return NULL;

}

/***************************************************************
  
   FUNCTION:       inc_esc()
  
   DESCRIPTION:    This function increments the expression
         stack counter.
  
***************************************************************/

int
inc_esc(void)
{

   bwx_DEBUG(__FUNCTION__);

   ++CURTASK expsc;
   if (CURTASK expsc >= ESTACKSIZE)
   {
      --CURTASK expsc;
      sprintf(bwb_ebuf, "in inc_esc(): Maximum expression stack exceeded <%d>",
         CURTASK expsc);
      bwb_error(bwb_ebuf);
      return OP_NULL;
   }
   CURTASK         exps[CURTASK expsc].type = NUMBER;
   CURTASK         exps[CURTASK expsc].operation = OP_NULL;
   CURTASK         exps[CURTASK expsc].pos_adv = 0;

   return TRUE;
}

/***************************************************************
  
   FUNCTION:       dec_esc()
  
   DESCRIPTION:    This function decrements the expression
         stack counter.
  
***************************************************************/

int
dec_esc(void)
{

   bwx_DEBUG(__FUNCTION__);

   --CURTASK expsc;
   if (CURTASK expsc < 0)
   {
      CURTASK         expsc = 0;
      sprintf(bwb_ebuf, "in dec_esc(): Expression stack counter < 0.");
      bwb_error(bwb_ebuf);
      return OP_NULL;
   }
   return TRUE;
}


/* EOF */
