  100 rem Purpose:  Verify OPTION BASE for NEGATIVE lower bounds
  110 rem  Author:  Howard Wulf, AF5NE
  120 rem    Date:  2015-09-03
  130 rem    Note:  OPTION VERSION MARK-I
  140 rem
  200 rem
  210 let L = -11
  220 let U = 12
  230 option base L
  240 dim A( U )
  250 for I = L TO U
  260   let A( I ) = I * 10
  270 next I
  280 mat print A,
  290 rem
  300 let A$="ABCD"
  310 change A$ to A
  320 print "len(A$)=";A( L )
  330 for I = L + 1 to L + A( L )
  340   print A( I ), chr$( A( I ) )
  350 next I
  999 end

