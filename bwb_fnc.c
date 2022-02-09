/****************************************************************
  
        bwb_fnc.c       Interpretation Routines
                        for Predefined Functions
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


#ifndef RAND_MAX     /* added in v1.11 */
#define RAND_MAX  32767
#endif



static time_t   t;
static struct tm *lt;


/*  ORD() Table 1    */

 /* ACRONYM */
typedef struct
{
   const int       Value;
   const char     *Name;
}               Acronym;

#define NUM_ACRONYMS (34)

Acronym         AcronymTable[NUM_ACRONYMS] =
{
 {0, "NUL"},
 {1, "SOH"},
 {2, "STX"},
 {3, "ETX"},
 {4, "EOT"},
 {5, "ENQ"},
 {6, "ACK"},
 {7, "BEL"},
 {8, "BS"},
 {9, "HT"},
 {10, "LF"},
 {11, "VT"},
 {12, "FF"},
 {13, "CR"},
 {14, "SO"},
 {15, "SI"},
 {16, "DLE"},
 {17, "DC1"},
 {18, "DC2"},
 {19, "DC3"},
 {20, "DC4"},
 {21, "NAK"},
 {22, "SYN"},
 {23, "ETB"},
 {24, "CAN"},
 {25, "EM"},
 {26, "SUB"},
 {27, "ESC"},
 {28, "FS"},
 {29, "GS"},
 {30, "RS"},
 {31, "US"},
 {32, "SP"},
 {127, "DEL"}
};

/* ... ORD() */

