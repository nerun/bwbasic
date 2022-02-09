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
/* Version 3.10 by Howard Wulf, AF5NE                            */
/*                                                               */
/*---------------------------------------------------------------*/



#include "bwbasic.h"

int
buff_is_eol( char *buffer, int * position )
{
   /*
   determines whether 'position' is at the NUL (BasicNulChar) in 'buffer'.
   if successful then returns TRUE
   otherwise returns FALSE.
   'position' is unchanged.
   */
   int p;
   
   p = *position;
   if( buffer[p] == BasicNulChar )
   {
      return TRUE;
   }
   return FALSE;
}

int
line_is_eol( LineType * line )
{
   return buff_is_eol( line->buffer, &(line->position) );
}

int
buff_skip_eol( char *buffer, int * position )
{
   /*
   skip to the NUL (BasicNulChar) in 'buffer'.
   if successful then 'position' is updated and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   */
   int p;
   
   p = *position;
   if( buffer[p] != BasicNulChar )
   {
      while( buffer[p] != BasicNulChar )
      {
         p++;
      }
      *position = p;
      return TRUE;
   }
   return FALSE;
}

int
line_skip_eol( LineType * line )
{
   return buff_skip_eol( line->buffer, &(line->position) );
}

int
buff_skip_spaces( char *buffer, int * position )
{
   /*
   skip spaces in 'buffer'.
   if successful then 'position' is updated and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   */
   int p;
   
   p = *position;
   
   if( buffer[p] == ' ' )
   {
      while( buffer[p] == ' ' )
      {
         p++;
      }
      *position = p;
      return TRUE;
   }
   return FALSE;
}

int
line_skip_spaces( LineType * line )
{
   return buff_skip_spaces( line->buffer, &(line->position) );
}


int
buff_peek_char( char *buffer, int * position, char find )
{
   /*
   determine whether the next non-space character in 'buffer' is 'find'.
   if successful then returns TRUE
   otherwise returns FALSE.
   'position' is unchanged.
   */
   int p;
   
   p = *position;
   
   
   buff_skip_spaces( buffer, &p );
   if( bwb_toupper(buffer[p]) == bwb_toupper(find) )
   {
      return TRUE;
   }
   return FALSE;
}

int
line_peek_char( LineType * line, char find )
{
   return buff_peek_char( line->buffer, &(line->position), find );
}

int
buff_skip_char( char *buffer, int * position, char find )
{
   /*
   skip the next non-space character in 'buffer' if it is 'find'.
   if successful then 'position' is updated past 'find' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   */
   int p;
   
   p = *position;
   
   
   buff_skip_spaces( buffer, &p );
   if( bwb_toupper(buffer[p]) == bwb_toupper(find) )
   {
      p++;
      *position = p;
      return TRUE;
   }
   return FALSE;
}


int
line_skip_char( LineType * line, char find )
{
   return buff_skip_char( line->buffer, &(line->position), find );
}


int
buff_skip_comma( char *buffer, int * position )
{
   /*
   skip the next non-space character in 'buffer' if it is a comma, semicolon, or colon.
   if successful then 'position' is updated past the character and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   */
   int p;
   
   p = *position;
   
   
   buff_skip_spaces( buffer, &p );
   switch( buffer[p] )
   {
   case ',': /* COMMA */
   case ';': /* SEMICOLON */
   case ':': /* COLON */
      p++;
      buff_skip_spaces( buffer, &p );
      *position = p;
      return TRUE;
   }
   return FALSE;
}


int
line_skip_comma( LineType * line )
{
   return buff_skip_comma( line->buffer, &(line->position) );
}

static int char_is_varfirst( char C )
{
   /*
   determine whether the character is allowed to be the first character of a BASIC variable name.
   if successful then returns TRUE
   otherwise returns FALSE.
   */
   if( bwb_isalpha( C ) )
   {
      return TRUE;
   }
   return FALSE;
}

static int char_is_varnext( char C )
{
   /*
   determine whether the character is allowed to be the second character of a BASIC variable name.
   if successful then returns TRUE
   otherwise returns FALSE.
   */
   if( bwb_isalnum( C ) )
   {
      return TRUE;
   }
   switch(C)
   {
   case '.':
   case '_':
      if( My->CurrentVersion->OptionFlags & (OPTION_BUGS_ON) /* varnamex: period and underscore are allowed */ )
      {
         return TRUE;
      }
   }
   return FALSE;
}

int char_is_vartype( char C )
{
   /*
   determine whether the character is allowed to be the type character of a BASIC variable name.
   if successful then returns TRUE
   otherwise returns FALSE.
   */
   switch( C )
   {
   case BasicStringSuffix:
      return TRUE;
   case BasicDoubleSuffix:
   case BasicSingleSuffix:
   case BasicCurrencySuffix:
   case BasicLongSuffix:
   case BasicIntegerSuffix:
   case BasicByteSuffix:
      if( My->CurrentVersion->OptionFlags & (OPTION_BUGS_ON) /* varname: numeric type-chars allowed */ )
      {
         return TRUE;
      }
   }
   return FALSE;
}

static int char_is_varhead( char C )
{
   if( char_is_varfirst( C ) )
   {
      return TRUE;
   }
   if( char_is_varnext( C ) )
   {
      return TRUE;
   }
   return FALSE;
}

int char_is_vartail( char C )
{
   if( char_is_varnext( C ) )
   {
      return TRUE;
   }
   if( char_is_vartype( C ) )
   {
      return TRUE;
   }
   return FALSE;
}

int
buff_peek_word( char *buffer, int * position, char * find )
{
   /*
   determine whether the next non-space word in 'buffer' is 'find';
   the word 'find' is not a sub-string of a bigger word.
   if successful then returns TRUE
   otherwise returns FALSE.
   'position' is unchanged.
   */
   int p;
   int n;
   p = *position;
   
   
   buff_skip_spaces( buffer, &p );
   if( buff_is_eol( buffer, &p ) )
   {
      return FALSE;
   }
   n = bwb_strlen(find);
   if( bwb_strnicmp( &(buffer[p]), find, n) == 0 )
   {
      if( p > 0 )
      {
         if( char_is_varhead( buffer[p-1] ) )
         {
            /* _TO */
            return FALSE;
         }
      }
      if( char_is_vartail( buffer[p+n] ) )
      {
         /* TO_ */
         return FALSE;
      }
      return TRUE;
   }
   return FALSE;
}

int
line_peek_word( LineType * line, char * find )
{
   return buff_peek_word( line->buffer, &(line->position), find );
}

int
buff_skip_word( char *buffer, int * position, char * find )
{
   /*
   skip the next non-space word in 'buffer' if it is 'find';
   the word 'find' is not a sub-string of a bigger word.
   if successful then 'position' is updated past 'find' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   */
   int p;
   int n;
   p = *position;
   
   
   buff_skip_spaces( buffer, &p );
   if( buff_is_eol( buffer, &p ) )
   {
      return FALSE;
   }
   n = bwb_strlen(find);
   if( bwb_strnicmp( &(buffer[p]), find, n) == 0 )
   {
      if( p > 0 )
      {
         if( char_is_varhead( buffer[p-1] ) )
         {
            /* _TO */
            return FALSE;
         }
      }
      if( char_is_vartail( buffer[p+n] ) )
      {
         /* TO_ */
         return FALSE;
      }
      p += n;
      *position = p;
      return TRUE;
   }
   return FALSE;
}

int
line_skip_word( LineType * line, char * find )
{
   return buff_skip_word( line->buffer, &(line->position), find );
}

int
buff_read_varname( char *buffer, int * position, char * varname )
{
   /*
   read the next non-space word in 'buffer' that conforms to a BASIC variable name into 'varname'.
   if successful then 'position' is updated past 'varname' and returns TRUE
   otherwise 'position' is unchanged ('varname' is truncated) and returns FALSE.
   'varname' shall be declared "char varname[BasicNameLengthMax + 1]".
   */
   int p;
   
   p = *position;
   
   buff_skip_spaces( buffer, &p );
   if( char_is_varfirst( buffer[p] ) )
   {
      int i;
      i = 0;
      
      if( i > BasicNameLengthMax )
      {
         i = BasicNameLengthMax;
      }
      varname[i] = buffer[p];
      p++;
      i++;
      while (char_is_varnext(buffer[p]))
      {
         if( i > BasicNameLengthMax )
         {
            i = BasicNameLengthMax;
         }
         varname[i] = buffer[p];
         p++;
         i++;
      }
      if (char_is_vartype(buffer[p]))
      {
         if( i > BasicNameLengthMax )
         {
            i = BasicNameLengthMax;
         }
         varname[i] = buffer[p];
         p++;
         i++;
      }
      varname[i] = BasicNulChar;
      *position = p;
      return TRUE;
   }
   varname[0] = BasicNulChar;
   return FALSE;
}

int
line_read_varname( LineType * line, char * varname )
{
   return buff_read_varname( line->buffer, &(line->position), varname );
}

int
buff_read_label( char * buffer, int * position, char * label )
{
   /*
   read the next non-space word in 'buffer' that conforms to a BASIC label name into 'label'.
   if successful then 'position' is updated past 'label' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   'label' shall be declared "char label[BasicNameLengthMax + 1]".
   */
   int p;
   
   p = *position;
   
   buff_skip_spaces( buffer, &p );

   if( char_is_varfirst( buffer[p] ) )
   {
      int i;
      i = 0;
      
      if( i > BasicNameLengthMax )
      {
         i = BasicNameLengthMax;
      }
      label[i] = buffer[p];
      p++;
      i++;
      while (char_is_varnext(buffer[p]))
      {
         if( i > BasicNameLengthMax )
         {
            i = BasicNameLengthMax;
         }
         label[i] = buffer[p];
         p++;
         i++;
      }
      label[i] = BasicNulChar;
      *position = p;
      return TRUE;
   }
   return FALSE;
}

