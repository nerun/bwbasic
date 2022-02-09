100 REM Purpose: Verify OPTION STATEMENT char
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-05-01
130 REM    Note: profile.bas must have OPTION STATEMENT \
140 REM    Note: The output should be identical to E066.BAS
150 REM
200 DIM X(6)\LET A$ = "SYSTEM"\CHANGE A$ TO X\LET N = X(0)\PRINT "N=";N\FOR I = 1 TO N\PRINT X(I);\NEXT I\PRINT
290 REM
300 DIM Y(6)\Y(0) = 6\Y(1) = ASC("H")\Y(2) = ASC("E")\Y(3) = ASC("L")\Y(4) = ASC("L")\Y(5) = ASC("O")\Y(6) = ASC("!")\B$ = "WORLD."\CHANGE Y TO B$\PRINT B$\END
