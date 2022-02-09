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
/*---------------------------------------------------------------*/



#include "bwbasic.h"




/* prototypes */

static int
fslt_clear(void);
static int
scan_readargs(struct fslte * f,
         struct bwb_line * line, int *position);
struct bwb_variable *
bwb_etov(struct bwb_variable * dst, struct exp_ese * src);
int
fslt_addlocalvar(struct fslte * f, struct bwb_variable * v);

/***************************************************************
  
        FUNCTION:       bwb_scan()
  
        DESCRIPTION: This function scans all lines of the
            program in memory and creates a FUNCTION-
            SUB lookup table (fslt) for the program.
  
***************************************************************/

int
bwb_scan(void)
{
   struct bwb_line *current;
   int             Indention = 0;

   bwx_DEBUG(__FUNCTION__);


   /* int position; */

   if (CURTASK rescan != TRUE)
   {
      return FALSE;
   }
   /* program needs to be scanned again, because a line was added or
    * deleted */



   /* first run through the FUNCTION - SUB loopkup table and free any
    * existing memory */

   fslt_clear();


   for (current = CURTASK bwb_start.next; current != &CURTASK bwb_end; current = current->next)
   {
      current->OtherLine = NULL;
   }
   /* run through the list of lines and identify SUB and FUNCTION
    * statements */

   for (current = CURTASK bwb_start.next; current != &CURTASK bwb_end; current = current->next)
   {

      /* position = current->Startpos; */
      switch (current->cmdnum)
      {
      case C_DEF:
      case C_FUNCTION:
      case C_SUB:
      case C_USER_LBL:
         fslt_add(current);
      case C_DO:
      case C_DO_WHILE:
      case C_DO_UNTIL:
      case C_WHILE:
      case C_UNTIL:
      case C_FOR:
      case C_IF:
      case C_SELECT_CASE:
         find_BottomLineInCode(current);
         break;
      }

      switch (current->cmdnum)
      {
      case C_NEXT:
      case C_LOOP:
      case C_LOOP_UNTIL:
      case C_LOOP_WHILE:
      case C_UEND:
      case C_WEND:
      case C_ELSEIF:
      case C_ELSE:
      case C_END_IF:
      case C_CASE:
      case C_CASE_IF:
      case C_CASE_IS:
      case C_CASE_ELSE:
      case C_END_SELECT:
      case C_END_SUB:
      case C_END_FUNCTION:
         Indention--;
         break;
      }
      if (Indention < 0)
      {
         Indention = 0;
      }
      current->Indention = Indention;
      switch (current->cmdnum)
      {
      case C_FOR:
      case C_DO:
      case C_DO_UNTIL:
      case C_DO_WHILE:
      case C_UNTIL:
      case C_WHILE:
      case C_IF_THEN:
      case C_ELSEIF:
      case C_ELSE:
      case C_SELECT_CASE:
      case C_CASE:
      case C_CASE_IF:
      case C_CASE_IS:
      case C_CASE_ELSE:
      case C_FUNCTION:
      case C_SUB:
         Indention++;
         break;
      }
   }


   /* return */

   CURTASK         rescan = FALSE;
   return TRUE;

}

/***************************************************************
  
        FUNCTION:       fslt_clear()
  
        DESCRIPTION: This C function clears all existing memory
            in the FUNCTION-SUB lookup table.
  
***************************************************************/

static int
fslt_clear(void)
{
   struct fslte   *current, *next;
   struct bwb_variable *c, *n;

   bwx_DEBUG(__FUNCTION__);

   /* run through table and clear memory */

   next = CURTASK fslt_start.next;
   for (current = CURTASK fslt_start.next; current != &CURTASK fslt_end;
        current = next)
   {

      /* check for local variables and free them */

      c = current->local_variable;
      while (c != NULL)
      {
         n = c->next;

         /* Revised to FREE pass-thru call by JBV */
         FREE(c, "fslt_clear");
         c = n;
      }

      next = current->next;

      /* Revised to FREE pass-thru calls by JBV */
      if (current->name != NULL)
      {
         FREE(current->name, "fslt_clear");  /* JBV */
         current->name = NULL;   /* JBV */
      }
      FREE(current, "fslt_clear");
      current = NULL;   /* JBV */
   }

   /* reset linkage */

   CURTASK         fslt_start.next = &CURTASK fslt_end;

   return TRUE;
}


/***************************************************************
  
        FUNCTION:       fslt_add()
  
        DESCRIPTION: This C function adds an entry to the
            FUNCTION-SUB lookup table.
  
***************************************************************/