int
line_read_label( LineType * line, char * label )
{
   return buff_read_label( line->buffer, &(line->position), label );
}

static int buff_read_keyword( char * buffer, int * position, char * keyword )
{
   /*
   read the next non-space word in 'buffer' that conforms to a BASIC keyword into 'keyword'.
   if successful then 'position' is updated past 'keyword' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   'label' shall be declared "char keyword[BasicNameLengthMax + 1]".
   */
   int p;
   
   p = *position;
   
   buff_skip_spaces( buffer, &p );
   if( char_is_varfirst( buffer[p] ) )
   {
      int i;
      i = 0;
      
      if( i > BasicNameLengthMax )
      {
         i = BasicNameLengthMax;
      }
      keyword[i] = buffer[p];
      p++;
      i++;
      while (char_is_varnext(buffer[p]))
      {
         if( i > BasicNameLengthMax )
         {
            i = BasicNameLengthMax;
         }
         keyword[i] = buffer[p];
         p++;
         i++;
      }
      if( buffer[p] == BasicStringSuffix )
      {
         if( i > BasicNameLengthMax )
         {
            i = BasicNameLengthMax;
         }
         keyword[i] = buffer[p];
         p++;
         i++;
      }
      keyword[i] = BasicNulChar;
      *position = p;
      return TRUE;
   }
   keyword[0] = BasicNulChar;
   return FALSE;
}


VariableType *
buff_read_scalar( char *buffer, int * position )
{
   /*
   read the next non-space word in 'buffer' that conforms to a BASIC variable name,
   including both scalar variables and subscripted array variables.
   if successful then 'position' is updated 
   past 'varname' for scalar variables
   (past ')' for subscripted array variables).
   and returns a pointer to the variable.
   otherwise 'position' is unchanged and returns NULL.
   */

   int p;
   VariableType *v;
   char varname[BasicNameLengthMax + 1];

   
   p = *position;

   /* Read a variable name */
   buff_skip_spaces( buffer, &p );   
   if( buff_read_varname(buffer, &p, varname) == FALSE )
   {
      WARN_VARIABLE_NOT_DECLARED;
      return NULL;
   }
   if( buff_peek_char( buffer, &p, BasicLeftParenChar ) )
   {
      /* MUST be a an array */
      register int    n;
      int             n_params;  /* number of parameters */
      int             pp[ MAX_DIMS ];
           
      /* get parameters because the variable is dimensioned */
      if( buff_read_array_dimensions(buffer, &p, &n_params, pp) == FALSE )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return NULL;
      }
      /* get the array variable */
      if( (v = var_find(varname,n_params,TRUE)) == NULL )
      {
         WARN_VARIABLE_NOT_DECLARED;
         return NULL;
      }
      for (n = 0; n < v->dimensions; n++)
      {
         v->array_pos[n] = pp[n];
      }
   }
   else
   {
      /* simple scalar variable */
      
      if( (v = var_find(varname,0,TRUE)) == NULL )
      {
         WARN_VARIABLE_NOT_DECLARED;
         return NULL;
      }
   }
   *position = p;
   return v;
}

VariableType *
line_read_scalar( LineType * line )
{
   return buff_read_scalar( line->buffer, &(line->position) );
}

VariableType *
buff_read_matrix( char *buffer, int * position )
{
   /*
   read the next non-space word in 'buffer' that conforms to a BASIC matrix name,
   including both simple matrix variables and redimensioned matrix variables.
   if successful then 'position' is updated 
   past 'varname' for matrix variables
   (past ')' for redimensioned matrix variables).
   and returns a pointer to the variable.
   otherwise 'position' is unchanged and returns NULL.
   */

   int p;
   VariableType *v;
   char varname[BasicNameLengthMax + 1];

   
   p = *position;

   /* Read a variable name */
   buff_skip_spaces( buffer, &p );   
   if( buff_read_varname(buffer, &p, varname) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return NULL;
   }
   buff_skip_spaces( buffer, &p );   
   v = mat_find( varname );
   if( v == NULL)
   {
      WARN_VARIABLE_NOT_DECLARED;
      return NULL;
   }
   if (buff_peek_char(buffer, &p, BasicLeftParenChar))
   {
      /* get requested matrix size, which is <= original matrix size */
      size_t array_units;
      int    n;
      int    n_params;  /* number of parameters */
      int    pp[ MAX_DIMS ];

      for (n = 0; n < MAX_DIMS; n++)
      {
         pp[n] = 0;
      }
      if( buff_read_array_dimensions(buffer, &p, &n_params, pp) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return NULL;
      }
      /* update array dimensions */
      array_units = 1;
      for (n = 0; n < n_params; n++)
      {
         if( pp[n] < My->CurrentVersion->OptionBaseValue )
         {
            WARN_SUBSCRIPT_OUT_OF_RANGE;
            return NULL;
         }
         array_units *= pp[n] - My->CurrentVersion->OptionBaseValue + 1;
      }
      if( array_units > v->array_units )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return NULL;
      }
      v->dimensions = n_params;
      for (n = 0; n < v->dimensions; n++)
      {
         v->LBOUND[n] = My->CurrentVersion->OptionBaseValue;
         v->UBOUND[n] = pp[n];
      }
   }
   *position = p;
   return v;
}

VariableType *
line_read_matrix( LineType * line )
{
   return buff_read_matrix( line->buffer, &(line->position) );
}

int
buff_read_line_number( char * buffer, int * position, int * linenum )
{
   /*
   read the next non-space word in 'buffer' that conforms to a BASIC line number.
   if successful then 'position' is updated past 'linenum' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   */
   int p;
   
   p = *position;
   
   buff_skip_spaces( buffer, &p );

   if( bwb_isdigit( buffer[p] ) )
   {
      int i;
      char label[BasicNameLengthMax + 1];
      
      i = 0;
      while (bwb_isdigit(buffer[p]))
      {
         if( i > BasicNameLengthMax )
         {
            i = BasicNameLengthMax;
         }
         label[i] = buffer[p];
         p++;
         i++;
      }
      label[i] = BasicNulChar;
      *linenum = atoi( label );
      *position = p;
      return TRUE;
   }
   return FALSE;
}

int
line_read_line_number( LineType * line, int * linenum )
{
   return buff_read_line_number( line->buffer, &(line->position), linenum );
}

int
buff_read_line_sequence( char *buffer, int * position, int * head, int * tail )
{
   /*
   read the next non-space words in 'buffer' that conforms to a BASIC line number sequnence.
   if successful then 'position' is updated past the line number sequence and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   */
   /*
   ###          head == tail
   ### -        head to BMAX
   ### - ###    head to tail
       - ###    BMIN to tail
   */
   int p;
   int h;
   int t;
   
   p = *position;
   
   buff_skip_spaces( buffer, &p );
   if( buff_skip_char( buffer, &p, '-' ) )
   {
      /* - ... */
      if( buff_read_line_number( buffer, &p, &t ) )
      {
         /* - ### */
         *head = BasicLineNumberMin;
         *tail = t;
         *position = p;
         return TRUE;
      }
   }
   else
   if( buff_read_line_number( buffer, &p, &h ) )
   {
      /* ### ... */
      if( buff_skip_char( buffer, &p, '-' ) )
      {
         /* ### - ... */
         if( buff_read_line_number( buffer, &p, &t ) )
         {
            /* ### - ### */
            *head = h;
            *tail = t;
            *position = p;
            return TRUE;
         }
         else
         {
            /* ### - */
            *head = h;
            *tail = BasicLineNumberMax;
            *position = p;
            return TRUE;
         }
      }
      else
      {
         /* ### */
         *head = h;
         *tail = h;
         *position = p;
         return TRUE;
      }
   }
   return FALSE;
}

int
line_read_line_sequence( LineType * line, int * head, int * tail )
{
   return buff_read_line_sequence( line->buffer, &(line->position), head, tail );
}


 
int
buff_read_integer_expression( char * buffer, int * position, int * Value )
{
   /*
   read the next non-space words in 'buffer' that conforms to a BASIC integer expression into 'Value'.
   if successful then 'position' is updated past 'Value' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   'Value' shall be declared "int Value".
   */
   BasicNumberType X;
   int p;
   bwx_DEBUG(__FUNCTION__);

   p = *position;
   buff_skip_spaces( buffer, &p );
   if( buff_read_numeric_expression( buffer, &p, &X ) )
   {
      /* we want the rounded value */
      X = bwb_rint( X );
      if( INT_MIN <= X && X <= INT_MAX )
      {
         /* OK */
         *Value = (int) bwb_rint( X );
         *position = p;
         return TRUE;
      }
   }
   /* ERROR */
   if( My->MaintainerDebugOn )
   {
      sprintf( My->bwb_ebuf, "%s, BUFFER:%s\n", __FUNCTION__, &( buffer[ p ] ) ); fputs( My->bwb_ebuf, My->SYSPRN->cfp );
   }
   return FALSE;
}

int
line_read_integer_expression( LineType * line, int * Value )
{
   return buff_read_integer_expression( line->buffer, &(line->position), Value );
}


int
buff_read_numeric_expression( char * buffer, int * position, BasicNumberType * Value )
{
   /*
   read the next non-space words in 'buffer' that conforms to a BASIC numeric expression into 'Value'.
   if successful then 'position' is updated past 'Value' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   'Value' shall be declared "BasicNumberType Value".
   */
   int p;
   VariantType x;
   VariantType * X = &x;

   p = *position;
   CLEAR_VARIANT( X );
   
   buff_skip_spaces( buffer, &p );
   if( buff_read_expression( buffer, &p, X ) == FALSE )
   {
      return FALSE;   
   }
   if( X->TypeChar != BasicStringSuffix ) 
   {
      /* OK */
      *Value = X->Number;
      *position = p;
      return TRUE;
   }
   RELEASE( X );
   return FALSE;
}


int
line_read_numeric_expression( LineType * line, BasicNumberType * Value )
{
   return buff_read_numeric_expression( line->buffer, &(line->position), Value );
}



