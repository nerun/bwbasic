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
/*---------------------------------------------------------------*/




#include "bwbasic.h"




#if REDIRECT_STDERR
static FILE    *newerr;
#endif



static int      program_run = 0;/* has the command-line program been run? */
jmp_buf         mark;



static
void
strupper(char *C)
{
   if (C == NULL)
   {
      return;
   }
   while (*C)
   {
      *C = ToUpper(*C);
      C++;
   }
}

static
void
strlower(char *C)
{
   if (C == NULL)
   {
      return;
   }
   while (*C)
   {
      *C = ToLower(*C);
      C++;
   }
}

/***************************************************************
  
        FUNCTION:       main()
  
        DESCRIPTION:    As in any C program, main() is the basic
                        function from which the rest of the
                        program is called. Some environments,
         however, provide their own main() functions
         (Microsoft Windows (tm) is an example).
         In these cases, the following code will
         have to be included in the initialization
         function that is called by the environment.
  
***************************************************************/


int
main(int argc, char **argv)
{
#if PROFILE
   struct bwb_variable *v;
   FILE           *profile;
#endif


   bwx_DEBUG(__FUNCTION__);

   /* Redirect stderr if specified */

#if REDIRECT_STDERR
   newerr = freopen(ERRFILE, "w", stderr);
   if (newerr == NULL)
   {
      sprintf(bwb_ebuf, "Failed to redirect error messages to file <%s>\n",
         ERRFILE);
      errfdevice = stdout;
      prn_xprintf(bwb_ebuf);
   }
   else
   {
      sprintf(bwb_ebuf, "NOTE: Error messages are redirected to file <%s>\n",
         ERRFILE);
      prn_xprintf(bwb_ebuf);
      errfdevice = stderr;
   }
#else
   errfdevice = stdout;
#endif




   bwb_init(argc, argv);


   if (err_number < 0 /* bwb_init() failed */ )
   {
      /* FATAL */
      return 1;
   }
   /* Signon message banner */
   if (argc < 2)
   {
      if (IsInteractive)
      {
         bwx_signon();
      }
      else
      {
         /* if INTERACTIVE is FALSE, then we must have a
          * program file */
         puts(err_noprogfile);
         return 1;
      }
   }


   /* initialize preset variables */

   ed = var_find(DEFVNAME_EDITOR);
   ed->preset = TRUE;
   ed->common = TRUE;
   str_ctob(var_findsval(ed, ed->array_pos), DEF_EDITOR);

   fi = var_find(DEFVNAME_FILES);
   fi->preset = TRUE;
   fi->common = TRUE;
   str_ctob(var_findsval(fi, fi->array_pos), DEF_FILES);

   pr = var_find(DEFVNAME_PROMPT);
   pr->preset = TRUE;
   pr->common = TRUE;
   str_ctob(var_findsval(pr, pr->array_pos), PROMPT);

   im = var_find(DEFVNAME_IMPL);
   im->preset = TRUE;
   im->common = TRUE;
   str_ctob(var_findsval(im, im->array_pos), IMP_IDSTRING);

   co = var_find(DEFVNAME_COLORS);
   co->preset = TRUE;
   co->common = TRUE;
   *var_findnval(co, co->array_pos) = DEF_COLORS;


   /* if there is a profile.bas, execute it */

#if PROFILE
   /* AS-IS */
   if (IsInteractive)
   {
      program_run = 0;
      signal(SIGINT, break_mes);
      setjmp(mark);
      if (program_run > 0)
      {
         /* error in profile */
         return 1;
      }
      program_run++;
   }
   profile = fopen(PROFILENAME, "r");
   if (profile != NULL)
   {
      /* FOUND */
      bwb_fload(profile);  /* load profile */
      if (err_number < 0 /* file load failed */ )
      {
         /* the file load has failed, so do NOT run the
          * program */
         return 1;
      }
      bwb_RUN(&CURTASK bwb_start);  /* run profile */

      /* profile must be run immediately, not by scheduler */

      while (CURTASK exsc > -1)
      {
         bwb_execline();
      }

      /* mark all profiled variables as preset */

      for (v = CURTASK var_start.next; v != &CURTASK var_end; v = v->next)
      {
         v->preset = TRUE;
      }

      bwb_NEW(&CURTASK bwb_start);  /* remove profile from memory */
   }
#endif


   /* set a buffer for jump: program execution returns to this point in
    * case of a jump (error, interrupt, or finish program) */

   if (IsInteractive)
   {
      program_run = 0;
      signal(SIGINT, break_mes);
      setjmp(mark);
      if (program_run > 0)
      {
         /* error in command line file */
         return 1;
      }
      program_run++;
   }
   /* check to see if there is a program file: but do this only the
    * first time around! */
   IsCommandLineFile = FALSE;
#if AUTOMATED_REGRESSION
   /* for automated testing */
   ExternalInputFile = NULL;
#endif            /* AUTOMATED_REGRESSION */


   if (argc > 1)
   {
      FILE           *input = NULL;

      input = NULL;
      if (input == NULL)
      {
         /* AS-IS */
         strcpy(CURTASK progfile, argv[1]);
         input = fopen(CURTASK progfile, "r");
      }
      if (input == NULL)
      {
         /* UPPERCASE */
         strcpy(CURTASK progfile, argv[1]);
         strupper(CURTASK progfile);
         input = fopen(CURTASK progfile, "r");
      }
      if (input == NULL)
      {
         /* LOWERCASE */
         strcpy(CURTASK progfile, argv[1]);
         strlower(CURTASK progfile);
         input = fopen(CURTASK progfile, "r");
      }
      if (input == NULL)
      {
         /* UPPERCASE + EXTENSION */
         strcpy(CURTASK progfile, argv[1]);
         strcat(CURTASK progfile, ".BAS");
         strupper(CURTASK progfile);
         input = fopen(CURTASK progfile, "r");
      }
      if (input == NULL)
      {
         /* LOWERCASE + EXTENSION */
         strcpy(CURTASK progfile, argv[1]);
         strcat(CURTASK progfile, ".bas");
         strlower(CURTASK progfile);
         input = fopen(CURTASK progfile, "r");
      }
      if (input == NULL)
      {
         /* NOT FOUND */
         CURTASK         progfile[0] = 0;
         sprintf(bwb_ebuf, err_openfile, argv[1]);
         bwb_error(bwb_ebuf);
         /* the file load has failed, so do NOT run the
          * program */
         return 1;
      }
      if (input != NULL)
      {
         /* FOUND */
#if AUTOMATED_REGRESSION
         /* for automated testing */
         char            ExternalInputFileName[FILENAME_MAX + 1];
         strcpy(ExternalInputFileName, argv[1]);
         strcat(ExternalInputFileName, ".INP");
         ExternalInputFile = fopen(ExternalInputFileName, "r");
#endif            /* AUTOMATED_REGRESSION */


         IsCommandLineFile = TRUE;
         bwb_fload(input);
         if (err_number < 0 /* file load failed */ )
         {
            /* the file load has failed, so do NOT run
             * the program */
            return 1;
         }
         bwb_RUN(&CURTASK bwb_start);
      }
   }
   if (IsInteractive)
   {
      program_run = 0;
      signal(SIGINT, break_mes);
      setjmp(mark);
      if (program_run > 0)
      {
         /* error in console mode */
      }
      program_run++;
   }
   /* main program loop */

   while (!feof(stdin)) /* condition !feof( stdin ) added in v1.11 */
   {
      bwb_mainloop();
   }

   bwx_terminate();  /* allow ^D (Unix) exit with grace */




   return 0;
}






