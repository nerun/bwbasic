  100 REM PURPOSE: VERIFY 'REPEAT-UNTIL' STRUCTURE
  110 REM  AUTHOR; HOWARD WULF, AF5NE
  120 REM    DATE: 2017-04-03
  130 REM    NOTE: OPTION VERSION "HAARDT"
  140 REM
  150 REM
  190 REM -----------------------------------------
  195 PRINT "TEST #1 - TEN TIMES"
  197 REM verify loop exits when result != 0
  200 repeat
  210   inc i
  220   print i
  230 until i = 10
  240 REM -----------------------------------------
  245 PRINT "TEST #2 - FIVE TIMES"
  247 REM verify EXIT REPEAT does exit the loop
  250 REPEAT
  260   INC J
  270   IF J > 5 THEN
    :     EXIT REPEAT
    :   END IF
  280   PRINT J
  290 UNTIL FALSE
  300 REM -----------------------------------------
  305 PRINT "TEST #3 - ONE TIME"
  307 REM verify loop executes once
  310 REPEAT
  320   INC K
  330   PRINT K
  340 UNTIL TRUE
  350 REM
  360 REM -----------------------------------------
  370 PRINT "TEST #4 - NO OUTPUT"
  375 REM verify no stmts between REPEAT and UNTIL
  380 REPEAT
    : UNTIL TRUE
  390 REM -----------------------------------------
  400 PRINT "TESTS COMPLETE"
  410 REM
  999 END

