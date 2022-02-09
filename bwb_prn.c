/***************************************************************
  
   bwb_prn.c       Print and Error-Handling Commands
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


/* ECMA-55, Section 14.4 */
#define ZONEWIDTH          ( SIGNIFICANT_DIGITS + EXPONENT_DIGITS + 6 )


/* Prototypes for functions visible only to this file */
static BasicFileNumberType FileNumber = CONSOLE_FILE_NUMBER;   /* console = 0, printer
                         * = 0xFF, OTHERWISE #
                         * file-number */

struct prn_fmt
{
   int             type;   /* STRING, NUMBER, SINGLE, or NUMBER */
   int             exponential;  /* TRUE = use exponential notation */
   int             right_justified; /* TRUE = right justified
                   * else left justified */
   int             width;  /* width of main section */
   int             precision; /* width after decimal point */
   int             commas; /* use commas every three steps */
   int             sign;   /* prefix sign to number */
   int             money;  /* prefix money sign to number */
   int             fill;   /* ASCII value for fill character, normally '
             * ' */
   int             minus;  /* postfix minus sign to number */
};

static struct prn_fmt *
get_prnfmt(char *buffer, int *position);
static int
xputc(char c);
static int
xxputc(char c);
static int
xxxputc(char c);
static int
bwb_xprint(struct bwb_line * l);
static int
prn_xxprintf(char *buffer);
static int      prn_getcol();
static int
prn_setcol(int NewValue);
static int      prn_getwidth();




/***************************************************************
  
      FUNCTION:       bwx_putc()
  
   DESCRIPTION:    This function outputs a single character
         to the default output device.
  
***************************************************************/

int
bwx_PRINT(char c)
{
   bwx_DEBUG(__FUNCTION__);

   return fputc(c, stdout);

}



int
prn_lprintf(char *buffer)
{
   bwx_DEBUG(__FUNCTION__);

   while (*buffer != '\0')
   {
      bwx_LPRINT(*buffer);
      buffer++;
   }
   return 0;
}

int
prn_xprintf(char *buffer)
{
   /* Catch-22: an error has occurred before the devicce table is loaded */
   int             n;
   bwx_DEBUG(__FUNCTION__);

   n = bwx_CONSOLE_WIDTH();

   if (n > 0)
   {
      int             i;
      i = 0;
      while (*buffer)
      {
         bwx_PRINT(*buffer);
         buffer++;
         if (*buffer == '\n')
         {
            i = 0;
         }
         i++;
         if (i >= n)
         {
            bwx_PRINT('\n');
            i = 0;
         }
      }
   }
   else
   {
      /* raw */
      while (*buffer)
      {
         bwx_PRINT(*buffer);
         buffer++;
      }
   }
   return 0;
}

static void
CleanNumericString(char *prnbuf, int RemoveDot)
{
   /* remove trailing zeroes */
   char           *E;
   char           *D;

   bwx_DEBUG(__FUNCTION__);

   E = strchr(prnbuf, 'E');
   if (E == NULL)
   {
      E = strchr(prnbuf, 'e');
   }
   if (E)
   {
      /* SCIENTIFIC == SCALED notation */
      /* trim leading zeroes in exponent */
      char           *F;
      char           *G;
      F = E;
      while (isalpha(*F))
      {
         F++;
      }
      while (*F == '+' || *F == '-')
      {
         /* skip sign */
         F++;
      }
      G = F;
      while (*G == '0' || *G == ' ')
      {
         /* skip leading zeroes or spaces */
         G++;
      }
      if (G > F)
      {
         strcpy(F, G);
      }
      G = NULL;   /* no longer valid */
      *E = '\0';  /* for strlen()  */
   }
   D = strchr(prnbuf, '.');
   if (D)
   {
      int             N = strlen(D);
      if (N > 1)
      {
         int             M;
         N--;
         M = N;
         while (D[N] == '0')
         {
            /* remove trailing zeroes */
            D[N] = '_';
            N--;
         }
         if (RemoveDot)
         {
            if (E)
            {
               /* SCIENTIFIC == SCALED notation */
               /* do NOT remove '.' */
            }
            else
            {
               /* NORMAL  == UNSCALED notation */
               /* remove trailing '.' */
               /* this will only occur for integer
                * values */
               while (D[N] == '.')
               {
                  /* _###. POSITIVE INTEGER */
                  /* -###. NEGATIVE INTEGER */
                  D[N] = '_';
                  N--;
               }
            }
         }
         if (N < M)
         {
            if (E)
            {
               /* SCIENTIFIC == SCALED notation */
               *E = 'E';
               E = NULL;
            }
            N++;
            /* if INTEGER, then N == 0, else N > 0 */
            M++;
            /* if SCIENTIFIC, then  *M == 'E' else *M ==
             * '\0' */
            strcpy(&(D[N]), &(D[M]));
         }
      }
   }
   if (E)
   {
      /* SCIENTIFIC == SCALED notation */
      *E = 'E';
      E = NULL;
   }
   if (prnbuf[1] == '0' && prnbuf[2] == '.')
   {
      /* _0.### POSITIVE FRACTION ==> _.### */
      /* -0.### NEGATIVE FRACTION ==> -.### */
      strcpy(&(prnbuf[1]), &(prnbuf[2]));
   }
   if (prnbuf[1] == '.' && prnbuf[2] == 'E')
   {
      /* _.E POSITIVE ZERO ==> _0 */
      /* -.E NEGATIVE ZERO ==> _0 */
      strcpy(prnbuf, " 0");
   }
}

