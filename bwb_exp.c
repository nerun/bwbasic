/****************************************************************
  
        bwb_exp.c       Expression Parser
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


/* 
--------------------------------------------------------------------------------------------
                               EXPRESSION PARSER

Inspired by https://groups.google.com/forum/m/#!topic/comp.compilers/RCyhEbLfs40
...
// Permission is given to use this source provided an acknowledgement is given.
// I'd also like to know if you've found it useful.
//
// The following Research Report describes the idea, and shows how the
// parsing method may be understood as an encoding of the usual family-of-
// parsing-procedures technique as used e.g. in Pascal compilers.
//     @techreport{QMW-DCS-383-1986a,
//       author       ="Clarke, Keith",
//       title        ="The Top-Down Parsing of Expressions",
//       institution  ="Department of Computer Science, Queen Mary College, University of London, England",
//       year         ="1986",
//       month        ="June",
//       number       ="QMW-DCS-1986-383",
//       scope        ="theory",
//       abstractURL  ="http://www.dcs.qmw.ac.uk/publications/report_abstracts/1986/383",
//       keywords     ="Recursive-descent parsing, expression parsing, operator precedence parsing."
//     }
// A formal proof of the algorithm was made, as part of his PhD thesis work,
// by A.M. Abbas of QMC, London, in the framework of Constructive Set Theory.
// copyright Keith Clarke, Dept of Computer Science, QMW, University of London,
// England.    email kei...@dcs.qmw.ac.uk
...
--------------------------------------------------------------------------------------------
*/

/* 
For all functions named "line_*",  "LineType * line" is the first parameter.
For all functions named "buff_*",  "char * buffer, int * position" are the first two parameters.
FALSE must be zero.
TRUE  any non-zero.
*/


#define NUL BasicNulChar

/* OperatorType.Arity */
#define UNARY  1
#define BINARY 2

/* OperatorType.IsAlpha */
#define IS_ALPHA  'T'
#define NO_ALPHA  'F'


#define COPY( X, Y ) if( X != NULL ) { bwb_memcpy( X, Y, sizeof( VariantType ) ); bwb_memset( Y, 0, sizeof( VariantType ) ); }

typedef unsigned char ResultType;
#define RESULT_SUCCESS     0 /* the value was successfully parsed, calculated, and so on */
#define RESULT_ERROR       1 /* an error has occurred, bwb_Warning() has been called */
#define RESULT_UNPARSED  255 /* the value was not (yet) parsed; this is NOT an error */

#define ERROR_NEGPOW       5 /* WARN_ILLEGAL_FUNCTION_CALL */
#define ERROR_ILLEGAL      5 /* WARN_ILLEGAL_FUNCTION_CALL */

/* helper functions */
static char min_value_type( VariantType * X );
static char max_number_type( char X, char Y );
static int is_string_type( VariantType * X );
static int is_number_type( VariantType * X );
static int is_integer_type( VariantType * X );
static int both_string_type( VariantType * X, VariantType * Y );
static int both_number_type( VariantType * X, VariantType * Y );
static int both_integer_type( VariantType * X, VariantType * Y );
static ResultType buff_read_expr( char * buffer, int * position, VariantType * X, unsigned char LastPrec );
static ResultType buff_read_primary( char * buffer, int * position, VariantType * X );



/* functions to resolve the value of an operator  */
/* static */ typedef ResultType ( OperatorFunctionType )( VariantType * X, VariantType * Y );
static ResultType OP_IMP ( VariantType * X, VariantType * Y );
static ResultType OP_EQV ( VariantType * X, VariantType * Y );
static ResultType OP_XOR ( VariantType * X, VariantType * Y );
static ResultType OP_AND ( VariantType * X, VariantType * Y );
static ResultType OP_OR  ( VariantType * X, VariantType * Y );
static ResultType OP_NOT ( VariantType * X, VariantType * Y );
static ResultType OP_LIKE( VariantType * X, VariantType * Y );
static ResultType OP_MAX ( VariantType * X, VariantType * Y );
static ResultType OP_MIN ( VariantType * X, VariantType * Y );
static ResultType OP_LE  ( VariantType * X, VariantType * Y );
static ResultType OP_GE  ( VariantType * X, VariantType * Y );
static ResultType OP_NE  ( VariantType * X, VariantType * Y );
static ResultType OP_LT  ( VariantType * X, VariantType * Y );
static ResultType OP_GT  ( VariantType * X, VariantType * Y );
static ResultType OP_EQ  ( VariantType * X, VariantType * Y );
static ResultType OP_AMP ( VariantType * X, VariantType * Y );
static ResultType OP_ADD ( VariantType * X, VariantType * Y );
static ResultType OP_SUB ( VariantType * X, VariantType * Y );
static ResultType OP_MUL ( VariantType * X, VariantType * Y );
static ResultType OP_DIV ( VariantType * X, VariantType * Y );
static ResultType OP_IDIV( VariantType * X, VariantType * Y );
static ResultType OP_MOD ( VariantType * X, VariantType * Y );
static ResultType OP_NEG ( VariantType * X, VariantType * Y );
static ResultType OP_POS ( VariantType * X, VariantType * Y );
static ResultType OP_EXP ( VariantType * X, VariantType * Y );


/* table of operators */
/* static */ typedef struct
{
   unsigned char ThisPrec;
   unsigned char NextPrec;  /* if BINARY and LEFT assoc, then ThisPrec+1, else ThisPrec */
   unsigned char Arity;     /* UNARY or BINARY */
   char IsAlpha;            /* IS_ALPHA or NO_ALPHA, determines how operator is matched */
   const char * Name;
   OperatorFunctionType * Eval;
   const char * Syntax;
   const char * Description;
   OptionVersionType OptionVersionBitmask;   /* OPTION VERSION bitmask */
} OperatorType;


/* 
In BASIC, 2 ^ 3 ^ 2 = ( 2 ^ 3 ) ^ 2 = 64, 
and -2 ^ 2 = - (2 ^ 2) = -4.
*/

