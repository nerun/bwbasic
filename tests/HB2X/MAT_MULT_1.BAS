  100 rem Purpose: Verify MAT A = B * C
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-06-29
  130 rem
  140 option base 1
  150 dim A(2,2), B(2,3), C(3,2)
  200 mat read B
  210 mat print B
  220 mat read C
  230 mat print C
  240 mat A = B * C
  250 mat print A
  300 data 1,2,3
  310 data 4,5,0
  320 data 1,4
  330 data 0,5
  340 data 3,0
  999 end

