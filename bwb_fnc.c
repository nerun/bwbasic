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
/* Version 3.10 by Howard Wulf, AF5NE                            */
/*                                                               */
/*---------------------------------------------------------------*/



#include "bwbasic.h"


#ifndef RAND_MAX
#define RAND_MAX  32767
#endif            /* RAND_MAX */

#ifndef PI
#define PI       3.14159265358979323846
#endif           /* PI */

#define FromDegreesToRadians(  X ) ( X * PI / 180.0 )
#define FromRadiansToDegrees(  X ) ( X * 180.0 / PI )

#define FromGradiansToRadians( X ) ( X * PI / 200.0 )
#define FromRadiansToGradians( X ) ( X * 200.0 / PI )


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

VariableType *
fnc_intrinsic(int argc, VariableType * argv, int unique_id)
{
   /* this is the generic handler for all intrinsic BASIC functions */
   /* BasicStringLengthMax must be <= INT_MAX */
   FunctionType *f;
   VariableType *argn;
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
#if FALSE /* keep third parameter */
   char           *C;   /* C$ - 3rd STRING parameter */
   BasicStringLengthType c;/* LEN( C$ ) */
#endif
   BasicNumberType X;   /* X  - 1st NUMBER parameter */
   BasicIntegerType x;  /* INT( X ) */
   BasicNumberType Y;   /* Y  - 2nd NUMBER parameter */
   BasicIntegerType y;  /* INT( Y ) */
#if FALSE /* keep third parameter */
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
         /* bwb_prefuncs[] in bwb_tbl.c is wrong -- this is really bad */
         WARN_INTERNAL_ERROR;
         return NULL;
      }
   }
   /* the RETURN variable is the first variable in the 'argv' vaariable chain */
   if (argv == NULL)
   {
      WARN_INTERNAL_ERROR;
      return NULL;
   }
   if ( VAR_IS_STRING( argv ) )
   {
      if (argv->memstr == NULL)
      {
         WARN_INTERNAL_ERROR;
         return NULL;
      }
      RESULT_LENGTH = 0;
      RESULT_BUFFER[RESULT_LENGTH] = BasicNulChar;
   }
   else
   {
      if (argv->memnum == NULL)
      {
         WARN_INTERNAL_ERROR;
         return NULL;
      }
      RESULT_NUMBER = 0;
   }
   argn = argv;
   /* don't make a bad situation worse */
   if( bwb_Warning_Pending() /* Keep This */ )
   {
      /* An unrecognized NON-FATAL ERROR is pending.  Just return a sane value. */
      /* LET N = LOG(SQR(X)) ' X = -1 */
      return argv;
   }
   /* Follow the BASIC naming conventions, so the code is easier to read and maintain */
   {
      int             i;
      int             StrCount = 0; /* count of STRING parameters - NEVER > 3 */
      int             NumCount = 0; /* count of NUMBER parameters - NEVER > 3 */
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
#if FALSE /* keep third parameter */
      C = NULL;   /* C$ - 3rd STRING parameter */
      c = 0;      /* LEN( C$ ) */
#endif
      X = 0;      /* X  - 1st NUMBER parameter */
      x = 0;      /* INT( X ) */
      Y = 0;      /* Y  - 2nd NUMBER parameter */
      y = 0;      /* INT( Y ) */
#if FALSE /* keep third parameter */
      Z = 0;      /* Z  - 3rd NUMBER parameter */
      z = 0;      /* INT( Z ) */
#endif
      /* assign actual values */
      if (f->ReturnType == BasicStringSuffix)
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
            WARN_INTERNAL_ERROR;
            return NULL;
         }
         if ( VAR_IS_STRING( argn ) )
         {
            if (argn->memstr == NULL)
            {
               WARN_INTERNAL_ERROR;
               return NULL;
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
#if FALSE /* keep third parameter */
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
               WARN_INTERNAL_ERROR;
               return NULL;
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
                  R = bwb_rint(X);
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
                  R = bwb_rint(Y);
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
#if FALSE /* keep third parameter */
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
                  R = bwb_rint(Z);
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


   /* execute the intrinsic function */
   /* WARNING -- do NOT execute a BASIC intrinsic function with bogus parameters */
   if (IsError == 0   )
   {
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
            /* determine number of parameters to the current USER DEFINED FUNCTION */
            int             n;
            n = 0;
            if ( My->stack_head != NULL )
            {
               int             Loop;
               StackType * stack_item;
               Loop = TRUE;
               for (stack_item = My->stack_head; stack_item != NULL && Loop == TRUE; stack_item = stack_item->next)
               {
                  if (stack_item->LoopTopLine != NULL)
                  {
                     switch (stack_item->LoopTopLine->cmdnum)
                     {
                     case C_FUNCTION:
                     case C_SUB:
                        /* we have checked all the way to a FUNCTION or SUB boundary */
                        /* FOUND */
                        {
                           VariableType *v;

                           for (v = stack_item->local_variable; v != NULL && Loop == TRUE; v = v->next)
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
      case F_ARGT4_X_S:
         /* S$ = ARGT$( X ) */
         {
            /* determine parameter type to the current USER DEFINED FUNCTION */
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
            if ( My->stack_head != NULL )
            {
               int             Loop;
               StackType * stack_item;
               Loop = TRUE;
               for (stack_item = My->stack_head; stack_item != NULL && Loop == TRUE; stack_item = stack_item->next)
               {
                  if (stack_item->LoopTopLine != NULL)
                  {
                     switch (stack_item->LoopTopLine->cmdnum)
                     {
                     case C_FUNCTION:
                     case C_SUB:
                        /* we have checked all the way to a FUNCTION or SUB boundary */
                        /* FOUND */
                        {
                           VariableType *v;


                           for (v = stack_item->local_variable; v != NULL && Loop == TRUE; v = v->next)
                           {
                              if (n == x)
                              {
                                 if ( VAR_IS_STRING( v ) )
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

      case F_ARGV4_X_S:
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
            if ( My->stack_head != NULL )
            {
               int             Loop;
               StackType * stack_item;
               Loop = TRUE;
               for (stack_item = My->stack_head; stack_item != NULL && Loop == TRUE; stack_item = stack_item->next)
               {
                  if (stack_item->LoopTopLine != NULL)
                  {
                     switch (stack_item->LoopTopLine->cmdnum)
                     {
                     case C_FUNCTION:
                     case C_SUB:
                        /* we have checked all the way to a FUNCTION or SUB boundary */
                        /* FOUND */
                        {
                           VariableType *v;


                           for (v = stack_item->local_variable; v != NULL && Loop == TRUE; v = v->next)
                           {
                              if (n == x)
                              {
                                 if ( VAR_IS_STRING( v ) )
                                 {
                                    s = v->memstr->length;
                                    bwb_memcpy(S, v->memstr->sbuffer, s);
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
            /* determine parameter value to the current USER DEFINED FUNCTION */
            int             Found;
            int             n;
            Found = FALSE;
            n = 0;
            if (x < 1)
            {
               /* bad param number  */
            }
            else
            if ( My->stack_head != NULL )
            {
               int             Loop;
               StackType * stack_item;
               Loop = TRUE;
               for (stack_item = My->stack_head; stack_item != NULL && Loop == TRUE; stack_item = stack_item->next)
               {
                  if (stack_item->LoopTopLine != NULL)
                  {
                     switch (stack_item->LoopTopLine->cmdnum)
                     {
                     case C_FUNCTION:
                     case C_SUB:
                        /* we have checked all the way to a FUNCTION or SUB boundary */
                        /* FOUND */
                        {
                           VariableType *v;


                           for (v = stack_item->local_variable; v != NULL && Loop == TRUE; v = v->next)
                           {
                              if (n == x)
                              {
                                 if ( VAR_IS_STRING( v ) )
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
      case F_BASE_N:
         /* N = BASE */
         {
            /* PNONE */
            N = My->CurrentVersion->OptionBaseValue;
         }
         break;
      case F_ASC_A_N:
      case F_ASCII_A_N:
      case F_CODE_A_N:
         /* N = ASC( A$ ) */
         /* N = ASCII( A$ ) */
         /* N = CODE( A$ ) */
         {
            /* P1BYT */
            N = A[0];
         }
         break;
      case F_ASC_A_X_N:
         /* N = ASC( A$, X ) */
         {
            /* P1BYT|P2POS */
            x--; /* BASIC -> C */
            if( x < a )
            {
                N = A[x];
            }
            else
            {
                IsError = 'X';
            }
         }
         break;
      case F_CDBL_X_N:
         /* N = CDBL( X ) */
         {
            /* P1DBL */
            N = X;
            /* argv->VariableType = BasicDoubleSuffix; */
         }
         break;
      case F_CSNG_X_N:
         /* N = CSNG( X ) */
         {
            /* P1FLT */
            N = X;
            /* argv->VariableType = BasicSingleSuffix; */
         }
         break;
      case F_CCUR_X_N:
         /* N = CCUR( X ) */
         {
            /* P1CUR */
            N = bwb_rint(X);
            /* argv->VariableType = BasicCurrencySuffix; */
         }
         break;
      case F_CLNG_X_N:
         /* N = CLNG( X ) */
         {
            /* P1LNG */
            N = bwb_rint(X);
            /* argv->VariableType = BasicLongSuffix; */
         }
         break;
      case F_CINT_X_N:
         /* N = CINT( X ) */
         {
            /* P1INT */
            N = bwb_rint(X);
            /* argv->VariableType = BasicIntegerSuffix; */
         }
         break;
      case F_MKD4_X_S:
         /* S$ = MKD$( X ) */
         {
            /* P1DBL */
            BasicDoubleType x;
            x = (BasicDoubleType) X;
            s = sizeof(BasicDoubleType);
            bwb_memcpy(S, &x, s);
         }
         break;
      case F_MKS4_X_S:
         /* S$ = MKS$( X ) */
         {
            /* P1FLT */
            BasicSingleType x;
            x = (BasicSingleType) X;
            s = sizeof(BasicSingleType);
            bwb_memcpy(S, &x, s);
         }
         break;
      case F_MKI4_X_S:
         /* S$ = MKI$( X ) */
         {
            /* P1INT */
            BasicIntegerType x;
            x = (BasicIntegerType) bwb_rint(X);
            s = sizeof(BasicIntegerType);
            bwb_memcpy(S, &x, s);
         }
         break;
      case F_MKL4_X_S:
         /* S$ = MKL$( X ) */
         {
            /* P1LNG */
            BasicLongType   x;
            x = (BasicLongType) bwb_rint(X);
            s = sizeof(BasicLongType);
            bwb_memcpy(S, &x, s);
         }
         break;
      case F_MKC4_X_S:
         /* S$ = MKC$( X ) */
         {
            /* P1CUR */
            BasicCurrencyType x;
            x = (BasicCurrencyType) bwb_rint(X);
            s = sizeof(BasicCurrencyType);
            bwb_memcpy(S, &x, s);
         }
         break;
      case F_CVD_A_N:
         /* N = CVD( A$ ) */
         {
            /* P1DBL */
            BasicDoubleType n;
            a = sizeof(BasicDoubleType);
            bwb_memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_CVS_A_N:
         /* N = CVS( X$ ) */
         {
            /* P1FLT */
            BasicSingleType n;
            a = sizeof(BasicSingleType);
            bwb_memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_CVI_A_N:
         /* N = CVI( X$ ) */
         {
            /* P1INT */
            BasicIntegerType n;
            a = sizeof(BasicIntegerType);
            bwb_memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_CVL_A_N:
         /* N = CVL( X$ ) */
         {
            /* P1LNG */
            BasicLongType   n;
            a = sizeof(BasicLongType);
            bwb_memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_CVC_A_N:
         /* N = CVC( X$ ) */
         {
            /* P1CUR */
            BasicCurrencyType n;
            a = sizeof(BasicCurrencyType);
            bwb_memcpy(&n, A, a);
            N = n;
         }
         break;
      case F_ENVIRON4_A_S:
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
               s = bwb_strlen(CharPointer);
               s = MIN(s, BasicStringLengthMax);
               if (s == 0)
               {
                  /* empty string */
               }
               else
               {
                  bwb_memcpy(S, CharPointer, s);
               }
            }
         }
         break;
      case F_ENVIRON_A_N:
         /* ENVIRON A$ */
         {
            /* P1BYT */

            char           *CharPointer;

            CharPointer = bwb_strchr(A, '=');
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

            while (*A == ' ')
            {
               A++;  /* LTRIM$ */
            }
            IsError = bwb_file_open( *A, x, B, y );
         }
         break;
      case F_OPEN_A_X_B_N:
         /* default  LEN is 128 for RANDOM, 0 for all others */
         /* OPEN "I"|"O"|"R"|"A", [#]n, filename [,rlen] */
         {
            /* P1STR|P2NUM|P3STR|P4NUM */
            /* P1BYT|P2DEV|P3BYT|P4INT */
            int             y = 0;

            while (*A == ' ')
            {
               A++;  /* LTRIM$ */
            }
            if( bwb_toupper(*A) == 'R' )
            {
               /* default RANDOM record size */
               y = 128;
            }
            IsError = bwb_file_open( *A, x, B, y );
         }
         break;
      case F_LOC_X_N:
         /* N = LOC( X ) */
         {
            /* P1DEV */
            if( x <= 0 )
            {
               N = 0;
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  N = 0;
               }
               else
               if( F == My->SYSOUT )
               {
                  N = 0;
               }
               else
               if( F == My->SYSPRN )
               {
                  N = 0;
               }
               else
               {
                  FILE           *fp;
                  fp = F->cfp;
                  N = ftell(fp);
                  if( My->CurrentVersion->OptionVersionBitmask & (G65 | G67)  )
                  {
                     /* byte position, regardless of 'mode' */
                  }
                  else
                  if (F->mode == DEVMODE_RANDOM)
                  {
                     /* record number */
                     if( F->width == 0 )
                     {
                        /* byte position */
                     }
                     else
                     {
                        N /= F->width;
                     }
                  }
                  else
                  if (F->mode == DEVMODE_BINARY)
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
         }
         break;
      case F_SEEK_X_N:
         /* N = SEEK( X ) */
         {
            /* P1DEV */
            if( x <= 0 )
            {
               N = 0;
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  N = 0;
               }
               else
               if( F == My->SYSOUT )
               {
                  N = 0;
               }
               else
               if( F == My->SYSPRN )
               {
                  N = 0;
               }
               else
               {
                  FILE           *fp;
                  fp = F->cfp;
                  N = ftell(fp);
                  if (F->mode == DEVMODE_RANDOM)
                  {
                     /* record number */
                     if( F->width > 0 )
                     {
                        N /= F->width;
                     }
                  }
                  else
                  {
                     /* byte positiion */
                  }
                  N = floor(N);
                  N++;  /* C to BASIC */
               }
            }
         }
         break;
      case F_SEEK_X_Y_N:
         /* SEEK X, Y */
         {
            /* P1DEV|P2INT */
            if( x <= 0 )
            {
               IsError = 'X';
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSOUT )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSPRN )
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
                  if (F->mode == DEVMODE_RANDOM)
                  {
                     if( F->width > 0 )
                     {
                        offset *= F->width;
                     }
                  }
                  if (fseek(F->cfp, offset, SEEK_SET) != 0)
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
         }
         break;
      case F_LOF_X_N:
         /* N = LOF( X ) */
         {
            /* P1DEV */
            if( x <= 0 )
            {
               N = 0;
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  N = 0;
               }
               else
               if( F == My->SYSOUT )
               {
                  N = 0;
               }
               else
               if( F == My->SYSPRN )
               {
                  N = 0;
               }
               else
               {
                  /* file size in bytes */
                  FILE           *fp;
                  long            current;
                  long            total;
                  fp = F->cfp;
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
         }
         break;
      case F_EOF_X_N:
         /* N = EOF( X ) */
         {
            /* P1DEV */
            if( x <= 0 )
            {
               N = 0;
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  N = 0;
               }
               else
               if( F == My->SYSOUT )
               {
                  N = 0;
               }
               else
               if( F == My->SYSPRN )
               {
                  N = 0;
               }
               else
               {
                  /* are we at the end? */
                  N = bwb_is_eof( F->cfp );
               }     
            }
         }
         break;
      case F_FILEATTR_X_Y_N:
         /* N = FILEATTR( X, Y ) */
         {
            /* P1DEV|P2INT */

            if( x <= 0 )
            {
               /* Printer and Console */
               IsError = 'X';
            }
            else
            if( y == 1 )
            {
               FileType * F;

               F = find_file_by_number( x );
               if( F == NULL )
               {
                  /* normal CLOSED file */
                  N = 0;
               }
               else
               {
                  /* normal OPEN file */
                  N = F->mode;
               }
            }
            else
            if( y == 2 )
            {
               N = 0;
            }
            else
            {
               IsError = 'Y';
            }
         }
         break;
      case F_CLOSE_X_N:
         /* CLOSE X */
         {
            /* P1DEV */            
            if( x <= 0 )
            {
               /* Printer and Console */
               IsError = 'X';
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSOUT )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSPRN )
               {
                  IsError = 'X';
               }
               else
               {
                  field_close_file( F );
                  file_clear( F );
                  N = 0;
               }
            }
         }
         break;
      case F_RESET_N:
      case F_CLOSE_N:
         /* RESET */
         /* CLOSE */
         {
            /* PNONE */
            FileType * F;

            for (F = My->file_head; F != NULL; F = F->next)
            {
               field_close_file( F );
               file_clear( F );
            }
            N = 0;
         }
         break;
      case F_FREEFILE_N:
         /* N = FREEFILE */
         {
            /* PNONE */
            int             x;
            FileType * F;

            x = 0;
            for (F = My->file_head; F != NULL; F = F->next)
            {
               if (F->mode != DEVMODE_CLOSED)
               {
                  if( F->FileNumber > x )
                  {
                     x = F->FileNumber;
                  }
               }
            }
            /* 'x' is the highest FileNumber that is currently open */
            x++;
            if( x > BasicFileNumberMax )
            {
               x = 0;
            }
            N = x;
         }
         break;
      case F_GET_X_Y_N:
         /* GET X, Y */
         {
            /* P1DEV|P2INT */
            if( x <= 0 )
            {
               /* Printer and Console */
               IsError = 'X';
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSOUT )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSPRN )
               {
                  IsError = 'X';
               }
               else
               if (F->mode != DEVMODE_RANDOM)
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
                  long offset;
                  offset = y;
                  offset--;   /* BASIC to C */
                  if (F->mode == DEVMODE_RANDOM)
                  {
                     if( F->width > 0 )
                     {
                        offset *= F->width;
                     }
                  }
                  if (fseek(F->cfp, offset, SEEK_SET) != 0)
                  {
                     IsError = 'Y';
                  }
                  else
                  {
                     int i;
                     for (i = 0; i < F->width; i++)
                     {
                        F->buffer[i] = fgetc(F->cfp);
                     }
                     field_get( F );
                     N = 0;
                  }
               }
            }
         }
         break;
      case F_GET_X_N:
         /* GET X */
         {
            /* PDEV1 */
            if( x <= 0 )
            {
               /* Printer and Console */
               IsError = 'X';
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSOUT )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSPRN )
               {
                  IsError = 'X';
               }
               else
               if (F->mode != DEVMODE_RANDOM)
               {
                  IsError = 'X';
               }
               else
               {
                  {
                     int i;
                     for (i = 0; i < F->width; i++)
                     {
                        F->buffer[i] = fgetc(F->cfp);
                     }
                     field_get( F );
                     N = 0;
                  }
               }
            }
         }
         break;
      case F_PUT_X_Y_N:
         /* PUT X, Y */
         {
            /* P1DEV|P2INT */
            if( x <= 0 )
            {
               /* Printer and Console */
               IsError = 'X';
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSOUT )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSPRN )
               {
                  IsError = 'X';
               }
               else
               if (F->mode != DEVMODE_RANDOM)
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
                  long offset;
                  offset = y;
                  offset--;   /* BASIC to C */
                  if (F->mode == DEVMODE_RANDOM)
                  {
                     if( F->width > 0 )
                     {
                        offset *= F->width;
                     }
                  }
                  if (fseek(F->cfp, offset, SEEK_SET) != 0)
                  {
                     IsError = 'Y';
                  }
                  else
                  {
                     int i;
                     field_put( F );
                     for (i = 0; i < F->width; i++)
                     {
                        fputc(F->buffer[i], F->cfp);
                        F->buffer[i] = ' '; /* flush  */
                     }
                     N = 0;
                  }
               }
            }
         }
         break;
      case F_PUT_X_N:
         /* PUT X */
         {
            /* P1DEV */
            if( x <= 0 )
            {
               /* Printer and Console */
               IsError = 'X';
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSIN )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSOUT )
               {
                  IsError = 'X';
               }
               else
               if( F == My->SYSPRN )
               {
                  IsError = 'X';
               }
               else
               if (F->mode != DEVMODE_RANDOM)
               {
                  IsError = 'X';
               }
               else
               {
                  {
                     int i;
                     field_put( F );
                     for (i = 0; i < F->width; i++)
                     {
                        fputc(F->buffer[i], F->cfp);
                        F->buffer[i] = ' '; /* flush  */
                     }
                     N = 0;
                  }
               }
            }
         }
         break;
      case F_WIDTH_X_N:
         /* WIDTH X */
         {
            /* P1BYT */
            /* console is #0 */
            My->SYSIN->width = x;
            My->SYSIN->col = 1;
            My->SYSOUT->width = x;
            My->SYSOUT->col = 1;
            N = 0;
         }
         break;
      case F_WIDTH_X_Y_N:
         /* WIDTH X, Y */
         {
            /*  WIDTH #file, cols */
            /* P1BYT|PB2YT */
            if( x == 0 )
            {
               My->SYSIN->width = y;
               My->SYSOUT->width = y;
               N = 0;
            }
            else
            if( x < 0 )
            {
               My->SYSPRN->width = y;
               N = 0;
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  /* WIDTH rows, cols */
                  My->SCREEN_ROWS = x;
                  My->SYSIN->width = y;
                  My->SYSIN->col = 1;
                  My->SYSOUT->width = y;
                  My->SYSOUT->col = 1;
                  N = 0;
               }
               else
               if (F->mode == DEVMODE_RANDOM)
               {
                  IsError = 'X';
               }
               else
               {
                  /* WIDTH # file, cols */
                  F->width = y;
                  F->col = 1;
                  N = 0;
               }
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
               n = a - b;  /* last valid search position */
               n++;

               x--;  /* BASIC to C */
               A += x;  /* advance to the start
                   * position */
               for (i = x; i < n; i++)
               {
                  if (bwb_memcmp(A, B, b) == 0)
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
      case F_INDEX_A_B_N:
         /* N = INSTR( A$, B$ ) */
         /* N = INDEX( A$, B$ ) */
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
                  if (bwb_memcmp(A, B, b) == 0)
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
      case F_SPACE4_X_S:
      case F_SPACE_X_S:
      case F_SPA_X_S:
         /* S$ = SPACE$( X ) */
         /* S$ = SPACE( X ) */
         /* S$ = SPA( X ) */
         {
            /* P1LEN */
            if (x == 0)
            {
               /* no copies */
            }
            else
            {
               bwb_memset(S, (char) ' ', x);
               s = x;
            }
         }
         break;
      case F_STRING4_X_Y_S:
      case F_STRING_X_Y_S:
      case F_STR_X_Y_S:
         /* S$ = STRING$( X, Y ) */
         /* S$ = STRING( X, Y ) */
         /* S$ = STR( X, Y ) */
         {
            /* P1LEN|P2BYT */
            if (x == 0)
            {
               /* no copies */
            }
            else
            {
               bwb_memset(S, (char) y, x);
               s = x;
            }
         }
         break;
      case F_STRING4_X_A_S:
         /* S$ = STRING$( X, A$ ) */
         {
            /* P1LEN|P2BYT */
            if (x == 0)
            {
               /* no copies */
            }
            else
            {
               bwb_memset(S, (char) A[0], x);
               s = x;
            }
         }
         break;
      case F_LIN_X_S:
         /* S$ = LIN( X ) */
         {
            /* P1LEN */
            if (x == 0)
            {
               /* no copies */
            }
            else
            {
               bwb_memset(S, (char) '\n', x);
               s = x;
            }
         }
         break;
      case F_MID4_A_X_S:
      case F_MID_A_X_S:
         /* S$ = MID$( A$, X ) */
         /* S$ = MID( A$, X ) */
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
               bwb_memcpy(S, A, a);
               s = a;
            }
         }
         break;
      case F_MID4_A_X_Y_S:
      case F_MID_A_X_Y_S:
      case F_SEG4_A_X_Y_S:
      case F_SEG_A_X_Y_S:
         /* S$ = MID$( A$, X, Y ) */
         /* S$ = MID( A$, X, Y ) */
         /* S$ = SEG$( A$, X, Y ) */
         /* S$ = SEG( A$, X, Y ) */
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
               bwb_memcpy(S, A, a);
               s = a;
            }
         }
         break;
      case F_LEFT4_A_X_S:
      case F_LEFT_A_X_S:
         /* S$ = LEFT$( A$, X ) */
         /* S$ = LEFT( A$, X ) */
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
               bwb_memcpy(S, A, a);
               s = a;
            }
         }
         break;
      case F_RIGHT4_A_X_S:
      case F_RIGHT_A_X_S:
         /* S$ = RIGHT$( A$, X ) */
         /* S$ = RIGHT( A$, X ) */
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
               bwb_memcpy(S, A, x);
               s = x;
            }
         }
         break;
      case F_HEX4_X_S:
         /* S$ = HEX$( X ) */
         {
            sprintf(S, "%X", x);
            s = bwb_strlen(S);
         }
         break;
      case F_OCT4_X_S:
         /* S$ = OCT$( X ) */
         {
            sprintf(S, "%o", x);
            s = bwb_strlen(S);
         }
         break;
      case F_CHR_X_S:
      case F_CHR4_X_S:
      case F_CHAR4_X_S:
         /* S$ = CHR( X ) */
         /* S$ = CHR$( X ) */
         /* S$ = CHAR$( X ) */

         /* P1ANY */
         if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
         {
            /* IBM System/360 & System/370 BASIC dialects: the opposite of N = NUM( A$ ) */
            BasicNumerc(X, S);
            s = bwb_strlen(S);
         }
         else
         {
            if( x < 0 || x > 255 )
            {
               IsError = 'X';
            }
            else
            {
               S[0] = (char) x;
               s = 1;
            }
         }
         break;
      case F_CHAR_X_Y_S:
         /* S$ = CHAR( X, Y ) ' same as STRING$(Y,X) */
         {
            /* P1BYT|P2LEN */
            if (y == 0)
            {
               /* no copies */
            }
            else
            {
               bwb_memset(S, (char) x, y);
               s = y;
            }
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
                  if (bwb_memcmp(A, B, b) == 0)
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
      case F_MATCH_A_B_X_N:
         /* N = POS( A$, B$, X ) */
         {
            N = str_match( A, a, B, b, x );
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
               n = a - b;  /* last valid search position */
               n++;

               /* search */
               x--;  /* BASIC to C */
               A += x;  /* advance to the start
                   * position */
               for (i = x; i < n; i++)
               {
                  if (bwb_memcmp(A, B, b) == 0)
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
      case F_NUM_A_N:
         /* N = VAL( A$ ) */
         /* N = NUM( A$ ) */
         {
            /* P1ANY */
            int             ScanResult;
            BasicNumberType Value;
            ScanResult = sscanf(A, BasicNumberScanFormat, &Value);
            if (ScanResult != 1)
            {
               /* not a number */
               if (My->CurrentVersion->OptionFlags & OPTION_BUGS_ON /* VAL("X") = 0 */ )
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
      case F_STR4_X_S:
      case F_NUM4_X_S:
         /* S$ = STR$( X ) */
         /* S$ = NUM$( X ) */
         {
            /* P1ANY */
            BasicNumerc(X, S);
            s = bwb_strlen(S);
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
      case F_DATE4_X_S:
      case F_DATE4_S:
      case F_DAT4_S:
         /* S$ = DATE$( X ) ' value of X is ignored */
         /* S$ = DATE$ */
         /* S$ = DAT$ */
         {
            /* PNONE */
            time(&t);
            lt = localtime(&t);
            s = strftime(S, BasicStringLengthMax, My->CurrentVersion->OptionDateFormat, lt);
         }
         break;
      case F_CLK_X_S:
      case F_CLK4_S:
      case F_TI4_S:
      case F_TIME4_S:
      case F_TIME4_X_S:
         /* S$ = CLK(X) ' the value of paameter X is ignored */
         /* S$ = CLK$ */
         /* S$ = TI$ */
         /* S$ = TIME$ */
         /* S$ = TIME$(X) ' the value of paameter X is ignored */
         {
            /* PNONE */
            time(&t);
            lt = localtime(&t);
            s = strftime(S, BasicStringLengthMax, My->CurrentVersion->OptionTimeFormat, lt);
         }
         break;
      case F_TI_N:
      case F_TIM_N:
      case F_TIME_N:
      case F_TIME_X_N:
      case F_TIMER_N:
         /* N = TI */
         /* N = TIM */
         /* N = TIME */
         /* N = TIME( X ) ' value of X is ignored */
         /* N = TIMER */
         /* N = CPU */
         {
            /* PNONE */
            time(&t);
            lt = localtime(&t);
            if( My->CurrentVersion->OptionVersionBitmask & ( G67 ) )
            {
               N = lt->tm_hour;
               N *= 60;
               N += lt->tm_min;
               N *= 60;
               N += lt->tm_sec;
               /* number of seconds since midnight */
               N -= My->StartTime; 
               /* elapsed run time */
            }
            else
            {
               N = lt->tm_hour;
               N *= 60;
               N += lt->tm_min;
               N *= 60;
               N += lt->tm_sec;
               /* number of seconds since midnight */
            }
         }
         break;
      case F_CLK_X_N:
         /* N = CLK( X ) ' value of X is ignored */
         {
            /* PNONE */
            time(&t);
            lt = localtime(&t);
            N = lt->tm_hour;
            N *= 60;
            N += lt->tm_min;
            N *= 60;
            N += lt->tm_sec;
            N /= 3600;
            /* decimal hours: 3:30 PM = 15.50 */
         }
         break;
      
      case F_TIM_X_N:
         /* N = TIM( X ) */
         {
            /* P1BYT */
            time(&t);
            lt = localtime(&t);
            
            if( My->CurrentVersion->OptionVersionBitmask & (G65 | G67) )
            {
               /* value of 'X' is ignored */
               N = lt->tm_hour;
               N *= 60;
               N += lt->tm_min;
               N *= 60;
               N += lt->tm_sec;
               /* number of seconds since midnight */
               N -= My->StartTime; 
               /* elapsed run time */
            }
            else
            {
               switch( x )
               {
               case 0:
                  /* TIM(0) == minute (0..59) */
                  N += lt->tm_min;
                  break;
               case 1:
                  /* TIM(1) == hour (0..23) */
                  N = lt->tm_hour;
                  break;
               case 2:
                  /* TIM(2) == day of year (1..366) */
                  N = 1 + lt->tm_yday;
                  break;
               case 3:
                  /* TIM(3) == year since 1900  (0..) */
                  N = lt->tm_year;
                  break;
               default:
                  IsError = 'X';
               }
            }
         }
         break;
      case F_COMMAND4_X_S:
         /* S$ = COMMAND$(X) */
         if( x < 0 || x > 9 )
         {
            IsError = 'X';
         }
         else
         {
            if( My->COMMAND5[x] == NULL )
            {
               s = 0;
            }
            else
            {
               bwb_strcpy( S, My->COMMAND5[x] );
               s = bwb_strlen( My->COMMAND5[x] );
            }
         }
         break;
      case F_COSH_X_N:
      case F_CSH_X_N:
      case F_HCS_X_N:
         /* N = COSH( X ) */
         /* N = CSH( X ) */
         /* N = HCS( X ) */
         {
            /* P1ANY */
            N = cosh(X);
         }
         break;
      case F_SINH_X_N:
      case F_SNH_X_N:
      case F_HSN_X_N:
         /* N = SINH( X ) */
         /* N = SNH( X ) */
         /* N = HSN( X ) */
         {
            /* P1ANY */
            N = sinh(X);
         }
         break;
      case F_TANH_X_N:
      case F_HTN_X_N:
         /* N = TANH( X ) */
         /* N = HTN( X ) */
         {
            /* P1ANY */
            N = tanh(X);
         }
         break;
      case F_CLG_X_N:
      case F_CLOG_X_N:
      case F_LOG10_X_N:
      case F_LGT_X_N:
         /* N = CLG( X ) */
         /* N = CLOG( X ) */
         /* N = LOG10( X ) */
         /* N = LGT( X ) */
         {
            /* P1GTZ */
            N = log10(X);
         }
         break;
      case F_SLEEP_X_N:
      case F_WAIT_X_N:
      case F_PAUSE_X_N:
         /* N = SLEEP( X ) */
         /* N = WAIT( X ) */
         /* N = PAUSE( X ) */
         {
            /* P1ANY */
            X = X * My->OptionSleepValue;
            if( X <= 0 || X > SHRT_MAX)
            {
               /* do nothing */
            }
            else
            {
               x = (int) bwb_rint( X );
               sleep(x);
            }
         }
         break;
      case F_LOG2_X_N:
      case F_LTW_X_N:
         /* N = LOG2( X ) */
         /* N = LTW( X ) */
         {
            /* P1GTZ */
            N = log(X) / log((BasicNumberType) 2);
         }
         break;
      case F_ACOS_X_N:
      case F_ACS_X_N:
         /* N = ACOS( X ) */
         /* N = ACS( X ) */
         {
            /* P1ANY */
            if (X < -1 || X > 1)
            {
               IsError = 'X';
            }
            else
            {
               N = acos(X);
               if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
               {
                  N = FromRadiansToDegrees( N );
               }
               else
               if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
               {
                  N = FromRadiansToGradians( N );
               }
            }
         }
         break;
      case F_ACSD_X_N:
         /* N = ACSD( X ) */
         {
            /* P1ANY */
            if (X < -1 || X > 1)
            {
               IsError = 'X';
            }
            else
            {
               N = acos(X);
               /* result is always in DEGREES, regardless of OPTION ANGLE setting */
               N = FromRadiansToDegrees( N );
            }
         }
         break;
      case F_ACSG_X_N:
         /* N = ACSG( X ) */
         {
            /* P1ANY */
            if (X < -1 || X > 1)
            {
               IsError = 'X';
            }
            else
            {
               N = acos(X);
               /* result is always in GRADIANS, regardless of OPTION ANGLE setting */
               N = FromRadiansToGradians( N );
            }
         }
         break;

      case F_ASIN_X_N:
      case F_ASN_X_N:
      case F_ARCSIN_X_N:
         /* N = ASIN( X ) */
         /* N = ASN( X ) */
         /* N = ARCSIN( X ) */
         {
            /* P1ANY */
            if (X < -1 || X > 1)
            {
               IsError = 'X';
            }
            else
            {
               N = asin(X);
               if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
               {
                  N = FromRadiansToDegrees( N );
               }
               else
               if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
               {
                  N = FromRadiansToGradians( N );
               }
            }
         }
         break;


      case F_ASND_X_N:
         /* N = ASND( X ) */
         {
            /* P1ANY */
            if (X < -1 || X > 1)
            {
               IsError = 'X';
            }
            else
            {
               N = asin(X);
               /* result is always in DEGREES, regardless of OPTION ANGLE setting */
               N = FromRadiansToDegrees( N );
            }
         }
         break;
      case F_ASNG_X_N:
         /* N = ASNG( X ) */
         {
            /* P1ANY */
            if (X < -1 || X > 1)
            {
               IsError = 'X';
            }
            else
            {
               N = asin(X);
               /* result is always in GRADIANS, regardless of OPTION ANGLE setting */
               N = FromRadiansToGradians( N );
            }
         }
         break;


      case F_COT_X_N:
         /* N = COT( X ) ' = 1 / TAN( X ) */
         {
            /* P1ANY */
            BasicNumberType T;
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = FromDegreesToRadians( X );
            }
            else
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
            {
               X = FromGradiansToRadians( X );
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
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = FromDegreesToRadians( X );
            }
            else
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
            {
               X = FromGradiansToRadians( X );
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
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = FromDegreesToRadians( X );
            }
            else
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
            {
               X = FromGradiansToRadians( X );
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
      case F_UCASE4_A_S:
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
               bwb_memcpy(S, A, a);
               s = a;
               /* BASIC allows embedded NULL
                * characters */
               for (i = 0; i < a; i++)
               {
                  S[i] = bwb_toupper(S[i]);
               }
            }
         }
         break;
      case F_LCASE4_A_S:
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
               bwb_memcpy(S, A, a);
               s = a;
               /* BASIC allows embedded NULL
                * characters */
               for (i = 0; i < a; i++)
               {
                  S[i] = bwb_tolower(S[i]);
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
               if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
               {
                  N = FromRadiansToDegrees( N );
               }
               else
               if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
               {
                  N = FromRadiansToGradians( N );
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
      case F_DET_N:
         /* N = DET */
         {
            /* PNONE */
            N = My->LastDeterminant;
         }
         break;
      case F_NUM_N:
         /* N = NUM */
         {
            /* PNONE */
            N = My->LastInputCount;
         }
         break;
      case F_DEG_N:
      case F_DEGREE_N:
         /* N = DEG */
         /* N = DEGREE */
         {
            /* PNONE */
            My->CurrentVersion->OptionFlags |= OPTION_ANGLE_DEGREES;
            My->CurrentVersion->OptionFlags &= ~OPTION_ANGLE_GRADIANS;
            N = 0;
         }
         break;
      case F_RAD_N:
      case F_RADIAN_N:
         /* N = RAD */
         /* N = RADIAN */
         {
            /* PNONE */
            My->CurrentVersion->OptionFlags &= ~OPTION_ANGLE_DEGREES;
            My->CurrentVersion->OptionFlags &= ~OPTION_ANGLE_GRADIANS;
            N = 0;
         }
         break;
      case F_GRAD_N:
      case F_GRADIAN_N:
         /* N = GRAD */
         /* N = GRADIAN */
         {
            /* PNONE */
            My->CurrentVersion->OptionFlags &= ~OPTION_ANGLE_DEGREES;
            My->CurrentVersion->OptionFlags |= OPTION_ANGLE_GRADIANS;
            N = 0;
         }
         break;
      case F_DEG_X_N:
      case F_DEGREE_X_N:
         /* N = DEG( X ) */
         /* N = DEGREE( X ) */
         {
            /* P1ANY */
            N = FromRadiansToDegrees( X );
         }
         break;
      case F_RAD_X_N:
         /* N = RAD( X ) */
         {
            /* P1ANY */
            N = FromDegreesToRadians( X );
         }
         break;
      case F_PI_N:
         /* N = PI */
         {
            /* PNONE */
            N = PI;
         }
         break;
      case F_PI_X_N:
         /* N = PI(X) */
         {
            /* P1ANY */
            N = PI * X;
         }
         break;
      case F_LTRIM4_A_S:
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
                  bwb_memcpy(S, A, a);
                  s = a;
               }
            }
         }
         break;
      case F_RTRIM4_A_S:
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
                  bwb_memcpy(S, A, a);
                  s = a;
               }
            }
         }
         break;
      case F_TRIM4_A_S:
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
                  bwb_memcpy(S, A, a);
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
                        /* bwb_memcpy( S,
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
      case F_MAX_A_B_S:
         /* S$ = MAX( A$, B$ ) */
         {
            StringType L;
            StringType R;
            
            L.length = a;
            R.length = b;
            L.sbuffer = A;
            R.sbuffer = B;
            if( str_cmp( &L, &R ) >= 0 )
            {
               /* A >= B */
               bwb_memcpy( S, A, a );
               s = a;
            }
            else
            {
               /* A < B */
               bwb_memcpy( S, B, b );
               s = b;
            }
         }
         break;
      case F_MIN_X_Y_N:
         /* N = MIN( X, Y ) */
         {
            N = MIN(X, Y);
         }
         break;
      case F_MIN_A_B_S:
         /* S$ = MIN( A$, B$ ) */
         {
            StringType L;
            StringType R;
            
            L.length = a;
            R.length = b;
            L.sbuffer = A;
            R.sbuffer = B;
            if( str_cmp( &L, &R ) <= 0 )
            {
               /* A <= B */
               bwb_memcpy( S, A, a );
               s = a;
            }
            else
            {
               /* A > B */
               bwb_memcpy( S, B, b );
               s = b;
            }
         }
         break;
      case F_FP_X_N:
      case F_FRAC_X_N:
         /* N = FP( X ) */
         /* N = FRAC( X ) */
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
               /* same as ASC(A$) */
               N = A[0];
            }
            else
            {
               /* lookup Acronym */
               int             c;
               N = -1;  /* not found */
               for (c = 0; c < NUM_ACRONYMS; c++)
               {
                  if (bwb_stricmp(AcronymTable[c].Name, A) == 0)
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
      case F_RENAME_A_B_N:
         /* N = RENAME( A$, B$ ) */
         {
            /* P1BYT | P2BYT */
            if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
            {
               strupper( A ); /* CBASIC-II: file names are always upper case */
               strupper( B ); /* CBASIC-II: file names are always upper case */
            }
            if( rename( A, B ) )
            {
               /* ERROR -- return FALSE */
               N = 0;
            }
            else
            {
               /* OK -- return TRUE */
               N = -1;
            }
         }
         break;
      case F_SIZE_A_N:
         /* N = SIZE( A$ ) */
         {
            /* P1BYT */
            FILE * F;
            
            if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
            {
               strupper( A ); /* CBASIC-II: file names are always upper case */
            }

            F = fopen( A, "rb" );
            if( F != NULL )
            {
               long n;

               fseek( F, 0, SEEK_END );
               n = ftell( F );
               fclose( F );

               if( n > 0 )
               {
                  /* round up filesize to next whole kilobyte */
                  n += 1023;
                  n /= 1024;
               }
               else
               {
                  /* a zero-length file returns 0 */
                  n = 0;
               }
               N = n;
            }
            /* a non-existing file returns 0 */
         }
         break;
      case F_REPEAT4_X_Y_S:
         /* S$ = REPEAT$( X, Y ) ' X is count, Y is code */
         {
            /* P1LEN | P2BYT */
            if (x == 0)
            {
               /* empty string */
            }
            else
            {
               bwb_memset(S, (char) y, x);
               s = x;
            }
         }
         break;
      case F_REPEAT4_X_A_S:
         /* S$ = REPEAT$( X, A$ ) ' X is count, A$ is code */
         {
            /* P1LEN | P2BYT */
            if (x == 0)
            {
               /* empty string */
            }
            else
            {
               bwb_memset(S, (char) A[0], x);
               s = x;
            }
         }
         break;
      case F_FIX_X_N:
         /* N = FIX( X ) */
         {
            /* N = bwb_rint(X); */
            if( X < 0 )
            {
               N = -floor(-X);
            }
            else
            {
               N = floor(X);
            }
         }
         break;
      case F_ABS_X_N:
         /* N = ABS( X ) */
         {
            N = fabs(X);
         }
         break;
      case F_ATN_X_N:
      case F_ATAN_X_N:
      case F_ARCTAN_X_N:
         /* N = ATN( X ) */
         /* N = ATAN( X ) */
         /* N = ARCTAN( X ) */
         {
            N = atan(X);
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
            {
               N = FromRadiansToDegrees( N );
            }
            else
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
            {
               N = FromRadiansToGradians( N );
            }
         }
         break;
      case F_ATND_X_N:
         /* N = ATND( X ) */
         {
            N = atan(X);
            N = FromRadiansToDegrees( N );
         }
         break;
      case F_ATNG_X_N:
         /* N = ATNG( X ) */
         {
            N = atan(X);
            N = FromRadiansToGradians( N );
         }
         break;
      case F_COS_X_N:
         /* N = COS( X ) */
         {
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = FromDegreesToRadians( X );
            }
            else
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
            {
               X = FromGradiansToRadians( X );
            }
            N = cos(X);
         }
         break;
      case F_COSD_X_N:
         /* N = COSD( X ) */
         {
            X = FromDegreesToRadians( X );
            N = cos(X);
         }
         break;
      case F_COSG_X_N:
         /* N = COSG( X ) */
         {
            X = FromGradiansToRadians( X );
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
      case F_FLOAT_X_N:
      case F_INT5_X_N:
         /* N = FLOAT( X ) */
         /* N = INT%( X ) */
         {
            N = bwb_rint(X);
         }
         break;
      case F_INITIALIZE_N:
         /* INITIALIZE */
         {
            N = 0;
         }
         break;
      case F_LOG_X_N:
      case F_LN_X_N:
      case F_LOGE_X_N:
         /* N = LOG( X ) */
         /* N = LN( X ) */
         /* N = LOGE( X ) */
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
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = FromDegreesToRadians( X );
            }
            else
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
            {
               X = FromGradiansToRadians( X );
            }
            N = sin(X);
         }
         break;
      case F_SIND_X_N:
         /* N = SIND( X ) */
         {
            X = FromDegreesToRadians( X );
            N = sin(X);
         }
         break;
      case F_SING_X_N:
         /* N = SING( X ) */
         {
            X = FromGradiansToRadians( X );
            N = sin(X);
         }
         break;
      case F_SQR_X_N:
      case F_SQRT_X_N:
         /* N = SQR( X ) */
         /* N = SQRT( X ) */
         {
            /* P1GEZ */
            N = sqrt(X);
         }
         break;
      case F_TAN_X_N:
         /* N = TAN( X ) */
         {
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_DEGREES)
            {
               X = FromDegreesToRadians( X );
            }
            else
            if (My->CurrentVersion->OptionFlags & OPTION_ANGLE_GRADIANS)
            {
               X = FromGradiansToRadians( X );
            }
            N = tan(X);
         }
         break;
      case F_TAND_X_N:
         /* N = TAND( X ) */
         {
            X = FromDegreesToRadians( X );
            N = tan(X);
         }
         break;
      case F_TANG_X_N:
         /* N = TANG( X ) */
         {
            X = FromGradiansToRadians( X );
            N = tan(X);
         }
         break;
      case F_SPC_X_S:
         /* S$ = SPC( X ) */
         {
            /* P1ANY */
            /* SPECIAL RULES APPLY.  PART OF PRINT COMMAND.  WIDTH > 0 */
            X = bwb_rint(X);
            if (X < 1 || X > 255)
            {
               if( bwb_Warning_Overflow("*** WARNING: INVALID SPC() ***") )
               {
                  /* ERROR */
               }
               /* CONTINUE */
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
            /* SPECIAL RULES APPLY.  PART OF PRINT COMMAND.  WIDTH > 0 */
            X = bwb_rint(X);
            if (X < 1 || X > 255)
            {
               if( bwb_Warning_Overflow("*** WARNING: INVALID TAB() ***") )
               {
                  /* ERROR */
               }
               /* CONTINUE */
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
            N = My->SYSOUT->col;
         }
         break;
      case F_COUNT_N:
         /* N = COUNT */
         /* COUNT = POS - 1 */
         {
            /* PNONE */
            N = My->SYSOUT->col;
            N--;
         }
         break;
      case F_POS_X_N:
         /* N = POS( X ) */
         {
            /* P1DEV */
            if( x == 0 )
            {
               N = My->SYSOUT->col;
            }
            else
            if( x < 0 )
            {
               N = My->SYSPRN->col;
            }
            else
            {
               FileType * F;
               F = find_file_by_number( x );
               if( F == NULL )
               {
                  IsError = 'X';
               }
               else
               {
                  N = F->col;
               }
            }
         }
         break;
      case F_INPUT4_X_Y_S:
         /* S$ = INPUT$( X, Y )  */
         {
            /* P1LEN|P2DEV */
            if( y <= 0 )
            {
               IsError = 'Y';
            }
            else
            {
               FileType * F;
   
               F = find_file_by_number( y );
               if( F == NULL )
               {
                  IsError = 'Y';
               }
               else
               {
                  if ((F->mode & DEVMODE_READ) == 0)
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
                     fp = F->cfp;
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
      case F_ERRN_N:
         /* N = ERR */
         /* N = ERRN */
         {
            /* PNONE */
            N = My->err_number;
         }
         break;
      case F_ERL_N:
      case F_ERRL_N:
         /* N = ERL */
         /* N = ERRL */
         {
            /* PNONE */
            if( My->err_line != NULL )
            {
                N = My->err_line->number;
            }
         }
         break;
      case F_ERR4_S:
      case F_ERROR4_S:
         /* S = ERR$ */
         /* S = ERROR$ */
         {
            /* PNONE */
            s = bwb_strlen(My->ErrMsg);
            if (s > 0)
            {
               bwb_strcpy(S, My->ErrMsg);
            }
         }
         break;


         /********************************************************************************************
         **  Keep the platform specific functions together.
         *********************************************************************************************/
      case F_INP_X_N:
      case F_PIN_X_N:
         /* N = INP( X ) */
         /* N = PIN( X ) */
         {
            /* P1BYT */
            IsError = 0xFF;
         }
         break;
      case F_PDL_X_N:
         /* N = PDL( X ) */
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
      case F_EXAM_X_N:
      case F_FETCH_X_N:
         /* N = PEEK( X ) */
         /* N = EXAM( X ) */
         /* N = FETCH( X ) */
         {
            /* P1INT */
            IsError = 0xFF;
         }
         break;
      case F_POKE_X_Y_N:
      case F_FILL_X_Y_N:
      case F_STUFF_X_Y_N:
         /* POKE X, Y */
         /* FILL X, Y */
         /* STUFF X, Y */
         {
            /* P1NUM|P2NUM */
            /* P1INT|P2BYT */
            IsError = 0xFF;
         }
         break;
      case F_CLS_N:
      case F_HOME_N:
         /* CLS */
         /* HOME */
         {
            /* PNONE */
            switch (My->OptionTerminalType)
            {
            case C_OPTION_TERMINAL_NONE:
               break;
            case C_OPTION_TERMINAL_ADM:
               fprintf(My->SYSOUT->cfp, "%c", 26);
               break;
            case C_OPTION_TERMINAL_ANSI:
               fprintf(My->SYSOUT->cfp, "%c[2J", 27);
               fprintf(My->SYSOUT->cfp, "%c[%d;%dH", 27, 1, 1);
               break;
            default:
               IsError = 0xFF;
               break;
            }
            fflush(My->SYSOUT->cfp);
         }
         break;
      case F_LOCATE_X_Y_N:
         /* LOCATE X, Y */
         {
            /* P1NUM|P2NUM */
            /* P1BYT|P2BYT */
            /* X is 1 based */
            /* Y is 1 based */
            switch (My->OptionTerminalType)
            {
            case C_OPTION_TERMINAL_NONE:
               break;
            case C_OPTION_TERMINAL_ADM:
               fprintf(My->SYSOUT->cfp, "%c=%c%c", 27, x + 32, y + 32);
               break;
            case C_OPTION_TERMINAL_ANSI:
               fprintf(My->SYSOUT->cfp, "%c[%d;%dH", 27, x, y);
               break;
            default:
               IsError = 0xFF;
               break;
            }
            fflush(My->SYSOUT->cfp);
         }
         break;
      case F_CUR_X_Y_S:
         /* CUR X, Y */
         {
            /* P1NUM|P2NUM */
            /* P1BYT|P2BYT */
            x++; /* X is 0 based */
            y++; /* Y is 0 based */
            switch (My->OptionTerminalType)
            {
            case C_OPTION_TERMINAL_NONE:
               break;
            case C_OPTION_TERMINAL_ADM:
               fprintf(My->SYSOUT->cfp, "%c=%c%c", 27, x + 32, y + 32);
               break;
            case C_OPTION_TERMINAL_ANSI:
               fprintf(My->SYSOUT->cfp, "%c[%d;%dH", 27, x, y);
               break;
            default:
               IsError = 0xFF;
               break;
            }
            fflush(My->SYSOUT->cfp);
            s = 0;
         }
         break;
      case F_VTAB_X_N:
         /* VTAB X  */
         {
            /* P1BYT */
            switch (My->OptionTerminalType)
            {
            case C_OPTION_TERMINAL_NONE:
               break;
            case C_OPTION_TERMINAL_ADM:
               fprintf(My->SYSOUT->cfp, "%c=%c%c", 27, x + 32, 1 + 32);
               break;
            case C_OPTION_TERMINAL_ANSI:
               fprintf(My->SYSOUT->cfp, "%c[%d;%dH", 27, x, 1);
               break;
            default:
               IsError = 0xFF;
               break;
            }
            fflush(My->SYSOUT->cfp);
         }
         break;
      case F_COLOR_X_Y_N:
         /* COLOR X, Y */
         {
            /* P1NUM|P2NUM */
            /* P1BYT|P2BYT */
            switch (My->OptionTerminalType)
            {
            case C_OPTION_TERMINAL_NONE:
               break;
            case C_OPTION_TERMINAL_ADM:
               break;
            case C_OPTION_TERMINAL_ANSI:
               fprintf(My->SYSOUT->cfp, "%c[%d;%dm", 27, 30 + x, 40 + y);
               break;
            default:
               IsError = 0xFF;
               break;
            }
            fflush(My->SYSOUT->cfp);
         }
         break;
      case F_FILES_N:
      case F_CATALOG_N:
         /* FILES */
         /* CATALOG */
         {
            /* PNONE */
            VariableType *v; 
            char   Buffer[BasicStringLengthMax + 1];

            if( (v = var_find(DEFVNAME_FILES,0,FALSE)) == NULL )
            {
                bwb_strcpy(Buffer, DEF_FILES);
            }
            else
            {
               VariantType variant;
               
               if( var_get( v, &variant ) == FALSE )
               {
                  bwb_strcpy( Buffer, DEF_FILES );
               }
               else
               {
                  if( variant.TypeChar == '$' )
                  {
                     bwb_strcpy( Buffer, variant.Buffer );
                     RELEASE( (&variant) );
                  }
                  else
                  {
                     bwb_strcpy( Buffer, DEF_FILES );
                  }
               }
            }
            N = system( Buffer );
         }
         break;
      case F_FILES_A_N:
      case F_CATALOG_A_N:
         /* FILES A$ */
         /* CATALOG A$ */
         {
            /* P1BYT */
            VariableType *v; 
            char   Buffer[BasicStringLengthMax + 1];

            if( (v = var_find(DEFVNAME_FILES,0,FALSE)) == NULL )
            {
                bwb_strcpy(Buffer, DEF_FILES);
            }
            else
            {
               VariantType variant;
               
               if( var_get( v, &variant ) == FALSE )
               {
                  bwb_strcpy( Buffer, DEF_FILES );
               }
               else
               {
                  if( variant.TypeChar == '$' )
                  {
                     bwb_strcpy( Buffer, variant.Buffer );
                     RELEASE( (&variant) );
                  }
                  else
                  {
                     bwb_strcpy( Buffer, DEF_FILES );
                  }
               }
            }
            bwb_strcat(Buffer, " ");
            bwb_strcat(Buffer, A);
            N = system(Buffer);
         }
         break;
      case F_FRE_N:
      case F_FRE_X_N:
      case F_FRE_A_N:
      case F_FREE_N:
      case F_FREE_X_N:
      case F_FREE_A_N:
      case F_MEM_N:
      case F_TOP_N:
         /* N = FRE(    ) */
         /* N = FRE( X  ) */
         /* N = FRE( X$ ) */
         /* N = FREE(    ) */
         /* N = FREE( X  ) */
         /* N = FREE( X$ ) */
         /* N = MEM(    ) */
         /* N = TOP(    ) */
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
      case F_UNSAVE_A_N:
         /* KILL A$ */
         /* UNSAVE A$ */
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
      case F_INPUT4_X_S:
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
                  c = fgetc( My->SYSIN->cfp );
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
      case F_INKEY4_S:
      case F_KEY4_S:
      case F_KEY_S:
         /* S$ = INKEY$ */
         /* S$ = KEY$ */
         /* S$ = KEY */
         {
            /* PNONE */
            int             c;

            c = fgetc( My->SYSIN->cfp );
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
            My->LPRINT_NULLS = x;
            N = 0;
         }
         break;
      case F_LWIDTH_X_N:
         /* LWIDTH X */
         {
            /* P1NUM */
            /* P1BYT */
            My->SYSPRN->width = x;
            My->SYSPRN->col = 1;
            N = 0;
         }
         break;
      case F_LPOS_N:
         /* N = LPOS */
         {
            /* PNONE */
            /* PNONE */
            N = My->SYSPRN->col;
         }
         break;
      case F_TRON_N:
      case F_TRACE_N:
      case F_FLOW_N:
         /* TRON */
         /* TRACE */
         /* FLOW */
         {
            /* PNONE */
            prn_xprintf("Trace is ON\n");
            My->bwb_trace = TRUE;
            N = 0;
         }
         break;
      case F_TROFF_N:
      case F_NOTRACE_N:
      case F_NOFLOW_N:
         /* TROFF */
         /* NOTRACE */
         /* NOFLOW */
         {
            /* PNONE */
            prn_xprintf("Trace is OFF\n");
            My->bwb_trace = FALSE;
            N = 0;
         }
         break;
      case F_RANDOMIZE_N:
      case F_RAN_N:
      case F_RANDOM_N:
         /* RANDOMIZE */
         /* RAN */
         /* RANDOM */
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
      case F_RAN_X_N:
      case F_RANDOM_X_N:
         /* RANDOMIZE X */
         /* RAN X */
         /* RANDOM X */
         {
            /* P1NUM */
            /* P1ANY */
            x = (int) bwb_rint( X );
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
   }
   /* sanity check */
   if (IsError == 0)
   {
      if (f->ReturnType == BasicStringSuffix)
      {
         /* STRING */
         if ( /* s < 0 || */ s > BasicStringLengthMax)
         {
            /* ERROR */
            WARN_INTERNAL_ERROR;
            return NULL;
         }
         else
         if (S != RESULT_BUFFER)
         {
            /* ERROR */
            WARN_INTERNAL_ERROR;
            return NULL;
         }
         else
         {
            RESULT_LENGTH = s;
            RESULT_BUFFER[RESULT_LENGTH] = BasicNulChar;
         }
      }
      else
      {
         /* NUMBER */
         if (isnan(N))
         {
            /* ERROR */
            /* this means the parameters were not properly checked */
            WARN_INTERNAL_ERROR;
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
            if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") )
            {
               /* ERROR */
            }
            /* CONTINUE */
         }
         RESULT_NUMBER = N;
      }
   }
   /* process errors */
   if (IsError == 0xFF)
   {
      /* NOT IMPLEMENTED ON THIS PLATFORM */
      sprintf(My->bwb_ebuf, "%s IS NOT IMPLEMENTED ON THIS PLATFORM", f->Name);
      bwb_Warning(73, My->bwb_ebuf); /* WARN_ADVANCED_FEATURE */
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
      sprintf(My->bwb_ebuf, "ILLEGAL FUUNCTION CALL: %s", Buffer);
      bwb_Warning(5, My->bwb_ebuf); /* WARN_ILLEGAL_FUNCTION_CALL */
   }
   return argv;      /* released by exp_function() in bwb_elx.c */
}

/* EOF */
