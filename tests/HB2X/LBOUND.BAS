  100 REM Purpose:  Verify DIM(), SIZE(), LBOUND() and UBOUND() functions
  110 REM  Author:  Howard Wulf, AF5NE
  120 REM    Date:  2015-09-02
  130 REM    Note:  these functions do not support for scalar variables
  140 REM   
  190 option base 1
  200 dim z, a(5,6,7), y,  b(31), x, d(12,13), w
  210 print dim(a), dim(b), dim(d)
  220 print lbound(a), lbound(a,1), lbound(a,2), lbound(a,3)
  230 print ubound(a), ubound(a,1), ubound(a,2), ubound(a,3)
  240 print lbound(b), lbound(b,1)
  250 print ubound(b), ubound(b,1)
  260 print lbound(d), lbound(d,1), lbound(d,2)
  270 print ubound(d), ubound(d,1), ubound(d,2)
  280 print size(a), size(b), size(d)
  900 print "TEST PASSED"
  999 end
