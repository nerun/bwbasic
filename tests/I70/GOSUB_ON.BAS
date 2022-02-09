  100 REM Purpose: Verify GOTO line [,...] ON expression
  120 REM  Author: Howard Wulf, AF5NE
  130 REM    Date: 2015-12-03
  140 REM    Note: OPTION VERSION SYSTEM-360
  150 REM  
  200 input "X=", X
  210 X = INT( X )
  220 gosub 1100,1200,1300 on X
  230 if X >= 1 and  X <= 3 goto 200
  240 print "None of the above"
  250 if X < 0 goto 9999
  260 goto 200
 1100 print "ONE",X
 1110 return
 1200 print "TWO",X
 1210 return
 1300 print "THREE",X
 1310 return
 9999 end
