/***************************************************************
  
        bwb_inp.c       Input Routines
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





/* Declarations of functions visible to this file only */




static struct bwb_line *
bwb_xinp(struct bwb_line * l, FILE * f);
static int
inp_str(struct bwb_line * l, char *buffer,
   char *var_list, int *position, int IsFake);
static int
inp_const(char *m_buffer, char *s_buffer, int *position);
static int
inp_assign(char *b, struct bwb_variable * v, int IsFake, int *ResultCode);

static int      last_inp_adv_rval = FALSE;   /* JBV */

/* ResultCode */
#define RESULT_OK 0
#define RESULT_UNKNOWN 1
#define RESULT_TYPE_MMISMATCH 2
#define RESULT_ARITHMETIC_OVERFLOW 3




/***************************************************************
  
      FUNCTION:       bwx_input()
  
   DESCRIPTION:    This function outputs the string pointed
         to by 'prompt', then inputs a character
         string.
  
***************************************************************/

int
bwx_input(char *prompt, char *buffer)
{

   bwx_DEBUG(__FUNCTION__);

   prn_xprintf(prompt);

   fflush(stdout);

#if AUTOMATED_REGRESSION
   /* for automated testing */
   if (IsCommandLineFile == TRUE)
   {
      if (ExternalInputFile != NULL)
      {
         fgets(buffer, BasicStringLengthMax, ExternalInputFile);
         /* stop reading from external file once all INPUT
          * lines have been read */
         if (feof(ExternalInputFile))
         {
            fclose(ExternalInputFile);
            ExternalInputFile = NULL;
         }
         else
         {
            fputs(buffer, stdout);
            fflush(stdout);
            ResetConsoleColumn();
            return TRUE;
         }
      }
   }
#endif            /* AUTOMATED_REGRESSION */
   fgets(buffer, BasicStringLengthMax, stdin);
   if (TRUE)
   {
      /* some compilers do NOT remove CR/LF, some compilers DO */
      char           *E;
      E = strchr(buffer, '\r');
      if (E != NULL)
      {
         *E = '\0';
      }
      E = strchr(buffer, '\n');
      if (E != NULL)
      {
         *E = '\0';
      }
   }
   ResetConsoleColumn();

   return TRUE;

}



/***************************************************************
  
        FUNCTION:       bwb_read()
  
        DESCRIPTION:    This function implements the BASIC READ
                        statement.
  
        SYNTAX:         READ variable[, variable...]
  
***************************************************************/

