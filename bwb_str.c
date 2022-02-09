/***************************************************************
  
   bwb_str.c       String-Management Routines
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



/***************************************************************
  
WARNING - WARNING - WARNING - WARNING - WARNING - WARNING
  
BASIC allows embedded NUL (0) characters.  C str*() does not. 
 
ALL the bstring code should use mem*() and ->length, but most does not.
ALL the bstring code should prevent string overflow, but most does not.
  
  
***************************************************************/




#include "bwbasic.h"





/***************************************************************
  
        FUNCTION:       str_btob()
  
        DESCRIPTION:    This C function assigns a bwBASIC string
         structure to another bwBASIC string
         structure.
  
***************************************************************/

int
str_btob(bstring * d, bstring * s)
{
   bwx_DEBUG(__FUNCTION__);

   /* get memory for new buffer */
   /* Only one of these two conditions necessitates reallocation (JBV) */
   if ((d->sbuffer == NULL) || (d->rab == TRUE))
   {
      if ((d->sbuffer = (char *) CALLOC(BasicStringLengthMax + 1, 1, "str_btob")) == NULL)
      {
         bwb_error("in str_btob(): failed to get memory for new buffer");
         return FALSE;
      }
   }
   /* write the b string to the b string */
   if (s->length > BasicStringLengthMax)
   {
      /* INTERNAL ERROR, so truncate */
      s->length = BasicStringLengthMax;
   }
   d->length = s->length;
   memcpy(d->sbuffer, s->sbuffer, d->length);
   d->sbuffer[d->length] = '\0';
   d->rab = (char) FALSE;  /* JBV */


   return TRUE;

}

/***************************************************************
  
        FUNCTION:       str_ctob()
  
        DESCRIPTION:    This C function assigns a null-terminated
         C string to a bwBASIC string structure.
  
***************************************************************/

int
str_ctob(bstring * s, char *buffer)
{
   bwx_DEBUG(__FUNCTION__);


   /* get memory for new buffer */


   /* Only one of these two conditions necessitates reallocation (JBV) */
   if ((s->sbuffer == NULL) || (s->rab == TRUE))
   {
      if ((s->sbuffer = (char *) CALLOC(BasicStringLengthMax + 1, 1, "str_ctob")) == NULL)
      {
         bwb_error("in str_ctob(): failed to get memory for new buffer");
         return FALSE;
      }
   }
   /* write the c string to the b string */
   s->length = strlen(buffer);
   if (s->length > BasicStringLengthMax)
   {
      /* INTERNAL ERROR, so truncate */
      s->length = BasicStringLengthMax;
   }
   memcpy(s->sbuffer, buffer, s->length);
   s->sbuffer[s->length] = '\0';
   s->rab = (char) FALSE;  /* JBV */


   return TRUE;

}

/***************************************************************
  
        FUNCTION:       str_btoc()
  
        DESCRIPTION:    This C function assigns a null-terminated
         C string to a bwBASIC string structure.
  
***************************************************************/

int
str_btoc(char *buffer, bstring * s)
{

   bwx_DEBUG(__FUNCTION__);

   /* write the b string to the c string */
   if (s->length > BasicStringLengthMax)
   {
      /* INTERNAL ERROR, so truncate */
      s->length = BasicStringLengthMax;
   }
   memcpy(buffer, s->sbuffer, s->length);
   buffer[s->length] = '\0';

   return TRUE;

}

/***************************************************************
  
        FUNCTION:       str_cat()
  
        DESCRIPTION:    This C function performs the equivalent
         of the C strcat() function, using BASIC
         strings.
  
***************************************************************/

bstring        *
str_cat(bstring * a, bstring * b)
{
   int             i;

   bwx_DEBUG(__FUNCTION__);

   if (a->length > BasicStringLengthMax)
   {
      /* INTERNAL ERROR, so truncate */
      a->length = BasicStringLengthMax;
   }
   if (b->length > BasicStringLengthMax)
   {
      /* INTERNAL ERROR, so truncate */
      b->length = BasicStringLengthMax;
   }
   i = b->length;
   if (a->length + i > BasicStringLengthMax)
   {
      /* INTERNAL ERROR, so truncate */
      i = BasicStringLengthMax - a->length;
   }
   if (i > 0)
   {
      char           *t;

      t = a->sbuffer;
      t += a->length;
      memcpy(t, b->sbuffer, i);
      a->length += i;
      a->sbuffer[a->length] = '\0';
   }
   return a;
}

/***************************************************************
  
        FUNCTION:       str_cmp()
  
   DESCRIPTION:    This C function performs the equivalent
         of the C strcmp() function, using BASIC
         strings.
  
***************************************************************/

int
str_cmp(bstring * a, bstring * b)
{
   bwx_DEBUG(__FUNCTION__);

   if (a->length > BasicStringLengthMax)
   {
      /* INTERNAL ERROR, so truncate */
      a->length = BasicStringLengthMax;
   }
   if (b->length > BasicStringLengthMax)
   {
      /* INTERNAL ERROR, so truncate */
      b->length = BasicStringLengthMax;
   }
   if (a->sbuffer == NULL)
   {
      if (b->sbuffer == NULL)
      {
         return 0;
      }
      if (b->length == 0)
      {
         return 0;
      }
      return 1;
   }
   else
   {
      a->sbuffer[a->length] = '\0';
   }

   if (b->sbuffer == NULL)
   {
      if (a->sbuffer == NULL)
      {
         return 0;
      }
      if (a->length == 0)
      {
         return 0;
      }
      return -1;
   }
   else
   {
      b->sbuffer[b->length] = '\0';
   }

   if (OptionFlags & OPTION_COMPARE_TEXT)
   {
      /* case insensitive */
      return strcasecmp(a->sbuffer, b->sbuffer);
   }
   else
   {
      /* case sensitive */
      return strcmp(a->sbuffer, b->sbuffer);
   }
}


/* EOF */
