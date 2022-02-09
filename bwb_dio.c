/***************************************************************
  
        bwb_dio.c       Device Input/Output Routines
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



static struct bwb_line *
dio_lrset(struct bwb_line * l, int rset);


/***************************************************************
  
        FUNCTION:       bwb_open()
  
   DESCRIPTION:    This function implements the BASIC OPEN
         command to open a stream for device input/output.
  
        SYNTAX: 1. OPEN "I"|"O"|"R"|"A", [#]n, filename [,rlen]
                2. OPEN filename [FOR INPUT|OUTPUT|APPEND|RANDOM|BINARY] AS [#]n [LEN=n]
  
***************************************************************/

struct bwb_line *
bwb_OPEN(struct bwb_line * l)
{
   /* OPEN filename [FOR mode] AS filenumber [LEN reclen] */
   char            filename[BasicStringLengthMax + 1];
   char            mode[BasicStringLengthMax + 1];
   char            filenumber[BasicStringLengthMax + 1];
   char            reclen[BasicStringLengthMax + 1];
   char            OutputBuffer[BasicStringLengthMax + 1];
   int             p;

   bwx_DEBUG(__FUNCTION__);

   /* OPEN filename [FOR mode] AS */
   if (GetKeyword(l, filename, " FOR "))
   {
      /* OPEN filename FOR */
      if (GetKeyword(l, mode, " AS "))
      {
         /* FOR mode AS */
      }
      else
      {
         bwb_error("syntax error");
      }
   }
   else
   if (GetKeyword(l, filename, " AS "))
   {
      /* OPEN filename AS */
      strcpy(mode, "BINARY"); /* default for structured OPEN */
   }
   else
   {
      bwb_error("syntax error");
   }

   /* AS filenumber [LEN reclen] */
   if (GetKeyword(l, filenumber, " LEN "))
   {
      /* AS filenumber LEN reclen */
      GetRestOfLine(l, reclen);
   }
   else
   {
      /* AS filenumber */
      GetRestOfLine(l, filenumber);
      strcpy(reclen, "128");  /* default for structured OPEN */
   }

   OutputBuffer[0] = '\0';
   strcat(OutputBuffer, "OPEN(");
   strcat(OutputBuffer, "\"");
   strcat(OutputBuffer, mode);
   strcat(OutputBuffer, "\"");
   strcat(OutputBuffer, ",");
   strcat(OutputBuffer, filenumber);
   strcat(OutputBuffer, ",");
   strcat(OutputBuffer, filename);
   strcat(OutputBuffer, ",");
   strcat(OutputBuffer, reclen);
   strcat(OutputBuffer, ")");


   p = 0;
   bwb_exp(OutputBuffer, FALSE, &p);
   if (ERROR_PENDING)
   {
      /* oops */
   }
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_name()
  
   DESCRIPTION:    This function implements the BASIC NAME
         command to rename a disk file.
  
   SYNTAX:         NAME old_filename AS new_filename
  
***************************************************************/


struct bwb_line *
bwb_NAME(struct bwb_line * l)
{
   int             r;
   int             position;
   struct exp_ese *e;
   char            atbuf[BasicStringLengthMax + 1];
   char            btbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   /* get the first argument in atbuf */

   adv_element(l->buffer, &(l->position), atbuf);

   /* interpret the first argument */

   position = 0;
   e = bwb_exp(atbuf, FALSE, &position);
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (e->type != STRING)
   {
      bwb_error(err_argstr);
      return bwb_zline(l);
   }
   /* this argument must be copied back to atbuf, else the next call to
    * bwb_exp() will overwrite the structure to which e refers */

   str_btoc(atbuf, &(e->sval));


   /* get the second argument in btbuf */

   adv_element(l->buffer, &(l->position), btbuf);


   if (strcasecmp(btbuf, "AS") != 0)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   /* get the third argument in btbuf */

   adv_element(l->buffer, &(l->position), btbuf);

   /* interpret the third argument */

   position = 0;
   e = bwb_exp(btbuf, FALSE, &position);
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (e->type != STRING)
   {
      bwb_error(err_argstr);
      return bwb_zline(l);
   }
   str_btoc(btbuf, &(e->sval));


   /* try to rename the file */

   r = rename(atbuf, btbuf);

   /* detect error */

   if (r != 0)
   {
      bwb_error(err_opsys);
   }
   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       bwb_field()
  
        DESCRIPTION: This C function implements the BASIC
         FIELD command.
  
***************************************************************/

struct bwb_line *
bwb_FIELD(struct bwb_line * l)
{
   int             dev_number;
   struct exp_ese *e;
   int             current_pos;
   char            atbuf[BasicStringLengthMax + 1];
   int             pos;

   bwx_DEBUG(__FUNCTION__);

   current_pos = 0;

   /* first read device number */

   adv_ws(l->buffer, &(l->position));
   if (l->buffer[l->position] == BasicFileNumberPrefix)
   {
      ++(l->position);
      adv_ws(l->buffer, &(l->position));
   }
   adv_element(l->buffer, &(l->position), atbuf);

   pos = 0;
   e = bwb_exp(atbuf, FALSE, &pos);
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   if (e->type != NUMBER)
   {
      bwb_error("in bwb_field(): Number was expected for device number");
      return bwb_zline(l);
   }
   dev_number = exp_getival(e);


   if (dev_number < 1 || dev_number > BasicFileNumberMax)
   {
      bwb_error("in bwb_field(): Requested device number is not VALID.");
      return bwb_zline(l);
   }
   /* be sure that the requested device is open */

   if (dev_table[dev_number].mode != DEVMODE_RANDOM)
   {
      bwb_error("in bwb_field(): Requested device number is not RANDOM.");
      return bwb_zline(l);
   }
   /* loop to read variables */

   do
   {
      int             length;
      struct bwb_variable *v;
      bstring        *b;


      /* read the comma and advance beyond it */

      adv_ws(l->buffer, &(l->position));
      if (l->buffer[l->position] == ',')
      {
         ++(l->position);
      }
      /* first find the size of the field */

      adv_element(l->buffer, &(l->position), atbuf);  /* get element */

      pos = 0;
      e = bwb_exp(atbuf, FALSE, &pos);
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      if (e->type != NUMBER)
      {
         bwb_error("in bwb_field(): number value for field size not found");
         return bwb_zline(l);
      }
      length = exp_getival(e);


      /* read the AS */

      adv_element(l->buffer, &(l->position), atbuf);  /* get element */


      if (strcasecmp(atbuf, "AS") != 0)
      {
         bwb_error("in bwb_field(): AS statement not found");
         return bwb_zline(l);
      }
      /* read the string variable name */
#if 0
      adv_element(l->buffer, &(l->position), atbuf);  /* get element */
#endif
      bwb_getvarname(l->buffer, atbuf, &(l->position));
      v = var_find(atbuf);

      if (v->type != STRING)
      {
         bwb_error("in bwb_field(): string variable name not found");
         return bwb_zline(l);
      }
      /* check for overflow of record length */

      if ((current_pos + length) > dev_table[dev_number].width)
      {
         bwb_error("in bwb_field(): record length exceeded");
         return bwb_zline(l);
      }
      /* set buffer */

      b = var_findsval(v, v->array_pos);
      b->sbuffer = dev_table[dev_number].buffer + current_pos;
      b->length = (unsigned int) length;  /* Was unsigned char
                      * (JBV 9/4/97) */
      b->rab = TRUE;

      current_pos += length;


      /* eat up any remaining whitespace */

      adv_ws(l->buffer, &(l->position));

   }

   while (l->buffer[l->position] == ',');

   /* return */

   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       bwb_lset()
  
        DESCRIPTION: This C function implements the BASIC
         LSET command.
  
   SYNTAX:     LSET string-variable$ = expression
  
***************************************************************/

struct bwb_line *
bwb_LSET(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   return dio_lrset(l, FALSE);
}

/***************************************************************
  
        FUNCTION:       bwb_rset()
  
        DESCRIPTION: This C function implements the BASIC
         RSET command.
  
   SYNTAX:     RSET string-variable$ = expression
  
***************************************************************/

struct bwb_line *
bwb_RSET(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);
   return dio_lrset(l, TRUE);
}

/***************************************************************
  
        FUNCTION:       dio_lrset()
  
        DESCRIPTION: This C function implements the BASIC
         RSET and LSET commands.
  
***************************************************************/

static struct bwb_line *
dio_lrset(struct bwb_line * l, int rset)
{
   char            varname[BasicNameLengthMax + 1];
   bstring        *d, *s;
   int            *pp;
   int             n_params;
   int             p;
   register int    n, i;
   int             startpos;
   struct exp_ese *e;
   struct bwb_variable *v;
   int             pos;

   bwx_DEBUG(__FUNCTION__);
   /* find the variable name */

   bwb_getvarname(l->buffer, varname, &(l->position));
   v = var_find(varname);

   if (v == NULL)
   {
      sprintf(bwb_ebuf, "in dio_lrset(): failed to find variable");
      bwb_error(bwb_ebuf);
   }
   if (v->type != STRING)
   {
      sprintf(bwb_ebuf, "in dio_lrset(): assignment must be to string variable");
      bwb_error(bwb_ebuf);
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

   /* find equals sign */

   adv_ws(l->buffer, &(l->position));
   if (l->buffer[l->position] != '=')
   {
      sprintf(bwb_ebuf, "in dio_lrset(): failed to find equal sign");
      bwb_error(bwb_ebuf);
   }
   ++(l->position);
   adv_ws(l->buffer, &(l->position));

   /* read remainder of line to get value */

   e = bwb_exp(l->buffer, FALSE, &(l->position));
   if (ERROR_PENDING)
   {
      return bwb_zline(l);
   }
   s = exp_getsval(e);

   /* set starting position */

   startpos = 0;
   if (rset == TRUE)
   {
      if (s->length < d->length)
      {
         startpos = d->length - s->length;
      }
   }
   /* write characters to new position */

   i = 0;
   for (n = startpos; (i < (int) s->length) && (n < (int) d->length); ++n)
   {
      d->sbuffer[n] = s->sbuffer[i];
      ++i;
   }

   /* return */

   return bwb_zline(l);

}

/* EOF */
