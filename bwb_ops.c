/****************************************************************
  
        bwb_ops.c       Expression Parsing Operations
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



/* declarations for functions visible in this file only */

static int
op_oplevel(int level);
static int
op_add(int level, int precision);
static int
op_subtract(int level, int precision);
static int
op_multiply(int level, int precision);
static int
op_divide(int level, int precision);
static int
op_assign(int level, int precision);
static int
op_equals(int level, int precision);
static int
op_lessthan(int level, int precision);
static int
op_greaterthan(int level, int precision);
static int
op_lteq(int level, int precision);
static int
op_gteq(int level, int precision);
static int
op_notequal(int level, int precision);
static int
op_modulus(int level, int precision);
static int
op_exponent(int level, int precision);
static int
op_intdiv(int level, int precision);
static int
op_or(int level, int precision);
static int
op_and(int level, int precision);
static int
op_not(int level, int precision);
static int
op_xor(int level, int precision);
static int
op_imp(int level, int precision);
static int
op_eqv(int level, int precision);
static int
op_negation(int level, int precision); /* JBV */
static int
op_posation(int level, int precision);
static int
op_islevelstr(int level);
static int
op_getprecision(int level);
static int
op_isoperator(int operation);
static int
op_pulldown(int how_far);

static int      op_level;

/***************************************************************
  
   FUNCTION:       exp_operation()
  
   DESCRIPTION:    This function performs whatever operations
         are necessary at the end of function bwb_exp()
         (i.e., the end of the parsing of an expression;
         see file bwb_exp.c).
  
***************************************************************/

int
exp_operation(int entry_level)
{
   register int    precedence;
   int             operator;

   bwx_DEBUG(__FUNCTION__);

   /* cycle through all levels of precedence and perform required
    * operations */

   for (precedence = 0; precedence <= MAX_PRECEDENCE; ++precedence)
   {

      /* Operation loop: cycle through every level above entry
       * level and perform required operations as needed */

      op_level = entry_level + 1;
      while ((op_level < CURTASK expsc)
             && (op_isoperator(CURTASK exps[op_level].operation) == FALSE))
      {
         ++op_level;
      }

      while ((op_level > entry_level) && (op_level < CURTASK expsc))
      {

         /* see if the operation at this level is an operator
          * with the appropriate precedence level by running
          * through the table of operators */

         for (operator = 0; operator < NUM_OPERATORS; ++operator)
         {

            if (exp_ops[operator].operation == CURTASK exps[op_level].operation)
            {

               /* check for appropriate level of
                * precedence */

               if (exp_ops[operator].precedence == precedence)
               {


                  op_oplevel(op_level);   /* perform the operation */

               }
            }
         }

         /* advance level if appropriate; one must check,
          * however, since the op_oplevel() function may have
          * decremented CURTASK expsc */

         if (op_level < CURTASK expsc)
         {
            ++op_level;


            while ((op_isoperator(CURTASK exps[op_level].operation) == FALSE)
                   && (op_level < CURTASK expsc))
            {
               ++op_level;


            }
         }  /* end of increment of op_level */
      }     /* end of for loop for stack levels */

   }        /* end of for loop for precedence levels */

   return TRUE;

}           /* end of function exp_operation() */


/***************************************************************
  
   FUNCTION:       op_oplevel()
  
   DESCRIPTION:    This function performs a specific operation
         at a specific level as the expression parser
         resolves its arguments.
  
***************************************************************/

