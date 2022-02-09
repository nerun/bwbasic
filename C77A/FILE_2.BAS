  100 rem Purpose: verify the FILE command
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-12-20
  130 rem    Note: OPTION VERSION CBASIC-II
  140 rem
  200 RESTORE
    : LET A$ = "FILE.TXT"
  210 PRINT "ABOUT TO OPEN FILE"
    : FILE A$(64)
    : PRINT "FILE WAS OPENED"
  230 READ B,B$
    : IF B = 0 OR B$ = "ZZZ" GOTO 250
    : LET B$ = "THIS IS THE " + B$ + " RECORD"
    : PRINT #1, B; B$
    : GOTO 230
  250 REM
  270 FOR I = 1 TO 9
    :   READ #1, I; B$
    :   PRINT B$
    : NEXT I
  300 FOR I = 9 TO 1 STEP -1
    :   READ #1, I; B$
    :   PRINT B$
    : NEXT I
  310 READ #1, 1 ;
  320 FOR I = 1 TO 9
    :   READ #1 ; B$
    :   PRINT B$
    : NEXT I
  800 REM quasi-random order
  810 DATA 7,7th
  820 DATA 2,2nd
  830 DATA 9,9th
  840 DATA 4,4th
  850 DATA 6,6th
  860 DATA 1,1st
  870 DATA 5,5th
  880 DATA 3,3rd
  890 DATA 8,8th
  900 DATA 0,ZZZ
  999 END

