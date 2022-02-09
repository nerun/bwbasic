  100 REM Purpose: Verify a matrix can be re-dimensioned using MAT PUT ...
  110 REM  Author: Howard Wulf, AF5NE
  120 REM    Date: 2015-12-09
  130 REM
  200 OPTION BASE 1
  210 DIM A(100), B(10,10), C(2,50), D(25,2,2)
  220 FILE # 12345, "REDIMC.DAT"
  230 SCRATCH # 12345
 1000 PRINT "----------------------------------------"
 1010 PRINT "MAT PUT varname( ubound [,...] ) , 1 dimension"
 1020 MAT A = CON
 1030 MAT B = CON
 1040 MAT C = CON
 1050 MAT D = CON
 1060 MAT PUT # 12345, A(1);B(2);C(3);D(4);
 1100 PRINT "----------------------------------------"
 1110 PRINT "MAT PUT varname( ubound [,...] ) , 2 dimensions"
 1120 MAT A = CON
 1130 MAT B = CON
 1140 MAT C = CON
 1150 MAT D = CON
 1160 MAT PUT # 12345, A(1,1);B(2,2);C(3,3);D(4,4);
 1200 PRINT "----------------------------------------"
 1210 PRINT "MAT PUT varname( ubound [,...] ) , 3 dimensions"
 1220 MAT A = CON
 1230 MAT B = CON
 1240 MAT C = CON
 1250 MAT D = CON
 1260 MAT PUT # 12345, A(1,1,1);B(2,2,2);C(3,3,3);D(4,4,4);
 9999 END  
