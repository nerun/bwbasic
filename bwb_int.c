/***************************************************************f
  
        bwb_int.c       Line Interpretation Routines
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
  
        FUNCTION:       adv_element()
  
        DESCRIPTION:    This function reads characters in <buffer>
                        beginning at <pos> and advances past a
                        line element, incrementing <pos> appropri-
                        ately and returning the line element in
                        <element>.
  
***************************************************************/

int
adv_element(char *buffer, int *pos, char *element)
{
   int             loop;   /* control loop */
   int             e_pos;  /* position in element buffer */
   int             str_const; /* boolean: building a string
                * constant */

   bwx_DEBUG(__FUNCTION__);

   /* advance beyond any initial whitespace */

   adv_ws(buffer, pos);


   /* now loop while building an element and looking for an element
    * terminator */

   loop = TRUE;
   e_pos = 0;
   element[e_pos] = '\0';
   str_const = FALSE;

   while (loop == TRUE)
   {

      if (buffer[*pos] == OptionCommentChar)
      {
         if (str_const == TRUE)
         {
            element[e_pos] = buffer[*pos];
            ++e_pos;
            ++(*pos);
            element[e_pos] = '\0';
         }
         else
         {
            return TRUE;
         }
      }
      switch (buffer[*pos])
      {
      case ',':   /* element terminators */
      case ';':
      case '=':
      case ' ':
         /* case '\0': *//* Removed by JBV (found by DD) */
         if (str_const == TRUE)
         {
            element[e_pos] = buffer[*pos];
            ++e_pos;
            ++(*pos);
            element[e_pos] = '\0';
         }
         else
         {
            return TRUE;
         }
         break;

      case '\0':  /* Added by JBV (found by DD) */
         if (str_const == TRUE)  /* termination of string
                   * constant */
         {
            element[e_pos] = '\"';
            element[++e_pos] = '\0';
         }
         return TRUE;
         break;

      case '\"':  /* string constant */
         element[e_pos] = buffer[*pos];
         ++e_pos;
         ++(*pos);
         element[e_pos] = '\0';
         if (str_const == TRUE)  /* termination of string
                   * constant */
         {
            return TRUE;
         }
         else  /* beginning of string constant */
         {
            str_const = TRUE;
         }
         break;

      default:
         element[e_pos] = buffer[*pos];
         ++e_pos;
         ++(*pos);
         element[e_pos] = '\0';
         break;
      }
   }

   /* This should not happen */

   return FALSE;

}

/***************************************************************
  
        FUNCTION:       adv_ws()
  
        DESCRIPTION:    This function reads characters in <buffer>
                        beginning at <pos> and advances past any
                        whitespace, incrementing <pos> appropri-
                        ately.
  
***************************************************************/

int
adv_ws(char *buffer, int *pos)
{
   bwx_DEBUG(__FUNCTION__);

   while (buffer[*pos] == ' ')
      (*pos)++;
   return FALSE;
}

/***************************************************************
  
   FUNCTION:       adv_eos()
  
   DESCRIPTION:    This function reads characters in <buffer>
         beginning at <pos> and advances to the
         end of a segment delimited by '\0',
         incrementing <pos> appropriately.
  
***************************************************************/

int
adv_eos(char *buffer, int *pos)
{
   bwx_DEBUG(__FUNCTION__);

   while (buffer[*pos] != '\0')
      (*pos)++;
   return FALSE;
}


/***************************************************************
  
        FUNCTION:       line_start()
  
        DESCRIPTION:    This function reads a line buffer in
                        <buffer> beginning at the position
                        <pos> and attempts to determine (a)
                        the position of the line number in the
                        buffer (returned in <lnpos>), (b) the
                        line number at this position (returned
                        in <lnum>), (c) the position of the
                        BASIC command in the buffer (returned
                        in <cmdpos>), (d) the position of this
                        BASIC command in the command table
                        (returned in <cmdnum>), and (e) the
                        position of the beginning of the rest
                        of the line (returned in <Startpos>).
                        Although <Startpos> must be returned
                        as a positive integer, the other
                        searches may fail, in which case FALSE
                        will be returned in their positions.
                        <pos> is not incremented.
  
***************************************************************/


