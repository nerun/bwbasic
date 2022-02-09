00100 REM ----------------------------------------------------
00110 REM FILENAME:  angle1.bas
00120 REM  PURPOSE:  Verify ANGLE()
00130 REM   AUTHOR:  Howard Wulf AF5NE
00140 REM     DATE:  October 15th, 2014
00150 REM ----------------------------------------------------
01000 REM ----------------------------------------------------
01010 REM TEST DEFAULT
01020 REM ----------------------------------------------------
01100 REM option angle radians 
01110 let x = 1
01120 let y = 1
01130 let a = angle( x, y )
01140 print a
01150 let a$ = str$( a )
01160 if left$( a$, 9 ) <> " .785398" then goto TestFailed
01170 print "*** TEST #1 PASSED ***"
02000 REM ----------------------------------------------------
02010 REM TEST DEGREES
02020 REM ----------------------------------------------------
02100 option angle degrees
02110 let x = 1
02120 let y = 1
02130 let a = angle( x, y )
02140 print a
02150 let a$ = str$( a )
02160 if left$( a$, 3 ) <> " 45" then goto TestFailed
02170 print "*** TEST #2 PASSED ***"
03000 REM ----------------------------------------------------
03010 REM TEST RADIANS
03020 REM ----------------------------------------------------
03100 option angle radians
03110 let x = 1
03120 let y = 1
03130 let a = angle( x, y )
03140 print a
03150 let a$ = str$( a )
03160 if left$( a$, 9 ) <> " .785398" then goto TestFailed
03170 print "*** TEST #3 PASSED ***"
8000 rem ------------------------------------------
8010 TestPassed:
8020 print "*** ALL TESTS PASSED ***"
8030 goto TheEnd
8500 rem ------------------------------------------
8510 TestFailed:
8520 print "*** TEST FAILED ***"
8530 goto TheEnd
9000 rem ------------------------------------------
9010 TheEnd:
9999 end
