  100 rem Filename: MATTO.BAS
  110 rem  Purpose: Verify MAT ... ( lbound TO ubound ) statements
  120 rem   Author: Howard Wulf, AF5NE
  130 rem     Date: 2017-01-13
  140 rem     Note: OPTION VERSION BYWATER
  150 rem     Note: OPTION ENABLE COMMAND "MAT"
  160 rem     Note: OPTION BASE 1
  170 rem
  180 rem
  190 rem ------------------------------------------------------------
  200 on error goto 900
  210 dim A(10)
  220 gosub 800
  230 rem
  240 MAT A(5)=ZER
  250 gosub 800
  260 rem
  270 MAT A=ZER(9)
  280 gosub 800
  290 MAT A(2 TO 7)=ZER
  300 gosub 800
  310 MAT A=ZER(21 TO 30)
  320 gosub 800
  330 goto 999
  800 print "LBOUND=";LBOUND(A),"UBOUND=";UBOUND(A)
  810 return
  820 rem
  900 print "ERL=";ERL,"ERR=";ERR,"ERR$=";ERR$
  910 resume next
  920 rem
  999 end
