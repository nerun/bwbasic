/***************************************************************
  
        bwx_tty.c       TTY front-end
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












/***************************************************************
  
      FUNCTION:       bwb_edit()
  
   DESCRIPTION:    This function implements the BASIC EDIT
         program by shelling out to a default editor
         specified by the variable BWB.EDITOR$.
  
   SYNTAX:     EDIT
  
***************************************************************/

LineType *
bwb_EDIT(LineType * l)
{
   VariableType *v; 
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   if( is_empty_filename( My->progfile ) )
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   if( (v = var_find(DEFVNAME_EDITOR,0,FALSE)) == NULL )
   {
       bwb_strcpy(tbuf, DEF_EDITOR );
   }
   else
   {
      VariantType variant;
      
      if( var_get( v, &variant ) == FALSE )
      {
         bwb_strcpy(tbuf, DEF_EDITOR );
      }
      else
      {
         if( variant.TypeChar == '$' )
         {
            bwb_strcpy(tbuf, variant.Buffer );
            RELEASE( (&variant) );
         }
         else
         {
            bwb_strcpy(tbuf, DEF_EDITOR );
         }
      }
   }
   bwb_strcat( tbuf, " " );
   bwb_strcat( tbuf,  My->progfile );
   system(tbuf);

   /* open edited file for read */
   bwb_NEW(l);    /* Relocated by JBV (bug found by DD) */
   if( bwb_fload( NULL /* My->progfile */ ) == FALSE )
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   return bwb_zline(l);
}

/***************************************************************
  
        FUNCTION:       bwb_renum()
  
   DESCRIPTION:    This function implements the BASIC RENUM
         command by shelling out to a default
         renumbering program called "renum".
         Added by JBV 10/95
  
   SYNTAX:     RENUM
  
***************************************************************/

LineType *
bwb_RENUM(LineType * l)
{
   VariableType *v; 
   char            tbuf[BasicStringLengthMax + 1];

   bwx_DEBUG(__FUNCTION__);

   if( is_empty_filename( My->progfile ) )
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   if( (v = var_find(DEFVNAME_RENUM,0,FALSE)) == NULL )
   {
       bwb_strcpy(tbuf, DEF_RENUM );
   }
   else
   {
      VariantType variant;
      
      if( var_get( v, &variant ) == FALSE )
      {
         bwb_strcpy(tbuf, DEF_EDITOR );
      }
      else
      {
         if( variant.TypeChar == '$' )
         {
            bwb_strcpy(tbuf, variant.Buffer );
            RELEASE( (&variant) );
         }
         else
         {
            bwb_strcpy(tbuf, DEF_EDITOR );
         }
      }
   }
   bwb_strcat( tbuf, " " );
   bwb_strcat( tbuf,  My->progfile );
   system(tbuf);

   /* open edited file for read */
   bwb_NEW(l);    /* Relocated by JBV (bug found by DD) */
   if( bwb_fload( NULL /* My->progfile */ ) == FALSE )
   {
      WARN_BAD_FILE_NAME;
      return bwb_zline(l);
   }
   return bwb_zline(l);
}



LineType *
bwb_RENUMBER(LineType * l)
{
   return bwb_RENUM(l); 
}


/* EOF */
