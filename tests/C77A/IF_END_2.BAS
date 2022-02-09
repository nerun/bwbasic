100 REM Purpose: Verify IF END command
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-12-23
130 REM
200 REM -----------------------------------
210 REM CREATE A FILE WITH 10 LINES
220 REM -----------------------------------
230 CREATE "IF_END_2.TXT" AS 777
240 FOR I = 1 TO 10
250   LET A$ = "THIS IS LINE " + STR$(I)
260   PRINT # 777; A$
270 NEXT I
280 CLOSE 777
300 REM -----------------------------------
310 REM READ ALL 10 LINES
320 REM -----------------------------------
330 OPEN "IF_END_2.TXT" AS 333
340 FOR I = 1 TO 10
350   READ # 333; B$
360   PRINT B$
370 NEXT I
400 REM -----------------------------------
410 REM THE NEXT READ SHOULD CAUSE BRANCH
420 REM -----------------------------------
430 IF END #333 THEN 500
440 PRINT "ABOUT TO EXECUTE TEST"
450 READ # 333; B$
460 PRINT "TEST FAILED - 'IF END' DID NOT EXECUTE"
470 GOTO 999
500 REM -----------------------------------
510 REM END-OF-FILE HANDLER
520 REM -----------------------------------
530 PRINT "TEST PASSED"
540 CLOSE 333
999 END
