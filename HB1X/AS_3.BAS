  100 rem Purpose: Verify AS ... (function parameter)
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-06-07
  130 rem
  140 def ABC2(X as double, Y as single, Z as long, A as string) as integer
  150 ABC2 = X * 2 + Y * 3 + Z * 4 + VAL(A)
  160 fnend
  170 def DEF3(X as long) as long
  180 DEF3 = X * 3
  190 fnend
  200 def GHI4(X as integer, Y as long) as single
  210 GHI4 = X * 4 + Y * 2
  220 fnend
  230 def JKL5(X as single) as double
  240 JKL5 = X * 5
  250 fnend
  260 def MNO6(X as string, Y as integer) as string
  270 MNO6 = "[" + X + "," + STR$(Y) + "]"
  280 fnend
  300 print ABC2(1,2,3,"4"),DEF3(4),GHI4(6,7),JKL5(8),MNO6("9",10)
  999 end
