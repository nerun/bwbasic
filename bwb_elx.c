/****************************************************************
  
        bwb_elx.c       Parse Elements of Expressions
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
  
   FUNCTION:       exp_paren()
  
   DESCRIPTION:    This function interprets a parenthetical
         expression, calling bwb_exp() (recursively)
         to resolve the internal expression.
  
***************************************************************/

int
exp_paren(char *expression)
{
   struct exp_ese *e;
   int             s_pos;  /* position in build buffer */
   int             loop;
   int             paren_level;

   bwx_DEBUG(__FUNCTION__);

   /* find a string enclosed by parentheses */

   CURTASK         exps[CURTASK expsc].pos_adv = 1;   /* start beyond open
                         * paren */
   s_pos = 0;
   loop = TRUE;
   paren_level = 1;
   CURTASK         exps[CURTASK expsc].string[0] = '\0';

   while (loop == TRUE)
   {

      /* check the current character */

      switch (expression[CURTASK exps[CURTASK expsc].pos_adv])
      {
      case '\0':
         bwb_error(err_incomplete);
         loop = FALSE;
         break;

      case '(':
         ++paren_level;
         CURTASK         exps[CURTASK expsc].string[s_pos]
         = expression[CURTASK exps[CURTASK expsc].pos_adv];
         ++s_pos;
         CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
         break;

      case ')':

         --paren_level;
         if (paren_level == 0)
         {
            loop = FALSE;
         }
         else
         {
            CURTASK         exps[CURTASK expsc].string[s_pos]
            = expression[CURTASK exps[CURTASK expsc].pos_adv];
            ++s_pos;
            CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
         }
         break;

      case '\"':  /* embedded string constant */
         ++CURTASK exps[CURTASK expsc].pos_adv;
         while ((expression[CURTASK exps[CURTASK expsc].pos_adv] != '\"')
                && (expression[CURTASK exps[CURTASK expsc].pos_adv] != '\0'))
         {
            CURTASK         exps[CURTASK expsc].string[s_pos]
            = expression[CURTASK exps[CURTASK expsc].pos_adv];
            ++s_pos;
            CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
            ++CURTASK exps[CURTASK expsc].pos_adv;
         }
         break;

      default:
         CURTASK exps[CURTASK expsc].string[s_pos]
            = expression[CURTASK exps[CURTASK expsc].pos_adv];
         ++s_pos;
         CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
         break;
      }

      /* advance the counter */

      ++CURTASK exps[CURTASK expsc].pos_adv;

   }


   /* call bwb_exp() recursively to interpret this expression */

   CURTASK         exps[CURTASK expsc].rec_pos = 0;
   e = bwb_exp(CURTASK exps[CURTASK expsc].string, FALSE,
          &(CURTASK exps[CURTASK expsc].rec_pos));

   /* assign operation and value at this level */

   CURTASK         exps[CURTASK expsc].type = e->type;

   switch (e->type)
   {
   case STRING:
      CURTASK exps[CURTASK expsc].operation = CONST_STRING;
      str_btob(exp_getsval(&(CURTASK exps[CURTASK expsc])), exp_getsval(e));
      break;
   default:
      CURTASK exps[CURTASK expsc].operation = NUMBER;
      CURTASK         exps[CURTASK expsc].nval = exp_getnval(e);
      break;
   }

   return TRUE;

}

/***************************************************************
  
        FUNCTION:    exp_strconst()
  
        DESCRIPTION: This function interprets a string
         constant.
  
***************************************************************/

