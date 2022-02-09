   10 REM Purpose:  Verify 'PUT filename$'
   20 REM  Author:  Howard Wulf, AF5NE
   30 REM    Date:  2015-11-11
   40 REM    Note:  OPTION VERSION SYSTEM-360
   50 REM    Note:  The results are read by GET_1.BAS
   60 REM
  100 REM --- CONSTANT FILE NAME ---
  110 PUT "PUT_1.TXT", 1, 2, 3
  120 PUT "PUT_1.TXT", "hello world"
  130 REM PUT "PUT_1.TXT"
  140 PUT "PUT_1.TXT", "ANOTHER","LINE","IS","HERE"
  150 PUT "PUT_1.TXT", 1, "abc", 2, "def", 3, "ghi"
  200 REM --- VARIABLE FILE NAME ---
  210 LET F$ = "PUT_1.TXT"
  220 PUT F$, 4, 5, 6
  230 PUT F$, "hello world again"
  240 REM PUT F$
  250 PUT F$, "hello world in", f$, "we are", 1
  260 PUT F$, "hello world in", f$, "we are", 2
  270 PUT F$, "hello world in", f$, "we are", 3
  280 PUT F$, "hello world in", f$, "we are", 4
  290 PUT F$, "/*EOF*/"
  300 PRINT "COMPLETED"
  999 END
