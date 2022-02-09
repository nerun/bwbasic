  100 rem Purpose: Verify DET( matrix ) function
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-12-05
  130 rem    Note: OPTION VERSION SYSTEM-370
  140 rem
  200 option base 1
  210 dim A(2,2)
  220 mat read A
  230 mat print A
  300 REM determinant of a specified matrix
  310 let C = DET( A )
  320 print "DET( A )=";C
  400 REM last determinant calculated
  410 let D = DET
  420 print "DET     =";D
  800 data 1,2,3,4
  999 END

