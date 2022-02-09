100 REM Purpose: Verify OPTION COMMENT char
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-05-01
130 REM    Note: profile.bas must have OPTION COMMENT !
140 REM    Note: The output should be identical to E066.BAS
150 REM
200 DIM X(6) ! comment
210 LET A$ = "SYSTEM" ! comment
220 CHANGE A$ TO X ! comment
230 LET N = X(0) ! comment
240 PRINT "N=";N ! comment
250 FOR I = 1 TO N ! comment
260 PRINT X(I); ! comment
270 NEXT I ! comment
280 PRINT ! comment
290 REM ! comment
300 DIM Y(6) ! comment
310 Y(0) = 6 ! comment
320 Y(1) = ASC("H") ! comment
330 Y(2) = ASC("E") ! comment
340 Y(3) = ASC("L") ! comment
350 Y(4) = ASC("L") ! comment
360 Y(5) = ASC("O") ! comment
370 Y(6) = ASC("!") ! comment
375 B$ = "WORLD." ! comment
380 CHANGE Y TO B$ ! comment
390 PRINT B$ ! comment
400 END ! comment
