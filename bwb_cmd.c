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
/*---------------------------------------------------------------*/


#include "bwbasic.h"



static int
xl_line(FILE * file, struct bwb_line * l);


static int
                LastLineNumber = -1;


/***************************************************************
  
   FUNCTION:       bwb_rem()
  
   DESCRIPTION:    This C function implements the BASIC rem
         (REMark) command, ignoring the remainder
         of the line.
  
***************************************************************/

struct bwb_line *
bwb_REM(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   adv_eos(l->buffer, &(l->position));
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_let()
  
   DESCRIPTION:    This C function implements the BASIC
         LET assignment command, even if LET
         is implied and not explicit.
  
   SYNTAX:     LET variable = expression
  
***************************************************************/

struct bwb_line *
bwb_LET(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);


   /* Call the expression interpreter to evaluate the assignment */

   bwb_exp(l->buffer, TRUE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       bwb_go
  
        DESCRIPTION: This C function implements the BASIC
         GO command, branching appropriately to
         GOTO or GOSUB.
  
***************************************************************/

struct bwb_line *
bwb_GO(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   bwb_error(err_syntax);

   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       bwb_goto
  
        DESCRIPTION: This C function implements the BASIC
         GOTO command.
  
      SYNTAX:     GOTO line | label
  
***************************************************************/


struct bwb_line *
bwb_GOTO(struct bwb_line * l)
{
   struct bwb_line *x;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   adv_ws(l->buffer, &(l->position));
   adv_element(l->buffer, &(l->position), tbuf);

   /* check for target label */
   x = find_label(tbuf);
   if (x != NULL)
   {
      x->position = 0;
      return x;
   }
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_gosub()
  
   DESCRIPTION:    This function implements the BASIC GOSUB
         command.
  
      SYNTAX:     GOSUB line | label
  
***************************************************************/

struct bwb_line *
bwb_GOSUB(struct bwb_line * l)
{
   struct bwb_line *x;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   adv_ws(l->buffer, &(l->position));
   adv_element(l->buffer, &(l->position), tbuf);

   x = find_label(tbuf);
   if (x != NULL)
   {
      bwb_zline(l);

      bwb_incexec();
      /* set the new position to x and return x */
      x->position = 0;
      bwb_setexec(x, 0, EXEC_GOSUB);

      return x;
   }
   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       bwb_return()
  
   DESCRIPTION:    This function implements the BASIC RETURN
         command.
  
      SYNTAX:     RETURN
  
***************************************************************/

struct bwb_line *
bwb_RETURN(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

   while (CURTASK excs[CURTASK exsc].code != EXEC_GOSUB)
   {
      bwb_decexec();
      if (CURTASK excs[CURTASK exsc].code == EXEC_NORM)  /* End of the line? */
      {
         bwb_error(err_retnogosub);
      }
   }

   /* decrement the EXEC stack counter */

   bwb_decexec();

   return CURTASK excs[CURTASK exsc].line;

}

/***************************************************************
  
        FUNCTION:       bwb_on
  
        DESCRIPTION:    This function implements the BASIC ON...
                        GOTO or ON...GOSUB statements.
  
         It will also detect the ON ERROR... statement
         and pass execution to bwb_onerror().
  
      SYNTAX:     ON variable GOTO|GOSUB line|label[,...]
  
  
***************************************************************/

struct bwb_line *
bwb_ON(struct bwb_line * l)
{
   struct bwb_line *x;
   char            tbuf[BasicStringLengthMax + 1];
   int             p;
   struct exp_ese *rvar;
   int             v;
   int             loop;
   int             num_lines;
   int             command;

   bwx_DEBUG(__FUNCTION__);

   /* Check for argument */

   adv_ws(l->buffer, &(l->position));

   if (l->buffer[l->position] == '\0' || l->buffer[l->position] == OptionCommentChar)
   {
      bwb_error(err_incomplete);
      return bwb_zline(l);
   }
   /* get the variable name, numerical constant, or expression */

   adv_element(l->buffer, &(l->position), tbuf);

   /* evaluate the variable name or constant */

   p = 0;
   rvar = bwb_exp(tbuf, FALSE, &p);
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   v = exp_getival(rvar);



   /* Get GOTO or GOSUB statements */

   adv_element(l->buffer, &(l->position), tbuf);
   if (strcasecmp(tbuf, "GO") == 0)
   {
      adv_ws(l->buffer, &(l->position));
      adv_element(l->buffer, &(l->position), tbuf);

      if (strcasecmp(tbuf, "TO") == 0)
      {
         command = C_GOTO;
      }
      else
      if (strcasecmp(tbuf, "SUB") == 0)
      {
         command = C_GOSUB;
      }
      else
      {
         sprintf(bwb_ebuf, ERR_ONNOGOTO);
         bwb_error(bwb_ebuf);

         return bwb_zline(l);
      }
   }
   else
   if (strcasecmp(tbuf, "GOTO") == 0)
   {
      command = C_GOTO;
   }
   else
   if (strcasecmp(tbuf, "GOSUB") == 0)
   {
      command = C_GOSUB;
   }
   else
   {
      sprintf(bwb_ebuf, ERR_ONNOGOTO);
      bwb_error(bwb_ebuf);

      return bwb_zline(l);
   }

   if (v < 1)
   {
      if (OptionFlags & OPTION_BUGS_ON)
      {
         /* fall-thru to next line without error */
         /* advance to end of segment */
         adv_eos(l->buffer, &(l->position));
      }
      else
      {
         /* ERROR */
         bwb_error(err_valoorange);
      }
      return bwb_zline(l);
   }
   num_lines = 0;

   loop = TRUE;
   while (loop == TRUE)
   {

      /* read a line|label */

      inp_adv(l->buffer, &(l->position));
      adv_element(l->buffer, &(l->position), tbuf);


      num_lines++;

      if (num_lines == v)
      {
         loop = FALSE;
      }
      /* check for end of line */

      adv_ws(l->buffer, &(l->position));
      if (l->buffer[l->position] == '\0' || l->buffer[l->position] == OptionCommentChar)
      {
         loop = FALSE;
      }
   }

   /* advance to end of segment */

   adv_eos(l->buffer, &(l->position));

   /* Be sure value is in range */

   if (v > num_lines)
   {
      if (OptionFlags & OPTION_BUGS_ON)
      {
         /* fall-thru to next line without error */
         /* advance to end of segment */
         adv_eos(l->buffer, &(l->position));
      }
      else
      {
         /* ERROR */
         bwb_error(err_valoorange);
      }
      return bwb_zline(l);
   }
   x = find_label(tbuf);
   if (x == NULL)
   {
      return bwb_zline(l);
   }
   if (command == C_GOTO)
   {


      x->position = 0;
      bwb_setexec(x, 0, CURTASK excs[CURTASK exsc].code);

      return x;

   }
   else
   if (command == C_GOSUB)
   {


      /* save current stack level */

      bwb_setexec(l, l->position, CURTASK excs[CURTASK exsc].code);

      /* increment exec stack */

      bwb_incexec();


      /* set the new position to x and return x */
      x->position = 0;
      bwb_setexec(x, 0, EXEC_GOSUB);

      return x;
   }
   sprintf(bwb_ebuf, "in bwb_on(): invalid value for command.");
   bwb_error(bwb_ebuf);

   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       bwb_stop()
  
   DESCRIPTION:    This C function implements the BASIC
         STOP command, interrupting program flow
         at a specific point.
  
      SYNTAX:     STOP
  
***************************************************************/

struct bwb_line *
bwb_STOP(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   stopped_line = l->number;
   bwx_STOP();

   return bwb_END(l);
}

/***************************************************************
  
        FUNCTION:       bwb_xend()
  
   DESCRIPTION:    This C function implements the BASIC
         END command, checking for END SUB
         or END FUNCTION, else stopping program
         execution for a simple END command.
  
***************************************************************/

struct bwb_line *
bwb_END(struct bwb_line * l)
{


   /* a simple END statement */
   bwx_DEBUG(__FUNCTION__);

   stopped_line = l->number;

   break_handler();

   return &CURTASK bwb_end;



}

/***************************************************************
  
        FUNCTION:       bwb_run()
  
        DESCRIPTION: This C function implements the BASIC
         RUN command.
  
         Even though RUN is not a core statement,
         the function bwb_run() is called from
         core, so it must be present for a minimal
         implementation.
  
      SYNTAX:     RUN [line]|[file-name]
  
***************************************************************/

struct bwb_line *
bwb_RUN(struct bwb_line * l)
{
   struct bwb_line *current;

   bwx_DEBUG(__FUNCTION__);

   /* see if there is an element */
   current = NULL;

   adv_ws(l->buffer, &(l->position));
   if (l->buffer[l->position] == '\0' || l->buffer[l->position] == OptionCommentChar)
   {
      current = CURTASK bwb_start.next;
      var_CLEAR();
   }
   else
   {
      struct exp_ese *e;

      e = bwb_exp(l->buffer, FALSE, &(l->position));
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      if (e->type == STRING)
      {
         /* check its type: if it is a STRING, open the file
          * and execute it */
         FILE           *input;
         char            tbuf[BasicStringLengthMax + 1];

         bwb_NEW(l); /* clear memory */
         str_btoc(tbuf, exp_getsval(e));  /* get string in tbuf */
         if ((input = fopen(tbuf, "r")) == NULL)   /* open file */
         {
            sprintf(bwb_ebuf, err_openfile, tbuf);
            bwb_error(bwb_ebuf);
         }
         bwb_fload(input); /* load program */
         current = &CURTASK bwb_start; /* JBV */
      }
      else
      {
         /* check its type: if it is a NUMBER, execute the
          * line */
         int             go_lnumber;   /* line number to go to */
         struct bwb_line *x;

         go_lnumber = exp_getival(e);
         for (x = CURTASK bwb_start.next; x != &CURTASK bwb_end; x = x->next)
         {
            if (x->number == go_lnumber)
            {
               current = x;
            }
         }
         if (current == NULL)
         {
            sprintf(bwb_ebuf, err_lnnotfound, go_lnumber);
            bwb_error(bwb_ebuf);
            return &CURTASK bwb_end;
         }
      }
   }
   bwb_scan();

   current->position = 0;
   CURTASK         exsc = 0;
   bwb_setexec(current, 0, EXEC_NORM);


   /* RUN */
   bwb_Warning_Clear();
   stopped_line = 0;
   SetOnError(0);


   return current;

}

/***************************************************************
  
        FUNCTION:       bwb_cont()
  
        DESCRIPTION: This C function implements the BASIC
         CONT command.
  
      SYNTAX:     CONT
  
***************************************************************/

struct bwb_line *
bwb_CONT(struct bwb_line * l)
{
   struct bwb_line *current, *x;

   bwx_DEBUG(__FUNCTION__);


   /* see if there is an element */

   current = NULL;

   if (stopped_line > 0)
   {
      for (x = CURTASK bwb_start.next; (x != &CURTASK bwb_end) && (current == NULL); x = x->next)
      {
         if (x->number > stopped_line)
         {
            current = x;
         }
      }
   }
   if (current == NULL)
   {
      /* same as "RUN" */
      current = CURTASK bwb_start.next;
   }
   bwb_scan();

   current->position = 0;
   CURTASK         exsc = 0;
   bwb_setexec(current, 0, EXEC_NORM);

   /* CONT */
   stopped_line = 0;


   return current;

}


/***************************************************************
  
        FUNCTION:       bwb_new()
  
        DESCRIPTION: This C function implements the BASIC
         NEW command.
  
         Even though NEW is not a core statement,
         the function bwb_run() is called from
         core, so it must be present for a minimal
         implementation.
  
   SYNTAX:     NEW
  
***************************************************************/

struct bwb_line *
bwb_NEW(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* clear program in memory */
   bwb_xnew(&bwb_start);

   /* clear all variables */
   var_CLEAR();

   /* NEW */
   bwb_Warning_Clear();
   stopped_line = 0;
   SetOnError(0);


   return bwb_zline(l);

}

/* End of Core Functions Section */


/***************************************************************
  
        FUNCTION:       bwb_system()
  
   DESCRIPTION:    This C function implements the BASIC
         SYSTEM command, exiting to the operating
         system (or calling program).  It is also
         called by the QUIT command, a functional
         equivalent for SYSTEM in Bywater BASIC.
  
   SYNTAX:     SYSTEM
         QUIT
  
***************************************************************/
struct bwb_line *
bwb_QUIT(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   return bwb_SYSTEM(l);
}

struct bwb_line *
bwb_SYSTEM(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   prn_xprintf("\n");
   bwx_terminate();
   return &CURTASK bwb_end;/* to make LINT happy */

}

/***************************************************************
  
        FUNCTION:       bwb_load()
  
        DESCRIPTION: This C function implements the BASIC
         LOAD command.
  
      SYNTAX:     LOAD [file-name]
  
***************************************************************/

struct bwb_line *
bwb_LOAD(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* clear current contents */
   bwb_NEW(l);

   /* call xload function to load program in memory */

   bwb_xload(l);

   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       bwb_xload()
  
        DESCRIPTION: This C function loads a BASIC program
         into memory.
  
***************************************************************/

struct bwb_line *
bwb_xload(struct bwb_line * l)
{
   FILE           *loadfile;

   bwx_DEBUG(__FUNCTION__);

   /* Get an argument for filename */

   adv_ws(l->buffer, &(l->position));

   if (l->buffer[l->position] == '\0' || l->buffer[l->position] == OptionCommentChar)
   {
      /* default is the last filename used by LOAD or SAVE */
   }
   else
   {
      /* Section added by JBV (bug found by DD) */
      struct exp_ese *e;   /* JBV */
      e = bwb_exp(l->buffer, FALSE, &(l->position));
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      if (e->type != STRING)
      {
         sprintf(bwb_ebuf, "in bwb_xload(): Missing filespec");
         bwb_error(bwb_ebuf);
         return bwb_zline(l);
      }
      str_btoc(CURTASK progfile, exp_getsval(e));  /* JBV */
   }
   if (strlen(CURTASK progfile) == 0)
   {
      bwb_error(err_nofn); /* Added by JBV (bug found by DD) */
      return bwb_zline(l);
   }
   if ((loadfile = fopen(CURTASK progfile, "r")) == NULL)
   {
      sprintf(bwb_ebuf, err_openfile, CURTASK progfile);
      bwb_error(bwb_ebuf);

      return bwb_zline(l);
   }
   bwb_fload(loadfile);


   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_save()
  
   DESCRIPTION:    This C function implements the BASIC
         SAVE command.
  
      SYNTAX:     SAVE [file-name]
  
***************************************************************/
#define CSAVE_VERSION_1 0x20150218

struct bwb_line *
bwb_CSAVE_(struct bwb_line * l)
{
   /* CSAVE* NumericArrayName */
   char            tbuf[BasicStringLengthMax + 1];
   struct bwb_variable *v;
   FILE           *f;
   unsigned long   n;
   unsigned long   t;


   bwx_DEBUG(__FUNCTION__);

   bwb_getvarname(l->buffer, tbuf, &(l->position));
   v = var_find(tbuf);
   if (v == NULL)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* variable MUST be numeric */
   if (v->type == STRING)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* variable MUST be an array */
   if ((v->dimensions == 1) && (v->array_sizes[0] == 1))
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* * variable storage is a mess, * we bypass that tradition here. */
   t = 1;
   for (n = 0; n < v->dimensions; n++)
   {
      t *= v->array_sizes[n];
   }
   if (t <= 1)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* open file */
   f = fopen(v->name, "w");
   if (f == NULL)
   {
      bwb_error(err_syntax);
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
   fclose(f);
   return bwb_zline(l);
}


struct bwb_line *
bwb_CLOAD_(struct bwb_line * l)
{
   /* CLOAD* NumericArrayName */
   char            tbuf[BasicStringLengthMax + 1];
   struct bwb_variable *v;
   FILE           *f;
   unsigned long   n;
   unsigned long   t;


   bwx_DEBUG(__FUNCTION__);

   bwb_getvarname(l->buffer, tbuf, &(l->position));
   v = var_find(tbuf);
   if (v == NULL)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* variable MUST be numeric */
   if (v->type == STRING)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* variable MUST be an array */
   if ((v->dimensions == 1) && (v->array_sizes[0] == 1))
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* variable storage is a mess, we bypass that tradition here. */
   t = 1;
   for (n = 0; n < v->dimensions; n++)
   {
      t *= v->array_sizes[n];
   }
   if (t <= 1)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* open file */
   f = fopen(v->name, "r");
   if (f == NULL)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* read version number */
   n = 0;
   fread(&n, sizeof(long), 1, f);
   if (n != CSAVE_VERSION_1)
   {
      fclose(f);
      bwb_error("Bad File");
      return bwb_zline(l);
   }
   /* read total number of elements */
   n = 0;
   fread(&n, sizeof(long), 1, f);
   if (n != t)
   {
      fclose(f);
      bwb_error("Size Mismatch");
      return bwb_zline(l);
   }
   /* read data */
   fread(v->memnum, sizeof(BasicNumberType), t, f);
   /* OK */
   fclose(f);
   return bwb_zline(l);
}



struct bwb_line *
bwb_CSAVE(struct bwb_line * l)
{
   /* CSAVE [filename$] */
   bwx_DEBUG(__FUNCTION__);
   return bwb_SAVE(l);
}

struct bwb_line *
bwb_CLOAD(struct bwb_line * l)
{
   /* CLOAD [filename$] */
   bwx_DEBUG(__FUNCTION__);
   return bwb_LOAD(l);
}


struct bwb_line *
bwb_SAVE(struct bwb_line * l)
{
   FILE           *outfile;

   bwx_DEBUG(__FUNCTION__);


   /* Get an argument for filename */

   adv_ws(l->buffer, &(l->position));
   if (l->buffer[l->position] == '\0' || l->buffer[l->position] == OptionCommentChar)
   {
      /* default is the last filename used by LOAD or SAVE */
   }
   else
   {
      /* Section added by JBV (bug found by DD) */
      struct exp_ese *e;   /* JBV */
      e = bwb_exp(l->buffer, FALSE, &(l->position));
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      if (e->type != STRING)
      {
         sprintf(bwb_ebuf, "in bwb_save(): Missing filespec");
         bwb_error(bwb_ebuf);
         return bwb_zline(l);
      }
      str_btoc(CURTASK progfile, exp_getsval(e));  /* JBV */
   }
   if (strlen(CURTASK progfile) == 0)
   {
      bwb_error(err_nofn); /* Added by JBV (bug found by DD) */
      return bwb_zline(l);
   }
   if ((outfile = fopen(CURTASK progfile, "w")) == NULL)
   {
      sprintf(bwb_ebuf, err_openfile, CURTASK progfile);
      bwb_error(bwb_ebuf);

      return bwb_zline(l);
   }
   bwb_xlist(l, outfile);
   fclose(outfile);


   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       bwb_list()
  
        DESCRIPTION: This C function implements the BASIC
         LIST command.
  
      SYNTAX:     LIST line[-line]
  
***************************************************************/

struct bwb_line *
bwb_LIST(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   bwb_xlist(l, stdout);
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_xlist()
  
        DESCRIPTION: This C function lists the program in
         memory to a specified output device.
  
***************************************************************/

struct bwb_line *
bwb_xlist(struct bwb_line * l, FILE * file)
{
   struct bwb_line *start, *end, *current;
   BasicLineNumberType s, e;
   int             f, r;

   bwx_DEBUG(__FUNCTION__);

   /* Default first value */
   start = CURTASK bwb_start.next;
   s = BasicLineNumberMin;


   /* Default last value */
   end = &CURTASK bwb_end;
   e = BasicLineNumberMax;


   /* Parse parameters */
   r = bwb_numseq(&(l->buffer[l->position]), &s, &e);
/*  
r example
= ================
0 LIST
1 LIST 1000
4 LIST     -
5 LIST 1000-
6 LIST     -9000
7 LIST 1000-9000
*/



   if (r == 1)
   {
      /* LIST 1000 */
      e = s;
   }
   /* advance to the end of the segment */
   adv_eos(l->buffer, &(l->position));


   /* abort if either number is out of range */
   if (s < BasicLineNumberMin || s > BasicLineNumberMax)
   {
      /* LIST 99999- */
      sprintf(bwb_ebuf, err_lnnotfound, s);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   if (e < BasicLineNumberMin || e > BasicLineNumberMax)
   {
      /* LIST -99999 */
      sprintf(bwb_ebuf, err_lnnotfound, e);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   if (s > e)
   {
      /* LIST 9999-1 */
      sprintf(bwb_ebuf, err_lnnotfound, s);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   /* find the first line that actually exists that is >= START */
   f = FALSE;
   for (current = CURTASK bwb_start.next; (f == FALSE) && (current != &CURTASK bwb_end); current = current->next)
   {
      if (current->number >= s)
      {
         f = TRUE;
         start = current;
      }
   }
   /* "start" now points at the first line that actually exists that is
    * >= START */

   if (r == 1)
   {
      /* LIST 999 */
      if (s != start->number)
      {
         /* THE SPECIFIED LINE DOES NOT EXIST */
         f = FALSE;
      }
   }
   /* check and see if a line number was found */
   if (f == FALSE)
   {
      if (l->number > 0)
      {
         /* this is an executing program */
         sprintf(bwb_ebuf, err_lnnotfound, s);
         bwb_error(bwb_ebuf);
      }
      return bwb_zline(l);
   }
   f = FALSE;
   if (e > s)
   {
      /* find the last line that actually exists that is <= END */
      for (current = start; current != &CURTASK bwb_end; current = current->next)
      {
         if (current->number <= e)
         {
            f = TRUE;
            end = current;
         }
      }
   }
   else
   {
      /* e == s */
      f = TRUE;
      end = start;
   }
   /* "end" now points at the last line that actually exists that is <=
    * END */



   if (r == 6 || r == 7)
   {
      /* LIST -999 */
      if (e < end->number)
      {
         /* THERE ARE NO LINES LESS THAN SPECIFIED */
         f = FALSE;
      }
   }
   /* "end" is always after "start" */
   end = end->next;
   while (end->number <= e)
   {
      end = end->next;
   }
   /* * "end" now points at the first line * that actually exists that
    * is > END */


   /* check and see if a line number was found */
   if (f == FALSE)
   {
      if (l->number > 0)
      {
         /* this is an executing program */
         sprintf(bwb_ebuf, err_lnnotfound, e);
         bwb_error(bwb_ebuf);
      }
      return bwb_zline(l);
   }
   /* now go through and list appropriate lines */
   bwb_scan();
   LastLineNumber = -1;
   for (current = start; current != end; current = current->next)
   {
      xl_line(file, current);
   }
   fprintf(file, "\n");

   return bwb_zline(l);


}

/***************************************************************
  
   FUNCTION:       xl_line()
  
   DESCRIPTION:    This function lists a single program
         line to a specified device of file.
         It is called by bwb_xlist();
  
***************************************************************/

static int
xl_line(FILE * file, struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   if ((file == stdout) || (file == stderr))
   {
      char            tbuf[32];


      if (LastLineNumber == l->number)
      {
         strcpy(tbuf, "     :");
      }
      else
      {
         sprintf(tbuf, "%5d:", l->number);
      }
      prn_xprintf(tbuf);

      if (OptionFlags & OPTION_COVERAGE_ON)
      {
#if 0
         sprintf(tbuf, "%c:", l->Coverage);
         prn_xprintf(tbuf);
#endif
         if( l->LineFlags & LINE_EXECUTED )
         {
             prn_xprintf("*");
         }
         else
         {
             prn_xprintf(" ");
         }
         prn_xprintf(":");
      }
      if (OptionIndentValue > 0)
      {
         int             i;
         for (i = 0; i < l->Indention; i++)
         {
            int             j;
            for (j = 0; j < OptionIndentValue; j++)
            {
               prn_xprintf(" ");
            }
         }
      }
      prn_xprintf(l->buffer);
      prn_xprintf("\n");

   }
   else
   {

#if 0
      if (LastLineNumber == l->number)
      {
         fprintf(file, "%s\n", l->buffer);
      }
      else
      {
         fprintf(file, "%d %s\n", l->number, l->buffer);
      }
#endif
      if (LastLineNumber == l->number && OptionStatementChar != '\0' )
      {
          fprintf(file, "%c", OptionStatementChar);
      }
      else
      if(LastLineNumber == -1 )
      {
          /* first line, do nothing */
      }
      else
      {
          fprintf(file, "\n");
      }        
      
      if( l->LineFlags & LINE_NUMBERED )
      {
         fprintf(file, "%d ", l->number);
      }
      
      fprintf(file, "%s", l->buffer);
   }
   LastLineNumber = l->number;

   return TRUE;
}

/***************************************************************
  
        FUNCTION:       bwb_delete()
  
   DESCRIPTION:    This C function implements the BASIC
         DELETE command for interactive programming,
         deleting a specified program line (or lines)
         from memory.
  
      SYNTAX:     DELETE line[-line]
  
***************************************************************/


struct bwb_line *
bwb_DELETE(struct bwb_line * l)
{
   struct bwb_line *start, *end, *current, *previous, *p, *next;
   BasicLineNumberType s, e;
   int             f, r;

   bwx_DEBUG(__FUNCTION__);


   /* Default first value */
   start = CURTASK bwb_start.next;
   s = BasicLineNumberMin;


   /* Default last value */
   end = &CURTASK bwb_end;
   e = BasicLineNumberMax;


   /* Default previous value */
   previous = p = &CURTASK bwb_start;


   r = bwb_numseq(&(l->buffer[l->position]), &s, &e);
/*  
r example
= ================
0 DELETE
1 DELETE 1000
4 DELETE     -
5 DELETE 1000-
6 DELETE     -9000
7 DELETE 1000-9000
*/

   if (r == 1)
   {
      /* DELETE 1000 */
      e = s;
   }
   /* advance to the end of the segment */
   adv_eos(l->buffer, &(l->position));


   /* abort if either number is out of range */
   if (s < BasicLineNumberMin || s > BasicLineNumberMax)
   {
      /* DELETE 99999- */
      sprintf(bwb_ebuf, err_lnnotfound, s);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   if (e < BasicLineNumberMin || e > BasicLineNumberMax)
   {
      /* DELETE -99999 */
      sprintf(bwb_ebuf, err_lnnotfound, e);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   if (s > e)
   {
      /* DELETE 9999-1 */
      sprintf(bwb_ebuf, err_lnnotfound, s);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   /* find the first line that exists that is >= START */
   f = FALSE;
   for (current = CURTASK bwb_start.next; (f == FALSE) && (current != &CURTASK bwb_end); current = current->next)
   {
      if (current->number >= s)
      {
         f = TRUE;
         previous = p;
         start = current;
      }
      p = current;
   }
   /* "start" now points at the first line that actually exists that is
    * >= START */
   /* "previous" now points at the first line that actually exists that
    * is < START */


   if (r == 1)
   {
      /* DELETE 1000 */
      if (s != start->number)
      {
         /* THE SPECIFIED LINE DOES NOT EXIST */
         f = FALSE;
      }
   }
   /* check and see if a line number was found */
   if (f == FALSE)
   {
      if (l->number > 0)
      {
         /* this is an executing program */
         sprintf(bwb_ebuf, err_lnnotfound, s);
         bwb_error(bwb_ebuf);
      }
      return bwb_zline(l);
   }
   f = FALSE;
   if (e > s)
   {
      /* find the last line that actually exists that is <= END */
      for (current = start; current != &CURTASK bwb_end; current = current->next)
      {
         if (current->number <= e)
         {
            f = TRUE;
            end = current;
         }
      }
   }
   else
   {
      /* e == s */
      f = TRUE;
      end = start;
   }
   /* "end" now points at the last line that actually exists that is <=
    * END */


   if (r == 6 || r == 7)
   {
      /* DELETE -999 */
      /* DELETE 1-999 */
      if (e < end->number)
      {
         /* THERE ARE NO LINES LESS THAN SPECIFIED */
         f = FALSE;
      }
   }
   /* "end" is always after "start" */
   end = end->next;
   while (end->number <= e)
   {
      end = end->next;
   }
   /* * "end" now points at the first line * that actually exists * that
    * is > END */


   /* check and see if a line number was found */
   if (f == FALSE)
   {
      if (l->number > 0)
      {
         /* this is an executing program */
         sprintf(bwb_ebuf, err_lnnotfound, e);
         bwb_error(bwb_ebuf);
      }
      return bwb_zline(l);
   }
   if (l->number > 0)
   {
      /* This is an executing progrram. So make sure DELETE removes
       * structured commands completely. The BASIC program must
       * delete all of a structured command. Do not allow a
       * dangling FOR, NEXT, and so on, since that is just a FATAL
       * error waiting to happen. FOR - NEXT DO - LOOP WHILE - WEND
       * UNTIL - UEND IF - END IF SELECT CASE - END SELECT */
      s = start->number;
      e = end->number;
      current = start;
      while ((current != end) && (current != &CURTASK bwb_end))
      {
         if (current->OtherLine != NULL)
         {
            /* this line is either the Top or the Bottomm
             * of a structured command */
            /* verify the other end of the block is also
             * in the DELETE range */
            int             n;
            n = current->OtherLine->number;
            if (n < s || n >= e)
            {
               sprintf(bwb_ebuf,
                  "Cannot delete at %d because %d is not included"
                  ,current->number
                  ,n
                  );
               bwb_error(bwb_ebuf);
               return bwb_zline(l);
            }
         }
         current = current->next;
      }
   }
   else
   {
      /* The user is executing DELETE interactively,  Allow user to
       * DELETE whatever they want. Whenever user attempts to run,
       * bwb_scan() will find any mismatch. */
   }


   /* now go through and delete appropriate lines */
   current = start;
   while ((current != end) && (current != &CURTASK bwb_end))
   {
      next = current->next;
      if (current == l)
      {
         l = previous;
      }
      if (current->OtherLine != NULL)
      {
         /* remove any existing structured command linkage */
         current->OtherLine->OtherLine = NULL;
         current->OtherLine = NULL;
      }
      bwb_freeline(current);
      current = next;
   }


   /* previous should now be set to the line previous to the first in
    * the omission list */
   previous->next = current;


   CURTASK         rescan = TRUE;   /* program needs to be scanned again */
   bwb_scan();

   return bwb_zline(l);


}


/***************************************************************
  
        FUNCTION:       bwb_donum()
  
   DESCRIPTION:    This function implements the BASIC DO
         NUM command, numbering all program lines
         in memory in increments of 10 beginning
         at 10.
  
   SYNTAX:     DO NUM
  
***************************************************************/
#if 0
struct bwb_line *
bwb_donum(struct bwb_line * l)
{
   struct bwb_line *current;
   register int    lnumber;

   bwx_DEBUG(__FUNCTION__);

   lnumber = 10;
   for (current = bwb_start.next; current != &bwb_end; current = current->next)
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

/***************************************************************
  
        FUNCTION:       bwb_dounnum()
  
   DESCRIPTION:    This function implements the BASIC DO
         UNNUM command, removing all line numbers
         from the program in memory.
  
   SYNTAX:     DO UNNUM
  
***************************************************************/

struct bwb_line *
bwb_dounnum(struct bwb_line * l)
{
   struct bwb_line *current;

   bwx_DEBUG(__FUNCTION__);

   for (current = bwb_start.next; current != &bwb_end; current = current->next)
   {
      current->number = 0;
   }

   return bwb_zline(l);
}

#endif            /* 0 */


/***************************************************************
  
        FUNCTION:       bwb_chain()
  
        DESCRIPTION: This C function implements the BASIC
         CHAIN command.
  
      SYNTAX:     CHAIN "file-name" | file-name$
  
***************************************************************/

struct bwb_line *
bwb_CHAIN(struct bwb_line * l)
{
   /* based upon bwb_xload() */
   FILE           *loadfile;
   struct exp_ese *e;

   bwx_DEBUG(__FUNCTION__);

   /* Get an argument for filename */

   adv_ws(l->buffer, &(l->position));
   if (l->buffer[l->position] == '\0' || l->buffer[l->position] == OptionCommentChar)
   {
      bwb_error(err_nofn); /* Added by JBV (bug found by DD) */
      return bwb_zline(l);
   }
   /* The file-name may be any string expression */
   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (e->type != STRING)
   {
      sprintf(bwb_ebuf, "in bwb_chain(): Missing filespec");
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   /* save the filename */
   str_btoc(CURTASK progfile, exp_getsval(e));

   /* deallocate all variables except common ones */
   var_delcvars();

   /* remove old program from memory */
   bwb_xnew(&bwb_start);


   /* call xload function to load new program in memory */

   if ((loadfile = fopen(CURTASK progfile, "r")) == NULL)
   {
      sprintf(bwb_ebuf, err_openfile, CURTASK progfile);
      bwb_error(bwb_ebuf);

      return bwb_zline(l);
   }
   bwb_fload(loadfile);

   bwb_scan();

   /* reset all stack counters */
   CURTASK         exsc = 0;
   CURTASK         expsc = 0;
   /* run the program */

   /* CHAIN */
   bwb_Warning_Clear();
   stopped_line = 0;
   SetOnError(0);

   CURTASK         bwb_start.position = 0;
   bwb_setexec(&CURTASK bwb_start, 0, EXEC_NORM);
   return &CURTASK bwb_start;
}


/***************************************************************
  
        FUNCTION:       bwb_merge()
  
        DESCRIPTION: This C function implements the BASIC
         MERGE command, merging command lines from
         a specified file into the program in memory
         without deleting the lines already in memory.
  
      SYNTAX:     MERGE file-name
  
***************************************************************/

struct bwb_line *
bwb_MERGE(struct bwb_line * l)
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
   int             i;

   bwx_DEBUG(__FUNCTION__);

   i = CURTASK exsc;
   while (i > 0)
   {
      struct bwb_line *current;

      current = CURTASK excs[i].LoopTopLine;
      if (current != NULL)
      {
         switch (current->cmdnum)
         {
         case C_FUNCTION:
         case C_SUB:
            /* FOUND */
            /* we are in a FUNCTION/SUB, so this is LOCAL */
            CURTASK excs[i].OnErrorGoto = LineNumber;
            return;
            break;
         }
      }
      i--;
   }
   /* NOT FOUND */
   /* we are NOT in a FUNCTION/SUB  */
   CURTASK         excs[0].OnErrorGoto = LineNumber;
}


int
GetOnError(void)
{
   /* scan the stack looking for a FUNCTION/SUB */
   int             i;

   bwx_DEBUG(__FUNCTION__);



   i = CURTASK exsc;
   while (i >= 0)
   {
      if (CURTASK excs[i].OnErrorGoto != 0)
      {
         /* FOUND */
         return CURTASK excs[i].OnErrorGoto;
      }
      i--;
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

struct bwb_line *
bwb_ON_ERROR_GOTO(struct bwb_line * l)
{
  /* ON ERROR GOTO line */
  struct bwb_line *x;
  char            tbuf[BasicStringLengthMax + 1];
  /* get the line number */
  adv_ws(l->buffer, &(l->position));
  adv_element(l->buffer, &(l->position), tbuf);
  if (strcasecmp(tbuf, "0") == 0)
  {
     SetOnError(0);
     return bwb_zline(l);
  }
  x = find_label(tbuf);
  if (x != NULL)
  {
     SetOnError(x->number);
     return bwb_zline(l);
  }
   bwb_error(err_syntax);

   return bwb_zline(l);
}

struct bwb_line *
bwb_ON_ERROR_GOSUB(struct bwb_line * l)
{
  /* ON ERROR GOSUB line */
  struct bwb_line *x;
  char            tbuf[BasicStringLengthMax + 1];
  /* get the line number */
  adv_ws(l->buffer, &(l->position));
  adv_element(l->buffer, &(l->position), tbuf);
  if (strcasecmp(tbuf, "0") == 0)
  {
     SetOnError(0);
     return bwb_zline(l);
  }
  x = find_label(tbuf);
  if (x != NULL)
  {
     SetOnError(x->number);
     return bwb_zline(l);
  }
   bwb_error(err_syntax);

   return bwb_zline(l);
}

struct bwb_line *
bwb_ON_ERROR_RESUME_NEXT(struct bwb_line * l)
{
     SetOnError(-1);
     return bwb_zline(l);
}

struct bwb_line *
bwb_ON_ERROR_RETURN_NEXT(struct bwb_line * l)
{
     SetOnError(-1);
     return bwb_zline(l);
}

struct bwb_line *
bwb_ON_TIMER(struct bwb_line * l)
{
   /* ON TIMER(...) GOSUB ... TIMER ON ... */
   char            tbuf[BasicStringLengthMax + 1];
   int             p;
   struct exp_ese *rvar;
   BasicNumberType v;
   BasicNumberType minv;


   bwx_DEBUG(__FUNCTION__);

   /* turn off the TIMER - so syntax errors will NOT fire the timer */
   bwb_Timer_Off();
   tmr_gotol = 0;
   tmr_count = 0;


   /* get the SECOMDS parameter */
   adv_element(l->buffer, &(l->position), tbuf);

   p = 0;
   rvar = bwb_exp(tbuf, FALSE, &p);
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   v = exp_getnval(rvar);
   minv = 1;
   minv /= CLOCKS_PER_SEC;

   if (v > minv)
   {
      /* get the GOSUB keyword */
      adv_ws(l->buffer, &(l->position));
      adv_element(l->buffer, &(l->position), tbuf);

      if (strcasecmp(tbuf, "GOSUB") == 0)
      {
         /* ON TIMER(X) GOSUB line */
         struct bwb_line *x;
         /* get the line number */
         adv_ws(l->buffer, &(l->position));
         adv_element(l->buffer, &(l->position), tbuf);
         x = find_label(tbuf);
         if (x != NULL)
         {
            tmr_gotol = x->number;
            tmr_count = v;
            return bwb_zline(l);
         }
      }
   }
   bwb_error(err_syntax);

   return bwb_zline(l);

}


struct bwb_line *
bwb_TIMER(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* turn off the TIMER - so syntax errors will NOT fire the timer */
   bwb_Timer_Off();

   bwb_error(err_syntax);

   return bwb_zline(l);
}

struct bwb_line *
bwb_TIMER_OFF(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* turn off the TIMER - so syntax errors will NOT fire the timer */
   bwb_Timer_Off();

   /* TIMER OFF */
   tmr_gotol = 0;
   tmr_count = 0;

   return bwb_zline(l);
}

struct bwb_line *
bwb_TIMER_ON(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* turn off the TIMER - so syntax errors will NOT fire the timer */
   bwb_Timer_Off();

   /* TIMER ON */
   if (tmr_count > 0 && tmr_gotol > 0)
   {
      tmr_expires = bwx_TIMER(tmr_count);
      bwb_Timer_On();
   }

   return bwb_zline(l);
}

struct bwb_line *
bwb_TIMER_STOP(struct bwb_line * l)
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
struct bwb_line *
bwb_RESUME(struct bwb_line * l)
{
   char            tbuf[BasicStringLengthMax + 1];
   struct bwb_line *x = NULL;

   bwx_DEBUG(__FUNCTION__);

   if (err_number == 0) /* RESUME without ERR are FATAL */
   {
      sprintf(bwb_ebuf, "RESUME WITHOUT ERR AT %d", CURTASK number);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   if (err_line == NULL)
   {
      /* RESUME without ERL are FATAL */
      sprintf(bwb_ebuf, "RESUME WITHOUT ERL AT %d", CURTASK number);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   if (err_line->number == 0)
   {
      /* RESUME without ERL are FATAL */
      sprintf(bwb_ebuf, "RESUME WITHOUT ERL AT %d", CURTASK number);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   /* Get optional argument for RESUME */

   adv_ws(l->buffer, &(l->position));
   adv_element(l->buffer, &(l->position), tbuf);


   if (strlen(tbuf) == 0 || strcasecmp(tbuf, "0") == 0)
   {
      /* RESUME [0] */
      /* Execution resumes at the statement which caused the error */
      x = err_line;
   }
   else
   if (strcasecmp(tbuf, "NEXT") == 0)
   {
      /* RESUME NEXT */
      /* Execution resumes at the statement immediately following
       * the one which caused the error */
       x = err_line->next;
   }
   else
   {
      /* RESUME line|label */
      x = find_label(tbuf);
   }



   if (x != NULL)
   {
      bwb_Warning_Clear();
      x->position = 0;
      return x;
   }
   return bwb_zline(l);
}





/***************************************************************
  
        FUNCTION:       bwb_xnew()
  
        DESCRIPTION: Clears the program in memory, but does not
         deallocate all variables.
  
***************************************************************/

void
bwb_xnew(struct bwb_line * l)
{
   struct bwb_line *current, *previous;
   int             wait;


   bwx_DEBUG(__FUNCTION__);

   previous = NULL;  /* JBV */
   wait = TRUE;
   for (current = l->next; current != &CURTASK bwb_end; current = current->next)
   {
      if (wait != TRUE)
      {
         /* Revised to FREE pass-thru call by JBV */
         FREE(previous, "bwb_xnew");
      }
      wait = FALSE;
      previous = current;
   }

   l->next = &bwb_end;

}


/***************************************************************
  
        FUNCTION:       bwb_cmds()
  
   DESCRIPTION:    This function implements a CMD command,
         which lists all commands implemented.
         It is not part of a BASIC specification,
         but is used for debugging bwBASIC.
  
   SYNTAX:     CMDS
  
***************************************************************/


struct bwb_line *
bwb_CMDS(struct bwb_line * l)
{
   register int    n;
   int             t;

   bwx_DEBUG(__FUNCTION__);

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

static void
CommandUniqueID(int i, char *UniqueID)
{
   char           *C;

   bwx_DEBUG(__FUNCTION__);

   strcpy(UniqueID, "C_");

   if (strcasecmp(bwb_cmdtable[i].name, "?") == 0)
   {
      strcat(UniqueID, "QUEST");
   }
   else
   {
      strcat(UniqueID, bwb_cmdtable[i].name);
   }
   C = UniqueID;
   while (*C != '\0')
   {
      if (!isalnum(*C))
         *C = '_';
      C++;
   }
}

static void
CommandVector(int i, char *Vector)
{
   char           *C;

   bwx_DEBUG(__FUNCTION__);

   strcpy(Vector, "bwb_");

   if (strcasecmp(bwb_cmdtable[i].name, "?") == 0)
   {
      strcat(Vector, "QUEST");
   }
   else
   {
      strcat(Vector, bwb_cmdtable[i].name);
   }
   C = Vector;
   while (*C != '\0')
   {
      if (!isalnum(*C))
         *C = '_';
      C++;
   }
}

static void
CommandOptionVersion(int n, char *OutputLine)
{
   int             i;
   int             j;

   bwx_DEBUG(__FUNCTION__);

   strcpy(OutputLine, "");
   j = 0;
   for (i = 0; i < NUM_VERSIONS; i++)
   {
      if (bwb_cmdtable[n].OptionVersionBitmask & bwb_vertable[i].OptionVersionBitmask)
      {
         if (j > 0)
         {
            strcat(OutputLine, " | ");
         }
         strcat(OutputLine, bwb_vertable[i].ID);
         j++;
      }
   }
}

void
SortAllCommands(void)
{
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
            if (strcasecmp(bwb_cmdtable[j].name, bwb_cmdtable[k].name) < 0)
            {
               k = j;
            }
         }
         if (k > i)
         {
            struct bwb_command t;
            memcpy(&t, &(bwb_cmdtable[i]), sizeof(struct bwb_command));
            memcpy(&(bwb_cmdtable[i]), &(bwb_cmdtable[k]), sizeof(struct bwb_command));
            memcpy(&(bwb_cmdtable[k]), &t, sizeof(struct bwb_command));
         }
      }
      IsSorted = TRUE;
   }
}

void
SortAllFunctions(void)
{
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
            n = strcasecmp(bwb_prefuncs[j].Name, bwb_prefuncs[k].Name);
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
            struct bwb_function t;
            memcpy(&t, &(bwb_prefuncs[i]), sizeof(struct bwb_function));
            memcpy(&(bwb_prefuncs[i]), &(bwb_prefuncs[k]), sizeof(struct bwb_function));
            memcpy(&(bwb_prefuncs[k]), &t, sizeof(struct bwb_function));
         }
      }
      IsSorted = TRUE;
   }
}


void
DumpAllCommandUniqueID(void)
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

static void
ProcessEscapeChars(const char *Input, char *Output)
{
   int             n;


   bwx_DEBUG(__FUNCTION__);

   n = 0;

   while (*Input != '\0')
   {
      switch (*Input)
      {
      case '\n':
         *Output = '\\';
         Output++;
         *Output = 'n';
         Output++;
         break;
      case '\t':
         *Output = '\\';
         Output++;
         *Output = 't';
         Output++;
         break;
      case '\"':
         *Output = '\\';
         Output++;
         *Output = '"';
         Output++;
         break;
      default:
         *Output = *Input;
         Output++;
         break;
      }
      Input++;
      *Output = '\0';
      n++;
      if (n > 60)
      {
         *Output = '\"';
         Output++;
         *Output = '\n';
         Output++;
         *Output = '\"';
         Output++;
         *Output = '\0';
         n = 0;
      }
   }
}

void
DumpAllCommandTableDefinitions(void)
{
   /* generate bwd_cmd.c */

   int             i;

   bwx_DEBUG(__FUNCTION__);

   prn_lprintf("/* COMMAND TABLE */\n\n");
   prn_lprintf("#include \"bwbasic.h\"\n\n");
   prn_lprintf("struct bwb_command bwb_cmdtable[ NUM_COMMANDS ] =\n");
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

void
DumpAllCommandSwitchStatement(void)
{
   int             i;

   bwx_DEBUG(__FUNCTION__);

   /* run through the command table and print comand #define */
   prn_lprintf("/* SWITCH */\n");
   prn_lprintf("struct bwb_line *bwb_vector( struct bwb_line *l )\n");
   prn_lprintf("{\n");

   prn_lprintf("   ");
   prn_lprintf("struct bwb_line *r;\n");

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
   prn_lprintf("sprintf( bwb_ebuf, \"in bwb_vector(), INTERNAL ERROR: %d not in switch\", l->cmdnum );\n");


   prn_lprintf("   ");
   prn_lprintf("   ");
   prn_lprintf("bwb_error( bwb_ebuf );\n");

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

void
DumpOneCommandSyntax(int n, int lp)
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
      if (lp)
      {
         prn_lprintf(tbuf);
      }
      else
      {
         prn_xprintf(tbuf);
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

void
DumpAllCommandSyntax(void)
{
   /* for the C maintainer */
   int             n;


   bwx_DEBUG(__FUNCTION__);

   for (n = 0; n < NUM_COMMANDS; n++)
   {
      prn_lprintf("------------------------------------------------------------\n");
      DumpOneCommandSyntax(n, TRUE);

   }
   prn_lprintf("------------------------------------------------------------\n");
}


void
DumpAllCommandHtmlTable(void)
{
   /* generate bwd_cmd.htm */

   int             i;
   int             j;

   bwx_DEBUG(__FUNCTION__);


   /* LEGEND */
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
      prn_lprintf(bwb_cmdtable[i].name);
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
   prn_lprintf("\n");
}





struct bwb_line *
bwb_HELP(struct bwb_line * l)
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
   strcpy( tbuf, C );

   C = strchr( tbuf, OptionCommentChar );
   if( C != NULL )
   {
       *C = '\0';
   }
   /* RTRIM$ */
   C = tbuf;
   if (*C != 0)
   {
      /* not an empty line, so remove one (or more) trailing spaces */
      char           *E;

      E = strchr(tbuf, 0);
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
      if (strcasecmp(bwb_cmdtable[n].name, tbuf) == 0)
      {
         prn_xprintf("------------------------------------------------------------\n");
         DumpOneCommandSyntax(n, FALSE);
         Found = TRUE;
      }
   }

   for (n = 0; n < NUM_FUNCTIONS; n++)
   {
      if (strcasecmp(bwb_prefuncs[n].Name, tbuf) == 0)
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
      Length = strlen(tbuf);

      for (n = 0; n < NUM_COMMANDS; n++)
      {
         if (strncasecmp(bwb_cmdtable[n].name, tbuf, Length) == 0)
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
         if (strncasecmp(bwb_prefuncs[n].Name, tbuf, Length) == 0)
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
   adv_eos(l->buffer, &(l->position));
   return bwb_zline(l);

}

int
NumberValueCheck(unsigned long ParameterTests, BasicNumberType X)
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
   XR = rint(X);
   ParameterTests &= 0x0000000F;
   switch (ParameterTests)
   {
   case P1ERR:
      {
         /* INTERNAL ERROR */
         return -1;
      }
      break;
   case P1ANY:
      if (XR < -DBL_MAX || XR > DBL_MAX)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
   case P1BYT:
      if (XR < 0 || XR > UCHAR_MAX)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
      break;
   case P1INT:
      if (XR < SHRT_MIN || XR > SHRT_MAX)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
      break;
   case P1LNG:
      if (XR < LONG_MIN || XR > LONG_MAX)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
      break;
   case P1CUR:
      if (XR < LONG_MIN || XR > LONG_MAX)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
      break;
   case P1FLT:
      if (X < -FLT_MAX || X > FLT_MAX)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
      break;
   case P1DBL:
      if (X < -DBL_MAX || X > DBL_MAX)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
      break;
   case P1DEV:
      /* BasicFileNumberMax must be <= INT16_MAX */
      if (XR < 0 || XR > BasicFileNumberMax)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
      break;
   case P1LEN:
      /* BasicStringLengthMax must be <= INT16_MAX */
      if (XR < 0 || XR > BasicStringLengthMax)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
      break;
   case P1POS:
      /* BasicStringLengthMax must be <= INT16_MAX */
      if (XR < 1 || XR > BasicStringLengthMax)
      {
         /* ERROR */
         return -1;
      }
      else
      {
         /* OK */
         return 0;
      }
      break;
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

int
StringLengthCheck(unsigned long ParameterTests, int s)
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
      {
         /* INTERNAL ERROR */
         return -1;
      }
      break;
   case P1ANY:
      {
         /* OK */
         return 0;
      }
      break;
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
      {
         /* ERROR */
         return -1;
      }
      break;
   }
   /* ERROR */
   return -1;
}

void
FunctionDefinitionCheck(struct bwb_function * f)
{
   /* function definition check -- look for obvious errors */
   char            bwb_ebuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);


   /* sanity check */
   if (f->ParameterCount == PNONE)
   {
      /* function has NO parameters */
      if (f->ParameterTypes == PNONE)
      {
         /* OK */
      }
      else
      {
         /* oops */
         sprintf(bwb_ebuf, "in fnc_init(): invalid ParameterTypes <%s>\n", f->Name);
         prn_xprintf(bwb_ebuf);
      }
      if (f->ParameterTests == PNONE)
      {
         /* OK */
      }
      else
      {
         /* oops */
         sprintf(bwb_ebuf, "in fnc_init(): invalid ParameterTests <%s>\n", f->Name);
         prn_xprintf(bwb_ebuf);
      }
   }
   else
   {
      /* function HAS parameters */
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
               sprintf(bwb_ebuf, "in fnc_init(): invalid ParameterTests <%s> parameter %d\n", f->Name, i + 1);
               prn_xprintf(bwb_ebuf);
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
               sprintf(bwb_ebuf, "in fnc_init(): invalid ParameterTests <%s> parameter %d\n", f->Name, i + 1);
               prn_xprintf(bwb_ebuf);
            }
         }
         ParameterTests = ParameterTests >> 4;
      }
      if (ParameterTests != 0)
      {
         /* oops */
         sprintf(bwb_ebuf, "in fnc_init(): invalid ParameterTests <%s> parameter %d\n", f->Name, i + 1);
         prn_xprintf(bwb_ebuf);
      }
   }
}

void
FunctionUniqueID(struct bwb_function * f, char *UniqueID)
{
   /* generate the function's UniqueID */
   /* manual fixup required for duplicates */
   char           *D;   /* location of $ */
   char            NumVar;
   char            StrVar;

   bwx_DEBUG(__FUNCTION__);


   NumVar = 'X';
   StrVar = 'A';

   /* F_  */
   strcpy(UniqueID, "F_");
   /* NAME  */
   strcat(UniqueID, f->Name);
   D = strchr(UniqueID, BasicStringSuffix);
   if (D == NULL)
   {
      /* NOT FOUND */
   }
   else
   {
      /* FOUND */
      *D = 0;     /* remove it */
   }
   /* PARAMETERS */
   if (f->ParameterCount == PNONE)
   {
      /* function has NO parameters */
   }
   else
   {
      /* function HAS parameters */
      int             i;
      unsigned int    ParameterTypes;
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
         strcat(UniqueID, VarName);
         ParameterTypes = ParameterTypes >> 1;
         /* ParameterTests = ParameterTests >> 4; */
      }
   }
   /* RETURN TYPE */
   switch (f->ReturnType)
   {
   case STRING:
      strcat(UniqueID, "_S");
      break;
   case NUMBER:
      strcat(UniqueID, "_N");
      break;
   default:
      strcat(UniqueID, "_INTERNAL ERROR");
      break;
   }
   /* fixup illegal characters,  "DEF FN" "BLOAD:", "CLOAD*" */
   {
      char           *P;
      P = UniqueID;
      while (*P)
      {
         if (isalnum(*P))
         {
            /* OK */
         }
         else
         {
            /* FIXUP */
            *P = '_';
         }
         P++;
      }
   }
}


void
FunctionSyntax(struct bwb_function * f, char *Syntax)
{
   /* generate the function's Syntax */
   char            NumVar;
   char            StrVar;

   bwx_DEBUG(__FUNCTION__);


   NumVar = 'X';
   StrVar = 'A';

   /* RETURN TYPE */
   switch (f->ReturnType)
   {
   case STRING:
      strcpy(Syntax, "S$ = ");
      break;
   case NUMBER:
      strcpy(Syntax, "N  = ");
      break;
   default:
      strcpy(Syntax, "INTERNAL ERROR = ");
      break;
   }
   /* NAME  */
   strcat(Syntax, f->Name);
   /* PARAMETERS */
   if (f->ParameterCount == PNONE)
   {
      /* function has NO parameters */
   }
   else
   {
      /* function HAS parameters */
      int             i;
      unsigned int    ParameterTypes;
      /* unsigned long ParameterTests; */
      ParameterTypes = f->ParameterTypes;
      /* ParameterTests = f->ParameterTests; */

      switch (f->ReturnType)
      {
      case STRING:
         strcat(Syntax, "( ");
         break;
      case NUMBER:
         strcat(Syntax, "( ");
         break;
      default:
         strcat(Syntax, "INTERNAL ERROR( ");
         break;
      }

      for (i = 0; i < f->ParameterCount; i++)
      {
         char            VarName[BasicNameLengthMax + 1];
         /* sanity check this parameter */
         /* unsigned long thischeck; thischeck =
          * ParameterTests & 0x0000000F; */
         if (i > 0)
         {
            strcat(Syntax, ", ");
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
         strcat(Syntax, VarName);
         ParameterTypes = ParameterTypes >> 1;
         /* ParameterTests = ParameterTests >> 4; */
      }
      switch (f->ReturnType)
      {
      case STRING:
         strcat(Syntax, " )");
         break;
      case NUMBER:
         strcat(Syntax, " )");
         break;
      default:
         strcat(Syntax, " INTERNAL ERROR)");
         break;
      }
   }
}

void
DumpAllFuctionUniqueID(void)
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

void
DumpAllFunctionSwitch(void)
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

void
DumpAllFuctionTableDefinitions(void)
{
   /* generate bwd_fun.c */
   int             n;

   bwx_DEBUG(__FUNCTION__);


   prn_lprintf("/* FUNCTION TABLE */\n\n");
   prn_lprintf("#include \"bwbasic.h\"\n\n");
   prn_lprintf("struct bwb_function bwb_prefuncs[ NUM_FUNCTIONS ] =\n");
   prn_lprintf("{\n");
   for (n = 0; n < NUM_FUNCTIONS; n++)
   {
      int             i;
      int             j;
      char            tbuf[BasicStringLengthMax + 1];
      char            UniqueID[BasicStringLengthMax + 1];
      char            Syntax[BasicStringLengthMax + 1];
      struct bwb_function *f;

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
      switch (f->ReturnType)
      {
      case STRING:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "STRING");
         break;
      case NUMBER:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "NUMBER");
         break;
      default:
         sprintf(tbuf, "  %s, /* ReturnType */\n", "INTERNAL ERROR");
         break;
      }
      prn_lprintf(tbuf);
      sprintf(tbuf, "  %d, /* ParameterCount */\n", f->ParameterCount);
      prn_lprintf(tbuf);
      if (f->ParameterCount == 0)
      {
         sprintf(tbuf, "  %s, /* ParameterTypes */\n", "PNONE");
         prn_lprintf(tbuf);
         sprintf(tbuf, "  %s, /* ParameterTests */\n", "PNONE");
         prn_lprintf(tbuf);
      }
      else
      {
         strcpy(tbuf, "  ");
         for (i = 0; i < f->ParameterCount; i++)
         {
            int             ParameterTypes;
            ParameterTypes = f->ParameterTypes >> i;
            ParameterTypes &= 0x1;
            if (i > 0)
            {
               strcat(tbuf, " | ");
            }
            if (ParameterTypes)
            {
               sprintf(strchr(tbuf, 0), "P%dSTR", i + 1);
            }
            else
            {
               sprintf(strchr(tbuf, 0), "P%dNUM", i + 1);
            }
         }
         strcat(tbuf, ", /* ParameterTypes */\n");
         prn_lprintf(tbuf);


         strcpy(tbuf, "  ");
         for (i = 0; i < f->ParameterCount; i++)
         {
            unsigned int    ParameterTests;
            ParameterTests = f->ParameterTests >> (i * 4);
            ParameterTests &= 0xF;

            if (i > 0)
            {
               strcat(tbuf, " | ");
            }
            sprintf(strchr(tbuf, 0), ParameterRangeID[ParameterTests], i + 1);
         }
         strcat(tbuf, ", /* ParameterTests */\n");
         prn_lprintf(tbuf);
      }
      sprintf(tbuf, "  %s, /* NextPointer */\n", "NULL");
      prn_lprintf(tbuf);
      strcpy(tbuf, "  ");
      j = 0;
      for (i = 0; i < NUM_VERSIONS; i++)
      {
         if (f->OptionVersionBitmask & bwb_vertable[i].OptionVersionBitmask)
         {
            if (j > 0)
            {
               strcat(tbuf, " | ");
            }
            strcat(tbuf, bwb_vertable[i].ID);
            j++;
         }
      }
      strcat(tbuf, " /* OptionVersionBitmask */\n");
      prn_lprintf(tbuf);
      prn_lprintf("},\n");
   }
   prn_lprintf("};\n\n");
}




void
DumpOneFunctionSyntax(int n, int lp)
{
   char            tbuf[BasicStringLengthMax + 1];
   struct bwb_function *f;

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
   if (f->ParameterCount == PNONE)
   {
      /* function has NO parameters */
   }
   else
   {
      /* function HAS parameters */
      int             i;
      unsigned int    ParameterTypes;
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
            StrVar++;
         }
         else
         {
            /* NUMBER */
            sprintf(tbuf, NumberVariableRange[thischeck], NumVar);
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
      if (lp)
      {
         prn_lprintf(tbuf);
      }
      else
      {
         prn_xprintf(tbuf);
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

void
DumpAllFuctionSyntax(void)
{
   /* for the C maintainer */
   int             n;

   bwx_DEBUG(__FUNCTION__);

   for (n = 0; n < NUM_FUNCTIONS; n++)
   {
      prn_lprintf("------------------------------------------------------------\n");
      DumpOneFunctionSyntax(n, TRUE);
   }
   prn_lprintf("------------------------------------------------------------\n");
}

void
DumpAllFunctionHtmlTable(void)
{
   /* generate bwd_cmd.htm */

   int             i;
   int             j;

   bwx_DEBUG(__FUNCTION__);


   /* LEGEND */
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
      prn_lprintf(bwb_prefuncs[i].Name);
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
   prn_lprintf("\n");
}

struct bwb_line *
bwb_DEF_SUB(struct bwb_line * l)
{
   /* user is executing a function, such as 100 COS X, as though it were
    * a command */
   /* this applies to both intrinsic functions and user defined
    * functions and subroutines */
   /* no special parsing is required, just add () around the parameters */

   char            Buffer[BasicStringLengthMax + 1];
   int             i;
   char           *C;
   char           *F;

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
   if (*C == '\0' || *C == OptionCommentChar)
   {
      bwb_error("bwb_DEF_SUB, INTERNAL null");
      return bwb_zline(l);
   }
   if (!isalpha(*C))
   {
      bwb_error("bwb_DEF_SUB, INTERNAL isalpha");
      return bwb_zline(l);
   }
   if (strlen(C) + 2 > BasicStringLengthMax)
   {
      bwb_error("Ibwb_DEF_SUB, NTERNAL strlen");
      return bwb_zline(l);
   }
   /* OK */
   while (isalpha(*C))
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
   /* left paren */
   *F = '(';
   F++;
   while (*C != '\0')
   {
      /* copy parameters */
      if( *C == '"' )
      {
          /* quoted string */
          {
              /* leading quote */
              *F = *C;
              F++;
              C++;
          }
          while( *C != '\0' && *C != '"' )
          {
              /* copy inclosed string */
              *F = *C;
              F++;
              C++;
          }
          if( *C == '"' )
          {
              /* trailing quote */
              *F = *C;
              F++;
              C++;
          }
      }
      else
      if( *C == OptionCommentChar )
      {
          /* comment */
          while( *C != '\0' )
          {
              /* skip to end of line */
              C++;
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
   /* right paren */
   *F = ')';
   F++;
   /* terminate */
   *F = '\0';
   /* Call the expression interpreter to evaluate the function */

   /* prn_xprintf(Buffer); */
   bwb_exp(Buffer, FALSE, &i);
   if (ERROR_PENDING)
   {
      /* prn_xprintf("oops"); */
   }
   adv_eos(l->buffer, &(l->position));
   return bwb_zline(l);
}



/***************************************************************
  
        FUNCTION:       bwb_zline()
  
        DESCRIPTION: This function is called at the exit from
            Bywater BASIC command functions.  It returns
            a pointer to the current position in the current line.
  
***************************************************************/

struct bwb_line *
bwb_zline(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   /* skip trailing spaces */
   while (l->buffer[l->position] == ' ')
      l->position++;
   /* skip trailing comment */
   if (l->buffer[l->position] == OptionCommentChar)
   {
      /* COMMENT */
      while (l->buffer[l->position] != 0)
         l->position++;
   }
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
struct bwb_line *
bwb_FNCS(struct bwb_line * l)
{

   register int    n;
   int             t;

   bwx_DEBUG(__FUNCTION__);

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


struct bwb_line *
bwb_MAINTAINER(struct bwb_line * l)
{

   int             ShowHelp;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   ShowHelp = TRUE;
   adv_element(l->buffer, &(l->position), tbuf);

   if (strcasecmp(tbuf, "CMDS") == 0)
   {

      adv_element(l->buffer, &(l->position), tbuf);

      if (strcasecmp(tbuf, "HTML") == 0)
      {
         DumpAllCommandHtmlTable();
         ShowHelp = FALSE;
      }
      else
      if (strcasecmp(tbuf, "ID") == 0)
      {
         DumpAllCommandUniqueID();
         ShowHelp = FALSE;
      }
      else
      if (strcasecmp(tbuf, "MANUAL") == 0)
      {
         DumpAllCommandSyntax();
         ShowHelp = FALSE;
      }
      else
      if (strcasecmp(tbuf, "SWITCH") == 0)
      {
         DumpAllCommandSwitchStatement();
         ShowHelp = FALSE;
      }
      else
      if (strcasecmp(tbuf, "TABLE") == 0)
      {
         DumpAllCommandTableDefinitions();
         ShowHelp = FALSE;
      }
   }
   else
   if (strcasecmp(tbuf, "FNCS") == 0)
   {
      adv_element(l->buffer, &(l->position), tbuf);

      if (strcasecmp(tbuf, "HTML") == 0)
      {
         DumpAllFunctionHtmlTable();
         ShowHelp = FALSE;
      }
      else
      if (strcasecmp(tbuf, "ID") == 0)
      {
         DumpAllFuctionUniqueID();
         ShowHelp = FALSE;
      }
      else
      if (strcasecmp(tbuf, "MANUAL") == 0)
      {
         DumpAllFuctionSyntax();
         ShowHelp = FALSE;
      }
      else
      if (strcasecmp(tbuf, "SWITCH") == 0)
      {
         DumpAllFunctionSwitch();
         ShowHelp = FALSE;
      }
      else
      if (strcasecmp(tbuf, "TABLE") == 0)
      {
         DumpAllFuctionTableDefinitions();
         ShowHelp = FALSE;
      }
   }
   else
   if (strcasecmp(tbuf, "DEBUG") == 0)
   {
      adv_element(l->buffer, &(l->position), tbuf);
      if (strcasecmp(tbuf, "ON") == 0)
      {
         MaintainerDebugOn = TRUE;
         ShowHelp = FALSE;
      }
      else
      if (strcasecmp(tbuf, "OFF") == 0)
      {
         MaintainerDebugOn = FALSE;
         ShowHelp = FALSE;
      }
   }
   else
   if (strcasecmp(tbuf, "STACK") == 0)
   {
      /* * dump the current execution stack, * Leftmost is the
       * bottom, * Rigthmost is the top. */
      int             i;
      for (i = 0; i <= CURTASK exsc; i++)
      {
         struct bwb_line *l;

         l = CURTASK excs[i].line;
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

int
fnc_init(int task)
{
   register int    n;
   struct bwb_function *f;

   bwx_DEBUG(__FUNCTION__);

   strcpy(LOCALTASK fnc_start.Name, "FNC_START");
   LOCALTASK       fnc_start.ReturnType = 'X';
   strcpy(LOCALTASK fnc_end.Name, "FNC_END");
   LOCALTASK       fnc_end.ReturnType = 'x';
   LOCALTASK       fnc_end.next = &LOCALTASK fnc_end;


   f = &LOCALTASK fnc_start;

   /* now go through each of the preestablished functions and set up
    * links between them; from this point the program address the
    * functions only as a linked list (not as an array) */

   for (n = 0; n < NUM_FUNCTIONS; ++n)
   {
      f->next = &(bwb_prefuncs[n]);
      f = f->next;
      FunctionDefinitionCheck(f);
   }

   /* link the last pointer to the end; this completes the list */

   f->next = &LOCALTASK fnc_end;

   return TRUE;
}





void
fnc_add_deffn(struct fslte * F)
{
   struct bwb_function *f;
   struct bwb_function *n;

   bwx_DEBUG(__FUNCTION__);

   /* get memory for fslt structure */

   if ((f = CALLOC(1, sizeof(struct bwb_function), "fnc_add_deffn")) == NULL)
   {
      bwb_error("in fnc_add_deffn(): failed to get memory for bwb_function structure");
      return;
   }
   strcpy(f->Name, F->name);
   if (strchr(F->name, BasicStringSuffix) != NULL)
   {
      f->ReturnType = STRING;
   }
   else
   {
      f->ReturnType = NUMBER;
   }
   f->ReturnType |= (F->line->cmdnum & 0x00FF) << 8;  /* UniqueID is a line
                         * number */
   f->UniqueID = F->line->number;
   f->ParameterCount = F->ParameterCount; /* 0..32, 0xFF == VARIANT */
   f->ParameterTypes = F->ParameterTypes;
   f->OptionVersionBitmask = OptionVersion;

   /* establish linkages */

   n = CURTASK fnc_start.next;
   CURTASK         fnc_start.next = f;
   f->next = n;

}




static struct fslte *
fslt_find_by_line(int LineNumber)
{
   struct fslte   *f;

   bwx_DEBUG(__FUNCTION__);

   /* SEARCH */

   for (f = CURTASK fslt_start.next; f != &CURTASK fslt_end; f = f->next)
   {
      if (f->line->number == LineNumber)
      {
         /* FOUND */
         return f;
      }
   }

   /* NOT FOUND */

   return NULL;

}



/* 
The generic handler for user defined functions.  
When called by exp_function(), f->id will be set to the line number of a specific DEF USR.
*/
struct bwb_variable *
fnc_deffn(int argc, struct bwb_variable * argv, int unique_id)
{
   struct fslte   *f;
   struct bwb_variable *v;
   struct bwb_variable *argn;
   int             i;
   struct bwb_line *call_line;
   int             save_elevel;

   bwx_DEBUG(__FUNCTION__);

   /* initialize the variable if necessary */

   f = fslt_find_by_line(unique_id);

   /* these errors should not occur */
   if (f == NULL)
   {
      sprintf(bwb_ebuf, "INTERNAL ERROR, f == NULL");
      bwb_error(bwb_ebuf);
      return NULL;
   }
   if (argv == NULL)
   {
      sprintf(bwb_ebuf, "INTERNAL ERROR, argv == NULL");
      bwb_error(bwb_ebuf);
      return NULL;
   }
   if (f->ParameterCount == 0xFF)
   {
      /* VARIANT */
   }
   else
   if (argc != f->ParameterCount)
   {
      sprintf(bwb_ebuf, "INTERNAL ERROR, argc != f->ParameterCount");
      bwb_error(bwb_ebuf);
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
            sprintf(bwb_ebuf, "INTERNAL ERROR, v == NULL");
            bwb_error(bwb_ebuf);
            return NULL;
         }
         if (argn == NULL)
         {
            sprintf(bwb_ebuf, "INTERNAL ERROR, argn == NULL");
            bwb_error(bwb_ebuf);
            return NULL;
         }
         if (v->type != argn->type)
         {
            sprintf(bwb_ebuf, "INTERNAL ERROR, v->type != argn->type");
            bwb_error(bwb_ebuf);
            return NULL;
         }
         /* NEW ... */
         if (strlen(v->name) > 0)
         {
            /* FIXME: this is a hack - the variable type
             * should be determined when the variable is
             * created */
            char           *E;
            char            e;
            int             IsError;
            IsError = 0;

            E = strchr(v->name, '\0');
            E--;
            e = *E;
            /* sprintf( bwb_ebuf, "LAST CHAR is %c, TYPE
             * is %c", e, v->type ); puts( bwb_ebuf ); */
            /* so the following code is easier to read
             * and maintain */
#define PARAM_NUMBER   *argn->memnum
#define PARAM_LENGTH    argn->memstr->length
#define PARAM_BUFFER    argn->memstr->sbuffer
            /* check actual values versus formal
             * parameter type */
            switch (e)
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
            }
            if (IsError != 0)
            {
               sprintf(bwb_ebuf, "ILLEGAL FUUNCTION CALL: %s(%s)", f->name, v->name);
               bwb_Warning_InvalidParameter(bwb_ebuf);
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
      adv_ws(call_line->buffer, &call_line->position);
      if (call_line->buffer[call_line->position] != '=')
      {
         sprintf(bwb_ebuf, "*** INTERNAL ERROR #%d (%s) ***", 5, "call_line->buffer[ ] != '='");
         bwb_error(bwb_ebuf);
         return argv;
      }
      call_line->position++;  /* NOTE - we skip past the '=' sign */
   }
/* PUSH STACK */
   save_elevel = CURTASK exsc;
   bwb_incexec();
   bwb_setexec(call_line, call_line->position, EXEC_FUNCTION);


   /* create variable chain */
   if (f->ParameterCount == 0xFF)
   {
      /* VARIANT */
   }
   else
   if (argc > 0)
   {
      struct bwb_variable *source = NULL; /* source variable */
      source = f->local_variable;
      argn = argv;
      for (i = 0; i < argc; i++)
      {
         argn = argn->next;
         /* copy the name */
         strcpy(argn->name, source->name);
         source = source->next;
      }
   }
   if (call_line->cmdnum == C_DEF)
   {
      struct exp_ese *e;

      /* the function return variable is hidden */
      CURTASK         excs[CURTASK exsc].local_variable = argv->next;
      e = bwb_exp(call_line->buffer, FALSE, &call_line->position);
      bwb_etov(argv, e);

      /* break variable chain */
      CURTASK         excs[CURTASK exsc].local_variable = NULL;


/* POP STACK */
      bwb_decexec();

   }
   else
   {
      /* the function return variable is visible */
      CURTASK         excs[CURTASK exsc].local_variable = argv;
      /* exp_isfn() uses these to determine use of the function
       * name as a variable */
      CURTASK         excs[CURTASK exsc].LoopTopLine = call_line;
      CURTASK         excs[CURTASK exsc].LoopBottomLine = call_line->OtherLine;

      /* execute until function returns */
      while (CURTASK exsc > save_elevel)
      {
         bwb_execline();
      }

   }

   if (f->ParameterCount == 0xFF)
   {
      /* VARIANT */
      f->local_variable = NULL;
   }
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

struct bwb_function *
fnc_find(char *name)
{
   struct bwb_function *f;

   bwx_DEBUG(__FUNCTION__);

   if (strlen(name) == 0)
   {
      return NULL;
   }
   for (f = CURTASK fnc_start.next; f != &CURTASK fnc_end; f = f->next)
   {
      if (OptionVersion & f->OptionVersionBitmask)
         if (strcasecmp(f->Name, name) == 0)
         {
            return f;
         }
   }

   /* search has failed: return NULL */

   return NULL;

}


struct bwb_function *
fnc_find_exact(char *name, int ParameterCount, int ParameterTypes)
{
   struct bwb_function *f;

   bwx_DEBUG(__FUNCTION__);

   for (f = CURTASK fnc_start.next; f != &CURTASK fnc_end; f = f->next)
   {
      if (OptionVersion & f->OptionVersionBitmask)
      {
         if (f->ParameterCount == ParameterCount)
         {
            if (f->ParameterTypes == ParameterTypes)
            {
               if (strcasecmp(f->Name, name) == 0)
               {
                  return f;
               }
            }
         }
      }
   }
   /* search has failed: return NULL */
   return NULL;
}



struct bwb_variable *
var_free(struct bwb_variable * v)
{
   /* // Release all the memory associated with a specific variable. //
    * This function returns NULL, so you can use it like this: //     v
    * = var_new(...); //     ... //     v = var_free( v ); */
   bwx_DEBUG(__FUNCTION__);

   if (v != NULL)
   {
      if (v->next != NULL)
      {
         /* This allows variable chains to be easily released. */
         v->next = var_free(v->next);
      }
      /* cleanup this variable */
      if (v->memnum != NULL)
      {
         FREE(v->memnum, "var_free");
         v->memnum = NULL;
      }
      if (v->memstr != NULL)
      {
         int             j;
         for (j = 0; j < v->array_units; ++j)
         {
            if (v->memstr[j].sbuffer != NULL)
            {
               FREE(v->memstr[j].sbuffer, "var_free");
               v->memstr[j].sbuffer = NULL;
            }
            v->memstr[j].rab = FALSE;
            v->memstr[j].length = 0;
         }
         FREE(v->memstr, "var_free");
         v->memstr = NULL;
      }
      if (v->array_sizes != NULL)
      {
         FREE(v->array_sizes, "var_free");
         v->array_sizes = NULL;
      }
      if (v->array_pos != NULL)
      {
         FREE(v->array_pos, "var_free");
         v->array_pos = NULL;
      }
   }
   return NULL;
}

struct bwb_function *
fnc_find_by_id(int unique_id)
{
   /* NOTE: This should ONLY search the INTRINSIC functions, not USER
    * functions  */
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

struct bwb_line *
bwb_CHANGE(struct bwb_line * l)
{
   /* * CHANGE A$ TO X * CHANGE X TO A$ */

   char            tbuf[BasicStringLengthMax + 1];
   struct bwb_variable *v = NULL;
   struct bwb_variable *A = NULL;
   struct bwb_variable *X = NULL;
   int             IsStringToArray = FALSE;


   bwx_DEBUG(__FUNCTION__);

   if (OptionFlags & OPTION_BASE_ONE)
   {
      bwb_error("CHANGE is only valid for OPTION BASE 0");
      return bwb_zline(l);
   }
   /* get 1st variable */
   adv_ws(l->buffer, &(l->position));
   bwb_getvarname(l->buffer, tbuf, &(l->position));
   v = var_find(tbuf);
   if (v == NULL)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   if (v->type == STRING)
   {
      A = v;
      IsStringToArray = TRUE;
   }
   else
   {
      /* variable MUST be an array */
      if ((v->dimensions == 1) && (v->array_sizes[0] == 1))
      {
         bwb_error(err_syntax);
         return bwb_zline(l);
      }
      X = v;
      IsStringToArray = FALSE;
   }
   /* get "TO" */
   adv_ws(l->buffer, &(l->position));
   adv_element(l->buffer, &(l->position), tbuf);
   if (strcasecmp(tbuf, "TO") != 0)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* get 2nd variable */
   adv_ws(l->buffer, &(l->position));
   bwb_getvarname(l->buffer, tbuf, &(l->position));
   v = var_find(tbuf);
   if (v == NULL)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   if (v->type == STRING)
   {
      A = v;
      if (IsStringToArray == TRUE)
      {
         bwb_error(err_syntax);
         return bwb_zline(l);
      }
   }
   else
   {
      /* variable MUST be an array */
      if ((v->dimensions == 1) && (v->array_sizes[0] == 1))
      {
         bwb_error(err_syntax);
         return bwb_zline(l);
      }
      X = v;
      if (IsStringToArray == FALSE)
      {
         bwb_error(err_syntax);
         return bwb_zline(l);
      }
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
         bwb_error("String Variable not allocated");
         return bwb_zline(l);
      }
      if (A->memstr->sbuffer == NULL)
      {
         bwb_error("String Variable not allocated");
         return bwb_zline(l);
      }
      /* variable storage is a mess, we bypass that tradition here. */
      t = 1;
      for (n = 0; n < X->dimensions; n++)
      {
         t *= X->array_sizes[n];
      }
      if (t <= A->memstr->length)
      {
         bwb_error("Numeric Array too small");
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
      int             i;
      int             n;
      char           *a;
      BasicNumberType *x;
      unsigned long   t;

      if (A->memstr == NULL)
      {
         bwb_error("String Variable not allocated");
         return bwb_zline(l);
      }
      if (A->memstr->sbuffer == NULL)
      {
         bwb_error("String Variable not allocated");
         return bwb_zline(l);
      }
      /* variable storage is a mess, we bypass that tradition here. */
      t = 1;
      for (n = 0; n < X->dimensions; n++)
      {
         t *= X->array_sizes[n];
      }
      if (t <= 1)
      {
         bwb_error("Numeric Array to small");
         return bwb_zline(l);
      }
      if (t > BasicStringLengthMax)
      {
         bwb_error("Numeric Array too large");
         return bwb_zline(l);
      }
      /* n = A->memstr->length; */
      a = A->memstr->sbuffer;
      x = X->memnum;
      n = *x;
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

/* EOF */
