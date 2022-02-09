100 REM Purpose: Verify MAT GET filename$
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-11-16
130 REM
200 DIM A(2,2)
210 LET F$ = "MAT_GET_1.TXT"
220 MAT GET F$, A
230 MAT PRINT A
999 END