static int
op_oplevel(int level)
{
   int             precision;

   bwx_DEBUG(__FUNCTION__);

   /* set the precision */

   if ((precision = op_getprecision(level)) == OP_ERROR)
   {
      op_pulldown(2);
      sprintf(bwb_ebuf, "exp_operation(): failed to set precision.");
      bwb_error(bwb_ebuf);
      return FALSE;
   }
   /* precision is set correctly */

   else
   {

      switch (CURTASK exps[level].operation)
      {
      case OP_ADD:
         op_add(level, precision);
         break;

      case OP_SUBTRACT:
         op_subtract(level, precision);
         break;

      case OP_MULTIPLY:
         op_multiply(level, precision);
         break;

      case OP_DIVIDE:
         op_divide(level, precision);
         break;

      case OP_ASSIGN:
         op_assign(level, precision);
         break;

      case OP_EQUALS:
         op_equals(level, precision);
         break;

      case OP_LESSTHAN:
         op_lessthan(level, precision);
         break;

      case OP_GREATERTHAN:
         op_greaterthan(level, precision);
         break;

      case OP_LTEQ:
         op_lteq(level, precision);
         break;

      case OP_GTEQ:
         op_gteq(level, precision);
         break;

      case OP_NOTEQUAL:
         op_notequal(level, precision);
         break;

      case OP_MODULUS:
         op_modulus(level, precision);
         break;

      case OP_INTDIVISION:
         op_intdiv(level, precision);
         break;

      case OP_OR:
         op_or(level, precision);
         break;

      case OP_AND:
         op_and(level, precision);
         break;

      case OP_NOT:
         op_not(level, precision);
         break;

      case OP_XOR:
         op_xor(level, precision);
         break;

      case OP_IMPLIES:
         op_imp(level, precision);
         break;

      case OP_EQUIV:
         op_eqv(level, precision);
         break;

      case OP_EXPONENT:
         op_exponent(level, precision);
         break;

      case OP_NEGATION: /* JBV */
         op_negation(level, precision);
         break;

      case OP_POSATION:
         op_posation(level, precision);
         break;

      default:
         sprintf(bwb_ebuf, "PROGRAMMING ERROR: operator <%d> not (yet) supported.", CURTASK exps[level].operation);
         op_pulldown(2);
         bwb_error(bwb_ebuf);
         return FALSE;
         break;
      }     /* end of case statement for operators */
   }        /* end of else statement, precision set */

   return TRUE;

}           /* end of function op_oplevel() */

/***************************************************************
  
   FUNCTION:       op_isoperator()
  
   DESCRIPTION:    This function detects whether its argument
         is an operator.
  
***************************************************************/

static int
op_isoperator(int operation)
{
   register int    c;

   bwx_DEBUG(__FUNCTION__);

   for (c = 0; c < NUM_OPERATORS; ++c)
   {
      if (operation == exp_ops[c].operation)
      {
         return TRUE;
      }
   }

   /* test failed; return FALSE */


   return FALSE;

}

/***************************************************************
  
        FUNCTION:    op_add()
  
        DESCRIPTION:    This function adds two numbers or
            concatenates two strings.
  
***************************************************************/

static int
op_add(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be strings for string
       * addition; if not, report an error */

      if ((op_islevelstr(level - 1) != TRUE)
          || (op_islevelstr(level + 1) != TRUE))
      {
         sprintf(bwb_ebuf, "in op_add(): Type mismatch in string addition.");
         bwb_error(bwb_ebuf);
         return FALSE;
      }
      /* concatenate the two strings */

      {
         static bstring  b;   /* JBV */
         b.rab = FALSE; /* JBV */


         str_btob(&b, exp_getsval(&(CURTASK exps[level - 1])));
         str_cat(&b, exp_getsval(&(CURTASK exps[level + 1])));
         str_btob(&(CURTASK exps[level - 1].sval), &b);
         CURTASK         exps[level - 1].operation = CONST_STRING;

      }
      break;

   case NUMBER:
      CURTASK exps[level - 1].nval
         = exp_getnval(&(CURTASK exps[level - 1]))
         + exp_getnval(&(CURTASK exps[level + 1]));
      CURTASK         exps[level - 1].operation = NUMBER;
      break;

   }

   /* set variable to requested precision */

   CURTASK         exps[level - 1].type = (char) precision;


   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_subtract()
  
   DESCRIPTION:    This function subtracts the number on
         the left from the number on the right.
  
***************************************************************/

static int
op_subtract(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be subtracted.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:
      CURTASK exps[level - 1].nval
         = exp_getnval(&(CURTASK exps[level - 1]))
         - exp_getnval(&(CURTASK exps[level + 1]));
      break;

   }

   /* set variable to requested precision */

   CURTASK         exps[level - 1].type = (char) precision;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_multiply()
  
   DESCRIPTION:    This function multiplies the number on
         the left by the number on the right.
  
***************************************************************/

