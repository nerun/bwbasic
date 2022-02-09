/***************************************************************
  
        bwb_var.c       Variable-Handling Routines
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


/* Prototypes for functions visible to this file only */

static int dim_check(VariableType * v);
static int var_defx(LineType * l, int type);
static size_t dim_unit(VariableType * v, int *pp);
static VariableType * mat_islocal(char *buffer);
static VariableType * var_islocal(char *buffer, int dimensions);
static LineType * dio_lrset(LineType * l, int rset);

#if NEW_VIRTUAL
static VirtualType * new_virtual( void );
static void clear_virtual( VirtualType * Z );
static void clear_virtual_by_variable( VariableType * Variable );
static VirtualType * find_virtual_by_variable( VariableType * Variable );
#endif /* NEW_VIRTUAL */


/***************************************************************
  
        FUNCTION:       var_init()
  
        DESCRIPTION: This function initializes the internal
         linked list of variables.
  
***************************************************************/

int
var_init( void )
{
   bwx_DEBUG(__FUNCTION__);

   My->var_head = NULL;

   return TRUE;
}




/***************************************************************
  
        FUNCTION:       bwb_common()
  
        DESCRIPTION: This C function implements the BASIC
            COMMON command.
  
   SYNTAX:     COMMON variable [, variable...]
  
***************************************************************/

LineType *
bwb_COMMON(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   do
   {
      int dimensions = 0;
      VariableType * variable;
      char            varname[BasicNameLengthMax + 1];

      /* get variable name and find variable */
      if( line_read_varname(l, varname) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( line_skip_char(l,BasicLeftParenChar) )
      {
         line_skip_spaces( l );
         if( bwb_isdigit( l->buffer[ l->position ] ) )
         {
            /* COMMON A(3) : DIM A( 5, 10, 20 ) */
            if( line_read_integer_expression( l, &dimensions ) == FALSE )
            {
               WARN_SYNTAX_ERROR;
               return bwb_zline(l);
            }
         }
         else
         {
            /* COMMON A(,,) : DIM A( 5, 10, 20 ) */
            dimensions++;
            while( line_skip_comma(l) );
            {
               dimensions++;
            }
         }
         if( line_skip_char(l,BasicRightParenChar) == FALSE)
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }
      if ((variable = var_find(varname,dimensions,TRUE)) == NULL)
      {
          WARN_VARIABLE_NOT_DECLARED;
          return bwb_zline(l);
      }
      
      /* mark as COMMON */
      variable->VariableFlags |= VARIABLE_COMMON;
   }
   while( line_skip_comma(l) );

   return bwb_zline(l);
}

/***********************************************************
  
        FUNCTION: bwb_erase()
  
   DESCRIPTION:    This C function implements the BASIC
         ERASE command.
  
   SYNTAX:     ERASE variable[, variable]...
  
***********************************************************/

LineType *
bwb_ERASE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   do
   {
      char            varname[BasicNameLengthMax + 1];

      /* get variable name and find variable */

      if( line_read_varname(l, varname) )
      {
         /* erase all matching SCALAR and ARRAY variables */
         int dimensions = 0;

         for( dimensions = 0; dimensions < MAX_DIMS; dimensions++ )
         {
            VariableType * variable;
   
            variable = var_find(varname,dimensions,FALSE);
            if( variable != NULL  )
            {
               /* found a variable */
               VariableType *p; /* previous variable in linked list */
   
               /* find then previous variable in chain */
               if( variable == My->var_head )
               {
                  /* free head */
                  My->var_head = variable->next;
                  variable->next = NULL;
                  var_free( variable );
               }
               else
               {
                  /* free tail */
                  for (p = My->var_head; p != NULL && p->next != variable; p = p->next)
                  {
                     ;
                  }
                  if(p == NULL)
                  {
                     /* this should never happen */
                     WARN_INTERNAL_ERROR;
                     return NULL;
                  }
                  if(p->next != variable)
                  {
                     /* this should never happen */
                     WARN_INTERNAL_ERROR;
                     return NULL;
                  }
                  /* reassign linkage */
                  p->next = variable->next;
                  variable->next = NULL;
                  var_free( variable );
               }
            }
         }
      }
   }
   while( line_skip_comma(l) );

   return bwb_zline(l);

}

/***********************************************************
  
        FUNCTION: bwb_swap()
  
   DESCRIPTION:    This C function implements the BASIC
         SWAP command.
  
   SYNTAX:     SWAP variable, variable
  
***********************************************************/
LineType *
bwb_EXCHANGE(LineType * l)
{
   return bwb_SWAP(l);
}



LineType *
bwb_SWAP(LineType * l)
{
   /* SWAP lhs, rhs */
   VariableType *lhs;
   VariableType *rhs;

   bwx_DEBUG(__FUNCTION__);



   if( line_skip_char(l, BasicLeftParenChar) == FALSE )
   {
      /* optional */
   }

   /* get left variable */
   if( (lhs = line_read_scalar( l )) == NULL )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   /* get required comma */
   if ( line_skip_comma(l) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }

   /* get right variable */
   if( (rhs = line_read_scalar( l )) == NULL )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   
   if( line_skip_char(l, BasicRightParenChar) == FALSE )
   {
      /* optional */
   }

   /* check to be sure that both variables are of the same type */
   if( VAR_IS_STRING( rhs ) != VAR_IS_STRING( lhs ) )
   {
      WARN_TYPE_MISMATCH;
      return bwb_zline(l);
   }

   /* copy lhs to temp, then rhs to lhs, then temp to rhs */
   /* if( TRUE ) */
   {
      VariantType L;
      VariantType R;

      if( var_get( lhs, &L ) == FALSE ) { WARN_SYNTAX_ERROR; return bwb_zline( l ); }
      if( var_get( rhs, &R ) == FALSE ) { WARN_SYNTAX_ERROR; return bwb_zline( l ); }

      if( var_set( lhs, &R ) == FALSE ) { WARN_SYNTAX_ERROR; return bwb_zline( l ); }
      if( var_set( rhs, &L ) == FALSE ) { WARN_SYNTAX_ERROR; return bwb_zline( l ); }
   }
   /* return */
   return bwb_zline(l);
}



VariableType *
var_free(VariableType * variable)
{
   /* 
   Release all the memory associated with a specific variable.
   This function returns NULL, so you can use it like this:
   variable = var_new(...);
   ...
   variable = var_free( variable ); 
   */
   bwx_DEBUG(__FUNCTION__);

   if (variable != NULL)
   {
      if (variable->next != NULL)
      {
         /* This allows variable chains to be easily released. */
         variable->next = var_free(variable->next);
      }

      /* cleanup this variable */
      field_free_variable( variable );
#if NEW_VIRTUAL
      clear_virtual_by_variable( variable );
#endif /* NEW_VIRTUAL */
      if (variable->memnum != NULL)
      {
         FREE(variable->memnum, "var_free");
         variable->memnum = NULL;
      }
      if (variable->memstr != NULL)
      {
         int             j;
         for (j = 0; j < variable->array_units; j++)
         {
            if (variable->memstr[j].sbuffer != NULL)
            {
               FREE(variable->memstr[j].sbuffer, "var_free");
               variable->memstr[j].sbuffer = NULL;
            }
            variable->memstr[j].length = 0;
         }
         FREE(variable->memstr, "var_free");
         variable->memstr = NULL;
      }
      FREE(variable, "var_free");
   }
   return NULL;
}



/***********************************************************
  
        FUNCTION: bwb_clear()
  
   DESCRIPTION:    This C function implements the BASIC
         CLEAR command.
  
   SYNTAX:     CLEAR
  
***********************************************************/

void
var_CLEAR(void)
{
   VariableType * variable;

   bwx_DEBUG(__FUNCTION__);

   for (variable = My->var_head; variable != NULL; )
   {
      if( variable->VariableFlags & VARIABLE_PRESET )
      {
         /* keep */
         variable = variable ->next;
      }
      else
      if( variable == My->var_head )
      {
         /* free head */
         My->var_head = variable->next;
         variable->next = NULL;
         var_free( variable );
         variable = My->var_head;
      }
      else
      {
         /* free tail */
         VariableType *z;
         z = variable->next;
         variable->next = NULL;
         var_free( variable );
         variable = z;
      }
   }
}

LineType *
bwb_CLEAR(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   var_CLEAR();

   /* ignoare all parameters */
   line_skip_eol(l);

   return bwb_zline(l);
}


LineType *
bwb_CLR(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   return bwb_CLEAR(l);
}

/***********************************************************
  
   FUNCTION:       var_delcvars()
  
   DESCRIPTION:    This function deletes all variables
         in memory except those previously marked
         as common.
  
***********************************************************/

int
var_delcvars(void)
{
   VariableType *v;

   bwx_DEBUG(__FUNCTION__);

   for (v = My->var_head; v != NULL; )
   {
      if( v->VariableFlags & VARIABLE_PRESET )
      {
         /* keep */
         v = v ->next;
      }
      else
      if( v->VariableFlags & VARIABLE_COMMON )
      {
         /* keep */
         v = v->next;
      }
      else
      if( v == My->var_head )
      {
         /* free head */
         My->var_head = v->next;
         v->next = NULL;
         var_free( v );
         v = My->var_head;
      }
      else
      {
         /* free tail */
         VariableType *z; /* next variable */

         z = v->next;
         v->next = NULL;
         var_free( v );
         v = z;
      }
   }
   return TRUE;
}

/***********************************************************
  
        FUNCTION: bwb_mid()
  
   DESCRIPTION:    This function implements the BASIC
         MID$ command.
  
         Same as MID$ function, except it will set
         the desired substring and not return its
         value.  Added by JBV 10/95
  
   SYNTAX:     MID$( string-variable$, start-position-in-string
         [, number-of-spaces ] ) = expression
  
***********************************************************/

