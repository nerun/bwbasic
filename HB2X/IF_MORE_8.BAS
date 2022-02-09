   10 REM Purpose:  Verify IF MORE # ... syntaxx
   20 REM  Author:  Howard Wulf, AF5NE
   30 REM    Date:  2015-10-21
   40 REM    Note:  OPTION VERSION MARK-I
   50 REM
  100 if more #0 then 120
  110 goto 999
  120 print "OK"
  130 if more #0, then 150
  140 goto 999
  150 print "OK"
  160 if more #0; then 180
  170 goto 999
  180 print "OK"
  190 if more #0: then 210
  200 goto 999
  210 print "OK"
  220 print "TEST PASSED"
  999 end