int
line_start(struct bwb_line * l)
{
   int             position;
   char            tbuf[BasicStringLengthMax + 1];


   bwx_DEBUG(__FUNCTION__);


   /* set initial values */
   l->Startpos = 0;
   l->cmdnum = 0;    /* not found */
   position = 0;

   adv_ws(l->buffer, &position);

   /* handle special cases */
   if (l->buffer[position] == '\0')
   {
      l->cmdnum = C_REM;
      return TRUE;
   }
   if (l->buffer[position] == OptionCommentChar)
   {
      l->cmdnum = C_REM;
      return TRUE;
   }
   if (strncasecmp(&l->buffer[position], "REM", 3) == 0)
   {
      l->cmdnum = C_REM;
      return TRUE;
   }
   if (l->buffer[position] == '?')
   {
      l->cmdnum = C_QUEST;
      l->Startpos = position;
      l->Startpos++;
      return TRUE;
   }
   
   /* advance beyond the first element */
   scan_element(l->buffer, &position, tbuf);
   adv_ws(l->buffer, &position);

   if (is_label(tbuf) == TRUE)
   {
      l->cmdnum = C_USER_LBL; /* bwb_USER_LBL */
      adv_ws(l->buffer, &position);
      l->Startpos = position;
   }
   else
   if ((l->cmdnum = is_cmd(tbuf)) > 0)
   {
      /* * NOTE:  This is NOT a full parser, this exists only to *
       * handle STRUCTURED commands. It is true that we also handle *
       * some other easy cases, but remember that this only exists *
       * to support STRUCTURED commands. Whether any other commands *
       * get processed here is simply because it was easy to do so. */
      adv_ws(l->buffer, &position);
      l->Startpos = position;
      if (l->cmdnum == C_CASE)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* syntax error */
            l->cmdnum = 0;
         }
         else
         if (strcasecmp(tbuf, "ELSE") == 0)
         {
            l->cmdnum = C_CASE_ELSE;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "IF") == 0)
         {
            l->cmdnum = C_CASE_IF;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "IS") == 0)
         {
            l->cmdnum = C_CASE_IS;
            l->Startpos = position;
         }
         else
         {
            /* CASE ... */
         }
      }
      else
      if (l->cmdnum == C_DO)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* plain DO */
         }
         else
         if (strcasecmp(tbuf, "UNTIL") == 0)
         {
            l->cmdnum = C_DO_UNTIL;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "WHILE") == 0)
         {
            l->cmdnum = C_DO_WHILE;
            l->Startpos = position;
         }
         else
         {
            /* syntax error */
         }
      }
      else
      if (l->cmdnum == C_END)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* plain END */
         }
         else
         if (strcasecmp(tbuf, "FUNCTION") == 0)
         {
            l->cmdnum = C_END_FUNCTION;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "IF") == 0)
         {
            l->cmdnum = C_END_IF;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "SELECT") == 0)
         {
            l->cmdnum = C_END_SELECT;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "SUB") == 0)
         {
            l->cmdnum = C_END_SUB;
            l->Startpos = position;
         }
         else
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_EXIT)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* syntax error */
            l->cmdnum = 0;
         }
         else
         if (strcasecmp(tbuf, "DO") == 0)
         {
            l->cmdnum = C_EXIT_DO;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "FOR") == 0)
         {
            l->cmdnum = C_EXIT_FOR;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "FUNCTION") == 0)
         {
            l->cmdnum = C_EXIT_FUNCTION;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "SUB") == 0)
         {
            l->cmdnum = C_EXIT_SUB;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "UNTIL") == 0)
         {
            l->cmdnum = C_EXIT_UNTIL;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "WHILE") == 0)
         {
            l->cmdnum = C_EXIT_WHILE;
            l->Startpos = position;
         }
         else
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_GO)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* syn tax 
             *
             * err or */
            l->cmdnum = 0;
         }
         else
         if (strcasecmp(tbuf, "SUB") == 0)
         {
            l->cmdnum = C_GOSUB;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "TO") == 0)
         {
            l->cmdnum = C_GOTO;
            l->Startpos = position;
         }
         else
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_IF)
      {
         /* sep era te cla ssi c IF fro m str uct ued 
          *
          * IF */
         if (GetKeyword(l, NULL, " GOTO "))
         {
            /* C L A S S I C */
            /* I F 
             *
             * .. . 
             *
             * GO T O 
             *
             * .. . */
         }
         else
         if (GetKeyword(l, NULL, " ELSE "))
         {
            /* C L A S S I C */
            /* I F 
             *
             * .. . 
             *
             * GO T O 
             *
             * .. . 
             *
             * EL S E 
             *
             * .. . */
            /* I F 
             *
             * .. . 
             *
             * TH E N 
             *
             * .. . 
             *
             * EL S E 
             *
             * .. . */
         }
         else
         if (IsLastKeyword(l, " THEN") == TRUE)
         {
            /* S T R U C T U R E D */
            /* I F 
             *
             * .. . 
             *
             * TH E N */
            l->cmdnum = C_IF_THEN;
         }
         else
         {
            /* C L A S S I C */
            /* I F 
             *
             * .. . 
             *
             * TH E N 
             *
             * .. . */
         }
      }
      else
      if (l->cmdnum == C_LOOP)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* p l a i n 
             *
             * LO O P */
         }
         else
         if (strcasecmp(tbuf, "UNTIL") == 0)
         {
            l->cmdnum = C_LOOP_UNTIL;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "WHILE") == 0)
         {
            l->cmdnum = C_LOOP_WHILE;
            l->Startpos = position;
         }
         else
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_ON)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* O N 
             *
             * .. . 
             *
             * GO . . . */
         }
         else
         if (strcasecmp(tbuf, "ERROR") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "GOTO") == 0)
            {
               l->cmdnum = C_ON_ERROR_GOTO;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "GOSUB") == 0)
            {
               l->cmdnum = C_ON_ERROR_GOSUB;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "RESUME") == 0)
            {
                scan_element(l->buffer, &position, tbuf);
                adv_ws(l->buffer, &position);
                if (strlen(tbuf) == 0)
                {
                   /* s y n t a x 
                    *
                    * er r o r */
                   l->cmdnum = 0;
                }
                else
                if (strcasecmp(tbuf, "NEXT") == 0)
                {
                   l->cmdnum = C_ON_ERROR_RESUME_NEXT;
                   l->Startpos = position;
                }
                else
                {
                   /* s y n t a x 
                    *
                    * er r o r */
                   l->cmdnum = 0;
                }
            }
            else
            if (strcasecmp(tbuf, "RETURN") == 0)
            {
                scan_element(l->buffer, &position, tbuf);
                adv_ws(l->buffer, &position);
                if (strlen(tbuf) == 0)
                {
                   /* s y n t a x 
                    *
                    * er r o r */
                   l->cmdnum = 0;
                }
                else
                if (strcasecmp(tbuf, "NEXT") == 0)
                {
                   l->cmdnum = C_ON_ERROR_RETURN_NEXT;
                   l->Startpos = position;
                }
                else
                {
                   /* s y n t a x 
                    *
                    * er r o r */
                   l->cmdnum = 0;
                }
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "TIMER") == 0)
         {
            l->cmdnum = C_ON_TIMER;
            l->Startpos = position;
         }
         else
         {
            /* O N 
             *
             * .. . 
             *
             * GO . . . */
         }
      }
      else
      if (l->cmdnum == C_OPEN)
      {
         /* s e p e r a t e 
          *
          * cl a s s i c 
          *
          * OP E N 
          *
          * fr o m 
          *
          * st r u c t u e d 
          *
          * OP E N */
         if (GetKeyword(l, NULL, " AS "))
         {
            /* S T R U C T U R E D */
         }
         else
         {
            /* C L A S S I C */
            l->cmdnum = C_DEF_SUB;
            l->Startpos = 0;
         }
      }
      else
      if (l->cmdnum == C_OPTION)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
         else
         if (strcasecmp(tbuf, "ANGLE") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "DEGREES") == 0)
            {
               l->cmdnum = C_OPTION_ANGLE_DEGREES;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "RADIANS") == 0)
            {
               l->cmdnum = C_OPTION_ANGLE_RADIANS;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "ARITHMETIC") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "DECIMAL") == 0)
            {
               l->cmdnum = C_OPTION_ARITHMETIC_DECIMAL;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "FIXED") == 0)
            {
               l->cmdnum = C_OPTION_ARITHMETIC_FIXED;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "NATIVE") == 0)
            {
               l->cmdnum = C_OPTION_ARITHMETIC_NATIVE;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "BASE") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "0") == 0)
            {
               l->cmdnum = C_OPTION_BASE_0;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "1") == 0)
            {
               l->cmdnum = C_OPTION_BASE_1;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "BUGS") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_BUGS_OFF;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_BUGS_ON;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "COMMENT") == 0)
         {
            l->cmdnum = C_OPTION_COMMENT;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "COMPARE") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "BINARY") == 0)
            {
               l->cmdnum = C_OPTION_COMPARE_BINARY;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "DATABASE") == 0)
            {
               l->cmdnum = C_OPTION_COMPARE_DATABASE;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "TEXT") == 0)
            {
               l->cmdnum = C_OPTION_COMPARE_TEXT;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "COVERAGE") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_COVERAGE_OFF;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_COVERAGE_ON;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "DATE") == 0)
         {
            l->cmdnum = C_OPTION_DATE;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "DISABLE") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "COMMAND") == 0)
            {
               l->cmdnum = C_OPTION_DISABLE_COMMAND;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "FUNCTION") == 0)
            {
               l->cmdnum = C_OPTION_DISABLE_FUNCTION;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "OPERATOR") == 0)
            {
               l->cmdnum = C_OPTION_DISABLE_OPERATOR;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "ENABLE") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "COMMAND") == 0)
            {
               l->cmdnum = C_OPTION_ENABLE_COMMAND;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "FUNCTION") == 0)
            {
               l->cmdnum = C_OPTION_ENABLE_FUNCTION;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "OPERATOR") == 0)
            {
               l->cmdnum = C_OPTION_ENABLE_OPERATOR;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "ERROR") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "GOSUB") == 0)
            {
               l->cmdnum = C_OPTION_ERROR_GOSUB;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "GOTO") == 0)
            {
               l->cmdnum = C_OPTION_ERROR_GOTO;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "INDENT") == 0)
         {
            l->cmdnum = C_OPTION_INDENT;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "LABELS") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_LABELS_OFF;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_LABELS_ON;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "STATEMENT") == 0)
         {
            l->cmdnum = C_OPTION_STATEMENT;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "STRICT") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_STRICT_OFF;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_STRICT_ON;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "TERMINAL") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "NONE") == 0)
            {
               l->cmdnum = C_OPTION_TERMINAL_NONE;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "ADM-3A") == 0)
            {
               l->cmdnum = C_OPTION_TERMINAL_ADM_3A;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "ANSI") == 0)
            {
               l->cmdnum = C_OPTION_TERMINAL_ANSI;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "TIME") == 0)
         {
            l->cmdnum = C_OPTION_TIME;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "TRACE") == 0)
         {
            scan_element(l->buffer, &position, tbuf);
            adv_ws(l->buffer, &position);
            if (strlen(tbuf) == 0)
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
            else
            if (strcasecmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_TRACE_OFF;
               l->Startpos = position;
            }
            else
            if (strcasecmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_TRACE_ON;
               l->Startpos = position;
            }
            else
            {
               /* s y n t a x 
                *
                * er r o r */
               l->cmdnum = 0;
            }
         }
         else
         if (strcasecmp(tbuf, "VERSION") == 0)
         {
            l->cmdnum = C_OPTION_VERSION;
            l->Startpos = position;
         }
         else
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_SELECT)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
         else
         if (strcasecmp(tbuf, "CASE") == 0)
         {
            l->cmdnum = C_SELECT_CASE;
            l->Startpos = position;
         }
         else
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_TIMER)
      {
         position = l->Startpos;
         scan_element(l->buffer, &position, tbuf);
         adv_ws(l->buffer, &position);
         if (strlen(tbuf) == 0)
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
         else
         if (strcasecmp(tbuf, "OFF") == 0)
         {
            l->cmdnum = C_TIMER_OFF;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "ON") == 0)
         {
            l->cmdnum = C_TIMER_ON;
            l->Startpos = position;
         }
         else
         if (strcasecmp(tbuf, "STOP") == 0)
         {
            l->cmdnum = C_TIMER_STOP;
            l->Startpos = position;
         }
         else
         {
            /* s y n t a x 
             *
             * er r o r */
            l->cmdnum = 0;
         }
      }
      /* */
      switch (l->cmdnum)
      {
      case C_DEF:
      case C_FUNCTION:
      case C_SUB:
      case C_USER_LBL:
         fslt_add(l);
         break;
      }
      /* */
   }
   else
   if (is_let(l->buffer) == TRUE)
   {
      /* this is an implied LET, such as A = ... */
      l->cmdnum = C_LET;   /* bwb_LET */
      l->Startpos = 0;
   }
   else
   if (fnc_find(tbuf) != NULL)
   {
      /* user is executing a function, such as 100 COS X */
      l->cmdnum = C_DEF_SUB;  /* bwb_DEF_SUB, in bwb_tbl.c */
      l->Startpos = 0;
   }
   /* return */

   return TRUE;

}