int
exp_strconst(char *expression)
{
   int             e_pos, s_pos;

   bwx_DEBUG(__FUNCTION__);

   /* assign values to structure */

   CURTASK         exps[CURTASK expsc].type = STRING;
   CURTASK         exps[CURTASK expsc].operation = CONST_STRING;

   /* set counters */

   s_pos = 0;
   CURTASK         exps[CURTASK expsc].pos_adv = e_pos = 1;
   CURTASK         exps[CURTASK expsc].string[0] = '\0';

   /* read the string up until the next quotation mark */

   /* While yer at it, check for a null terminator too (JBV, found by
    * DD) */
   while ((expression[e_pos] != '\"') && (expression[e_pos] != '\0'))
   {
      CURTASK         exps[CURTASK expsc].string[s_pos] = expression[e_pos];
      ++e_pos;
      ++s_pos;
      ++CURTASK exps[CURTASK expsc].pos_adv;
      CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
      if (s_pos > BasicStringLengthMax)
      {
         sprintf(bwb_ebuf, "string <%s> exceeds maximum size (%d) for string constant.",
            expression, BasicStringLengthMax);
         bwb_error(bwb_ebuf);
         return OP_NULL;
      }
   }

   /* now write string over to bstring */

   str_ctob(&(CURTASK exps[CURTASK expsc].sval), CURTASK exps[CURTASK expsc].string);

   /* advance past last quotation mark */

   /*-------------------------------------------------------------*/
   /* Of course, it doesn't hurt to make sure it's really a quote */
   /* (JBV, found by DD)                                          */
   /*-------------------------------------------------------------*/
   if (expression[e_pos] == '\"')
      ++CURTASK exps[CURTASK expsc].pos_adv;

   /* return */

   return TRUE;

}

/***************************************************************
  
        FUNCTION: exp_numconst()
  
        DESCRIPTION: This function interprets a numerical
         constant.
  
***************************************************************/