static OperatorType OperatorTable[] =
{
   /* LOGICAL */
{ 0x01, 0x02, BINARY, IS_ALPHA,  "IMP" , OP_IMP , "X IMP Y", "Bitwise IMP", 
  B15 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x02, 0x03, BINARY, IS_ALPHA,  "EQV" , OP_EQV ,"X EQV Y", "Bitwise EQV", 
  B15 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x03, 0x04, BINARY, IS_ALPHA,  "XOR" , OP_XOR ,"X XOR Y", "Bitwise Exclusive OR", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x03, 0x04, BINARY, IS_ALPHA,  "XRA" , OP_XOR , "X XRA Y", "Bitwise Exclusive OR", 
  HB2 },
{ 0x04, 0x05, BINARY, IS_ALPHA,  "OR"  , OP_OR  , "X OR Y", "Bitwise OR", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x05, 0x06, BINARY, IS_ALPHA,  "AND" , OP_AND , "X AND Y", "Bitwise AND", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x06, 0x06, UNARY , IS_ALPHA,  "NOT" , OP_NOT , "NOT X", "Bitwise NOT", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
/* RELATIONAL */
{ 0x07, 0x08, BINARY, IS_ALPHA,  "NE"  , OP_NE  , "X NE Y", "Not Equal", 
  0},
{ 0x07, 0x08, BINARY, NO_ALPHA,  "#"   , OP_NE  , "X # Y", "Not Equal", 
  0},
{ 0x07, 0x08, BINARY, NO_ALPHA,  "<>"  , OP_NE  , "X <> Y", "Not Equal", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x07, 0x08, BINARY, NO_ALPHA,  "><"  , OP_NE  , "X >< Y", "Not Equal", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x07, 0x08, BINARY, IS_ALPHA,  "GE"  , OP_GE  , "X GE Y", "Greater than or Equal", 
  0},
{ 0x07, 0x08, BINARY, NO_ALPHA,  ">="  , OP_GE  , "X >= Y", "Greater than or Equal", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x07, 0x08, BINARY, NO_ALPHA,  "=>"  , OP_GE  , "X => Y", "Greater than or Equal", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x07, 0x08, BINARY, IS_ALPHA,  "LE"  , OP_LE  , "X LE Y", "Less than or Equal", 
  0},
{ 0x07, 0x08, BINARY, NO_ALPHA,  "<="  , OP_LE  , "X <= Y", "Less than or Equal", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x07, 0x08, BINARY, NO_ALPHA,  "=<"  , OP_LE  , "X =< Y", "Less than or Equal", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x07, 0x08, BINARY, IS_ALPHA,  "EQ"  , OP_EQ  , "X EQ Y", "Equal", 
  0}, 
{ 0x07, 0x08, BINARY, NO_ALPHA,  "="   , OP_EQ  , "X = Y", "Equal", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX}, 
{ 0x07, 0x08, BINARY, IS_ALPHA,  "LT"  , OP_LT  , "X LT Y", "Less than", 
  0},
{ 0x07, 0x08, BINARY, NO_ALPHA,  "<"   , OP_LT  , "X < Y", "Less than", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x07, 0x08, BINARY, IS_ALPHA,  "GT"  , OP_GT  , "X GT Y", "Greater than", 
  0},
{ 0x07, 0x08, BINARY, NO_ALPHA,  ">"   , OP_GT  , "X > Y", "Greater than", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x07, 0x08, BINARY, IS_ALPHA,  "LIKE", OP_LIKE,  "A$ LIKE B$", "Compare A$ to the pattern in B$",
  B15 | VBX},
{ 0x07, 0x08, BINARY, IS_ALPHA,  "MAX" , OP_MAX , "X MAX Y", "Maximum",
  0},
{ 0x07, 0x08, BINARY, IS_ALPHA,  "MIN" , OP_MIN , "X MIN Y", "Minimum",
  0},
/* CONCATENATION */
{ 0x08, 0x09, BINARY, NO_ALPHA,  "&"   , OP_AMP , "X & Y", "Concatenation", 
  B15 | HB2 | VBX}, 
/* ARITHMETIC */
{ 0x09, 0x0A, BINARY, NO_ALPHA,  "+"   , OP_ADD , "X + Y", "Addition",
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX}, 
{ 0x09, 0x0A, BINARY, NO_ALPHA,  "-"   , OP_SUB , "X - Y", "Subtraction", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX}, 
{ 0x0A, 0x0B, BINARY, IS_ALPHA,  "MOD" , OP_MOD , "X MOD Y", "Integer Modulus",
  B15 | HB1 | HB2 | D71 | M80 | T80 | M85 | M88 | VBX}, 
{ 0x0B, 0x0C, BINARY, NO_ALPHA,  "\\"  , OP_IDIV, "X \\ Y", "Integer Division", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX}, 
{ 0x0C, 0x0D, BINARY, NO_ALPHA,  "*"   , OP_MUL , "X * Y", "Multiplication",
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX}, 
{ 0x0C, 0x0D, BINARY, NO_ALPHA,  "/"   , OP_DIV , "X / Y", "Division",
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX}, 
{ 0x0D, 0x0D, UNARY , NO_ALPHA,  "#"   , OP_POS , "# X", "Posation",
  B15 | HB1 | HB2 | D64 | G65 | G67       | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x0D, 0x0D, UNARY , NO_ALPHA,  "+"   , OP_POS , "+ X", "Posation",
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x0D, 0x0D, UNARY , NO_ALPHA,  "-"   , OP_NEG , "- X", "Negation", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | T80 | M85 | M88 | VBX},
{ 0x0E, 0x0F, BINARY, NO_ALPHA,  "^"   , OP_EXP , "X ^ Y", "Exponential",
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78 | E86 | M80 | M85 | M88 | VBX},
{ 0x0E, 0x0F, BINARY, NO_ALPHA,  "["   , OP_EXP , "X [ Y", "Exponential", 
  B15 | HB1 | HB2 | T80},
{ 0x0E, 0x0F, BINARY, NO_ALPHA,  "**"  , OP_EXP , "X ** Y", "Exponential", 
  B15 | HB1 | HB2 | D64 | G65 | G67 | I70 | I73 | C77 | D71 | E78},
};

#define NUM_OPS ( sizeof( OperatorTable ) / sizeof( OperatorType ) )

/*
--------------------------------------------------------------------------------------------
                               Helper Functions
--------------------------------------------------------------------------------------------
*/

void SortAllOperators( void )  /* SortAllOperators() should be called by bwb_init() */   
{
   /* sort the operators by decreasing length, so "**" matches before "*" and so on. */
   int i;
   bwx_DEBUG(__FUNCTION__);


   for( i = 0; i < NUM_OPS - 1; i++ )
   {
      int j;
      int k;
      int m;
      
      k = i;
      m = bwb_strlen( OperatorTable[ i ].Name );
      
      for( j = i + 1; j < NUM_OPS; j++ )
      {
         int n;
         n = bwb_strlen( OperatorTable[ j ].Name );
         if( n > m )
         {
            m = n;
            k = j;
         }
      }
      if( k > i )
      {
         /* swap */
         OperatorType t;
         OperatorType *T = &t;
         OperatorType *I = &OperatorTable[ i ];
         OperatorType *K = &OperatorTable[ k ];
         
         bwb_memcpy( T, I, sizeof( t ) );
         bwb_memcpy( I, K, sizeof( t ) );
         bwb_memcpy( K, T, sizeof( t ) );
      }
   }
}

static char min_value_type( VariantType * X )
{
   /* returns the minimal Typechar, based upon a NUMBER's value */
   bwx_DEBUG(__FUNCTION__);


   if (isnan( X->Number ))
   {
      /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
      WARN_INTERNAL_ERROR;
      return NUL;
   }
   if( X->Number == bwb_rint( X->Number ) )
   {
      /* INTEGER */
      if (0  <= X->Number &&  X->Number <= UCHAR_MAX)
      {
         return BasicByteSuffix;
      }
      if (SHRT_MIN  <= X->Number &&  X->Number <= SHRT_MAX)
      {
         return BasicIntegerSuffix;
      }
      if (LONG_MIN  <= X->Number &&  X->Number <= LONG_MAX)
      {
         return BasicLongSuffix;
      }
      if (LONG_MIN  <= X->Number &&  X->Number <= LONG_MAX)
      {
         return BasicCurrencySuffix;
      }
   }
   /* FLOAT */
   if ( -(FLT_MAX)  <= X->Number &&  X->Number <= FLT_MAX )
   {
      return BasicSingleSuffix;
   }
   if ( -(DBL_MAX)  <= X->Number &&  X->Number <= DBL_MAX ) 
   {
      return BasicDoubleSuffix;
   }
   /* OVERFLOW */
   if( X->Number < 0 )
   {
      X->Number = -DBL_MAX;
   }
   else
   {
      X->Number = DBL_MAX;
   }
   if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") )
   {
      /* ERROR */
   }
   /* CONTINUE */
   return BasicDoubleSuffix;
}



static char max_number_type( char X, char Y )
{
   /* returns the maximal TypeChar, given two NUMBER TypeChar's */
   bwx_DEBUG(__FUNCTION__);


   if( X == BasicDoubleSuffix || Y == BasicDoubleSuffix )
   {
      return BasicDoubleSuffix;
   }
   if( X == BasicSingleSuffix || Y == BasicSingleSuffix )
   {
      return BasicSingleSuffix;
   }
   if( X == BasicCurrencySuffix || Y == BasicCurrencySuffix )
   {
      return BasicCurrencySuffix;
   }
   if( X == BasicLongSuffix || Y == BasicLongSuffix )
   {
      return BasicLongSuffix;
   }
   if( X == BasicIntegerSuffix || Y == BasicIntegerSuffix )
   {
      return BasicIntegerSuffix;
   }
   if( X == BasicByteSuffix || Y == BasicByteSuffix )
   {
      return BasicByteSuffix;
   }
   /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
   WARN_INTERNAL_ERROR;
   return NUL;
}
static char math_type( VariantType * X, VariantType * Y )
{
   /* returns the TypeChar resulting from a math operation, such as addition. */
   /* 
   the return TypeChar should be the maximal of:
   - the original X's TypeChar
   - the original Y's TypeChar
   - the minimal result's TypeChar
   */
   bwx_DEBUG(__FUNCTION__);


   return max_number_type( max_number_type( X->TypeChar, Y->TypeChar ), min_value_type( X ) );
}

static int is_string_type( VariantType * X )
{
   /* if value is a STRING, then TRUE, else FALSE */
   bwx_DEBUG(__FUNCTION__);


   switch( X->TypeChar )
   {
   case BasicStringSuffix  : 
      if( X->Buffer == NULL )
      {
         /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
      return TRUE;
   case BasicDoubleSuffix  :
   case BasicSingleSuffix  :
   case BasicCurrencySuffix:
   case BasicLongSuffix    :
   case BasicIntegerSuffix :
   case BasicByteSuffix    : 
      if( X->Buffer != NULL )
      {
         /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
      return FALSE;
   }
   /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
   WARN_INTERNAL_ERROR;
   return FALSE;
}
static int is_number_type( VariantType * X )
{
   /* if value is a NUMBER, then TRUE, else FALSE */
   bwx_DEBUG(__FUNCTION__);


   switch( X->TypeChar )
   {
   case BasicStringSuffix  : 
      if( X->Buffer == NULL )
      {
         /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
      return FALSE;
   case BasicDoubleSuffix  :
   case BasicSingleSuffix  :
   case BasicCurrencySuffix:
   case BasicLongSuffix    :
   case BasicIntegerSuffix :
   case BasicByteSuffix    :
      if( X->Buffer != NULL )
      {
         /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
      return TRUE;
   }
   /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
   WARN_INTERNAL_ERROR;
   return FALSE; /* never reached */
}
static int is_integer_type( VariantType * X )
{
   /* if value is an INTEGER, then TRUE, else FALSE */
   switch( X->TypeChar )
   {
   case BasicStringSuffix  : return FALSE;
   case BasicDoubleSuffix  : return FALSE;
   case BasicSingleSuffix  : return FALSE;
   case BasicCurrencySuffix: return TRUE;
   case BasicLongSuffix    : return TRUE;
   case BasicIntegerSuffix : return TRUE;
   case BasicByteSuffix    : return TRUE;
   }
   /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
   WARN_INTERNAL_ERROR;
   return FALSE;
}
static int both_string_type( VariantType * X, VariantType * Y )
{
   /* if both values are a STRING, then TRUE, else FALSE */
   if( is_string_type( X ) && is_string_type( Y ) )
   {
      return TRUE;
   }
   return FALSE;
}
static int both_number_type( VariantType * X, VariantType * Y )
{
   /* if both values are a NUMBER, then TRUE, else FALSE */
   if( is_number_type( X ) && is_number_type( Y ) )
   {
      return TRUE;
   }
   return FALSE;
}
static int both_integer_type( VariantType * X, VariantType * Y )
{
   /* if both values are an INTEGER, then TRUE, else FALSE */
   if( is_integer_type( X ) && is_integer_type( Y ) )
   {
      return TRUE;
   }
   return FALSE;
}


static int is_long_value( VariantType * X )
{
   /* if the NUMBER's value can be a LONG, then TRUE, else FALSE */
   bwx_DEBUG(__FUNCTION__);

   if (isnan( X->Number ))
   {
      /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
      WARN_INTERNAL_ERROR;
      return FALSE;
   }
   if( X->Number == bwb_rint( X->Number ) )
   {
      if (LONG_MIN  <= X->Number &&  X->Number <= LONG_MAX)
      {
         return TRUE;
      }
   }
   return FALSE;
}

static int both_are_long( VariantType * X, VariantType * Y )
{
   /* if both values can be a LONG, then TRUE, else FALSE */
   if( is_long_value( X ) && is_long_value( Y ) )
   {
      return TRUE;
   }
   return FALSE;
}


static int bwb_isodigit( int C )
{
  switch( C )
  {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
    return TRUE;
  }
  return FALSE;
}



/*
--------------------------------------------------------------------------------------------
                               Operator Functions
--------------------------------------------------------------------------------------------
*/

static ResultType OP_ADD( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X + Y) */
      X->Number += Y->Number;
      if( both_integer_type( X, Y ) )
      {
         X->Number = bwb_rint( X->Number );
      }
      X->TypeChar = math_type( X, Y );
      return RESULT_SUCCESS;
   }
   if( both_string_type( X, Y ) )
   {
      /* X$ = (X$ + Y$) */
      VariantType t;
      VariantType * T = &t;
      T->TypeChar = BasicStringSuffix;
      T->Length = X->Length + Y->Length;
      /* we always allocate a buffer, even for non-empty strings */
      T->Buffer = calloc( T->Length + 1 /* NUL */, sizeof(char) );
      if( T->Buffer == NULL )
      {
         WARN_OUT_OF_MEMORY;  
         return RESULT_ERROR;
      }
      if( T->Length > 0 )
      {
         if( X->Length > 0 )
         {
            bwb_memcpy( T->Buffer, X->Buffer, X->Length );
         }
         if( Y->Length > 0 )
         {
            bwb_memcpy( &T->Buffer[ X->Length ] , Y->Buffer, Y->Length );
         }
      }
      T->Buffer[ T->Length ] = NUL;
      RELEASE( X );
      RELEASE( Y );
      COPY( X, T );
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_AMP( VariantType * X, VariantType * Y )
{
   /* X$ = (X  & Y ) */
   /* X$ = (X  & Y$) */
   /* X$ = (X$ & Y ) */
   /* X$ = (X$ & Y$) */
   if( X->TypeChar != BasicStringSuffix )
   {
      /* coerce X to X$ */
      X->Buffer = calloc( 32, sizeof(char) ); /* free() called by OP_ADD() */
      if( X->Buffer == NULL )
      {
         WARN_OUT_OF_MEMORY;  
         return RESULT_ERROR;
      }
      sprintf( X->Buffer, "%f", X->Number );
      X->Length = bwb_strlen( X->Buffer );
      X->TypeChar = BasicStringSuffix;
   }
   if( Y->TypeChar != BasicStringSuffix )
   {
      /* coerce Y to Y$ */
      Y->Buffer = calloc( 32, sizeof(char) ); /* free() called by OP_ADD() */
      if( Y->Buffer == NULL )
      {
         WARN_OUT_OF_MEMORY;  
         return RESULT_ERROR;
      }
      sprintf( Y->Buffer, "%f", Y->Number );
      Y->Length = bwb_strlen( Y->Buffer );
      Y->TypeChar = BasicStringSuffix;
   }
   return OP_ADD( X, Y );
}
static ResultType OP_SUB( VariantType * X, VariantType * Y )
{
   /* X = (X - Y) */
   if( both_number_type( X, Y ) )
   {
      X->Number -= Y->Number;
      if( both_integer_type( X, Y ) )
      {
         X->Number = bwb_rint( X->Number );
      }
      X->TypeChar = math_type( X, Y );
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_MUL( VariantType * X, VariantType * Y )
{
   /* X = (X * Y) */
   if( both_number_type( X, Y ) )
   {
      X->Number *= Y->Number;
      if( both_integer_type( X, Y ) )
      {
         X->Number = bwb_rint( X->Number );
      }
      X->TypeChar = math_type( X, Y );
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_IDIV( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X \ Y) */
      X->Number = bwb_rint( X->Number );
      Y->Number = bwb_rint( Y->Number );
      if( Y->Number == 0 )
      {
         /* - Evaluation of an expression results in division
          * by zero (nonfatal, the recommended recovery
          * procedure is to supply machine infinity with the
          * sign of the numerator and continue) 
          */
         if (X->Number < 0)
         {
            /* NEGATIVE */
            X->Number = -DBL_MAX; /* NEGATIVE INFINITY */
         }
         else
         {
            /* POSITIVE  */
            X->Number = DBL_MAX;  /* POSITIVE INFINITY */
         }
         if( bwb_Warning_Overflow("*** WARNING:  Divide by 0 ***") )
         {
            /* ERROR */
         }
         /* CONTINUE */
      }
      else
      {
         BasicNumberType N;
         BasicNumberType I;
         N = X->Number / Y->Number;
         N = N - modf(N, &I);
         X->Number = bwb_rint( N );
      }
      X->TypeChar = math_type( X, Y );
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_DIV( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X / Y) */
      if( both_integer_type( X, Y ) )
      {
         return OP_IDIV( X, Y );
      }
      if( Y->Number == 0 )
      {
         /* - Evaluation of an expression results in division
          * by zero (nonfatal, the recommended recovery
          * procedure is to supply machine infinity with the
          * sign of the numerator and continue) 
          */
         if (X->Number < 0)
         {
            /* NEGATIVE */
            X->Number = -DBL_MAX; /* NEGATIVE INFINITY */
         }
         else
         {
            /* POSITIVE  */
            X->Number = DBL_MAX;  /* POSITIVE INFINITY */
         }
         if( bwb_Warning_Overflow("*** WARNING:  Divide by 0 ***") )
         {
            /* ERROR */
         }
         /* CONTINUE */
      }
      else
      {
         X->Number /= Y->Number;
      }
      X->TypeChar = math_type( X, Y );
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_MOD( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X MOD Y) */
      X->Number = bwb_rint( X->Number );
      Y->Number = bwb_rint( Y->Number );
      if( Y->Number == 0 )
      {
         /* - Evaluation of an expression results in division
          * by zero (nonfatal, the recommended recovery
          * procedure is to supply machine infinity with the
          * sign of the numerator and continue) 
          */
         if (X->Number < 0)
         {
            /* NEGATIVE */
            X->Number = -DBL_MAX; /* NEGATIVE INFINITY */
         }
         else
         {
            /* POSITIVE  */
            X->Number = DBL_MAX;  /* POSITIVE INFINITY */
         }
         if( bwb_Warning_Overflow("*** WARNING:  Divide by 0 ***") )
         {
            /* ERROR */
         }
         /* CONTINUE */
      }
      else
      {
         BasicNumberType N;
         BasicNumberType I;
         N = X->Number / Y->Number;
         modf(N, &I);
         N = X->Number - Y->Number * I;
         X->Number = bwb_rint( N );
      }
      X->TypeChar = math_type( X, Y );
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_EXP( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X ^ Y) */
      if (X->Number < 0 && Y->Number != bwb_rint( Y->Number ) )
      {
         /*** FATAL ***/
         /* - Evaluation of the operation of
          * involution results in a negative number
          * being raised to a non-integral power
          * (fatal). */
         X->Number = 0;
         bwb_Warning( ERROR_NEGPOW, "NEGATIVE QUANTITY RAISED TO A NON-INTEGRAL POWER" );  return RESULT_ERROR;
      }
      if (X->Number == 0 && Y->Number < 0)
      {
         /* - Evaluation of the operation of
          * involution results in a zero being
          * raised to a negative value (nonfatal, the
          * recommended recovery procedure is to
          * supply positive machine infinity and
          * continue). */
   
         X->Number = DBL_MAX;
         if( bwb_Warning_Overflow("*** Arithmetic Overflow ***"))
         {
            /* ERROR */
         }
         /* CONTINUE */
      }
      else
      {
         X->Number = pow(X->Number, Y->Number);
      }
      X->TypeChar = math_type( X, Y );
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_NEG( VariantType * X, VariantType * Y )
{
   if( Y != NULL )
   {
      WARN_INTERNAL_ERROR;
      return RESULT_ERROR;
   }
   if( is_number_type( X ) )
   {
      /* X = (- X) */
      X->Number = -X->Number;
      X->TypeChar = min_value_type( X );
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_POS( VariantType * X, VariantType * Y )
{
   if( Y != NULL )
   {
      WARN_INTERNAL_ERROR;
      return RESULT_ERROR;
   }
   if( is_number_type( X ) )
   {
      /* X = (+ X) */
      /*
      X->Number = X->Number;
      X->TypeChar = min_value_type( X );
      */
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_OR( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X OR Y) */
      if( both_are_long( X, Y ) )
      {
         long x = (long) bwb_rint( X->Number );
         long y = (long) bwb_rint( Y->Number );
         
         x = x | y;

         X->Number = x;
         X->TypeChar = min_value_type( X );
         return RESULT_SUCCESS;
      }
      WARN_OVERFLOW;  
      return RESULT_ERROR;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_AND( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X AND Y) */
      if( both_are_long( X, Y ) )
      {
         long x = (long) bwb_rint( X->Number );
         long y = (long) bwb_rint( Y->Number );
         
         x = x & y;

         X->Number = x;
         X->TypeChar = min_value_type( X );
         return RESULT_SUCCESS;
      }
      WARN_OVERFLOW;  
      return RESULT_ERROR;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_XOR( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X XOR Y) */
      if( both_are_long( X, Y ) )
      {
         long x = (long) bwb_rint( X->Number );
         long y = (long) bwb_rint( Y->Number );
         
         x = x ^ y;

         X->Number = x;
         X->TypeChar = min_value_type( X );
         return RESULT_SUCCESS;
      }
      WARN_OVERFLOW;  
      return RESULT_ERROR;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_EQV( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X EQV Y)  = NOT ( X XOR Y ) */
      if( both_are_long( X, Y ) )
      {
         long x = (long) bwb_rint( X->Number );
         long y = (long) bwb_rint( Y->Number );
         
         x = ~(x ^ y);

         X->Number = x;
         X->TypeChar = min_value_type( X );
         return RESULT_SUCCESS;
      }
      WARN_OVERFLOW;  
      return RESULT_ERROR;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_IMP( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X IMP Y)  = (X AND Y) OR (NOT X) */
      if( both_are_long( X, Y ) )
      {
         long x = (long) bwb_rint( X->Number );
         long y = (long) bwb_rint( Y->Number );

         x = (x & y) | (~x);
         
         X->Number = x;
         X->TypeChar = min_value_type( X );
         return RESULT_SUCCESS;
      }
      WARN_OVERFLOW;  
      return RESULT_ERROR;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_NOT( VariantType * X, VariantType * Y )
{
   if( Y != NULL )
   {
      WARN_INTERNAL_ERROR;
      return RESULT_ERROR;
   }
   if( is_number_type( X ) )
   {
      /* X = (NOT X) */
      if( is_long_value( X ) )
      {
         long x = (long) bwb_rint( X->Number );
         
         x = ~x;

         X->Number = x;
         X->TypeChar = min_value_type( X );
         return RESULT_SUCCESS;
      }
      WARN_OVERFLOW;  
      return RESULT_ERROR;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_MAX( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X MAX Y) = IIF( X < Y, Y, X ) */
      if( X->Number < Y->Number )
      {
         X->Number = Y->Number;
      }
      if( both_integer_type( X, Y ) )
      {
         X->Number = bwb_rint( X->Number );
      }
      X->TypeChar = math_type( X, Y );
      return RESULT_SUCCESS;
   }
   if( both_string_type( X, Y ) )
   {
      /* X$ = ( X$ MAX Y$ ) == IIF( X$ < Y$,  Y$, X$ ) */
      if( bwb_stricmp( X->Buffer, Y->Buffer ) < 0 )
      {
         RELEASE( X );
         COPY( X, Y );
      }
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_MIN( VariantType * X, VariantType * Y )
{
   if( both_number_type( X, Y ) )
   {
      /* X = (X MIN Y) = IIF( X > Y, Y, X ) */
      if( X->Number > Y->Number )
      {
         X->Number = Y->Number;
      }
      if( both_integer_type( X, Y ) )
      {
         X->Number = bwb_rint( X->Number );
      }
      X->TypeChar = math_type( X, Y );
      return RESULT_SUCCESS;
   }
   if( both_string_type( X, Y ) )
   {
      /* X$ = ( X$ MIN Y$ ) == IIF( X$ > Y$, Y$, X$ ) */
      if( bwb_stricmp( X->Buffer, Y->Buffer ) > 0 )
      {
         RELEASE( X );
         COPY( X, Y );
      }
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}

/*
COMPARISON OPERATORS - these all return a TRUE/FALSE result in X
*/


/* ------------------- equality */

static ResultType test_eq( VariantType * X, VariantType * Y , int TrueValue, int FalseValue )
{
   if( both_number_type( X, Y ) )
   {
      /* X = IIF( X = Y, TrueValue, FalseValue ) */
      if( both_are_long( X, Y ) )
      {
         long x = (long) bwb_rint( X->Number );
         long y = (long) bwb_rint( Y->Number );

         if( x == y )
         {
            X->Number = TrueValue;
         }
         else
         {
            X->Number = FalseValue;
         }
      }
      else
      {
         if( X->Number == Y->Number )
         {
            X->Number = TrueValue;
         }
         else
         {
            X->Number = FalseValue;
         }
         
      }
      X->TypeChar = BasicIntegerSuffix;
      return RESULT_SUCCESS;
   }
   if( both_string_type( X, Y ) )
   {
      /* X = IIF( X$ = Y$, TrueValue, FalseValue ) */
      /* NOTE: embedded NUL characters terminate comparison */
      if (My->CurrentVersion->OptionFlags & OPTION_COMPARE_TEXT)
      {
         /* case insensitive */
         if( bwb_stricmp( X->Buffer, Y->Buffer ) == 0 )
         {
               X->Number = TrueValue;
         }
         else
         {
               X->Number = FalseValue;
         }
      }
      else
      {
         /* case sensitive */
         if( bwb_strcmp( X->Buffer, Y->Buffer ) == 0 )
         {
               X->Number = TrueValue;
         }
         else
         {
               X->Number = FalseValue;
         }
      }
      RELEASE( X );
      RELEASE( Y );
      X->TypeChar = BasicIntegerSuffix;
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_EQ( VariantType * X, VariantType * Y )
{
   return test_eq( X, Y, TRUE, FALSE );
}
static ResultType OP_NE( VariantType * X, VariantType * Y )
{
   return test_eq( X, Y, FALSE, TRUE );
}

/* ------------------- greater */

static ResultType test_gt( VariantType * X, VariantType * Y , int TrueValue, int FalseValue )
{
   if( both_number_type( X, Y ) )
   {
      /* X = IIF( X > Y, TrueValue, FalseValue ) */
      if( both_are_long( X, Y ) )
      {
         long x = (long) bwb_rint( X->Number );
         long y = (long) bwb_rint( Y->Number );

         if( x > y )
         {
            X->Number = TrueValue;
         }
         else
         {
            X->Number = FalseValue;
         }
      }
      else
      {
         if( X->Number > Y->Number )
         {
            X->Number = TrueValue;
         }
         else
         {
            X->Number = FalseValue;
         }
         
      }
      X->TypeChar = BasicIntegerSuffix;
      return RESULT_SUCCESS;
   }
   if( both_string_type( X, Y ) )
   {
      /* X = IIF( X$ > Y$, TrueValue, FalseValue ) */
      /* NOTE: embedded NUL characters terminate comparison */
      if (My->CurrentVersion->OptionFlags & OPTION_COMPARE_TEXT)
      {
         /* case insensitive */
         if( bwb_stricmp( X->Buffer, Y->Buffer ) > 0 )
         {
               X->Number = TrueValue;
         }
         else
         {
               X->Number = FalseValue;
         }
      }
      else
      {
         /* case sensitive */
         if( bwb_strcmp( X->Buffer, Y->Buffer ) > 0 )
         {
               X->Number = TrueValue;
         }
         else
         {
               X->Number = FalseValue;
         }
      }
      RELEASE( X );
      RELEASE( Y );
      X->TypeChar = BasicIntegerSuffix;
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_GT( VariantType * X, VariantType * Y )
{
   return test_gt( X, Y, TRUE, FALSE );
}
static ResultType OP_LE( VariantType * X, VariantType * Y )
{
   return test_gt( X, Y, FALSE, TRUE );
}

/* ------------------- lesser */

static ResultType test_lt( VariantType * X, VariantType * Y , int TrueValue, int FalseValue )
{
   if( both_number_type( X, Y ) )
   {
      /* X = IIF( X < Y, TrueValue, FalseValue ) */
      if( both_are_long( X, Y ) )
      {
         long x = (long) bwb_rint( X->Number );
         long y = (long) bwb_rint( Y->Number );

         if( x < y )
         {
            X->Number = TrueValue;
         }
         else
         {
            X->Number = FalseValue;
         }
      }
      else
      {
         if( X->Number < Y->Number )
         {
            X->Number = TrueValue;
         }
         else
         {
            X->Number = FalseValue;
         }
         
      }
      X->TypeChar = BasicIntegerSuffix;
      return RESULT_SUCCESS;
   }
   if( both_string_type( X, Y ) )
   {
      /* X = IIF( X$ < Y$, TrueValue, FalseValue ) */
      /* NOTE: embedded NUL characters terminate comparison */
      if (My->CurrentVersion->OptionFlags & OPTION_COMPARE_TEXT)
      {
         /* case insensitive */
         if( bwb_stricmp( X->Buffer, Y->Buffer ) < 0 )
         {
               X->Number = TrueValue;
         }
         else
         {
               X->Number = FalseValue;
         }
      }
      else
      {
         /* case sensitive */
         if( bwb_strcmp( X->Buffer, Y->Buffer ) < 0 )
         {
               X->Number = TrueValue;
         }
         else
         {
               X->Number = FalseValue;
         }
      }
      RELEASE( X );
      RELEASE( Y );
      X->TypeChar = BasicIntegerSuffix;
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}
static ResultType OP_LT( VariantType * X, VariantType * Y )
{
   return test_lt( X, Y, TRUE, FALSE );
}
static ResultType OP_GE( VariantType * X, VariantType * Y )
{
   return test_lt( X, Y, FALSE, TRUE );
}

/* ------------------- like */

static ResultType OP_LIKE( VariantType * X, VariantType * Y )
{
   if( both_string_type( X, Y ) )
   {
      /* X = (X$ LIKE Y$) */
      int X_count = 0;
      int Y_count = 0;

      if( IsLike( X->Buffer, &X_count, X->Length, 
                  Y->Buffer, &Y_count, Y->Length) )
      {
            X->Number = TRUE;
      }
      else
      {
            X->Number = FALSE;
      }
      RELEASE( X );
      RELEASE( Y );
      X->TypeChar = BasicIntegerSuffix;
      return RESULT_SUCCESS;
   }
   WARN_TYPE_MISMATCH;
   return RESULT_ERROR;
}


/*
--------------------------------------------------------------------------------------------
                               Line Parsing Utilities
--------------------------------------------------------------------------------------------
*/

static OperatorType * buff_read_operator( char * buffer, int * position, unsigned char LastPrec, unsigned char Arity )
{
   int p;
   bwx_DEBUG(__FUNCTION__);
   
   p = *position;
   if( bwb_isalpha( buffer[ p ] ) )
   {
      /* only consider alphabetic operators */
      /* spaces between any character of the operator is not allowed */
      char name[ BasicNameLengthMax + 1 ];
      
      if( buff_read_varname( buffer, &p, name ) )
      {
         int i;
         for( i = 0; i < NUM_OPS; i++ )
         {
            OperatorType * T = &OperatorTable[ i ];
            if( T->OptionVersionBitmask & My->CurrentVersion->OptionVersionBitmask )
            {
               if( T->ThisPrec >= LastPrec && T->Arity == Arity && T->IsAlpha == IS_ALPHA )
               {
                  /* possible */
                  if( bwb_stricmp( T->Name, name ) == 0 )
                  {
                     /* FOUND */
                     *position = p;
                     return T;
                  }
               }
            }
         }
      }
   }
   else
   {
      /* only consider non-alphabetic operators */
      /* spaces between any character of the operator is allowed */
      int i;
      for( i = 0; i < NUM_OPS; i++ )
      {
         OperatorType * T = &OperatorTable[ i ];
         if( T->OptionVersionBitmask & My->CurrentVersion->OptionVersionBitmask )
         {
            if( T->ThisPrec >= LastPrec && T->Arity == Arity && T->IsAlpha == NO_ALPHA )
            {
               /* possible */
               int m;                     /* number of characters actually matched */
               int n = bwb_strlen( T->Name ); /* number of characters to match */
               int q = p;
               
               for( m = 0; m < n && buff_skip_char( buffer, &q, T->Name[ m ] ); m++ );
               if( m == n )
               {
                  /* FOUND */
                  *position = q;
                  return T;
               }
            }
         }
      }
   }
   /* NOT FOUND */
   return NULL;
}
#if FALSE /* keep line_... */
static OperatorType * line_read_operator( LineType * line, unsigned char LastPrec, unsigned char Arity )
{
   bwx_DEBUG(__FUNCTION__);
   return buff_read_operator( line->buffer, &(line->position), LastPrec, Arity );
}
#endif
static ResultType buff_read_string_constant( char * buffer, int * position, VariantType * X )
{
   int p;
   bwx_DEBUG(__FUNCTION__);

   p = *position;
   if( buffer[ p ] == BasicQuoteChar )
   {
      int q; /* start of constant */
      X->TypeChar = BasicStringSuffix;
      p++; /* skip leading quote */
      /* determine the length of the quoted string */
      X->Length = 0;
      q = p;
      while( buffer[ p ] )
      {
         if( buffer[ p ] == BasicQuoteChar )
         {
            p++; /* quote */
            if( buffer[ p ] == BasicQuoteChar )
            {
               /* embedded string "...""..." */
            }
            else
            {
               /* properly terminated string "...xx..." */
               break;
            }
         }
         X->Length++;
         p++;
      }
      X->Buffer = calloc( X->Length + 1 /* NUL */, sizeof( char ) );
      /* copy the quoted string */
      X->Length = 0;
      p = q;
      while( buffer[ p ] )
      {
         if( buffer[ p ] == BasicQuoteChar )
         {
            p++; /* skip quote */
            if( buffer[ p ] == BasicQuoteChar )
            {
               /* embedded string "...""..." */
            }
            else
            {
               /* properly terminated string "...xx..." */
               break;
            }
         }
         X->Buffer[ X->Length ] = buffer[ p ];
         X->Length++;
         p++;
      }
      X->Buffer[ X->Length ] = NUL;
      *position = p;
      return RESULT_SUCCESS;
   }
   /* NOT FOUND */
   return RESULT_UNPARSED;
}
#if FALSE /* keep line_... */
static ResultType line_read_string_constant( LineType * line, VariantType * X )
{
   bwx_DEBUG(__FUNCTION__);
   return buff_read_string_constant( line->buffer, &(line->position), X );
}
#endif
static ResultType buff_read_hexadecimal_constant( char * buffer, int * position, VariantType * X )
{
   /* &h... */
   int p;
   bwx_DEBUG(__FUNCTION__);


   p = *position;
   if( buffer[ p ] == '&' )
   {
      p++; /* skip '&' */
      if( bwb_isalpha( buffer[ p ] ) )
      {
         if( bwb_tolower( buffer[ p ] ) == 'h' )
         {
            /* &h... */
            p++; /* skip 'h' */
            if( bwb_isxdigit( buffer[ p ] ) )
            {
               /* &hABCD */
               int n; /* number of characters read */
               unsigned long x; /* value read */
      
               if( sscanf( &buffer[ p ], "%lx%n", &x, &n ) == 1 )
               {
                  /* FOUND */
                  p += n;
                  
                  X->Number = x;
                  switch( buffer[ p ] )
                  {
                  case BasicStringSuffix  : 
                     /* oops */
                     WARN_SYNTAX_ERROR;
                     return RESULT_ERROR;
                     /* break; */
                  case BasicDoubleSuffix  :
                  case BasicSingleSuffix  :
                  case BasicCurrencySuffix:
                  case BasicLongSuffix    :
                  case BasicIntegerSuffix :
                  case BasicByteSuffix    :
                     X->TypeChar = buffer[ p ];
                     p++; /* skip TypeChar */
                     /* TODO: verify value fits in the specified type */
                     break;
                  default:
                     X->TypeChar = min_value_type( X );
                  }
                  *position = p;
                  return RESULT_SUCCESS;
               }
            }
            /* not HEXADECIMAL */
         }
      }
   }
   /* NOT FOUND */
   return RESULT_UNPARSED;
}
#if FALSE /* keep line_... */
static ResultType line_read_hexadecimal_constant( LineType * line, VariantType * X )
{
   bwx_DEBUG(__FUNCTION__);
   return buff_read_hexadecimal_constant( line->buffer, &(line->position), X );
}
#endif
static ResultType buff_read_octal_constant( char * buffer, int * position, VariantType * X )
{
   /* &o... */
   int p;
   bwx_DEBUG(__FUNCTION__);


   p = *position;
   if( buffer[ p ] == '&' )
   {
      p++; /* skip '&' */
      if( bwb_tolower( buffer[ p ] ) == 'o' )
      {
         /* &o777 */
         p++;  /* skip 'o' */
         /* fall-thru */
      }
      if( bwb_isodigit( buffer[ p ] ) )
      {
         /* &o777 */
         /* &777 */
         int n; /* number of characters read */
         unsigned long x; /* value read */

         if( sscanf( &buffer[ p ], "%lo%n", &x, &n ) == 1 )
         {
            /* FOUND */
            p += n;
            
            X->Number = x;
            switch( buffer[ p ] )
            {
            case BasicStringSuffix  : 
               /* oops */
               WARN_SYNTAX_ERROR;
               return RESULT_ERROR;
               /* break; */
            case BasicDoubleSuffix  :
            case BasicSingleSuffix  :
            case BasicCurrencySuffix:
            case BasicLongSuffix    :
            case BasicIntegerSuffix :
            case BasicByteSuffix    :
               X->TypeChar = buffer[ p ];
               p++; /* skip TypeChar */
               /* TODO: verify value fits in the specified type */
               break;
            default:
               X->TypeChar = min_value_type( X );
            }
            *position = p;
            return RESULT_SUCCESS;
         }
      }
   }
   /* NOT FOUND */
   return RESULT_UNPARSED;
}
#if FALSE /* keep line_... */
static ResultType line_read_octal_constant( LineType * line, VariantType * X )
{
   bwx_DEBUG(__FUNCTION__);
   return buff_read_octal_constant( line->buffer, &(line->position), X );
}
#endif
static ResultType buff_read_internal_constant( char * buffer, int * position, VariantType * X )
{
   /* &... */
   int p;
   bwx_DEBUG(__FUNCTION__);


   p = *position;

   if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
   {
      /* IBM System/360 and System/370 BASIC dialects */
      if( buffer[ p ] == '&' )
      {
         p++; /* skip '&' */
         if( bwb_isalpha( buffer[ p ] ) )
         {
            char * S;
            S = &( buffer[ p ] );
            if ( bwb_strnicmp( S, "PI", 2 ) == 0 )
            {
               /* &PI */
               p += 2;
               X->Number = 3.14159265358979;
               X->TypeChar = BasicDoubleSuffix;
               *position = p;
               return RESULT_SUCCESS;
            }
            if ( bwb_strnicmp( S, "E", 1 ) == 0 )
            {
               /* &E */
               p += 1;
               X->Number = 2.71828182845905;
               X->TypeChar = BasicDoubleSuffix;
               *position = p;
               return RESULT_SUCCESS;
            }
            if ( bwb_strnicmp( S, "SQR2", 4 ) == 0 )
            {
               /* &SQR2 */
               p += 4;
               X->Number = 1.41421356237309;
               X->TypeChar = BasicDoubleSuffix;
               *position = p;
               return RESULT_SUCCESS;
            }
            /* NOT a magic word */
         }
      }
   }
   /* NOT FOUND */
   return RESULT_UNPARSED;
}
#if FALSE /* keep line_... */
static ResultType line_read_internal_constant( LineType * line, VariantType * X )
{
   bwx_DEBUG(__FUNCTION__);
   return buff_read_internal_constant( line->buffer, &(line->position), X );
}
#endif
static ResultType buff_read_decimal_constant( char * buffer, int * position, VariantType * X )
{
   int p;
   bwx_DEBUG(__FUNCTION__);


   p = *position;
   if( bwb_isdigit( buffer[ p ] ) || buffer[ p ] == '.' )
   {
      /* .12345 */
      /* 123.45 */
      /* 123456 */
      /* 123E45 */
      /* TODO:  'D' instead of 'E' */
      int n; /* number of characters read */

      if( sscanf( &buffer[ p ], "%lg%n", &X->Number, &n ) == 1 )
      {
         /* FOUND */
         p += n;
         
         switch( buffer[ p ] )
         {
         case BasicStringSuffix  : 
            /* oops */
            WARN_SYNTAX_ERROR;
            return RESULT_ERROR;
            /* break; */
         case BasicDoubleSuffix  :
         case BasicSingleSuffix  :
         case BasicCurrencySuffix:
         case BasicLongSuffix    :
         case BasicIntegerSuffix :
         case BasicByteSuffix    :
            X->TypeChar = buffer[ p ];
            p++; /* skip TypeChar */
            /* TODO: verify value fits in the specified type */
            break;
         default:
            X->TypeChar = BasicDoubleSuffix; /* min_value_type( X ); */
         }
         *position = p;
         return RESULT_SUCCESS;
      }
   }
   /* NOT FOUND */
   return RESULT_UNPARSED;
}
#if FALSE /* keep line_... */
static int line_read_decimal_constant( LineType * line, VariantType * X )
{
   return buff_read_decimal_constant( line->buffer, &(line->position), X );
}
#endif
static ResultType buff_read_function( char * buffer, int * position, VariantType * X )
{
   int p;
   char name[ BasicNameLengthMax + 1 ];
   bwx_DEBUG(__FUNCTION__);


   p = *position;
   if( buff_read_varname( buffer, &p, name ) )
   {
      FunctionType *f;
      f = fnc_find( name );
      if( f != NULL )
      {
         /* if( TRUE ) */
         {
            /* here we handle some pseudo-functions that return information about arrays */
            char Xbound = NUL;
            
            if( bwb_stricmp( name, "DET" ) == 0 )
            {
               if( buff_peek_char( buffer, &p, BasicLeftParenChar ) )
               {
                  /* N = DET( varname ) */
                  Xbound = 'd';
               }
               else
               {
                  /* N = DET is handled by F_DET_N */
               }
            }
            else
            if( bwb_stricmp( name, "DIM" ) == 0 )
            {
               /* N = DIM( varname ) */
               /* return total number of dimensions */
               Xbound = 'D';
            }
            else
            if( bwb_stricmp( name, "SIZE" ) == 0 )
            {
               if( My->CurrentVersion->OptionVersionBitmask & ( C77 ) )
               {
                  /* N = SIZE( filename ) is handled by F_SIZE_A_N */
               }
               else
               {
                  /* N = SIZE( varname ) */
                  /* return total number of elements */
                  Xbound = 'S';
               }
            }
            else
            if( bwb_stricmp( name, "LBOUND" ) == 0 )
            {
               /* N = LBOUND( varname [ , dimension ] ) */
               /* return LOWER bound */
               Xbound = 'L';
            }
            else
            if( bwb_stricmp( name, "UBOUND" ) == 0 )
            {
               /* N = UBOUND( varname [ , dimension ] ) */
               /* return UPPER bound */
               Xbound = 'U';
            }
            if( Xbound != NUL )
            {
               VariableType * v = NULL;
               int dimension = 0; /* default */
               char varname[ BasicNameLengthMax + 1 ];

               if( buff_skip_char( buffer, &p, BasicLeftParenChar ) == FALSE )
               {
                  WARN_SYNTAX_ERROR;
                  return RESULT_ERROR;
               }
               if( buff_read_varname( buffer, &p, varname ) == FALSE )
               {
                  WARN_SYNTAX_ERROR;
                  return RESULT_ERROR;
               }
               /* search for array */
               v = mat_find( varname );
               if( v == NULL )
               {
                  WARN_TYPE_MISMATCH;  
                  return RESULT_ERROR;
               }
               if( v->dimensions == 0 )
               {
                  /* calling DIM() or SIZE() or LBOUND() or UBOUND() on a scalar is an ERROR */
                  WARN_TYPE_MISMATCH;  
                  return RESULT_ERROR;
               }
               switch( Xbound )
               {
               case 'd': /* DET() */
               case 'D': /* DIM() */
               case 'S': /* SIZE() */
                  break;
               case 'L': /* LBOUND() */
               case 'U': /* UBOUND() */
                  if( buff_skip_comma( buffer, &p ) )
                  {
                     ResultType ResultCode;
                     VariantType t;
                     VariantType *T = &t;

                     ResultCode = buff_read_expr( buffer, &p, T, 1 );
                     if( ResultCode != RESULT_SUCCESS )
                     {
                        /* ERROR */
                        RELEASE( T );
                        return ResultCode;
                     }
                     if( is_string_type( T ) )
                     {
                        RELEASE( T );
                        WARN_TYPE_MISMATCH;
                        return RESULT_ERROR;
                     }
                     T->Number = bwb_rint( T->Number );
                     if( T->Number < 1 || T->Number > v->dimensions )
                     {
                        WARN_TYPE_MISMATCH;
                        return RESULT_ERROR;
                     }
                     dimension = (int) bwb_rint( T->Number );
                     dimension--; /* BASIC to C */
                  }
                  else
                  {
                     dimension = 0; /* default */
                  }
                  break;
               default:
                  WARN_INTERNAL_ERROR;
                  return RESULT_ERROR;
                  /* break; */
               }
               if( buff_skip_char( buffer, &p, BasicRightParenChar ) == FALSE )
               {
                  WARN_SYNTAX_ERROR;
                  return RESULT_ERROR;
               }
               /* OK */
               switch( Xbound )
               {
               case 'd': /* DET() */
                  if( Determinant( v ) )
                  {
                     X->Number = My->LastDeterminant;
                  }
                  else
                  {
                     X->Number = 0;
                  }
                  break;
               case 'D': /* DIM() */
                  X->Number = v->dimensions;
                  break;
               case 'S': /* SIZE() */
                  X->Number = v->array_units;
                  break;
               case 'L': /* LBOUND() */
                  X->Number = v->LBOUND[ dimension ];
                  break;
               case 'U': /* UBOUND() */
                  X->Number = v->UBOUND[ dimension ];
                  break;
               default:
                  WARN_INTERNAL_ERROR;
                  return RESULT_ERROR;
                  /* break; */
               }
               X->TypeChar = BasicLongSuffix;
               *position = p;
               return RESULT_SUCCESS;
            }
         }
         /* if( TRUE ) */
         {
            /* it is a function */
            unsigned char   ParameterCount = 0;
            unsigned long   ParameterTypes = 0;
            VariableType * argv;
            VariableType * argn;
   

#define PARAM_NUMBER  *argn->memnum
#define PARAM_BUFFER   argn->memstr->sbuffer
#define PARAM_LENGTH   argn->memstr->length

            argv = var_chain(NULL); /* RETURN variable */
            argn = NULL;
   
            if( buff_skip_char( buffer, &p, BasicLeftParenChar ) )
            {
               if( buff_skip_char( buffer, &p, BasicRightParenChar ) )
               {
                  /*  RND() */
               }
               else
               {
                  /*  RND( 1, 2, 3 ) */
                  do
                  {
                     ResultType ResultCode;
                     VariantType T;

                     ResultCode = buff_read_expr( buffer, &p, &T, 1 );
                     if( ResultCode != RESULT_SUCCESS )
                     {
                        /* ERROR */
                        var_free(argv); /* free ARGV chain */
                        return ResultCode;
                     }
                     /* add value to ARGV chain      */
                     argn = var_chain(argv);
                     /* 'argn' is the variable to use */
                     if( is_string_type( &T ) )
                     {
                        /* STRING */
                        var_make(argn, BasicStringSuffix);
                        argn->memstr = calloc( 1, sizeof( StringType ) );
                        PARAM_LENGTH = T.Length;
                        /* PARAM_BUFFER = T.Buffer; */
                        PARAM_BUFFER = calloc( 1, T.Length + 1 );
                        bwb_memcpy( PARAM_BUFFER, T.Buffer, T.Length );
                        PARAM_BUFFER[ PARAM_LENGTH ] = NUL;
                        /* add type  to ParameterTypes */
                        if( ParameterCount < 32 )
                        {
                           ParameterTypes |= (1 << ParameterCount);
                        }
                     }
                     else
                     {
                        /* NUMBER */
                        var_make(argn, BasicDoubleSuffix);
                        PARAM_NUMBER = T.Number;
                     }
                     /* increment ParameterCount */
                     if( ParameterCount < 255 )
                     {
                        ParameterCount++;
                     }
                     /* RELEASE( &T ); */
                  }
                  while( buff_skip_comma( buffer, &p ) );
                  
                  
                  if( buff_skip_char( buffer, &p, BasicRightParenChar ) == FALSE )
                  {
                     /* ERROR */
                     var_free(argv); /* free ARGV chain */
                     WARN_SYNTAX_ERROR;
                     return RESULT_ERROR;
                  }
               }
            }
            else
            {
               /* RND */
            }

            /* search for exact match to the function parameter signature */
            if (ParameterCount > 32)
            {
               /* FORCE VARIANT */
               ParameterCount = 0xFF;
               ParameterTypes = 0;
            }
            /* did we find the correct function above? */
            if (f->ParameterCount != ParameterCount || f->ParameterTypes != ParameterTypes)
            {
               /* oops */
               f = fnc_find_exact(name, ParameterCount, ParameterTypes);
               if (f == NULL)
               {
                  /* NOT FOUND */
                  f = fnc_find_exact(name, 0xFF, 0);  /* look for VARIANT */
               }
               if (f == NULL)
               {
                  /* NOT FOUND */
                  var_free(argv); /* free ARGV chain */
                  sprintf( My->bwb_ebuf, "Illegal Function Call (%s)", name ); 
                  bwb_Warning( ERROR_ILLEGAL, My->bwb_ebuf );  return RESULT_ERROR;
               }
               /* FOUND */
            }
            /* defaullt the return value */
#define RESULT_NUMBER  *argv->memnum
#define RESULT_BUFFER   argv->memstr->sbuffer
#define RESULT_LENGTH   argv->memstr->length
            var_make(argv, f->ReturnType & 0x00FF);
            bwb_strcpy(argv->name, name);
            if( VAR_IS_STRING( argv ) )
            {
               RESULT_BUFFER = (char *) calloc( BasicStringLengthMax + 1, sizeof( char ) );
               RESULT_LENGTH = 0;
               RESULT_BUFFER[ RESULT_LENGTH ] = NUL;
            }
            else
            {
               RESULT_NUMBER = 0;
            }
            /* execute function */      
            if (f->ReturnType & 0xFF00  /* UniqueID is a line number */ )
            {
               /* for all USER DEFINED FUNCTIONS: f->UniqueID == line number of DEF FN... */
               unsigned short  cmdnum;
               cmdnum = (f->ReturnType & 0xFF00) >> 8;
               switch (cmdnum)
               {
               case C_DEF:       /* execute a user function   declared using DEF FN   ...(...) = ... */
               case C_FUNCTION:  /* execute a user function   declared using FUNCTION ...(...) */
               case C_SUB:       /* execute a user subroutine declared using SUB      ...(...) */
                  fnc_deffn(ParameterCount, argv, f->UniqueID);
                  break;
               case C_USER_LBL:  /* IF ERL > label1 AND ERL < label2 THEN ... */
                  if (ParameterCount > 0)
                  {
                     var_free(argv); /* free ARGV chain */
                     sprintf( My->bwb_ebuf, "Illegal Function Call (%s)", name ); 
                     bwb_Warning( ERROR_ILLEGAL, My->bwb_ebuf );  return RESULT_ERROR;
                  }
                  /* return the line number associated with the label */
                  RESULT_NUMBER = f->UniqueID;
                  break;
               default:
                  /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
                  var_free(argv); /* free ARGV chain */
                  WARN_INTERNAL_ERROR;
                  return RESULT_ERROR;
                  /* break; */
               }
            }
            else
            {
               /* for all INTRINSIC FUNCTIONS: f->UniqueID == #define F_... */
               fnc_intrinsic(ParameterCount, argv, f->UniqueID);
            }
            /* return results */
            X->TypeChar = argv->VariableTypeChar;
            if( VAR_IS_STRING( argv ) )
            {
               X->Length = RESULT_LENGTH;
               X->Buffer = RESULT_BUFFER;
               RESULT_BUFFER = NULL;
            }
            else
            {
               X->Number = RESULT_NUMBER;
            }
            /* free ARGV chain */
            var_free(argv);
            /* OK */
            *position = p;
            return RESULT_SUCCESS;
         }
      }
   }
   /* NOT FOUND */
   return RESULT_UNPARSED;
}
#if FALSE /* keep line_... */
static int line_read_function( LineType * line, VariantType * X )
{
   return buff_read_function( line->buffer, &(line->position), X );
}
#endif


static ResultType buff_read_variable( char * buffer, int * position, VariantType * X )
{
   int p;
   char name[ BasicNameLengthMax + 1 ];
   bwx_DEBUG(__FUNCTION__);


   p = *position;
   if( buff_read_varname( buffer, &p, name ) )
   {
      VariableType *v;
      int             n_params;
      int             pp[MAX_DIMS];

      if( buff_peek_char( buffer, &p, BasicLeftParenChar ) )
      {
         /* array */
         if( buff_peek_array_dimensions( buffer, &p, &n_params ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return RESULT_ERROR;
         }
         v = var_find(name,n_params,TRUE);
      }
      else
      {
         /* scalar */
         v = var_find(name,0,TRUE);
      }
      if( v == NULL )
      {
         WARN_VARIABLE_NOT_DECLARED;  
         return RESULT_ERROR;
      }
      if( v->dimensions > 0 )
      {
         /* array */
         register int    n;

         if( buff_read_array_dimensions( buffer, &p, &n_params, pp ) == FALSE )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;  
            return RESULT_ERROR;
         }
         for (n = 0; n < v->dimensions; ++n)
         {
            if ( pp[n] < v->LBOUND[n] || pp[n] > v->UBOUND[n] )
            {
               WARN_SUBSCRIPT_OUT_OF_RANGE;  
               return RESULT_ERROR;
            }
            v->array_pos[ n ] = pp[ n ];
         }
      }
      if( var_get( v, X ) == FALSE ) { WARN_TYPE_MISMATCH; return RESULT_ERROR; }
      *position = p;
      return RESULT_SUCCESS;
   }
   /* NOT FOUND */
   return RESULT_UNPARSED;
}
#if FALSE /* keep line_... */
static int line_read_variable( LineType * line, VariantType * X )
{
   return buff_read_variable( line->buffer, &(line->position), X );
}
#endif
/*
--------------------------------------------------------------------------------------------
                               Precedence Climbing Expression Parser
--------------------------------------------------------------------------------------------
*/

/*
// Read an infix expression containing top-level operators that bind at least
// as tightly as the given precedence.
// Don't consume the first non-digit character after the last number.
// Complain if you can't even find the first number,
// or if there is an operator with no following number.
*/
static ResultType buff_read_expr( char * buffer, int * position, VariantType * X, unsigned char LastPrec ) 
{
   ResultType ResultCode;
   OperatorType *C;
   int p;
   bwx_DEBUG(__FUNCTION__);
   p = *position;
   
   bwb_memset( X, 0, sizeof( VariantType ) ); /* NOTE */
   
   ResultCode = buff_read_primary( buffer, &p, X );
   if( ResultCode != RESULT_SUCCESS )
   {
      return ResultCode;
   }
   if( X->TypeChar == NUL )
   {
      /* we do not know the primary's type */
      WARN_INTERNAL_ERROR;
      return RESULT_ERROR;
   }
   buff_skip_spaces( buffer, &p );
   while ( (C = buff_read_operator( buffer, &p, LastPrec, BINARY ) ) != NULL )
   {
      VariantType Y;
            
      ResultCode = buff_read_expr( buffer, &p, &Y, C->NextPrec );
      if( ResultCode != RESULT_SUCCESS )
      {
         /* ERROR */
         if( Y.Buffer != NULL )
         {
            free( Y.Buffer ); Y.Buffer = NULL;
         }
         return ResultCode;
      }
      ResultCode = C->Eval( X, &Y );
      if( Y.Buffer != NULL )
      {
         free( Y.Buffer ); Y.Buffer = NULL;
      }
      if( ResultCode != RESULT_SUCCESS )
      {
         /* ERROR */
         return ResultCode;
      }
      /* OK */
   }
   /*
   Normal termination, such as end-of-line, ',', or "THEN".
   */
   *position = p;
   return RESULT_SUCCESS;
}
#if FALSE /* keep line_... */
static ResultType line_read_expr( LineType * line, VariantType * X, unsigned char LastPrec ) 
{
   return buff_read_expr( line->buffer, &(line->position), X, LastPrec );
}
#endif
static ResultType buff_read_primary( char * buffer, int * position, VariantType * X ) 
{
   ResultType ResultCode;
   OperatorType * C;
   int p;
   bwx_DEBUG(__FUNCTION__);


   p = *position;
   buff_skip_spaces( buffer, &p );
   if( buff_is_eol( buffer, &p ) )
   {
      /* we expected to find something, but there is nothing here */
      WARN_SYNTAX_ERROR;
      return RESULT_ERROR;
   }
   /* there is something to parse */   
   if( buff_skip_char( buffer, &p, BasicLeftParenChar ) )
   {
      /* nested expression */
      ResultCode = buff_read_expr( buffer, &p, X, 1 );
      if( ResultCode != RESULT_SUCCESS )
      {
         return ResultCode;
      }
      if( buff_skip_char( buffer, &p, BasicRightParenChar ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return RESULT_ERROR;
      }
      *position = p;
      return RESULT_SUCCESS;   
   }
   /* not a nested expression */
   C = buff_read_operator( buffer, &p, 1, UNARY );
   if( C != NULL )
   {
      ResultCode = buff_read_expr( buffer, &p, X, C->NextPrec );
      if( ResultCode != RESULT_SUCCESS )
      {
         return ResultCode;
      }
      ResultCode = C->Eval( X, NULL );
      if( ResultCode != RESULT_SUCCESS )
      {
         return ResultCode;
      }
      *position = p;
      return RESULT_SUCCESS;   
   }
   /* not an operator */
   ResultCode = buff_read_string_constant( buffer, &p, X );
   if( ResultCode != RESULT_UNPARSED )
   {
      /* either OK or ERROR */
      if( ResultCode == RESULT_SUCCESS )
      {
         *position = p;
      }
      return ResultCode;
   }
   ResultCode = buff_read_hexadecimal_constant( buffer, &p, X );
   if( ResultCode != RESULT_UNPARSED )
   {
      /* either OK or ERROR */
      if( ResultCode == RESULT_SUCCESS )
      {
         *position = p;
      }
      return ResultCode;
   }
   ResultCode = buff_read_octal_constant( buffer, &p, X );
   if( ResultCode != RESULT_UNPARSED )
   {
      /* either OK or ERROR */
      if( ResultCode == RESULT_SUCCESS )
      {
         *position = p;
      }
      return ResultCode;
   }
   ResultCode = buff_read_internal_constant( buffer, &p, X );
   if( ResultCode != RESULT_UNPARSED )
   {
      /* either OK or ERROR */
      if( ResultCode == RESULT_SUCCESS )
      {
         *position = p;
      }
      return ResultCode;
   }
   ResultCode = buff_read_decimal_constant( buffer, &p, X );
   if( ResultCode != RESULT_UNPARSED )
   {
      /* either OK or ERROR */
      if( ResultCode == RESULT_SUCCESS )
      {
         *position = p;
      }
      return ResultCode;
   }
   /* not a constant */
   ResultCode = buff_read_function( buffer, &p, X );
   if( ResultCode != RESULT_UNPARSED )
   {
      /* either OK or ERROR */
      if( ResultCode == RESULT_SUCCESS )
      {
         *position = p;
      }
      return ResultCode;
   }
   /* not a function */
   ResultCode = buff_read_variable( buffer, &p, X );
   /* 
   the variable will be implicitly created unless:
   OPTION EXPLICIT ON, or
   the varname matches an existing command/function/operator.
   */
   if( ResultCode != RESULT_UNPARSED )
   {
      /* either OK or ERROR */
      if( ResultCode == RESULT_SUCCESS )
      {
         *position = p;
      }
      return ResultCode;
   }
   /* not a variable */
   WARN_SYNTAX_ERROR;
   return RESULT_ERROR;
}
#if FALSE /* keep line_... */
static ResultType line_read_primary( LineType * line, VariantType * X ) 
{
   return buff_read_primary( line->buffer, &(line->position), X );
}
#endif


int buff_read_expression( char * buffer, int * position, VariantType * X ) 
{
   int p;
   bwx_DEBUG(__FUNCTION__);

   p = *position;
   
   
   if( buff_read_expr( buffer, &p, X, 1 ) == RESULT_SUCCESS )
   {
      switch( X->TypeChar )
      {
      case BasicStringSuffix  : 
         /* OK */
         if( My->MaintainerDebugOn )
         {
            sprintf( My->bwb_ebuf, "%s, STRING:%s\n", __FUNCTION__, X->Buffer ); fputs( My->bwb_ebuf, My->SYSPRN->cfp );
         }
         break;
      case BasicDoubleSuffix  :
      case BasicSingleSuffix  :
      case BasicCurrencySuffix:
      case BasicLongSuffix    :
      case BasicIntegerSuffix :
      case BasicByteSuffix    :
         /* OK */
         if( My->MaintainerDebugOn )
         {
            sprintf( My->bwb_ebuf, "%s, NUMBER:%f\n", __FUNCTION__, X->Number ); fputs( My->bwb_ebuf, My->SYSPRN->cfp );
         }
         break;
      default                 :
         /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
         RELEASE( X );
         WARN_INTERNAL_ERROR;
         return FALSE;
         /* break; */
      }
      *position = p;
      return TRUE;
   }
   RELEASE( X ); /* NEW */
   return FALSE;
}


int line_read_expression( LineType * line, VariantType * X ) 
{
   return buff_read_expression( line->buffer, &(line->position), X );
}

/*
--------------------------------------------------------------------------------------------
                               BASIC commands
--------------------------------------------------------------------------------------------
*/

#if FALSE /* keep line_... */
LineType * bwb_EVAL( LineType * line )
{
   /*
   EVAL 1 + 2 + 3
   EVAL "ABC" & "DEF"
   */
   ResultType ResultCode;
   VariantType x;
   VariantType * X = &x;
   bwx_DEBUG(__FUNCTION__);


   ResultCode = line_read_expression( line, X );
   if( ResultCode != RESULT_SUCCESS )
   {
      return bwb_zline( line );
   }

   switch( X->TypeChar )
   {
   case BasicStringSuffix  :
      printf(" STRING: %s, %c\n", X->Buffer, X->TypeChar );
      break;
   case BasicDoubleSuffix  :
   case BasicSingleSuffix  :
   case BasicCurrencySuffix:
   case BasicLongSuffix    :
   case BasicIntegerSuffix :
   case BasicByteSuffix    :
      printf(" NUMBER: %g, %c\n", X->Number, X->TypeChar );
      break;
   default                 :
      /*** FATAL - INTERNAL ERROR - SHOULD NEVER HAPPEN ***/
      WARN_INTERNAL_ERROR;
      break;
   }
   RELEASE( X );
   return bwb_zline( line );
}
#endif

LineType * bwb_OPTION_DISABLE_OPERATOR(LineType * l)
{
   /* OPTION DISABLE OPERATOR ... */
   int             IsFound = FALSE;

   bwx_DEBUG(__FUNCTION__);
   /* Get OPERATOR */
   {
      char * Value = NULL;
      
      if( line_read_string_expression( l, &Value ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( Value == NULL )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      {
         /* Name */
         int i;
         for( i = 0; i < NUM_OPS; i++ )
         {
             if( bwb_stricmp( Value, OperatorTable[i].Name ) == 0 )
             {
                 /* FOUND */
                 /* DISABLE OPERATOR */
                 OperatorTable[i].OptionVersionBitmask &= ~My->CurrentVersion->OptionVersionBitmask;
                 IsFound = TRUE;
             }
         }
      }
      free( Value );
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( My->bwb_ebuf, "IGNORED: %s\n", l->buffer); fputs(My->bwb_ebuf, My->SYSOUT->cfp );
   }
   return bwb_zline(l);
}

LineType * bwb_OPTION_ENABLE_OPERATOR(LineType * l)
{
   /* OPTION ENABLE OPERATOR ... */
   int             IsFound = FALSE;

   bwx_DEBUG(__FUNCTION__);
   /* Get OPERATOR */
   {
      char * Value = NULL;
      
      if( line_read_string_expression( l, &Value ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( Value == NULL )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      {
         /* Name */
         int i;
         for( i = 0; i < NUM_OPS; i++ )
         {
             if( bwb_stricmp( Value, OperatorTable[i].Name ) == 0 )
             {
                 /* FOUND */
                 /* ENABLE OPERATOR */
                 OperatorTable[i].OptionVersionBitmask |= My->CurrentVersion->OptionVersionBitmask;
                 IsFound = TRUE;
             }
         }
      }
      free( Value );
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( My->bwb_ebuf, "IGNORED: %s\n", l->buffer); fputs(My->bwb_ebuf, My->SYSOUT->cfp );
   }
   return bwb_zline(l);
}

void DumpOneOperatorSyntax(int n, int lp)
{
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   if (n < 0 || n >= NUM_OPS)
   {
      return;
   }
   /* NAME */
   {
      sprintf(tbuf, "     SYNTAX: %s\n", OperatorTable[n].Syntax);
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
                     
      sprintf(tbuf, "DESCRIPTION: %s\n", OperatorTable[n].Description);
      FixDescription( tbuf );
      {
         char *C;
         
         C = tbuf;
         while( *C != BasicNulChar )
         {
            if( *C == '\t' )
            {
               char tbuf2[32];
               bwb_strcpy( tbuf2, "\n             " );
               if (lp)
               {
                  prn_lprintf(tbuf2);
               }
               else
               {
                  prn_xprintf(tbuf2);
               }
               C++;
               while( *C == ' ' )
               {
                  C++;
               }
            }
            else
            {
               if( lp )
               {
                  bwx_LPRINT( *C );
               }
               else
               {
                  bwx_PRINT( *C );
               }
               C++;
            }
         }
      }
   }
   /* PRECEDENCE */
   {
      sprintf(tbuf, " PRECEDENCE: %d\n", OperatorTable[n].ThisPrec);
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
         if (OperatorTable[n].OptionVersionBitmask & bwb_vertable[i].OptionVersionBitmask)
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

void DumpAllOperatorSyntax(void)
{
   /* for the C maintainer */
   int             n;
   bwx_DEBUG(__FUNCTION__);

   prn_lprintf("============================================================\n");
   prn_lprintf("                    OPERATORS                               \n");
   prn_lprintf("============================================================\n");
   prn_lprintf("\n");
   prn_lprintf("\n");
   for (n = 0; n < NUM_OPS; n++)
   {
      prn_lprintf("------------------------------------------------------------\n");
      DumpOneOperatorSyntax(n, TRUE);

   }
   prn_lprintf("------------------------------------------------------------\n");
}

/* EOF */
