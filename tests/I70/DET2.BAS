  100 rem Purpose: Verify DET function
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-12-05
  130 rem    Note: OPTION VERSION SYSTEM-370
  140 rem
  200 option base 1
  210 dim A(2,2), B(2,2)
  220 mat read A
  230 mat print A
  500 REM determinant of last inverted matrix
  510 mat B = inv( A )
  520 let D = DET
  530 print "DET     =";D
  800 data 1,2,3,4
  999 END

