 5 OPTION BASE 1
10 DIM A(20,20),B(20,20),C(20,20)
20 READ N
30 MAT A= CON(N,N)
40 MAT B=CON(N,N)
45 MAT C = ZER(N,N)
50 FOR I= 1 TO N
60 FOR J = 1 TO N
70 LET A(I,J) = 1/(I+J-1)
80 NEXT J
90 NEXT I
93 PRINT " HILBERT MATRIX 0F 0RDER "N
95 MAT PRINT A,
100 MAT B = INV(A)
105 PRINT"INVERSE 0F HIL8ERT MATRIX 0F 0RDER "N
110 MAT PRINT B,
115 MAT C=A*B
117 PRINT"HILBERT MATRIX TIMES ITS 0WN INVERSE 0RDER "N
118 MAT PRINT C,
120 GOTO 20
190 DATA 2,3 
1999 END
9900 REM ------------------------------------------------------------------------
9910 REM Purpose: HILTST example 
9920 REM  Author: General Electric
9930 REM    Date: 1965-12-17
9940 REM    Note: OPTION VERSION MARK-I
9950 REM  Origin: 802207A_Time-SharingServiceBASICLanguageExtensionsReferenceManual_Feb1968.pdf
9960 REM    Page: 8
9970 REM ------------------------------------------------------------------------