struct bwb_line *
bwb_READ(struct bwb_line * l)
{
   register int    n;
   int             main_loop;
   struct bwb_variable *v;
   int             n_params;  /* number of parameters */
   int            *pp;  /* pointer to parameter values */
   char            tbuf[BasicStringLengthMax + 1];
   int             ResultCode;


   bwx_DEBUG(__FUNCTION__);

   /* Process each variable read from the READ statement */

   main_loop = TRUE;
   while (main_loop == TRUE)
   {
      int             adv_loop;

      /* first check position in l->buffer and advance beyond
       * whitespace */

      adv_loop = TRUE;
      while (adv_loop == TRUE)
      {


         switch (l->buffer[l->position])
         {
         case ',':   /* comma delimiter */
         case ' ':   /* whitespace */
            ++l->position;
            break;
         case '\0':
            adv_loop = FALSE; /* break out of advance
                      * loop */
            main_loop = FALSE;   /* break out of main
                      * loop */
            break;
         default: /* anything else */
            adv_loop = FALSE; /* break out of advance
                      * loop */
            break;
         }
         if( l->buffer[l->position] == OptionCommentChar )
         {
            adv_loop = FALSE; /* break out of advance
                      * loop */
            main_loop = FALSE;   /* break out of main
                      * loop */
         }
      }


      /* be sure main_loop id still valid after checking the line */

      if (main_loop == TRUE)
      {

         /* Read a variable name */

         bwb_getvarname(l->buffer, tbuf, &(l->position));
         inp_adv(l->buffer, &(l->position));
         if (bwb_isvar(tbuf) == FALSE)
         {
            /* not an existing variable */
            char           *expression = l->buffer;
            int             LastPosition = l->position;
            adv_ws(expression, &(LastPosition));
            if (expression[LastPosition] == '(')
            {
               /* MUST be a dynamically created
                * array, READ A(10) ... ' variable
                * "A" has NOT been dimensioned */
               int             NumDimensions;

               NumDimensions = DetermineNumberOfDimensions(expression, LastPosition);
               if (NumDimensions < 1)
               {
                  bwb_error(err_incomplete);
                  return bwb_zline(l);
               }
               if (ImplicitDim(tbuf, NumDimensions) != TRUE)
               {
                  bwb_error(err_syntax);
                  return bwb_zline(l);
               }
            }  /* if( expression[ LastPosition ] ==
                * '(' ) */
         }
         v = var_find(tbuf);


         /* advance beyond whitespace or comma in data buffer */

         inp_adv(CURTASK data_line->buffer, &CURTASK data_pos);

         if (OptionFlags & OPTION_COVERAGE_ON)
         {
            /* this line has been READ */
#if 0
            data_line->Coverage = '*';
#endif
            data_line->LineFlags |= LINE_EXECUTED;
         }
         /* Advance to next line if end of buffer */

         if (CURTASK data_line->buffer[CURTASK data_pos] == '\0')
         {
            /* end of buffer */

            CURTASK         data_line = CURTASK data_line->next;

            /* advance farther to line with DATA
             * statement if necessary */
            CURTASK         data_pos = CURTASK data_line->Startpos;


         }
         while (CURTASK data_line->cmdnum != C_DATA)
         {

            if (CURTASK data_line == &CURTASK bwb_end)
            {

               if (OptionVersion & (B14 | E78 | E86))
               {
                  /* halt */
                  bwb_Warning_OutOfData("Out of DATA");
                  return bwb_zline(l);
               }
               else
               {
                  /* wrap around */
                  CURTASK         data_line = CURTASK bwb_start.next;
               }

            }
            else
            {
               CURTASK         data_line = CURTASK data_line->next;
            }

            CURTASK         data_pos = CURTASK data_line->Startpos;


         }

         /* advance beyond whitespace in data buffer */

         adv_loop = TRUE;
         while (adv_loop == TRUE)
         {
            switch (CURTASK data_line->buffer[CURTASK data_pos])
            {
            case '\0':  /* end of buffer */
               bwb_error(err_od);
               return bwb_zline(l);
            case ' ':   /* whitespace */
               ++CURTASK data_pos;
               break;
            default:
               adv_loop = FALSE; /* carry on */
               break;
            }
            if( CURTASK data_line->buffer[CURTASK data_pos] == OptionCommentChar )
            {
               bwb_error(err_od);
               return bwb_zline(l);
            }
         }

         /* now at last we have a variable in v that needs to
          * be assigned data from the data_buffer at position
          * CURTASK data_pos. What remains to be done is to
          * get one single bit of data, a string constant or
          * numerical constant, into the small buffer */

         /* leading whitespace is NOT part of the DATA item */
         while (CURTASK data_line->buffer[CURTASK data_pos] == ' ')
         {
            ++(CURTASK data_pos);
         }
         if (CURTASK data_line->buffer[CURTASK data_pos] == '"')
         {
            if (v->type != STRING)
            {
               bwb_error("Type mismatch");
            }
         }
         inp_const(CURTASK data_line->buffer, tbuf, &CURTASK data_pos);



         /* get parameters if the variable is dimensioned */

         adv_ws(l->buffer, &(l->position));
         if (l->buffer[l->position] == '(')
         {
            dim_getparams(l->buffer, &(l->position), &n_params, &pp);
            for (n = 0; n < v->dimensions; ++n)
            {
               v->array_pos[n] = pp[n];
            }
         }
         /* finally assign the data to the variable */
         ResultCode = RESULT_UNKNOWN;
         inp_assign(tbuf, v, FALSE, &ResultCode);
         switch (ResultCode)
         {
         case RESULT_OK:
            break;
         case RESULT_UNKNOWN:
            bwb_error("Internal Error");
            return bwb_zline(l);
            break;
         case RESULT_TYPE_MMISMATCH:
            bwb_error("Type Mismatch");
            return bwb_zline(l);
            break;
         case RESULT_ARITHMETIC_OVERFLOW:
            bwb_Warning_Overflow("*** Arithmetic Overflow ***");
            break;
         }
         /* OK */
      }     /* end of remainder of main loop */
   }        /* end of main_loop */


   return bwb_zline(l);

}

/***************************************************************
  
        FUNCTION:       bwb_data()
  
        DESCRIPTION:    This function implements the BASIC DATA
                        statement, although at the point at which
                        DATA statements are encountered, no
                        processing is done.  All actual processing
                        of DATA statements is accomplished by READ
                        (bwb_read()).
  
        SYNTAX:         DATA constant[, constant]...
  
***************************************************************/