/***************************************************************
  
   FUNCTION:       scan_element()
  
        DESCRIPTION:    This function reads characters in <buffer>
                        beginning at <pos> and advances past a
                        line element, incrementing <pos> appropri-
                        ately and returning the line element in
         <element>.
  
         This function stops at first non-alnum.  
  
***************************************************************/

int
scan_element(char *buffer, int *pos, char *element)
{
   /* HERE-Here-here */
   
   char           *C;
   char           *E;
   int             p;

   p = *pos;
   C = buffer;
   C += p;
   E = element;

   while (*C == ' ')
   {
      C++;
   }
   if (isalnum(*C))
   {
      if (OptionFlags & OPTION_BUGS_ON)
      {
         while (isalnum(*C) || *C == '_' || *C == '.')
         {
            *E = *C;
            E++;
            C++;
         }
      }
      else
      {
         while (isalnum(*C))
         {
            *E = *C;
            E++;
            C++;
         }
      }
      if( *C == OptionStatementChar 
      ||  *C == BasicStringSuffix 
      ||  *C == '*'
      )
      {
         /* LABEL: */
         /* MID$   */
         /* CLOAD* */
         *E = *C;
         E++;
         C++;
      }
   }
   while (*C == ' ')
   {
      C++;
   }
   *E = '\0';
   p = C - buffer;
   *pos = p;
   return TRUE;

#if 0


   int             loop;   /* control loop */
   int             e_pos;  /* position in element buffer */
   int             str_const; /* boolean: building a string
                * constant */


   bwx_DEBUG(__FUNCTION__);


   /* advance beyond any initial whitespace */
   adv_ws(buffer, pos);


   /* now loop while building an element and looking for an element
    * terminator */

   loop = TRUE;
   e_pos = 0;
   element[e_pos] = '\0';
   str_const = FALSE;

   while (loop == TRUE)
   {

      if (buffer[*pos] == OptionCommentChar)
      {
         return TRUE;
      }
      switch (buffer[*pos])
      {
      case ',':   /* element terminators */
      case ';':
      case '=':
      case ' ':
      case '\0':
      case '\"':  /* string constant */
      case '#':
         if (str_const == TRUE)
         {
            element[e_pos] = buffer[*pos];
            ++e_pos;
            ++(*pos);
            element[e_pos] = '\0';
         }
         else
         {
            return TRUE;
         }
         break;
#if 0
      case '\"':  /* string constant */
         element[e_pos] = buffer[*pos];
         ++e_pos;
         ++(*pos);
         element[e_pos] = '\0';
         if (str_const == TRUE)  /* termination of string
                   * constant */
         {
            return TRUE;
         }
         else  /* beginning of string constant */
         {
            str_const = TRUE;
         }
         break;
#endif
      case '(':   /* MID$ command termination (JBV) */
         /* If MID$ is here, get out */
         if (strcasecmp(element, "MID$") == 0)
         {
            return TRUE;
         }
         /* else add it to the accumulation element */
         element[e_pos] = buffer[*pos];
         ++e_pos;
         ++(*pos);
         element[e_pos] = '\0';
         break;

      default:
         element[e_pos] = buffer[*pos];
         ++e_pos;
         ++(*pos);
         element[e_pos] = '\0';
         break;
      }
   }

   /* This should not happen */

   return FALSE;
#endif

}

