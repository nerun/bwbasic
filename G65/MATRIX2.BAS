   10 DIM A(20,20), B(20,20)
   20 READ N
   30 MAT A = CON(N,N)
   40 MAT B = CON(N,N)
   50 FOR I = 0 TO N
   60   FOR J = 0 TO N
   70     LET A(I,J) = 1/(I+J+1)
   80   NEXT J
   90 NEXT I
  100 MAT B = INV( A )
  120 MAT PRINT B,
  190 DATA 3
  199 END
 9900 REM ------------------------------------------------------------------------
 9910 REM Purpose: Example program
 9920 REM  Author: General Electric
 9930 REM    Date: 1965-12-20
 9940 REM    Note: OPTION VERSION MARK-I
 9950 REM  Origin: 202026A_BASIC_Language_Reference_Manual_Jan1967.pdf
 9960 REM    Page: 34
 9970 REM ------------------------------------------------------------------------

