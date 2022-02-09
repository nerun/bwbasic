 5 OPTION BASE 1
10 DIM A(5,5),B(5,5),C(5,5),D(5,5),E(5,5),F(5,5),G(5,5),H(5,5)
20 READ M,N
30 MAT READ A(M,M)
31 MAT PRINT A,
32 MAT READ B(M,M)
34 MAT READ D(M,N)
36 MAT READ G(N,M)
40 MAT C=ZER(M,M)
50 PRINT "MATRIX A 0F 0RDER "M
60 MAT PRINT A;
70 PRINT "MATRIX B 0F 0RDER "M
80 MAT PRINT B;
90 MAT C=A+B
100 PRINT "      C=A+B"
110 MAT PRINT C;
120 MAT F=ZER(M,N)
130 MAT F=C*D
140 MAT H=ZER(N,N)
150 MAT H=G*F
160 PRINT " H "
170 MAT PRINT H;
1800 DATA 3,1
1810 DATA 1,2,3,4,5,6,7,8,9,9,8,7,6,5,4,3,2,1,1,2,3,3,2,1
1999 END
9900 REM ------------------------------------------------------------------------
9910 REM Purpose: SAMPLE example 
9920 REM  Author: General Electric
9930 REM    Date: 1965-12-17
9940 REM    Note: OPTION VERSION MARK-I
9950 REM  Origin: 802207A_Time-SharingServiceBASICLanguageExtensionsReferenceManual_Feb1968.pdf
9960 REM    Page: 7
9970 REM ------------------------------------------------------------------------
