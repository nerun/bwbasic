  100 REM Purpose: Verify a matrix can be re-dimensioned using MAT GET ...
  110 REM  Author: Howard Wulf,AF5NE
  120 REM    Date: 2015-12-09
  130 REM
  200 OPTION BASE 1
  210 DIM A(100),B(10,10),C(2,50),D(25,2,2)
 1000 PRINT "----------------------------------------"
 1010 PRINT "MAT GET ...  varname( ubound [,...] ) ,1 dimension"
 1020 MAT GET "REDIM5.DAT",  A(1)
 1030 MAT GET "REDIM5.DAT",  B(2)
 1040 MAT GET "REDIM5.DAT",  C(3)
 1050 MAT GET "REDIM5.DAT",  D(4)
 1060 MAT PRINT A;B;C;D;
 1100 PRINT "----------------------------------------"
 1110 PRINT "MAT GET ...  varname( ubound [,...] ) ,2 dimensions"
 1120 MAT GET "REDIM5.DAT",  A(1,1)
 1130 MAT GET "REDIM5.DAT",  B(2,2)
 1140 MAT GET "REDIM5.DAT",  C(3,3)
 1150 MAT GET "REDIM5.DAT",  D(4,4)
 1160 MAT PRINT A;B;C;D;
 1200 PRINT "----------------------------------------"
 1210 PRINT "MAT GET ...  varname( ubound [,...] ),3 dimensions"
 1220 MAT GET "REDIM5.DAT",  A(1,1,1)
 1230 MAT GET "REDIM5.DAT",  B(2,2,2)
 1240 MAT GET "REDIM5.DAT",  C(3,3,3)
 1250 MAT GET "REDIM5.DAT",  D(4,4,4)
 1260 MAT PRINT A;B;C;D;
 9999 END  
