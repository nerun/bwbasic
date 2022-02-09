/***************************************************************
  
        bwb_var.c       Variable-Handling Routines
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



static int      dimmed = FALSE;  /* has DIM been called? */
static int      first, last;  /* first, last for DEFxxx commands */

/* Prototypes for functions visible to this file only */

static int
dim_check(struct bwb_variable * v, int *pp);
static int
var_defx(struct bwb_line * l, int type);
static int
var_letseq(char *buffer, int *position, int *start, int *end);
static          size_t
dim_unit(struct bwb_variable * v, int *pp);
static struct bwb_variable *
var_islocal(char *buffer);


/***************************************************************
  
        FUNCTION:       var_init()
  
        DESCRIPTION: This function initializes the internal
         linked list of variables.
  
***************************************************************/

int
var_init(int task)
{
   bwx_DEBUG(__FUNCTION__);

   LOCALTASK       var_start.next = &(LOCALTASK var_end);
   strcpy(LOCALTASK var_start.name, "<START>");
   strcpy(LOCALTASK var_end.name, "<END>");
   return TRUE;
}


/***************************************************************
  
        FUNCTION:       bwb_common()
  
        DESCRIPTION: This C function implements the BASIC
            COMMON command.
  
   SYNTAX:     COMMON variable [, variable...]
  
***************************************************************/

struct bwb_line *
bwb_COMMON(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* loop while arguments are available */

   while (TRUE)
   {

      struct bwb_variable *v;
      char            tbuf[BasicStringLengthMax + 1];
      /* get variable name and find variable */

      bwb_getvarname(l->buffer, tbuf, &(l->position));

      if ((v = var_find(tbuf)) == NULL)
      {
         bwb_error(err_syntax);
         return bwb_zline(l);
      }
      v->common = TRUE; /* set common flag to true */

      /* check for comma */

      adv_ws(l->buffer, &(l->position));
      if (l->buffer[l->position] != ',')
      {
         /* no comma; leave */
         return bwb_zline(l);
      }
      ++(l->position);
      adv_ws(l->buffer, &(l->position));

   }

   return bwb_zline(l);

}

/***********************************************************
  
        FUNCTION: bwb_erase()
  
   DESCRIPTION:    This C function implements the BASIC
         ERASE command.
  
   SYNTAX:     ERASE variable[, variable]...
  
***********************************************************/

struct bwb_line *
bwb_ERASE(struct bwb_line * l)
{
   register int    loop;
   struct bwb_variable *v;
   struct bwb_variable *p; /* previous variable in linked list */
   char            tbuf[BasicStringLengthMax + 1];
   bstring        *sp;  /* JBV */
   register int    n;   /* JBV */

   bwx_DEBUG(__FUNCTION__);

   /* loop while arguments are available */

   loop = TRUE;
   while (loop == TRUE)
   {

      /* get variable name and find variable */

      bwb_getvarname(l->buffer, tbuf, &(l->position));

      if ((v = var_find(tbuf)) == NULL)
      {
         bwb_error(err_syntax);
         return bwb_zline(l);
      }
      /* be sure the variable is dimensioned */

      if ((v->dimensions < 1) || (v->array_sizes[0] < 1))
      {
         bwb_error(err_dimnotarray);
         return bwb_zline(l);
      }
      /* find previous variable in chain */

      for (p = &CURTASK var_start; p->next != v; p = p->next)
      {
         ;
      }

      /* reassign linkage */

      p->next = v->next;

      /* deallocate memory */

      /* Revised to FREE pass-thru calls by JBV */
      FREE(v->array_sizes, "bwb_erase");
      v->array_sizes = NULL;  /* JBV */
      FREE(v->array_pos, "bwb_erase");
      v->array_pos = NULL; /* JBV */
      if (v->type == NUMBER)
      {
         /* Revised to FREE pass-thru call by JBV */
         FREE(v->memnum, "bwb_erase");
         v->memnum = NULL; /* JBV */
      }
      else
      {
         /* Following section added by JBV */
         sp = v->memstr;
         for (n = 0; n < (int) v->array_units; ++n)
         {
            if (sp[n].sbuffer != NULL)
            {
               /* Revised to FREE pass-thru call by
                * JBV */
               FREE(sp[n].sbuffer, "bwb_erase");
               sp[n].sbuffer = NULL;
            }
            sp[n].rab = FALSE;
            sp[n].length = 0;
         }
         /* Revised to FREE pass-thru call by JBV */
         FREE(v->memstr, "bwb_erase");
         v->memstr = NULL; /* JBV */
      }
      /* Revised to FREE pass-thru call by JBV */
      FREE(v, "bwb_erase");
      v = NULL;   /* JBV */

      /* check for comma */

      adv_ws(l->buffer, &(l->position));
      if (l->buffer[l->position] != ',')
      {
         return bwb_zline(l); /* no comma; leave */
      }
      ++(l->position);
      adv_ws(l->buffer, &(l->position));

   }

   return bwb_zline(l);

}

/***********************************************************
  
        FUNCTION: bwb_swap()
  
   DESCRIPTION:    This C function implements the BASIC
         SWAP command.
  
   SYNTAX:     SWAP variable, variable
  
***********************************************************/

struct bwb_line *
bwb_SWAP(struct bwb_line * l)
{
   struct bwb_variable tmp;/* temp holder */
   struct bwb_variable *lhs, *rhs;  /* left and right- hand side of swap
                * statement */
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   /* get left variable name and find variable */

   bwb_getvarname(l->buffer, tbuf, &(l->position));

   if ((lhs = var_find(tbuf)) == NULL)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* check for comma */

   adv_ws(l->buffer, &(l->position));
   if (l->buffer[l->position] != ',')
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   ++(l->position);
   adv_ws(l->buffer, &(l->position));

   /* get right variable name */


   bwb_getvarname(l->buffer, tbuf, &(l->position));


   if ((rhs = var_find(tbuf)) == NULL)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* check to be sure that both variables are of the same type */

   if (rhs->type != lhs->type)
   {
      bwb_error(err_mismatch);
      return bwb_zline(l);
   }
   /* copy lhs to temp, rhs to lhs, then temp to rhs */
   tmp.memnum = NULL;   /* AF5NE */
   tmp.memstr = NULL;   /* AF5NE */

   if (lhs->type == NUMBER)
   {
      tmp.memnum = lhs->memnum;
   }
   else
   {
      tmp.memstr = lhs->memstr;
   }
   tmp.array_sizes = lhs->array_sizes;
   tmp.array_units = lhs->array_units;
   tmp.array_pos = lhs->array_pos;
   tmp.dimensions = lhs->dimensions;


   lhs->memnum = NULL;  /* AF5NE */
   lhs->memstr = NULL;  /* AF5NE */


   if (lhs->type == NUMBER)
   {
      lhs->memnum = rhs->memnum;
   }
   else
   {
      lhs->memstr = rhs->memstr;
   }
   lhs->array_sizes = rhs->array_sizes;
   lhs->array_units = rhs->array_units;
   lhs->array_pos = rhs->array_pos;
   lhs->dimensions = rhs->dimensions;


   rhs->memnum = NULL;  /* AF5NE */
   rhs->memstr = NULL;  /* AF5NE */


   if (lhs->type == NUMBER)
   {
      rhs->memnum = tmp.memnum;
   }
   else
   {
      rhs->memstr = tmp.memstr;
   }
   rhs->array_sizes = tmp.array_sizes;
   rhs->array_units = tmp.array_units;
   rhs->array_pos = tmp.array_pos;
   rhs->dimensions = tmp.dimensions;

   /* return */

   return bwb_zline(l);

}


/***********************************************************
  
        FUNCTION: bwb_clear()
  
   DESCRIPTION:    This C function implements the BASIC
         CLEAR command.
  
   SYNTAX:     CLEAR
  
***********************************************************/

void
var_CLEAR(void)
{
   struct bwb_variable *v;
   register int    n;
   bstring        *sp;
   BasicNumberType *np;

   bwx_DEBUG(__FUNCTION__);

   for (v = CURTASK var_start.next; v != &CURTASK var_end; v = v->next)
   {
      if (v->preset != TRUE)
      {
         switch (v->type)
         {
         case NUMBER:
            np = v->memnum;
            for (n = 0; n < (int) v->array_units; ++n)
            {
               np[n] = (BasicNumberType) 0.0;
            }
            break;
         case STRING:
            sp = v->memstr;
            for (n = 0; n < (int) v->array_units; ++n)
            {
               if (sp[n].sbuffer != NULL)
               {
                  /* Revised to FREE pass-thru
                   * call by JBV */
                  FREE(sp[n].sbuffer, "bwb_clear");
                  sp[n].sbuffer = NULL;
               }
               sp[n].rab = FALSE;
               sp[n].length = 0;
            }
            break;
         }
      }
   }

   dimmed = FALSE;
}

