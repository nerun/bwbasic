00100 REM ----------------------------------------------------
00110 REM FILENAME:  delete2.bas
00120 REM  PURPOSE:  Verify DELETE <non-existing-line-number>
00130 REM   AUTHOR:  Howard Wulf AF5NE
00140 REM     DATE:  October 14th, 2014
00150 REM ----------------------------------------------------
01000 PRINT "LINE 02000 SHOULD CAUSE A FATAL ERROR"
02000 DELETE 3000
09999 END