/***************************************************************
  
        FUNCTION:       is_cmd()
  
   DESCRIPTION:    This function determines whether the
         string in 'buffer' is a BASIC command
         statement, returning 'id' or 0.
         
  
***************************************************************/




int
is_cmd(char *buffer)
{
   register int    n;

   bwx_DEBUG(__FUNCTION__);


   /* Go through the command table and search for a match. */

   for (n = 0; n < NUM_COMMANDS; ++n)
   {
      if (OptionVersion & bwb_cmdtable[n].OptionVersionBitmask)
         if (strcasecmp(bwb_cmdtable[n].name, buffer) == 0)
         {
            return bwb_cmdtable[n].id;
         }
   }
   /* No command name was found */
   return 0;

}

/***************************************************************
  
        FUNCTION:       is_let()
  
        DESCRIPTION:    This function tries to determine if the
                        expression in <buffer> is a LET statement
                        without the LET command specified.
  
***************************************************************/
int
is_let(char *buffer)
{
   register int    n;

   bwx_DEBUG(__FUNCTION__);

   /* Go through the expression and search for an unquoted assignment operator. */

   for (n = 0; buffer[n] != '\0'; ++n)
   {
      switch (buffer[n])
      {
      case '\"':  /* string constant */
         ++n;
         while (buffer[n] != '\"')
         {
            ++n;
            if (buffer[n] == '\0')
            {
               sprintf(bwb_ebuf, "Incomplete string constant");
               bwb_error(bwb_ebuf);
               return FALSE;
            }
         }
         ++n;
         break;
      case '=':
         return TRUE;
      }
   }

   /* No command name was found */

   return FALSE;

}