int
exp_numconst(char *expression)
{
   int             base;   /* numerical base for the constant */
   static struct bwb_variable mantissa;   /* mantissa of floating-point
                   * number */
   static int      init = FALSE; /* is mantissa variable initialized? */
   int             exponent;  /* exponent for floating point number */
   int             man_start; /* starting point of mantissa */
   int             s_pos;  /* position in build string */
   int             build_loop;
   int             need_pm;
   unsigned int    u;

   bwx_DEBUG(__FUNCTION__);

   /* initialize the variable if necessary */


   if (init == FALSE)
   {
      init = TRUE;
      var_make(&mantissa, NUMBER);
   }
   /* be sure that the array_pos[ 0 ] for mantissa is set to dim_base;
    * this is necessary because mantissa might be used before dim_base
    * is set */

   mantissa.array_pos[0] = dim_base;


   need_pm = FALSE;
   CURTASK         exps[CURTASK expsc].nval = 0;

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
      if ((expression[1] == 'H') || (expression[1] == 'h'))
      {
         base = 16;  /* hexadecimal constant */
         man_start = 2; /* starts at position 2 */
      }
      else
      {
         base = 8;   /* octal constant */
         if ((expression[1] == 'O') || (expression[1] == 'o'))
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

      sprintf(bwb_ebuf, "expression <%s> is not a numerical constant.",
         expression);
      bwb_error(bwb_ebuf);
      return OP_NULL;
   }

   /* now build the mantissa according to the numerical base */

   switch (base)
   {

   case 10:    /* decimal constant */

      /* initialize counters */

      CURTASK exps[CURTASK expsc].pos_adv = man_start;
      CURTASK         exps[CURTASK expsc].type = NUMBER;
      CURTASK         exps[CURTASK expsc].string[0] = '\0';
      s_pos = 0;
      exponent = OP_NULL;
      build_loop = TRUE;

      /* loop to build the string */

      while (build_loop == TRUE)
      {
         switch (expression[CURTASK exps[CURTASK expsc].pos_adv])
         {
         case '-':   /* prefixed plus or minus */
         case '+':

            /* in the first position, a plus or minus
             * sign can be added to the beginning of the
             * string to be scanned */

            if (CURTASK exps[CURTASK expsc].pos_adv == man_start)
            {
               CURTASK         exps[CURTASK expsc].string[s_pos] = expression[CURTASK exps[CURTASK expsc].pos_adv];
               ++CURTASK exps[CURTASK expsc].pos_adv; /* advance to next
                               * character */
               ++s_pos;
               CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
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
            CURTASK exps[CURTASK expsc].string[s_pos] = expression[CURTASK exps[CURTASK expsc].pos_adv];
            ++CURTASK exps[CURTASK expsc].pos_adv; /* advance to next
                            * character */
            ++s_pos;
            CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
            break;

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
            ++CURTASK exps[CURTASK expsc].pos_adv; /* advance to next
                            * character */
            CURTASK         exps[CURTASK expsc].type = NUMBER;
            exponent = FALSE;
            build_loop = FALSE;
            break;

         case 'E':   /* exponential, single precision */
         case 'e':
            ++CURTASK exps[CURTASK expsc].pos_adv; /* advance to next
                            * character */
            CURTASK         exps[CURTASK expsc].type = NUMBER;
            exponent = TRUE;
            build_loop = FALSE;
            break;

         case 'D':   /* exponential, double precision */
         case 'd':
            ++CURTASK exps[CURTASK expsc].pos_adv; /* advance to next
                            * character */
            CURTASK         exps[CURTASK expsc].type = NUMBER;
            exponent = TRUE;
            build_loop = FALSE;
            break;

         default: /* anything else, terminate */
            build_loop = FALSE;
            break;
         }

      }

      /* assign the value to the mantissa variable */

      sscanf(CURTASK exps[CURTASK expsc].string, BasicNumberScanFormat,
             var_findnval(&mantissa, mantissa.array_pos));


      /* test if integer bounds have been exceeded */

      if (CURTASK exps[CURTASK expsc].type == NUMBER)
      {
         int             i;
         BasicNumberType d;

         i = (int) var_getnval(&mantissa);
         d = (BasicNumberType) i;
         if (d != var_getnval(&mantissa))
         {
            CURTASK         exps[CURTASK expsc].type = NUMBER;
         }
      }
      /* read the exponent if there is one */

      if (exponent == TRUE)
      {

         /* allow a plus or minus once at the beginning */

         need_pm = TRUE;

         /* initialize counters */

         CURTASK         exps[CURTASK expsc].string[0] = '\0';
         s_pos = 0;
         build_loop = TRUE;

         /* loop to build the string */

         while (build_loop == TRUE)
         {
            switch (expression[CURTASK exps[CURTASK expsc].pos_adv])
            {
            case '-':   /* prefixed plus or minus */
            case '+':

               if (need_pm == TRUE) /* only allow once */
               {
                  CURTASK         exps[CURTASK expsc].string[s_pos] = expression[CURTASK exps[CURTASK expsc].pos_adv];
                  ++CURTASK exps[CURTASK expsc].pos_adv; /* advance to next
                                  * character */
                  ++s_pos;
                  CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
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

               CURTASK exps[CURTASK expsc].string[s_pos] = expression[CURTASK exps[CURTASK expsc].pos_adv];
               ++CURTASK exps[CURTASK expsc].pos_adv; /* advance to next
                               * character */
               ++s_pos;
               CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
               need_pm = FALSE;
               break;

            default: /* anything else, terminate */
               build_loop = FALSE;
               break;
            }

         }  /* end of build loop for exponent */

         /* assign the value to the user variable */

         sscanf(CURTASK exps[CURTASK expsc].string, BasicNumberScanFormat,
                &(CURTASK exps[CURTASK expsc].nval));


      }     /* end of exponent search */
      if (CURTASK exps[CURTASK expsc].nval == 0)
      {
         CURTASK         exps[CURTASK expsc].nval = var_getnval(&mantissa);
      }
      else
      {
         CURTASK         exps[CURTASK expsc].nval = var_getnval(&mantissa)
         * pow(10.0, CURTASK exps[CURTASK expsc].nval);
      }

      break;

   case 8:     /* octal constant */

      /* initialize counters */

      CURTASK exps[CURTASK expsc].pos_adv = man_start;
      CURTASK         exps[CURTASK expsc].type = NUMBER;
      CURTASK         exps[CURTASK expsc].string[0] = '\0';
      s_pos = 0;
      exponent = OP_NULL;
      build_loop = TRUE;

      /* loop to build the string */

      while (build_loop == TRUE)
      {
         switch (expression[CURTASK exps[CURTASK expsc].pos_adv])
         {
         case '0':   /* or ordinary digit */
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
            CURTASK exps[CURTASK expsc].string[s_pos] = expression[CURTASK exps[CURTASK expsc].pos_adv];
            ++CURTASK exps[CURTASK expsc].pos_adv; /* advance to next
                            * character */
            ++s_pos;
            CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
            break;

         default: /* anything else, terminate */
            build_loop = FALSE;
            break;
         }

      }

      /* now scan the string to determine the number */

      sscanf(CURTASK exps[CURTASK expsc].string, "%o", &u);
      CURTASK         exps[CURTASK expsc].nval = u;

      break;

   case 16:    /* hexadecimal constant */

      /* initialize counters */

      CURTASK exps[CURTASK expsc].pos_adv = man_start;
      CURTASK         exps[CURTASK expsc].type = NUMBER;
      CURTASK         exps[CURTASK expsc].string[0] = '\0';
      s_pos = 0;
      exponent = OP_NULL;
      build_loop = TRUE;

      /* loop to build the string */

      while (build_loop == TRUE)
      {
         switch (expression[CURTASK exps[CURTASK expsc].pos_adv])
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
            CURTASK exps[CURTASK expsc].string[s_pos] = expression[CURTASK exps[CURTASK expsc].pos_adv];

            ++CURTASK exps[CURTASK expsc].pos_adv; /* advance to next
                            * character */
            ++s_pos;
            CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
            break;

         default: /* anything else, terminate */
            build_loop = FALSE;
            break;
         }

      }

      /* now scan the string to determine the number */

      sscanf(CURTASK exps[CURTASK expsc].string, "%x", &u);
      CURTASK         exps[CURTASK expsc].nval = u;
      break;
   }

   /* note that the operation at this level is now a determined NUMBER */

   CURTASK         exps[CURTASK expsc].operation = NUMBER;


   return TRUE;

}

/***************************************************************
  
   FUNCTION:       exp_function()
  
   DESCRIPTION:    This function interprets a function,
         calling bwb_exp() (recursively) to resolve any
         parameters to the function.
  
***************************************************************/

struct bwb_variable *
var_chain(struct bwb_variable * argv)
{
   /* create a variable chain */
   struct bwb_variable *argn;

   bwx_DEBUG(__FUNCTION__);

   if (argv == NULL)
   {
      /* we are the first variable in the chain */
      argn = (struct bwb_variable *) CALLOC(1, sizeof(struct bwb_variable), "exp_function");
   }
   else
   {
      /* find the end of the chain */
      for (argn = argv; argn->next != NULL; argn = argn->next);
      /* add ourself to the end */
      argn->next = (struct bwb_variable *) CALLOC(1, sizeof(struct bwb_variable), "exp_function");
      argn = argn->next;
   }
   argn->next = NULL;
   /* return pointer to the variable just created */
   return argn;
}


int
exp_function(char *expression)
{
   struct exp_ese *e;
   int             s_pos;  /* position in build buffer */
   int             loop;
   int             paren_level;
   int             n_args;
   /* struct bwb_variable argv[ MAX_FARGS ]; *//* Removed by JBV */
   struct bwb_variable *argv; /* Added by JBV */
   struct bwb_variable *argn;
   bstring        *b;

   bwx_DEBUG(__FUNCTION__);


   argv = var_chain(NULL); /* RETURN variable */
   argn = NULL;

   /* assign pointers to argument stack */

   /* get the function name */

   exp_getvfname(expression, CURTASK exps[CURTASK expsc].string);


   /* now find the function itself */

   CURTASK         exps[CURTASK expsc].function = fnc_find(CURTASK exps[CURTASK expsc].string);

   /* check to see if it is valid */

   if (CURTASK exps[CURTASK expsc].function == NULL)
   {
      sprintf(bwb_ebuf, "Failed to find function <%s>.",
         CURTASK exps[CURTASK expsc].string);
      bwb_error(bwb_ebuf);
      return OP_ERROR;
   }
   /* note that this level is a function */

   CURTASK         exps[CURTASK expsc].operation = FUNCTION;
   CURTASK         exps[CURTASK expsc].pos_adv = strlen(CURTASK exps[CURTASK expsc].string);

   /* check for begin parenthesis */

   loop = TRUE;
   while (loop == TRUE)
   {
      switch (expression[CURTASK exps[CURTASK expsc].pos_adv])
      {

      case ' ':   /* whitespace */
         ++CURTASK exps[CURTASK expsc].pos_adv; /* advance */
         break;

      case '(':   /* begin paren */


         ++CURTASK exps[CURTASK expsc].pos_adv; /* advance beyond it */
         paren_level = 1;  /* set paren_level */
         loop = FALSE;  /* and break out */
         break;

      default: /* anything else */
         loop = FALSE;
         paren_level = 0;  /* do not look for parameters */
         break;
      }
   }

   /* find parameters within parentheses */
   /* for each argument, find a string ending with ',' or with end
    * parenthesis */

   n_args = 0;
   s_pos = 0;
   CURTASK         exps[CURTASK expsc].string[0] = '\0';

   while (paren_level > 0)
   {

      /* check the current character */

      switch (expression[CURTASK exps[CURTASK expsc].pos_adv])
      {

      case ',':   /* end of an argument */

         if (paren_level == 1)   /* ignore ',' within
                   * parentheses */
         {

            /* call bwb_exp() recursively to resolve the
             * argument */

            if (exp_validarg(CURTASK exps[CURTASK expsc].string) == TRUE)
            {

               CURTASK         exps[CURTASK expsc].rec_pos = 0;
               e = bwb_exp(CURTASK exps[CURTASK expsc].string, FALSE,
                      &(CURTASK exps[CURTASK expsc].rec_pos));

               /* assign operation and value at this
                * level */
               argn = var_chain(argv);
               /* 'argn' is the vaiable to use */
               var_make(argn, e->type);
               switch (argn->type)
               {
               case NUMBER:
                  *var_findnval(argn, argn->array_pos)
                     = exp_getnval(e);
                  break;
               case STRING:
                  str_btob(var_findsval(argn,
                              argn->array_pos), exp_getsval(e));
                  break;
               }
               ++n_args;   /* increment number of
                      * parameters */

            }
            s_pos = 0;  /* reset counter */
            CURTASK         exps[CURTASK expsc].string[0] = '\0';
         }
         else
         {
            CURTASK         exps[CURTASK expsc].string[s_pos]
            = expression[CURTASK exps[CURTASK expsc].pos_adv];
            ++s_pos;
            CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
         }
         break;

      case '(':
         ++paren_level;
         CURTASK         exps[CURTASK expsc].string[s_pos]
         = expression[CURTASK exps[CURTASK expsc].pos_adv];
         ++s_pos;
         CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
         break;

      case ')':
         --paren_level;


         if (paren_level == 0)
         {


            /* call bwb_exp() recursively to resolve the
             * argument */

            if (exp_validarg(CURTASK exps[CURTASK expsc].string) == TRUE)
            {

               CURTASK         exps[CURTASK expsc].rec_pos = 0;
               e = bwb_exp(CURTASK exps[CURTASK expsc].string, FALSE,
                      &(CURTASK exps[CURTASK expsc].rec_pos));


               /* assign operation and value at this
                * level */
               argn = var_chain(argv);
               /* 'argn' is the vaiable to use */
               var_make(argn, e->type);
               switch (argn->type)
               {
               case NUMBER:
                  *var_findnval(argn, argn->array_pos)
                     = exp_getnval(e);
                  break;
               case STRING:
                  str_btob(var_findsval(argn,
                              argn->array_pos), exp_getsval(e));
                  break;
               }
               ++n_args;   /* increment number of
                      * parameters */

            }
            s_pos = 0;  /* reset counter */
            CURTASK         exps[CURTASK expsc].string[0] = '\0';
         }
         else
         {
            CURTASK         exps[CURTASK expsc].string[s_pos]
            = expression[CURTASK exps[CURTASK expsc].pos_adv];
            ++s_pos;
            CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
         }
         break;

      case '\"':  /* embedded string constant */

         /* add the initial quotation mark */

         CURTASK exps[CURTASK expsc].string[s_pos]
            = expression[CURTASK exps[CURTASK expsc].pos_adv];
         ++s_pos;
         CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
         ++CURTASK exps[CURTASK expsc].pos_adv;

         /* add intervening characters */

         while ((expression[CURTASK exps[CURTASK expsc].pos_adv] != '\"')
                && (expression[CURTASK exps[CURTASK expsc].pos_adv] != '\0'))
         {
            CURTASK         exps[CURTASK expsc].string[s_pos]
            = expression[CURTASK exps[CURTASK expsc].pos_adv];
            ++s_pos;
            CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
            ++CURTASK exps[CURTASK expsc].pos_adv;
         }

         /* add the concluding quotation mark */

         CURTASK         exps[CURTASK expsc].string[s_pos]
         = expression[CURTASK exps[CURTASK expsc].pos_adv];
         ++s_pos;
         CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
         /* the following bracketed out 14 July 1992; since
          * this counter */
         /* incremented at the end of the switch statement,
          * this may */
         /* increment it past the next character needed */
         /* ++CURTASK exps[ CURTASK expsc ].pos_adv; */
         break;

      default:
         CURTASK exps[CURTASK expsc].string[s_pos]
            = expression[CURTASK exps[CURTASK expsc].pos_adv];
         ++s_pos;
         CURTASK         exps[CURTASK expsc].string[s_pos] = '\0';
         break;
      }

      /* advance the counter */

      ++CURTASK exps[CURTASK expsc].pos_adv;

   }


   /* call the function vector */


   if (TRUE)
   {
      /* Compare the parameter signature of the caller to the
       * callee. This allows for function overloading based upon
       * different parameter signatures. It also eliminates the
       * need for any intrinsic function to check it's parameter
       * count or check the type of it's parameters.  The function
       * will only be callled with the correct type and count of
       * parameters.  Examples of common intrinsic functions which
       * are overloaded based upon the parameter signature are
       * INSTR and MID$. INSTR( [Start], In$, Find$ ) -> instr(
       * string, string )          -> 2 parameters instr( number,
       * string, string )  -> 3 parameters */
      struct bwb_function *f;
      char           *name;

      f = CURTASK exps[CURTASK expsc].function;
      name = f->Name;


      /* There are two choices for USER DEFINE FUNCTIONS: #1 - A
       * fixed number of upto 32 parameters, declared as FUNCTION
       * FRED( A$, B$, X, Y ) #2 - A variant number of upto MAXINT
       * parameters, declared as FUNCTION FRED( ... ) */

      /* Determine the caller's parameter signature, using an
       * algorithm matching the intrinsic table: First argument is
       * bit 0; if it is 1 then STRING else NUMBER. Second argument
       * is bit 1, and so on. */
      if (TRUE)
      {
         /* VERIFY */
         unsigned char   ParameterCount;
         unsigned long   ParameterTypes;

         if (n_args > 32)
         {
            /* VARIANT */
            ParameterCount = 0xFF;
            ParameterTypes = 0;
         }
         else
         {
            int             i;
            ParameterCount = n_args;   /* <= 32 */
            ParameterTypes = 0;
            argn = argv;
            for (i = 0; i < ParameterCount; i++)
            {
               argn = argn->next;
               if (argn->type == STRING)
               {
                  ParameterTypes |= (1 << i);
               }
            }
         }
         /* did we find the correct function above? */
         if (f->ParameterCount != ParameterCount || f->ParameterTypes != ParameterTypes)
         {
            /* oops */

            f = fnc_find_exact(name, ParameterCount, ParameterTypes);
            if (f == NULL)
            {
               /* NOT FOUND */
               f = fnc_find_exact(name, 0xFF, 0);  /* look for VARIANT */
            }
            if (f == NULL)
            {
               /* NOT FOUND */
               sprintf(bwb_ebuf, "*** Illegal Function Call (%s) ***", name);
               bwb_error(bwb_ebuf);
               return OP_ERROR;
            }
            /* FOUND */
            CURTASK         exps[CURTASK expsc].function = f;
         }
      }
   }
   /* defaullt the return value */
#define RESULT_NUMBER  *argv->memnum
#define RESULT_BUFFER   argv->memstr->sbuffer
#define RESULT_LENGTH   argv->memstr->length
   /* defaullt the return value */
   {
      if ((CURTASK exps[CURTASK expsc].function->ReturnType & 0x00FF) == STRING)
      {
         var_make(argv, STRING);
         RESULT_BUFFER = (char *) CALLOC(BasicStringLengthMax + 1, 1, "exp_function");
         RESULT_LENGTH = 0;
         RESULT_BUFFER[RESULT_LENGTH] = '\0';
      }
      else
      {
         var_make(argv, NUMBER);
         RESULT_NUMBER = 0;
      }
      strcpy(argv->name, CURTASK exps[CURTASK expsc].function->Name);
   }
   if (CURTASK exps[CURTASK expsc].function->ReturnType & 0xFF00  /* UniqueID is a line
           number */ )
   {
      /* for all USER DEFINED FUNCTIONS: f->UniqueID == line number
       * of DEF FN... */
      unsigned short  cmdnum;
      cmdnum = (CURTASK exps[CURTASK expsc].function->ReturnType & 0xFF00) >> 8;
      switch (cmdnum)
      {
      case C_DEF: /* execute a user function declared using DEF
             * FN...(...) = ... */
      case C_FUNCTION:  /* execute a user function declared
                * using FUNCTION ...(...) */
      case C_SUB: /* execute a user subroutine declared using
             * SUB ...(...) */
         fnc_deffn(n_args, argv, CURTASK exps[CURTASK expsc].function->UniqueID);
         break;
      case C_USER_LBL:  /* IF ERL > label1 AND ERL < label2
                * THEN ... */
         if (n_args > 0)
         {
            bwb_error("SYNTAX ERROR");
         }
         /* return the line number associated with the label */
         RESULT_NUMBER = CURTASK exps[CURTASK expsc].function->UniqueID;
         break;
      default:
         sprintf(bwb_ebuf, "INTERNAL ERROR, in exp_function(): unexpected cmdnum %d", cmdnum);
         bwb_error(bwb_ebuf);
         break;
      }
   }
   else
   {
      /* for all INTRINSIC FUNCTIONS: f->UniqueID == #define F_... */
      fnc_intrinsic(n_args, argv, CURTASK exps[CURTASK expsc].function->UniqueID);
   }


   CURTASK         exps[CURTASK expsc].type = (char) argv->type;

   switch (argv->type)
   {
   case STRING:
      CURTASK exps[CURTASK expsc].operation = CONST_STRING;


      b = var_findsval(argv, argv->array_pos);
      str_btob(exp_getsval(&(CURTASK exps[CURTASK expsc])), b);


      break;

   default:
      CURTASK exps[CURTASK expsc].operation = NUMBER;
      CURTASK         exps[CURTASK expsc].nval = var_getnval(argv);
      break;
   }
   /*-------------------------------------------------*/
   /* Now free the argv memory                        */
   /* (some other less fortunate routine may need it) */
   /* JBV, 10/95                                      */
   /*-------------------------------------------------*/

   var_free(argv);
   /* return */

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       exp_variable()
  
   DESCRIPTION:    This function interprets a variable.
  
***************************************************************/

int
exp_variable(char *expression)
{
   int             pos;
   register int    n;
   struct bwb_variable *v;
   bstring        *b;
   int             n_params;
   int             pp[MAX_DIMS];

   bwx_DEBUG(__FUNCTION__);

   /* get the variable name */

   exp_getvfname(expression, CURTASK exps[CURTASK expsc].string);

   /* now find the variable itself */

   v = CURTASK exps[CURTASK expsc].xvar = var_find(CURTASK exps[CURTASK expsc].string);


   /* note that this level is a variable */

   CURTASK         exps[CURTASK expsc].operation = VARIABLE;

   /* read subscripts */

   pos = strlen(CURTASK exps[CURTASK expsc].string);
   if ((v->dimensions == 1) && (v->array_sizes[0] == 1))
   {
      pos = strlen(v->name);
      n_params = 1;
      pp[0] = dim_base;
   }
   else
   {
      v->IsInDim++;
      n_params = dim_getparams_FIXED(expression, &pos, pp);
      v->IsInDim--;
   }
   if (n_params != v->dimensions)
   {
      sprintf(bwb_ebuf, "DIMENSION OUT OF RANGE %s(%d)", v->name, n_params);
      bwb_error(bwb_ebuf);
      return FALSE;
   }
   CURTASK         exps[CURTASK expsc].pos_adv = pos;
   for (n = 0; n < v->dimensions; ++n)
   {
      /* newvar->array_sizes[ n ] = pp[ n ] + ( 1 - dim_base ); */
      if ((pp[n] < dim_base) || ((pp[n] + (1 - dim_base)) > v->array_sizes[n]))
      {
         sprintf(bwb_ebuf, "SUBSCRIPT OUT OF RANGE %s(%d)", v->name, pp[n]);
         bwb_error(bwb_ebuf);
         return FALSE;
      }
      CURTASK         exps[CURTASK expsc].array_pos[n] = v->array_pos[n] = pp[n];
   }


   /* assign the type and value at this level */

   CURTASK         exps[CURTASK expsc].type = (char) v->type;

   switch (v->type)
   {
   case STRING:
      b = var_findsval(v, v->array_pos);
      str_btob(&(CURTASK exps[CURTASK expsc].sval), b);
      break;
   default:
      CURTASK exps[CURTASK expsc].nval = var_getnval(v);



      break;
   }


   /* return */

   return TRUE;

}

/* EOF */