static int
op_multiply(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be multiplied.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:
      CURTASK exps[level - 1].nval
         = exp_getnval(&(CURTASK exps[level - 1]))
         * exp_getnval(&(CURTASK exps[level + 1]));
      break;

   }

   /* set variable to requested precision */

   CURTASK         exps[level - 1].type = (char) precision;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_divide()
  
   DESCRIPTION:    This function divides the number on
         the left by the number on the right.
  
***************************************************************/

static int
op_divide(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for
       * division; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be divided.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:

      if (exp_getnval(&(CURTASK exps[level + 1]))
          == 0)
      {
         /* - Evaluation of an expression results in division
          * by zero (nonfatal, the recommended recovery
          * procedure is to supply machine infinity with the
          * sign of the numerator and continue) */

         if (exp_getnval(&(CURTASK exps[level - 1])) < 0)
         {
            /* NEGATIVE */
            CURTASK         exps[level - 1].nval = -(DBL_MAX); /* NEGATIVE INFINITY */
         }
         else
         {
            /* POSITIVE  */
            CURTASK         exps[level - 1].nval = (DBL_MAX);  /* POSITIVE INFINITY */
         }
         bwb_Warning_Overflow("*** WARNING:  Divide by 0 ***");
      }
      else
      {
         CURTASK         exps[level - 1].nval
         = exp_getnval(&(CURTASK exps[level - 1]))
         / exp_getnval(&(CURTASK exps[level + 1]));
      }

      break;
   }

   /* set variable to requested precision */

   CURTASK         exps[level - 1].type = (char) precision;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}


BasicNumberType
VerifyNumeric(BasicNumberType Value)
{
   bwx_DEBUG(__FUNCTION__);
   /* check Value */
   if (isnan(Value))
   {
      /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
      bwb_error("NOT A NUMBER");
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
      bwb_Warning_Overflow("*** Arithmetic Overflow ***");
   }
   return Value;
}


/***************************************************************
  
   FUNCTION:       op_assign()
  
   DESCRIPTION:    This function assigns the value in the
         right hand side to the variable in the
         left hand side.
  
***************************************************************/