struct bwb_variable *
fnc_intrinsic(int argc, struct bwb_variable * argv, int unique_id)
{
   /* // this is the generic handler for all intrinsic BASIC functions */
   /* BasicStringLengthMax must be <= INT_MAX */
   struct bwb_function *f;
   struct bwb_variable *argn;
   unsigned char   IsError;/* for ERROR messages */
   /* Follow the BASIC naming conventions, so the code is easier to
    * maintain */
   char           *S;   /* S$ - STRING functions */
   BasicStringLengthType s;/* LEN( S$ ) */
   BasicNumberType N;   /* N  -  NUMBER functions */
   char           *A;   /* A$ - 1st STRING parameter */
   BasicStringLengthType a;/* LEN( A$ ) */
   char           *B;   /* B$ - 2nd STRING parameter */
   BasicStringLengthType b;/* LEN( B$ ) */
#if FALSE
   char           *C;   /* C$ - 3rd STRING parameter */
   BasicStringLengthType c;/* LEN( C$ ) */
#endif
   BasicNumberType X;   /* X  - 1st NUMBER parameter */
   BasicIntegerType x;  /* INT( X ) */
   BasicNumberType Y;   /* Y  - 2nd NUMBER parameter */
   BasicIntegerType y;  /* INT( Y ) */
#if FALSE
   BasicNumberType Z;   /* Z  - 3rd NUMBER parameter */
   BasicIntegerType z;  /* INT( Z ) */
#endif
   bwx_DEBUG(__FUNCTION__);


   /* so the following code is easier to read and maintain */
#define PARAM_NUMBER   *argn->memnum
#define PARAM_LENGTH    argn->memstr->length
#define PARAM_BUFFER    argn->memstr->sbuffer
#define RESULT_NUMBER  *argv->memnum
#define RESULT_BUFFER   argv->memstr->sbuffer
#define RESULT_LENGTH   argv->memstr->length

   /* no errors have occurred (yet) */
   IsError = 0;

   /* look up the intrinsic function, so we can get the details */
   {
      f = fnc_find_by_id(unique_id);
      if (f == NULL)
      {
         /* bwb_prefuncs[] in bwb_tbl.c is wrong -- this is
          * really bad */
         sprintf(bwb_ebuf, "INTERNAL ERROR in fnc_intrinsic() - did not find unique_id %d", unique_id);
         bwb_error(bwb_ebuf);
         return NULL;
      }
   }
   /* the RETURN variable is the first variable in the 'argv' vaariable
    * chain */
   if (argv == NULL)
   {
      bwb_error("INTERNAL ERROR");
      return argv;
   }
   if (argv->type == STRING)
   {
      if (argv->memstr == NULL)
      {
         bwb_error("INTERNAL ERROR");
         return argv;
      }
      RESULT_LENGTH = 0;
      RESULT_BUFFER[RESULT_LENGTH] = '\0';
   }
   else
   {
      if (argv->memnum == NULL)
      {
         bwb_error("INTERNAL ERROR");
         return argv;
      }
      RESULT_NUMBER = 0;
   }
   argn = argv;
   /* don't make a bad situation worse */
   if (ERROR_PENDING)
   {
      /* An unrecognized NON-FATAL ERROR is pending.  Just return a
       * sane value. */
      /* LET N = LOG(SQR(X)) ' X = -1 */
      return argv;
   }
   /* Follow the BASIC naming conventions, so the code is easier to read
    * and maintain */
   {
      int             i;
      int             StrCount = 0; /* count of STRING parameters
                   * - NEVER > 3 */
      int             NumCount = 0; /* count of NUMBER parameters
                   * - NEVER > 3 */
      unsigned long   ParameterTests;
      ParameterTests = f->ParameterTests;
      /* assign reasonable default values */
      S = NULL;   /* S$ - return value is a STRING */
      s = 0;      /* LEN( S$ ) */
      N = 0;      /* N  - return value is a NUMBER */
      A = NULL;   /* A$ - 1st STRING parameter */
      a = 0;      /* LEN( A$ ) */
      B = NULL;   /* B$ - 2nd STRING parameter */
      b = 0;      /* LEN( B$ ) */
#if FALSE
      C = NULL;   /* C$ - 3rd STRING parameter */
      c = 0;      /* LEN( C$ ) */
#endif
      X = 0;      /* X  - 1st NUMBER parameter */
      x = 0;      /* INT( X ) */
      Y = 0;      /* Y  - 2nd NUMBER parameter */
      y = 0;      /* INT( Y ) */
#if FALSE
      Z = 0;      /* Z  - 3rd NUMBER parameter */
      z = 0;      /* INT( Z ) */
#endif
      /* assign actual values */
      if (f->ReturnType == STRING)
      {
         S = RESULT_BUFFER;
         s = RESULT_LENGTH;
      }
      else
      {
         N = RESULT_NUMBER;
      }
      for (i = 0; i < argc && IsError == 0; i++)
      {
         argn = argn->next;
         if (argn == NULL)
         {
            bwb_error("INTERNAL ERROR");
            return argv;
         }
         if (argn->type == STRING)
         {
            if (argn->memstr == NULL)
            {
               bwb_error("INTERNAL ERROR");
               return argv;
            }
            StrCount++;
            switch (StrCount)
            {
            case 1:
               /* 1st STRING parameter = A$ */
               A = PARAM_BUFFER;
               a = PARAM_LENGTH;
               if (StringLengthCheck(ParameterTests, a))
               {
                  IsError = 'A';
               }
               else
               {
                  A[a] = 0;
               }
               break;
            case 2:
               /* 2nd STRING parameter = B$ */
               B = PARAM_BUFFER;
               b = PARAM_LENGTH;
               if (StringLengthCheck(ParameterTests, b))
               {
                  IsError = 'B';
               }
               else
               {
                  B[b] = 0;
               }
               break;
#if FALSE
            case 3:
               /* 3rd STRING parameter = C$ */
               /* not currently used */
               C = PARAM_BUFFER;
               c = PARAM_LENGTH;
               if (StringLengthCheck(ParameterTests, c))
               {
                  IsError = 'C';
               }
               else
               {
                  C[c] = 0;
               }
               break;
#endif
            default:
               /* Nth STRING parameter = ERROR */
               IsError = i + 1;
               break;
            }
         }
         else
         {
            if (argn->memnum == NULL)
            {
               bwb_error("INTERNAL ERROR");
               return argv;
            }
            NumCount++;
            switch (NumCount)
            {
            case 1:
               /* 1st NUMBER parameter = X */
               X = PARAM_NUMBER;
               if (NumberValueCheck(ParameterTests, X))
               {
                  IsError = 'X';
               }
               else
               {
                  BasicNumberType R;
                  R = rint(X);
                  if (R < INT_MIN || R > INT_MAX)
                  {
                     /* certainly not a
                      * classic BASIC
                      * integer */
                  }
                  else
                  {
                     /* Many classic BASIC
                      * intrinsic
                      * functions use the
                      * rounded integer
                      * value. */
                     x = (int) R;
                  }
               }
               break;
            case 2:
               /* 2nd NUMBER parameter = Y */
               Y = PARAM_NUMBER;
               if (NumberValueCheck(ParameterTests, Y))
               {
                  IsError = 'Y';
               }
               else
               {
                  BasicNumberType R;
                  R = rint(Y);
                  if (R < INT_MIN || R > INT_MAX)
                  {
                     /* certainly not a
                      * classic BASIC
                      * integer */
                  }
                  else
                  {
                     /* Many classic BASIC
                      * intrinsic
                      * functions use the
                      * rounded integer
                      * value. */
                     y = (int) R;
                  }
               }
               break;
#if FALSE
            case 3:
               /* 3rd NUMBER parameter = Z */
               /* not currently used */
               Z = PARAM_NUMBER;
               if (NumberValueCheck(ParameterTests, Z))
               {
                  IsError = 'Z';
               }
               else
               {
                  BasicNumberType R;
                  R = rint(Z);
                  if (R < INT_MIN || R > INT_MAX)
                  {
                     /* certainly not a
                      * classic BASIC
                      * integer */
                  }
                  else
                  {
                     /* Many classic BASIC
                      * intrinsic
                      * functions use the
                      * rounded integer
                      * value. */
                     z = (int) R;
                  }
               }
               break;
#endif
            default:
               /* Nth NUMBER parameter = ERROR */
               IsError = i + 1;
               break;
            }
         }
         ParameterTests = ParameterTests >> 4;
      }
   }


#ifndef PI
#define PI       3.14159265358979323846
#endif            /* PI */
#define MIN( X, Y ) X < Y ? X : Y;
#define MAX( X, Y ) X > Y ? X : Y;

   /* execute the intrinsic function */
   if (IsError == 0  /* WARNING -- do NOT execute a BASIC
           intrinsic function with bogus parameters */ )
      switch (unique_id)
      {
         /* ALL paramters have  been checked for TYPE MISMATCH
          * and INVALID RANGE */
         /* ONLY A HANDFUL OF ERRORS CAN OCCUR */
      case 0:
         {
            /* INTERNAL ERROR */
            IsError = 1;
         }
         break;
      case F_DEF_FN_N:
         {
            /* INTERNAL ERROR */
            IsError = 1;
         }
         break;
      case F_ARGC_N:
         /* N = ARGC */
         {
            /* determine number of parameters to the
             * current USER DEFINED FUNCTION */
            int             n;
            n = 0;
            if (CURTASK exsc >= 0)
            {
               int             Loop;
               int             i;
               Loop = TRUE;
               for (i = CURTASK exsc; i >= 0 && Loop == TRUE; i--)
               {
                  if (CURTASK excs[i].LoopTopLine != NULL)
                  {
                     switch (CURTASK excs[i].LoopTopLine->cmdnum)
                     {
                     case C_FUNCTION:
                     case C_SUB:
                        /* we have
                         * checked
                         * all the
                         * way to a
                         * FUNCTION
                         * or SUB
                         * boundary */
                        /* FOUND */
                        {
                           struct bwb_variable *v;

                           for (v = CURTASK excs[i].local_variable; v != NULL && Loop == TRUE; v = v->next)
                           {
                              n++;
                           }
                        }
                        Loop = FALSE;
                        break;
                     }
                  }
               }
            }
            n--;  /* FUNCTION or SUB name */
            N = n;
         }
         break;
      case F_ARGT_X_S:
         /* S$ = ARGT$( X ) */
         {
            /* determine parameter type to the current
             * USER DEFINED FUNCTION */
            int             Found;
            int             n;
            Found = FALSE;
            n = 0;
            s = 0;
            if (x < 1)
            {
               /* bad param number  */
            }
            else
            if (CURTASK exsc >= 0)
            {
               int             Loop;
               int             i;
               Loop = TRUE;
               for (i = CURTASK exsc; i >= 0 && Loop == TRUE; i--)
               {
                  if (CURTASK excs[i].LoopTopLine != NULL)
                  {
                     switch (CURTASK excs[i].LoopTopLine->cmdnum)
                     {
                     case C_FUNCTION:
                     case C_SUB:
                        /* we hav e
                         * che cke d
                         * all 
                         *
                         * the 
                         *
                         * way to a FUN
                         * CTI ON or
                         * SUB 
                         *
                         * boun dar y */
                        /* FOU ND */
                        {
                           struct bwb_variable *v;


                           for (v = CURTASK excs[i].local_variable; v != NULL && Loop == TRUE; v = v->next)
                           {
                              if (n == x)
                              {
                                 if (v->type == STRING)
                                 {
                                    S[0] = BasicStringSuffix;
                                    s = 1;
                                    Found = TRUE;
                                 }
                                 else
                                 {
                                    S[0] = BasicDoubleSuffix;
                                    s = 1;
                                    Found = TRUE;
                                 }
                                 Loop = FALSE;
                              }
                              n++;
                           }
                        }
                        Loop = FALSE;
                        break;
                     }
                  }
               }
            }
            if (Found == FALSE)
            {
               IsError = 'X';
            }
         }
         break;

      case F_ARGV_X_S:
         /* S$ = ARGV$( X ) */
         {
            /* determine parameter value to the current
             * USER DEFINED FUNCTION */
            int             Found;
            int             n;
            Found = FALSE;
            n = 0;
            if (x < 1)
            {
               /* bad param number  */
            }
            else
            if (CURTASK exsc >= 0)
            {
               int             Loop;
               int             i;
               Loop = TRUE;
               for (i = CURTASK exsc; i >= 0 && Loop == TRUE; i--)
               {
                  if (CURTASK excs[i].LoopTopLine != NULL)
                  {
                     switch (CURTASK excs[i].LoopTopLine->cmdnum)
                     {
                     case C_FUNCTION:
                     case C_SUB:
                        /* FOU ND */
                        {
                           struct bwb_variable *v;


                           for (v = CURTASK excs[i].local_variable; v != NULL && Loop == TRUE; v = v->next)
                           {
                              if (n == x)
                              {
                                 if (v->type == STRING)
                                 {
                                    s = v->memstr->length;
                                    memcpy(S, v->memstr->sbuffer, s);
                                    Found = TRUE;
                                 }
                                 else
                                 {
                                 }
                                 Loop = FALSE;
                              }
                              n++;
                           }
                        }
                        Loop = FALSE;
                        break;
                     }
                  }
               }
            }
            if (Found == FALSE)
            {
               IsError = 'X';
            }
         }
         break;

      case F_ARGV_X_N:
         /* S$ = ARGV( X ) */
         {
            /* determine parameter value to the current
             * USER DEFINED FUNCTION */
            int             Found;
            int             n;
            Found = FALSE;
            n = 0;
            if (x < 1)
            {
               /* bad param number  */
            }
            else
            if (CURTASK exsc >= 0)
            {
               int             Loop;
               int             i;
               Loop = TRUE;
               for (i = CURTASK exsc; i >= 0 && Loop == TRUE; i--)
               {
                  if (CURTASK excs[i].LoopTopLine != NULL)
                  {
                     switch (CURTASK excs[i].LoopTopLine->cmdnum)
                     {
                     case C_FUNCTION:
                     case C_SUB:
                        /* FOU ND */
                        {
                           struct bwb_variable *v;


                           for (v = CURTASK excs[i].local_variable; v != NULL && Loop == TRUE; v = v->next)
                           {
                              if (n == x)
                              {
                                 if (v->type == STRING)
                                 {
                                 }
                                 else
                                 {
                                    N = *v->memnum;
                                    Found = TRUE;
                                 }
                                 Loop = FALSE;
                              }
                              n++;
                           }
                        }
                        Loop = FALSE;
                        break;
                     }
                  }
               }
            }
            if (Found == FALSE)
            {
               IsError = 'X';
            }
         }
         break;



      case F_ASC_A_N:
         /* N = ASC( A$ ) */
         {
            /* P1BYT */
            N = A[0];
         }
         break;
      case F_CDBL_X_N:
         /* N = CDBL( X ) */
         {
            /* P1DBL */
            N = X;
         }
         break;
      case F_CSNG_X_N:
         /* N = CSNG( X ) */
         {
            /* P1FLT */
            N = X;
         }
         break;
      case F_CINT_X_N:
         /* N = CINT( X ) */
         {
            /* P1INT */
            N = rint(X);
         }
         break;
      case F_CLNG_X_N:
         /* N = CLNG( X ) */
         {
            /* P1LNG */
            N = rint(X);
         }
         break;
      case F_CCUR_X_N:
         /* N = CCUR( X ) */
         {
            /* P1CUR */
            N = rint(X);
         }
         break;
      case F_MKD_X_S:
         /* S$ = MKD$( X ) */
         {
            /* P1DBL */
            BasicDoubleType x;
            x = (BasicDoubleType) X;
            s = sizeof(BasicDoubleType);
            memcpy(S, &x, s);
         }
         break;
      case F_MKS_X_S:
         /* S$ = MKS$( X ) */
         {
            /* P1FLT */
            BasicSingleType x;
            x = (BasicSingleType) X;
            s = sizeof(BasicSingleType);
            memcpy(S, &x, s);
         }
         break;
      case F_MKI_X_S:
         /* S$ = MKI$( X ) */
         {
            /* P1INT */
            BasicIntegerType x;
            x = (BasicIntegerType) rint(X);
            s = sizeof(BasicIntegerType);
            memcpy(S, &x, s);
         }
         break;
      case F_MKL_X_S:
         /* S$ = MKL$( X ) */
         {
            /* P1LNG */
            BasicLongType   x;
            x = (BasicLongType) rint(X);
            s = sizeof(BasicLongType);
            memcpy(S, &x, s);
         }
         break;
      case F_MKC_X_S:
         /* S$ = MKC$( X ) */
         {
            /* P1CUR */
            BasicCurrencyType x;
            x = (BasicCurrencyType) rint(X);
            s = sizeof(BasicCurrencyType);
            memcpy(S, &x, s);
         }
         break;
      case F_CVD_A_N:
         /* N = CVD( A$ ) */
         {
            /* P1DBL */
            BasicDoubleType n;
            a = sizeof(BasicDoubleType);
            memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_CVS_A_N:
         /* N = CVS( X$ ) */
         {
            /* P1FLT */
            BasicSingleType n;
            a = sizeof(BasicSingleType);
            memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_CVI_A_N:
         /* N = CVI( X$ ) */
         {
            /* P1INT */
            BasicIntegerType n;
            a = sizeof(BasicIntegerType);
            memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_CVL_A_N:
         /* N = CVL( X$ ) */
         {
            /* P1LNG */
            BasicLongType   n;
            a = sizeof(BasicLongType);
            memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_CVC_A_N:
         /* N = CVC( X$ ) */
         {
            /* P1CUR */
            BasicCurrencyType n;
            a = sizeof(BasicCurrencyType);
            memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_ENVIRON_A_S:
         /* S$ = ENVIRON$( A$ ) */
         {
            /* P1BYT */
            char           *CharPointer;

            CharPointer = getenv(A);
            if (CharPointer == NULL)
            {
               /* empty string */
            }
            else
            {
               s = strlen(CharPointer);
               s = MIN(s, BasicStringLengthMax);
               if (s == 0)
               {
                  /* empty string */
               }
               else
               {
                  memcpy(S, CharPointer, s);
               }
            }
         }
         break;
      case F_ENVIRON_A_N:
         /* ENVIRON A$ */
         {
            /* P1BYT */

            char           *CharPointer;

            CharPointer = strchr(A, '=');
            if (CharPointer == NULL)
            {
               /* missing required '=' */
               IsError = 'A';
            }
            else
            {
               if (putenv(A) == -1)
               {
                  IsError = 'A';
               }
               else
               {
                  /* OK */
                  N = 0;
               }
            }
         }
         break;

      case F_OPEN_A_X_B_Y_N:
         /* OPEN "I"|"O"|"R"|"A", [#]n, filename [,rlen] */
         {
            /* P1STR|P2NUM|P3STR|P4NUM */
            /* P1BYT|P2DEV|P3BYT|P4INT */

            int             mode;

            while (*A == ' ')
            {
               A++;  /* LTRIM$ */
            }
            mode = ToUpper(*A);
            switch (mode)
            {
            case 'I':
            case 'O':
            case 'A':
            case 'B':
            case 'R':
               break;
            default:
               mode = DEVMODE_CLOSED;
               break;
            }

            if (x == CONSOLE_FILE_NUMBER)
            {
               IsError = 'X';
            }
            else
            if (mode == DEVMODE_CLOSED)
            {
               IsError = 'A';
            }
            else
            if (dev_table[x].mode != DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            if (y < 0)
            {
               IsError = 'Y';
            }
            else
            if (y == 0 && mode == 'R')
            {
               IsError = 'Y';
            }
            else
            {
               FILE           *fp = NULL;
               char           *buffer = NULL;
               switch (mode)
               {
               case 'I':
                  mode = DEVMODE_INPUT;
                  fp = fopen(B, "r");
                  y = 0;
                  break;
               case 'O':
                  mode = DEVMODE_OUTPUT;
                  fp = fopen(B, "w");
                  y = 0;
                  break;
               case 'A':
                  mode = DEVMODE_APPEND;
                  fp = fopen(B, "a");
                  y = 0;
                  break;
               case 'B':
                  mode = DEVMODE_BINARY;
                  fp = fopen(B, "r+");
                  if (fp == NULL)
                  {
                     fp = fopen(B, "w");
                     fclose(fp);
                     fp = fopen(B, "r+");
                  }
                  y = 0;
                  break;
               case 'R':
                  mode = DEVMODE_RANDOM;
                  fp = fopen(B, "r+");
                  if (fp == NULL)
                  {
                     fp = fopen(B, "w");
                     fclose(fp);
                     fp = fopen(B, "r+");
                  }
                  if (fp != NULL)
                  {
                     buffer = CALLOC(y, 1, "F_OPEN_A_X_B_Y_V");
                  }
                  break;
               }
               if (fp == NULL)
               {
                  /* i n v a l i d 
                   *
                   * fi l e 
                   *
                   * na m e */
                  IsError = 'B';
               }
               else
               if (mode == DEVMODE_RANDOM && buffer == NULL)
               {
                  /* i n v a l i d 
                   *
                   * re c o r d 
                   *
                   * le n g t h */
                  IsError = 'Y';
               }
               else
               {
                  dev_table[x].mode = mode;
                  dev_table[x].cfp = fp;
                  dev_table[x].width = y;
                  /* N O T E : 
                   *
                   * WI D T H 
                   *
                   * == 
                   * RE C L E N */
                  dev_table[x].col = 1;
                  dev_table[x].buffer = buffer;
                  strcpy(dev_table[x].filename, B);
                  if (mode == DEVMODE_APPEND)
                  {
                     fseek(fp, 0, SEEK_END);
                  }
                  else
                  if (mode == DEVMODE_RANDOM)
                  {
                     memset(buffer, ' ', y); /* flush */
                  }
               }
            }
         }
         break;
      case F_OPEN_A_X_B_N:
         /* default  LEN is 128 for RANDOM, 0 for all others */
         /* OPEN "I"|"O"|"R"|"A", [#]n, filename [,rlen] */
         {
            /* P1STR|P2NUM|P3STR|P4NUM */
            /* P1BYT|P2DEV|P3BYT|P4INT */
            int             mode;
            int             y = 0;

            while (*A == ' ')
            {
               A++;  /* LTRIM$ */
            }
            mode = ToUpper(*A);
            switch (mode)
            {
            case 'I':
            case 'O':
            case 'A':
            case 'B':
               break;
            case 'R':
               y = bwx_RANDOM_RECORD_SIZE();
               break;
            default:
               mode = DEVMODE_CLOSED;
               break;
            }

            if (x == CONSOLE_FILE_NUMBER)
            {
               IsError = 'X';
            }
            else
            if (mode == DEVMODE_CLOSED)
            {
               IsError = 'A';
            }
            else
            if (dev_table[x].mode != DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            if (y < 0)
            {
               IsError = 'Y';
            }
            else
            if (y == 0 && mode == 'R')
            {
               IsError = 'Y';
            }
            else
            if (y > 0 && mode == 'B')
            {
               IsError = 'Y';
            }
            else
            {
               FILE           *fp = NULL;
               char           *buffer = NULL;
               switch (mode)
               {
               case 'I':
                  mode = DEVMODE_INPUT;
                  fp = fopen(B, "r");
                  y = 0;
                  break;
               case 'O':
                  mode = DEVMODE_OUTPUT;
                  fp = fopen(B, "w");
                  y = 0;
                  break;
               case 'A':
                  mode = DEVMODE_APPEND;
                  fp = fopen(B, "a");
                  y = 0;
                  break;
               case 'B':
                  mode = DEVMODE_BINARY;
                  fp = fopen(B, "r+");
                  if (fp == NULL)
                  {
                     fp = fopen(B, "w");
                     fclose(fp);
                     fp = fopen(B, "r+");
                  }
                  y = 0;
                  break;
               case 'R':
                  mode = DEVMODE_RANDOM;
                  fp = fopen(B, "r+");
                  if (fp == NULL)
                  {
                     fp = fopen(B, "w");
                     fclose(fp);
                     fp = fopen(B, "r+");
                  }
                  if (fp != NULL)
                  {
                     buffer = CALLOC(y, 1, "F_OPEN_A_X_B_Y_V");
                  }
                  break;
               }
               if (fp == NULL)
               {
                  /* i n v a l i d 
                   *
                   * fi l e 
                   *
                   * na m e */
                  IsError = 'B';
               }
               else
               if (mode == DEVMODE_RANDOM && buffer == NULL)
               {
                  /* i n v a l i d 
                   *
                   * re c o r d 
                   *
                   * le n g t h */
                  IsError = 'Y';
               }
               else
               {
                  dev_table[x].mode = mode;
                  dev_table[x].cfp = fp;
                  dev_table[x].width = y;
                  /* N O T E : 
                   *
                   * WI D T H 
                   *
                   * == 
                   * RE C L E N */
                  dev_table[x].col = 1;
                  dev_table[x].buffer = buffer;
                  strcpy(dev_table[x].filename, B);
                  if (mode == DEVMODE_APPEND)
                  {
                     fseek(fp, 0, SEEK_END);
                  }
                  else
                  if (mode == DEVMODE_RANDOM)
                  {
                     memset(buffer, ' ', y); /* flush */
                  }
               }
            }
         }
         break;
      case F_LOC_X_N:
         /* N = LOC( X ) */
         {
            /* P1DEV */
            if (x == CONSOLE_FILE_NUMBER)
            {
               N = 0;
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            {
               FILE           *fp;
               fp = dev_table[x].cfp;
               N = ftell(fp);
               if (dev_table[x].mode == DEVMODE_RANDOM)
               {
                  /* record number */
                  N /= dev_table[x].width;
               }
               else
               if (dev_table[x].mode == DEVMODE_BINARY)
               {
                  /* byte position */
               }
               else
               {
                  /* byte positiion / 128 */
                  N /= 128;
               }
               N = floor(N);
               N++;  /* C to BASIC */
            }
         }
         break;
      case F_SEEK_X_N:
         /* N = SEEK( X ) */
         {
            /* P1DEV */
            if (x == CONSOLE_FILE_NUMBER)
            {
               N = 0;
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            {
               FILE           *fp;
               fp = dev_table[x].cfp;
               N = ftell(fp);
               if (dev_table[x].mode == DEVMODE_RANDOM)
               {
                  /* record number */
                  N /= dev_table[x].width;
               }
               else
               {
                  /* byte positiion */
               }
               N = floor(N);
               N++;  /* C to BASIC */
            }
         }
         break;
      case F_SEEK_X_Y_N:
         /* SEEK X, Y */
         {
            /* P1DEV|P2INT */
            if (x == CONSOLE_FILE_NUMBER)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            if (y < 1)
            {
               IsError = 'Y';
            }
            else
            {
               long            offset;
               offset = y;
               offset--;   /* BASIC to C */
               if (dev_table[x].mode == DEVMODE_RANDOM)
               {
                  offset *= dev_table[x].width;
               }
               if (fseek(dev_table[x].cfp, offset, SEEK_SET) != 0)
               {
                  IsError = 'Y';
               }
               else
               {
                  /* OK */
                  N = 0;
               }
            }
         }
         break;
      case F_LOF_X_N:
         /* N = LOF( X ) */
         {
            /* P1DEV */
            if (x == CONSOLE_FILE_NUMBER)
            {
               N = 0;
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            {
               /* file size in bytes */
               FILE           *fp;
               int             current;
               int             total;
               fp = dev_table[x].cfp;
               current = ftell(fp);
               fseek(fp, 0, SEEK_END);
               total = ftell(fp);
               if (total == current)
               {
                  /* EOF */
               }
               else
               {
                  fseek(fp, current, SEEK_SET);
               }
               N = total;
            }
         }
         break;
      case F_EOF_X_N:
         /* N = EOF( X ) */
         {
            /* P1DEV */
            if (x == CONSOLE_FILE_NUMBER)
            {
               N = 0;
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            {
               /* are we at the end? */
               FILE           *fp;
               int             current;
               int             total;
               fp = dev_table[x].cfp;
               current = ftell(fp);
               fseek(fp, 0, SEEK_END);
               total = ftell(fp);
               if (total == current)
               {
                  /* EOF */
                  N = -1;
               }
               else
               {
                  fseek(fp, current, SEEK_SET);
                  N = 0;
               }
            }
         }
         break;
      case F_FILEATTR_X_Y_N:
         /* N = FILEATTR( X, Y ) */
         {
            /* P1DEV|P2INT */
            if (x == CONSOLE_FILE_NUMBER)
            {
               IsError = 'X';
            }
            else
            {
               if (y == 1)
               {
                  N = dev_table[x].mode;
               }
               else
               if (y == 2)
               {
                  N = 0;
               }
               else
               {
                  IsError = 'Y';
               }
            }
         }
         break;
      case F_CLOSE_X_N:
         /* CLOSE X */
         {
            /* P1DEV */
            if (x == CONSOLE_FILE_NUMBER)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            {
               if (dev_table[x].cfp != NULL)
               {
                  fclose(dev_table[x].cfp);
               }
               if (dev_table[x].buffer != NULL)
               {
                  FREE(dev_table[x].buffer, "F_CLOSE_X_N");
               }
               dev_table[x].mode = DEVMODE_CLOSED;
               dev_table[x].width = 0;
               dev_table[x].col = 0;
               dev_table[x].filename[0] = '\0';
               dev_table[x].cfp = NULL;
               dev_table[x].buffer = NULL;
               N = 0;
            }
         }
         break;
      case F_FREEFILE_N:
         /* N = FREEFILE */
         {
            /* PNONE */
            int             x;
            for (x = 0; x <= BasicFileNumberMax; x++)
            {
               if (x == CONSOLE_FILE_NUMBER)
               {
                  /* ignore */
               }
               else
               if (dev_table[x].mode == DEVMODE_CLOSED)
               {
                  N = x;
                  break;
               }
            }
         }
         break;
      case F_RESET_N:
         /* RESET */
         {
            /* PNONE */
            int             x;
            for (x = 0; x <= BasicFileNumberMax; x++)
            {
               if (x == CONSOLE_FILE_NUMBER)
               {
                  /* ignore */
               }
               else
               if (dev_table[x].mode != DEVMODE_CLOSED)
               {
                  if (dev_table[x].cfp != NULL)
                  {
                     fclose(dev_table[x].cfp);
                  }
                  if (dev_table[x].buffer != NULL)
                  {
                     FREE(dev_table[x].buffer, "F_RESET_V");
                  }
                  dev_table[x].mode = DEVMODE_CLOSED;
                  dev_table[x].width = 0;
                  dev_table[x].col = 0;
                  dev_table[x].filename[0] = '\0';
                  dev_table[x].cfp = NULL;
                  dev_table[x].buffer = NULL;
               }
            }
            N = 0;
         }
         break;
      case F_GET_X_Y_N:
         /* GET X, Y */
         {
            /* P1DEV|P2INT */
            if (x == CONSOLE_FILE_NUMBER)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode != DEVMODE_RANDOM)
            {
               IsError = 'X';
            }
            else
            if (y < 1)
            {
               IsError = 'Y';
            }
            else
            {
               long            offset;
               offset = y;
               offset--;   /* BASIC to C */
               offset *= dev_table[x].width;
               if (fseek(dev_table[x].cfp, offset, SEEK_SET) != 0)
               {
                  IsError = 'Y';
               }
               else
               {
                  int             i;
                  for (i = 0; i < dev_table[x].width; i++)
                  {
                     dev_table[x].buffer[i] = fgetc(dev_table[x].cfp);
                  }
                  N = 0;
               }
            }
         }
         break;
      case F_GET_X_N:
         /* GET X */
         {
            /* PDEV1 */
            if (x == CONSOLE_FILE_NUMBER)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode != DEVMODE_RANDOM)
            {
               IsError = 'X';
            }
            else
            {
               {
                  int             i;
                  for (i = 0; i < dev_table[x].width; i++)
                  {
                     dev_table[x].buffer[i] = fgetc(dev_table[x].cfp);
                  }
                  N = 0;
               }
            }
         }
         break;
      case F_PUT_X_Y_N:
         /* PUT X, Y */
         {
            /* P1DEV|P2INT */
            if (x == CONSOLE_FILE_NUMBER)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode != DEVMODE_RANDOM)
            {
               IsError = 'X';
            }
            else
            if (y < 1)
            {
               IsError = 'Y';
            }
            else
            {
               long            offset;
               offset = y;
               offset--;   /* BASIC to C */
               offset *= dev_table[x].width;
               if (fseek(dev_table[x].cfp, offset, SEEK_SET) != 0)
               {
                  IsError = 'Y';
               }
               else
               {
                  int             i;
                  for (i = 0; i < dev_table[x].width; i++)
                  {
                     fputc(dev_table[x].buffer[i], dev_table[x].cfp);
                     dev_table[x].buffer[i] = ' '; /* flush  */
                  }
                  N = 0;
               }
            }
         }
         break;
      case F_PUT_X_N:
         /* PUT X */
         {
            /* P1DEV */
            if (x == CONSOLE_FILE_NUMBER)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode != DEVMODE_RANDOM)
            {
               IsError = 'X';
            }
            else
            {
               {
                  int             i;
                  for (i = 0; i < dev_table[x].width; i++)
                  {
                     fputc(dev_table[x].buffer[i], dev_table[x].cfp);
                     dev_table[x].buffer[i] = ' '; /* flush  */
                  }
                  N = 0;
               }
            }
         }
         break;
      case F_WIDTH_X_N:
         /* WIDTH X */
         {
            /* P1BYT */
            /* console is #0 */
            dev_table[CONSOLE_FILE_NUMBER].width = x;
            dev_table[CONSOLE_FILE_NUMBER].col = 1;
            N = 0;
         }
         break;
      case F_WIDTH_X_Y_N:
         /* WIDTH X, Y */
         {
            /* P1DEV|PB2YT */
            if (dev_table[x].mode == DEVMODE_CLOSED)
            {
               IsError = 'X';
            }
            else
            if (dev_table[x].mode == DEVMODE_RANDOM)
            {
               IsError = 'X';
            }
            else
            {
               dev_table[x].width = y;
               dev_table[x].col = 1;
               N = 0;
            }
         }
         break;



      case F_INSTR_X_A_B_N:
         /* N = INSTR( X, A$, B$ ) */
         {
            /* P1POS */
            if (a == 0)
            {
               /* empty searched */
            }
            else
            if (b == 0)
            {
               /* empty pattern */
            }
            else
            if (b > a)
            {
               /* pattern is longer than searched */
            }
            else
            {
               /* search */
               int             i;
               int             n;
               n = a - b;  /* last valid search
                      * position */
               n++;

               x--;  /* BASIC to C */
               A += x;  /* advance to the start
                   * position */
               for (i = x; i < n; i++)
               {
                  if (memcmp(A, B, b) == 0)
                  {
                     /* FOU ND */
                     i++;  /* C to BASIC */
                     N = i;
                     i = n;   /* exit for */
                  }
                  A++;
               }
            }
         }
         break;
      case F_INSTR_A_B_N:
         /* N = INSTR( A$, B$ ) */
         {
            if (a == 0)
            {
               /* empty searched */
            }
            else
            if (b == 0)
            {
               /* empty pattern */
            }
            else
            if (b > a)
            {
               /* pattern is longer than searched */
            }
            else
            {
               /* search */
               int             i;
               int             n;
               n = a - b;  /* last valid search
                      * position */
               n++;
               /* search */
               for (i = 0; i < n; i++)
               {
                  if (memcmp(A, B, b) == 0)
                  {
                     /* FOU ND */
                     i++;  /* C to BASIC */
                     N = i;
                     i = n;   /* exit for */
                  }
                  A++;
               }
            }
         }
         break;
      case F_SPACE_X_S:
         /* S$ = SPACE$( X ) */
         {
            /* P1LEN */
            if (x == 0)
            {
               /* no copies */
            }
            else
            {
               memset(S, ' ', x);
               s = x;
            }
         }
         break;
      case F_STRING_X_Y_S:
         /* S$ = STRING$( X, Y ) */
         {
            /* P1LEN|P2BYT */
            if (x == 0)
            {
               /* no copies */
            }
            else
            {
               memset(S, (char) y, x);
               s = x;
            }
         }
         break;
      case F_STRING_X_A_S:
         /* S$ = STRING$( X, A$ ) */
         {
            /* P1LEN|P2BYT */
            if (x == 0)
            {
               /* no copies */
            }
            else
            {
               memset(S, (char) A[0], x);
               s = x;
            }
         }
         break;
      case F_MID_A_X_S:
         /* S$ = MID$( A$, X ) */
         {
            /* P1ANY|P2POS */
            if (a == 0)
            {
               /* empty string */
            }
            else
            if (x > a)
            {
               /* start beyond length */
            }
            else
            {
               x--;  /* BASIC to C */
               a -= x;  /* nummber of characters to
                   * copy */
               A += x;  /* pointer to first character
                   * to copy */
               memcpy(S, A, a);
               s = a;
            }
         }
         break;
      case F_MID_A_X_Y_S:
         /* S$ = MID$( A$, X, Y ) */
         {
            /* P1ANY|P2POS|P3LEN */
            if (a == 0)
            {
               /* empty string */
            }
            else
            if (x > a)
            {
               /* start beyond length */
            }
            else
            if (y == 0)
            {
               /* empty string */
            }
            else
            {
               x--;  /* BASIC to C */
               a -= x;
               /* maximum nummber of characters to
                * copy */
               a = MIN(a, y);
               A += x;
               /* pointer to first character to copy */
               memcpy(S, A, a);
               s = a;
            }
         }
         break;
      case F_LEFT_A_X_S:
         /* S$ = LEFT$( A$, X ) */
         {
            /* P1ANY|P2LEN */
            if (a == 0)
            {
               /* empty string */
            }
            else
            if (x == 0)
            {
               /* empty string */
            }
            else
            {
               a = MIN(a, x);
               memcpy(S, A, a);
               s = a;
            }
         }
         break;
      case F_RIGHT_A_X_S:
         /* S$ = RIGHT$( A$, X ) */
         {
            /* P1ANY|P2LEN */
            if (a == 0)
            {
               /* empty string */
            }
            else
            if (x == 0)
            {
               /* empty string */
            }
            else
            {
               x = MIN(a, x);
               A += a;
               A -= x;
               memcpy(S, A, x);
               s = x;
            }
         }
         break;
      case F_HEX_X_S:
         /* S$ = HEX$( X ) */
         {
            sprintf(S, "%X", x);
            s = strlen(S);
         }
         break;
      case F_OCT_X_S:
         /* S$ = OCT$( X ) */
         {
            sprintf(S, "%o", x);
            s = strlen(S);
         }
         break;
      case F_CHR_X_S:
         /* S$ = CHR$( X ) */
         {
            S[0] = (char) x;
            s = 1;
         }
         break;
      case F_LEN_A_N:
         /* N = LEN( A$ ) */
         {
            N = a;
         }
         break;
      case F_POS_A_B_N:
         /* N = POS( A$, B$ ) */
         {
            if (b == 0)
            {
               /* empty pattern */
               N = 1;
            }
            else
            if (a == 0)
            {
               /* empty searched */
            }
            else
            if (b > a)
            {
               /* pattern is longer than searched */
            }
            else
            {
               /* search */
               int             i;
               int             n;
               n = a - b;  /* last valid search
                      * position */
               n++;
               /* search */
               for (i = 0; i < n; i++)
               {
                  if (memcmp(A, B, b) == 0)
                  {
                     /* FOU ND */
                     i++;  /* C to BASIC */
                     N = i;
                     i = n;   /* exit for */
                  }
                  A++;
               }
            }
         }
         break;
      case F_POS_A_B_X_N:
         /* N = POS( A$, B$, X ) */
         {
            if (b == 0)
            {
               /* empty pattern */
               N = 1;
            }
            else
            if (a == 0)
            {
               /* empty searched */
            }
            else
            if (b > a)
            {
               /* pattern is longer than searched */
            }
            else
            {
               /* search */
               int             i;
               int             n;
               n = a - b;  /* last valid search
                      * position */
               n++;

               /* search */
               x--;  /* BASIC to C */
               A += x;  /* advance to the start
                   * position */
               for (i = x; i < n; i++)
               {
                  if (memcmp(A, B, b) == 0)
                  {
                     /* FOU ND */
                     N = i + 1;  /* C to BASIC */
                     i = n;   /* exit for */
                  }
                  A++;
               }
            }
         }
         break;
      case F_VAL_A_N:
         /* N = VAL( A$ ) */
         {
            /* P1BYT */
            /* FIXME: use the BASIC numeric value parse
             * routine */
            int             ScanResult;
            BasicNumberType Value;
            ScanResult = sscanf(A, BasicNumberScanFormat, &Value);
            if (ScanResult != 1)
            {
               /* not a number */
               if (OptionFlags & OPTION_BUGS_ON)
               {
                  /* IGNORE */
                  N = 0;
               }
               else
               {
                  /* ERROR */
                  IsError = 'A';
               }
            }
            else
            {
               /* OK */
               N = Value;
            }
         }
         break;
      case F_STR_X_S:
         /* S$ = STR$( X ) */
         {
            /* P1ANY */
            /* sprintf( S, BasicNumberPrintFormat, X );  */
            BasicNumerc(X, S);
            s = strlen(S);
         }
         break;
      case F_DATE_N:
         /* N = DATE ' YYYYDDD  */
         {
            /* PNONE */
            /* ECMA-116 */
            time(&t);
            lt = localtime(&t);
            N = lt->tm_year;
            N *= 1000;
            N += lt->tm_yday;
            N += 1;
         }
         break;
      case F_DATE_S:
         /* S$ = DATE$ */
         {
            /* PNONE */
            time(&t);
            lt = localtime(&t);
            s = strftime(S, BasicStringLengthMax, OptionDateFormat, lt);
         }
         break;
      case F_TIME_S:
         /* S$ = TIME$ */
         {
            /* PNONE */
            time(&t);
            lt = localtime(&t);
#if 0
            sprintf(S, "%02d:%02d:%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);
            s = strlen(S);
#endif
            s = strftime(S, BasicStringLengthMax, OptionTimeFormat, lt);
         }
         break;
      case F_TIMER_N:
         /* N = TIMER */
      case F_TIME_N:
         /* N = TIME */
         {
            /* PNONE */
            time(&t);
            lt = localtime(&t);
            N = lt->tm_hour;
            N *= 60;
            N += lt->tm_min;
            N *= 60;
            N += lt->tm_sec;
         }
         break;
      case F_COSH_X_N:
         /* N = COSH( X ) */
         {
            /* P1ANY */
            N = cosh(X);
         }
         break;
      case F_SINH_X_N:
         /* N = SINH( X ) */
         {
            /* P1ANY */
            N = sinh(X);
         }
         break;
      case F_TANH_X_N:
         /* N = TANH( X ) */
         {
            /* P1ANY */
            N = tanh(X);
         }
         break;
      case F_LOG10_X_N:
         /* N = LOG10( X ) */
         {
            /* P1GTZ */
            N = log10(X);
         }
         break;
      case F_LOG2_X_N:
         /* N = LOG2( X ) */
         {
            /* P1GTZ */
            N = log(X) / log((BasicNumberType) 2);
         }
         break;
      case F_ACOS_X_N:
         /* N = ACOS( X ) */
         {
            /* P1ANY */
            if (X < -1 || X > 1)
            {
               IsError = 'X';
            }
            else
            {
               N = acos(X);
               if (OptionFlags & OPTION_ANGLE_DEGREES)
               {
                  N = N * 180 / PI;
               }
            }
         }
         break;
      case F_ASIN_X_N:
         /* N = ASIN( X ) */
         {
            /* P1ANY */
            if (X < -1 || X > 1)
            {
               IsError = 'X';
            }
            else
            {
               N = asin(X);
               if (OptionFlags & OPTION_ANGLE_DEGREES)
               {
                  N = N * 180 / PI;
               }
            }
         }
         break;
      case F_COT_X_N:
         /* N = COT( X ) ' = 1 / TAN( X ) */
         {
            /* P1ANY */
            BasicNumberType T;
            if (OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = X * PI / 180;
            }
            T = tan(X);
            if (T == 0)
            {
               IsError = 'X';
            }
            else
            {
               N = 1.0 / T;
            }
         }
         break;
      case F_CSC_X_N:
         /* N = CSC( X ) ' = 1 / SIN( X ) */
         {
            /* P1ANY */
            BasicNumberType T;
            if (OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = X * PI / 180;
            }
            T = sin(X);
            if (T == 0)
            {
               IsError = 'X';
            }
            else
            {
               N = 1.0 / T;
            }
         }
         break;
      case F_SEC_X_N:
         /* N = SEC( X ) ' = 1 / COS( X ) */
         {
            /* P1ANY */
            BasicNumberType T;
            if (OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = X * PI / 180;
            }
            T = cos(X);
            if (T == 0)
            {
               IsError = 'X';
            }
            else
            {
               N = 1.0 / T;
            }
         }
         break;
      case F_UCASE_A_S:
         /* S$ = UCASE$( A$ ) */
         {
            /* P1ANY */
            if (a == 0)
            {
               /* empty string */
            }
            else
            {
               int             i;
               memcpy(S, A, a);
               s = a;
               /* BASIC allows embedded NULL
                * characters */
               for (i = 0; i < a; i++)
               {
                  S[i] = ToUpper(S[i]);
               }
            }
         }
         break;
      case F_LCASE_A_S:
         /* S$ = LCASE$( A$ ) */
         {
            /* P1ANY */
            if (a == 0)
            {
               /* empty string */
            }
            else
            {
               int             i;
               memcpy(S, A, a);
               s = a;
               /* BASIC allows embedded NULL
                * characters */
               for (i = 0; i < a; i++)
               {
                  S[i] = ToLower(S[i]);
               }
            }
         }
         break;
      case F_ANGLE_X_Y_N:
         /* N = ANGLE( X, Y ) */
         {
            /* P1ANY|P2ANY */
            if (X == 0 && Y == 0)
            {
               IsError = 'X';
            }
            else
            {
               N = atan2(Y, X);
               if (OptionFlags & OPTION_ANGLE_DEGREES)
               {
                  N = N * 180 / PI;
               }
            }
         }
         break;
      case F_CEIL_X_N:
         /* N = CEIL( X ) */
         {
            /* P1ANY */
            N = ceil(X);
         }
         break;
      case F_DEG_X_N:
         /* N = DEG( X ) */
         {
            /* P1ANY */
            N = X * 180.0 / PI;
         }
         break;
      case F_RAD_X_N:
         /* N = RAD( X ) */
         {
            /* P1ANY */
            N = X * PI / 180.0;
         }
         break;
      case F_PI_N:
         /* N = PI */
         {
            /* PNONE */
            N = PI;
         }
         break;
      case F_LTRIM_A_S:
         /* S$ = LTRIM$( A$ ) */
         {
            /* P1ANY */
            if (a == 0)
            {
               /* empty string */
            }
            else
            {
               int             i;
               /* BASIC allows embedded NULL
                * characters */
               for (i = 0; i < a && A[i] == ' '; i++)
               {
                  /* skip spaces */
               }
               /* 'A[ i ]' is first non-space
                * character */
               if (i >= a)
               {
                  /* empty string */
               }
               else
               {
                  A += i;
                  a -= i;
                  memcpy(S, A, a);
                  s = a;
               }
            }
         }
         break;
      case F_RTRIM_A_S:
         /* S$ = RTRIM$( A$ ) */
         {
            /* P1ANY */
            if (a == 0)
            {
               /* empty string */
            }
            else
            {
               int             i;
               /* BASIC allows embedded NULL
                * characters */
               for (i = a - 1; i >= 0 && A[i] == ' '; i--)
               {
                  /* skip spaces */
               }
               /* 'A[ i ]' is last non-space
                * character */
               if (i < 0)
               {
                  /* empty string */
               }
               else
               {
                  a = i + 1;
                  memcpy(S, A, a);
                  s = a;
               }
            }
         }
         break;
      case F_TRIM_A_S:
         /* S$ = TRIM$( A$ ) */
         {
            /* P1ANY */
            if (a == 0)
            {
               /* empty string */
            }
            else
            {
               /* LTRIM */
               int             i;
               /* BASIC allows embedded NULL
                * characters */
               for (i = 0; i < a && A[i] == ' '; i++)
               {
                  /* skip spaces */
               }
               /* 'A[ i ]' is first non-space
                * character */
               if (i >= a)
               {
                  /* empty string */
               }
               else
               {
                  A += i;
                  a -= i;
                  memcpy(S, A, a);
                  s = a;
                  /* RTRIM */
                  A = S;
                  a = s;
                  if (a == 0)
                  {
                     /* empty string */
                  }
                  else
                  {
                     int             i;
                     /* BASIC allows
                      * embedded NULL
                      * characters */
                     for (i = a - 1; i >= 0 && A[i] == ' '; i--)
                     {
                        /* skip
                         * spaces */
                     }
                     /* 'A[ i ]' is last
                      * non-space
                      * character */
                     if (i < 0)
                     {
                        /* empty
                         * string */
                     }
                     else
                     {
                        a = i + 1;
                        /* memcpy( S,
                         * A, a ); */
                        s = a;
                     }
                  }
               }
            }
         }
         break;
      case F_MAX_X_Y_N:
         /* N = MAX( X, Y ) */
         {
            N = MAX(X, Y);
         }
         break;
      case F_MIN_X_Y_N:
         /* N = MIN( X, Y ) */
         {
            N = MIN(X, Y);
         }
         break;
      case F_FP_X_N:
         /* N = FP( X ) */
         {
            BasicNumberType FP;
            BasicNumberType IP;
            FP = modf(X, &IP);
            N = FP;
         }
         break;
      case F_IP_X_N:
         /* N = IP( X ) */
         {
            BasicNumberType IP;
            modf(X, &IP);
            N = IP;
         }
         break;
      case F_EPS_X_N:
         /* N = EPS( Number ) */
         {
            N = DBL_MIN;
         }
         break;
      case F_MAXLVL_N:
         /* N = MAXLVL */
         {
            N = EXECLEVELS;
         }
         break;
      case F_MAXNUM_N:
         /* N = MAXNUM */
         {
            N = DBL_MAX;
         }
         break;
      case F_MINNUM_N:
         /* N = MINNUM */
         {
            N = -DBL_MAX;
         }
         break;
      case F_MAXDBL_N:
         /* N = MAXDBL */
         {
            N = DBL_MAX;
         }
         break;
      case F_MINDBL_N:
         /* N = MINDBL */
         {
            N = -DBL_MAX;
         }
         break;
      case F_MAXSNG_N:
         /* N = MAXSNG */
         {
            N = FLT_MAX;
         }
         break;
      case F_MINSNG_N:
         /* N = MINSNG */
         {
            N = -FLT_MAX;
         }
         break;
      case F_MAXCUR_N:
         /* N = MAXCUR */
         {
            N = LONG_MAX;
         }
         break;
      case F_MINCUR_N:
         /* N = MINCUR */
         {
            N = LONG_MIN;
         }
         break;
      case F_MAXLNG_N:
         /* N = MAXLNG */
         {
            N = LONG_MAX;
         }
         break;
      case F_MINLNG_N:
         /* N = MINLNG */
         {
            N = LONG_MIN;
         }
         break;
      case F_MAXINT_N:
         /* N = MAXINT */
         {
            N = SHRT_MAX;
         }
         break;
      case F_MININT_N:
         /* N = MININT */
         {
            N = SHRT_MIN;
         }
         break;
      case F_MAXBYT_N:
         /* N = MAXBYT */
         {
            N = UCHAR_MAX;
         }
         break;
      case F_MINBYT_N:
         /* N = MINBYT */
         {
            N = 0;
         }
         break;
      case F_MAXDEV_N:
         /* N = MAXDEV */
         {
            N = BasicFileNumberMax;
         }
         break;
      case F_MINDEV_N:
         /* N = MINDEV */
         {
            N = 0;
         }
         break;

      case F_MOD_X_Y_N:
         /* N = MOD( X, Y ) */
         {
            /* P1ANY|P2NEZ */
            BasicNumberType IP;

            IP = floor(X / Y);
            N = X - (Y * IP);
         }
         break;
      case F_REMAINDER_X_Y_N:
         /* REMAINDER( X, Y ) */
         {
            /* P1ANY|P2NEZ */
            BasicNumberType Value;
            BasicNumberType IP;

            Value = X / Y;
            modf(Value, &IP);
            N = X - (Y * IP);
         }
         break;
      case F_ROUND_X_Y_N:
         /* N = ROUND( X, Y ) == INT(X*10^Y+.5)/10^Y */
         {
            /* P1ANY | P2INT */
            if (y < -32 || y > 32)
            {
               IsError = 'Y';
            }
            else
            {
               BasicNumberType T;   /* 10^Y */

               T = pow(10.0, Y);
               if (T == 0)
               {
                  IsError = 'Y';
               }
               else
               {
                  N = floor(X * T + 0.5) / T;
               }
            }
         }
         break;
      case F_TRUNCATE_X_Y_N:
         /* N = TRUNCATE( X, Y ) == INT(X*10^Y)/10^Y */
         {
            /* P1ANY | P2INT */
            if (y < -32 || y > 32)
            {
               IsError = 'Y';
            }
            else
            {
               BasicNumberType T;   /* 10^Y */

               T = pow(10.0, Y);
               if (T == 0)
               {
                  IsError = 'Y';
               }
               else
               {
                  N = floor(X * T) / T;
               }
            }
         }
         break;
      case F_MAXLEN_A_N:
         /* N = MAXLEN( A$ ) */
         {
            N = BasicStringLengthMax;
         }
         break;
      case F_ORD_A_N:
         /* N = ORD( A$ ) */
         {
            /* P1BYT */
            if (a == 1)
            {
               N = A[0];
            }
            else
            {
               int             c;
               N = -1;  /* not found */
               for (c = 0; c < NUM_ACRONYMS; c++)
               {
                  if (strcasecmp(AcronymTable[c].Name, A) == 0)
                  {
                     /* found */
                     N = AcronymTable[c].Value;
                     c = NUM_ACRONYMS; /* exit for */
                  }
               }
               if (N < 0)
               {
                  /* not found */
                  IsError = 'A';
               }
            }
         }
         break;
      case F_REPEAT_X_Y_S:
         /* S$ = REPEAT$( X, Y ) ' X is count, Y is code */
         {
            /* P1LEN | P2BYT */
            if (x == 0)
            {
               /* empty string */
            }
            else
            {
               memset(S, (char) y, x);
               s = x;
            }
         }
         break;
      case F_REPEAT_X_A_S:
         /* S$ = REPEAT$( X, A$ ) ' X is count, A$ is code */
         {
            /* P1LEN | P2BYT */
            if (x == 0)
            {
               /* empty string */
            }
            else
            {
               memset(S, (char) A[0], x);
               s = x;
            }
         }
         break;
      case F_FIX_X_N:
         /* N = FIX( X ) */
         {
            N = rint(X);
         }
         break;
      case F_ABS_X_N:
         /* N = ABS( X ) */
         {
            N = fabs(X);
         }
         break;
      case F_ATN_X_N:
         /* N = ATN( X ) */
         {
            N = atan(X);
            if (OptionFlags & OPTION_ANGLE_DEGREES)
            {
               N = N * 180 / PI;
            }
         }
         break;
      case F_COS_X_N:
         /* N = COS( X ) */
         {
            if (OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = X * PI / 180;
            }
            N = cos(X);
         }
         break;
      case F_EXP_X_N:
         /* N = EXP( X ) */
         {
            N = exp(X);
         }
         break;
      case F_INT_X_N:
         /* N = INT( X ) */
         {
            N = floor(X);
         }
         break;
      case F_LOG_X_N:
         /* N = LOG( X ) */
         {
            /* P1GTZ */
            N = log(X);
         }
         break;
      case F_RND_N:
         /* N = RND */
         {
            N = rand();
            N /= RAND_MAX;
         }
         break;
      case F_RND_X_N:
         /* N = RND( X ) */
         {
            N = rand();
            N /= RAND_MAX;
         }
         break;
      case F_SGN_X_N:
         /* N = SGN( X ) */
         {
            if (X > 0)
            {
               N = 1;
            }
            else
            if (X < 0)
            {
               N = -1;
            }
            else
            {
               N = 0;
            }
         }
         break;
      case F_SIN_X_N:
         /* N = SIN( X ) */
         {
            if (OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = X * PI / 180;
            }
            N = sin(X);
         }
         break;
      case F_SQR_X_N:
         /* N = SQR( X ) */
         {
            /* P1GEZ */
            N = sqrt(X);
         }
         break;
      case F_TAN_X_N:
         /* N = TAN( X ) */
         {
            if (OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = X * PI / 180;
            }
            N = tan(X);
         }
         break;
      case F_SPC_X_S:
         /* S$ = SPC( X ) */
         {
            /* P1ANY */
            /* SPECIAL RULES APPLY.  PART OF PRINT
             * COMMAND. WIDTH > 0 */
            X = rint(X);
            if (X < 1 || X > 255)
            {
               bwb_Warning_Overflow("*** WARNING: INVALID SPC() ***");
               X = 1;
            }
            x = (int) X;
            S[0] = PRN_SPC;
            S[1] = (char) x;
            s = 2;
         }
         break;
      case F_TAB_X_S:
         /* S$ = TAB( X ) */
         {
            /* P1ANY */
            /* SPECIAL RULES APPLY.  PART OF PRINT
             * COMMAND. WIDTH > 0 */
            X = rint(X);
            if (X < 1 || X > 255)
            {
               bwb_Warning_Overflow("*** WARNING: INVALID TAB() ***");
               X = 1;
            }
            x = (int) X;
            S[0] = PRN_TAB;
            S[1] = (char) x;
            s = 2;
         }
         break;
      case F_POS_N:
         /* N = POS */
         {
            /* PNONE */
            N = dev_table[CONSOLE_FILE_NUMBER].col;
         }
         break;
      case F_POS_X_N:
         /* N = POS( X ) */
         {
            /* PDEV1 */
            N = dev_table[x].col;
         }
         break;
      case F_INPUT_X_Y_S:
         /* S$ = INPUT$( X, Y )  */
         {
            /* P1LEN|P2DEV */
            {
               if ((dev_table[y].mode & DEVMODE_READ) == 0)
               {
                  IsError = 'Y';
               }
               else
               if (x == 0)
               {
                  /* empty string */
               }
               else
               {
                  FILE           *fp;
                  fp = dev_table[y].cfp;
                  if (fp == NULL)
                  {
                     IsError = 'Y';
                  }
                  else
                  {
                     s = fread(S, 1, x, fp);
                     s = MAX(s, 0); /* if( s < 0 ) s = 0; */
                  }
               }
            }
         }
         break;
      case F_ERROR_X_N:
         /* ERROR X */
         {
            /* P1BYT */
            bwb_Warning(x, "");
            N = 0;
         }
         break;
      case F_ERROR_X_A_N:
         /* ERROR X, A$ */
         {
            /* P1BYT */
            bwb_Warning(x, A);
            N = 0;
         }
         break;
      case F_ERR_N:
         /* N = ERR */
         {
            /* PNONE */
            N = err_number;
         }
         break;
      case F_ERL_N:
         /* N = ERL */
         {
            /* PNONE */
            if( err_line != NULL )
            {
                N = err_line->number;
            }
         }
         break;
      case F_ERR_S:
         /* S = ERR$ */
         {
            /* PNONE */
            s = strlen(ErrMsg);
            if (s > 0)
            {
               strcpy(S, ErrMsg);
            }
         }
         break;


         /********************************************************************************************
                                                      **
                                                      **  Keep the platform specific functions together.  They should all call  bwx_* functions.
                                                      **
                                                      *********************************************************************************************/
      case F_INP_X_N:
         /* N = INP( X ) */
         {
            /* P1BYT */
            IsError = 0xFF;
         }
         break;
      case F_WAIT_X_Y_N:
         /* WAIT X, Y */
         {
            /* P1NUM|P2NUM */
            /* P1INT|P2BYT */
            IsError = 0xFF;
         }
         break;
      case F_WAIT_X_Y_Z_N:
         /* WAIT X, Y, Z */
         {
            /* P1NUM|P2NUM|P3NUM */
            /* P1INT|P2BYT|P3BYT */
            IsError = 0xFF;
         }
         break;
      case F_OUT_X_Y_N:
         /* OUT X, Y */
         {
            /* P1NUM|P2NUM */
            /* P1INT|P2BYT */
            IsError = 0xFF;
         }
         break;
      case F_PEEK_X_N:
         /* N = PEEK( X ) */
         {
            /* P1INT */
            IsError = 0xFF;
         }
         break;
      case F_POKE_X_Y_N:
         /* POKE X, Y */
         {
            /* P1NUM|P2NUM */
            /* P1INT|P2BYT */
            IsError = 0xFF;
         }
         break;
      case F_CLS_N:
         /* CLS */
         {
            /* PNONE */
            switch (OptionTerminalType)
            {
            case C_OPTION_TERMINAL_NONE:
               break;
            case C_OPTION_TERMINAL_ADM_3A:
               fprintf(stdout, "%c", 26);
               break;
            case C_OPTION_TERMINAL_ANSI:
               fprintf(stdout, "%c[2J", 27);
               fprintf(stdout, "%c[%d;%dH", 27, 1, 1);
               break;
            default:
               IsError = 0xFF;
               break;
            }
            fflush(stdout);
         }
         break;
      case F_LOCATE_X_Y_N:
         /* LOCATE X, Y */
         {
            /* P1NUM|P2NUM */
            /* P1BYT|P2BYT */
            switch (OptionTerminalType)
            {
            case C_OPTION_TERMINAL_NONE:
               break;
            case C_OPTION_TERMINAL_ADM_3A:
               fprintf(stdout, "%c=%c%c", 27, x + 32, y + 32);
               break;
            case C_OPTION_TERMINAL_ANSI:
               fprintf(stdout, "%c[%d;%dH", 27, x, y);
               break;
            default:
               IsError = 0xFF;
               break;
            }
            fflush(stdout);
         }
         break;
      case F_COLOR_X_Y_N:
         /* COLOR X, Y */
         {
            /* P1NUM|P2NUM */
            /* P1BYT|P2BYT */
            switch (OptionTerminalType)
            {
            case C_OPTION_TERMINAL_NONE:
               break;
            case C_OPTION_TERMINAL_ADM_3A:
               break;
            case C_OPTION_TERMINAL_ANSI:
               fprintf(stdout, "%c[%d;%dm", 27, 30 + x, 40 + y);
               break;
            default:
               IsError = 0xFF;
               break;
            }
            fflush(stdout);
         }
         break;
      case F_FILES_N:
         /* FILES */
         {
            /* PNONE */
            char   Buffer[BasicStringLengthMax + 1];
            struct bwb_variable *v; 

            v = var_find(DEFVNAME_FILES); 
            str_btoc(Buffer, var_getsval(v)); 
            N = system(Buffer);
         }
         break;
      case F_FILES_A_N:
         /* FILES A$ */
         {
            /* P1BYT */
            char   Buffer[BasicStringLengthMax + 1];
            struct bwb_variable *v; 

            v = var_find(DEFVNAME_FILES); 
            str_btoc(Buffer, var_getsval(v)); 
            strcat(Buffer, " ");
            strcat(Buffer, A);
            N = system(Buffer);
         }
         break;
      case F_FRE_N:
      case F_FRE_X_N:
      case F_FRE_A_N:
         /* N = FRE(    ) */
         /* N = FRE( X  ) */
         /* N = FRE( X$ ) */
         {
            N = 32000;  /* reasonable value */
         }
         break;
      case F_SHELL_A_N:
         /* N = SHELL( A$ ) */
         {
            /* P1BYT */
            N = system(A);
         }
         break;
      case F_CHDIR_A_N:
         /* CHDIR A$ */
         {
            /* P1BYT */
#if DIRECTORY_CMDS
            N = chdir(A);
#else
            IsError = 0xFF;
#endif
         }
         break;
      case F_MKDIR_A_N:
         /* MKDIR A$ */
         {
            /* P1BYT */
#if DIRECTORY_CMDS
#if MKDIR_ONE_ARG
            N = mkdir(A);
#else
            N = mkdir(A, PERMISSIONS);
#endif
#else
            IsError = 0xFF;
#endif
         }
         break;
      case F_RMDIR_A_N:
         /* RMDIR A$ */
         {
            /* P1BYT */
#if DIRECTORY_CMDS
            N = rmdir(A);
#else
            IsError = 0xFF;
#endif
         }
         break;
      case F_KILL_A_N:
         /* KILL A$ */
         {
            /* P1BYT */
            N = remove(A);
         }
         break;
      case F_NAME_A_B_N:
         /* NAME A$ AS B$      */
         /* N = NAME( A$, B$ ) */
         {
            /* P1BYT|P2BYT */
            N = rename(A, B);
         }
         break;
      case F_INPUT_X_S:
         /* S$ = INPUT$( X )  */
         {
            /* P1LEN */
            if (x == 0)
            {
               /* empty string */
            }
            else
            {
               for (s = 0; s < x; s++)
               {
                  int             c;
                  c = getchar();
                  if ((c == EOF) || (c == '\n') || (c == '\r'))
                  {
                     break;
                  }
                  S[s] = c;
               }
               S[s] = 0;
            }
         }
         break;
      case F_INKEY_S:
         /* S$ = INKEY$ */
         {
            /* PNONE */
            int             c;

            c = getchar();
            if (c < 0 || c > 255)
            {
               /* EOF */
            }
            else
            {
               S[s] = c;
               s++;
            }
            S[s] = 0;
         }
         break;
      case F_NULL_X_N:
         /* NULL X */
         {
            /* P1NUM */
            /* P1BYT */
            LPRINT_NULLS = x;
            N = 0;
         }
         break;
      case F_LWIDTH_X_N:
         /* LWIDTH X */
         {
            /* P1NUM */
            /* P1BYT */
            LPRINT_WIDTH = x;
            LPRINT_COLUMN = 1;
            N = 0;
         }
         break;
      case F_LPOS_N:
         /* N = LPOS */
         {
            /* PNONE */
            /* PNONE */
            N = LPRINT_COLUMN;
         }
         break;
      case F_TRON_N:
         /* TRON */
         {
            /* PNONE */
            prn_xprintf("Trace is ON\n");
            bwb_trace = TRUE;
            N = 0;
         }
         break;
      case F_TROFF_N:
         /* TROFF */
         {
            /* PNONE */
            prn_xprintf("Trace is OFF\n");
            bwb_trace = FALSE;
            N = 0;
         }
         break;
      case F_RANDOMIZE_N:
         /* RANDOMIZE */
         {
            /* PNONE */
            /* USE THE CURRENT TIME AS THE SEED */
            time(&t);
            lt = localtime(&t);
            x = lt->tm_hour * 3600 + lt->tm_min * 60 + lt->tm_sec;
            srand(x);
            N = 0;
         }
         break;
      case F_RANDOMIZE_X_N:
         /* RANDOMIZE X */
         {
            /* P1NUM */
            /* P1ANY */
            x = rint( X );
            srand(x);
            N = 0;
         }
         break;
      default:
         {
            /* NOT IMPLEMENTED ON THIS PLATFORM */
            IsError = 0xFF;
         }
      }
   /* sanity check */
   if (IsError == 0)
   {
      if (f->ReturnType == STRING)
      {
         /* STRING */
         if ( /* s < 0 || */ s > BasicStringLengthMax)
         {
            /* ERROR */
            sprintf(bwb_ebuf, "INTERNAL ERROR (%s) INVALID STRING LENGTH", f->Name);
            bwb_error(bwb_ebuf);
            return NULL;
         }
         else
         if (S != RESULT_BUFFER)
         {
            /* ERROR */
            sprintf(bwb_ebuf, "INTERNAL ERROR (%s) INVALID STRING BUFFER", f->Name);
            bwb_error(bwb_ebuf);
            return NULL;
         }
         else
         {
            RESULT_LENGTH = s;
            RESULT_BUFFER[RESULT_LENGTH] = '\0';
         }
      }
      else
      {
         /* NUMBER */
         if (isnan(N))
         {
            /* ERROR */
            /* this means the parameters were not
             * properly checked */
            sprintf(bwb_ebuf, "INTERNAL ERROR (%s) NOT A NUMBER", f->Name);
            bwb_error(bwb_ebuf);
            return NULL;
         }
         else
         if (isinf(N))
         {
            /* Evaluation of an expression results in an
             * overflow (nonfatal, the recommended
             * recovery procedure is to supply machine
             * in- finity with the algebraically correct
             * sign and continue). */
            if (N < 0)
            {
               N = -DBL_MAX;
            }
            else
            {
               N = DBL_MAX;
            }
            bwb_Warning_Overflow("*** Arithmetic Overflow ***");
         }
         RESULT_NUMBER = N;
      }
   }
   /* process errors */
   if (IsError == 0xFF)
   {
      /* NOT IMPLEMENTED ON THIS PLATFORM */
      sprintf(bwb_ebuf, "%s IS NOT IMPLEMENTED ON THIS PLATFORM", f->Name);
      bwb_Warning_AdvancedFeature(bwb_ebuf);
   }
   else
   if (IsError != 0)
   {
      /* ERROR */
      char            Buffer[80];
      switch (IsError)
      {
      case 'A':
      case 'B':
      case 'C':
         /* STRING parameter's value is invalid */
         sprintf(Buffer, "%s(%c$)", f->Name, IsError);
         break;
      case 'X':
      case 'Y':
      case 'Z':
         /* NUMBER parameter's value is invalid */
         sprintf(Buffer, "%s(%c)", f->Name, IsError);
         break;
      default:
         /* All other errors */
         sprintf(Buffer, "%s() #%d", f->Name, IsError);
         break;
      }
      sprintf(bwb_ebuf, "ILLEGAL FUUNCTION CALL: %s", Buffer);
      bwb_Warning_InvalidParameter(bwb_ebuf);
   }
   return argv;      /* released by exp_function() in bwb_elx.c */
}

/* EOF */