struct bwb_line *
bwb_DATA(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

   while (l->buffer[l->position] != '\0')
   {
      l->position++;
   }

   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_restore()
  
        DESCRIPTION:    This function implements the BASIC RESTORE
                        statement.
  
        SYNTAX:         RESTORE [line number]
  
***************************************************************/

struct bwb_line *
bwb_RESTORE(struct bwb_line * l)
{
   struct bwb_line *r;
   struct bwb_line *r_line;
   int             n;
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   /* get the first element beyond the starting position */

   adv_element(l->buffer, &(l->position), tbuf);

   /* if the line is not a numerical constant, then there is no
    * argument; set the current line to the first in the program */

   if (is_numconst(tbuf) != TRUE)
   {
      CURTASK         data_line = &CURTASK bwb_start;
      CURTASK         data_pos = 0;
      return bwb_zline(l);
   }
   /* find the line */

   n = atoi(tbuf);


   r_line = NULL;
   for (r = CURTASK bwb_start.next; r != &CURTASK bwb_end; r = r->next)
   {

      if (r->number == n)
      {
         r_line = r;
      }
   }

   if (r_line == NULL)
   {
      sprintf(bwb_ebuf, "at line %d: Can't find line number for RESTORE.",
         l->number);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   /* verify that line is a data statement */

   if (r_line->cmdnum != C_DATA)
   {
      sprintf(bwb_ebuf, "at line %d: Line %d is not a DATA statement.",
         l->number, r_line->number);
      bwb_error(bwb_ebuf);
      return bwb_zline(l);
   }
   /* reassign CURTASK data_line */

   CURTASK         data_line = r_line;
   CURTASK         data_pos = CURTASK data_line->Startpos;

   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_input()
  
        DESCRIPTION:    This function implements the BASIC INPUT
                        statement.
  
        SYNTAX:         INPUT [;][prompt$;]variable[$,variable]...
                        INPUT#n variable[$,variable]...
  
***************************************************************/

struct bwb_line *
bwb_INPUT(struct bwb_line * l)
{
   FILE           *fp;
   int             pos;
   int             req_devnumber;
   int             is_prompt;
   int             suppress_qm;
   char            tbuf[BasicStringLengthMax + 1];
   char            pstring[BasicStringLengthMax + 1];
   int             Loop;
   int             LastPosition;

   bwx_DEBUG(__FUNCTION__);

   pstring[0] = '\0';
   req_devnumber = CONSOLE_FILE_NUMBER;



   adv_ws(l->buffer, &(l->position));

   if (l->buffer[l->position] == BasicFileNumberPrefix)
   {
      struct exp_ese *v;

      ++(l->position);
      adv_ws(l->buffer, &(l->position));
      adv_element(l->buffer, &(l->position), tbuf);
      pos = 0;
      v = bwb_exp(tbuf, FALSE, &pos);
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
         bwb_error("in bwb_input(): no comma after #n");
         return bwb_zline(l);
      }

      req_devnumber = exp_getival(v);


      /* check the requested device number */

      if ((req_devnumber < 0) || (req_devnumber > BasicFileNumberMax))
      {
         bwb_error("in bwb_input(): Requested device number is out if range.");
         return bwb_zline(l);
      }
      if ((dev_table[req_devnumber].mode & DEVMODE_READ) == 0)
      {
         bwb_error("in bwb_input(): Requested device is not open for reading");

         return bwb_zline(l);
      }
      /* look up the requested device in the device table */

   }
   if (req_devnumber == CONSOLE_FILE_NUMBER)
   {
      fp = stdin;
   }
   else
   {
      fp = dev_table[req_devnumber].cfp;
   }

   /* if input is not from stdin, then branch to bwb_xinp() */

   if (fp != stdin)
   {
      return bwb_xinp(l, fp);
   }
   /* from this point we presume that input is from stdin */



   /* check for a semicolon or a quotation mark, not in first position:
    * this should indicate a prompt string */

   suppress_qm = is_prompt = FALSE;

   adv_ws(l->buffer, &(l->position));

   switch (l->buffer[l->position])
   {
   case '\"':
      is_prompt = TRUE;
      break;

   case ';':

      /* AGENDA: add code to suppress newline if a semicolon is
       * used here; this may not be possible using ANSI C alone,
       * since it has not functions for unechoed console input. */

      is_prompt = TRUE;
      ++l->position;
      break;

   case ',':

      /* QUERY: why is this code here? the question mark should be
       * suppressed if a comma <follows> the prompt string. */

      suppress_qm = TRUE;
      ++l->position;
      break;
   }


   /* get prompt string and print it */

   if (is_prompt == TRUE)
   {

      /* get string element */

      inp_const(l->buffer, tbuf, &(l->position));

      /* advance past semicolon to beginning of variable */

      /*--------------------------------------------------------*/
      /* Since inp_const was just called and inp_adv is called  */
      /* within that, it will have already noted and passed the */
      /* comma by the time it gets here.  Therefore one must    */
      /* refer instead to the last returned value for inp_adv!  */
      /* (JBV, 10/95)                                           */
      /*--------------------------------------------------------*/
      /* suppress_qm = inp_adv( l->buffer, &( l->position ) ); */
      suppress_qm = last_inp_adv_rval;

      /* print the prompt string */

      strncpy(pstring, tbuf, BasicStringLengthMax);
   }        /* end condition: prompt string */
   /* * * ' print out the question mark delimiter unless it has been
    * suppressed */

   if (suppress_qm != TRUE)
   {
      strncat(pstring, "? ", BasicStringLengthMax);
   }
   Loop = TRUE;
   LastPosition = l->position;
   while (Loop == TRUE)
   {
      int             SavedPosition;


      l->position = LastPosition;


      /* read a line into the input buffer */

      bwx_input(pstring, tbuf);
      bwb_stripcr(tbuf);


      /* reset print column to account for LF at end of fgets() */

      ResetConsoleColumn();


      SavedPosition = l->position;
      if (inp_str(l, tbuf, l->buffer, &(l->position), TRUE) == TRUE)
      {
         /* successful input, FAKE run  */
         l->position = SavedPosition;
         if (inp_str(l, tbuf, l->buffer, &(l->position), FALSE) == TRUE)
         {
            /* successful input, REAL run  */
            Loop = FALSE;
         }
      }
      else
      {
         puts("*** Retry INPUT ***");
      }
   }        /* while( Loop == TRUE ) */
   return bwb_zline(l);;


}


static struct bwb_line *
bwb_xinp(struct bwb_line * l, FILE * f)
{
   /* INPUT # is similar to READ, where each file line is a DATA line, 
    *
    */

   register int    n;
   int             main_loop;
   struct bwb_variable *v;
   int             n_params;  /* number of parameters */
   int            *pp;  /* pointer to parameter values */
   char            tbuf[BasicStringLengthMax + 1];
#if 0
   int             ResultCode;
#endif
   int             c;   /* character */


   bwx_DEBUG(__FUNCTION__);

   /* Process each variable read from the READ statement */

   main_loop = TRUE;
   while (main_loop == TRUE)
   {
      int             adv_loop;

      /* first check position in l->buffer and advance beyond
       * whitespace */

      adv_loop = TRUE;
      while (adv_loop == TRUE)
      {


         switch (l->buffer[l->position])
         {
         case ',':   /* comma delimiter */
         case ' ':   /* whitespace */
            ++l->position;
            break;
         case '\0':
            adv_loop = FALSE; /* break out of advance
                      * loop */
            main_loop = FALSE;   /* break out of main
                      * loop */
            break;
         default: /* anything else */
            adv_loop = FALSE; /* break out of advance
                      * loop */
            break;
         }
         if( l->buffer[l->position] == OptionCommentChar )
         {
            adv_loop = FALSE; /* break out of advance
                      * loop */
            main_loop = FALSE;   /* break out of main
                      * loop */
         }

      }


      /* be sure main_loop id still valid after checking the line */

      if (main_loop == TRUE)
      {

         /* Read a variable name */

         bwb_getvarname(l->buffer, tbuf, &(l->position));
         inp_adv(l->buffer, &(l->position));
         if (bwb_isvar(tbuf) == FALSE)
         {
            /* not an existing variable */
            char           *expression = l->buffer;
            int             LastPosition = l->position;
            adv_ws(expression, &(LastPosition));
            if (expression[LastPosition] == '(')
            {
               /* MUST be a dynamically created
                * array, READ A(10) ... ' variable
                * "A" has NOT been dimensioned */
               int             NumDimensions;

               NumDimensions = DetermineNumberOfDimensions(expression, LastPosition);
               if (NumDimensions < 1)
               {
                  bwb_error(err_incomplete);
                  return bwb_zline(l);
               }
               if (ImplicitDim(tbuf, NumDimensions) != TRUE)
               {
                  bwb_error(err_syntax);
                  return bwb_zline(l);
               }
            }  /* if( expression[ LastPosition ] ==
                * '(' ) */
         }
         v = var_find(tbuf);
         /* get parameters if the variable is dimensioned */
         adv_ws(l->buffer, &(l->position));
         if (l->buffer[l->position] == '(')
         {
            dim_getparams(l->buffer, &(l->position), &n_params, &pp);
            for (n = 0; n < v->dimensions; ++n)
            {
               v->array_pos[n] = pp[n];
            }
         }


         /* advance beyond whitespace or comma in data buffer */
         /* Advance to next line if end of buffer */
         /* advance beyond whitespace in data buffer */
         /* leading whitespace is NOT part of the DATA item */
         c = ' ';
         while (c <= ' ')
         {
            c = fgetc(f);
            if (c < 0)
            {
               /* EOF */
               bwb_error(err_od);
               return bwb_zline(l);
            }
         }

         /* now at last we have a variable in v that needs to
          * be assigned data from the data_buffer at position
          * CURTASK data_pos. What remains to be done is to
          * get one single bit of data, a string constant or
          * numerical constant, into the small buffer */

         {

            int             string; /* a quoted string */
            int             s_pos;
            int             loop;


            string = FALSE;
            s_pos = 0;
            loop = TRUE;

            /* build the constant string */
            while (loop == TRUE)
            {

               if (c < ' ')
               {
                  /* END-OF-LINE */
                  loop = FALSE;
               }
               else
               if (c == ',')
               {
                  if (string == FALSE)
                  {
                     /* end of argument */
                     loop = FALSE;
                  }
                  else
                  {
                     /* internal comma is
                      * part of the DATA
                      * item */
                     tbuf[s_pos] = c;
                     ++s_pos;
                  }
               }
               else
               if (c == '"')
               {
                  /* * Once we finish reading a
                   * quoted string, * we want
                   * to continue reading spaces *
                   * until EOL or comma */
                  if (string == TRUE)
                  {
                     string = FALSE;
                  }
                  else
                  {
                     string = TRUE;
                  }
                  tbuf[s_pos] = c;
                  ++s_pos;
               }
               else
               {
                  tbuf[s_pos] = c;
                  ++s_pos;
               }
               if (loop == TRUE)
               {
                  /* read another character */
                  c = fgetc(f);
                  if (c < 0)
                  {
                     /* EOF */
                     bwb_error(err_od);
                     return bwb_zline(l);
                  }
               }
            }

            if (string == FALSE)
            {
               /* trailing whitespace is NOT part of
                * the DATA item */
               while (s_pos > 0 && tbuf[s_pos - 1] == ' ')
               {
                  s_pos--;
               }
            }
            tbuf[s_pos] = '\0';
            /* clean-up quoted string */
            if (s_pos > 0 && tbuf[0] == '"')
            {
               /* not an empty string */
               char           *Q;

               Q = strrchr(&tbuf[1], '"');
               if (Q != NULL)
               {
                  *Q = '\0';
               }
               strcpy(tbuf, &tbuf[1]);
            }
         }

         /* finally assign the data to the variable */
         if (v->type == STRING)
         {
            str_ctob(var_findsval(v, v->array_pos), tbuf);
         }
         else
         {
            /* N = VAL( A$ ) */
            BasicNumberType Value = 0;
            if (strlen(tbuf) > 0)
            {
               int             ScanResult;
               ScanResult = sscanf(tbuf, BasicNumberScanFormat, &Value);
               if (ScanResult != 1)
               {
                  /* not a number */
                  Value = 0;
               }
               else
               {
                  /* OK */
               }
            }
            *(var_findnval(v, v->array_pos)) = Value;
         }
#if 0
         ResultCode = RESULT_UNKNOWN;
         inp_assign(tbuf, v, FALSE, &ResultCode);
         switch (ResultCode)
         {
         case RESULT_OK:
            break;
         case RESULT_UNKNOWN:
            bwb_error("Internal Error");
            return bwb_zline(l);
            break;
         case RESULT_TYPE_MMISMATCH:
            bwb_error("Type Mismatch");
            return bwb_zline(l);
            break;
         case RESULT_ARITHMETIC_OVERFLOW:
            bwb_Warning_Overflow("*** Arithmetic Overflow ***");
            break;
         }
#endif
         /* OK */
      }     /* end of remainder of main loop */
   }        /* end of main_loop */


   return bwb_zline(l);

}


/***************************************************************
  
        FUNCTION:       inp_str()
  
        DESCRIPTION:    This function does INPUT processing
                        from a determined string of input
                        data and a determined variable list
                        (both in memory).  This presupposes
         that input has been taken from stdin,
         not from a disk file or device.
  
***************************************************************/

static int
inp_str(struct bwb_line * l, char *input_buffer, char *var_list, int *vl_position, int IsFake)
{
   int             i;
   register int    n;
   int             loop;
   int            *pp;
   int             n_params;
   char            ttbuf[BasicStringLengthMax + 1];   /* build element */
   char            varname[BasicStringLengthMax + 1]; /* build element */

   int             Result;
   int             ReadAllVars;
   int             ReadAllData;
   int             ResultCode;

   bwx_DEBUG(__FUNCTION__);

   Result = FALSE;
   ReadAllVars = FALSE;
   ReadAllData = FALSE;




   /* Read elements, and assign them to variables */

   i = 0;
   loop = TRUE;
   while (loop == TRUE)
   {
      struct bwb_variable *v;

      /* get a variable name from the list */

      bwb_getvarname(var_list, varname, vl_position); /* get name */
      if (strlen(varname) == 0)
      {
         bwb_error("Syntax Error");
         return FALSE;
      }
      if (bwb_isvar(varname) == FALSE)
      {
         /* not an existing variable */
         char           *expression = var_list;
         int             LastPosition = *vl_position;
         adv_ws(expression, &(LastPosition));
         if (expression[LastPosition] == '(')
         {
            /* MUST be a dynamically created array, INPUT
             * A(10) ... ' variable "A" has NOT been
             * dimensioned */
            int             NumDimensions;

            NumDimensions = DetermineNumberOfDimensions(expression, LastPosition);
            if (NumDimensions < 1)
            {
               bwb_error(err_incomplete);
               return FALSE;
            }
            if (ImplicitDim(varname, NumDimensions) != TRUE)
            {
               bwb_error(err_syntax);
               return FALSE;
            }
         }  /* if( expression[ LastPosition ] == '(' ) */
      }
      v = var_find(varname);


      /* read subscripts if appropriate */

      adv_ws(var_list, vl_position);
      if (var_list[*vl_position] == '(')
      {
         dim_getparams(var_list, vl_position, &n_params, &pp);
         for (n = 0; n < v->dimensions; ++n)
         {
            v->array_pos[n] = pp[n];
         }
      }
      /* build string from input buffer in ttbuf */

      n = 0;
      ttbuf[0] = '\0';
      while ((input_buffer[i] != ',')
             && (input_buffer[i] != '\0'))
      {
         if (input_buffer[i] == '"')
         {
            ttbuf[n] = input_buffer[i];   /* copy the first quote */
            i++;  /* skip past first quote */
            n++;
            while ((input_buffer[i] != '"')
                   && (input_buffer[i] != '\0'))
            {
               ttbuf[n] = input_buffer[i];
               i++;
               n++;
            }
            if (input_buffer[i] == '"')
            {
               ttbuf[n] = input_buffer[i];   /* copy the last quote */
               i++;  /* skip past last quote */
               n++;
            }
         }
         else
         {
            ttbuf[n] = input_buffer[i];
            i++;  /* skip past first quote */
            n++;
         }
      }
      ttbuf[n] = '\0';/* terminate */




      if (TRUE)
      {
         char           *b;   /* pointer to the beginning
                   * of the string */
         char           *e;   /* pointer to the ending of
                   * the string */

         /* remove contol characters */
         b = ttbuf;
         while (*b)
         {
            if (*b < ' ')
            {
               *b = ' ';
            }
            b++;
         }
         /* remove leading spaces */
         b = ttbuf;
         while (*b == ' ')
         {
            b++;
         }
         /* remove trailing spaces */
         e = strchr(b, '\0');
         if (e != NULL)
         {
            int             WasQuoted;

            WasQuoted = FALSE;
            e--;
            while (e > b && *e == ' ')
            {
               *e = '\0';
               e--;
            }
            if (strlen(b) == 0)
            {
               if (OptionFlags & OPTION_BUGS_ON)
               {
                  /* silently ignore */
               }
               else
               {
                  puts("*** Type Mismatch ***");
                  return FALSE;
               }
            }
            /* remove matching quotes */
            if (*b == '"')
            {
               /* first char is a quote */
               e = strchr(b, '\0');
               if (e != NULL)
               {
                  e--;
                  if (e > b)
                  {
                     if (*e == '"')
                     {
                        /* last char
                         * is a quote */
                        b++;
                        *e = '\0';
                        WasQuoted = TRUE;
                     }
                  }
               }
            }
            if (OptionFlags & OPTION_BUGS_ON)
            {
               /* silently ignore */
            }
            else
            if (WasQuoted == FALSE)
            {
               /* if was NOT quoted, then the only
                * valid chars are ' ', '+', '-',
                * '.', digit, letter */
               char           *P;
               P = b;
               while (*P != '\0')
               {
                  char            C;

                  C = *P;
                  P++;
                  /* switch */
                  if (C == ' ' || C == '+' || C == '-' || C == '.' || isdigit(C) || isalpha(C))
                  {
                     /* OK */
                  }
                  else
                  {
                     /* ERROR */
                     puts("*** Type Mismatch ***");
                     return FALSE;
                  }
               }
            }
         }
         strcpy(ttbuf, b);
      }
      /* perform type-specific input */

      ResultCode = RESULT_UNKNOWN;
      inp_assign(ttbuf, v, IsFake, &ResultCode);
      switch (ResultCode)
      {
      case RESULT_OK:
         break;
      case RESULT_UNKNOWN:
         bwb_error("Internal Error");
         return FALSE;
         break;
      case RESULT_TYPE_MMISMATCH:
         puts("*** Type Mismatch ***");
         return FALSE;
         break;
      case RESULT_ARITHMETIC_OVERFLOW:
         puts("*** Overflow ***");
         return FALSE;
         break;
      }
      /* OK */


      /* check for commas in variable list and input list and
       * advance */

      adv_ws(var_list, vl_position);
      switch (var_list[*vl_position])
      {
      case '\0':
         loop = FALSE;
         ReadAllVars = TRUE;
         break;
      case ',':
         ++(*vl_position);
         break;
      }
      adv_ws(var_list, vl_position);

      adv_ws(input_buffer, &i);
      switch (input_buffer[i])
      {
      case '\0':
         loop = FALSE;
         ReadAllData = TRUE;
         break;
      case ',':
         ++i;
         break;
      }
      adv_ws(input_buffer, &i);

   }


   /* return */

   if (ReadAllVars == TRUE && ReadAllData == TRUE)
   {
      Result = TRUE;
   }
   else
   {
      /* READ/DATA mismatch */
      puts("*** Count Mismatch ***");
      return FALSE;
   }

   return Result;

}

/***************************************************************
  
        FUNCTION:       inp_assign()
  
        DESCRIPTION:    This function assigns the value of a
                        numerical or string constant to a
                        variable.
  
  
***************************************************************/



static int
inp_assign(char *b, struct bwb_variable * v, int IsFake, int *ResultCode)
{
   bwx_DEBUG(__FUNCTION__);

   switch (v->type)
   {
   case STRING:
      if (IsFake == TRUE)
      {
      }
      else
      {
         str_ctob(var_findsval(v, v->array_pos), b);
      }
      if (strchr(b, '"') != NULL)
      {
         /* embedded quotes */
         *ResultCode = RESULT_TYPE_MMISMATCH;
      }
      else
      {
         *ResultCode = RESULT_OK;
      }
      break;

   case NUMBER:
      if (strlen(b) == 0)
      {
         if (IsFake == TRUE)
         {
         }
         else
         {
            *(var_findnval(v, v->array_pos)) = 0.0;
         }
         if (OptionFlags & OPTION_BUGS_ON)
         {
            *ResultCode = RESULT_OK;
         }
         else
         {
            *ResultCode = RESULT_TYPE_MMISMATCH;
         }
      }
      else
      {
         if (IsFake == TRUE)
         {
            inp_numconst(b, ResultCode);
         }
         else
         {
            *(var_findnval(v, v->array_pos)) = inp_numconst(b, ResultCode);
         }
      }
      break;

   default:
      *ResultCode = RESULT_TYPE_MMISMATCH;
      return FALSE;

   }

   return FALSE;

}

/***************************************************************
  
        FUNCTION:       inp_adv()
  
        DESCRIPTION:    This function advances the string pointer
                        past whitespace and the item delimiter
                        (comma).
  
***************************************************************/

int
inp_adv(char *b, int *c)
{
   int             rval;

   bwx_DEBUG(__FUNCTION__);

   rval = FALSE;

   while (TRUE)
   {
      switch (b[*c])
      {
      case ' ':   /* whitespace */
      case ';':   /* semicolon, end of prompt string */
         ++*c;
         break;
      case ',':   /* comma, variable delimiter */
         rval = TRUE;
         ++*c;
         break;
      case '\0':  /* end of line */
         rval = TRUE;
         last_inp_adv_rval = rval;  /* JBV */
         return rval;
      default:
         last_inp_adv_rval = rval;  /* JBV */
         return rval;
      }
   }
}

/***************************************************************
  
        FUNCTION:       inp_const()
  
        DESCRIPTION:    This function reads a numerical or string
                        constant from <m_buffer> into <s_buffer>,
                        incrementing <position> appropriately.
  
***************************************************************/

static int
inp_const(char *m_buffer, char *s_buffer, int *position)
{
   int             string;
   int             s_pos;
   int             loop;

   bwx_DEBUG(__FUNCTION__);

   /* leading whitespace is NOT part of the DATA item */
   while (m_buffer[*position] == ' ')
   {
      ++(*position);
   }




   string = FALSE;

   /* first detect string constant */

   if (m_buffer[*position] == '\"')
   {
      string = TRUE;
      ++(*position);
   }
   else
   {
      string = FALSE;
   }

   /* build the constant string */

   s_pos = 0;
   s_buffer[s_pos] = '\0';


   loop = TRUE;

   while (loop == TRUE)
   {
      if( m_buffer[*position] == OptionCommentChar && string == FALSE )
      {
         /* end of string */
            /* trailing whitespace is NOT part of the
             * DATA item */
            while (s_pos > 0 && s_buffer[s_pos - 1] == ' ')
            {
               s_pos--;
               s_buffer[s_pos] = '\0';
            }
         return TRUE;
      }

      switch (m_buffer[*position])
      {
      case '\0':  /* end of string */
         return TRUE;
         /* internal whitespace is part of the DATA item */
      case ',':   /* or end of argument */
         if (string == FALSE)
         {
            /* trailing whitespace is NOT part of the
             * DATA item */
            while (s_pos > 0 && s_buffer[s_pos - 1] == ' ')
            {
               s_pos--;
               s_buffer[s_pos] = '\0';
            }
            return TRUE;
         }
         else
         {
            s_buffer[s_pos] = m_buffer[*position];
            ++(*position);
            ++s_pos;
            s_buffer[s_pos] = '\0';
         }
         break;
      case '\"':
         if (string == TRUE)
         {
            ++(*position); /* advance beyond quotation
                   * mark */
            inp_adv(m_buffer, position);
            return TRUE;
         }
         else
         {
            sprintf(bwb_ebuf, "Unexpected character in numerical constant.");
            bwb_error(bwb_ebuf);
            return FALSE;
         }
      default:
         s_buffer[s_pos] = m_buffer[*position];
         ++(*position);
         ++s_pos;
         s_buffer[s_pos] = '\0';
         break;
      }

   }

   return FALSE;

}


/***************************************************************
  
        FUNCTION:       bwb_line()
  
        DESCRIPTION:    This function implements the BASIC LINE
                        INPUT statement.
  
   SYNTAX:         LINE INPUT [[#] device-number,]["prompt string";] string-variable$
  
***************************************************************/

struct bwb_line *
bwb_LINE(struct bwb_line * l)
{
   int             dev_no;
   struct bwb_variable *v;
   FILE           *inp_device;
   char            tbuf[BasicStringLengthMax + 1];
   char            pstring[BasicStringLengthMax + 1];
   struct exp_ese *e;   /* JBV */
   int             pos; /* JBV */

   bwx_DEBUG(__FUNCTION__);

   /* assign default values */

   inp_device = stdin;

   pstring[0] = '\0';

   /* advance to first element (INPUT statement) */

   adv_element(l->buffer, &(l->position), tbuf);
   if (strcasecmp(tbuf, "INPUT") != 0)
   {
      bwb_error(err_syntax);
      return bwb_zline(l);
   }
   adv_ws(l->buffer, &(l->position));

   /* check for semicolon in first position */

   if (l->buffer[l->position] == ';')
   {
      ++l->position;
      adv_ws(l->buffer, &(l->position));
   }
   /* else check for# for file number in first position */

   else
   if (l->buffer[l->position] == BasicFileNumberPrefix)
   {
      ++l->position;
      adv_ws(l->buffer, &(l->position));
      adv_element(l->buffer, &(l->position), tbuf);
      adv_ws(l->buffer, &(l->position));
      /* dev_no = atoi( tbuf ); *//* We really need more, added
       * next (JBV) */
      pos = 0;
      e = bwb_exp(tbuf, FALSE, &pos);
      if (ERROR_PENDING)
      {
         return bwb_zline(l);
      }
      dev_no = exp_getival(e);


      if (dev_table[dev_no].cfp == NULL)
      {
         bwb_error(err_dev);
         return bwb_zline(l);
      }
      else
      {
         inp_device = dev_table[dev_no].cfp;
      }
   }
   /* check for comma */

   if (l->buffer[l->position] == ',')
   {
      ++(l->position);
      adv_ws(l->buffer, &(l->position));
   }
   /* check for quotation mark indicating prompt */

   if (l->buffer[l->position] == '\"')
   {
      inp_const(l->buffer, pstring, &(l->position));
   }
   /* read the variable for assignment */


#if 0
   adv_element(l->buffer, &(l->position), tbuf);
#endif
   bwb_getvarname(l->buffer, tbuf, &(l->position));
   if (bwb_isvar(tbuf) == FALSE)
   {
      /* not an existing variable */
      char           *expression = l->buffer;
      int             LastPosition = l->position;
      adv_ws(expression, &(LastPosition));
      if (expression[LastPosition] == '(')
      {
         /* MUST be a dynamically created array, LINE INPUT
          * A(10) ... ' variable "A" has NOT been dimensioned */
         int             NumDimensions;

         NumDimensions = DetermineNumberOfDimensions(expression, LastPosition);
         if (NumDimensions < 1)
         {
            bwb_error(err_incomplete);
            return bwb_zline(l);
         }
         if (ImplicitDim(tbuf, NumDimensions) != TRUE)
         {
            bwb_error(err_syntax);
            return bwb_zline(l);
         }
      }     /* if( expression[ LastPosition ] == '(' ) */
   }
   v = var_find(tbuf);
   if (v->type != STRING)
   {
      bwb_error("in bwb_line(): String variable required");
      return bwb_zline(l);
   }
   /* read a line of text into the bufffer */

   if (inp_device == stdin)
   {
      bwx_input(pstring, tbuf);
   }
   else
   {
      fgets(tbuf, BasicStringLengthMax, inp_device);
   }
   bwb_stripcr(tbuf);
   str_ctob(var_findsval(v, v->array_pos), tbuf);

   /* end: return next line */

   return bwb_zline(l);
}


/***************************************************************
  
        FUNCTION: inp_numconst()
  
        DESCRIPTION: This function interprets a numerical
         constant.  Added by JBV 10/95
  
***************************************************************/

BasicNumberType
inp_numconst(char *expression, int *ResultCode)
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

   /* Expression stack stuff */
   /* char type; */
   BasicNumberType nval;
   char            string[BasicStringLengthMax + 1];
   int             pos_adv;

   /* initialize the variable if necessary */

   bwx_DEBUG(__FUNCTION__);


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
   nval = 0;

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
      *ResultCode = RESULT_TYPE_MMISMATCH;
      return 0;
   }

   /* now build the mantissa according to the numerical base */

   switch (base)
   {

   case 10:    /* decimal constant */

      /* initialize counters */

      pos_adv = man_start;
      /* type = NUMBER; */
      string[0] = '\0';
      s_pos = 0;
      exponent = 0;
      build_loop = TRUE;

      /* loop to build the string */

      while (build_loop == TRUE)
      {
         switch (expression[pos_adv])
         {
         case '-':   /* prefixed plus or minus */
         case '+':

            /* in the first position, a plus or minus
             * sign can be added to the beginning of the
             * string to be scanned */

            if (pos_adv == man_start)
            {
               string[s_pos] = expression[pos_adv];
               ++pos_adv;  /* advance to next
                      * character */
               ++s_pos;
               string[s_pos] = '\0';
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
            string[s_pos] = expression[pos_adv];
            ++pos_adv;  /* advance to next character */
            ++s_pos;
            string[s_pos] = '\0';
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
            ++pos_adv;  /* advance to next character */
            /* type = NUMBER; */
            exponent = FALSE;
            build_loop = FALSE;
            break;

         case 'E':   /* exponential, single precision */
         case 'e':
            ++pos_adv;  /* advance to next character */
            /* type = NUMBER; */
            exponent = TRUE;
            build_loop = FALSE;
            break;
         case '\0':
            build_loop = FALSE;
            break;
         default:
            /* not numeric */
            /* ERROR messages are displayed by the
             * calling routine */
            *ResultCode = RESULT_TYPE_MMISMATCH;
            return 0;
            break;
         }

      }

      /* assign the value to the mantissa variable */

      sscanf(string, BasicNumberScanFormat, var_findnval(&mantissa, mantissa.array_pos));


      /* test if integer bounds have been exceeded */

      if (TRUE /* type == NUMBER */ )
      {
         int             i;
         BasicNumberType d;

         i = (int) var_getnval(&mantissa);
         d = (BasicNumberType) i;
         if (d != var_getnval(&mantissa))
         {
            /* type = NUMBER; */
         }
      }
      /* read the exponent if there is one */

      if (exponent == TRUE)
      {

         /* allow a plus or minus once at the beginning */

         need_pm = TRUE;

         /* initialize counters */

         string[0] = '\0';
         s_pos = 0;
         build_loop = TRUE;

         /* loop to build the string */

         while (build_loop == TRUE)
         {
            switch (expression[pos_adv])
            {
            case '-':   /* prefixed plus or minus */
            case '+':

               if (need_pm == TRUE) /* only allow once */
               {
                  string[s_pos] = expression[pos_adv];
                  ++pos_adv;  /* advance to next
                         * character */
                  ++s_pos;
                  string[s_pos] = '\0';
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

               string[s_pos] = expression[pos_adv];
               ++pos_adv;  /* advance to next
                      * character */
               ++s_pos;
               string[s_pos] = '\0';
               need_pm = FALSE;
               break;

            default: /* anything else, terminate */
               build_loop = FALSE;
               break;
            }

         }  /* end of build loop for exponent */

         /* assign the value to the user variable */

         sscanf(string, BasicNumberScanFormat, &nval);


      }     /* end of exponent search */
      if (nval == 0)
      {
         nval = var_getnval(&mantissa);
      }
      else
      {
         nval = var_getnval(&mantissa)
            * pow(10.0, nval);
      }

      break;

   case 8:     /* octal constant */

      /* initialize counters */

      pos_adv = man_start;
      /* type = NUMBER; */
      string[0] = '\0';
      s_pos = 0;
      exponent = 0;
      build_loop = TRUE;

      /* loop to build the string */

      while (build_loop == TRUE)
      {
         switch (expression[pos_adv])
         {
         case '0':   /* or ordinary digit */
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
            string[s_pos] = expression[pos_adv];
            ++pos_adv;  /* advance to next character */
            ++s_pos;
            string[s_pos] = '\0';
            break;

         default: /* anything else, terminate */
            build_loop = FALSE;
            break;
         }

      }

      /* now scan the string to determine the number */

      sscanf(string, "%o", &u);
      nval = u;

      break;

   case 16:    /* hexadecimal constant */

      /* initialize counters */

      pos_adv = man_start;
      /* type = NUMBER; */
      string[0] = '\0';
      s_pos = 0;
      exponent = 0;
      build_loop = TRUE;

      /* loop to build the string */

      while (build_loop == TRUE)
      {
         switch (expression[pos_adv])
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
            string[s_pos] = expression[pos_adv];

            ++pos_adv;  /* advance to next character */
            ++s_pos;
            string[s_pos] = '\0';
            break;

         default: /* anything else, terminate */
            build_loop = FALSE;
            break;
         }

      }

      /* now scan the string to determine the number */

      sscanf(string, "%x", &u);
      nval = u;
      break;
   }


   /* check Value */
   if (isnan(nval))
   {
      nval = 0;
      /* ERROR messages are displayed by the calling routine */
      *ResultCode = RESULT_TYPE_MMISMATCH;
   }
   else
   if (isinf(nval))
   {
      if (nval < 0)
      {
         nval = -(DBL_MAX);   /* FLT_MAX */
      }
      else
      {
         nval = (DBL_MAX); /* FLT_MAX */
      }
      /* ERROR messages are displayed by the calling routine */
      *ResultCode = RESULT_ARITHMETIC_OVERFLOW;
   }
   else
   {
      *ResultCode = RESULT_OK;
   }

   return nval;

}

/* EOF */
