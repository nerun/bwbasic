   10 REM Purpose:  Verify 'PUT filename$' with RESET
   20 REM  Author:  Howard Wulf, AF5NE
   30 REM    Date:  2015-11-11
   40 REM    Note:  OPTION VERSION SYSTEM-360
   50 REM    Note:  The results are read by GET_2.BAS
   60 REM
   70 GOSUB 100
   75 RESET "PUT_2.TXT"
   80 GOSUB 100
   85 RESET "not an open file", "", "*", "?", "+", ":", "PUT_2.TXT"
   90 GOSUB 100
   95 GOTO 999
  100 REM --- CONSTANT FILE NAME ---
  110 PUT "PUT_2.TXT", 1, 2, 3
  120 PUT "PUT_2.TXT", "hello world"
  130 REM PUT "PUT_2.TXT"
  140 PUT "PUT_2.TXT", "ANOTHER","LINE","IS","HERE"
  150 PUT "PUT_2.TXT", 1, "abc", 2, "def", 3, "ghi"
  200 REM --- VARIABLE FILE NAME ---
  210 LET F$ = "PUT_2.TXT"
  220 PUT F$, 4, 5, 6
  230 PUT F$, "hello world again"
  240 REM PUT F$
  250 PUT F$, "hello world in", f$, "we are", 1
  260 PUT F$, "hello world in", f$, "we are", 2
  270 PUT F$, "hello world in", f$, "we are", 3
  280 PUT F$, "hello world in", f$, "we are", 4
  290 PUT F$, "/*EOF*/"
  300 PRINT "COMPLETED"
  399 RETURN
  999 END
