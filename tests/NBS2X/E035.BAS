00100 REM ----------------------------------------------------
00110 REM FILENAME:  ucase1.bas
00120 REM  PURPOSE:  Verify UCASE$(<string>)
00130 REM   AUTHOR:  Howard Wulf AF5NE
00140 REM     DATE:  October 15th, 2014
00150 REM ----------------------------------------------------
01100 let a$ = "aaaaa"
01110 let u$ = ucase$(a$)
01120 if u$ <> "AAAAA" then print "UCASE$() FAILED": ERROR 1
01130 print "*** TEST PASSED ***"
09999 QUIT