int
fslt_add(struct bwb_line * line /* , int *position , int code */ )
{
   char            tbuf[BasicStringLengthMax + 1];
   char           *name;
   struct fslte   *f, *n;
   int             p;
   int            *position;
   position = &p;
   p = line->Startpos;

   bwx_DEBUG(__FUNCTION__);

   /* get the element for name */

   if ( /* code == EXEC_LABEL */ line->cmdnum == C_USER_LBL)
   {
      p = 0;
      scan_element(line->buffer, &p, tbuf);
      tbuf[strlen(tbuf) - 1] = '\0';   /* remove trailing colon */
   }
   else
   {
      adv_ws(line->buffer, position);
      exp_getvfname(&(line->buffer[*position]), tbuf);
      *position += strlen(tbuf);
   }


   /* get memory for name buffer */

   /* Revised to CALLOC pass-thru call by JBV */
   if ((name = CALLOC(1, strlen(tbuf) + 1, "fslt_add")) == NULL)
   {
      bwb_error("in fslt_add(): failed to get memory for name buffer");
      return FALSE;
   }
   strcpy(name, tbuf);

   /* get memory for fslt structure */

   if ((f = CALLOC(1, sizeof(struct fslte), "fslt_add")) == NULL)
   {
      bwb_error("in fslt_add(): failed to get memory for fslt structure");
      return FALSE;
   }
   /* fill in structure */

   f->line = line;
   f->name = name;
   f->local_variable = NULL;
   f->ParameterCount = 0;  /* 0..32, 255 == VARIAMT */
   f->ParameterTypes = 0;  /* bit 0 is first parameter */







   /* read arguments */

   adv_ws(line->buffer, position);

   f->startpos = *position;



   if (line->buffer[*position] == '(')
   {
      scan_readargs(f, line, position);
   }
   /* establish linkages */

   n = CURTASK fslt_start.next;
   CURTASK         fslt_start.next = f;
   f->next = n;



   fnc_add_deffn(f);



   return TRUE;

}

/***************************************************************
  
        FUNCTION:       scan_readargs()
  
        DESCRIPTION: This C function reads arguments (variable
            names for an entry added to the FUNCTION-
         SUB lookup table.
  
***************************************************************/

static int
scan_readargs(struct fslte * f, struct bwb_line * line, int *position)
{
   int             control_loop;
   struct bwb_variable *v;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   f->ParameterCount = 0;  /* 0..254, 255 == ... */
   f->ParameterTypes = 0;  /* bit 0 is first parameter */


   /* if we are at begin paren, advance */

   if (line->buffer[*position] == '(')
   {
      ++(*position);
   }
   /* loop through looking for arguments */

   control_loop = TRUE;
   adv_ws(line->buffer, position);


   if (strncasecmp(&(line->buffer[*position]), "...", 3) == 0)
   {
      /* FUNCTION FRED( ... ) */
      (*position) += 3;
      adv_ws(line->buffer, position);
      if (line->buffer[*position] == ')')
      {
         f->ParameterCount = 0xFF;  /* VARIANT */
         f->ParameterTypes = 0;
         ++(*position);
         f->startpos = *position;
         return TRUE;
      }
      bwb_error(err_syntax);
      return FALSE;
   }
   while (control_loop == TRUE)
   {


      if (line->buffer[*position] == '\0' || line->buffer[*position] == OptionCommentChar)
      {
         /* premature end of line */
         control_loop = FALSE;
         f->startpos = *position;
         bwb_error(err_syntax);
         return FALSE;
      }
      switch (line->buffer[*position])
      {
      case ')':   /* end of argument list */
         ++(*position);
         control_loop = FALSE;
         f->startpos = *position;
         return TRUE;


      case '=':   /* end of NO argument list */
         control_loop = FALSE;
         f->startpos = *position;
         return TRUE;


      default: /* presume beginning of argument == variable
             * name */

         exp_getvfname(&(line->buffer[*position]), tbuf);
         *position += strlen(tbuf);


         /* initialize the variable and add it to local chain */

         v = var_new(tbuf);
         fslt_addlocalvar(f, v);

         /* advance past the comma */

         if (line->buffer[*position] == ',')
         {
            ++(*position);
         }
         if (f->ParameterCount > 32)
         {
            /* should have been declared VARIANT */
            bwb_error(err_syntax);
            return FALSE;
         }
         else
         {
            if (v->type == STRING)
            {
               f->ParameterTypes |= (1 << f->ParameterCount);
            }
            f->ParameterCount++; /* 0..32, 255 == VARIANT */
         }

         break;
      }

      adv_ws(line->buffer, position);
   }

   return TRUE;

}


/***************************************************************
  
        FUNCTION:       bwb_def()
  
        DESCRIPTION:    This C function implements the BASIC
                        DEF statement. Since DEF and FUNCTION
         are equivalent, it simply passes execution
         to bwb_function().
  
   SYNTAX:     DEF FNname(arg...)] = expression
  
   NOTE:    It is not a strict requirement that the
         function name should begin with "FN".
  
***************************************************************/