/***************************************************************
  
   FUNCTION:       is_label()
  
   DESCRIPTION:    This function determines whether the string
         pointed to by 'buffer' is a label (i.e.,
         ends with colon).
  
***************************************************************/

int
is_label(char *buffer)
{
   bwx_DEBUG(__FUNCTION__);


   if (OptionFlags & OPTION_LABELS_ON)
   {
       /* labels are possible */
       if (buffer[strlen(buffer) - 1] == OptionStatementChar)
       {
          return TRUE;
       }
   }
   return FALSE;

}


/***************************************************************
  
        FUNCTION:       bwb_stripcr()
  
   DESCRIPTION:    This function strips the carriage return
         or line-feed from the end of a string.
  
***************************************************************/

int
bwb_stripcr(char *s)
{
   bwx_DEBUG(__FUNCTION__);

   while (*s)
   {
      if (*s < ' ')
         *s = ' ';
      s++;
   }
   return TRUE;
}

/***************************************************************
  
        FUNCTION:       is_numconst()
  
        DESCRIPTION:    This function reads the string in <buffer>
                        and returns TRUE if it is a numerical
                        constant and FALSE if it is not. At
                        this point, only decimal (base 10)
                        constants are detected.
  
***************************************************************/

int
is_numconst(char *buffer)
{
   char           *p;

   bwx_DEBUG(__FUNCTION__);

   /* Return FALSE for empty buffer */

   if (buffer[0] == '\0')
   {
      return FALSE;
   }
   /* else check digits */

   p = buffer;
   while (*p != '\0')
   {
      switch (*p)
      {
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
         break;
      default:
         return FALSE;
      }
      ++p;
   }

   /* only numerical characters detected */

   return TRUE;

}

