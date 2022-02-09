00100 REM ----------------------------------------------------
00110 REM FILENAME:  ceil1.bas
00120 REM  PURPOSE:  Verify CEIL()
00130 REM   AUTHOR:  Howard Wulf AF5NE
00140 REM     DATE:  October 15th, 2014
00150 REM ----------------------------------------------------
01000 REM ----------------------------------------------------
01010 REM TEST POSITIVE VALUE
01020 REM ----------------------------------------------------
01100 let a = 2.1
01110 let c = ceil( a )
01120 print c
01130 if c <> 3 then goto TestFailed
01140 print "*** TEST #1 PASSED ***"
02000 REM ----------------------------------------------------
02010 REM TEST NEGATIVE VALUE
02020 REM ----------------------------------------------------
02100 let a = -2.1
02110 let c = ceil( a )
02120 print c
02130 if c <> -2 then goto TestFailed
02140 print "*** TEST #2 PASSED ***"
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