int
buff_read_string_expression( char * buffer, int * position, char ** Value )
{
   /*
   read the next non-space words in 'buffer' that conforms to a BASIC string expression into 'Value'.
   if successful then 'position' is updated past 'Value' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   'Value' shall be declared "char * Value = NULL".
   */
   int p;
   VariantType x;
   VariantType * X = &x;

   p = *position;
   CLEAR_VARIANT( X );
   
   buff_skip_spaces( buffer, &p );
   if( buff_read_expression( buffer, &p, X ) == FALSE )
   {
      return FALSE;   
   }
   if( X->TypeChar == BasicStringSuffix ) 
   {
      /* OK */
      X->Buffer[ X->Length ] = BasicNulChar;
      *Value = X->Buffer;
      *position = p;
      return TRUE;
   }
   return FALSE;
}

int
line_read_string_expression( LineType * line, char ** Value )
{
   return buff_read_string_expression( line->buffer, &(line->position), Value );
}

int
buff_read_index_item( char * buffer, int * position, int Index, int * Value )
{
   /*
   read the next non-space words in 'buffer' that conforms to a BASIC integer expression list into 'Value',
   selecting the item matching 'Index'.  The first 'Index' value is one; 
   if successful then 'position' is updated past 'Value' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   'Value' shall be declared "int Value".
   */
   int p;
   int i;
      
   p = *position;
   
   buff_skip_spaces( buffer, &p );
   if( buff_is_eol( buffer, &p ) )
   {
      return FALSE;
   }
   if( Index < 1 )
   {
      return FALSE;
   }
   /* Index >= 1 */
   i = 0;
   do
   {
      int v;

      if( buff_read_integer_expression( buffer, &p, &v ) )
      {
         i++;
         if( i == Index )
         {
            *Value = v;
            *position = p;
            return TRUE;
         }
      }
      else
      {
         return FALSE;
      }
   }
   while( buff_skip_comma( buffer, &p ) );
   return FALSE;
}

int
line_read_index_item( LineType * line, int Index, int * Value )
{
   return buff_read_index_item( line->buffer, &(line->position), Index, Value );
}


int
buff_read_element( char * buffer, int * position, char * element )
{
   /*
   read the next non-space word in 'buffer' that conforms to a BASIC element.
   if successful then 'position' is updated past the element and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   'element' shall be declared "char element[BasicStringLengthMax + 1]".
   */
   int p;
   int i;
      
   p = *position;
   i = 0;
   
   buff_skip_spaces( buffer, &p );
   if( buff_is_eol( buffer, &p ) )
   {
      return FALSE;
   }

   while( TRUE )
   {

      switch (buffer[p])
      {
      case BasicNulChar:
      case ',': /* COMMA */   
      case ';': /* SEMICOLON */
      case ':': /* COLON */
      case '=':
      case ' ':
         /* element terminators */
         element[i] = BasicNulChar;
         *position = p;
         return TRUE;
         /* break; */
      case BasicQuoteChar:  
         /* string constant */
         if( i > 0 )
         {  
            /* terminates previous element */
            element[i] = BasicNulChar;
            *position = p;
            return TRUE;
         }
         /* return the string constant */
         if( i > BasicStringLengthMax )
         {
            i = BasicStringLengthMax;
         }
         element[i] = buffer[p];
         i++;
         p++;
         while( buffer[p] != BasicNulChar && buffer[p] != BasicQuoteChar )
         {
            if( i > BasicStringLengthMax )
            {
               i = BasicStringLengthMax;
            }
            element[i] = buffer[p];
            i++;
            p++;
         }
         if( buffer[p] == BasicQuoteChar )
         {
            p++;
         }
         if( i > BasicStringLengthMax )
         {
            i = BasicStringLengthMax;
         }
         element[i] = BasicQuoteChar;
         i++;
         element[i] = BasicNulChar;
         *position = p;
         return TRUE;
         /* break; */
      default:
         if( i > BasicStringLengthMax )
         {
            i = BasicStringLengthMax;
         }
         element[i] = buffer[p];
         i++;
         p++;
         break;
      }
   }
   /* return FALSE; */ /* never reached */
}

int
line_read_element( LineType * line, char * element )
{
   return buff_read_element( line->buffer, &(line->position), element );
}

int
buff_read_letter_sequence(char *buffer, int *position, char *start, char *end)
{
   /*
   read the next non-space alphabetic character in 'buffer' into 'start';
   if seperated by a '-' then read the next non-space alphabetic character into 'end'.
   if successful then 'position' is updated past 'start' (or 'end') and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   */
   int p;
   char s;
   char e;

   p = *position;
   
   buff_skip_spaces(buffer, &p);
   if ( ! bwb_isalpha(buffer[p]) )
   {
      /* character at this position must be a letter */
      return FALSE;
   }
   s = buffer[p];
   p++;

   /* check for hyphen, indicating sequence of more than one letter */
   if ( buff_skip_char( buffer, &p, '-') )
   {
      buff_skip_spaces(buffer, &p);
      if ( ! bwb_isalpha(buffer[p]) )
      {
         /* character at this position must be a letter */
         return FALSE;
      }
      e = buffer[p];
      p++;
   }
   else
   {
      e = s;
   }
   *start = s;
   *end = e;
   *position = p;
   return TRUE;
}

int
line_read_letter_sequence( LineType * line, char *start, char *end)
{
   return buff_read_letter_sequence( line->buffer, &(line->position), start, end );
}


int
buff_read_array_dimensions(char *buffer, int *position, int *n_params , int params[ /* MAX_DIMS */ ] )
{
   /*
   read the next non-space words in 'buffer' that conform to BASIC array index values;
   if successful then 'position' is updated past the ending ')' and returns TRUE
   otherwise 'position' is unchanged and returns FALSE.
   */
   int p;
   int n;
   
   p = *position;
   n = 0;
   
   buff_skip_spaces(buffer, &p);
   if ( buff_skip_char(  buffer, &p, BasicLeftParenChar) )
   {
      /* matrix */
      do
      {
         int Value;

         if( n < MAX_DIMS )
         {
            /* OK */
            if( buff_read_integer_expression( buffer, &p, &Value) )
            {
               params[n] = Value;
               n++;
            }
            else
            {
               return FALSE;
            }
         }
         else
         {
            /* ERROR */
            return FALSE;
         }
      }
      while( buff_skip_comma(  buffer, &p ) );
      
      if ( buff_skip_char(  buffer, &p, BasicRightParenChar) == FALSE )
      {
         /* ERROR */
         return FALSE;
      }
   }
   else
   {
      /* scalar */
      n = 0;
   }
   *n_params = n;
   *position = p;
   return TRUE;   
}

int
line_read_array_dimensions( LineType * line, int *n_params , int params[ /* MAX_DIMS */ ] )
{
   return buff_read_array_dimensions(line->buffer, &(line->position), n_params , params );
}

int
buff_peek_array_dimensions(char *buffer, int *position, int *n_params )
{
   /*
   peek the next non-space words in 'buffer' that conform to BASIC array index values;
   if successful then 'n_params' is updated and returns TRUE
   otherwise 'n_params' is unchanged and returns FALSE.
   'position' is always unchanged.
   */
   int p;
   int             ParenLevel = 0;
   int             NumDimensions = 1;

   p = *position;
   buff_skip_spaces( buffer, &p );   
   while( buffer[p] != BasicNulChar )
   {
      /* check the current character */
      buff_skip_spaces( buffer, &p );   
      if ( buff_skip_char(  buffer, &p, BasicLeftParenChar) )
      {
         ParenLevel++;
      }
      else
      if ( buff_skip_char(  buffer, &p, BasicRightParenChar) )
      {
         ParenLevel--;
         if (ParenLevel < 0)
         {
            return FALSE;
         }
         if (ParenLevel == 0)
         {
            *n_params = NumDimensions;
            return TRUE;
         }
      }
      else
      if ( buff_skip_comma(  buffer, &p) )
      {
         if (ParenLevel == 1)
         {
            NumDimensions++;
         }
      }
      else
      if( buffer[p] == BasicQuoteChar )
      {
         /* embedded string constant */
         p++;
         while ((buffer[p] != BasicQuoteChar) && (buffer[p] != BasicNulChar))
         {
            p++;
         }
         if (buffer[p] == BasicQuoteChar)
         {
            p++;
         }
      }
      else
      {
         /* normal character */
         p++;
      }
   }
   return FALSE;
}

int
line_peek_array_dimensions( LineType * line, int *n_params )
{
   return buff_peek_array_dimensions(line->buffer, &(line->position), n_params );
}

char
buff_read_type_declaration( char *buffer, int * position )
{
   /*
   skip the next non-space words in 'buffer' if it is a BASIC type declaration.
   if successful then 'position' is updated past the BASIC type declaration and returns the TypeChar
   otherwise 'position' is unchanged and returns BasicNulChar.
   */
   int p;
   char TypeChar;
   p = *position;
   TypeChar = BasicNulChar; 
   
   
   buff_skip_spaces( buffer, &p );
   if( buff_is_eol( buffer, &p ) )
   {
      return FALSE;
   }

   if( buff_skip_word(buffer, &p, "AS") == TRUE )
   {
      /* AS ... */
      
      if( buff_skip_word(buffer, &p, "STRING" ) )
      {
         /* AS STRING */
         TypeChar = BasicStringSuffix;
      }
      else
      if( buff_skip_word(buffer, &p, "DOUBLE" ) )
      {
         /* AS DOUBLE */
         TypeChar = BasicDoubleSuffix;
      }
      else
      if( buff_skip_word(buffer, &p, "SINGLE" ) )
      {
         /* AS SINGLE */
         TypeChar = BasicSingleSuffix;
      }
      else
      if( buff_skip_word(buffer, &p, "CURRENCY" ) )
      {
         /* AS CURRENCY */
         TypeChar = BasicCurrencySuffix;
      }
      else
      if( buff_skip_word(buffer, &p, "LONG" ) )
      {
         /* AS LONG */
         TypeChar = BasicLongSuffix;
      }
      else
      if( buff_skip_word(buffer, &p, "INTEGER" ) )
      {
         /* AS INTEGER */
         TypeChar = BasicIntegerSuffix;
      }
      else
      if( buff_skip_word(buffer, &p, "BYTE" ) )
      {
         /* AS BYTE */
         TypeChar = BasicByteSuffix;
      }
      else
      {
         /* invalid type */
      }
   }
   if( TypeChar != BasicNulChar )
   {
      /* success */
      *position = p;
   }
   return TypeChar;
}