LineType *
bwb_MID4(LineType * l)
{
   /* MID$( target$, start% [ , length% ] ) = source$ */


   VariantType       target;
   int               start;
   int               length;
   VariantType       source;
   VariableType     *variable;
   int               maxlen;
   bwx_DEBUG(__FUNCTION__);
   
   if( line_skip_char(l, BasicLeftParenChar) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( (variable = line_read_scalar( l )) == NULL )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if ( VAR_IS_STRING( variable ) )
   {
      /* OK */
   }
   else
   {
      /* ERROR */
      WARN_TYPE_MISMATCH;
      return bwb_zline(l);
   }
   if( var_get( variable, &target ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( target.TypeChar != '$' )
   {
      WARN_TYPE_MISMATCH;
      return bwb_zline(l);
   }
   if( line_skip_comma(l) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_integer_expression(l, &start) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( start < 1 )
   {
      WARN_ILLEGAL_FUNCTION_CALL;
      return bwb_zline(l);
   }
   if( start > target.Length )
   {
      WARN_ILLEGAL_FUNCTION_CALL;
      return bwb_zline(l);
   }
   maxlen = 1 + target.Length - start;
   if( line_skip_comma(l) )
   {
      line_skip_spaces(l);
      if( line_read_integer_expression(l, &length) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( length < 0 )
      {
         WARN_ILLEGAL_FUNCTION_CALL;
         return bwb_zline(l);
      }
   }
   else
   {
      length = -1; /* MAGIC */
   }
   if( line_skip_char(l, BasicRightParenChar) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_skip_char(l, '=') == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( line_read_expression( l, &source ) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( source.TypeChar != '$' )
   {
      WARN_TYPE_MISMATCH;
      return bwb_zline(l);
   }
   if( length == -1 /* MAGIC */ )
   {
      length = source.Length;
   }
   length = MIN( length, maxlen );
   length = MIN( length, source.Length );
   if( length < 0 )
   {
      WARN_INTERNAL_ERROR;
      return bwb_zline(l);
   }
   if( length > 0 )
   {
      int i;
      
      start--; /* BASIC to C */
      for( i =0; i < length; i++ )
      {
         target.Buffer[ start + i ] = source.Buffer[ i ];
      }
      target.Buffer[ target.Length ] = BasicNulChar;
      if( var_set( variable, &target ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
   }
   RELEASE( (&source) );
   RELEASE( (&target) );
   return bwb_zline(l);
}


/***********************************************************
  
        FUNCTION: bwb_ddbl()
  
   DESCRIPTION:    This function implements the BASIC
         DEFDBL command.
  
   SYNTAX:     DEFDBL letter[-letter](, letter[-letter])...
  
***********************************************************/

LineType *
bwb_DEFBYT(LineType * l)
{
   /*
   DEFBYT letter[-letter](, letter[-letter])...
   */

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with BYTE set */

   var_defx(l, BasicByteSuffix);

   return bwb_zline(l);

}

LineType *
bwb_DEFCUR(LineType * l)
{
   /*
   DEFCUR letter[-letter](, letter[-letter])...
   */

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with CURRENCY set */

   var_defx(l, BasicCurrencySuffix);

   return bwb_zline(l);

}

LineType *
bwb_DEFDBL(LineType * l)
{
   /*
   DEFDBL letter[-letter](, letter[-letter])...
   */

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with DOUBLE set */

   var_defx(l, BasicDoubleSuffix);

   return bwb_zline(l);

}

/***********************************************************
  
        FUNCTION: bwb_dint()
  
   DESCRIPTION:    This function implements the BASIC
         DEFINT command.
  
   SYNTAX:     DEFINT letter[-letter](, letter[-letter])...
  
***********************************************************/

LineType *
bwb_DEFINT(LineType * l)
{
   /*
   DEFINT letter[-letter](, letter[-letter])...
   */

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with INTEGER set */

   var_defx(l, BasicIntegerSuffix);

   return bwb_zline(l);

}

LineType *
bwb_DEFLNG(LineType * l)
{
   /*
   DEFLNG letter[-letter](, letter[-letter])...
   */

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with LONG set */

   var_defx(l, BasicLongSuffix);

   return bwb_zline(l);

}

/***********************************************************
  
        FUNCTION: bwb_dsng()
  
   DESCRIPTION:    This function implements the BASIC
         DEFSNG command.
  
   SYNTAX:     DEFSNG letter[-letter](, letter[-letter])...
  
***********************************************************/

LineType *
bwb_DEFSNG(LineType * l)
{
   /*
   DEFSNG letter[-letter](, letter[-letter])...
   */

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with SINGLE set */

   var_defx(l, BasicSingleSuffix);

   return bwb_zline(l);

}

/***********************************************************
  
        FUNCTION: bwb_dstr()
  
   DESCRIPTION:    This function implements the BASIC
         DEFSTR command.
  
   SYNTAX:     DEFSTR letter[-letter](, letter[-letter])...
  
***********************************************************/

LineType *
bwb_DEFSTR(LineType * l)
{
   /*
   DEFSTR letter[-letter](, letter[-letter])...
   */

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with STRING set */

   var_defx(l, BasicStringSuffix);

   return bwb_zline(l);

}

LineType *
bwb_TEXT(LineType * l)
{
   /*
   TEXT letter[-letter](, letter[-letter])...
   */

   bwx_DEBUG(__FUNCTION__);

   /* call generalized DEF handler with STRING set */

   var_defx(l, BasicStringSuffix);

   return bwb_zline(l);

}

LineType *
bwb_TRACE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   prn_xprintf("Trace is ON\n");
   My->bwb_trace = TRUE;

   return bwb_zline(l);
}

LineType *
bwb_TRACE_ON(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   prn_xprintf("Trace is ON\n");
   My->bwb_trace = TRUE;

   return bwb_zline(l);
}

LineType *
bwb_TRACE_OFF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);

   prn_xprintf("Trace is OFF\n");
   My->bwb_trace = FALSE;

   return bwb_zline(l);
}

int
VarTypeIndex(char C)
{
  bwx_DEBUG(__FUNCTION__);
  switch( C )
  {
  case 'A': return 0;
  case 'B': return 1;
  case 'C': return 2;
  case 'D': return 3;
  case 'E': return 4;
  case 'F': return 5;
  case 'G': return 6;
  case 'H': return 7;
  case 'I': return 8;
  case 'J': return 9;
  case 'K': return 10;
  case 'L': return 11;
  case 'M': return 12;
  case 'N': return 13;
  case 'O': return 14;
  case 'P': return 15;
  case 'Q': return 16;
  case 'R': return 17;
  case 'S': return 18;
  case 'T': return 19;
  case 'U': return 20;
  case 'V': return 21;
  case 'W': return 22;
  case 'X': return 23;
  case 'Y': return 24;
  case 'Z': return 25;
  case 'a': return 0;
  case 'b': return 1;
  case 'c': return 2;
  case 'd': return 3;
  case 'e': return 4;
  case 'f': return 5;
  case 'g': return 6;
  case 'h': return 7;
  case 'i': return 8;
  case 'j': return 9;
  case 'k': return 10;
  case 'l': return 11;
  case 'm': return 12;
  case 'n': return 13;
  case 'o': return 14;
  case 'p': return 15;
  case 'q': return 16;
  case 'r': return 17;
  case 's': return 18;
  case 't': return 19;
  case 'u': return 20;
  case 'v': return 21;
  case 'w': return 22;
  case 'x': return 23;
  case 'y': return 24;
  case 'z': return 25;
  }
  WARN_INTERNAL_ERROR;
  return 0;
}

/***********************************************************
  
        Function: var_defx()
  
   DESCRIPTION:    This function is a generalized DEFxxx handler.
  
***********************************************************/

static int var_defx(LineType * l, int type)
{
   /*
   DEFxxx letter[-letter](, letter[-letter])...
   */

   bwx_DEBUG(__FUNCTION__);

   do
   {
      char firstc;
      char lastc;
      int      first;
      int       last;  
      int    c;

      /* find a sequence of letters for variables */
      if ( line_read_letter_sequence(l, &firstc, &lastc) == FALSE )
      {
         /* DEFINT 0-9 */
         WARN_SYNTAX_ERROR;
         return FALSE;
      }
      first = VarTypeIndex(firstc);
      last = VarTypeIndex(lastc);
      if( first > last )
      {
         /* DEFINT Z-A */
         WARN_SYNTAX_ERROR;
         return FALSE;
      }
      for (c = first; c <= last; c++)
      {
         My->DefaultVariableType[c] = type;
      }
   }
   while( line_skip_comma(l) );

   return TRUE;

}


char
var_nametype( char * name )
{
   char * C;
   
   C = name;
   while( *C )
   {
      if( char_is_vartype( *C ) )
      {
         return *C;
      }
      C++;
   }
   return BasicNulChar;
}


/***************************************************************
  
        FUNCTION:       var_find()
  
   DESCRIPTION:    This C function attempts to find a variable
         name matching the argument in buffer. If
         it fails to find a matching name, it
         sets up a new variable with that name.
  
***************************************************************/

VariableType *
mat_find(char *name)
{
   /*
   similar to var_find, but returns the first matrix found
   */
   VariableType *v;
   bwx_DEBUG(__FUNCTION__);

   /* check for NULL variable name */
   if (name == NULL)
   {
      WARN_INTERNAL_ERROR;
      return NULL;
   }
   if (name[0] == BasicNulChar)
   {
      WARN_SYNTAX_ERROR;
      return NULL;
   }
   /* check for a local variable at this EXEC level */

   v = mat_islocal(name);
   if (v != NULL)
   {
      return v;
   }
   /* now run through the global variable list and try to find a match */
   for (v = My->var_head; v != NULL; v = v->next)
   {
      if( v->dimensions > 0 )
      {
         if (bwb_stricmp(v->name, name) == 0)
         {
            return v;
         }
      }
   }
   return NULL;
}

VariableType *
var_find(char *name, int dimensions, int IsImplicit)
{
   VariableType *v;
   int n;

   bwx_DEBUG(__FUNCTION__);

   /* check for NULL variable name */
   if (name == NULL)
   {
      WARN_INTERNAL_ERROR;
      return NULL;
   }
   if (name[0] == BasicNulChar)
   {
      WARN_SYNTAX_ERROR;
      return NULL;
   }
   if( dimensions < 0 )
   {
      WARN_INTERNAL_ERROR;
      return NULL;
   }
   
   /* check for a local variable at this EXEC level */

   v = var_islocal(name, dimensions);
   if (v != NULL)
   {
      return v;
   }
   /* now run through the global variable list and try to find a match */
   for (v = My->var_head; v != NULL; v = v->next)
   {
      if( v->dimensions == dimensions )
      {
         if (bwb_stricmp(v->name, name) == 0)
         {
            return v;
         }
      }
   }
   if( IsImplicit == FALSE )
   {
      return NULL;
   }
   if( My->CurrentVersion->OptionFlags & OPTION_EXPLICIT_ON )
   {
       /* NO implicit creation - all variables must be created via DIM */
       WARN_VARIABLE_NOT_DECLARED;
       return NULL;
   }
   if (My->CurrentVersion->OptionFlags & OPTION_STRICT_ON)
   {
      if( dimensions > 0 )
      {
         /* Implicit ARRAY is not allowed */
         WARN_VARIABLE_NOT_DECLARED;
         return NULL;
      }
   }

   /* this is a IMPLICIT variable, so initialize it... */

   /* initialize new variable */
   /* Revised to CALLOC pass-thru call by JBV */
   v = (VariableType *) CALLOC(1, sizeof(VariableType), "var_find");
   if( v == NULL )
   {
      WARN_OUT_OF_MEMORY;
      return NULL;
   }

   /* copy the name into the appropriate structure */

   bwb_strcpy(v->name, name);
   
   /* determine variable type */
   v->VariableTypeChar = var_nametype( name );
   if( v->VariableTypeChar == BasicNulChar )
   {
      /* variable name has no declared type */
      v->VariableTypeChar = My->DefaultVariableType[VarTypeIndex(name[0])];
   }
   v->VariableFlags = 0;
   v->dimensions = dimensions;
   v->array_units = 1;
   for (n = 0; n < v->dimensions; n++)
   {
            v->UBOUND[n] = 10;
            v->LBOUND[n] = My->CurrentVersion->OptionBaseValue;
            if( v->UBOUND[n] < v->LBOUND[n] )
            {
               WARN_SUBSCRIPT_OUT_OF_RANGE;
               return NULL;
            }
            v->array_pos[n] = v->LBOUND[n];
            v->array_units *= v->UBOUND[n] - v->LBOUND[n] + 1;
   }

   /* assign array memory */
   if( VAR_IS_STRING( v ) )
   {
      if ((v->memstr = (StringType *)
           CALLOC(v->array_units, sizeof(StringType), "var_find")) == NULL)
      {
         WARN_OUT_OF_MEMORY;
         return NULL;
      }
   }
   else
   {
      /* Revised to CALLOC pass-thru call by JBV */
      if ((v->memnum = (BasicNumberType *)
           CALLOC(v->array_units, sizeof(BasicNumberType), "var_find")) == NULL)
      {
         WARN_OUT_OF_MEMORY;
         return NULL;
      }
   }

   /* insert variable at the beginning of the variable chain */
   v->next = My->var_head;
   My->var_head = v;
   return v;
}

/***************************************************************
  
        FUNCTION:       var_new()
  
   DESCRIPTION:    This function assigns memory for a new variable.
  
***************************************************************/

VariableType *
var_new(char *name, char TypeChar)
{
   VariableType *v;


   bwx_DEBUG(__FUNCTION__);
   /* get memory for new variable */

   if (name == NULL)
   {
      WARN_INTERNAL_ERROR;
      return NULL;
   }
   if (name[0] == BasicNulChar)
   {
      WARN_SYNTAX_ERROR;
      return NULL;
   }
   /* Revised to CALLOC pass-thru call by JBV */
   if ((v = (VariableType *) CALLOC(1, sizeof(VariableType), "var_new"))
       == NULL)
   {
      WARN_OUT_OF_MEMORY;
      return NULL;
   }
   /* copy the name into the appropriate structure */

   bwb_strcpy(v->name, name);

   /* set memory in the new variable */
   var_make(v, TypeChar);

   /* and return */

   return v;

}


/***************************************************************
  
   FUNCTION:   bwb_dim()
  
   DESCRIPTION:   This function implements the BASIC DIM
         statement, allocating memory for a
         dimensioned array of variables.
  
   SYNTAX:     DIM variable(elements...)[,variable(elements...)]
  
***************************************************************/

static void var_link_new_variable( VariableType *v )
{
   /* 
   We are called by DIM, so this is an explicitly created variable.
   There are only two possibilities:
   1.  We are a LOCAL variable of a SUB or FUNCTION.
   2.  We are a GLOBAL variable.
   */
   if ( My->stack_head != NULL )
   {
      StackType * stack_item;
      for (stack_item = My->stack_head; stack_item != NULL; stack_item = stack_item->next)
      {
         if (stack_item->LoopTopLine != NULL)
         {
            switch (stack_item->LoopTopLine->cmdnum)
            {
            case C_FUNCTION:
            case C_SUB:
               /* we have found a FUNCTION or SUB boundary, must be LOCAL */
               v->next = stack_item->local_variable;
               stack_item->local_variable = v;
               return;
               /* break; */
            }
         }
      }
   }
   /* no FUNCTION or SUB on the stack, must be GLOBAL */
   v->next = My->var_head;
   My->var_head = v;
}


#if NEW_VIRTUAL
static VirtualType * new_virtual( void )
{
   VirtualType * Z;

   /* look for an empty slot */
   for( Z = My->virtual_head; Z != NULL; Z = Z->next )
   {
      if( Z->Variable == NULL )
      {
         /* FOUND */
         return Z;
      }
   }
   /* NOT FOUND */
   Z = calloc( 1, sizeof( VirtualType ) );
   if( Z == NULL )
   {
      WARN_OUT_OF_MEMORY;
   }
   else
   {
      Z->next = My->virtual_head;
      My->virtual_head = Z;
   }   
   return Z;
}
static void clear_virtual( VirtualType * Z )
{
   Z->Variable = NULL;
   Z->FileNumber = 0;
   Z->FileOffset = 0;
   Z->FileLength = 0;
}
static void clear_virtual_by_variable( VariableType * Variable )
{
   VirtualType * Z;

   for( Z = My->virtual_head; Z != NULL; Z = Z->next )
   {
      if( Z->Variable == Variable )
      {
         /* FOUND */
         clear_virtual( Z );
      }
   }
}
extern void clear_virtual_by_file( int FileNumber )
{
   /* called by file_clear() */
   VirtualType * Z;

   for( Z = My->virtual_head; Z != NULL; Z = Z->next )
   {
      if( Z->FileNumber == FileNumber )
      {
         /* FOUND */
         clear_virtual( Z );
      }
   }
}
static VirtualType * find_virtual_by_variable( VariableType * Variable )
{
   VirtualType * Z;

   for( Z = My->virtual_head; Z != NULL; Z = Z->next )
   {
      if( Z->Variable == Variable )
      {
         /* FOUND */
         return Z;
      }
   }
   /* NOT FOUND */
   return NULL;
}
#endif /* NEW_VIRTUAL */

LineType *
bwb_DIM(LineType * l)
{
#if NEW_VIRTUAL
   int            FileNumber = 0;  /* the file might not be OPEN when the variable is declared */
   size_t         FileOffset = 0;  /* from beginning of file */
   int            FileLength = 0;  /* sizeof( BasicNumberType ) or Fixed String Length */
#endif /* NEW_VIRTUAL */

   bwx_DEBUG(__FUNCTION__);

#if NEW_VIRTUAL
   if( line_skip_char( l, BasicFileNumberPrefix ) )
   {
      /* DIM # filenum , ... */
      if( line_read_integer_expression( l, &FileNumber ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( FileNumber <= 0 )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      if( line_skip_comma( l ) == FALSE )
      {
         WARN_BAD_FILE_NUMBER;
         return bwb_zline(l);
      }
      FileOffset = 0;
      FileLength = 0;
   }
#endif /* NEW_VIRTUAL */

   do
   {
      VariableType *v;
      register int    n;
      int      n_params;  /* number of parameters */
      int       pp[ MAX_DIMS ];       
      char            TypeChar;
      char            varname[BasicNameLengthMax + 1];


      /* Get variable name */
      if( line_read_varname(l, varname) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      
      /* read parameters */
      n_params = 0;
      if( line_peek_char(l, BasicLeftParenChar) )
      {
         if( line_read_array_dimensions( l, &n_params, pp ) == FALSE )
         {
            WARN_SYNTAX_ERROR;
            return bwb_zline(l);
         }
      }

      /* determine variable type */
      TypeChar = var_nametype( varname );
      if( TypeChar == BasicNulChar )
      {
         /* variable has no explicit type char */
         TypeChar = line_read_type_declaration( l );
         if( TypeChar == BasicNulChar )
         {
            /* variable has no declared type */
            TypeChar = My->DefaultVariableType[VarTypeIndex(varname[0])];
         }
      }

#if NEW_VIRTUAL
      if( TypeChar == BasicStringSuffix )
      {
         /* DIM # file_num , var_name AS STRING * fixed_length */

         FileLength = 16; /* default */
         if( line_skip_char( l, '*' ) || line_skip_char( l, '=' ) )
         {
            /* optional fixed length */
            if( line_read_integer_expression( l, &FileLength ) == FALSE )
            {
               WARN_STRING_FORMULA_TOO_COMPLEX;
               return bwb_zline(l);
            }
            if( FileLength <= 0 )
            {
               WARN_STRING_FORMULA_TOO_COMPLEX;
               return bwb_zline(l);
            }
         }
      }
      else
      {
         /* DIM # file_num , var_name AS DOUBLE */
         FileLength = sizeof( BasicNumberType );
      }
#endif /* NEW_VIRTUAL */
      v = var_find( varname, n_params, FALSE );
      if( v == NULL )
      {
         /*  a new variable */
         v = CALLOC( 1, sizeof( VariableType ), "bwb_dim" );
         if( v == NULL )
         {
            WARN_OUT_OF_MEMORY;
            return bwb_zline(l);
         }
         bwb_strcpy(v->name, varname);
         v->VariableTypeChar = TypeChar;
         v->dimensions = n_params;

         /* calculate the array size */
         v->array_units = 1;
         for (n = 0; n < v->dimensions; n++)
         {
            v->UBOUND[n] = pp[n];
            v->LBOUND[n] = My->CurrentVersion->OptionBaseValue;
            if( v->UBOUND[n] < v->LBOUND[n] )
            {
               WARN_SUBSCRIPT_OUT_OF_RANGE;
               return bwb_zline(l);
            }
            v->array_pos[n] = v->LBOUND[n];
            v->array_units *= v->UBOUND[n] - v->LBOUND[n] + 1;
         }
         /* assign array memory */

#if NEW_VIRTUAL
         if( FileNumber > 0 )
         {
            /* the new variable is VIRTUAL */
            v->VariableFlags = VARIABLE_VIRTUAL;
            /* if( TRUE ) */
            {
               /* OK */
               VirtualType * Z;
               Z = find_virtual_by_variable( v );
               if( Z == NULL )
               {
                  Z = new_virtual();
                  if( Z == NULL )
                  {
                     WARN_OUT_OF_MEMORY;
                     return bwb_zline(l);
                  }
                  Z->Variable = v;
               }
               /* update file information */
               Z->FileNumber = FileNumber;
               Z->FileOffset = FileOffset;
               Z->FileLength = FileLength;
               FileOffset += FileLength * v->array_units;
            }
         }
         else
#endif /* NEW_VIRTUAL */
         if ( VAR_IS_STRING( v ) )
         {
            if ((v->memstr = (StringType *)
                 CALLOC(v->array_units, sizeof(StringType), "bwb_dim")) == NULL)
            {
               WARN_OUT_OF_MEMORY;
               return bwb_zline(l);
            }
         }
         else
         {
            /* Revised to CALLOC pass-thru call by JBV */
            if ((v->memnum = (BasicNumberType *)
                 CALLOC(v->array_units, sizeof(BasicNumberType), "bwb_dim")) == NULL)
            {
               WARN_OUT_OF_MEMORY;
               return bwb_zline(l);
            }
         }
         /* set place at beginning of variable chain */
         var_link_new_variable( v );

         /* end of conditional for new variable */
      }
      else
      {
         /* old variable */
         if( v->VariableTypeChar != TypeChar )
         {
            WARN_TYPE_MISMATCH;
            return bwb_zline(l);
         }
      
         /* check to be sure the number of dimensions is the same */
         if (v->dimensions != n_params)
         {
            WARN_REDIMENSION_ARRAY;
            return bwb_zline(l);
         }
         /* check to be sure sizes for each dimension are the same */
         for (n = 0; n < v->dimensions; n++)
         {
            if( v->UBOUND[n] != pp[n] )
            {
               WARN_REDIMENSION_ARRAY;
               return bwb_zline(l);
            }
            if( v->LBOUND[n] != My->CurrentVersion->OptionBaseValue )
            {
               WARN_REDIMENSION_ARRAY;
               return bwb_zline(l);
            }
            
         }
#if NEW_VIRTUAL
         if( FileNumber > 0 )
         {
            /* the existing variable MUST be Virtual */
            if( v->VariableFlags & VARIABLE_VIRTUAL )
            {
               /* OK */
               VirtualType * Z;
               Z = find_virtual_by_variable( v );
               if( Z == NULL )
               {
                  Z = new_virtual();
                  if( Z == NULL )
                  {
                     WARN_OUT_OF_MEMORY;
                     return bwb_zline(l);
                  }
                  Z->Variable = v;
               }
               /* update file information */
               Z->FileNumber = FileNumber;
               Z->FileOffset = FileOffset;
               Z->FileLength = FileLength;
               FileOffset += FileLength * v->array_units;
            }
            else
            {
               /* the existing variable is NOT virtual */
               WARN_TYPE_MISMATCH;
               return bwb_zline(l);
            }
         }
         else
         {
            /* the existing variable CANNOT be Virtual */
            if( v->VariableFlags & VARIABLE_VIRTUAL )
            {
               /* the existing variable IS virtual */
               WARN_TYPE_MISMATCH;
               return bwb_zline(l);
            }
            else
            {
               /* OK */
            }
         }
#endif /* NEW_VIRTUAL */
         /* end of conditional for old variable */
      }     

   }
   while( line_skip_comma( l ) );

   /* return */
   return bwb_zline(l);
}




/***************************************************************
  
        FUNCTION:       dim_unit()
  
        DESCRIPTION:    This function calculates the unit
            position for an array.
  
***************************************************************/

static size_t dim_unit(VariableType * v, int *pp)
{
   size_t          r;
   size_t          b;
   register int    n;

   bwx_DEBUG(__FUNCTION__);

   /* Calculate and return the address of the dimensioned array */

   /* Check EACH dimension for out-of-bounds, AND check correct number
    * of dimensions.  NBS_P076_0250 errors correctly. */
    
   /*
   Ux = Upper bound of dimension
   Lx = Lower bound of dimension
   Ix = Selected idex in dimension
   
   dimensions   b   
   0            1   
   1            b0 * ( U0 - L0 + 1 )
   2            b1 * ( U1 - L1 + 1 )
   3            b2 * ( U2 - L2 + 1 )


   dimensions   r   
   0            0   
   1            r0 + ( I0 - L0 ) * b0
   2            r1 + ( I1 - L1 ) * b1
   3            r2 + ( I2 - L2 ) * b2

   */

   r = 0;
   b = 1;
   for (n = 0; n < v->dimensions; n++)
   {
      if( pp[n] < v->LBOUND[n] || pp[n] > v->UBOUND[n] )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return 0;
      }
      r += b * ( pp[n] - v->LBOUND[n] );
      b *= v->UBOUND[n] - v->LBOUND[n] + 1;
   }


   if (r > v->array_units)
   {
      WARN_SUBSCRIPT_OUT_OF_RANGE;
      return 0;
   }
   return r;

}


/***************************************************************
  
        FUNCTION:       bwb_option()
  
        DESCRIPTION:    This function implements the BASIC OPTION
                        BASE statement, designating the base (1 or
                        0) for addressing DIM arrays.
  
   SYNTAX:     OPTION BASE number
  
***************************************************************/

void
OptionVersionSet(int i)
{
   bwx_DEBUG(__FUNCTION__);

   My->CurrentVersion = &bwb_vertable[i];
}

LineType *
bwb_OPTION(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ANGLE(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ANGLE_DEGREES(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ANGLE DEGREES */
   My->CurrentVersion->OptionFlags |= OPTION_ANGLE_DEGREES;
   My->CurrentVersion->OptionFlags &= ~OPTION_ANGLE_GRADIANS;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ANGLE_GRADIANS(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ANGLE DEGREES */
   My->CurrentVersion->OptionFlags &= ~OPTION_ANGLE_DEGREES;
   My->CurrentVersion->OptionFlags |= OPTION_ANGLE_GRADIANS;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ANGLE_RADIANS(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ANGLE RADIANS */
   My->CurrentVersion->OptionFlags &= ~OPTION_ANGLE_DEGREES;
   My->CurrentVersion->OptionFlags &= ~OPTION_ANGLE_GRADIANS;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ARITHMETIC(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ARITHMETIC_DECIMAL(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ARITHMETIC DECIMAL */
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ARITHMETIC_FIXED(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ARITHMETIC FIXED */
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ARITHMETIC_NATIVE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ARITHMETIC NATIVE */
   return bwb_zline(l);
}

LineType *
bwb_OPTION_BASE(LineType * l)
{
   int Value;
   bwx_DEBUG(__FUNCTION__);
   /* OPTION BASE integer */
   if( line_read_integer_expression(l, &Value) )
   {
      /* OK */
      My->CurrentVersion->OptionBaseValue = Value;   
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_BUGS(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_BUGS_ON(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION BUGS ON */
   My->CurrentVersion->OptionFlags |= OPTION_BUGS_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_BUGS_OFF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION BUGS OFF */
   My->CurrentVersion->OptionFlags &= ~OPTION_BUGS_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_COMMENT(LineType * l)
{
   /* OPTION COMMENT "char" */

   bwx_DEBUG(__FUNCTION__);

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
      /* OK */
      My->CurrentVersion->OptionCommentChar = Value[0];
      free( Value );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_COMPARE(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_COMPARE_BINARY(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COMPARE BINARY */
   My->CurrentVersion->OptionFlags &= ~OPTION_COMPARE_TEXT;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_COMPARE_DATABASE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COMPARE DATABASE */
   My->CurrentVersion->OptionFlags |= OPTION_COMPARE_TEXT;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_COMPARE_TEXT(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COMPARE TEXT */
   My->CurrentVersion->OptionFlags |= OPTION_COMPARE_TEXT;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_COVERAGE(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_COVERAGE_ON(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COVERAGE ON */
   My->CurrentVersion->OptionFlags |= OPTION_COVERAGE_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_COVERAGE_OFF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION COVERAGE OFF */
   My->CurrentVersion->OptionFlags &= ~OPTION_COVERAGE_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_DATE(LineType * l)
{
   /* OPTION DATE "format" */

   bwx_DEBUG(__FUNCTION__);
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
      /* OK */
      {
         int n;
         n = bwb_strlen( Value );
         if( n > 32 )
         {
            Value[ 32 ] = BasicNulChar;
         }
      }
      bwb_strcpy( My->CurrentVersion->OptionDateFormat, Value );
      free( Value );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_DISABLE(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_DISABLE_COMMAND(LineType * l)
{
   /* OPTION DISABLE COMMAND ... */
   int             IsFound = FALSE;

   bwx_DEBUG(__FUNCTION__);
   /* Get COMMAND */
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
      /* OK */
      {
         /* Name */
         int i;
         for( i = 0; i < NUM_COMMANDS; i++ )
         {
             if( bwb_stricmp( Value, bwb_cmdtable[i].name ) == 0 )
             {
                 /* FOUND */
                 /* DISABLE COMMAND */
                 bwb_cmdtable[i].OptionVersionBitmask &= ~My->CurrentVersion->OptionVersionBitmask;
                 IsFound = TRUE;
             }
         }
      }
      free( Value );
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( My->bwb_ebuf, "IGNORED: %s\n", l->buffer); fputs( My->bwb_ebuf, My->SYSOUT->cfp );
   }
   return bwb_zline(l);
}


LineType *
bwb_OPTION_DISABLE_FUNCTION(LineType * l)
{
   /* OPTION DISABLE FUNCTION ... */
   int             IsFound = FALSE;

   bwx_DEBUG(__FUNCTION__);

   /* Get FUNCTION */
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
      /* OK */
      {
         /* Name */
         int i;
         for( i = 0; i < NUM_FUNCTIONS; i++ )
         {
             if( bwb_stricmp( Value, bwb_prefuncs[i].Name ) == 0 )
             {
                 /* FOUND */
                 /* DISABLE FUNCTION */
                 bwb_prefuncs[i].OptionVersionBitmask &= ~My->CurrentVersion->OptionVersionBitmask;
                 IsFound = TRUE;
             }
         }
      }
      free( Value );
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( My->bwb_ebuf, "IGNORED: %s\n", l->buffer); fputs( My->bwb_ebuf, My->SYSOUT->cfp );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ENABLE(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ENABLE_COMMAND(LineType * l)
{
   /* OPTION ENABLE COMMAND ... */
   int             IsFound = FALSE;

   bwx_DEBUG(__FUNCTION__);

   /* Get COMMAND */
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
      /* OK */
      {
         /* Name */
         int i;
         for( i = 0; i < NUM_COMMANDS; i++ )
         {
             if( bwb_stricmp( Value, bwb_cmdtable[i].name ) == 0 )
             {
                 /* FOUND */
                 /* ENABLE COMMAND */
                 bwb_cmdtable[i].OptionVersionBitmask |= My->CurrentVersion->OptionVersionBitmask;
                 IsFound = TRUE;
             }
         }
      }
      free( Value );
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( My->bwb_ebuf, "IGNORED: %s\n", l->buffer); fputs( My->bwb_ebuf, My->SYSOUT->cfp );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ENABLE_FUNCTION(LineType * l)
{
   /* OPTION ENABLE FUNCTION ... */
   int             IsFound = FALSE;

   bwx_DEBUG(__FUNCTION__);

   /* Get FUNCTION */
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
      /* OK */
      {
         /* Name */
         int i;
         for( i = 0; i < NUM_FUNCTIONS; i++ )
         {
             if( bwb_stricmp( Value, bwb_prefuncs[i].Name ) == 0 )
             {
                 /* FOUND */
                 /* ENABLE FUNCTION */
                 bwb_prefuncs[i].OptionVersionBitmask |= My->CurrentVersion->OptionVersionBitmask;
                 IsFound = TRUE;
             }
         }
      }
      free( Value );
   }
   if( IsFound == FALSE )
   {
       /* display warning message */
       sprintf( My->bwb_ebuf, "IGNORED: %s\n", l->buffer); fputs( My->bwb_ebuf, My->SYSOUT->cfp );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ERROR(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ERROR_GOSUB(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ERROR GOSUB */
   My->CurrentVersion->OptionFlags |= OPTION_ERROR_GOSUB;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ERROR_GOTO(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ERROR GOTO */
   My->CurrentVersion->OptionFlags &= ~OPTION_ERROR_GOSUB;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_EXPLICIT(LineType * l)
{
   /* OPTION EXPLICIT */

   bwx_DEBUG(__FUNCTION__);
   My->CurrentVersion->OptionFlags |= OPTION_EXPLICIT_ON;
   return bwb_zline(l);
}


LineType *
bwb_OPTION_IMAGE(LineType * l)
{
   /* OPTION IMAGE "char" */
   
   bwx_DEBUG(__FUNCTION__);

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
      /* OK */
      My->CurrentVersion->OptionImageChar = Value[0];
      free( Value );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_IMPLICIT(LineType * l)
{
   /* OPTION IMPLICIT */

   bwx_DEBUG(__FUNCTION__);
   My->CurrentVersion->OptionFlags &= ~OPTION_EXPLICIT_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_INDENT(LineType * l)
{
   /* OPTION INDENT number */
   if( line_read_integer_expression(l, &My->OptionIndentValue) == FALSE)
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_LABELS(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_LABELS_ON(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION LABELS ON */
   My->CurrentVersion->OptionFlags |= OPTION_LABELS_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_LABELS_OFF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION LABELS OFF */
   My->CurrentVersion->OptionFlags &= ~OPTION_LABELS_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_PRINT(LineType * l)
{
   /* OPTION PRINT "char" */
   
   bwx_DEBUG(__FUNCTION__);
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
      /* OK */
      My->CurrentVersion->OptionPrintChar = Value[0];
      free( Value );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ROUND(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ROUND_BANK(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ROUND BANK */
   My->OptionRoundType = C_OPTION_ROUND_BANK;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ROUND_MATH(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ROUND MATH */
   My->OptionRoundType = C_OPTION_ROUND_MATH;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_ROUND_TRUNCATE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION ROUND TRUNCATE */
   My->OptionRoundType = C_OPTION_ROUND_TRUNCATE;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_SLEEP(LineType * l)
{
   /* OPTION SLEEP number */
   if( line_read_numeric_expression(l, &My->OptionSleepValue) == FALSE)
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_STDERR(LineType * l)
{
   /* OPTION STDERR "filename" */

   bwx_DEBUG(__FUNCTION__);
   if( line_is_eol( l ) )
   {
      My->SYSPRN->cfp = stderr;
   }
   else
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
      /* OK */
      if( bwb_strlen( Value ) == 0 )
      {
         My->SYSPRN->cfp = stderr;
      }
      else
      {
         My->SYSPRN->cfp = fopen( Value, "w+" );
         if( My->SYSPRN->cfp == NULL )
         {
            My->SYSPRN->cfp = stderr;
            WARN_BAD_FILE_NAME;
         }
      }
      free( Value );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_STDIN(LineType * l)
{
   /* OPTION STDIN "filename" */

   bwx_DEBUG(__FUNCTION__);
   if( line_is_eol( l ) )
   {
      My->SYSIN->cfp = stdin;
   }
   else
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
      /* OK */
      if( bwb_strlen( Value ) == 0 )
      {
         My->SYSIN->cfp = stdin;
      }
      else
      {
         My->SYSIN->cfp = fopen( Value, "r" );
         if( My->SYSIN->cfp == NULL )
         {
            My->SYSIN->cfp = stdin;
            WARN_BAD_FILE_NAME;
         }
      }
      free( Value );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_STDOUT(LineType * l)
{
   /* OPTION STDOUT "filename" */

   bwx_DEBUG(__FUNCTION__);
   if( line_is_eol( l ) )
   {
      My->SYSOUT->cfp = stdout;
   }
   else
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
      /* OK */
      if( bwb_strlen( Value ) == 0 )
      {
         My->SYSOUT->cfp = stdout;
      }
      else
      {
         My->SYSOUT->cfp = fopen( Value, "w+" );
         if( My->SYSOUT->cfp == NULL )
         {
            My->SYSOUT->cfp = stdout;
            WARN_BAD_FILE_NAME;
         }
      }
      free( Value );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_STATEMENT(LineType * l)
{
   /* OPTION STATEMENT "char" */

   bwx_DEBUG(__FUNCTION__);
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
      /* OK */
      My->CurrentVersion->OptionStatementChar = Value[0];
      free( Value );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_STRICT(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_STRICT_ON(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION STRICT ON */
   My->CurrentVersion->OptionFlags |= OPTION_STRICT_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_STRICT_OFF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION STRICT OFF */
   My->CurrentVersion->OptionFlags &= ~OPTION_STRICT_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_TERMINAL(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_TERMINAL_NONE(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TERMINAL NONE */
   My->OptionTerminalType = C_OPTION_TERMINAL_NONE;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_TERMINAL_ADM(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TERMINAL ADM-3A */
   My->OptionTerminalType = C_OPTION_TERMINAL_ADM;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_TERMINAL_ANSI(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TERMINAL ANSI */
   My->OptionTerminalType = C_OPTION_TERMINAL_ANSI;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_TIME(LineType * l)
{
   /* OPTION TIME format */

   bwx_DEBUG(__FUNCTION__);
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
      /* OK */
      {
         int n;
         n = bwb_strlen( Value );
         if( n > 32 )
         {
            Value[32] = BasicNulChar;
         }
      }
      bwb_strcpy( My->CurrentVersion->OptionTimeFormat, Value );
      free( Value );
   }
   return bwb_zline(l);
}

LineType *
bwb_OPTION_TRACE(LineType * l)
{
   WARN_SYNTAX_ERROR;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_TRACE_ON(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TRACE ON */
   My->CurrentVersion->OptionFlags |= OPTION_TRACE_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_TRACE_OFF(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   /* OPTION TRACE OFF */
   My->CurrentVersion->OptionFlags &= ~OPTION_TRACE_ON;
   return bwb_zline(l);
}

LineType *
bwb_OPTION_USING(LineType * l)
{
   /* OPTION USING "magic" */
   /* 
   This is a short-term solution.
   The long-term solution is:
   OPTION USING DIGIT "#"
   OPTION USING EXRAD "^"
   and so on.
   */
   int  i;
   int  n;
   char tbuf[12 + 1];
   bwx_DEBUG(__FUNCTION__);
   

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
      /* OK */
      {
         int n;
         n = bwb_strlen( Value );
         if( n > 12 )
         {
            Value[12] = BasicNulChar;
         }
      }
      bwb_strcpy( tbuf, Value );
      free( Value );
   }
   /* OK */
   /* 
   this command is special:
   a space in a position indicates 
   that position is disabled.
   The following would disable all magic characters:
   100 OPTION USING "                "
   */
   n = bwb_strlen( tbuf );
   /* PAD RIGHT with NUL */
   for( i = n; n < 12; i++ )
   {
      tbuf[ i ] = BasicNulChar;
   }
   /* convert space to NUL */
   for( i = 0; i < n; i++ )
   {
      if( tbuf[ i ] == ' ' )
      {
         tbuf[ i ] = BasicNulChar;
      }
   }
   My->CurrentVersion->UsingDigit   = tbuf[  0 ];    /* Digit placeholder, typically '#' */
   My->CurrentVersion->UsingComma   = tbuf[  1 ];    /* Comma, such as thousands and millions, typically ',' */
   My->CurrentVersion->UsingPeriod  = tbuf[  2 ];    /* Period, such as between dollars and cents, typically '.' */
   My->CurrentVersion->UsingPlus    = tbuf[  3 ];    /* Plus  sign, positive value, typically '+' */
   My->CurrentVersion->UsingMinus   = tbuf[  4 ];    /* Minus sign, negative value, typically '-' */
   My->CurrentVersion->UsingExrad   = tbuf[  5 ];    /* Exponential format, typically '^' */
   My->CurrentVersion->UsingDollar  = tbuf[  6 ];    /* Currency symbol, such as dollar or pound, typically '$' */
   My->CurrentVersion->UsingFiller  = tbuf[  7 ];    /* Print filler, such as payroll checks, typically '*' */
   My->CurrentVersion->UsingLiteral = tbuf[  8 ];    /* Print the next character as a literal, typically '_' */
   My->CurrentVersion->UsingFirst   = tbuf[  9 ];    /* Print the first character of the string, typically '!' */
   My->CurrentVersion->UsingAll     = tbuf[ 10 ];    /* Print the entire string, typically '&' */
   My->CurrentVersion->UsingLength  = tbuf[ 11 ];    /* Print a substring, typically '%' */
   return bwb_zline(l);
}


LineType *
bwb_OPTION_VERSION(LineType * l)
{
   /* OPTION VERSION ... */
   char            tbuf[BasicStringLengthMax + 1];
   int             i;


   bwx_DEBUG(__FUNCTION__);

   if( line_read_element(l, tbuf) )
   {
       /*  a version was specified */
       for (i = 0; i < NUM_VERSIONS; i++)
       {
          if (bwb_stricmp(tbuf, bwb_vertable[i].Name) == 0)
          {
             /* FOUND */
             OptionVersionSet(i);
             return bwb_zline(l);
          }
       }
       /* NOT FOUND */
       sprintf(My->bwb_ebuf, "OPTION VERSION %s IS INVALID\n", tbuf); fputs( My->bwb_ebuf, My->SYSOUT->cfp );
   }
   sprintf(My->bwb_ebuf, "VALID CHOICES ARE:\n"); fputs( My->bwb_ebuf, My->SYSOUT->cfp );
   for (i = 0; i < NUM_VERSIONS; i++)
   {
      sprintf(My->bwb_ebuf, "OPTION VERSION %-16s ' %s\n",
         bwb_vertable[i].Name, bwb_vertable[i].Description);
      fputs( My->bwb_ebuf, My->SYSOUT->cfp );
   }
   return bwb_zline(l);
}

int var_get( VariableType * variable, VariantType * variant )
{
   size_t          offset;

   /* check sanity */
   if( variable == NULL ) { WARN_INTERNAL_ERROR; return FALSE; }
   if( variant  == NULL ) { WARN_INTERNAL_ERROR; return FALSE; }

   /* Check subscripts */
   if( dim_check( variable ) == FALSE ) { WARN_SUBSCRIPT_OUT_OF_RANGE; return FALSE; }

   /* Determine offset from array base ( for scalars the offset is always zero ) */
   offset = dim_unit( variable, variable->array_pos );
   
   /* Force compatibility */
   CLEAR_VARIANT( variant );
   variant->TypeChar = variable->VariableTypeChar;
   
   if( variable->VariableTypeChar == '$' ) 
   {
      /* Variable is a STRING */
      StringType Value;
      
      Value.sbuffer = NULL;
      Value.length  = 0;
      /* both STRING */

#if NEW_VIRTUAL
      if( variable->VariableFlags & VARIABLE_VIRTUAL ) /* var_set() */
      {
         /* get file information */
         VirtualType * Z;
         FileType * F;

         Z = find_virtual_by_variable( variable );
         if( Z == NULL )
         {
            WARN_INTERNAL_ERROR;
            return FALSE;
         }
         offset *= Z->FileLength; /* Byte offset */
         offset += Z->FileOffset; /* Beginning of this data */
         /* update file information */
         F = find_file_by_number( Z->FileNumber );
         if( F == NULL )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( F->mode != DEVMODE_BINARY )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( F->cfp == NULL )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( fseek(F->cfp, offset, SEEK_SET ) != 0 )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         Value.length = Z->FileLength;
         Value.sbuffer = calloc( 1, Value.length + 1 /* NUL */ );
         if( Value.sbuffer == NULL ) { WARN_OUT_OF_MEMORY; return FALSE; }
         if( fread( Value.sbuffer, Value.length, 1, F->cfp ) != 1 )
         {
            WARN_DISK_IO_ERROR;
            return FALSE;
         }
      }
      else
#else
      if( TRUE )
#endif /* NEW_VIRTUAL */
      {
         StringType * string;

         string = variable->memstr;
         if( string == NULL ) { WARN_INTERNAL_ERROR; return FALSE; }
         string += offset;
         if( str_btob(&Value, string) == FALSE ) { WARN_INTERNAL_ERROR; return FALSE; }
      }
      variant->Buffer = Value.sbuffer;
      variant->Length = Value.length;
   }
   else
   {
      /* Variable is a NUMBER */
      BasicNumberType Value;
      /* both NUMBER */

#if NEW_VIRTUAL
      if( variable->VariableFlags & VARIABLE_VIRTUAL ) /* var_set() */
      {
         /* get file information */
         VirtualType * Z;
         FileType * F;

         Z = find_virtual_by_variable( variable );
         if( Z == NULL )
         {
            WARN_INTERNAL_ERROR;
            return FALSE;
         }
         offset *= Z->FileLength; /* Byte offset */
         offset += Z->FileOffset; /* Beginning of this data */
         /* update file information */
         F = find_file_by_number( Z->FileNumber );
         if( F == NULL )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( F->mode != DEVMODE_BINARY )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( F->cfp == NULL )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( fseek(F->cfp, offset, SEEK_SET ) != 0 )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( fread( &Value, sizeof(Value), 1, F->cfp ) != 1 )
         {
            WARN_DISK_IO_ERROR;
            return FALSE;
         }
      }
      else
#else
      /* if( TRUE ) */
#endif /* NEW_VIRTUAL */
      {
         BasicNumberType * number;
         
         number = variable->memnum;
         if( number == NULL ) { WARN_INTERNAL_ERROR; return FALSE; }
         number += offset;
         /* copy value */
         Value = *number;
      }
      Value = VerifyNumeric(Value);
      switch( variable->VariableTypeChar )
      {
      case BasicByteSuffix:
      case BasicIntegerSuffix:
      case BasicLongSuffix:
      case BasicCurrencySuffix:
         /* integer values */
         Value = bwb_rint( Value );
         break;
      case BasicSingleSuffix:
      case BasicDoubleSuffix:
         /* float values */
         break;
      default:
         /* ERROR */
         { WARN_INTERNAL_ERROR; return FALSE; }
      }
      variant->Number = Value;
   }
   return TRUE;
}

int var_set( VariableType * variable, VariantType * variant )
{
   size_t          offset;

   /* check sanity */
   if( variable == NULL ) { WARN_INTERNAL_ERROR; return FALSE; }
   if( variant  == NULL ) { WARN_INTERNAL_ERROR; return FALSE; }

   /* Check subscripts */
   if( dim_check( variable ) == FALSE ) { WARN_SUBSCRIPT_OUT_OF_RANGE; return FALSE; }

   /* Determine offset from array base ( for scalars the offset is always zero ) */
   offset = dim_unit( variable, variable->array_pos );

   /* Verify compatibility */
   if( variable->VariableTypeChar == '$' ) 
   {
      /* Variable is a STRING */
      StringType Value;

      /* Verify value is a STRING */
      if( variant->TypeChar != '$' ) { WARN_TYPE_MISMATCH; return FALSE; }
      Value.sbuffer = variant->Buffer;
      Value.length  = variant->Length;
      /* both STRING */
      
#if NEW_VIRTUAL
      if( variable->VariableFlags & VARIABLE_VIRTUAL ) /* var_set() */
      {
         /* get file information */
         VirtualType * Z;
         FileType * F;
         int count;

         Z = find_virtual_by_variable( variable );
         if( Z == NULL )
         {
            WARN_INTERNAL_ERROR;
            return FALSE;
         }
         offset *= Z->FileLength; /* Byte offset */
         offset += Z->FileOffset; /* Beginning of this data */
         /* update file information */
         F = find_file_by_number( Z->FileNumber );
         if( F == NULL )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( F->mode != DEVMODE_BINARY )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( F->cfp == NULL )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( fseek(F->cfp, offset, SEEK_SET ) != 0 )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         count = MIN( Value.length, Z->FileLength );
         if( fwrite( Value.sbuffer, sizeof(char), count, F->cfp ) != count )
         {
            WARN_DISK_IO_ERROR;
            return FALSE;
         }
         /* PADR */
         while( count < Z->FileLength )
         {
            if( fputc( ' ', F->cfp ) == EOF )
            {
               WARN_BAD_FILE_MODE;
               return FALSE;
            }
            count++;
         }
      }
      else
#else
      /* if( TRUE ) */
#endif /* NEW_VIRTUAL */ 
      {
         StringType * string;

         string = variable->memstr;
         if( string == NULL ) { WARN_INTERNAL_ERROR; return FALSE; }
         string += offset;
         if( str_btob(string, &Value) == FALSE ) { WARN_INTERNAL_ERROR; return FALSE; }
      }



      if(variable->VariableFlags & VARIABLE_DISPLAY) /* var_set() */
      {
         char tbuf[32];
         
         prn_xprintf( "#" );
         sprintf( tbuf, "%d", My->ThisLine->number);
         prn_xprintf( tbuf );
         prn_xprintf( " " );
         prn_xprintf( variable->name ); 
         prn_xprintf( "=" );
         prn_xprintf( variant->Buffer );
         prn_xprintf( "\n" );
      }
   }
   else
   {
      /* Variable is a NUMBER */
      BasicNumberType Value;

      /* Verify value is a NUMBER */
      if( variant->TypeChar == '$' ) { WARN_TYPE_MISMATCH; return FALSE; }

      /* both NUMBER */
      Value = variant->Number;
      Value = VerifyNumeric(Value);
      switch( variable->VariableTypeChar )
      {
      case BasicByteSuffix:
         Value = bwb_rint( Value );
         if( NumberValueCheck(P1BYT,Value) )
         {
            if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") ) { return FALSE; }
         }
         break;
      case BasicIntegerSuffix:
         Value = bwb_rint( Value );
         if( NumberValueCheck(P1INT,Value) )
         {
            if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") ) { return FALSE; }
         }
         break;
      case BasicLongSuffix:
         Value = bwb_rint( Value );
         if( NumberValueCheck(P1LNG,Value) )
         {
            if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") ) { return FALSE; }
         }
         break;
      case BasicCurrencySuffix:
         Value = bwb_rint( Value );
         if( NumberValueCheck(P1CUR,Value) )
         {
            if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") ) { return FALSE; }
         }
         break;
      case BasicSingleSuffix:
         if( NumberValueCheck(P1FLT,Value) )
         {
            if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") ) { return FALSE; }
         }
         break;
      case BasicDoubleSuffix:
         if( NumberValueCheck(P1DBL,Value) )
         {
            if( bwb_Warning_Overflow("*** Arithmetic Overflow ***") ) { return FALSE; }
         }
         break;
      default:
         /* ERROR */
         { WARN_INTERNAL_ERROR; return FALSE; }
      }
#if NEW_VIRTUAL
      if( variable->VariableFlags & VARIABLE_VIRTUAL ) /* var_set() */
      {
         /* get file information */
         VirtualType * Z;
         FileType * F;

         Z = find_virtual_by_variable( variable );
         if( Z == NULL )
         {
            WARN_INTERNAL_ERROR;
            return FALSE;
         }
         offset *= Z->FileLength; /* Byte offset */
         offset += Z->FileOffset; /* Beginning of this data */
         /* update file information */
         F = find_file_by_number( Z->FileNumber );
         if( F == NULL )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( F->mode != DEVMODE_BINARY )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( F->cfp == NULL )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( fseek(F->cfp, offset, SEEK_SET ) != 0 )
         {
            WARN_BAD_FILE_MODE;
            return FALSE;
         }
         if( fwrite( &Value, sizeof(Value), 1, F->cfp ) != 1 )
         {
            WARN_DISK_IO_ERROR;
            return FALSE;
         }
      }
      else
#else
      /* if( TRUE ) */
#endif /* NEW_VIRTUAL */ 
      {
         BasicNumberType * number;
         number = variable->memnum;
         if( number == NULL ) { WARN_INTERNAL_ERROR; return FALSE; }
         number += offset;
         *number = Value;
      }
      if(variable->VariableFlags & VARIABLE_DISPLAY) /* var_set() */
      {
         char tbuf[32];
         
         prn_xprintf( "#" );
         sprintf( tbuf, "%d", My->ThisLine->number);
         prn_xprintf( tbuf );
         prn_xprintf( " " );
         prn_xprintf( variable->name ); 
         prn_xprintf( "=" );
         sprintf( tbuf, "%f", Value );
         prn_xprintf( tbuf );
         prn_xprintf( "\n" );
      }
   }
   return TRUE;
}

/***************************************************************
  
        FUNCTION:       dim_check()
  
        DESCRIPTION:    This function checks subscripts of a
                        specific variable to be sure that they
                        are within the correct range.
  
***************************************************************/

static int dim_check( VariableType * variable )
{
   register int    n;

   /* Check for validly allocated array */


#if NEW_VIRTUAL
   if( variable->VariableFlags & VARIABLE_VIRTUAL ) /* var_set() */
   {
      if(variable->memstr != NULL)
      {
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
      if (variable->memnum != NULL)
      {
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
   }
   else
#endif /* NEW_VIRTUAL */
   if( VAR_IS_STRING( variable ) )
   {
      if(variable->memstr == NULL)
      {
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
      if (variable->memnum != NULL)
      {
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
   }
   else
   {
      if(variable->memstr != NULL)
      {
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
      if (variable->memnum == NULL)
      {
         WARN_INTERNAL_ERROR;
         return FALSE;
      }
   }
   /* Now check subscript values */
   for (n = 0; n < variable->dimensions; n++)
   {
      if( variable->array_pos[n] < variable->LBOUND[n] || variable->array_pos[n] > variable->UBOUND[n] )
      {
         WARN_SUBSCRIPT_OUT_OF_RANGE;
         return FALSE;
      }
   }
   /* No problems found */
   return TRUE;
}

/***************************************************************
  
        FUNCTION:       var_make()
  
        DESCRIPTION: This function initializes a variable,
            allocating necessary memory for it.
  
***************************************************************/

int
var_make(VariableType * variable, char type)
{
   /* ALL variables are created here */
   
   bwx_DEBUG(__FUNCTION__);

   switch( type )
   {
   case BasicStringSuffix:
   case BasicDoubleSuffix:
   case BasicSingleSuffix:
   case BasicCurrencySuffix:
   case BasicLongSuffix:
   case BasicIntegerSuffix:
   case BasicByteSuffix:
      /* OK */
      break;
   default:
      /* ERROR */
      WARN_TYPE_MISMATCH;
      return FALSE;
   }
   
   variable->VariableTypeChar = type;

   /* get memory for array */

   /* First cleanup the joint (JBV) */
   if (variable->memnum != NULL)
   {
      /* Revised to FREE pass-thru call by JBV */
      FREE(variable->memnum, "var_make");
      variable->memnum = NULL;
   }
   if (variable->memstr != NULL)
   {
      /* Remember to deallocate those far-flung branches! (JBV) */
      StringType        *sp;  /* JBV */
      register int    n;   /* JBV */

      sp = variable->memstr;
      for (n = 0; n < (int) variable->array_units; ++n)
      {
         if (sp[n].sbuffer != NULL)
         {
            /* Revised to FREE pass-thru call by JBV */
            FREE(sp[n].sbuffer, "var_make");
            sp[n].sbuffer = NULL;
         }
         sp[n].length = 0;
      }
      /* Revised to FREE pass-thru call by JBV */
      FREE(variable->memstr, "var_make");
      variable->memstr = NULL;
   }

   variable->dimensions = 0;
   variable->array_units = 1;

   if ( VAR_IS_STRING( variable ) )
   {
      /* Revised to CALLOC pass-thru call by JBV */
      if ((variable->memstr = CALLOC(variable->array_units, sizeof(StringType), "var_make")) == NULL)
      {
         WARN_OUT_OF_MEMORY;
         return FALSE;
      }
   }
   else
   {
      /* Revised to CALLOC pass-thru call by JBV */
      if ((variable->memnum = CALLOC(variable->array_units, sizeof(BasicNumberType), "var_make")) == NULL)
      {
         WARN_OUT_OF_MEMORY;
         return FALSE;
      }
   }
   return TRUE;

}

/***************************************************************
  
   FUNCTION:       var_islocal()
  
   DESCRIPTION:    This function determines whether the string
         pointed to by 'buffer' has the name of
         a local variable at the present EXEC stack
         level.
  
***************************************************************/

static VariableType * mat_islocal(char *buffer)
{
   /*
   similar to var_islocal, but returns first matrix found.
   */

   bwx_DEBUG(__FUNCTION__);

   if ( My->stack_head != NULL )
   {
      StackType * stack_item;
      for (stack_item = My->stack_head; stack_item != NULL; stack_item = stack_item->next)
      {
         if (stack_item->LoopTopLine != NULL)
         {
            switch (stack_item->LoopTopLine->cmdnum)
            {
            case C_DEF:
            case C_FUNCTION:
            case C_SUB:
               /* we have found a FUNCTION or SUB boundary */
               {
                  VariableType * variable;
   
                  for ( variable = stack_item->local_variable; variable != NULL; variable = variable->next)
                  {
                     if( variable->dimensions > 0 )
                     {
                        if (bwb_stricmp(variable->name, buffer) == 0)
                        {
                           /* FOUND */
                           return variable;
                        }
                     }
                  }
               }
               /* we have checked all the way to a FUNCTION or SUB boundary */
               /* NOT FOUND */
               return NULL;
               /* break; */
            }
         }
      }
   }
   /* NOT FOUND */
   return NULL;
}


static VariableType * var_islocal(char *buffer, int dimensions)
{

   bwx_DEBUG(__FUNCTION__);

   if ( My->stack_head != NULL )
   {
      StackType * stack_item;
      for (stack_item = My->stack_head; stack_item != NULL; stack_item = stack_item->next)
      {
         if (stack_item->LoopTopLine != NULL)
         {
            switch (stack_item->LoopTopLine->cmdnum)
            {
            case C_DEF:
            case C_FUNCTION:
            case C_SUB:
               /* we have found a FUNCTION or SUB boundary */
               {
                  VariableType * variable;
   
                  for (variable = stack_item->local_variable; variable != NULL; variable = variable->next)
                  {
                     if( variable->dimensions == dimensions )
                     {
                        if (bwb_stricmp(variable->name, buffer) == 0)
                        {
                           /* FOUND */
                           return variable;
                        }
                     }
                  }
               }
               /* we have checked all the way to a FUNCTION or SUB boundary */
               /* NOT FOUND */
               return NULL;
               /* break; */
            }
         }
      }
   }
   /* NOT FOUND */
   return NULL;
}

/***************************************************************
  
        FUNCTION:       bwb_vars()
  
        DESCRIPTION:    This function implements the Bywater-
            specific debugging command VARS, which
            gives a list of all variables defined
            in memory.
  
***************************************************************/


LineType *
bwb_VARS(LineType * l)
{
   VariableType * variable;
   
   bwx_DEBUG(__FUNCTION__);

   /* run through the variable list and print variables */


   sprintf(My->bwb_ebuf, "%-*s %-6s %-4s %s\n", BasicNameLengthMax, "Name", "Type", "Dims", "Value");
   prn_xprintf(My->bwb_ebuf);

   for (variable = My->var_head; variable != NULL; variable = variable->next)
   {
      VariantType variant;

      if( var_get( variable, &variant ) == FALSE )
      {
         WARN_VARIABLE_NOT_DECLARED;
         return bwb_zline(l);
      }
      if( variant.TypeChar == '$' )
      {
         sprintf(My->bwb_ebuf, "%-*s %-6s %-4d %s\n"
         , BasicNameLengthMax, variable->name, "STRING", variable->dimensions, variant.Buffer );
      }
      else
      {
         sprintf(My->bwb_ebuf, "%-*s %-6s %-4d " BasicNumberPrintFormat "\n"
         , BasicNameLengthMax, variable->name, "NUMBER", variable->dimensions, variant.Number );
      }
      RELEASE( (&variant) );
      prn_xprintf(My->bwb_ebuf);
   }
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_field()
  
        DESCRIPTION: This C function implements the BASIC
         FIELD command.
  
***************************************************************/

static
void
field_clear( FieldType * Field )
{
   int i;

   Field->File = NULL;
   Field->FieldOffset = 0;
   Field->FieldLength = 0;
   Field->Var = NULL;
   for( i = 0; i < MAX_DIMS; i++ )
   {
      Field->array_pos[ i ] = 0;
   }
}

static
FieldType * 
field_new( void )
{
   /* search for an empty slot */
   FieldType * Field;

   for( Field = My->field_head; Field != NULL; Field = Field->next )
   {
      if( Field->File == NULL || Field->Var == NULL )
      {
         field_clear( Field );
         return Field;
      }
   }
   /* not found */
   Field = calloc( 1, sizeof( FieldType ) );
   if( Field == NULL )
   {
      WARN_OUT_OF_MEMORY;
      return NULL;
   }
   Field->next = My->field_head;
   My->field_head = Field;
   return Field;
}

void
field_close_file( FileType * File )
{
   /* a CLOSE of a file is in progress, release associated fields */
   FieldType * Field;

   for( Field = My->field_head; Field != NULL; Field = Field->next )
   {
      if( Field->File == File )
      {
         Field->File = NULL;
         Field->Var  = NULL;
      }
   }
}
void
field_free_variable( VariableType * Var )
{
   /* an ERASE of a variable is in progress, release associated fields */
   FieldType * Field;

   for( Field = My->field_head; Field != NULL; Field = Field->next )
   {
      if( Field->Var == Var )
      {
         Field->File = NULL;
         Field->Var  = NULL;
      }
   }
}


void 
field_get( FileType * File )
{
   /* a GET of the RANDOM file is in progress, update variables from FILE buffer */
   FieldType * Field;

   if( File == NULL )
   {
      WARN_BAD_FILE_NUMBER;
      return;
   }
   if( File->buffer == NULL )
   {
      WARN_BAD_FILE_MODE;
      return;
   }
   for( Field = My->field_head; Field != NULL; Field = Field->next )
   {
      if( Field->File == File && Field->Var != NULL )
      {
         /* from file to variable */
         VariantType variant;

         if( Field->FieldOffset < 0 )
         {
            WARN_FIELD_OVERFLOW;
            return;
         }
         if( Field->FieldLength <= 0 )
         {
            WARN_FIELD_OVERFLOW;
            return;
         }
         if( (Field->FieldOffset + Field->FieldLength) > File->width )
         {
            WARN_FIELD_OVERFLOW;
            return;
         }
         variant.TypeChar = BasicStringSuffix;
         variant.Length = Field->FieldLength;
         variant.Buffer = calloc( 1, variant.Length + 1 /* NUL */ );
         if( variant.Buffer == NULL )
         {
            WARN_OUT_OF_MEMORY;
            return;
         }
         /* if( TRUE ) */
         {
            int i;

            for( i = 0; i < Field->Var->dimensions; i++ )
            {
               Field->Var->array_pos[ i ] =  Field->array_pos[ i ];
            }
         }
         /* if( TRUE ) */
         {
            int i;
            char * Buffer;

            Buffer = File->buffer;
            Buffer += Field->FieldOffset;
            for( i = 0; i < variant.Length; i++ )
            {
               variant.Buffer[ i ] = Buffer[ i ];
            }
            variant.Buffer[ variant.Length ] = BasicNulChar;
         }
         if( var_set( Field->Var, &variant ) == FALSE ) { WARN_VARIABLE_NOT_DECLARED; return; }
         RELEASE( (&variant) );
      }
   }
}
void
field_put( FileType * File )
{
   /* a PUT of the RANDOM file is in progress, update FILE buffer from variables  */
   FieldType * Field;

   if( File == NULL )
   {
      WARN_BAD_FILE_NUMBER;
      return;
   }
   if( File->buffer == NULL )
   {
      WARN_BAD_FILE_MODE;
      return;
   }
   for( Field = My->field_head; Field != NULL; Field = Field->next )
   {
      if( Field->File == File && Field->Var != NULL )
      {
         /* from variable to file */
         VariantType variant;

         if( Field->FieldOffset < 0 )
         {
            WARN_FIELD_OVERFLOW;
            return;
         }
         if( Field->FieldLength <= 0 )
         {
            WARN_FIELD_OVERFLOW;
            return;
         }
         if( (Field->FieldOffset + Field->FieldLength) > File->width )
         {
            WARN_FIELD_OVERFLOW;
            return;
         }
         /* if( TRUE ) */
         {
            int i;

            for( i = 0; i < Field->Var->dimensions; i++ )
            {
               Field->Var->array_pos[ i ] =  Field->array_pos[ i ];
            }
         }
         if( var_get( Field->Var, &variant ) == FALSE ) { WARN_VARIABLE_NOT_DECLARED; return; }
         if( variant.TypeChar != BasicStringSuffix ) { WARN_TYPE_MISMATCH; return; }
         /* if( TRUE ) */
         {
            int i = 0;
            int n = 0;
            char * Buffer;
            
            Buffer = File->buffer;
            Buffer += Field->FieldOffset;
            
            if( variant.Buffer != NULL )
            {
               n = MIN( variant.Length, Field->FieldLength );
            }
            for( i = 0; i < n; i++ )
            {
               Buffer[ i ] = variant.Buffer[ i ];
            }
            for( i = n; i < Field->FieldLength; i++ )
            {
               /* Pad on the right with spaces */
               Buffer[ i ] = ' ';
            }
         }
         RELEASE( (&variant) );
      }
   }
}


LineType *
bwb_FIELD(LineType * l)
{
   FileType * File;
   int             FileNumber;
   int             FieldOffset;

   bwx_DEBUG(__FUNCTION__);

   FieldOffset = 0;

   /* first read device number */
   if( line_skip_char(l, BasicFileNumberPrefix) == FALSE )
   {
      /* optional */
   }
   if( line_read_integer_expression(l, &FileNumber) == FALSE )
   {
      WARN_SYNTAX_ERROR;
      return bwb_zline(l);
   }
   if( FileNumber <= 0 )
   {
      /* "FIELD # 0" is an error */
      WARN_BAD_FILE_NUMBER;
      return bwb_zline(l);
   }
   File = find_file_by_number( FileNumber );
   if( File == NULL )
   {
      WARN_BAD_FILE_NUMBER;
      return bwb_zline(l);
   }
   if (File->mode != DEVMODE_RANDOM)
   {
      WARN_BAD_FILE_MODE;
      return bwb_zline(l);
   }
   /* loop to read variables */


   /* read the comma and advance beyond it */
   while (line_skip_comma(l))
   {
      int             FieldLength;
      VariableType *  variable;
      VariantType variant;

      /* first find the size of the field */

      if( line_read_integer_expression(l, &FieldLength) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }
      if( FieldLength <= 0 )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }

      /* read the AS */
      if( line_skip_word( l, "AS" ) == FALSE )
      {
         WARN_SYNTAX_ERROR;
         return bwb_zline(l);
      }

      /* read the string variable name */
      if( (variable = line_read_scalar(l)) == NULL )
      {
          WARN_VARIABLE_NOT_DECLARED;
          return bwb_zline(l);
      }

      if ( VAR_IS_STRING( variable ) )
      {
         /* OK */
      }
      else
      {
         WARN_TYPE_MISMATCH;
         return bwb_zline(l);
      }
      /* check for overflow of record length */
      if ((FieldOffset + FieldLength) > File->width)
      {
         WARN_FIELD_OVERFLOW;
         return bwb_zline(l);
      }
      /* set buffer */
      variant.TypeChar = BasicStringSuffix;
      /* if( TRUE ) */
      {
         FieldType * Field;
         int i;
         
         Field = field_new();
         if( Field == NULL )
         {
            WARN_OUT_OF_MEMORY;
            return bwb_zline(l);
         }
         Field->File = File;
         Field->FieldOffset = FieldOffset;
         Field->FieldLength = FieldLength;
         Field->Var = variable;
         for( i = 0; i < MAX_DIMS; i++ )
         {
            Field->array_pos[ i ] = variable->array_pos[ i ];
         }
         variant.Length = FieldLength;
         variant.Buffer = calloc( 1, variant.Length + 1 /* NUL */ );
         if( variant.Buffer == NULL )
         {
            WARN_OUT_OF_MEMORY;
            return bwb_zline(l);
         }
         bwb_memset( variant.Buffer, ' ', variant.Length );
         variant.Buffer[ variant.Length ] = BasicNulChar;
      }
      if( var_set( variable, &variant) == FALSE ) { WARN_VARIABLE_NOT_DECLARED; return bwb_zline( l ); }
      RELEASE( (&variant) );
      FieldOffset += FieldLength;
   }
   /* return */
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_lset()
  
        DESCRIPTION: This C function implements the BASIC
         LSET command.
  
   SYNTAX:     LSET string-variable$ = expression
  
***************************************************************/

LineType *
bwb_LSET(LineType * l)
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

LineType *
bwb_RSET(LineType * l)
{
   bwx_DEBUG(__FUNCTION__);
   return dio_lrset(l, TRUE);
}

/***************************************************************
  
        FUNCTION:       dio_lrset()
  
        DESCRIPTION: This C function implements the BASIC
         RSET and LSET commands.
  
***************************************************************/

static LineType * dio_lrset(LineType * l, int rset)
{
   /* LSET and RSET */
   VariantType variant;
   register int    n, i;
   int             startpos;
   VariableType *v;
   VariantType   t;      /* no leaks */
   VariantType * T = &t; /* no leaks */

   bwx_DEBUG(__FUNCTION__);
   CLEAR_VARIANT( T );
   CLEAR_VARIANT( (&variant) );

   /* find the variable name */
   if( (v = line_read_scalar(l)) == NULL ) { WARN_VARIABLE_NOT_DECLARED; return bwb_zline(l); }
   if ( VAR_IS_STRING( v ) == FALSE ) { WARN_TYPE_MISMATCH; return bwb_zline(l); }

   /* find equals sign */
   if (line_skip_char(l, '=') == FALSE) { WARN_SYNTAX_ERROR; return bwb_zline(l); }
   line_skip_spaces(l);
   /* read remainder of line to get value */
   if( line_read_expression( l, T ) == FALSE ) { WARN_SYNTAX_ERROR; return bwb_zline(l); }
   if( T->TypeChar != BasicStringSuffix ) { WARN_TYPE_MISMATCH; return bwb_zline(l); }
   if( var_get( v, &variant ) == FALSE ) { WARN_VARIABLE_NOT_DECLARED; return bwb_zline(l); }
   /* determine starting position */
   startpos = 0;
   if( rset == TRUE && T->Length < variant.Length )
   {
      /* 
      LET A$ = "123_456" ' variant.Length = 7
      LET B$ = "789"     '      T->Length = 3
      RSET A$ = B$       '       startpos = 4
      PRINT "[";A$;"]"   ' [123_789]
      */
      startpos = variant.Length - T->Length;
   }
   /* write characters to new position */
   for( n = startpos, i = 0; (n < (int) variant.Length) && (i < (int) T->Length); n++, i++ )
   {
      variant.Buffer[ n ] = T->Buffer[ i ];
   }
   if( var_set( v, &variant ) == FALSE ) { WARN_VARIABLE_NOT_DECLARED; return bwb_zline(l); }
   /* OK  */
   RELEASE( T );
   RELEASE( (&variant) );
   
   return bwb_zline(l);
}

/* EOF */
