  100 REM Purpose: Verify a matrix can be re-dimensioned using MAT ... CON
  110 REM  Author: Howard Wulf, AF5NE
  120 REM    Date: 2015-12-09
  130 REM
  200 OPTION BASE 1
  210 DIM A(100), B(10,10), C(2,50), D(25,2,2)
 1000 PRINT "----------------------------------------"
 1010 PRINT "MAT varname( ubound [,...] ) = CON, 1 dimension"
 1020 MAT A(1) = CON
 1030 MAT B(2) = CON
 1040 MAT C(3) = CON
 1050 MAT D(4) = CON
 1060 MAT PRINT A;B;C;D;
 1100 PRINT "----------------------------------------"
 1110 PRINT "MAT varname( ubound [,...] ) = CON, 2 dimensions"
 1120 MAT A(1,1) = CON
 1130 MAT B(2,2) = CON
 1140 MAT C(3,3) = CON
 1150 MAT D(4,4) = CON
 1160 MAT PRINT A;B;C;D;
 1200 PRINT "----------------------------------------"
 1210 PRINT "MAT varname( ubound [,...] ) = CON, 3 dimensions"
 1220 MAT A(1,1,1) = CON
 1230 MAT B(2,2,2) = CON
 1240 MAT C(3,3,3) = CON
 1250 MAT D(4,4,4) = CON
 1260 MAT PRINT A;B;C;D;
 2000 PRINT "----------------------------------------"
 2010 PRINT "MAT varname = CON( ubound [,...] ), 1 dimension"
 2020 MAT A = CON(4)
 2030 MAT B = CON(3)
 2040 MAT C = CON(2)
 2050 MAT D = CON(1)
 2060 MAT PRINT A;B;C;D;
 2100 PRINT "----------------------------------------"
 2110 PRINT "MAT varname = CON( ubound [,...] ), 2 dimensions"
 2120 MAT A = CON(4,4)
 2130 MAT B = CON(3,3)
 2140 MAT C = CON(2,2)
 2150 MAT D = CON(1,1)
 2160 MAT PRINT A;B;C;D;
 2200 PRINT "----------------------------------------"
 2210 PRINT "MAT varname = CON( ubound [,...] ), 3 dimensions"
 2220 MAT A = CON(4,4,4)
 2230 MAT B = CON(3,3,3)
 2240 MAT C = CON(2,2,2)
 2250 MAT D = CON(1,1,1)
 2260 MAT PRINT A;B;C;D;
 9999 END  