static int
SignificantDigits(char *Buffer)
{
   int             NumDigits;
   char           *P;

   bwx_DEBUG(__FUNCTION__);


   /* determine the number of significant digits */
   NumDigits = 0;
   P = Buffer;
   while (*P)
   {
      if (isalpha(*P))
      {
         /* 'E', 'e', and so on. */
         break;
      }
      if (isdigit(*P))
      {
         NumDigits++;
      }
      P++;
   }
   return NumDigits;
}

void
BasicNumerc(BasicNumberType Input, char *Output)
{

   char           *E;

   bwx_DEBUG(__FUNCTION__);

   /* * print in scientific form first, * to determine exponent and *
    * significant digits */
   sprintf(Output, "% 1.*E", SIGNIFICANT_DIGITS - 1, Input);
   E = strchr(Output, 'E');
   if (E == NULL)
   {
      E = strchr(Output, 'e');
   }
   if (E)
   {
      /* valid */
      int             Exponent;
      int             NumDigits;
      int             DisplayDigits;
      int             zz;
      char           *F;   /* pointer to the exponent's value */
      F = E;
      while (isalpha(*F))
      {
         F++;
      }
      Exponent = atoi(F);
      CleanNumericString(Output, 0);
      NumDigits = SignificantDigits(Output);
      /* DisplayDigits = min(NumDigits,SIGNIFICANT_DIGITS) */
      DisplayDigits = NumDigits;
      if (DisplayDigits > SIGNIFICANT_DIGITS)
      {
         DisplayDigits = SIGNIFICANT_DIGITS;
      }
      /* zz = max(Exponent,DisplayDigits - Exponent - 2) */
      zz = DisplayDigits - Exponent - 2;
      if (zz < Exponent)
      {
         zz = Exponent;
      }
      if (zz >= SIGNIFICANT_DIGITS)
      {
         /* SCIENTIFIC */
         sprintf(Output, "%# 1.*E", DisplayDigits - 1, Input);
      }
      else
      if (Input == (int) Input)
      {
         /* INTEGER */
         sprintf(Output, "% *d", DisplayDigits, (int) Input);
      }
      else
      {
         /* FLOAT */
         int             N;
         /* number of digits before the '.' */
         int             M;
         /* number of digits after  the '.' */
         N = Exponent + 1;
         if (N < 0)
         {
            N = 0;
         }
         M = SIGNIFICANT_DIGITS - N;
         if (M < 0)
         {
            M = 0;
         }
         sprintf(Output, "%# *.*f", N, M, Input);
      }
      CleanNumericString(Output, 0);
   }
   else
   {
      /* ERROR, NAN, INFINITY, ETC. */
   }
   return;
}



struct bwb_line *
bwb_LPRINT(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   FileNumber = LPRINT_FILE_NUMBER;
   bwb_xprint(l);
   return bwb_zline(l);
}


/***************************************************************
  
        FUNCTION:       bwb_print()
  
        DESCRIPTION:    This function implements the BASIC PRINT
                        command.
  
   SYNTAX:     PRINT [# device-number,][USING format-string$;] expressions...
  
***************************************************************/

struct bwb_line *
bwb_QUEST(struct bwb_line * l)
{
   bwx_DEBUG(__FUNCTION__);

   return bwb_PRINT(l);
}