/***************************************************************
  
      FUNCTION:       bwb_edit()
  
   DESCRIPTION:    This function implements the BASIC EDIT
         program by shelling out to a default editor
         specified by the variable BWB.EDITOR$.
  
   SYNTAX:     EDIT
  
***************************************************************/

struct bwb_line *
bwb_EDIT(struct bwb_line * l)
{
   char            tbuf[BasicStringLengthMax + 1];
   FILE           *loadfile;
   struct bwb_variable *v; 

   bwx_DEBUG(__FUNCTION__);

   v = var_find(DEFVNAME_EDITOR); 
   str_btoc(tbuf, var_getsval(v)); 
   strcat( tbuf, " " );
   strcat( tbuf, CURTASK progfile );

   system(tbuf);

   /* open edited file for read */

   if ((loadfile = fopen(CURTASK progfile, "r")) == NULL)
   {
      sprintf(bwb_ebuf, err_openfile, CURTASK progfile);
      bwb_error(bwb_ebuf);

      return bwb_zline(l);
   }
   /* clear current contents */

   bwb_NEW(l);    /* Relocated by JBV (bug found by DD) */

   /* and (re)load the file into memory */

   bwb_fload(loadfile);


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

struct bwb_line *
bwb_RENUM(struct bwb_line * l)
{
   char            tbuf[BasicStringLengthMax + 1];
   FILE           *loadfile;

   bwx_DEBUG(__FUNCTION__);

   sprintf(tbuf, "renum %s", CURTASK progfile);

   system(tbuf);

   /* open edited file for read */

   if ((loadfile = fopen(CURTASK progfile, "r")) == NULL)
   {
      sprintf(bwb_ebuf, err_openfile, CURTASK progfile);
      bwb_error(bwb_ebuf);

      return bwb_zline(l);
   }
   /* clear current contents */


   bwb_NEW(l);    /* Relocated by JBV (bug found by DD) */

   /* and (re)load the file into memory */

   bwb_fload(loadfile);


   return bwb_zline(l);
}



/* EOF */