/***************************************************************
  
        FUNCTION:       bwb_numseq()
  
   DESCRIPTION:    This function reads in a sequence of
         numbers (e.g., "10-120"), returning
         the first and last numbers in the sequence
         in the integers pointed to by 'start' and
         'end'.
  
***************************************************************/

int
bwb_numseq(char *buffer, BasicLineNumberType * start, BasicLineNumberType * end)
{
/*  This is used by the BASIC commands LIST and DELETE
Result   Example
======   ===============
0  LIST
1  LIST 1000
4  LIST     -
5  LIST 1000-
6  LIST     -9000
7  LIST 1000-9000
*/
   int             Result;


   register int    b, n;
   int             numbers;
   static char    *tbuf;
   static int      init = FALSE;

   bwx_DEBUG(__FUNCTION__);

   Result = 0;
   /* get memory for temporary buffer if necessary */

   if (init == FALSE)
   {
      init = TRUE;

      /* Revised to CALLOC pass-thru call by JBV */
      if ((tbuf = CALLOC(BasicStringLengthMax + 1, sizeof(char), "bwb_numseq")) == NULL)
      {
         bwb_error("in bwb_numseq(): failed to find memory for tbuf");
         return Result;
      }
   }
/* The CALLING function sets the DEFAULT values
   *start = 0;
   *end = 0;
*/
   while (*buffer == ' ')
      buffer++;

   if (*buffer == '\0')
   {
      return Result;
   }
   numbers = n = b = 0;
   tbuf[0] = 0;

   while (TRUE)
   {
      switch (buffer[b])
      {
      case '\0':  /* end of string */
         if (n > 0)
         {
            if (numbers == 0)
            {
               *start = atoi(tbuf);
               Result |= 1;
            }
            else
            {
               *end = atoi(tbuf);
               Result |= 2;
            }
         }
         ++numbers;
         return Result;

      case ' ':   /* whitespace */
         ++b;
         break;

      case '-':   /* or skip to next number */
         Result |= 4;
         if (n > 0)
         {
            if (numbers == 0)
            {
               *start = atoi(tbuf);
               Result |= 1;
            }
            else
            {
               *end = atoi(tbuf);
               Result |= 2;
            }
         }
         ++numbers;
         ++b;
         n = 0;
         break;
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
         tbuf[n] = buffer[b];
         ++n;
         tbuf[n] = 0;
         ++b;
         break;
      default:
         sprintf(bwb_ebuf,
            "ERROR: character <%c> unexpected in numerical sequence",
            buffer[b]);
         ++b;
         bwb_error(bwb_ebuf);
         return Result;
         break;
      }
   }


   return Result;

}