char
line_read_type_declaration( LineType * line )
{
   return buff_read_type_declaration(line->buffer, &(line->position) );
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
line_start(LineType * l)
{
   int             position;
   char            tbuf[BasicNameLengthMax + 1];


   bwx_DEBUG(__FUNCTION__);


   /* set initial values */
   l->Startpos = 0;
   l->cmdnum = 0;    /* not found */
   position = 0;

   buff_skip_spaces(l->buffer, &position);

   /* handle special cases */
   if (l->buffer[position] == BasicNulChar)
   {
      /* the NUL (0) char must be handled first */
      l->cmdnum = C_REM;
      return TRUE;
   }
   if (l->buffer[position] == My->CurrentVersion->OptionCommentChar)
   {
      l->cmdnum = C_REM;
      buff_skip_eol(l->buffer, &(position));
      l->Startpos = position;
      return TRUE;
   }
   if (l->buffer[position] == My->CurrentVersion->OptionPrintChar)
   {
      l->cmdnum = C_PRINT;
      l->Startpos = position;
      /* skip past '?' */
      l->Startpos++;
      return TRUE;
   }
   if (l->buffer[position] == My->CurrentVersion->OptionImageChar)
   {
      l->cmdnum = C_IMAGE;
      l->Startpos = position;
      /* skip past ':' */
      l->Startpos++;
      return TRUE;
   }
   if (bwb_strnicmp(&l->buffer[position], "REM", 3) == 0)
   {
      l->cmdnum = C_REM;
      buff_skip_eol(l->buffer, &(position));
      l->Startpos = position;
      return TRUE;
   }
   /* not a SPECIAL */  
   
   /* get the first keyword */
   if( buff_read_keyword(l->buffer, &position, tbuf) == FALSE )
   {
      /* ERROR */
      return FALSE;   
   }
   buff_skip_spaces(l->buffer, &position);


   /* check for COMMAND */
   if ((l->cmdnum = is_cmd(tbuf)) > 0)
   {
      /* it is a COMMAND */
      /* 
       * NOTE:  This is NOT a full parser, this exists only to 
       * handle STRUCTURED commands. It is true that we also handle 
       * some other easy cases, but remember that this only exists 
       * to support STRUCTURED commands. Whether any other commands 
       * get processed here is simply because it was easy to do so. 
       */

      int cmdnum = 0;

#define MAX_COMMAND_LENGTH 63       
      char xbuf[ MAX_COMMAND_LENGTH + 1 /* NUL */ ]; /* ON ERROR RESUME NEXT */
      
      bwb_strcpy( xbuf, tbuf );
      
      do
      {
         cmdnum = 0;
      
         buff_skip_spaces(l->buffer, &position);
         l->Startpos = position;
         if( buff_read_keyword(l->buffer, &position, tbuf) )
         {
            int n;
            n = bwb_strlen( xbuf ) + 1 + bwb_strlen( tbuf );
            if( n < MAX_COMMAND_LENGTH )
            {
               /* not too long */            
               bwb_strcat( xbuf, " "  );
               bwb_strcat( xbuf, tbuf );
               cmdnum = is_cmd( xbuf );
               if( cmdnum > 0 )
               {
                  /* longer command is valid */
                  buff_skip_spaces(l->buffer, &position);
                  l->cmdnum = cmdnum;
                  l->Startpos = position;
               }
            }
         }
      }
      while( cmdnum > 0 );

      /* process special cases here */
      switch (l->cmdnum)
      {
      case C_CLOAD:
         {
            position = l->Startpos;
            if( buff_skip_char(l->buffer, &position, '*' ) )
            {
               /* CLOAD* */
               buff_skip_spaces(l->buffer, &position);
               l->cmdnum = C_CLOAD8;
               l->Startpos = position;
            }
         }
         break;
      case C_CSAVE:
         {
            position = l->Startpos;
            if( buff_skip_char(l->buffer, &position, '*' ) )
            {
               /* CSAVE* */
               buff_skip_spaces(l->buffer, &position);
               l->cmdnum = C_CSAVE8;
               l->Startpos = position;
            }
         }
         break;
      case C_DEF:
         if( bwb_strchr( l->buffer, '=' ) == NULL )
         {
            /* multi-line DEF ... FNEND */
            l->cmdnum = C_FUNCTION;
         }
         fslt_add(l);
         break;
      case C_FEND: 
         l->cmdnum = C_END_FUNCTION;
         break;
      case C_FNEND:
         l->cmdnum = C_END_FUNCTION;
         break;
      case C_FUNCTION:
         fslt_add(l);
         break;
      case C_IF:
         if( IsLastKeyword( l, " THEN" )  )
         {
            /* STRUCTURED */
            l->cmdnum = C_IF_THEN;
         }
         break;
      case C_OPEN:
         {
   
            if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
            {
               /* OPEN filenum, filename$, INPUT | OUTPUT */
            }
            else
            if (GetKeyword(l, NULL, " AS "))
            {
               /* STRUCTURED */
            }
            else
            {
               /* CLASSIC */
               l->cmdnum = C_DEF_SUB;
               l->Startpos = 0;
            }
         }
         break;
      case C_SUB:
         fslt_add(l);
         break;
      case C_SUBEND:
         l->cmdnum = C_END_SUB;
         break;
      case C_USER_LBL:
         fslt_add(l);
         break;
      }
      /* done */
      return TRUE;


#if FALSE      
      if (l->cmdnum == C_CASE)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "ELSE") == 0)
         {
            l->cmdnum = C_CASE_ELSE;
            l->Startpos = position;
         }
         else
         {
            /* CASE ... */
            /* CASE IF ... */
            /* CASE IS ... */
         }
      }
      else
      if (l->cmdnum == C_CLOAD)
      {
         position = l->Startpos;
         if( buff_skip_char(l->buffer, &position, '*' ) )
         {
            /* CLOAD* */
            buff_skip_spaces(l->buffer, &position);
            l->cmdnum = C_CLOAD8;
            l->Startpos = position;
         }
         else
         {
            /* CLOAD ... */
         }
      }
      else
      if (l->cmdnum == C_CSAVE)
      {
         position = l->Startpos;
         if( buff_skip_char(l->buffer, &position, '*' ) )
         {
            /* CSAVE* */
            buff_skip_spaces(l->buffer, &position);
            l->cmdnum = C_CSAVE8;
            l->Startpos = position;
         }
         else
         {
            /* CSAVE ... */
         }
      }
      else
      if (l->cmdnum == C_DEF)
      {
         if(bwb_strchr(l->buffer,'=') == NULL)
         {
            /* multi-line DEF ... FNEND */
            l->cmdnum = C_FUNCTION;
            l->Startpos = position;
         }
         else
         {
            /* single line DEF */
         }
      }
      else
      if (l->cmdnum == C_FNEND)
      {
         l->cmdnum = C_END_FUNCTION;
      }
      else
      if (l->cmdnum == C_FEND)
      {
         l->cmdnum = C_END_FUNCTION;
      }
      else
      if (l->cmdnum == C_END)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "FUNCTION") == 0)
         {
            l->cmdnum = C_END_FUNCTION;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "IF") == 0)
         {
            l->cmdnum = C_END_IF;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "SELECT") == 0)
         {
            l->cmdnum = C_END_SELECT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "SUB") == 0)
         {
            l->cmdnum = C_END_SUB;
            l->Startpos = position;
         }
         else
         {
            /* plain END */
         }
      }
      else
      if (l->cmdnum == C_EXIT)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "DO") == 0)
         {
            l->cmdnum = C_EXIT_DO;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "FOR") == 0)
         {
            l->cmdnum = C_EXIT_FOR;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "FUNCTION") == 0)
         {
            l->cmdnum = C_EXIT_FUNCTION;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "SUB") == 0)
         {
            l->cmdnum = C_EXIT_SUB;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "UNTIL") == 0)
         {
            l->cmdnum = C_EXIT_UNTIL;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "WHILE") == 0)
         {
            l->cmdnum = C_EXIT_WHILE;
            l->Startpos = position;
         }
         else
         {
            /* EXIT line */
         }
      }
      else
      if (l->cmdnum == C_GO)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "SUB") == 0)
         {
            l->cmdnum = C_GOSUB;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "TO") == 0)
         {
            l->cmdnum = C_GOTO;
            l->Startpos = position;
         }
         else
         {
            /* syntax error */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_IF)
      {
         if (IsLastKeyword(l, " THEN") == TRUE)
         {
            /* STRUCTURED */
            l->cmdnum = C_IF_THEN;
         }
         else
         {
            /* CLASSIC */
            position = l->Startpos;
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "END") == 0)
            {
               /* IF END # X THEN ... */
               l->cmdnum = C_IF_END;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "MORE") == 0)
            {
               /* IF MORE # X THEN ... */
               l->cmdnum = C_IF_MORE;
               l->Startpos = position;
            }
            else
            {
               /* normal */
            }
         }
      }
      else
      if (l->cmdnum == C_MAT)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "GET") == 0)
         {
            l->cmdnum = C_MAT_GET;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "INPUT") == 0)
         {
            l->cmdnum = C_MAT_INPUT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "PRINT") == 0)
         {
            l->cmdnum = C_MAT_PRINT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "PUT") == 0)
         {
            l->cmdnum = C_MAT_PUT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "READ") == 0)
         {
            l->cmdnum = C_MAT_READ;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "WRITE") == 0)
         {
            l->cmdnum = C_MAT_WRITE;
            l->Startpos = position;
         }
         else
         {
            /* plain MAT */
         }
      }
      else
      if (l->cmdnum == C_ON)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "ERROR") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "GOTO") == 0)
            {
               l->cmdnum = C_ON_ERROR_GOTO;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "GOSUB") == 0)
            {
               l->cmdnum = C_ON_ERROR_GOSUB;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "RESUME") == 0)
            {
                buff_read_keyword(l->buffer, &position, tbuf);
                buff_skip_spaces(l->buffer, &position);
                if (bwb_stricmp(tbuf, "NEXT") == 0)
                {
                   l->cmdnum = C_ON_ERROR_RESUME_NEXT;
                   l->Startpos = position;
                }
                else
                {
                  /* syntax error */
                   l->cmdnum = 0;
                }
            }
            else
            if (bwb_stricmp(tbuf, "RETURN") == 0)
            {
                buff_read_keyword(l->buffer, &position, tbuf);
                buff_skip_spaces(l->buffer, &position);
                if (bwb_stricmp(tbuf, "NEXT") == 0)
                {
                   l->cmdnum = C_ON_ERROR_RETURN_NEXT;
                   l->Startpos = position;
                }
                else
                {
                  /* syntax error */
                   l->cmdnum = 0;
                }
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "TIMER") == 0)
         {
            l->cmdnum = C_ON_TIMER;
            l->Startpos = position;
         }
         else
         {
            /* ON ... GO ... */
         }
      }
      else
      if (l->cmdnum == C_OPEN)
      {

         if( My->CurrentVersion->OptionVersionBitmask & ( I70 | I73 ) )
         {
            /* OPEN filenum, filename$, INPUT | OUTPUT */
         }
         else
         if (GetKeyword(l, NULL, " AS "))
         {
            /* STRUCTURED */
         }
         else
         {
            /* CLASSIC */
            l->cmdnum = C_DEF_SUB;
            l->Startpos = 0;
         }
      }
      else
      if (l->cmdnum == C_OPTION)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "ANGLE") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "DEGREES") == 0)
            {
               l->cmdnum = C_OPTION_ANGLE_DEGREES;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "GRADIANS") == 0)
            {
               l->cmdnum = C_OPTION_ANGLE_GRADIANS;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "RADIANS") == 0)
            {
               l->cmdnum = C_OPTION_ANGLE_RADIANS;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "ARITHMETIC") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "DECIMAL") == 0)
            {
               l->cmdnum = C_OPTION_ARITHMETIC_DECIMAL;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "FIXED") == 0)
            {
               l->cmdnum = C_OPTION_ARITHMETIC_FIXED;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "NATIVE") == 0)
            {
               l->cmdnum = C_OPTION_ARITHMETIC_NATIVE;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "BASE") == 0)
         {
            l->cmdnum = C_OPTION_BASE;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "BUGS") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_BUGS_OFF;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_BUGS_ON;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "COMMENT") == 0)
         {
            l->cmdnum = C_OPTION_COMMENT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "COMPARE") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "BINARY") == 0)
            {
               l->cmdnum = C_OPTION_COMPARE_BINARY;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "DATABASE") == 0)
            {
               l->cmdnum = C_OPTION_COMPARE_DATABASE;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "TEXT") == 0)
            {
               l->cmdnum = C_OPTION_COMPARE_TEXT;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "COVERAGE") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_COVERAGE_OFF;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_COVERAGE_ON;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "DATE") == 0)
         {
            l->cmdnum = C_OPTION_DATE;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "DISABLE") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "COMMAND") == 0)
            {
               l->cmdnum = C_OPTION_DISABLE_COMMAND;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "FUNCTION") == 0)
            {
               l->cmdnum = C_OPTION_DISABLE_FUNCTION;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "OPERATOR") == 0)
            {
               l->cmdnum = C_OPTION_DISABLE_OPERATOR;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "ENABLE") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "COMMAND") == 0)
            {
               l->cmdnum = C_OPTION_ENABLE_COMMAND;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "FUNCTION") == 0)
            {
               l->cmdnum = C_OPTION_ENABLE_FUNCTION;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "OPERATOR") == 0)
            {
               l->cmdnum = C_OPTION_ENABLE_OPERATOR;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "ERROR") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "GOSUB") == 0)
            {
               l->cmdnum = C_OPTION_ERROR_GOSUB;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "GOTO") == 0)
            {
               l->cmdnum = C_OPTION_ERROR_GOTO;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "EXPLICIT") == 0)
         {
            l->cmdnum = C_OPTION_EXPLICIT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "IMAGE") == 0)
         {
            l->cmdnum = C_OPTION_IMAGE;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "IMPLICIT") == 0)
         {
            l->cmdnum = C_OPTION_IMPLICIT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "INDENT") == 0)
         {
            l->cmdnum = C_OPTION_INDENT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "LABELS") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_LABELS_OFF;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_LABELS_ON;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "PRINT") == 0)
         {
            l->cmdnum = C_OPTION_PRINT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "ROUND") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "BANK") == 0)
            {
               l->cmdnum = C_OPTION_ROUND_BANK;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "MATH") == 0)
            {
               l->cmdnum = C_OPTION_ROUND_MATH;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "TRUNCATE") == 0)
            {
               l->cmdnum = C_OPTION_ROUND_TRUNCATE;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "SLEEP") == 0)
         {
            l->cmdnum = C_OPTION_SLEEP;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "STATEMENT") == 0)
         {
            l->cmdnum = C_OPTION_STATEMENT;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "STRICT") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_STRICT_OFF;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_STRICT_ON;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "TERMINAL") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "NONE") == 0)
            {
               l->cmdnum = C_OPTION_TERMINAL_NONE;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "ADM-3A") == 0)
            {
               l->cmdnum = C_OPTION_TERMINAL_ADM;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "ANSI") == 0)
            {
               l->cmdnum = C_OPTION_TERMINAL_ANSI;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "TIME") == 0)
         {
            l->cmdnum = C_OPTION_TIME;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "TRACE") == 0)
         {
            buff_read_keyword(l->buffer, &position, tbuf);
            buff_skip_spaces(l->buffer, &position);
            if (bwb_stricmp(tbuf, "OFF") == 0)
            {
               l->cmdnum = C_OPTION_TRACE_OFF;
               l->Startpos = position;
            }
            else
            if (bwb_stricmp(tbuf, "ON") == 0)
            {
               l->cmdnum = C_OPTION_TRACE_ON;
               l->Startpos = position;
            }
            else
            {
               /* syntax error */
               l->cmdnum = 0;
            }
         }
         else
         if (bwb_stricmp(tbuf, "VERSION") == 0)
         {
            l->cmdnum = C_OPTION_VERSION;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "USING") == 0)
         {
            l->cmdnum = C_OPTION_USING;
            l->Startpos = position;
         }
         else
         {
            /* syntax error */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_SELECT)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "CASE") == 0)
         {
            l->cmdnum = C_SELECT_CASE;
            l->Startpos = position;
         }
         else
         {
            /* syntax error */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_SUBEND)
      {
         l->cmdnum = C_END_SUB;
      }
      else
      if (l->cmdnum == C_TIMER)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "OFF") == 0)
         {
            l->cmdnum = C_TIMER_OFF;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "ON") == 0)
         {
            l->cmdnum = C_TIMER_ON;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "STOP") == 0)
         {
            l->cmdnum = C_TIMER_STOP;
            l->Startpos = position;
         }
         else
         {
            /* syntax error */
            l->cmdnum = 0;
         }
      }
      else
      if (l->cmdnum == C_TRACE)
      {
         position = l->Startpos;
         buff_read_keyword(l->buffer, &position, tbuf);
         buff_skip_spaces(l->buffer, &position);
         if (bwb_stricmp(tbuf, "OFF") == 0)
         {
            l->cmdnum = C_TRACE_OFF;
            l->Startpos = position;
         }
         else
         if (bwb_stricmp(tbuf, "ON") == 0)
         {
            l->cmdnum = C_TRACE_ON;
            l->Startpos = position;
         }
         else
         {
            /* plain TRACE */
         }
      }
      /* check for a bogus assignment to a command */
      /* 100 PRINT = X */
      if (l->cmdnum == 0)
      {
         /* syntax error */
         return FALSE;
      }
      /* add to the Function-Sub-Label list */
      switch (l->cmdnum)
      {
      case C_DEF:
      case C_FUNCTION:
      case C_SUB:
      case C_USER_LBL:
         fslt_add(l);
         break;
      }
      /* done */
      return TRUE;
