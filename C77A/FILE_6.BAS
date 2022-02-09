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
  230 PRINT #1; "THIS IS THE 1st RECORD"
    : PRINT #1; "THIS IS THE 2nd RECORD"
    : PRINT #1; "THIS IS THE 3rd RECORD"
    : PRINT #1; "THIS IS THE 4th RECORD"
    : PRINT #1; "THIS IS THE 5th RECORD"
    : PRINT #1; "THIS IS THE 6th RECORD"
    : PRINT #1; "THIS IS THE 7th RECORD"
    : PRINT #1; "THIS IS THE 8th RECORD"
    : PRINT #1; "THIS IS THE 9th RECORD"
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
  999 END

