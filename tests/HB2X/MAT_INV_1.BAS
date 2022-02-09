  100 rem Purpose: Verify MAT A = INV B
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-06-29
  130 rem
  140 option base 1
  150 dim A(3,3), B(3,3)
  160 data 1,2,3
  170 data 1,4,6
  180 data 1,8,9
  190 rem
  200 mat read B
  210 mat print B
  220 mat A = inv( B )
  230 mat print A
  240 print "DET=";det
  250 dim C(3,3)
  260 mat C = (2) * A
  270 mat print C
  280 mat C = (2 + 4/4 - 5/5)*A
  290 mat print C
  999 end