/***************************************************************
  
        FUNCTION:       bwb_freeline()
  
   DESCRIPTION:    This function frees memory associated
         with a program line in memory.
  
***************************************************************/

int
bwb_freeline(struct bwb_line * l)
{

   bwx_DEBUG(__FUNCTION__);

   /* free arguments if there are any */

   /* Revised to FREE pass-thru calls by JBV */
   if (l->buffer != NULL)
   {
      FREE(l->buffer, "bwb_freeline");
      l->buffer = NULL; /* JBV */
   }
   FREE(l, "bwb_freeline");
   /* l = NULL; *//* Assignment of function parameter has no effect
    * outside the function. */

   return TRUE;
}


/***************************************************************
  
   FUNCTION:       is_eol()
  
   DESCRIPTION:    This function determines whether the buffer
         is at the end of a line.
  
***************************************************************/

int
is_eol(char *buffer, int *position)
{
   char            c;

   bwx_DEBUG(__FUNCTION__);


   adv_ws(buffer, position);
   c = buffer[*position];


   if (c == '\0' || c == OptionCommentChar)
   {
      return TRUE;
   }
   return FALSE;
}

char           *
GetRestOfLine(struct bwb_line * l, char *OutputBuffer)
{
   char           *S;
   char           *C;

   bwx_DEBUG(__FUNCTION__);


   S = l->buffer;
   S += l->position;
   C = S;
   while (*C != '\0' && *C != OptionCommentChar)
   {
      C++;
   }
   /* FOUND */
   if (OutputBuffer != NULL)
   {
      char            c;

      c = *C;
      *C = '\0';
      l->position += strlen(S);
      strcpy(OutputBuffer, S);
      *C = c;
   }
   return C;
}