#endif
   }
   /* not a COMMAND */  

   /* check for implied LET */
   if (is_let(l->buffer) == TRUE)
   {
      /* this is an implied LET */
      /* 100 A = 123 */
      l->cmdnum = C_LET;   /* bwb_LET */
      l->Startpos = 0;
      return TRUE;
   }
   /* not an implied LET */  

   /* check for FUNCTION */
   if (fnc_find(tbuf) != NULL)
   {
      /* check for a bogus assignment to a function */
      /* 100 COS = X */
      if( buff_peek_char(l->buffer, &position, '=' ) )
      {
         /* syntax error */
         l->cmdnum = 0;
         return FALSE;
      }
      /* user is executing a function, such as 100 COS X */
      l->cmdnum = C_DEF_SUB;  /* bwb_DEF_SUB, in bwb_tbl.c */
      l->Startpos = 0;
      return TRUE;
   }
   /* not a FUNCTION */  

   /* check for LABEL */
   if (My->CurrentVersion->OptionFlags & OPTION_LABELS_ON)
   {
      /* labels are enabled */
      if( My->CurrentVersion->OptionStatementChar != BasicNulChar )
      {
         /* a Statement seperator exists */
         if( buff_skip_char(l->buffer, &position, My->CurrentVersion->OptionStatementChar) )
         {
            /* this is a label */
            /* 100 MyLabel: */
            l->cmdnum = C_USER_LBL; /* bwb_USER_LBL */
            buff_skip_spaces(l->buffer, &position);
            l->Startpos = position;
            return TRUE;
         }
      }
   }
   /* not a LABEL */  


   /* ERROR */
   return FALSE;

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
#if THE_PRICE_IS_RIGHT
   char C;
   
   bwx_DEBUG(__FUNCTION__);

