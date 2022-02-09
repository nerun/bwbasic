   10 rem Filename: DIMTO.BAS
   20 rem  Purpose: Test DIM( lbound TO ubound ) statement
   30 rem   Author: Howard Wulf, AF5NE
   40 rem     Date: 2017-01-13
   50 rem     Note: OPTION VERSION BYWATER
   60 rem     Note: OPTION BASE 1
   70 rem
   80 rem
   90 rem ------------------------------------------------
  100 dim a$(10 to 20)
  110 print "LBOUND=";LBOUND(A$),"UBOUND=";UBOUND(A$)
  120 rem
  130 print "Line", "Result"
  140 for i = 10 to 20
  150   let a$(i) = "#" & i
  160 next i
  170 for i = 10 to 20
  180   print 180, a$(i)
  190 next i
  200 on error goto 900
  210 rem Verify LBOUND is checked 
  220 print 220, a$(9)
  230 rem Verify UBOUND is checked
  240 print 240, a$(21)
  250 goto 999
  900 rem Error Handler
  910 print "ERL=";ERL,"ERR=";ERR,"ERR$=";ERR$
  920 resume next
  930 rem
  999 end