static int
op_assign(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   /* Make sure the position one level below is a variable */

   if (CURTASK exps[level - 1].operation != VARIABLE)
   {
      op_pulldown(2);
      sprintf(bwb_ebuf, "in op_assign(): Assignment must be to variable: level -1 <%d> op <%d>",
         level - 1, CURTASK exps[level - 1].operation);
      bwb_error(bwb_ebuf);
      return FALSE;
   }
   if (CURTASK exps[level - 1].type != CURTASK exps[level + 1].type)
   {
      bwb_error("Type Mismatch");
      return FALSE;
   }
   /* if the assignment is numerical, then the precision should be set
    * to that of the variable on the left-hand side of the assignment */

   if (precision != STRING)
   {
      precision = (int) CURTASK exps[level - 1].type;
   }
   switch (precision)
   {
   case STRING:


      str_btob(exp_getsval(&(CURTASK exps[level - 1])),
          exp_getsval(&(CURTASK exps[level + 1])));
      break;

   case NUMBER:
      if (TRUE)
      {
         double          Value;
         Value = exp_getnval(&(CURTASK exps[level + 1]));
         Value = VerifyNumeric(Value);
         /* assign Value */
         *var_findnval(CURTASK exps[level - 1].xvar,
                  CURTASK exps[level - 1].array_pos) =
            CURTASK exps[level - 1].nval =
            Value;
      }
      break;

   default:
      sprintf(bwb_ebuf, "in op_assign(): Variable before assignment operator has unidentified type.");
      bwb_error(bwb_ebuf);
      return FALSE;

   }

   /* set variable to requested precision */

   CURTASK         exps[level - 1].type = (char) precision;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_equals()
  
   DESCRIPTION:    This function compares two values and
         returns an integer value: TRUE if they are
         the same and FALSE if they are not.
  
***************************************************************/

static int
op_equals(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be strings for string
       * addition; if not, report an error */

      if ((op_islevelstr(level - 1) != TRUE)
          || (op_islevelstr(level + 1) != TRUE))
      {
         sprintf(bwb_ebuf, "in op_equals(): Type mismatch in string comparison.");
         bwb_error(bwb_ebuf);
         return FALSE;
      }
      /* compare the two strings */

      if (str_cmp(exp_getsval(&(CURTASK exps[level - 1])),
             exp_getsval(&(CURTASK exps[level + 1]))) == 0)
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   case NUMBER:
      if (exp_getnval(&(CURTASK exps[level - 1]))
          == exp_getnval(&(CURTASK exps[level + 1])))
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   }

   /* set variable to integer and operation to NUMBER: this must be done
    * at the end, since at the beginning it might cause op_islevelstr()
    * to return a false error */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_lessthan()
  
   DESCRIPTION:    This function compares two values and
         returns an integer value: TRUE if the
         left hand value is less than the right,
         and FALSE if it is not.
  
***************************************************************/

static int
op_lessthan(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      if ((op_islevelstr(level - 1) != TRUE)
          || (op_islevelstr(level + 1) != TRUE))
      {
         sprintf(bwb_ebuf, "Type mismatch in string comparison.");
         bwb_error(bwb_ebuf);
         return FALSE;
      }
      /* compare the two strings */

      if (str_cmp(exp_getsval(&(CURTASK exps[level - 1])),
             exp_getsval(&(CURTASK exps[level + 1]))) < 0)
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   case NUMBER:
      if (exp_getnval(&(CURTASK exps[level - 1]))
          < exp_getnval(&(CURTASK exps[level + 1])))
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   }

   /* set variable to integer and operation to NUMBER: this must be done
    * at the end, since at the beginning it might cause op_islevelstr()
    * to return a false error */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_greaterthan()
  
   DESCRIPTION:    This function compares two values and
         returns an integer value: TRUE if the
         left hand value is greater than the right,
         and FALSE if it is not.
  
***************************************************************/

static int
op_greaterthan(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      if ((op_islevelstr(level - 1) != TRUE)
          || (op_islevelstr(level + 1) != TRUE))
      {
         sprintf(bwb_ebuf, "Type mismatch in string comparison.");
         bwb_error(bwb_ebuf);
         return FALSE;
      }
      /* compare the two strings */

      if (str_cmp(exp_getsval(&(CURTASK exps[level - 1])),
             exp_getsval(&(CURTASK exps[level + 1]))) > 0)
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   case NUMBER:
      if (exp_getnval(&(CURTASK exps[level - 1]))
          > exp_getnval(&(CURTASK exps[level + 1])))
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   }

   /* set variable to integer and operation to NUMBER: this must be done
    * at the end, since at the beginning it might cause op_islevelstr()
    * to return a false error */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_lteq()
  
   DESCRIPTION:    This function compares two values and
         returns an integer value: TRUE if the
         left hand value is less than or equal
         to the right, and FALSE if it is not.
  
***************************************************************/

static int
op_lteq(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      if ((op_islevelstr(level - 1) != TRUE)
          || (op_islevelstr(level + 1) != TRUE))
      {
         sprintf(bwb_ebuf, "Type mismatch in string comparison.");
         bwb_error(bwb_ebuf);
         return FALSE;
      }
      /* compare the two strings */

      if (str_cmp(exp_getsval(&(CURTASK exps[level - 1])),
             exp_getsval(&(CURTASK exps[level + 1]))) <= 0)
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   case NUMBER:
      if (exp_getnval(&(CURTASK exps[level - 1]))
          <= exp_getnval(&(CURTASK exps[level + 1])))
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   }

   /* set variable to integer and operation to NUMBER: this must be done
    * at the end, since at the beginning it might cause op_islevelstr()
    * to return a false error */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_gteq()
  
   DESCRIPTION:    This function compares two values and
         returns an integer value: TRUE if the
         left hand value is greater than or equal
         to the right, and FALSE if it is not.
  
***************************************************************/

static int
op_gteq(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      if ((op_islevelstr(level - 1) != TRUE)
          || (op_islevelstr(level + 1) != TRUE))
      {
         sprintf(bwb_ebuf, "Type mismatch in string comparison.");
         bwb_error(bwb_ebuf);
         return FALSE;
      }
      /* compare the two strings */

      if (str_cmp(exp_getsval(&(CURTASK exps[level - 1])),
             exp_getsval(&(CURTASK exps[level + 1]))) >= 0)
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   case NUMBER:
      if (exp_getnval(&(CURTASK exps[level - 1]))
          >= exp_getnval(&(CURTASK exps[level + 1])))
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   }

   /* set variable to integer and operation to NUMBER: this must be done
    * at the end, since at the beginning it might cause op_islevelstr()
    * to return a false error */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_notequal()
  
   DESCRIPTION:    This function compares two values and
         returns an integer value: TRUE if they
         are not the same and FALSE if they are.
  
***************************************************************/

static int
op_notequal(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      if ((op_islevelstr(level - 1) != TRUE)
          || (op_islevelstr(level + 1) != TRUE))
      {
         sprintf(bwb_ebuf, "Type mismatch in string comparison.");
         bwb_error(bwb_ebuf);
         return FALSE;
      }
      /* compare the two strings */

      if (str_cmp(exp_getsval(&(CURTASK exps[level - 1])),
             exp_getsval(&(CURTASK exps[level + 1]))) != 0)
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   case NUMBER:
      if (exp_getnval(&(CURTASK exps[level - 1]))
          != exp_getnval(&(CURTASK exps[level + 1])))
      {
         CURTASK         exps[level - 1].nval = TRUE;
      }
      else
      {
         CURTASK         exps[level - 1].nval = FALSE;
      }
      break;

   }

   /* set variable to integer and operation to NUMBER: this must be done
    * at the end, since at the beginning it might cause op_islevelstr()
    * to return a false error */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_modulus()
  
   DESCRIPTION:    This function divides the number on
         the left by the number on the right
         and returns the remainder.
  
***************************************************************/

static int
op_modulus(int level, int precision)
{
   static double   iportion;

   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be divided.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:
      {
         /* N = X MOD Y */
         if (exp_getnval(&(CURTASK exps[level + 1]))
             == 0)
         {
            CURTASK         exps[level - 1].nval = -1;
            op_pulldown(2);
            sprintf(bwb_ebuf, "Modulus by 0.");
            bwb_error(bwb_ebuf);
            return FALSE;
         }
         CURTASK         exps[level].nval
         = exp_getnval(&(CURTASK exps[level - 1]))
         / exp_getnval(&(CURTASK exps[level + 1]));
         modf((double) CURTASK exps[level].nval, &iportion);
         CURTASK         exps[level - 1].nval
         = exp_getnval(&(CURTASK exps[level - 1]))
         - (exp_getnval(&(CURTASK exps[level + 1]))
            * iportion);
      }
      break;

   }

   /* set variable to requested precision */

   CURTASK         exps[level - 1].type = (char) precision;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_exponent()
  
   DESCRIPTION:    This function finds the exponential value
         of a number (on the left) to the power
         indicated on the right-hand side.
  
***************************************************************/

static int
op_exponent(int level, int precision)
{

   bwx_DEBUG(__FUNCTION__);


   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be taken as exponents.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:
      if (TRUE)
      {
         /* N = X ^ Y */
         BasicNumberType X;
         BasicNumberType Y;
         BasicNumberType N;

         X = exp_getnval(&(CURTASK exps[level - 1]));
         Y = exp_getnval(&(CURTASK exps[level + 1]));

         if (X < 0 && Y != (int) Y)
         {
            /*** FATAL ***/
            /* - Evaluation of the operation of
             * involution results in a nega- tive number
             * being raised to a non-integral power
             * (fatal). */
            N = 0;
            bwb_error("NEGATIVE QUANTITY RAISED TO A NON-INTEGRAL POWER");
            return FALSE;
         }
         else
         if (X == 0 && Y < 0)
         {
            /* - Evaluation of the operation of
             * involution results in a zero be- ing
             * raised to a negative value (nonfatal, the
             * recommended re- covery procedure is to
             * supply positive machine infinity and
             * continue). */

            N = (DBL_MAX);
            bwb_Warning_Overflow("*** Arithmetic Overflow ***");
         }
         else
         {
            N = pow(X, Y);
         }
         CURTASK         exps[level - 1].nval = N;
      }
      break;

   }

   /* set variable to requested precision */

   CURTASK         exps[level - 1].type = (char) precision;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_intdiv()
  
   DESCRIPTION:    This function divides the number on
         the left by the number on the right,
         returning the result as an integer.
  
***************************************************************/

static int
op_intdiv(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for string
       * addition; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be divided.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   default:
      {
         /* N = X \ Y  */
         long            X;
         long            Y;
         long            N;
         X = exp_getival(&(CURTASK exps[level - 1]));
         Y = exp_getival(&(CURTASK exps[level + 1]));
         if (Y == 0)
         {
            sprintf(bwb_ebuf, "Integer Divide by 0.");
            bwb_error(bwb_ebuf);
            return FALSE;
         }
         N = X / Y;
         CURTASK         exps[level - 1].nval = N;
      }
      break;
   }

   /* set variable to requested precision */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_or()
  
   DESCRIPTION:    This function compares two integers and
         performs a logical OR on them.
  
***************************************************************/

static int
op_or(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for logical
       * comparison; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be compared logically.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:
      {
         /* N = X OR Y  */
         long            X;
         long            Y;
         long            N;
         X = exp_getival(&(CURTASK exps[level - 1]));
         Y = exp_getival(&(CURTASK exps[level + 1]));
         N = X | Y;
         CURTASK         exps[level - 1].nval = N;
      }
      break;

   }

   /* set variable type to integer */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_and()
  
   DESCRIPTION:    This function compares two integers and
         performs a logical AND on them.
  
***************************************************************/

static int
op_and(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:


      /* both sides of the operation should be numbers for logical
       * comparison; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be compared logically.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:
      {
         /* N = X AND Y  */
         long            X;
         long            Y;
         long            N;
         X = exp_getival(&(CURTASK exps[level - 1]));
         Y = exp_getival(&(CURTASK exps[level + 1]));
         N = X & Y;
         CURTASK         exps[level - 1].nval = N;
      }
      break;

   }

   /* set variable type to integer */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_not()
  
   DESCRIPTION:    This function performs a logical NOT on
         the integer to the right.
  
