00100 REM ----------------------------------------------------
00110 REM FILENAME:  lcase1.bas
00120 REM  PURPOSE:  Verify LCASE$(<string>)
00130 REM   AUTHOR:  Howard Wulf AF5NE
00140 REM     DATE:  October 15th, 2014
00150 REM ----------------------------------------------------
01100 let z$ = "ZZZZZ"
01110 let l$ = lcase$(z$)
01120 if l$ <> "zzzzz" then print "LCASE$() FAILED": ERROR 1
01130 print "*** TEST PASSED ***"
09999 QUIT

