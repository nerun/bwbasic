   10 REM Purpose: Verify IF END #... syntax
   20 REM  Author: Howard Wulf, AF5NE
   30 REM    Date: 2015-10-21
   40 REM    Note: OPTION VERSION MARK-I
   50 REM
  100 if end #0 then 999
  110 print "OK"
  120 if end #0, then 999
  130 print "OK"
  140 if end #0; then 999
  150 print "OK"
  160 if end #0: then 999
  170 print "OK"
  180 print "TEST PASSED"
  999 end