struct bwb_line *
bwb_DEF(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* this line will be executed by fnc_deffn() in bwb_fnc.c */
   adv_eos(l->buffer, &(l->position));

   return bwb_zline(l);
}



/***************************************************************
  
        FUNCTION:       bwb_call()
  
        DESCRIPTION: This C function implements the BASIC
            CALL subroutine command.
  
   SYNTAX:     CALL subroutine-name
  
***************************************************************/

struct bwb_line *
bwb_CALL(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* Call the expression interpreter to evaluate the function */

   /* prn_xprintf(Buffer); */
   bwb_exp(l->buffer, FALSE, &l->position);
   if (ERROR_PENDING)
   {
      /* prn_xprintf("oops"); */
   }
   adv_eos(l->buffer, &(l->position));
   return bwb_zline(l);
#if 0
   return bwb_DEF_SUB(l);
#endif
}


/***************************************************************
  
   FUNCTION:   find_label()
  
   DESCRIPTION:   This C function finds a program line that
         begins with the label included in <buffer>.
  
***************************************************************/

struct bwb_line *
find_label(char *buffer)
{
   bwx_DEBUG(__FUNCTION__);

   while (*buffer == ' ')
      buffer++;


   if (*buffer == 0)
   {
      bwb_error(err_noln);
   }
   else
   if (isdigit(*buffer))
   {
      /* LINE NUMBER */
      int             number;
      number = atoi(buffer);
      if (number > 0)
      {
         struct bwb_line *x;
         for (x = &CURTASK bwb_start; x != &CURTASK bwb_end; x = x->next)
         {
            if (x->number == number)
            {
               return x;
            }
         }
      }
      sprintf(bwb_ebuf, "Line number %d not found", number);
      bwb_error(bwb_ebuf);

   }
   else
   if (OptionFlags & OPTION_LABELS_ON)
   {
      /* LABEL */
      if (isalpha(*buffer))
      {
         struct fslte   *f;

         for (f = CURTASK fslt_start.next; f != &(CURTASK fslt_end); f = f->next)
         {
            if (strcasecmp(buffer, f->name) == 0)
            {
               return f->line;
            }
         }
      }
      sprintf(bwb_ebuf, "Line label '%s' not found", buffer);
      bwb_error(bwb_ebuf);
   }
   else
   {
      sprintf(bwb_ebuf, "Line label '%s' is not allowed", buffer);
      bwb_error(bwb_ebuf);
   }

   return NULL;

}

/***************************************************************
  
        FUNCTION:       bwb_exit()
  
   DESCRIPTION:    This C function implements the BASIC EXIT
         statement, calling subroutines for either
         EXIT FOR or EXIT DO.
  
   SYNTAX:     EXIT FOR|DO
  
***************************************************************/

struct bwb_line *
bwb_EXIT(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   sprintf(bwb_ebuf, "in bwb_exit(): Nonsense or nothing following EXIT");
   bwb_error(bwb_ebuf);

   return bwb_zline(l);
}



/***************************************************************
  
   FUNCTION:       bwb_etov()
  
   DESCRIPTION:    This function assigns the value of a
         bwBASIC expression stack element (src)
         to the value of a bwBASIC variable (dst).
  
***************************************************************/

struct bwb_variable *
bwb_etov(struct bwb_variable * dst, struct exp_ese * src)
{

   if ((int) src->type != dst->type)
   {
      sprintf(bwb_ebuf, "in bwb_etov(): mismatch src <%d> dst <%d>",
         src->type, dst->type);
      bwb_error(bwb_ebuf);
      return NULL;
   }
   if (dst->type == NUMBER)
   {
      *var_findnval(dst, dst->array_pos) = exp_getnval(src);
   }
   else
   {
      str_btob(var_getsval(dst), exp_getsval(src));
   }

   return dst;
}





/***************************************************************
  
   FUNCTION:       fslt_addlocalvar()
  
   DESCRIPTION:    This function adds a local variable
         to the FUNCTION-SUB lookup table at
         a specific level.
  
***************************************************************/

int
fslt_addlocalvar(struct fslte * f, struct bwb_variable * v)
{
   bwx_DEBUG(__FUNCTION__);

   /* find end of local chain */

   if (f->local_variable == NULL)
   {
      f->local_variable = v;
   }
   else
   {
      struct bwb_variable *p;
      struct bwb_variable *c;

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

/***************************************************************
  
   FUNCTION:       fslt_init()
  
   DESCRIPTION:    This function initializes the FUNCTION-SUB
         lookup table.
  
***************************************************************/

int
fslt_init(int task)
{
   bwx_DEBUG(__FUNCTION__);

   LOCALTASK       fslt_start.next = &(LOCALTASK fslt_end);
   return TRUE;
}


struct bwb_line *
bwb_USER_LBL(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   adv_eos(l->buffer, &(l->position));

   return bwb_zline(l);
}





/* EOF */