struct bwb_line *
bwb_CLEAR(struct bwb_line * l)
{

   var_CLEAR();

   /* ignoare all parameters */
   adv_eos(l->buffer, &(l->position));

   return bwb_zline(l);
}

/***********************************************************
  
   FUNCTION:       var_delcvars()
  
   DESCRIPTION:    This function deletes all variables
         in memory except those previously marked
         as common.
  
***********************************************************/

int
var_delcvars(void)
{
   struct bwb_variable *v;
   struct bwb_variable *p; /* previous variable */
   bstring        *sp;  /* JBV */
   register int    n;   /* JBV */


   bwx_DEBUG(__FUNCTION__);


   for (v = CURTASK var_start.next; (v != NULL) && (v != &CURTASK var_end); v = v->next)
   {


      if ((v->preset != TRUE) && (v->common != TRUE))

      {



         /* if the variable is dimensioned, release allocated
          * memory */

         if (v->dimensions > 0)
         {

            /* deallocate memory */

            /* Revised to FREE pass-thru calls by JBV */
            FREE(v->array_sizes, "var_delcvars");
            v->array_sizes = NULL;  /* JBV */
            FREE(v->array_pos, "var_delcvars");
            v->array_pos = NULL; /* JBV */
            if (v->type == NUMBER)
            {
               /* Revised to FREE pass-thru call by
                * JBV */
               FREE(v->memnum, "var_delcvars");
               v->memnum = NULL; /* JBV */
            }
            else
            {
               /* Following section added by JBV */
               sp = v->memstr;
               for (n = 0; n < (int) v->array_units; ++n)
               {
                  if (sp[n].sbuffer != NULL)
                  {
                     /* Revised to FREE
                      * pass-thru call by
                      * JBV */
                     FREE(sp[n].sbuffer, "var_delcvars");
                     sp[n].sbuffer = NULL;
                  }
                  sp[n].rab = FALSE;
                  sp[n].length = 0;
               }
               /* Revised to FREE pass-thru call by
                * JBV */
               FREE(v->memstr, "var_delcvars");
               v->memstr = NULL; /* JBV */
            }
         }
         /* find previous variable in chain */
         for (p = &CURTASK var_start; p->next != v; p = p->next)
         {
            ;
         }


         /* reassign linkage */

         p->next = v->next;

         /* deallocate the variable itself */

         /* Revised to FREE pass-thru call by JBV */
         FREE(v, "var_delcvars");
         /* v = NULL; *//* Variable 'v' is reassigned a value
          * before the old one has been used. */
         v = p;

      }
   }

   return TRUE;

}

/***********************************************************
  
        FUNCTION: bwb_mid()
  
   DESCRIPTION:    This function implements the BASIC
         MID$ command.
  
         Same as MID$ function, except it will set
         the desired substring and not return its
         value.  Added by JBV 10/95
  
   SYNTAX:     MID$( string-variable$, start-position-in-string
         [, number-of-spaces ] ) = expression
  
***********************************************************/