***************************************************************/

static int
op_not(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:


      /* both sides of the operation should be numbers for logical
       * comparison; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be compared logically.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   default:
      {
         /* N = NOT X  */
         long            X;
         long            N;
         X = exp_getival(&(CURTASK exps[level + 1]));
         N = ~X;
         CURTASK         exps[level /*- 1*/ ].nval = N;
      }
      break;
   }

   /* set variable type to integer */

   CURTASK         exps[level].type = NUMBER;
   CURTASK         exps[level].operation = NUMBER;

   /* decrement the stack once */

   op_pulldown(1);


   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_xor()
  
   DESCRIPTION:    This function compares two integers and
         performs a logical XOR on them.
  
***************************************************************/

static int
op_xor(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for logical
       * comparison; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be compared logically.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:
      {
         /* N = X XOR Y  */
         long            X;
         long            Y;
         long            N;
         X = exp_getival(&(CURTASK exps[level - 1]));
         Y = exp_getival(&(CURTASK exps[level + 1]));
         N = X ^ Y;
         CURTASK         exps[level - 1].nval = N;
      }
      break;

   }

   /* set variable type to integer */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_eqv()
  
   DESCRIPTION:    This function compares two integers and
         performs a logical EQV on them.
  
***************************************************************/

static int
op_eqv(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for logical
       * comparison; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be compared logically.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:
      {
         /* N = X EQV Y = NOT ( X XOR Y ) */
         long            X;
         long            Y;
         long            N;
         X = exp_getival(&(CURTASK exps[level - 1]));
         Y = exp_getival(&(CURTASK exps[level + 1]));
         N = ~(X ^ Y);
         CURTASK         exps[level - 1].nval = N;
      }
      break;

   }

   /* set variable type to integer */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_imp()
  
   DESCRIPTION:    This function compares two integers and
         performs a logical IMP on them.
  