#if FALSE
printf( "check [%s]\n", buffer ); /* HWULF */
#endif
   
   C = buffer[0];

   if( bwb_isalpha( C ) )
   {
      /* start with the closest command, without going over */
      int n;
      n = My->CommandStart[ VarTypeIndex( C ) ]; /* first command starting with this letter, assigned in SortAllCommands() */
      if( n < 0 )
      {
         /* NOT FOUND */
         return 0;
      }
      for( ; n < NUM_COMMANDS; n++ )
      {
         if (My->CurrentVersion->OptionVersionBitmask & bwb_cmdtable[n].OptionVersionBitmask)
         {
            int result;
            result = bwb_stricmp(bwb_cmdtable[n].name, buffer);
            if( result == 0 )
            {
               /* FOUND */
#if FALSE
printf( "found [%s]\n", buffer ); /* HWULF */
#endif
               return bwb_cmdtable[n].id;
            }
            if( result > 0 /* found > searched */ )
            {
               /* NOT FOUND */
               return 0;
            }
         }
      }
   }
   /* NOT FOUND */
   return 0;

#else
   register int    n;
   bwx_DEBUG(__FUNCTION__);


   /* Go through the command table and search for a match. */

   for (n = 0; n < NUM_COMMANDS; ++n)
   {
      if (My->CurrentVersion->OptionVersionBitmask & bwb_cmdtable[n].OptionVersionBitmask)
         if (bwb_stricmp(bwb_cmdtable[n].name, buffer) == 0)
         {
            return bwb_cmdtable[n].id;
         }
   }
   /* No command name was found */
   return 0;