struct bwb_line *
bwb_MID_(struct bwb_line * l)
{
   char            tbuf[BasicStringLengthMax + 1];
   char            source_string[BasicStringLengthMax + 1];
   struct bwb_variable *v;
   int             pos;
   bstring        *d;
   int            *pp;
   int             n_params;
   int             p;
   register int    n;
   int             startpos;
   int             numchars;
   int             endpos;
   int             source_length;
   int             target_length;



   int             target_terminate;
   struct exp_ese *e;

   bwx_DEBUG(__FUNCTION__);

   /* Get past left parenthesis */
   adv_ws(l->buffer, &(l->position));
   ++(l->position);
   adv_ws(l->buffer, &(l->position));

   /* Get variable name and find variable */
   bwb_getvarname(l->buffer, tbuf, &(l->position));
   v = var_find(tbuf);
   if (v == NULL)
   {
      sprintf(bwb_ebuf, "in bwb_mid(): failed to find variable");
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   if (v->type != STRING)
   {
      sprintf(bwb_ebuf, "in bwb_mid(): assignment must be to string variable");
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   /* read subscripts */
   pos = 0;
   if ((v->dimensions == 1) && (v->array_sizes[0] == 1))
   {
      n_params = 1;
      pp = &p;
      pp[0] = dim_base;
   }
   else
   {
      dim_getparams(l->buffer, &(l->position), &n_params, &pp);
   }

   CURTASK         exps[CURTASK expsc].pos_adv = pos;
   for (n = 0; n < v->dimensions; ++n)
   {
      v->array_pos[n] = pp[n];
   }

   /* get bstring pointer */
   d = var_findsval(v, pp);

   /* Get past next comma and white space */
   adv_ws(l->buffer, &(l->position));
   ++(l->position);
   adv_ws(l->buffer, &(l->position));

   /* Get starting position (expression) */
   adv_element(l->buffer, &(l->position), tbuf);
   pos = 0;
   e = bwb_exp(tbuf, FALSE, &pos);
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   startpos = exp_getival(e);


   /* Get past next comma and white space (if they exist) */
   adv_ws(l->buffer, &(l->position));
   if (l->buffer[l->position] == ',')
   {
      target_terminate = 0;
      ++(l->position);
      adv_ws(l->buffer, &(l->position));
      adv_element(l->buffer, &(l->position), tbuf);
      pos = 0;
      e = bwb_exp(tbuf, FALSE, &pos);
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      numchars = exp_getival(e);
      if (numchars == 0)
      {
         sprintf(bwb_ebuf, "in bwb_mid(): destination string no. of chars out of range");
         bwb_error(bwb_ebuf);
         return bwb_zline(l);
      }
   }
   else
   {
      target_terminate = 1;
      numchars = 0;
   }

   if (numchars < 0)
   {
      sprintf(bwb_ebuf, "in bwb_mid(): negative string length");
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   /* Get past equal sign */
   adv_ws(l->buffer, &(l->position));
   if (l->buffer[l->position] == ')')
   {
      ++(l->position);
      adv_ws(l->buffer, &(l->position));
   }
   ++(l->position);
   adv_ws(l->buffer, &(l->position));

   /* Evaluate string expression */
   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (e->type != STRING)
   {
      sprintf(bwb_ebuf, "in bwb_mid(): assignment must be from string expression");
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   /* Prepare to MID the string */
   str_btoc(source_string, exp_getsval(e));
   str_btoc(tbuf, d);
   target_length = strlen(tbuf);
   if (startpos > (target_length + 1))
   {
      sprintf(bwb_ebuf, "in bwb_mid(): non-contiguous string created");
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   if (startpos < 1)
   {
      sprintf(bwb_ebuf, "in bwb_mid(): destination string start position out of range");
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   source_length = strlen(source_string);
   if (numchars == 0)
      numchars = source_length;
   endpos = startpos + numchars - 1;

   /* MID the string */
   if (endpos < startpos)
      tbuf[startpos - 1] = '\0';
   else
   {
      int             source_counter;

      source_counter = 0;
      for (n = startpos - 1; n < endpos; ++n)
      {
         if (source_counter < source_length)
            tbuf[n] = source_string[source_counter];
         else
            tbuf[n] = ' ';
         ++source_counter;
      }
      /* Terminate if indicated or characters were added */
      if ((endpos > target_length) || (target_terminate == 1))
         tbuf[endpos] = '\0';
   }
   str_ctob(d, tbuf);

   adv_eos(l->buffer, &(l->position));

   return bwb_zline(l);

}


/***********************************************************
  
        FUNCTION: bwb_ddbl()
  
   DESCRIPTION:    This function implements the BASIC
         DEFDBL command.
  
   SYNTAX:     DEFDBL letter[-letter](, letter[-letter])...
  
***********************************************************/

struct bwb_line *
bwb_DEFDBL(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with DOUBLE set */

   var_defx(l, NUMBER);

   return bwb_zline(l);

}

/***********************************************************
  
        FUNCTION: bwb_dint()
  
   DESCRIPTION:    This function implements the BASIC
         DEFINT command.
  
   SYNTAX:     DEFINT letter[-letter](, letter[-letter])...
  
***********************************************************/

struct bwb_line *
bwb_DEFINT(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with INTEGER set */

   var_defx(l, NUMBER);

   return bwb_zline(l);

}

/***********************************************************
  
        FUNCTION: bwb_dsng()
  
   DESCRIPTION:    This function implements the BASIC
         DEFSNG command.
  
   SYNTAX:     DEFSNG letter[-letter](, letter[-letter])...
  
***********************************************************/

struct bwb_line *
bwb_DEFSNG(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with SINGLE set */

   var_defx(l, NUMBER);

   return bwb_zline(l);

}

/***********************************************************
  
        FUNCTION: bwb_dstr()
  
   DESCRIPTION:    This function implements the BASIC
         DEFSTR command.
  
   SYNTAX:     DEFSTR letter[-letter](, letter[-letter])...
  
***********************************************************/

struct bwb_line *
bwb_DEFSTR(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with STRING set */

   var_defx(l, STRING);

   return bwb_zline(l);

}

static
int
VarTypeIndex(int C)
{
   bwx_DEBUG(__FUNCTION__);

   if (isalpha(C))
   {
      static char     A2Z[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
      char           *P;

      P = strchr(A2Z, ToUpper(C));
      if (P == NULL)
      {
         bwb_error("INTERNAL: VarTypeIndex() called for non-char");
      }
      else
      {
         return P - A2Z;
      }
   }
   else
   {
      bwb_error("INTERNAL: VarTypeIndex() called for non-alpha");
   }
   return 0;
}

/***********************************************************
  
        Function: var_defx()
  
   DESCRIPTION:    This function is a generalized DEFxxx handler.
  
***********************************************************/

static int
var_defx(struct bwb_line * l, int type)
{
   int             loop;
   register int    c;
#if 0
   char            vname[2];
   struct bwb_variable *v;
#endif

   bwx_DEBUG(__FUNCTION__);

   /* loop while there are variable names to process */

   loop = TRUE;
   while (loop == TRUE)
   {

      /* check for end of line or line segment */

      adv_ws(l->buffer, &(l->position));
      switch (l->buffer[l->position])
      {
      case '\0':
         return FALSE;
      }
      if( l->buffer[l->position] == OptionCommentChar )
      {
         return FALSE;
      }

      /* find a sequence of letters for variables */

      if (var_letseq(l->buffer, &(l->position), &first, &last) == FALSE)
      {
         return FALSE;
      }
#if 0
      /* loop through the list getting variables */

      for (c = first; c <= last; ++c)
      {
         vname[0] = (char) c;
         vname[1] = '\0';


         v = var_find(vname);

         /* but var_find() assigns on the basis of name
          * endings (so all in this case should be SINGLEs),
          * so we must force the type of the variable */

         var_make(v, type);

      }
#endif
      if (isalpha(first) && isalpha(last))
      {
         first = VarTypeIndex(first);
         last = VarTypeIndex(last);
         for (c = first; c <= last; c++)
         {
            DefaultVariableType[c] = type;
         }
      }
   }

   return TRUE;

}


/***********************************************************
  
        Function: var_letseq()
  
   DESCRIPTION:    This function finds a sequence of letters
         for a DEFxxx command.
  
***********************************************************/

static int
var_letseq(char *buffer, int *position, int *start, int *end)
{

   bwx_DEBUG(__FUNCTION__);

   /* advance beyond whitespace */

   adv_ws(buffer, position);

   /* check for end of line */

   switch (buffer[*position])
   {
   case '\0':
      return TRUE;
   }

   /* character at this position must be a letter */

   if (isalpha(buffer[*position]) == 0)
   {
      bwb_error(err_defchar);
      return FALSE;
   }
   *end = *start = buffer[*position];

   /* advance beyond character and whitespace */

   ++(*position);
   adv_ws(buffer, position);

   /* check for hyphen, indicating sequence of more than one letter */

   if (buffer[*position] == '-')
   {

      ++(*position);

      /* advance beyond whitespace */

      adv_ws(buffer, position);

      /* character at this position must be a letter */

      if (isalpha(buffer[*position]) == 0)
      {
         *end = *start;
      }
      else
      {
         *end = buffer[*position];
         ++(*position);
      }

   }
   /* advance beyond comma if present */

   if (buffer[*position] == ',')
   {
      ++(*position);
   }
   return TRUE;
}


/***********************************************************
  
   FUNCTION:       bwb_getvarname()
  
   DESCRIPTION:    This function takes the string in lb
         (the large buffer), finds a variable name,
         and returns it in sb (the small buffer),
         appropriately incrementing the integer
         pointed to by n.
  
***********************************************************/

int
bwb_getvarname(char *lb, char *sb, int *n)
{
   /* FIXME: use BasicNameLengthMax */
#if 0
   register int    s;

   bwx_DEBUG(__FUNCTION__);

   s = 0;

   /* advance beyond whitespace */
   adv_ws(lb, n);

   while (TRUE)
   {
      switch (lb[*n])
      {
      case ' ':   /* whitespace */
      case '\0':
      case ',':
      case ';':
      case '(':   /* beginning of parameter list for
             * dimensioned array */
      case '+':   /* add variables *//* FIXME: looks like a bug */
      case '=':   /* Don't forget this one (JBV) */
         sb[s] = 0;
         return TRUE;
      default:
         sb[s] = lb[*n];
         break;
      }

      ++*n;    /* advance to next character in large buffer */
      ++s;     /* advance to next position in small buffer */
      sb[s] = 0;  /* terminate with 0 */

   }
#endif


   bwx_DEBUG(__FUNCTION__);

   /* advance beyond whitespace */
   adv_ws(lb, n);

   lb += (*n);
   if (isalpha(*lb))
   {
      *sb = *lb;
      sb++;
      lb++;
      (*n)++;

      if (OptionFlags & OPTION_BUGS_ON)
      {
         /* allow '.' and '_' in variable names */
         while (isalnum(*lb) || *lb == '.' || *lb == '_')
         {
            *sb = *lb;
            sb++;
            lb++;
            (*n)++;
         }
      }
      else
      {
         while (isalnum(*lb))
         {
            *sb = *lb;
            sb++;
            lb++;
            (*n)++;
         }
      }
      if (OptionFlags & OPTION_BUGS_ON)
      {
         /* allow '!','@','#', '%', '&' and '$' in variable names */
         switch (*lb)
         {
         case BasicDoubleSuffix:
         case BasicSingleSuffix:
         case BasicCurrencySuffix:
         case BasicLongSuffix:
         case BasicIntegerSuffix:
         case BasicStringSuffix:
            /* suffix IS part of the name. *
             * A$ is distinct from A. */
            *sb = *lb;
            sb++;
            lb++;
            (*n)++;
            break;
         }
      }
      else
      {
         switch (*lb)
         {
         case BasicStringSuffix:
            /* suffix IS part of the name. *
             * A$ is distinct from A. */
            *sb = *lb;
            sb++;
            lb++;
            (*n)++;
            break;
         }
      }
   }
   *sb = '\0';
   return TRUE;
}

/***************************************************************
  
        FUNCTION:       var_find()
  
   DESCRIPTION:    This C function attempts to find a variable
         name matching the argument in buffer. If
         it fails to find a matching name, it
         sets up a new variable with that name.
  
***************************************************************/

struct bwb_variable *
var_find(char *buffer)
{
   struct bwb_variable *v;

   bwx_DEBUG(__FUNCTION__);

   /* check for NULL variable name */

   if (strlen(buffer) == 0)
   {
      sprintf(bwb_ebuf, "in var_find(): NULL variable name received\n");
      bwb_error(bwb_ebuf);
      return NULL;
   }
   /* check for a local variable at this EXEC level */

   v = var_islocal(buffer);
   if (v != NULL)
   {
      return v;
   }
   /* now run through the global variable list and try to find a match */

   for (v = CURTASK var_start.next; v != &CURTASK var_end; v = v->next)
   {

      if (strcasecmp(v->name, buffer) == 0)
      {
         switch (v->type)
         {
         case STRING:
         case NUMBER:
            break;
         default:
            sprintf(bwb_ebuf, "in var_find(): inappropriate precision for variable <%s>",
               v->name);
            bwb_error(bwb_ebuf);
            return NULL;
            break;
         }

         return v;
      }
   }

   /* presume this is a new variable, so initialize it... */

   /* initialize new variable */

   v = var_new(buffer);

   /* set place at beginning of variable chain */

   v->next = CURTASK var_start.next;
   CURTASK         var_start.next = v;

   /* normally not a preset */

   v->preset = FALSE;


   return v;

}

/***************************************************************
  
        FUNCTION:       var_new()
  
   DESCRIPTION:    This function assigns memory for a new variable.
  
***************************************************************/

struct bwb_variable *
var_new(char *name)
{
   struct bwb_variable *v;
   char            C;
   int             type;


   bwx_DEBUG(__FUNCTION__);
   /* get memory for new variable */

   if (name == NULL)
   {
      bwb_error("INTERNAL ERROR - not a valid variable name - name == NULL");
      return NULL;
   }
   if (strlen(name) == 0)
   {
      bwb_error("INTERNAL ERROR - not a valid variable name - strlen(name) == 0");
      return NULL;
   }
   if (isalpha(*name))
   {
      /* OK */
   }
   else
   {
      bwb_error("INTERNAL ERROR - not a valid variable name - !isalpha(*name)");
      return NULL;
   }
   C = ToUpper(*name);
   /* Revised to CALLOC pass-thru call by JBV */
   if ((v = (struct bwb_variable *) CALLOC(1, sizeof(struct bwb_variable), "var_new"))
       == NULL)
   {
      bwb_error(err_getmem);
      return NULL;
   }
   /* copy the name into the appropriate structure */

   strcpy(v->name, name);

#if 0
   var_make(v, (int) v->name[strlen(v->name) - 1]);
#endif
   /* set memory in the new variable */

   type = NUMBER;    /* DEFAULT */
   switch (v->name[strlen(v->name) - 1])
   {
   case BasicStringSuffix:
      type = STRING;
      break;
   case BasicDoubleSuffix:
   case BasicSingleSuffix:
   case BasicCurrencySuffix:
   case BasicLongSuffix:
   case BasicIntegerSuffix:
      type = NUMBER;
      break;
   default:
      type = DefaultVariableType[VarTypeIndex(C)];
      break;
   }
   var_make(v, type);

   /* and return */

   return v;

}

/***************************************************************
  
        FUNCTION:       bwb_isvar()
  
   DESCRIPTION:    This function determines if the string
         in 'buffer' is the name of a previously-
         existing variable.
  
***************************************************************/

int
bwb_isvar(char *buffer)
{
   struct bwb_variable *v;


   bwx_DEBUG(__FUNCTION__);

   /* run through the variable list and try to find a match */

   for (v = CURTASK var_start.next; v != &CURTASK var_end; v = v->next)
   {

      if (strcasecmp(v->name, buffer) == 0)
      {
         return TRUE;
      }
   }

   /* search failed */

   return FALSE;

}

/***************************************************************
  
   FUNCTION:       var_getnval()
  
   DESCRIPTION:    This function returns the current value of
         the variable argument as a number.
  
***************************************************************/

BasicNumberType
var_getnval(struct bwb_variable * nvar)
{

   bwx_DEBUG(__FUNCTION__);

   switch (nvar->type)
   {
   case NUMBER:
      return *(var_findnval(nvar, nvar->array_pos));
   }

   sprintf(bwb_ebuf, "in var_getnval(): type is <%d>=<%c>.",
      nvar->type, nvar->type);
   bwb_error(bwb_ebuf);
   return 0;

}


/***************************************************************
  
   FUNCTION:       var_getsval()
  
   DESCRIPTION:    This function returns the current value of
         the variable argument as a pointer to a BASIC
         string structure.
  
***************************************************************/

bstring        *
var_getsval(struct bwb_variable * nvar)
{
   static bstring  b;

   bwx_DEBUG(__FUNCTION__);

   b.rab = FALSE;

   switch (nvar->type)
   {
   case STRING:
      return var_findsval(nvar, nvar->array_pos);
   case NUMBER:
      sprintf(bwb_ebuf, "%*f ", prn_precision(nvar),
         *(var_findnval(nvar, nvar->array_pos)));
      str_ctob(&b, bwb_ebuf);
      return &b;
   default:
      sprintf(bwb_ebuf, "in var_getsval(): type is <%d>=<%c>.",
         nvar->type, nvar->type);
      bwb_error(bwb_ebuf);
      return NULL;
   }

}

/***************************************************************
  
   FUNCTION:   bwb_dim()
  
   DESCRIPTION:   This function implements the BASIC DIM
         statement, allocating memory for a
         dimensioned array of variables.
  
   SYNTAX:     DIM variable(elements...)[variable(elements...)]...
  
***************************************************************/

struct bwb_line *
bwb_DIM(struct bwb_line * l)
{
   register int    n;
   static int      n_params;  /* number of parameters */
   static int     *pp;  /* pointer to parameter values */
   struct bwb_variable *newvar;
   BasicNumberType *np;
   int             loop;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);




   loop = TRUE;
   while (loop == TRUE)
   {
      int             old_name;
      int             old_dimensions;

      old_name = FALSE;

      /* Get variable name */

      adv_ws(l->buffer, &(l->position));
      bwb_getvarname(l->buffer, tbuf, &(l->position));


      /* check for previously used variable name */

      if (bwb_isvar(tbuf) == TRUE)
      {
         old_name = TRUE;
      }
      /* get the new variable */

      newvar = var_find(tbuf);


      /* note that DIM has been called */

      dimmed = TRUE;

      /* read parameters */

      old_dimensions = newvar->dimensions;
      dim_getparams(l->buffer, &(l->position), &n_params, &pp);
      newvar->dimensions = n_params;

      /* Check parameters for an old variable name */

      if (old_name == TRUE)
      {

         /* check to be sure the number of dimensions is the
          * same */

         if (newvar->dimensions != old_dimensions)
         {
            sprintf(bwb_ebuf, "in bwb_dim(): variable <%s> cannot be re-dimensioned",
               newvar->name);
            bwb_error(bwb_ebuf);
            return bwb_zline(l);
         }
         /* check to be sure sizes for the old variable are
          * the same */

         for (n = 0; n < newvar->dimensions; ++n)
         {
            if ((pp[n] + (1 - dim_base)) != newvar->array_sizes[n])
            {
               sprintf(bwb_ebuf, "in bwb_dim(): variable <%s> parameter <%d> cannot be resized",
                  newvar->name, n);
               bwb_error(bwb_ebuf);
               return bwb_zline(l);
            }
         }

      }     /* end of conditional for old variable */
      /*       a new variable */

      else
      {

         /* assign memory for parameters */

         /* Revised to CALLOC pass-thru call by JBV */
         if ((newvar->array_sizes = (int *) CALLOC(n_params, sizeof(int), "bwb_dim")) == NULL)
         {
            sprintf(bwb_ebuf, "in line %d: Failed to find memory for array_sizes for <%s>",
               l->number, newvar->name);
            bwb_error(bwb_ebuf);
            return bwb_zline(l);
         }
         for (n = 0; n < newvar->dimensions; ++n)
         {
            newvar->array_sizes[n] = pp[n] + (1 - dim_base);
         }

         /* assign memory for current position */

         /* Revised to CALLOC pass-thru call by JBV */
         if ((newvar->array_pos = (int *) CALLOC(n_params, sizeof(int), "bwb_dim")) == NULL)
         {
            sprintf(bwb_ebuf, "in line %d: Failed to find memory for array_pos for <%s>",
               l->number, newvar->name);
            bwb_error(bwb_ebuf);
            return bwb_zline(l);
         }
         for (n = 0; n < newvar->dimensions; ++n)
         {
            newvar->array_pos[n] = dim_base;
         }

         /* calculate the array size */
#define MAXINTSIZE      2147483647  /* this is a HACK */

         newvar->array_units = (size_t) MAXINTSIZE;   /* avoid error in
                            * dim_unit() */
         newvar->array_units = dim_unit(newvar, pp) + 1;


         /* assign array memory */

         switch (newvar->type)
         {
         case STRING:
            /*------------------------------------------------------*/
            /* memnum, not memstr, was used here --
             * incorrect (JBV) */
            /* Revised to CALLOC pass-thru call by JBV              */
            /*------------------------------------------------------*/
            if ((newvar->memstr = (bstring *)
                 CALLOC(newvar->array_units, sizeof(bstring), "bwb_dim")) == NULL)
            {
               sprintf(bwb_ebuf, "in line %d: Failed to find memory for array <%s>",
                  l->number, newvar->name);
               bwb_error(bwb_ebuf);
               return bwb_zline(l);
            }
            break;
         case NUMBER:

            /* Revised to CALLOC pass-thru call by JBV */
            if ((np = (BasicNumberType *)
                 CALLOC(newvar->array_units, sizeof(BasicNumberType), "bwb_dim")) == NULL)
            {
               sprintf(bwb_ebuf, "in line %d: Failed to find memory for array <%s>",
                  l->number, newvar->name);
               bwb_error(bwb_ebuf);
               return bwb_zline(l);
            }
            newvar->memnum = np;
            break;
         default:
            sprintf(bwb_ebuf, "in line %d: New variable has unrecognized type.",
               l->number);
            bwb_error(bwb_ebuf);
            return bwb_zline(l);
         }

      }     /* end of conditional for new variable */

      /* now check for end of string */

      if (l->buffer[l->position] == ')')
      {
         ++(l->position);
      }
      adv_ws(l->buffer, &(l->position));
      if (l->buffer[l->position] == OptionCommentChar)
      {
         loop = FALSE;
      }
      else
      {
         switch (l->buffer[l->position])
         {
         case '\0':  /* end of string */
            loop = FALSE;
            break;
         case ',':
            ++(l->position);
            adv_ws(l->buffer, &(l->position));
            loop = TRUE;
            break;
         default:
            loop = FALSE;
            sprintf(bwb_ebuf, "in bwb_dim(): unexpected end of string, buf <%s>",
               &(l->buffer[l->position]));
            bwb_error(bwb_ebuf);
            return bwb_zline(l);
            break;
         }
      }

   }        /* end of loop through variables */

   /* return */

   return bwb_zline(l);

}

int
DetermineNumberOfDimensions(char *expression, int LastPosition)
{
   /* MUST be a dynamically created array, READ A(10) = ... ' variable
    * "A" has NOT been dimensioned */
   int             ParenLevel = 0;
   int             NumDimensions = 1;
   int             Loop = TRUE;

   bwx_DEBUG(__FUNCTION__);

   while (Loop == TRUE)
   {
      char            C;
      C = expression[LastPosition];
      /* check the current character */


      switch (C)
      {
      case '\0':
         return 0;
         break;

      case '=':
         Loop = FALSE;
         break;

      case '(':
         LastPosition++;
         ParenLevel++;
         break;
      case ')':
         LastPosition++;
         ParenLevel--;
         if (ParenLevel == 0)
         {
            Loop = FALSE;
         }
         break;
      case ',':
         LastPosition++;
         if (ParenLevel == 1)
         {
            NumDimensions++;
         }
         break;
      case '\"':  /* embedded string constant */
         LastPosition++;
         while ((expression[LastPosition] != '\"')
                && (expression[LastPosition] != '\0'))
         {
            LastPosition++;
         }
         if (expression[LastPosition] == '\"')
         {
            LastPosition++;
         }
         break;
      default:
         LastPosition++;
      }
   }

   return NumDimensions;
}

int
ImplicitDim(char *tbuf, int n_params)
{
   /* This is only for ECMA-55.  ECMA-116 requires arrays to be
    * dimensioned prior to being used. */


   register int    n;   /* number of parameters */
   BasicNumberType *np;
   static int     *pp;  /* pointer to parameter values */
   struct bwb_variable *newvar;
   static int      params[MAX_DIMS];

   bwx_DEBUG(__FUNCTION__);

   if (OptionFlags & OPTION_STRICT_ON)
   {
      /* Implicit DIM is not allowed */
      return FALSE;
   }
   for (n = 0; n < MAX_DIMS; n++)
   {
      params[n] = 10;   /* UBOUND() = 10, LBOUND() = OPTION BASE */
   }
   pp = &params[0];

   /* get the new variable */

   newvar = var_find(tbuf);

   /* note that DIM has been (implicitly) called */

   dimmed = TRUE;

   newvar->dimensions = n_params;



   /* assign memory for parameters */

   /* Revised to CALLOC pass-thru call by JBV */
   if ((newvar->array_sizes = (int *) CALLOC(n_params, sizeof(int), "bwb_dim")) == NULL)
   {
      bwb_error(err_getmem);
      return FALSE;
   }
   for (n = 0; n < newvar->dimensions; ++n)
   {
      newvar->array_sizes[n] = pp[n] + (1 - dim_base);
   }

   /* assign memory for current position */

   /* Revised to CALLOC pass-thru call by JBV */
   if ((newvar->array_pos = (int *) CALLOC(n_params, sizeof(int), "bwb_dim")) == NULL)
   {
      bwb_error(err_getmem);
      return FALSE;
   }
   for (n = 0; n < newvar->dimensions; ++n)
   {
      newvar->array_pos[n] = dim_base;
   }

   /* calculate the array size */
#define MAXINTSIZE      2147483647  /* this is a HACK */
   newvar->array_units = (size_t) MAXINTSIZE;   /* avoid error in
                      * dim_unit() */
   newvar->array_units = dim_unit(newvar, pp) + 1;

   /* assign array memory */

   switch (newvar->type)
   {
   case STRING:
      /*------------------------------------------------------*/
      /* memnum, not memstr, was used here -- incorrect (JBV) */
      /* Revised to CALLOC pass-thru call by JBV              */
      /*------------------------------------------------------*/
      if ((newvar->memstr = (bstring *)
           CALLOC(newvar->array_units, sizeof(bstring), "bwb_dim")) == NULL)
      {
         bwb_error(err_getmem);
         return FALSE;
      }
      break;
   case NUMBER:

      /* Revised to CALLOC pass-thru call by JBV */
      if ((np = (BasicNumberType *)
           CALLOC(newvar->array_units, sizeof(BasicNumberType), "bwb_dim")) == NULL)
      {
         bwb_error(err_getmem);
         return FALSE;
      }
      newvar->memnum = np;
      break;
   default:
      bwb_error(err_syntax);
      return FALSE;
   }

   return TRUE;
}




/***************************************************************
  
        FUNCTION:       dim_unit()
  
        DESCRIPTION:    This function calculates the unit
            position for an array.
  
***************************************************************/

static          size_t
dim_unit(struct bwb_variable * v, int *pp)
{
   size_t          r;
   size_t          b;
   register int    n;

   bwx_DEBUG(__FUNCTION__);

   /* Calculate and return the address of the dimensioned array */

   /* Check EACH dimension for out-of-bounds, AND check correct number
    * of dimensions.  NBS_P076_0250 errors correctly. */

   b = 1;
   r = 0;
   for (n = 0; n < v->dimensions; ++n)
   {

      /* newvar->array_sizes[ n ] = pp[ n ] + ( 1 - dim_base ); */
      if ((pp[n] < dim_base) || ((pp[n] + (1 - dim_base)) > v->array_sizes[n]))
      {
         sprintf(bwb_ebuf, "SUBSCRIPT OUT OF RANGE %s(%d)", v->name, pp[n]);
         bwb_error(bwb_ebuf);
         return 0;
      }
      r += b * (pp[n] - dim_base);
      b *= v->array_sizes[n];
   }

   if (r > v->array_units)
   {



      sprintf(bwb_ebuf, "SUBSCRIPT OUT OF RANGE %s(%ld)", v->name, (long) r);
      bwb_error(bwb_ebuf);

      return 0;
   }
   return r;

}

/***************************************************************
  
        FUNCTION:       dim_getparams()
  
   DESCRIPTION:    This function reads a string in <buffer>
                        beginning at position <pos> and finds a
                        list of parameters surrounded by paren-
                        theses, returning in <n_params> the number
                        of parameters found, and returning in
                        <pp> an array of n_params integers giving
                        the sizes for each dimension of the array.
  
***************************************************************/

int
dim_getparams_FIXED(char *buffer, int *pos, int params[ /* MAX_DIMS */ ])
{
   int             n_params;
   int             loop;
   int             x_pos, s_pos;
   struct exp_ese *e;
   char            tbuf[BasicStringLengthMax + 1];
   int             paren_level, quote_level; /* JBV 1/97 */

   bwx_DEBUG(__FUNCTION__);

   /* set initial values */

   n_params = 0;

   /* advance and check for undimensioned variable */

   adv_ws(buffer, pos);
   if (buffer[*pos] != '(')
   {
      n_params = 1;
      params[0] = dim_base;
      return n_params;
   }
   else
   {
      ++(*pos);
   }

   /* Variable has DIMensions: Find each parameter */

   s_pos = 0;
   tbuf[0] = '\0';
   loop = TRUE;
   paren_level = 1;  /* JBV 1/97 */
   quote_level = 0;  /* JBV 1/97 */
   while (loop == TRUE)
   {
      switch (buffer[*pos])
      {
      case ')':   /* end of parameter list */
         /*-----------------------------------------------------*/
         /* paren_level and quote_level check added by JBV
          * 1/97 */
         /*-----------------------------------------------------*/
         if (quote_level == 0)
            --paren_level;
         if (paren_level != 0 || quote_level != 0) /* Still not done? */
         {
            tbuf[s_pos] = buffer[*pos];
            ++(*pos);
            ++s_pos;
            tbuf[s_pos] = '\0';
            break;
         }
         x_pos = 0;
         if (tbuf[0] == '\0')
         {
            params[n_params] = DEF_SUBSCRIPT;
         }
         else
         {





            e = bwb_exp(tbuf, FALSE, &x_pos);
            if (ERROR_PENDING)
            {
               return 0;
            }
            /* Subscripts are rounded */
            params[n_params] = exp_getival(e);




         }
         n_params++;
         loop = FALSE;
         ++(*pos);
         break;

      case ',':   /* end of a parameter */
         /*-----------------------------------------------------*/
         /* paren_level and quote_level check added by JBV
          * 1/97 */
         /*-----------------------------------------------------*/
         if (paren_level != 1 || quote_level != 0) /* Still not done? */
         {
            tbuf[s_pos] = buffer[*pos];
            ++(*pos);
            ++s_pos;
            tbuf[s_pos] = '\0';
            break;
         }
         x_pos = 0;
         if (tbuf[0] == '\0')
         {
            params[n_params] = DEF_SUBSCRIPT;
         }
         else
         {


            e = bwb_exp(tbuf, FALSE, &x_pos);
            if (ERROR_PENDING)
            {
               return 0;
            }
            /* Subscripts are rounded */
            params[n_params] = exp_getival(e);


         }
         n_params++;
         tbuf[0] = '\0';
         ++(*pos);
         s_pos = 0;
         break;

      case ' ':   /* whitespace -- skip */
         ++(*pos);
         break;

      default:
         if (buffer[*pos] == '(' && quote_level == 0)
            ++paren_level; /* JBV 1/97 */
         if (buffer[*pos] == (char) 34)
         {
            if (quote_level == 0)
               quote_level = 1;
            else
               quote_level = 0;
         }
         tbuf[s_pos] = buffer[*pos];
         ++(*pos);
         ++s_pos;
         tbuf[s_pos] = '\0';
         break;
      }

      if (n_params >= MAX_DIMS)
      {
         loop = FALSE;
      }
   }


   /* return params stack */






   return n_params;


}





int
dim_getparams(char *buffer, int *pos, int *n_params, int **pp)
{
   int             loop;
   static int      params[MAX_DIMS];   /* can NOT be called via
                   * bwb_exp() */
   int             x_pos, s_pos;
   struct exp_ese *e;
   char            tbuf[BasicStringLengthMax + 1];
   int             paren_level, quote_level; /* JBV 1/97 */

   bwx_DEBUG(__FUNCTION__);

   /* set initial values */

   *n_params = 0;

   /* advance and check for undimensioned variable */

   adv_ws(buffer, pos);
   if (buffer[*pos] != '(')
   {
      *n_params = 1;
      params[0] = dim_base;
      *pp = params;
      return TRUE;
   }
   else
   {
      ++(*pos);
   }

   /* Variable has DIMensions: Find each parameter */

   s_pos = 0;
   tbuf[0] = '\0';
   loop = TRUE;
   paren_level = 1;  /* JBV 1/97 */
   quote_level = 0;  /* JBV 1/97 */
   while (loop == TRUE)
   {
      switch (buffer[*pos])
      {
      case ')':   /* end of parameter list */
         /*-----------------------------------------------------*/
         /* paren_level and quote_level check added by JBV
          * 1/97 */
         /*-----------------------------------------------------*/
         if (quote_level == 0)
            --paren_level;
         if (paren_level != 0 || quote_level != 0) /* Still not done? */
         {
            tbuf[s_pos] = buffer[*pos];
            ++(*pos);
            ++s_pos;
            tbuf[s_pos] = '\0';
            break;
         }
         x_pos = 0;
         if (tbuf[0] == '\0')
         {
            params[*n_params] = DEF_SUBSCRIPT;
         }
         else
         {



            e = bwb_exp(tbuf, FALSE, &x_pos);
            if (ERROR_PENDING)
            {
               return FALSE;
            }
            /* Subscripts are rounded */
            params[*n_params] = exp_getival(e);




         }
         ++(*n_params);
         loop = FALSE;
         ++(*pos);
         break;

      case ',':   /* end of a parameter */
         /*-----------------------------------------------------*/
         /* paren_level and quote_level check added by JBV
          * 1/97 */
         /*-----------------------------------------------------*/
         if (paren_level != 1 || quote_level != 0) /* Still not done? */
         {
            tbuf[s_pos] = buffer[*pos];
            ++(*pos);
            ++s_pos;
            tbuf[s_pos] = '\0';
            break;
         }
         x_pos = 0;
         if (tbuf[0] == '\0')
         {
            params[*n_params] = DEF_SUBSCRIPT;
         }
         else
         {


            e = bwb_exp(tbuf, FALSE, &x_pos);
            if (ERROR_PENDING)
            {
               return FALSE;
            }
            /* Subscripts are rounded */
            params[*n_params] = exp_getival(e);


         }
         ++(*n_params);
         tbuf[0] = '\0';
         ++(*pos);
         s_pos = 0;
         break;

      case ' ':   /* whitespace -- skip */
         ++(*pos);
         break;

      default:
         if (buffer[*pos] == '(' && quote_level == 0)
            ++paren_level; /* JBV 1/97 */
         if (buffer[*pos] == (char) 34)
         {
            if (quote_level == 0)
               quote_level = 1;
            else
               quote_level = 0;
         }
         tbuf[s_pos] = buffer[*pos];
         ++(*pos);
         ++s_pos;
         tbuf[s_pos] = '\0';
         break;
      }
   }


   /* return params stack */

   *pp = params;





   return TRUE;

}

/***************************************************************
  
        FUNCTION:       bwb_option()
  
        DESCRIPTION:    This function implements the BASIC OPTION
                        BASE statement, designating the base (1 or
                        0) for addressing DIM arrays.
  
   SYNTAX:     OPTION BASE number
  
***************************************************************/

void
OptionBaseHelper(int newval, struct bwb_line * l   /* just for error
           messages */ )
{
   struct bwb_variable *current;

   bwx_DEBUG(__FUNCTION__);

   /* OPTION BASE ... */

   /* If DIM has already been called, do not allow OPTION BASE */

   if (dimmed != FALSE)
   {
      sprintf(bwb_ebuf, "at line %d: OPTION BASE must be called before DIM.",
         l->number);
      bwb_error(bwb_ebuf);
      return;
   }
   /* Test the new value. */


   if ((newval < 0) || (newval > 1))
   {
      sprintf(bwb_ebuf, "at line %d: value for OPTION BASE must be 1 or 0.",
         l->number);
      bwb_error(bwb_ebuf);
      return;
   }
   /* Set the new value. */

   dim_base = newval;

   /* run through the variable list and change any positions that had
    * set 0 before OPTION BASE was run */

   for (current = CURTASK var_start.next; current != &CURTASK var_end; current = current->next)
   {
      current->array_pos[0] = dim_base;
   }


}

void
OptionVersionSet(int i)
{
   bwx_DEBUG(__FUNCTION__);

   OptionVersion = bwb_vertable[i].OptionVersionBitmask;
   OptionFlags = bwb_vertable[i].OptionFlags;
   OptionCommentChar = bwb_vertable[i].OptionCommentChar;
   OptionStatementChar = bwb_vertable[i].OptionStatementChar;
   strcpy(OptionDateFormat,bwb_vertable[i].OptionDateFormat);
   strcpy(OptionTimeFormat,bwb_vertable[i].OptionTimeFormat);

}

struct bwb_line *
bwb_OPTION(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION  ... */
   bwb_error(err_syntax);
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_ANGLE_DEGREES(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ANGLE DEGREES */
   OptionFlags |= OPTION_ANGLE_DEGREES;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_ANGLE_RADIANS(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ANGLE RADIANS */
   OptionFlags &= ~OPTION_ANGLE_DEGREES;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_ARITHMETIC_DECIMAL(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ARITHMETIC DECIMAL */
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_ARITHMETIC_FIXED(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ARITHMETIC FIXED */
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_ARITHMETIC_NATIVE(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ARITHMETIC NATIVE */
   return bwb_zline(l);
}


struct bwb_line *
bwb_OPTION_BASE_1(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION BASE 1 */
   OptionFlags |= OPTION_BASE_ONE;
   OptionBaseHelper(1, l);
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_BASE_0(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION BASE 0 */
   OptionFlags &= ~OPTION_BASE_ONE;
   OptionBaseHelper(0, l);
   return bwb_zline(l);
}


struct bwb_line *
bwb_OPTION_BUGS_ON(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION BUGS ON */
   OptionFlags |= OPTION_BUGS_ON;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_BUGS_OFF(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION BUGS OFF */
   OptionFlags &= ~OPTION_BUGS_ON;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_COMMENT(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COMMENT char */
   OptionCommentChar = l->buffer[l->position];
   if( OptionCommentChar != '\0' )
   {
      l->position++;
   }
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_COMPARE_BINARY(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COMPARE BINARY */
   OptionFlags &= ~OPTION_COMPARE_TEXT;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_COMPARE_DATABASE(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COMPARE DATABASE */
   OptionFlags |= OPTION_COMPARE_TEXT;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_COMPARE_TEXT(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COMPARE TEXT */
   OptionFlags |= OPTION_COMPARE_TEXT;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_COVERAGE_ON(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COVERAGE ON */
   OptionFlags |= OPTION_COVERAGE_ON;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_COVERAGE_OFF(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COVERAGE OFF */
   OptionFlags &= ~OPTION_COVERAGE_ON;
   return bwb_zline(l);
}

static void
StripQuotes( char * tbuf )
{
   /* Remove Leading & Trailing Quotes */
   if( tbuf[0] == '"' )
   {
      char * Q;
      strcpy( tbuf, &(tbuf[1]) );
      Q = strchr( tbuf, '"' );
      if( Q != NULL )
      {
         *Q = '\0';
      }
   }
}

struct bwb_line *
bwb_OPTION_DATE(struct bwb_line * l)
{
   /* OPTION DATE format */
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);
   /* Get FORMAT */
   adv_element(l->buffer, &(l->position), tbuf);
   StripQuotes( tbuf );
   tbuf[ 80 ] = '\0';
   strcpy( OptionDateFormat, tbuf ); 
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_DISABLE_COMMAND(struct bwb_line * l)
{
   /* OPTION DISABLE COMMAND ... */
   int             IsFound;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   IsFound = FALSE;
   /* Get COMMAND */
   adv_element(l->buffer, &(l->position), tbuf);
   StripQuotes( tbuf );
   {
      /* Name */
      int i;
      for( i = 0; i < NUM_COMMANDS; i++ )
      {
          if( strcasecmp( tbuf, bwb_cmdtable[i].name ) == 0 )
          {
              /* FOUND */
              /* DISABLE COMMAND */
              bwb_cmdtable[i].OptionVersionBitmask &= ~OptionVersion;
              IsFound = TRUE;
          }
      }
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( bwb_ebuf, "IGNORED: %s", l->buffer);      
       puts(bwb_ebuf);
   }
   return bwb_zline(l);
}


struct bwb_line *
bwb_OPTION_DISABLE_FUNCTION(struct bwb_line * l)
{
   /* OPTION DISABLE FUNCTION ... */
   int             IsFound;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   IsFound = FALSE;
   /* Get FUNCTION */
   adv_element(l->buffer, &(l->position), tbuf);
   StripQuotes( tbuf );
   {
      /* Name */
      int i;
      for( i = 0; i < NUM_FUNCTIONS; i++ )
      {
          if( strcasecmp( tbuf, bwb_prefuncs[i].Name ) == 0 )
          {
              /* FOUND */
              /* DISABLE FUNCTION */
              bwb_prefuncs[i].OptionVersionBitmask &= ~OptionVersion;
              IsFound = TRUE;
          }
      }
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( bwb_ebuf, "IGNORED: %s", l->buffer);      
       puts(bwb_ebuf);
   }
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_DISABLE_OPERATOR(struct bwb_line * l)
{
   /* OPTION DISABLE OPERATOR ... */
   int             IsFound;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   IsFound = FALSE;
   /* Get OPERATOR */
   adv_element(l->buffer, &(l->position), tbuf);
   StripQuotes( tbuf );
   {
      /* Name */
      int i;
      for( i = 0; i < NUM_OPERATORS; i++ )
      {
          if( strcasecmp( tbuf, exp_ops[i].symbol ) == 0 )
          {
              /* FOUND */
              /* DISABLE OPERATOR */
              exp_ops[i].OptionVersionBitmask &= ~OptionVersion;
              IsFound = TRUE;
          }
      }
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( bwb_ebuf, "IGNORED: %s", l->buffer);      
       puts(bwb_ebuf);
   }
   return bwb_zline(l);
}



struct bwb_line *
bwb_OPTION_ENABLE_COMMAND(struct bwb_line * l)
{
   /* OPTION ENABLE COMMAND ... */
   int             IsFound;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   IsFound = FALSE;
   /* Get COMMAND */
   adv_element(l->buffer, &(l->position), tbuf);
   StripQuotes( tbuf );
   {
      /* Name */
      int i;
      for( i = 0; i < NUM_COMMANDS; i++ )
      {
          if( strcasecmp( tbuf, bwb_cmdtable[i].name ) == 0 )
          {
              /* FOUND */
              /* ENABLE COMMAND */
              bwb_cmdtable[i].OptionVersionBitmask |= OptionVersion;
              IsFound = TRUE;
          }
      }
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( bwb_ebuf, "IGNORED: %s", l->buffer);      
       puts(bwb_ebuf);
   }
   return bwb_zline(l);
}


struct bwb_line *
bwb_OPTION_ENABLE_FUNCTION(struct bwb_line * l)
{
   /* OPTION ENABLE FUNCTION ... */
   int             IsFound;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   IsFound = FALSE;
   /* Get FUNCTION */
   adv_element(l->buffer, &(l->position), tbuf);
   StripQuotes( tbuf );
   {
      /* Name */
      int i;
      for( i = 0; i < NUM_FUNCTIONS; i++ )
      {
          if( strcasecmp( tbuf, bwb_prefuncs[i].Name ) == 0 )
          {
              /* FOUND */
              /* ENABLE FUNCTION */
              bwb_prefuncs[i].OptionVersionBitmask |= OptionVersion;
              IsFound = TRUE;
          }
      }
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( bwb_ebuf, "IGNORED: %s", l->buffer);      
       puts(bwb_ebuf);
   }
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_ENABLE_OPERATOR(struct bwb_line * l)
{
   /* OPTION ENABLE OPERATOR ... */
   int             IsFound;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   IsFound = FALSE;
   /* Get OPERATOR */
   adv_element(l->buffer, &(l->position), tbuf);
   StripQuotes( tbuf );
   {
      /* Name */
      int i;
      for( i = 0; i < NUM_OPERATORS; i++ )
      {
          if( strcasecmp( tbuf, exp_ops[i].symbol ) == 0 )
          {
              /* FOUND */
              /* ENABLE OPERATOR */
              exp_ops[i].OptionVersionBitmask |= OptionVersion;
              IsFound = TRUE;
          }
      }
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( bwb_ebuf, "IGNORED: %s", l->buffer);      
       puts(bwb_ebuf);
   }
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_ERROR_GOSUB(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ERROR GOSUB */
   OptionFlags |= OPTION_ERROR_GOSUB;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_ERROR_GOTO(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ERROR GOTO */
   OptionFlags &= ~OPTION_ERROR_GOSUB;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_INDENT(struct bwb_line * l)
{
   /* OPTION VERSION ... */
   char            tbuf[BasicStringLengthMax + 1];
   struct exp_ese *e;
   int             pos;

   /* Get starting position (expression) */
   adv_element(l->buffer, &(l->position), tbuf);
   pos = 0;
   e = bwb_exp(tbuf, FALSE, &pos);
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (e->type == STRING)
   {
      sprintf(bwb_ebuf, "Type Mismatch");
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   OptionIndentValue = exp_getival(e);
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_LABELS_ON(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION LABELS ON */
   OptionFlags |= OPTION_LABELS_ON;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_LABELS_OFF(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION LABELS OFF */
   OptionFlags &= ~OPTION_LABELS_ON;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_STATEMENT(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION STATEMENT char */
   OptionStatementChar = l->buffer[l->position];
   if( OptionStatementChar != '\0' )
   {
      l->position++;
   }
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_STRICT_ON(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION STRICT ON */
   OptionFlags |= OPTION_STRICT_ON;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_STRICT_OFF(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION STRICT OFF */
   OptionFlags &= ~OPTION_STRICT_ON;
   return bwb_zline(l);
}


struct bwb_line *
bwb_OPTION_TERMINAL_NONE(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TERMINAL NONE */
   OptionTerminalType = C_OPTION_TERMINAL_NONE;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_TERMINAL_ADM_3A(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TERMINAL ADM-3A */
   OptionTerminalType = C_OPTION_TERMINAL_ADM_3A;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_TERMINAL_ANSI(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TERMINAL ANSI */
   OptionTerminalType = C_OPTION_TERMINAL_ANSI;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_TIME(struct bwb_line * l)
{
   /* OPTION TIME format */
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);
   /* Get FORMAT */
   adv_element(l->buffer, &(l->position), tbuf);
   StripQuotes( tbuf );
   tbuf[80] = '\0';
   strcpy( OptionTimeFormat, tbuf );
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_TRACE_ON(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TRACE ON */
   OptionFlags |= OPTION_TRACE_ON;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_TRACE_OFF(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TRACE OFF */
   OptionFlags &= ~OPTION_TRACE_ON;
   return bwb_zline(l);
}

struct bwb_line *
bwb_OPTION_VERSION(struct bwb_line * l)
{
   /* OPTION VERSION ... */
   char            tbuf[BasicStringLengthMax + 1];
   int             i;


   bwx_DEBUG(__FUNCTION__);



   adv_element(l->buffer, &(l->position), tbuf);
   for (i = 0; i < NUM_VERSIONS; i++)
   {
      if (strcasecmp(tbuf, bwb_vertable[i].Name) == 0)
      {
         /* FOUND */
         OptionVersionSet(i);
         return bwb_zline(l);
      }
   }
   /* NOT FOUND */
   sprintf(bwb_ebuf, "OPTION VERSION %s IS INVALID.  VALID CHOICES ARE:", tbuf);
   puts(bwb_ebuf);
   for (i = 0; i < NUM_VERSIONS; i++)
   {
      sprintf(bwb_ebuf, "OPTION VERSION %s ' %s",
         bwb_vertable[i].Name, bwb_vertable[i].Description);
      puts(bwb_ebuf);
   }
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       var_findnval()
  
        DESCRIPTION:    This function returns the address of
                        the number for the variable <v>.  If
                        <v> is a dimensioned array, the address
                        returned is for the BasicNumberType at the
                        position indicated by the integer array
                        <pp>.
  
***************************************************************/


BasicNumberType *
var_findnval(struct bwb_variable * v, int *pp)
{
   size_t          offset;
   BasicNumberType *p;

   bwx_DEBUG(__FUNCTION__);
   /* Check for appropriate type */

   if (v->type != NUMBER)
   {
      sprintf(bwb_ebuf, "in var_findnval(): Variable <%s> is not a number.",
         v->name);
      bwb_error(bwb_ebuf);
      return NULL;
   }
   /* Check subscripts */

   if (dim_check(v, pp) == FALSE)
   {
      return NULL;
   }
   /* Calculate and return the address of the dimensioned array */

   offset = dim_unit(v, pp);


   p = v->memnum;
   return (p + offset);

}

/***************************************************************
  
        FUNCTION:       var_findsval()
  
        DESCRIPTION:    This function returns the address of
                        the string for the variable <v>.  If
                        <v> is a dimensioned array, the address
                        returned is for the string at the
                        position indicated by the integer array
                        <pp>.
  
***************************************************************/

bstring        *
var_findsval(struct bwb_variable * v, int *pp)
{
   size_t          offset;
   bstring        *p;

   bwx_DEBUG(__FUNCTION__);

   /* Check for appropriate type */

   if (v->type != STRING)
   {
      sprintf(bwb_ebuf, "in var_findsval(): Variable <%s> is not a string.", v->name);
      bwb_error(bwb_ebuf);
      return NULL;
   }
   /* Check subscripts */

   if (dim_check(v, pp) == FALSE)
   {
      return NULL;
   }
   /* Calculate and return the address of the dimensioned array */

   offset = dim_unit(v, pp);


   p = v->memstr;
   return (p + offset);

}

/***************************************************************
  
        FUNCTION:       dim_check()
  
        DESCRIPTION:    This function checks subscripts of a
                        specific variable to be sure that they
                        are within the correct range.
  
***************************************************************/

static int
dim_check(struct bwb_variable * v, int *pp)
{
   register int    n;

   /* Check for dimensions */

   if (v->dimensions < 1)
   {
      sprintf(bwb_ebuf, "in dim_check(): var <%s> dimensions <%d>",
         v->name, v->dimensions);
      bwb_error(bwb_ebuf);
      return FALSE;
   }
   /* Check for validly allocated array */

   if ((v->type == NUMBER) && (v->memnum == NULL))
   {
      sprintf(bwb_ebuf, "in dim_check(): numerical var <%s> memnum not allocated",
         v->name);
      bwb_error(bwb_ebuf);
      return FALSE;
   }
   if ((v->type == STRING) && (v->memstr == NULL))
   {
      sprintf(bwb_ebuf, "in dim_check(): string var <%s> memstr not allocated",
         v->name);
      bwb_error(bwb_ebuf);
      return FALSE;
   }
   /* Now check subscript values */

   for (n = 0; n < v->dimensions; ++n)
   {


      if ((pp[n] < dim_base) || ((pp[n] + (1 - dim_base)) > v->array_sizes[n]))
      {

         sprintf(bwb_ebuf, "SUBSCRIPT OUT OF RANGE %s(%d)", v->name, pp[n]);
         bwb_error(bwb_ebuf);


         return FALSE;
      }
   }

   /* No problems found */

   return TRUE;

}

/***************************************************************
  
        FUNCTION:       var_make()
  
        DESCRIPTION: This function initializes a variable,
            allocating necessary memory for it.
  
***************************************************************/

int
var_make(struct bwb_variable * v, int type)
{
   bwx_DEBUG(__FUNCTION__);

   switch (type)
   {
   case STRING:
      v->type = STRING;
      break;
   default:
      v->type = NUMBER;
      break;
   }

   /* get memory for array */

   /* First kleanup the joint (JBV) */
   if (v->memnum != NULL)
   {
      /* Revised to FREE pass-thru call by JBV */
      FREE(v->memnum, "var_make");
      v->memnum = NULL;
   }
   if (v->memstr != NULL)
   {
      /* Remember to deallocate those far-flung branches! (JBV) */
      bstring        *sp;  /* JBV */
      register int    n;   /* JBV */

      sp = v->memstr;
      for (n = 0; n < (int) v->array_units; ++n)
      {
         if (sp[n].sbuffer != NULL)
         {
            /* Revised to FREE pass-thru call by JBV */
            FREE(sp[n].sbuffer, "var_make");
            sp[n].sbuffer = NULL;
         }
         sp[n].rab = FALSE;
         sp[n].length = 0;
      }
      /* Revised to FREE pass-thru call by JBV */
      FREE(v->memstr, "var_make");
      v->memstr = NULL;
   }
   /* Revised to FREE pass-thru calls by JBV */
   if (v->array_sizes != NULL)
   {
      FREE(v->array_sizes, "var_make");
      v->array_sizes = NULL;  /* JBV */
   }
   if (v->array_pos != NULL)
   {
      FREE(v->array_pos, "var_make");
      v->array_pos = NULL; /* JBV */
   }
   if (v->type == NUMBER)
   {
      /* Revised to CALLOC pass-thru call by JBV */
      if ((v->memnum = CALLOC(2, sizeof(BasicNumberType), "var_make")) == NULL)
      {
         bwb_error(err_getmem);
         return FALSE;
      }
   }
   else
   {
      /* Revised to CALLOC pass-thru call by JBV */
      if ((v->memstr = CALLOC(2, sizeof(bstring), "var_make")) == NULL)
      {
         bwb_error(err_getmem);
         return FALSE;
      }
   }

   /* get memory for array_sizes and array_pos */

   /* Revised to CALLOC pass-thru call by JBV */
   if ((v->array_sizes = (int *) CALLOC(2, sizeof(int), "var_make")) == NULL)
   {
      bwb_error(err_getmem);
      return FALSE;
   }
   /* Revised to CALLOC pass-thru call by JBV */
   if ((v->array_pos = (int *) CALLOC(2, sizeof(int), "var_make")) == NULL)
   {
      bwb_error(err_getmem);
      return FALSE;
   }
   v->array_pos[0] = dim_base;
   v->array_sizes[0] = 1;
   v->dimensions = 1;
   v->common = FALSE;
   v->array_units = 1;
   v->IsInDim = 0;

   if (type == STRING)
   {
      bstring        *b;


      b = var_findsval(v, v->array_pos);
      b->rab = FALSE;
   }
   return TRUE;

}

/***************************************************************
  
   FUNCTION:       var_islocal()
  
   DESCRIPTION:    This function determines whether the string
         pointed to by 'buffer' has the name of
         a local variable at the present EXEC stack
         level.
  
***************************************************************/

static struct bwb_variable *
var_islocal(char *buffer)
{
   struct bwb_variable *v;

   bwx_DEBUG(__FUNCTION__);

   /* Prevent the expression in the initial value of the for loop below
    * from violating the lower bound of the "excs" array. This would
    * happen during startup when "exsc" is initially set to -1 and
    * bwbasic.exe would fail with a memory exception when compiled with
    * Open Watcom C. */


   if (CURTASK exsc >= 0)
   {
      int             i;
      for (i = CURTASK exsc; i >= 0; i--)
      {
         for (v = CURTASK excs[i].local_variable; v != NULL; v = v->next)
         {
            if (strcasecmp(v->name, buffer) == 0)
            {
               /* FOUND */
               return v;
            }
         }
         if (CURTASK excs[i].LoopTopLine != NULL)
         {
            switch (CURTASK excs[i].LoopTopLine->cmdnum)
            {
            case C_FUNCTION:
            case C_SUB:
               /* we have checked all the way to a
                * FUNCTION or SUB boundary */
               /* NOT FOUND */
               return NULL;
               break;
            }
         }
      }
   }
   /* NOT FOUND */

   return NULL;

}

/***************************************************************
  
        FUNCTION:       bwb_vars()
  
        DESCRIPTION:    This function implements the Bywater-
            specific debugging command VARS, which
            gives a list of all variables defined
            in memory.
  
***************************************************************/


struct bwb_line *
bwb_VARS(struct bwb_line * l)
{
   struct bwb_variable *v;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   /* run through the variable list and print variables */

   for (v = CURTASK var_start.next; v != &CURTASK var_end; v = v->next)
   {
      sprintf(bwb_ebuf, "variable <%s>\t", v->name);
      prn_xprintf(bwb_ebuf);
      switch (v->type)
      {
      case STRING:
         str_btoc(tbuf, var_getsval(v));
         sprintf(bwb_ebuf, "STRING\tval: <%s>\n", tbuf);
         prn_xprintf(bwb_ebuf);
         break;
      case NUMBER:
         sprintf(bwb_ebuf, "NUMBER\tval: <" BasicNumberPrintFormat ">\n", var_getnval(v));
         prn_xprintf(bwb_ebuf);
         break;
      default:
         sprintf(bwb_ebuf, "ERROR: type is <%c>", (char) v->type);
         prn_xprintf(bwb_ebuf);
         break;
      }
   }

   return bwb_zline(l);
}


/* EOF */
