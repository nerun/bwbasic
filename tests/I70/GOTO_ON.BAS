  100 REM Purpose: Verify GOTO line [,...] ON expression
  120 REM  Author: Howard Wulf, AF5NE
  130 REM    Date: 2015-12-03
  140 REM    Note: OPTION VERSION SYSTEM-360
  150 REM  
  200 input "X=", X
  210 goto 1100,1200,1300 on X
  220 print "None of the above"
  230 if X < 0 goto 9999
  240 goto 200
 1100 print "ONE",X
 1110 goto 200
 1200 print "TWO",X
 1210 goto 200
 1300 print "THREE",X
 1310 goto 200
 9999 end
