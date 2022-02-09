  100 rem Purpose: Verify AS ... (function return type)
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-06-07
  130 rem
  140 def ABC2(X) as integer
  150 ABC2 = X * 2
  160 fnend
  170 def DEF3(X) as long
  180 DEF3 = X * 3
  190 fnend
  200 def GHI4(X) as single
  210 GHI4 = X * 4
  220 fnend
  230 def JKL5(X) as double
  240 JKL5 = X * 5
  250 fnend
  260 def MNO6(X) as string
  270  MNO6 = "[" + STR$(X) + "]"
  280 fnend
  300 print ABC2(1),DEF3(2),GHI4(3),JKL5(4),MNO6(5)
  999 end