char           *
GetKeyword(struct bwb_line * l, char *OutputBuffer, char *Keyword)
{
   /* if OutputBuffer is NULL then we just want to know if a keyword
    * exists, and if so where to look after the keyword. this is used by
    * line_start() in bwb_int.c to determine various command flavors.
    * else we want to extract the expression before the keyword and
    * update the position to after the keyword. this is used by various
    * bwb_XXX() when executing BASIC commands. end if 
    *
    * if FOUND then if OutputBuffer != NULL then copy line's buffer BEFORE
    * keyword into OutputBuffer update line's position to AFTER keyword
    * end if return pointer AFTER keyword else return NULL end if    */
   char           *S;
   char           *C;
   int             n;

   bwx_DEBUG(__FUNCTION__);

   S = l->buffer;
   S += l->position;
   C = S;
   n = strlen(Keyword);


   while (*C != '\0' && *C != OptionCommentChar)
   {
      if (strncasecmp(C, Keyword, n) == 0)
      {
         /* FOUND */
         char            c;

         c = *C;
         *C = '\0';
         if (OutputBuffer != NULL)
         {
            l->position += strlen(S);
            l->position += n;
            strcpy(OutputBuffer, S);
         }
         *C = c;
         C += n;
         return C;
      }
      else
      if (*C == '\"')
      {
         /* skip string constant */
         C++;
         while (*C != '\0' && *C != '\"')
         {
            C++;
         }
         if (*C == '\"')
         {
            C++;
         }
      }
      else
      {
         /* skip normal character */
         C++;
      }
   }
   /* NOT FOUND */
   return NULL;
}

int
IsLastKeyword(struct bwb_line * l, char *Keyword)
{

   /* find the end of the line ( 0 or ' ) */
   /* backup thru spaces */
   int             n;
   char           *S;
   char           *C;

   bwx_DEBUG(__FUNCTION__);


   S = l->buffer;
   S += l->position;
   C = S;
   n = strlen(Keyword);
   S += n;
   /* IF x THEN0 IF x THEN'0 IF    x    THEN    '     0 */
   while (*C != '\0' && *C != OptionCommentChar)
      C++;
   if (C > S)
   {
      C--;
      while (C > S && *C == ' ')
         C--;
      C++;
      if (C > S)
      {
         C -= n;
         if (strncasecmp(C, Keyword, n) == 0)
         {
            /* FOUND */
            return TRUE;
         }
      }
   }
   /* NOT FOUND */
   return FALSE;
}

int
ToUpper(int c)
{
   /* * I have lost count of the number of C * libraries that implement
    * toupper() * as "return (c+'A'-'a');" */
   if (islower(c))
   {
      return toupper(c);
   }
   return c;
}

int
ToLower(int c)
{
   /* * I have lost count of the number of C * libraries that implement
    * tolower() * as "return (c+'a'-'A');" */
   if (isupper(c))
   {
      return tolower(c);
   }
   return c;
}

#if HAVE_ISNAN

/* these are intrinsic C functions in my environment using -ansi */

#else          /* ! HAVE_ISNAN */

int
isnan(double X)
{
   return FALSE;
}

int
isinf(double X)
{
   return FALSE;
}

double
rint(double x)
{
   return (x >= 0.0) ? floor(x + 0.5) : ceil(x - 0.5);
}

int
strcasecmp(const char *s1, const char *s2)
{
   const unsigned char *p1;
   const unsigned char *p2;

   p1 = (const unsigned char *) s1;
   p2 = (const unsigned char *) s2;
   while (*p1 != '\0')
   {
      char            c1;
      char            c2;
      c1 = ToUpper(*p1);
      c2 = ToUpper(*p2);
      if (c1 < c2)
         return (-1);
      else
      if (c1 > c2)
         return (1);
      p1++;
      p2++;
   }
   if (*p2 == '\0')
      return (0);
   else
      return (-1);
}

int
strncasecmp(const char *s1, const char *s2, size_t n)
{
   const unsigned char *p1;
   const unsigned char *p2;
   size_t          x = 0;

   p1 = (const unsigned char *) s1;
   p2 = (const unsigned char *) s2;
   while (x < n)
   {
      char            c1;
      char            c2;
      c1 = ToUpper(p1[x]);
      c2 = ToUpper(p2[x]);
      if (c1 < c2)
         return (-1);
      else
      if (c1 > c2)
         return (1);
      else
      if (c1 == '\0')
         return (0);
      x++;
   }
   return (0);
}

#endif            /* ! HAVE_ISNAN */


#if BROKEN_STRCPY
extern void 
broken_strcpy( char * Target, char * Source )
{
    char C;
    
    do
    {
        C = *Source;
        *Target = C;
        Source++;
        Target++;
    }
    while( C != 0 );
}
#endif /* BROKEN_STRCPY */



/* EOF */
