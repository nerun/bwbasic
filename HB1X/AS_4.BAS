  100 rem Purpose: Verify AS INTEGER rounds
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-06-07
  130 rem
  200 def ABC(X) as integer
  210 let ABC = X
  220 fnend
  230 print abc(1),abc(1.4),abc(1.6)
  300 def DDD(X as integer)
  310 let DDD = X
  320 fnend
  330 print ddd(1),ddd(1.4),ddd(1.6)