***************************************************************/

static int
op_imp(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:

      /* both sides of the operation should be numbers for logical
       * comparison; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be compared logically.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   case NUMBER:
      {
         /* N = X IMP Y  = (X AND Y) OR (NOT X) */
         long            X;
         long            Y;
         long            N;
         X = exp_getival(&(CURTASK exps[level - 1]));
         Y = exp_getival(&(CURTASK exps[level + 1]));
         N = (X & Y) | (~X);
         CURTASK         exps[level - 1].nval = N;
      }
      break;

   }

   /* set variable type to integer */

   CURTASK         exps[level - 1].type = NUMBER;
   CURTASK         exps[level - 1].operation = NUMBER;

   /* decrement the stack twice */

   op_pulldown(2);

   return TRUE;

}

/***************************************************************
  
   FUNCTION:       op_negation()
  
   DESCRIPTION:    This function performs a negation on the
         element to the right.
         Added by JBV 10/95
  
***************************************************************/

static int
op_negation(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:


      /* both sides of the operation should be numbers for logical
       * comparison; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be compared logically.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   default:


      CURTASK exps[level].nval =
         -(exp_getnval(&(CURTASK exps[level + 1])));

      break;
   }

   /* set variable type to requested precision (JBV) */

   CURTASK         exps[level].type = (char) precision;
   CURTASK         exps[level].operation = NUMBER;

   /* decrement the stack once */

   op_pulldown(1);


   return TRUE;

}

