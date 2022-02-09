  100 REM Purpose: Verify a matrix can be re-dimensioned using MAT ... IDN
  110 REM  Author: Howard Wulf, AF5NE
  120 REM    Date: 2015-12-09
  130 REM
  200 OPTION BASE 1
  210 DIM A(100), B(10,10), C(2,50), D(25,2,2)
 1100 PRINT "----------------------------------------"
 1110 PRINT "MAT varname( ubound [,...] ) = IDN, 2 dimensions"
 1120 MAT A(1,1) = IDN
 1130 MAT B(2,2) = IDN
 1140 MAT C(3,3) = IDN
 1150 MAT D(4,4) = IDN
 1160 MAT PRINT A;B;C;D;
 2100 PRINT "----------------------------------------"
 2110 PRINT "MAT varname = IDN( ubound [,...] ), 2 dimensions"
 2120 MAT A = IDN(4,4)
 2130 MAT B = IDN(3,3)
 2140 MAT C = IDN(2,2)
 2150 MAT D = IDN(1,1)
 2160 MAT PRINT A;B;C;D;
 9999 END  
