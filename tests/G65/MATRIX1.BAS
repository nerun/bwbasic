   10 DIM A(20,20), B(20,20), C(20,20)
   20 READ M,N
   30 MAT READ A(M,N), B(N,N)
   40 MAT C = ZER(M,N)
  100 MAT C = A+A
  120 MAT PRINT C;
  130 PRINT "A="
  131 MAT PRINT A;
  132 PRINT
  135 PRINT "B="
  136 MAT PRINT B;
  137 PRINT  
  140 MAT C = A*B
  150 PRINT "A * B ="
  160 PRINT
  170 MAT PRINT C
  190 DATA 1,2
  191 DATA 1,2,3
  192 DATA 4,5,6
  193 DATA 1,0,-1
  194 DATA 0,-1,-1
  195 DATA -1,0,0
  199 END
 9900 REM ------------------------------------------------------------------------
 9910 REM Purpose: Example program
 9920 REM  Author: General Electric
 9930 REM    Date: 1965-12-20
 9940 REM    Note: OPTION VERSION MARK-I
 9950 REM  Origin: 202026A_BASIC_Language_Reference_Manual_Jan1967.pdf
 9960 REM    Page: 33
 9970 REM ------------------------------------------------------------------------