#endif
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

   for (n = 0; buffer[n] != BasicNulChar; ++n)
   {
      switch (buffer[n])
      {
      case BasicQuoteChar:  /* string constant */
         ++n;
         while (buffer[n] != BasicQuoteChar)
         {
            ++n;
            if (buffer[n] == BasicNulChar)
            {
               WARN_SYNTAX_ERROR;
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
  
        FUNCTION:       bwb_stripcr()
  
   DESCRIPTION:    This function strips the carriage return
         or line-feed from the end of a string.
  
***************************************************************/

int
bwb_stripcr(char *s)
{
    char *E;

   bwx_DEBUG(__FUNCTION__);

    E = bwb_strchr(s, '\r');
    if( E != NULL )
    {
       *E = BasicNulChar;
    }
    E = bwb_strchr(s, '\n');
    if( E != NULL )
    {
       *E = BasicNulChar;
    }

   while (*s)
   {
      if ( ! bwb_isprint( *s ) )
      {
         *s = ' ';
      }
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

   if (buffer[0] == BasicNulChar)
   {
      return FALSE;
   }
   /* else check digits */

   p = buffer;
   while (*p != BasicNulChar)
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
  
        FUNCTION:       bwb_freeline()
  
   DESCRIPTION:    This function frees memory associated
         with a program line in memory.
  
***************************************************************/

int
bwb_freeline(LineType * l)
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


#if FALSE
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


   buff_skip_spaces(buffer, position);
   c = buffer[*position];


   if (c == BasicNulChar || c == My->CurrentVersion->OptionCommentChar)
   {
      return TRUE;
   }
   return FALSE;
}

char           *
GetRestOfLine(LineType * l, char *OutputBuffer)
{
   char           *S;
   char           *C;

   bwx_DEBUG(__FUNCTION__);


   S = l->buffer;
   S += l->position;
   C = S;
   while (*C != BasicNulChar && *C != My->CurrentVersion->OptionCommentChar)
   {
      C++;
   }
   /* FOUND */
   if (OutputBuffer != NULL)
   {
      char            c;

      c = *C;
      *C = BasicNulChar;
      l->position += bwb_strlen(S);
      bwb_strcpy(OutputBuffer, S);
      *C = c;
   }
   return C;
}
#endif /* FALSE */

char           *
GetKeyword(LineType * l, char *OutputBuffer, char *Keyword)
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
   n = bwb_strlen(Keyword);


   while (*C != BasicNulChar && *C != My->CurrentVersion->OptionCommentChar)
   {
      if (bwb_strnicmp(C, Keyword, n) == 0)
      {
         /* FOUND */
         char            c;

         c = *C;
         *C = BasicNulChar;
         if (OutputBuffer != NULL)
         {
            l->position += bwb_strlen(S);
            l->position += n;
            bwb_strcpy(OutputBuffer, S);
         }
         *C = c;
         C += n;
         return C;
      }
      else
      if (*C == BasicQuoteChar)
      {
         /* skip string constant */
         C++;
         while (*C != BasicNulChar && *C != BasicQuoteChar)
         {
            C++;
         }
         if (*C == BasicQuoteChar)
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
IsLastKeyword(LineType * l, char *Keyword)
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
   n = bwb_strlen(Keyword);
   S += n;
   /* IF x THEN0 IF x THEN'0 IF    x    THEN    '     0 */
   while (*C != BasicNulChar && *C != My->CurrentVersion->OptionCommentChar)
   {
      /* skip string constants */
      if( *C == BasicQuoteChar )
      {
          /* skip leading quote */
          C++;
          while (*C != BasicNulChar && *C != BasicQuoteChar)
          {
              C++;
          }
          /* skip trailing quote */
          if( *C == BasicQuoteChar )
          {
              C++;
          }
      }
      else
      {
          C++;
      }
   }
   if (C > S)
   {
      C--;
      while (C > S && *C == ' ')
      {
         C--;
      }
      C++;
      if (C > S)
      {
         C -= n;
         if (bwb_strnicmp(C, Keyword, n) == 0)
         {
            /* FOUND */
            return TRUE;
         }
      }
   }
   /* NOT FOUND */
   return FALSE;
}


int bwb_isalnum( int C )
{
   /*
   4.3.1.1 The isalnum function
   
   Synopsis
   
            #include <ctype.h>
            int isalnum(int c);
   
   Description
   
      The isalnum function tests for any character for which isalpha or
   isdigit is true.
   */
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
  case '8':
  case '9':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
    return TRUE;
  }
  return FALSE;
}

int bwb_isalpha( int C )
{
   /*
   4.3.1.2 The isalpha function
   
   Synopsis
   
            #include <ctype.h>
            int isalpha(int c);
   
   Description
   
      The isalpha function tests for any character for which isupper or
   islower is true, or any of an implementation-defined set of characters
   for which none of iscntrl , isdigit , ispunct , or isspace is true.
   In the C locale, isalpha returns true only for the characters for
   which isupper or islower is true.
   */
  switch( C )
  {
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
    return TRUE;
  }
  return FALSE;
}

int bwb_iscntrl( int C )
{
   /*
   4.3.1.3 The iscntrl function
   
   Synopsis
   
            #include <ctype.h>
            int iscntrl(int c);
   
   Description
   
      The iscntrl function tests for any control character.  
   */
  switch( C )
  {
  case ' ':
  case '!':
  case '"':
  case '#':
  case '$':
  case '%':
  case '&':
  case '\'':
  case '(':
  case ')':
  case '*':
  case '+':
  case ',':
  case '-':
  case '.':
  case '/':
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
  case ':':
  case ';':
  case '<':
  case '=':
  case '>':
  case '?':
  case '@':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case '[':
  case '\\':
  case ']':
  case '^':
  case '_':
  case '`':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
  case '{':
  case '|':
  case '}':
  case '~':
    return FALSE;
  }
  return TRUE;
}

int bwb_isdigit( int C )
{
   /*
   4.3.1.4 The isdigit function
   
   Synopsis
   
            #include <ctype.h>
            int isdigit(int c);
   
   Description
   
      The isdigit function tests for any decimal-digit character (as
   defined in $2.2.1).
   */
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
  case '8':
  case '9':
    return TRUE;
  }
  return FALSE;
}

int bwb_isgraph( int C )
{
   /*
   4.3.1.5 The isgraph function
   
   Synopsis
   
            #include <ctype.h>
            int isgraph(int c);
   
   Description
   
      The isgraph function tests for any printing character except space (' ').  
   */
  switch( C )
  {
  case '!':
  case '"':
  case '#':
  case '$':
  case '%':
  case '&':
  case '\'':
  case '(':
  case ')':
  case '*':
  case '+':
  case ',':
  case '-':
  case '.':
  case '/':
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
  case ':':
  case ';':
  case '<':
  case '=':
  case '>':
  case '?':
  case '@':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case '[':
  case '\\':
  case ']':
  case '^':
  case '_':
  case '`':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
  case '{':
  case '|':
  case '}':
  case '~':
    return TRUE;
  }
  return FALSE;
}

int bwb_islower( int C )
{
   /*
   4.3.1.6 The islower function
   
   Synopsis
   
            #include <ctype.h>
            int islower(int c);
   
   Description
   
      The islower function tests for any lower-case letter or any of an
   implementation-defined set of characters for which none of iscntrl ,
   isdigit , ispunct , or isspace is true.  In the C locale, islower
   returns true only for the characters defined as lower-case letters (as
   defined in $2.2.1).
   */
  switch( C )
  {
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
    return TRUE;
  }
  return FALSE;
}

int bwb_isprint( int C )
{
   /*
   4.3.1.7 The isprint function
   
   Synopsis
   
            #include <ctype.h>
            int isprint(int c);
   
   Description
   
      The isprint function tests for any printing character including
   space (' ').
   */
  switch( C )
  {
  case ' ':
  case '!':
  case '"':
  case '#':
  case '$':
  case '%':
  case '&':
  case '\'':
  case '(':
  case ')':
  case '*':
  case '+':
  case ',':
  case '-':
  case '.':
  case '/':
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
  case ':':
  case ';':
  case '<':
  case '=':
  case '>':
  case '?':
  case '@':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case '[':
  case '\\':
  case ']':
  case '^':
  case '_':
  case '`':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
  case '{':
  case '|':
  case '}':
  case '~':
    return TRUE;
  }
  return FALSE;
}

int bwb_ispunct( int C )
{
   /*
   4.3.1.8 The ispunct function
   
   Synopsis
   
            #include <ctype.h>
            int ispunct(int c);
   
   Description
   
      The ispunct function tests for any printing character except space
   (' ') or a character for which isalnum is true.
   */
  switch( C )
  {
  case '!':
  case '"':
  case '#':
  case '$':
  case '%':
  case '&':
  case '\'':
  case '(':
  case ')':
  case '*':
  case '+':
  case ',':
  case '-':
  case '.':
  case '/':
  case ':':
  case ';':
  case '<':
  case '=':
  case '>':
  case '?':
  case '@':
  case '[':
  case '\\':
  case ']':
  case '^':
  case '_':
  case '`':
  case '{':
  case '|':
  case '}':
  case '~':
    return TRUE;
  }
  return FALSE;
}

int bwb_isspace( int C )
{
   /*
   4.3.1.9 The isspace function
   
   Synopsis
   
            #include <ctype.h>
            int isspace(int c);
   
   Description
   
      The isspace function tests for the standard white-space characters
   or for any of an implementation-defined set of characters for which
   isalnum is false.  The standard white-space characters are the
   following: space (' '), form feed ('\f'), new-line ('\n'), carriage
   return ('\r'), horizontal tab ('\t'), and vertical tab ('\v').  In the
   C locale, isspace returns true only for the standard white-space
   characters.
   */
  switch( C )
  {
  case '\f':
  case '\n':
  case '\r':
  case '\t':
  case '\v':
  case ' ':
    return TRUE;
  }
  return FALSE;
}

int bwb_isupper( int C )
{
   /*
   4.3.1.10 The isupper function
   
   Synopsis
   
            #include <ctype.h>
            int isupper(int c);
   
   Description
   
      The isupper function tests for any upper-case letter or any of an
   implementation-defined set of characters for which none of iscntrl ,
   isdigit , ispunct , or isspace is true.  In the C locale, isupper
   returns true only for the characters defined as upper-case letters (as
   defined in $2.2.1).
   */
  switch( C )
  {
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
    return TRUE;
  }
  return FALSE;
}

int bwb_isxdigit( int C )
{
   /*
   4.3.1.11 The isxdigit function
   
   Synopsis
   
            #include <ctype.h>
            int isxdigit(int c);
   
   Description
   
      The isxdigit function tests for any hexadecimal-digit character (as
   defined in $3.1.3.2).
   */
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
  case '8':
  case '9':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
    return TRUE;
  }
  return FALSE;
}

int bwb_tolower( int C )
{
   /*
   4.3.2.1 The tolower function
   
   Synopsis
   
            #include <ctype.h>
            int tolower(int c);
   
   Description
   
      The tolower function converts an upper-case letter to the
   corresponding lower-case letter.
   
   Returns
   
      If the argument is an upper-case letter, the tolower function
   returns the corresponding lower-case letter if there is one; otherwise
   the argument is returned unchanged.  In the C locale, tolower maps
   only the characters for which isupper is true to the corresponding
   characters for which islower is true.
   */
  switch( C )
  {
  case 'A':
    return 'a';
  case 'B':
    return 'b';
  case 'C':
    return 'c';
  case 'D':
    return 'd';
  case 'E':
    return 'e';
  case 'F':
    return 'f';
  case 'G':
    return 'g';
  case 'H':
    return 'h';
  case 'I':
    return 'i';
  case 'J':
    return 'j';
  case 'K':
    return 'k';
  case 'L':
    return 'l';
  case 'M':
    return 'm';
  case 'N':
    return 'n';
  case 'O':
    return 'o';
  case 'P':
    return 'p';
  case 'Q':
    return 'q';
  case 'R':
    return 'r';
  case 'S':
    return 's';
  case 'T':
    return 't';
  case 'U':
    return 'u';
  case 'V':
    return 'v';
  case 'W':
    return 'w';
  case 'X':
    return 'x';
  case 'Y':
    return 'y';
  case 'Z':
    return 'z';
  }
  return C;
}

int bwb_toupper( int C )
{
   /*
   4.3.2.2 The toupper function
   
   Synopsis
   
            #include <ctype.h>
            int toupper(int c);
   
   Description
   
      The toupper function converts a lower-case letter to the corresponding upper-case letter.  
   
   Returns
   
      If the argument is a lower-case letter, the toupper function
   returns the corresponding upper-case letter if there is one; otherwise
   the argument is returned unchanged.  In the C locale, toupper maps
   only the characters for which islower is true to the corresponding
   characters for which isupper is true.
   */
  switch( C )
  {
  case 'a':
    return 'A';
  case 'b':
    return 'B';
  case 'c':
    return 'C';
  case 'd':
    return 'D';
  case 'e':
    return 'E';
  case 'f':
    return 'F';
  case 'g':
    return 'G';
  case 'h':
    return 'H';
  case 'i':
    return 'I';
  case 'j':
    return 'J';
  case 'k':
    return 'K';
  case 'l':
    return 'L';
  case 'm':
    return 'M';
  case 'n':
    return 'N';
  case 'o':
    return 'O';
  case 'p':
    return 'P';
  case 'q':
    return 'Q';
  case 'r':
    return 'R';
  case 's':
    return 'S';
  case 't':
    return 'T';
  case 'u':
    return 'U';
  case 'v':
    return 'V';
  case 'w':
    return 'W';
  case 'x':
    return 'X';
  case 'y':
    return 'Y';
  case 'z':
    return 'Z';
  }
  return C;
}

 
void *
bwb_memcpy(void *s1, const void *s2, size_t n)
{
   /*
   4.11.2.1 The memcpy function

   Synopsis
   
            #include <string.h>
            void *memcpy(void *s1, const void *s2, size_t n);
   
   Description
   
      The memcpy function copies n characters from the object pointed to
   by s2 into the object pointed to by s1 .  If copying takes place
   between objects that overlap, the behavior is undefined.
   
   Returns
   
      The memcpy function returns the value of s1 .  
   */
   if( n > 0 )
   {   
      char * Target;
      char * Source;
      int p;
      
      
      Target = (char *) s1;
      Source = (char *) s2;
      p = 0;
      while( p < n )
      {
         Target[p] = Source[p];
         p++;
      }
   }
   return s1;
}


void *
bwb_memmove(void *s1, const void *s2, size_t n)
{
   /*
   4.11.2.2 The memmove function
   
   Synopsis
   
            #include <string.h>
            void *memmove(void *s1, const void *s2, size_t n);
   
   Description
   
      The memmove function copies n characters from the object pointed to
   by s2 into the object pointed to by s1 .  Copying takes place as if
   the n characters from the object pointed to by s2 are first copied
   into a temporary array of n characters that does not overlap the
   objects pointed to by s1 and s2 , and then the n characters from the
   temporary array are copied into the object pointed to by s1 .
   
   Returns
   
      The memmove function returns the value of s1 .  
   */
   
   if( n > 0 )
   {   
      char * Target;
      char * Source;
      char * Temp;

      Target = (char *) s1;
      Source = (char *) s2;
      Temp   = (char *) malloc(n);
      if( Temp != NULL )
      {
         int p;

         p = 0;
         while( p < n )
         {
            Temp[p] = Source[p];
            p++;
         }
         p = 0;
         while( p < n )
         {
            Target[p] = Temp[p];
            p++;
         }
         free(Temp);
      }
   }
   return s1;
}


char *
bwb_strcpy(char *s1, const char *s2)
{
   /*
   4.11.2.3 The strcpy function
   
   Synopsis
   
            #include <string.h>
            char *strcpy(char *s1, const char *s2);
   
   Description
   
      The strcpy function copies the string pointed to by s2 (including
   the terminating null character) into the array pointed to by s1 .  If
   copying takes place between objects that overlap, the behavior is
   undefined.
   
   Returns
   
      The strcpy function returns the value of s1 .  
   */
   char C;
   int p;
   
   p = 0;
   do
   {
      C = s2[p];
      s1[p] = C;
      p++;
   }
   while( C != BasicNulChar );
   return s1;
}


char *
bwb_strncpy(char *s1, const char *s2, size_t n)
{
   /*
   4.11.2.4 The strncpy function
   
   Synopsis
   
            #include <string.h>
            char *strncpy(char *s1, const char *s2, size_t n);
   
   Description
   
      The strncpy function copies not more than n characters (characters
   that follow a null character are not copied) from the array pointed to
   by s2 to the array pointed to by s1 ./120/ If copying takes place
   between objects that overlap, the behavior is undefined.
   
      If the array pointed to by s2 is a string that is shorter than n
   characters, null characters are appended to the copy in the array
   pointed to by s1 , until n characters in all have been written.
   
   Returns
   
      The strncpy function returns the value of s1 .  
   */
   if( n > 0 )
   {
      char C;
      int p;
      
      p = 0;
      do
      {
         C = s2[p];
         s1[p] = C;
         p++;
      }
      while( C != BasicNulChar && p < n );
      while( p < n )
      {
         s1[p] = BasicNulChar;
         p++;
      }
   }   
   return s1;
}


char *
bwb_strcat(char *s1, const char *s2)
{
   /*
   4.11.3.1 The strcat function
   
   Synopsis
   
            #include <string.h>
            char *strcat(char *s1, const char *s2);
   
   Description
   
      The strcat function appends a copy of the string pointed to by s2
   (including the terminating null character) to the end of the string
   pointed to by s1 .  The initial character of s2 overwrites the null
   character at the end of s1 .  If copying takes place between objects
   that overlap, the behavior is undefined.
   
   Returns
   
      The strcat function returns the value of s1 .  
   */
   char * Temp;
   
   Temp = bwb_strchr( s1, BasicNulChar );
   bwb_strcpy( Temp, s2 );
   return s1;
}



char *
bwb_strncat(char *s1, const char *s2, size_t n)
{
   /*
   4.11.3.2 The strncat function
   
   Synopsis
   
            #include <string.h>
            char *strncat(char *s1, const char *s2, size_t n);
   
   Description
   
      The strncat function appends not more than n characters (a null
   character and characters that follow it are not appended) from the
   array pointed to by s2 to the end of the string pointed to by s1 .
   The initial character of s2 overwrites the null character at the end
   of s1 .  A terminating null character is always appended to the
   result./121/ If copying takes place between objects that overlap, the
   behavior is undefined.
   
   Returns
   
      The strncat function returns the value of s1 .  
   */
   char * Temp;
   
   Temp = bwb_strchr( s1, BasicNulChar );
   bwb_strncpy( Temp, s2, n );
   return s1;
}


int 
bwb_memcmp(const void *s1, const void *s2, size_t n)
{
   /*
   4.11.4.1 The memcmp function
   
   Synopsis
   
            #include <string.h>
            int memcmp(const void *s1, const void *s2, size_t n);
   
   Description
   
      The memcmp function compares the first n characters of the object
   pointed to by s1 to the first n characters of the object pointed to by
   s2 ./122/
   
   Returns
   
      The memcmp function returns an integer greater than, equal to, or
   less than zero, according as the object pointed to by s1 is greater
   than, equal to, or less than the object pointed to by s2 .
   */
   if( n > 0 )
   {
      int p;
      char * L;
      char * R;
      
      p = 0;
      L = (char *) s1;
      R = (char *) s2;
      while( p < n )
      {
         if( L[p] > R[p] )
         {
            return 1;
         }
         if( L[p] < R[p] )
         {
            return -1;
         }
         /* L[p] == R[p] */
         p++;
      }
   }
   return 0;
}


int 
bwb_strcmp(const char *s1, const char *s2)
{
   /*
   4.11.4.2 The strcmp function
   
   Synopsis
   
            #include <string.h>
            int strcmp(const char *s1, const char *s2);
   
   Description
   
      The strcmp function compares the string pointed to by s1 to the
   string pointed to by s2 .
   
   Returns
   
      The strcmp function returns an integer greater than, equal to, or
   less than zero, according as the string pointed to by s1 is greater
   than, equal to, or less than the string pointed to by s2 .
   */
   char C;
   int p;
   
   p = 0;
   do
   {
      if( s1[p] > s2[p] )
      {
         return 1;
      }
      if( s1[p] < s2[p] )
      {
         return -1;
      }
      /* s1[p] == s2[p] */
      C = s1[p];
      p++;
   }
   while( C != BasicNulChar );
   return 0;
}


int 
bwb_strncmp(const char *s1, const char *s2, size_t n)
{
   /*
   4.11.4.4 The strncmp function
   
   Synopsis
   
            #include <string.h>
            int strncmp(const char *s1, const char *s2, size_t n);
   
   Description
   
      The strncmp function compares not more than n characters
   (characters that follow a null character are not compared) from the
   array pointed to by s1 to the array pointed to by s2 .
   
   Returns
   
      The strncmp function returns an integer greater than, equal to, or
   less than zero, according as the possibly null-terminated array
   pointed to by s1 is greater than, equal to, or less than the possibly
   null-terminated array pointed to by s2 .
   */
   if( n > 0 )
   {
      char C;
      int p;
      
      p = 0;
      do
      {
         if( s1[p] > s2[p] )
         {
            return 1;
         }
         if( s1[p] < s2[p] )
         {
            return -1;
         }
         /* s1[p] == s2[p] */
         C = s1[p];
         p++;
      }
      while( C != BasicNulChar && p < n );
   }
   return 0;
}


void *
bwb_memchr(const void *s, int c, size_t n)
{
   /*
   4.11.5.1 The memchr function
   
   Synopsis
   
            #include <string.h>
            void *memchr(const void *s, int c, size_t n);
   
   Description
   
      The memchr function locates the first occurrence of c (converted to
   an unsigned char ) in the initial n characters (each interpreted as
   unsigned char ) of the object pointed to by s .
   
   Returns
   
      The memchr function returns a pointer to the located character, or
   a null pointer if the character does not occur in the object.
   */
   if( n > 0 )
   {
      int p;
      unsigned char *Check;
      unsigned char Find;
      
      p = 0;
      Check = (unsigned char *)s;
      Find = (unsigned char) c;
      do
      {
         if( Check[p] == Find )
         {
            return (void *) &(Check[p]);
         }
         p++;
      }
      while( p < n );
   }
   return NULL;
}


char *
bwb_strchr(const char *s, int c)
{
   /*
   4.11.5.2 The strchr function
   
   Synopsis
   
            #include <string.h>
            char *strchr(const char *s, int c);
   
   Description
   
      The strchr function locates the first occurrence of c (converted to
   a char ) in the string pointed to by s .  The terminating null
   character is considered to be part of the string.
   
   Returns
   
      The strchr function returns a pointer to the located character, or
   a null pointer if the character does not occur in the string.
   */
   int p;
   char Find;
   char C;

   p = 0;   
   Find = (char) c;
   do
   {
      C = s[p];
      if( C == Find )
      {
         return (char *) &(s[p]);
      }
      p++;
   }
   while( C != BasicNulChar );
   return NULL;
}


char *
bwb_strrchr(const char *s, int c)
{
   /*
   4.11.5.5 The strrchr function
   
   Synopsis
   
            #include <string.h>
            char *strrchr(const char *s, int c);
   
   Description
   
      The strrchr function locates the last occurrence of c (converted to
   a char ) in the string pointed to by s .  The terminating null
   character is considered to be part of the string.
   
   Returns
   
      The strrchr function returns a pointer to the character, or a null
   pointer if c does not occur in the string.
   */
   int p;
   char Find;
   char * Found;
   char C;

   p = 0;   
   Find = (char) c;
   Found = NULL;
   do
   {
      C = s[p];
      if( C == Find )
      {
         Found = (char *) &(s[p]);
      }
      p++;
   }
   while( C != BasicNulChar );
   return Found;
}


void *
bwb_memset(void *s, int c, size_t n)
{
   /*
   4.11.6.1 The memset function
   
   Synopsis
   
            #include <string.h>
            void *memset(void *s, int c, size_t n);
   
   Description
   
      The memset function copies the value of c (converted to an unsigned
   char ) into each of the first n characters of the object pointed to by
   s .
   
   Returns
   
      The memset function returns the value of s .  
   */
   if( n > 0 )
   {
      int p;
      unsigned char * Target;
      unsigned char Value;
      
      p = 0;
      Target = (unsigned char *)s;
      Value = (unsigned char) c;
      do
      {
         Target[p] = Value;
         p++;
      }
      while( p < n );
   }
   return s;
}



size_t 
bwb_strlen(const char *s)
{
   /*
   4.11.6.3 The strlen function
   
   Synopsis
   
            #include <string.h>
            size_t strlen(const char *s);
   
   Description
   
      The strlen function computes the length of the string pointed to by s .  
   
   Returns
   
      The strlen function returns the number of characters that precede
   the terminating null character.
   */
   size_t p;
   p = 0;
   while( s[p] != BasicNulChar )
   {
      p++;
   }
   return p;
}


#if HAVE_UNIX_GCC

/* these are intrinsic C functions in my environment using -ansi */

#else          /* ! HAVE_UNIX_GCC */

unsigned int
sleep( unsigned int X )
{
   return X;
}

#endif            /* ! HAVE_UNIX_GCC */


double
bwb_rint(double x)
{
   /* BASIC dialects have different rounding rules */
   double Result;
   if( x < 0 )
   {
      return - bwb_rint(-x);
   }
   /* x >= 0 */
   switch( My->OptionRoundType )
   {
   case C_OPTION_ROUND_BANK:
      /* normal financial rounding */
      Result = floor(x + 0.5);
      if( x - floor( x ) == 0.5 )
      {
         /* midway */
         double Half;
         Half = Result / 2.0;
         if( Half != floor( Half ) )
         {
            /* odd -> even */
            Result--;
         }
      }
      break;
   case C_OPTION_ROUND_MATH:
      /* normal mathematical rounding */
      Result = floor(x + 0.5);
      break;
   case C_OPTION_ROUND_TRUNCATE:
      /* simple truncation */
      Result = floor(x);
      break;
   }
   return Result;
}

int
bwb_stricmp(const char *s1, const char *s2)
{
   const unsigned char *p1;
   const unsigned char *p2;

   p1 = (const unsigned char *) s1;
   p2 = (const unsigned char *) s2;
   while (*p1 != BasicNulChar)
   {
      char            c1;
      char            c2;
      c1 = bwb_toupper(*p1);
      c2 = bwb_toupper(*p2);
      if (c1 < c2)
      {
         return -1;
      }
      if (c1 > c2)
      {
         return 1;
      }
      p1++;
      p2++;
   }
   if (*p2 == BasicNulChar)
   {
      return 0;
   }
   return -1;
}

int
bwb_strnicmp(const char *s1, const char *s2, size_t n)
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
      c1 = bwb_toupper(p1[x]);
      c2 = bwb_toupper(p2[x]);
      if (c1 < c2)
      {
         return -1;
      }
      if (c1 > c2)
      {
         return 1;
      }
      if (c1 == BasicNulChar)
      {
         return 0;
      }
      x++;
   }
   return 0;
}


/* EOF */

