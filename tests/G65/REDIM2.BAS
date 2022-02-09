  100 REM Purpose: Verify a matrix can be re-dimensioned using MAT ... ZER
  110 REM  Author: Howard Wulf, AF5NE
  120 REM    Date: 2015-12-09
  130 REM
  200 OPTION BASE 1
  210 DIM A(100), B(10,10), C(2,50), D(25,2,2)
 1000 PRINT "----------------------------------------"
 1010 PRINT "MAT varname( ubound [,...] ) = ZER, 1 dimension"
 1020 MAT A(1) = ZER
 1030 MAT B(2) = ZER
 1040 MAT C(3) = ZER
 1050 MAT D(4) = ZER
 1060 MAT PRINT A;B;C;D;
 1100 PRINT "----------------------------------------"
 1110 PRINT "MAT varname( ubound [,...] ) = ZER, 2 dimensions"
 1120 MAT A(1,1) = ZER
 1130 MAT B(2,2) = ZER
 1140 MAT C(3,3) = ZER
 1150 MAT D(4,4) = ZER
 1160 MAT PRINT A;B;C;D;
 1200 PRINT "----------------------------------------"
 1210 PRINT "MAT varname( ubound [,...] ) = ZER, 3 dimensions"
 1220 MAT A(1,1,1) = ZER
 1230 MAT B(2,2,2) = ZER
 1240 MAT C(3,3,3) = ZER
 1250 MAT D(4,4,4) = ZER
 1260 MAT PRINT A;B;C;D;
 2000 PRINT "----------------------------------------"
 2010 PRINT "MAT varname = ZER( ubound [,...] ), 1 dimension"
 2020 MAT A = ZER(4)
 2030 MAT B = ZER(3)
 2040 MAT C = ZER(2)
 2050 MAT D = ZER(1)
 2060 MAT PRINT A;B;C;D;
 2100 PRINT "----------------------------------------"
 2110 PRINT "MAT varname = ZER( ubound [,...] ), 2 dimensions"
 2120 MAT A = ZER(4,4)
 2130 MAT B = ZER(3,3)
 2140 MAT C = ZER(2,2)
 2150 MAT D = ZER(1,1)
 2160 MAT PRINT A;B;C;D;
 2200 PRINT "----------------------------------------"
 2210 PRINT "MAT varname = ZER( ubound [,...] ), 3 dimensions"
 2220 MAT A = ZER(4,4,4)
 2230 MAT B = ZER(3,3,3)
 2240 MAT C = ZER(2,2,2)
 2250 MAT D = ZER(1,1,1)
 2260 MAT PRINT A;B;C;D;
 9999 END  