static int
op_posation(int level, int precision)
{
   bwx_DEBUG(__FUNCTION__);

   switch (precision)
   {
   case STRING:


      /* both sides of the operation should be numbers for logical
       * comparison; if not, report an error */

      sprintf(bwb_ebuf, "Strings cannot be compared logically.");
      bwb_error(bwb_ebuf);
      return FALSE;

      break;

   default:


      CURTASK exps[level].nval =
         (exp_getnval(&(CURTASK exps[level + 1])));


      break;
   }

   /* set variable type to requested precision (JBV) */

   CURTASK         exps[level].type = (char) precision;
   CURTASK         exps[level].operation = NUMBER;

   /* decrement the stack once */

   op_pulldown(1);


   return TRUE;

}




/***************************************************************
  
   FUNCTION:       op_islevelstr()
  
   DESCRIPTION:    This function determines whether the
         operation at a specified level involves a
         string constant or variable.
  
***************************************************************/

static int
op_islevelstr(int level)
{

   bwx_DEBUG(__FUNCTION__);

   /* first see if the level holds a string constant */

   if (CURTASK exps[level].operation == CONST_STRING)
   {


      return TRUE;
   }
   /* see if the level holds a string variable */

   if (CURTASK exps[level].operation == VARIABLE)
   {
      if (CURTASK exps[level].xvar->type == STRING)
      {


         return TRUE;
      }
   }
   /* test has failed, return FALSE */


   return FALSE;

}

/***************************************************************
  
   FUNCTION:       op_getprecision()
  
   DESCRIPTION:    This function finds the precision for
         an operation by comparing the precision
         at this level and that two levels below.
  
***************************************************************/

static int
op_getprecision(int level)
{
   bwx_DEBUG(__FUNCTION__);

   /* first test for string value */

   if ((CURTASK exps[level + 1].type == STRING)
       || (CURTASK exps[level - 1].type == STRING))
   {
      return STRING;
   }
   /* Both are numbers, so we should be able to find a suitable
    * precision level by starting with the top and moving down; check
    * first for double precision */

   else
   {
      return NUMBER;
   }

}

/***************************************************************
  
   FUNCTION:       op_pulldown()
  
   DESCRIPTION:    This function pulls the expression stack
         down a specified number of levels, decrementing
         the expression stack counter (bycalling dec_esc())
         and decrementing the current "level" of operation
         processing.
  
***************************************************************/

static int
op_pulldown(int how_far)
{
   int             level;
   register int    c;

   bwx_DEBUG(__FUNCTION__);

   /* first pull down the actual variables themselves */

   level = op_level + (2 - how_far);
   while (CURTASK expsc >= (level + how_far))
   {

      /*------------------------------------------------------*/
      /* But before memcpy, deallocate sbuffer for level, and */
      /* afterwards, set sbuffer for level + how_far to NULL! */
      /* Else konfusion reigns the next time around... (JBV)  */
      /*------------------------------------------------------*/

      if (CURTASK exps[level].sval.sbuffer != NULL)   /* JBV */
         FREE(CURTASK exps[level].sval.sbuffer, "op_pulldown");
      memcpy(&CURTASK exps[level], &CURTASK exps[level + how_far],
             (size_t) (sizeof(struct exp_ese)));
      CURTASK         exps[level + how_far].sval.sbuffer = NULL;  /* JBV */

      ++level;

   }

   /* decrement the expression stack counter */

   for (c = 0; c < how_far; ++c)
   {

      if (dec_esc() == TRUE)
      {
         --op_level;
      }
      else
      {
         return FALSE;
      }

   }

   return TRUE;

}

/* EOF */