struct bwb_line *
bwb_PRINT(struct bwb_line * l)
{
   static int      pos;
   static char    *s_buffer;  /* small, temporary buffer */
   static int      init = FALSE;

   bwx_DEBUG(__FUNCTION__);


   /* initialize buffers if necessary */

   if (init == FALSE)
   {
      init = TRUE;

      /* Revised to CALLOC pass-thru call by JBV */
      if ((s_buffer = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_print")) == NULL)
      {
         bwb_error("in bwb_print(): failed to get memory for s_buffer");
         return bwb_zline(l);
      }
   }
   FileNumber = CONSOLE_FILE_NUMBER;


   adv_ws(l->buffer, &(l->position));

   if (l->buffer[l->position] == '@')
   {
      /* PRINT @ position, ... */
      struct exp_ese *v;
      int             pos;
      int             position;
      int             r;
      int             c;

      position = 0;
      r = 0;
      c = 0;

      ++(l->position);
      adv_ws(l->buffer, &(l->position));
      adv_element(l->buffer, &(l->position), s_buffer);
      pos = 0;
      v = bwb_exp(s_buffer, FALSE, &pos);
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      adv_ws(l->buffer, &(l->position));
      if (l->buffer[l->position] == ',')
      {
         ++(l->position);
      }
      else
      {
         bwb_error("in bwb_print(): no comma after @n");
         return bwb_zline(l);
      }

      position = exp_getival(v);
      if (position < 0 || position > 80 * 24 - 1)
      {
         bwb_error("in bwb_input(): Requested position is out of range.");
         return bwb_zline(l);
      }
      r = position / prn_getwidth();
      c = position - r * prn_getwidth();
      r++;
      c++;


      switch (OptionTerminalType)
      {
      case C_OPTION_TERMINAL_NONE:
         break;
      case C_OPTION_TERMINAL_ADM_3A:
         fprintf(stdout, "%c=%c%c", 27, r + 32, c + 32);
         break;
      case C_OPTION_TERMINAL_ANSI:
         fprintf(stdout, "%c[%d;%dH", 27, r, c);
         break;
      default:
         sprintf(bwb_ebuf, "%s IS NOT IMPLEMENTED ON THIS PLATFORM", "PRINT @");
         bwb_error(bwb_ebuf);
         break;
      }

   }
   else
   if (l->buffer[l->position] == BasicFileNumberPrefix)
   {
      /* PRINT # file, ... */
      struct exp_ese *v;
      int             UserFileNumber;


      ++(l->position);
      adv_ws(l->buffer, &(l->position));
      adv_element(l->buffer, &(l->position), s_buffer);
      pos = 0;
      v = bwb_exp(s_buffer, FALSE, &pos);
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      adv_ws(l->buffer, &(l->position));
      if (l->buffer[l->position] == ',')
      {
         ++(l->position);
      }
      else
      {
         bwb_error("in bwb_print(): no comma after #n");
         return bwb_zline(l);
      }

      UserFileNumber = exp_getival(v);
      /* check the requested device number */
      if (UserFileNumber < 0)
      {
         /* PRINTER, CASSETTE, etc.  These are all sent to
          * bwx_LPRINT */
         FileNumber = LPRINT_FILE_NUMBER;
      }
      else
      if (UserFileNumber > BasicFileNumberMax)
      {
         bwb_error("in bwb_input(): Requested device number is out of range.");
         return bwb_zline(l);
      }
      else
      {
         FileNumber = (BasicFileNumberType) UserFileNumber;
         if ((dev_table[FileNumber].mode & DEVMODE_WRITE) == 0)
         {
            bwb_error("in bwb_print(): Requested device is not open for writing");
            return bwb_zline(l);
         }
      }


   }
   bwb_xprint(l);


   if (FileNumber == CONSOLE_FILE_NUMBER)
   {
      /* FOR I = 1 TO 1000: PRINT "."; : NEXT I : PRINT */
      fflush(stdout);
   }
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_xprint()
  
   DESCRIPTION:    This function implements the PRINT
         command, utilizing a specified file our
         output device.
  
***************************************************************/
/* static unsigned char FileNumber has been assigned */
static int
bwb_xprint(struct bwb_line * l)
{
   struct exp_ese *e;
   int             loop;
   static int      p;
   static int      fs_pos;
   struct prn_fmt *format;
   static char    *format_string;
   static char    *output_string;
   static char    *element;
   static char    *prnbuf;
   static int      init = FALSE;
   register int    i, j;   /* JBV */
   int             dig_pos, dec_pos;   /* JBV */
   char            tbuf[BasicStringLengthMax + 1]; /* JBV */
   int             OutputCR;

   bwx_DEBUG(__FUNCTION__);

   /* initialize buffers if necessary */

   if (init == FALSE)
   {
      init = TRUE;

      /* Revised to CALLOC pass-thru call by JBV */
      if ((format_string = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_xprint")) == NULL)
      {
         bwb_error("in bwb_xprint(): failed to get memory for format_string");
         return FALSE;
      }
      /* Revised to CALLOC pass-thru call by JBV */
      if ((output_string = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_xprint")) == NULL)
      {
         bwb_error("in bwb_xprint(): failed to get memory for output_string");
         return FALSE;
      }
      /* Revised to CALLOC pass-thru call by JBV */
      if ((element = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_xprint")) == NULL)
      {
         bwb_error("in bwb_xprint(): failed to get memory for element buffer");
         return FALSE;
      }
      /* Revised to CALLOC pass-thru call by JBV */
      if ((prnbuf = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_xprint")) == NULL)
      {
         bwb_error("in bwb_xprint(): failed to get memory for prnbuf");
         return FALSE;
      }
   }
   /* Detect USING Here */

   fs_pos = -1;

   /* get "USING" in format_string */

   p = l->position;
   adv_element(l->buffer, &p, format_string);


   /* check to be sure */

   if (strcasecmp(format_string, "USING") == 0)
   {
      l->position = p;
      adv_ws(l->buffer, &(l->position));

      /* now get the format string in format_string */

      e = bwb_exp(l->buffer, FALSE, &(l->position));
      if (ERROR_PENDING)
      {
         return FALSE;
      }
      if (e->type == STRING)
      {

         /* copy the format string to buffer */

         str_btoc(format_string, exp_getsval(e));

         /* look for ';' after format string */

         fs_pos = 0;
         adv_ws(l->buffer, &(l->position));
         if (l->buffer[l->position] == ';')
         {
            ++l->position;
            adv_ws(l->buffer, &(l->position));
         }
         else
         {
            bwb_error("Failed to find ; after format string in PRINT USING");
            return FALSE;
         }


      }
      else
      {
         bwb_error("Failed to find format string after PRINT USING");
         return FALSE;
      }
   }
   /* if no arguments, simply print CR and return */

   adv_ws(l->buffer, &(l->position));
   switch (l->buffer[l->position])
   {
   case '\0':
      xputc('\n');
      return TRUE;
   default:
      break;
   }

   /* LOOP THROUGH PRINT ELEMENTS */

   OutputCR = TRUE;

   loop = TRUE;
   while (loop == TRUE)
   {

      /* 1980 PRINT  , , ,"A" */
      adv_ws(l->buffer, &(l->position));
      switch (l->buffer[l->position])
      {
      case '\0':  /* end of buffer */
         loop = FALSE;
         break;
      case ',':   /* tab over */
         /* Tab only if there's no format specification! (JBV) */
         /* putchar('+'); */
         if ((fs_pos == -1) || (strlen(element) == 0))
         {
            xputc('\t');
         }
         ++l->position;
         OutputCR = FALSE;
         continue;   /* while( loop == TRUE ) */
         break;
      case ';':   /* concatenate strings */
         ++l->position;
         OutputCR = FALSE;
         continue;   /* while( loop == TRUE ) */
         break;
      default:
         break;
      }
      if(l->buffer[l->position] == OptionCommentChar)
      {
         /* PRINT ' comment */
         loop = FALSE;
      }
      if (loop == FALSE)
      {
         break;   /* while( loop == TRUE ) */
      }
      OutputCR = TRUE;


      /* resolve the string */

      e = bwb_exp(l->buffer, FALSE, &(l->position));
      if (ERROR_PENDING)
      {
         return FALSE;
      }
      /* an OP_NULL probably indicates a terminating ';', but this
       * will be detected later, so we can ignore it for now */

      if (e->operation != OP_NULL)
      {
         str_btoc(element, exp_getsval(e));
      }
      else
      {
         element[0] = '\0';
      }


      /* print with format if there is one */

      if ((fs_pos > -1) && (strlen(element) > 0))
      {

         format = get_prnfmt(format_string, &fs_pos);
         if (fs_pos < 0)
         {
            /* wrap around */
            fs_pos = 0;
            format = get_prnfmt(format_string, &fs_pos);
         }
         switch (format->type)
         {
         case STRING:
#if 0
            if (e->type != STRING)
            {
               bwb_error("Type mismatch in PRINT USING");
               return FALSE;
            }
#endif
            if (format->width == -1)   /* JBV */
               sprintf(output_string, "%s", element);
            else
               sprintf(output_string, "%.*s", format->width, element);


            prn_xxprintf(output_string);  /* do not expand TAB or
                         * SPC */
            break;

         case NUMBER:
            if (e->type == STRING)
            {
               bwb_error("Type mismatch in PRINT USING");
               return FALSE;
            }
            if (format->exponential == TRUE)
            {
               /*------------------------------------------------------*/
               /* NOTE: Width and fill have no
                * effect on C exponential */
               /* format (JBV)                                         */
               /*------------------------------------------------------*/
               if (format->sign == TRUE)  /* Added by JBV */
                  sprintf(output_string, "%+e", exp_getnval(e));
               else
                  sprintf(output_string, "%e", exp_getnval(e));
            }
            else
            {
               /*---------------------------------------------------*/
               /* NOTE: Minus, commas, and money are
                * only valid for */
               /* floating point format (JBV)                       */
               /*---------------------------------------------------*/
               if (format->sign == TRUE)  /* Added by JBV */
                  sprintf(output_string, "%+*.*f",
                     format->width, format->precision, exp_getnval(e));
               else
               if (format->minus == TRUE) /* Added by JBV */
               {
                  sprintf(output_string, "%*.*f",
                     format->width, format->precision, exp_getnval(e));
                  for (i = 0; i < strlen(output_string); ++i)
                  {
                     if (output_string[i] != ' ')
                     {
                        if (output_string[i] == '-')
                        {
                           output_string[i] = ' ';
                           strcat(output_string, "-");
                        }
                        else
                           strcat(output_string, " ");
                        break;
                     }
                  }
               }
               else
                  sprintf(output_string, "%*.*f",
                     format->width, format->precision, exp_getnval(e));

               if (format->commas == TRUE)   /* Added by JBV */
               {
                  dig_pos = -1;
                  dec_pos = -1;
                  for (i = 0; i < strlen(output_string); ++i)
                  {
                     if ((isdigit(output_string[i]) != 0)
                      && (dig_pos == -1))
                        dig_pos = i;
                     if ((output_string[i] == '.')
                      && (dec_pos == -1))
                        dec_pos = i;
                     if ((dig_pos != -1) && (dec_pos != -1))
                        break;
                  }
                  if (dec_pos == -1)
                     dec_pos = strlen(output_string);
                  j = 0;
                  for (i = 0; i < strlen(output_string); ++i)
                  {
                     if (((dec_pos - i) % 3 == 0)
                         && (i > dig_pos) && (i < dec_pos))
                     {
                        tbuf[j] = ',';
                        ++j;
                        tbuf[j] = '\0';
                     }
                     tbuf[j] = output_string[i];
                     ++j;
                     tbuf[j] = '\0';
                  }
                  strcpy(output_string,
                         &tbuf[strlen(tbuf) - strlen(output_string)]);
               }
               if (format->money == TRUE) /* Added by JBV */
               {
                  for (i = 0; i < strlen(output_string); ++i)
                  {
                     if (output_string[i] != ' ')
                     {
                        if (i > 0)
                        {
                           if (isdigit(output_string[i]) == 0)
                           {
                              output_string[i - 1]
                                 = output_string[i];
                              output_string[i] = '$';
                           }
                           else
                              output_string[i - 1] = '$';
                        }
                        break;
                     }
                  }
               }
            }

            if (format->fill == '*')   /* Added by JBV */
               for (i = 0; i < strlen(output_string); ++i)
               {
                  if (output_string[i] != ' ')
                     break;
                  output_string[i] = '*';
               }


            prn_xxprintf(output_string);  /* do not expand TAB or
                         * SPC */
            break;

         default:
            sprintf(bwb_ebuf, "in bwb_xprint(): get_prnfmt() returns unknown type <%d>",
               format->type);
            bwb_error(bwb_ebuf);
            return FALSE;
            break;
         }


      }
      /* not a format string: use defaults */

      else
      if (strlen(element) > 0)
      {


         switch (e->type)
         {
         case STRING:
            prn_iprintf(element);
            break;
         default:
            /* [space]number[space]    POSITIVE or ZERO
             * [minus]number[space]   NEGATIVE */
            BasicNumerc(exp_getnval(e), prnbuf);
            prn_iprintf(prnbuf);
            xputc(' ');
            break;
         }
      }
      /* 1980 PRINT  , , ,"A" */

   }        /* end of loop through print elements */


   /* 1980 PRINT  , , ,"A" */
   adv_ws(l->buffer, &(l->position));


   if (OutputCR == TRUE)
   {
      /* did not end with ',' or ';' */
      xputc('\n');
   }
   return TRUE;

}           /* end of function bwb_xprint() */


/***************************************************************
  
        FUNCTION:       get_prnfmt()
  
   DESCRIPTION:    This function gets the PRINT USING
         format string, returning a structure
         to the format.
  
***************************************************************/

static struct prn_fmt *
get_prnfmt(char *buffer, int *position)
{
   static struct prn_fmt retstruct;
   int             loop;

   bwx_DEBUG(__FUNCTION__);

   /* set some defaults */

   retstruct.precision = 0;
   retstruct.type = FALSE;
   retstruct.exponential = FALSE;
   retstruct.right_justified = FALSE;
   retstruct.commas = FALSE;
   retstruct.sign = FALSE;
   retstruct.money = FALSE;
   retstruct.fill = ' ';
   retstruct.minus = FALSE;
   retstruct.width = 0;

   /* check for negative position */
   if (*position < 0)
   {
      return &retstruct;
   }
   /* advance past whitespace */

   /* adv_ws( buffer, position ); *//* Don't think we want this (JBV) */

   /* check first character: a lost can be decided right here */


   loop = TRUE;
   while (loop == TRUE)
   {


      switch (buffer[*position])
      {
      case ' ':   /* end of this format segment */
         xxputc(buffer[*position]); /* Gotta output it (JBV) */
         ++(*position); /* JBV */
         if (retstruct.type != FALSE)
         {
            loop = FALSE;  /* JBV */
         }
         break;
      case '\0':  /* end of format string */
         *position = -1;
         return &retstruct;
      case '_':   /* print next character as literal */
         ++(*position);
         xxputc(buffer[*position]); /* Not xputc, no tabs
                      * (JBV) */
         ++(*position);
         break;
      case '!':
         retstruct.type = STRING;
         retstruct.width = 1;
         ++(*position); /* JBV */
         return &retstruct;
      case '&':   /* JBV */
         retstruct.type = STRING;
         retstruct.width = -1;
         ++(*position);
         return &retstruct;
      case '\\':
         retstruct.type = STRING;
         ++retstruct.width;
         ++(*position);
         for (; buffer[*position] == ' '; ++(*position))
         {
            ++retstruct.width;
         }
         if (buffer[*position] == '\\')
         {
            ++retstruct.width;
            ++(*position);
         }
         return &retstruct;
      case '%':
         retstruct.type = STRING;
         ++retstruct.width;
         ++(*position);
         for (; buffer[*position] == ' '; ++(*position))
         {
            ++retstruct.width;
         }
         if (buffer[*position] == '%')
         {
            ++retstruct.width;
            ++(*position);
         }
         return &retstruct;
      case '$':
         ++retstruct.width;   /* JBV */
         ++(*position);
         retstruct.money = TRUE;
         if (buffer[*position] == '$')
         {
            ++retstruct.width;   /* JBV */
            ++(*position);
         }
         break;
      case '*':
         ++retstruct.width;   /* JBV */
         ++(*position);
         retstruct.fill = '*';
         if (buffer[*position] == '*')
         {
            ++retstruct.width;   /* JBV */
            ++(*position);
         }
         break;
      case '+':
         ++(*position);
         retstruct.sign = TRUE;
         break;
      case '#':
         retstruct.type = NUMBER;   /* for now */
         /* ++( *position ); *//* Removed by JBV */
         /* The initial condition shouldn't be retstruct.width
          * = 1 (JBV) */
         for (; buffer[*position] == '#'; ++(*position))
         {
            ++retstruct.width;
         }
         if (buffer[*position] == ',')
         {
            retstruct.commas = TRUE;
            ++retstruct.width;   /* JBV */
            ++(*position); /* JBV */
         }
         if (buffer[*position] == '.')
         {
            retstruct.type = NUMBER;
            ++retstruct.width;
            ++(*position);
            for (retstruct.precision = 0; buffer[*position] == '#'; ++(*position))
            {
               ++retstruct.precision;
               ++retstruct.width;
            }
         }
         if (buffer[*position] == '-')
         {
            retstruct.minus = TRUE;
            ++(*position);
         }
         return &retstruct;

      case '^':
         retstruct.type = NUMBER;
         retstruct.exponential = TRUE;
         for (retstruct.width = 1; buffer[*position] == '^'; ++(*position))
         {
            ++retstruct.width;
         }
         return &retstruct;

      default: /* JBV */
         xxputc(buffer[*position]); /* Gotta output it (JBV) */
         ++(*position);
         break;

      }
   }        /* end of loop */

   puts("5");
   return &retstruct;
}


/***************************************************************
  
        FUNCTION:       prn_iprintf()
  
   DESCRIPTION:    This function outputs a null-terminated
         string to a specified file or output
         device.
  
***************************************************************/

int
prn_iprintf(char *buffer)
{
   int             n;

   bwx_DEBUG(__FUNCTION__);

   if (prn_getwidth() == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      while (*buffer)
      {
         xxxputc(*buffer);
         buffer++;
      }
      return 0;
   }
   /* check to see if width will be exceeded */
   n = prn_getcol() + strlen(buffer) - 1;
   if (n > prn_getwidth())
   {
      xputc('\n');
   }
   /* output the string */
   while (*buffer)
   {
      xputc(*buffer);
      buffer++;
   }
   return 0;
}

/***************************************************************
  
        FUNCTION:       prn_xxprintf()
  
   DESCRIPTION:    This function outputs a null-terminated
         string to a specified file or output
         device without expanding tabs.
         Added by JBV 10/95
  
***************************************************************/

static int
prn_xxprintf(char *buffer)
{
   int             n;

   bwx_DEBUG(__FUNCTION__);

   if (prn_getwidth() == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      while (*buffer)
      {
         xxxputc(*buffer);
         buffer++;
      }
      return 0;
   }
   /* check to see if width will be exceeded */
   n = prn_getcol() + strlen(buffer) - 1;
   if (n > prn_getwidth())
   {
      xxputc('\n');
   }
   /* output the string */
   while (*buffer)
   {
      xxputc(*buffer);
      buffer++;
   }
   return 0;
}

/***************************************************************
  
        FUNCTION:       xputc()
  
   DESCRIPTION:    This function outputs a character to a
         specified file or output device, expanding
         TABbed output approriately.
  
***************************************************************/

static int
xputc(char c)
{
   static char     CHR_pending = FALSE;

   bwx_DEBUG(__FUNCTION__);

   if (prn_getwidth() == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      xxxputc(c);
      return 0;
   }
   /* check for pending SPC */
   if (CHR_pending == PRN_SPC)
   {
      /* 190 PRINT SPC(A);"X" ' A = 0...255 */
      int             i;
      for (i = 0; i < c; i++)
      {
         xxputc(' ');
      }
      CHR_pending = FALSE;
      return TRUE;
   }
   /* check for pending TAB */
   if (CHR_pending == PRN_TAB)
   {
      /* WIDTH 80 */
      while (c > prn_getwidth())
      {
         /* If n is greater than the margin m, then n is
          * reduced by an integral multiple of m so that it is
          * in the range 1 <= n <= m; */
         c -= prn_getwidth();
      }
      /* 190 PRINT TAB(A);"X" ' A = 0 */
      if (c == 0)
      {
         /* use the value of one */
         c = 1;
         /* continue processing */
      }
      if ((int) c < prn_getcol())
      {
         xxputc('\n');
      }
      while (prn_getcol() < (int) c)
      {
         xxputc(' ');
      }
      CHR_pending = FALSE;
      return TRUE;
   }
   /* check for specific output options */
   switch (c)
   {
   case PRN_SPC:
   case PRN_TAB:
      CHR_pending = c;
      break;
   case '\t':
      {
         int             LastZoneColumn;
         LastZoneColumn = 1;
         while (LastZoneColumn < prn_getwidth())
         {
            LastZoneColumn += ZONEWIDTH;
         }
         LastZoneColumn -= ZONEWIDTH;

         if (prn_getcol() >= LastZoneColumn)
         {
            /* advance to a new line */
            xxputc('\n');
         }
         else
         {
            /* advance to the next print zone */
            if ((prn_getcol() % ZONEWIDTH) == 1)
            {
               xxputc(' ');
            }
            while ((prn_getcol() % ZONEWIDTH) != 1)
            {
               xxputc(' ');
            }
         }
      }
      break;
   default:
      xxputc(c);
      break;
   }
   return 0;
}

/***************************************************************
  
        FUNCTION:       xxputc()
  
   DESCRIPTION:    This function outputs a character to a
         specified file or output device, checking
         to be sure the PRINT width is within
         the bounds specified for that device.
  
***************************************************************/

static int
xxputc(char c)
{
   bwx_DEBUG(__FUNCTION__);

   if (prn_getwidth() == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      xxxputc(c);
      return 0;
   }
   /* check to see if width has been exceeded */
   if (c != '\n')
   {
      /* REM this should print one line, not two lines WIDTH 80
       * PRINT SPACE$( 80 ) */
      if (prn_getcol() > prn_getwidth())
      {
         xxxputc('\n'); /* output LF */
      }
   }
   /* output the character */
   xxxputc(c);

   return 0;
}

/***************************************************************
  
   FUNCTION:       xxxputc()
  
   DESCRIPTION:    This function sends a character to a
         specified file or output device.
  
***************************************************************/


static int
xxxputc(char c)
{
   bwx_DEBUG(__FUNCTION__);

   if (FileNumber == CONSOLE_FILE_NUMBER)
   {
      bwx_PRINT(c);
   }
   else
   if (FileNumber == LPRINT_FILE_NUMBER)
   {
      bwx_LPRINT(c);
      if (c == '\n' && LPRINT_WIDTH > 0 && LPRINT_NULLS > 0)
      {
         int             i;
         for (i = 0; i < LPRINT_NULLS; i++)
         {
            bwx_LPRINT(0);
         }
      }
   }
   else
   {
      fputc(c, dev_table[FileNumber].cfp);
   }
   /* update current column position */
   if (prn_getwidth() == 0)
   {
      /* ignore when WIDTH == 0 -- BINARY output */
      prn_setcol(1);
   }
   else
   if (c == '\n')
   {
      prn_setcol(1);
   }
   else
   {
      prn_setcol(prn_getcol() + 1);
   }
   return 0;
}

/***************************************************************
  
        FUNCTION:       prn_getcol()
  
   DESCRIPTION:    This function returns a pointer to an
         integer containing the current PRINT
         column for a specified file or device.
  
***************************************************************/

static
int
prn_getcol()
{

   bwx_DEBUG(__FUNCTION__);

   if (FileNumber == LPRINT_FILE_NUMBER)
   {
      return LPRINT_COLUMN;
   }
   return dev_table[FileNumber].col;

}

static int
prn_setcol(int NewValue)
{
   bwx_DEBUG(__FUNCTION__);

   if (FileNumber == LPRINT_FILE_NUMBER)
   {
      LPRINT_COLUMN = NewValue;
      return 0;
   }
   dev_table[FileNumber].col = NewValue;
   return 0;

}

int
ResetConsoleColumn()
{
   bwx_DEBUG(__FUNCTION__);

   dev_table[CONSOLE_FILE_NUMBER].col = 1;   /* reset column */
   return 0;
}

/***************************************************************
  
        FUNCTION:       prn_getwidth()
  
   DESCRIPTION:    This function returns the PRINT width for
         a specified file or output device.
  
***************************************************************/

static
int
prn_getwidth()
{
   bwx_DEBUG(__FUNCTION__);

   if (FileNumber == LPRINT_FILE_NUMBER)
   {
      return LPRINT_WIDTH;
   }
   return dev_table[FileNumber].width;

}

/***************************************************************
  
        FUNCTION:       prn_precision()
  
   DESCRIPTION:    This function returns the level of precision
         required for a specified numerical value.
  
***************************************************************/

int
prn_precision(struct bwb_variable * v)
{
   int             max_precision = 6;
   BasicNumberType nval, d;
   int             r;

   bwx_DEBUG(__FUNCTION__);

   /* check for numeric value */

   if (v->type == NUMBER)
   {
      max_precision = 12;
   }
   /* get the value in nval */

   nval = fabs(var_getnval(v));

   /* cycle through until precision is found */

   d = 1;
   for (r = 0; r < max_precision; ++r)
   {


      if (fmod(nval, d) < 0.0000001)   /* JBV */
      {
         return r;
      }
      d /= 10;
   }

   /* return */

   return r;

}



/***************************************************************
  
        FUNCTION:       bwb_write()
  
   DESCRIPTION:    This C function implements the BASIC WRITE
         command.
  
   SYNTAX:     WRITE [# device-number,] element [, element ]....
  
***************************************************************/

struct bwb_line *
bwb_WRITE(struct bwb_line * l)
{
   static int      pos;
   static char    *s_buffer;  /* small, temporary buffer */
   static int      init = FALSE;
   int             loop;
#if 0
   struct bwb_variable nvar;
#endif
   char           *tbuf;

   bwx_DEBUG(__FUNCTION__);

   /* initialize buffers if necessary */

   if (init == FALSE)
   {
      init = TRUE;

      /* Revised to CALLOC pass-thru call by JBV */
      if ((s_buffer = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_write")) == NULL)
      {
         bwb_error("in bwb_writw(): failed to get memory for s_buffer");
         return bwb_zline(l);
      }
   }
   FileNumber = CONSOLE_FILE_NUMBER;




   adv_ws(l->buffer, &(l->position));

   if (l->buffer[l->position] == BasicFileNumberPrefix)
   {
      struct exp_ese *v;
      int             UserFileNumber;



      ++(l->position);
      adv_ws(l->buffer, &(l->position));
      adv_element(l->buffer, &(l->position), s_buffer);
      pos = 0;
      v = bwb_exp(s_buffer, FALSE, &pos);
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      adv_ws(l->buffer, &(l->position));
      if (l->buffer[l->position] == ',')
      {
         ++(l->position);
      }
      else
      {
         bwb_error("in bwb_write(): no comma after #n");
         return bwb_zline(l);
      }

      UserFileNumber = exp_getival(v);
      /* check the requested device number */
      if (UserFileNumber < 0)
      {
         /* * PRINTER, CASSETTE, etc.  * These are all sent to *
          * bwx_LPRINT */
         FileNumber = LPRINT_FILE_NUMBER;
      }
      else
      if (UserFileNumber > BasicFileNumberMax)
      {
         bwb_error("in bwb_write(): Requested device number is out of range.");
         return bwb_zline(l);
      }
      else
      {
         FileNumber = (BasicFileNumberType) UserFileNumber;
         if ((dev_table[FileNumber].mode & DEVMODE_WRITE) == 0)
         {
            bwb_error("in bwb_write(): Requested device is not open for writing");
            return bwb_zline(l);
         }
      }





   }
   tbuf = s_buffer;

   /* be sure there is an element to print */

   adv_ws(l->buffer, &(l->position));
   loop = TRUE;
   switch (l->buffer[l->position])
   {
   case '\0':
      loop = FALSE;
      break;
   }

   /* loop through elements */

   while (loop == TRUE)
   {
      struct exp_ese *e;

      /* get the next element */

      e = bwb_exp(l->buffer, FALSE, &(l->position));
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      /* perform type-specific output */

      switch (e->type)
      {
      case STRING:
         xputc('\"');
         str_btoc(tbuf, exp_getsval(e));
         prn_iprintf(tbuf);
         xputc('\"');
         break;
      default:
#if 0
         *var_findnval(&nvar, nvar.array_pos) =
            exp_getnval(e);
         sprintf(tbuf, " %.*f", prn_precision(&nvar),
            var_getnval(&nvar));
#endif
         BasicNumerc(exp_getnval(e), tbuf);
         prn_iprintf(tbuf);
         break;
      }     /* end of case for type-specific output */

      /* seek a comma at end of element */

      adv_ws(l->buffer, &(l->position));
      if (l->buffer[l->position] == ',')
      {
         xputc(',');
         ++(l->position);
      }
      /* no comma: end the loop */

      else
      {
         loop = FALSE;
      }

   }        /* end of loop through elements */

   /* print LF */

   xputc('\n');

   /* return */

   return bwb_zline(l);
}


/*  EOF  */
