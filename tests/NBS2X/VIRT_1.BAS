  100 rem Purpose: Verify VIRTUAL variables
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2016-04-01
  130 rem    Note: results are tested by VIRT_2.BAS
  140 rem -----------------------------------------
  150 rem
  160 rem
  170 rem
  180 rem
  190 kill "VIRT_1.DAT"
  200 option base 1
  210 dim # 7 , X( 100 )
  220 open "VIRT_1.DAT" for VIRTUAL as # 7
  230 for I = 1 to 100
  240   let X( I ) = I / 10
  250 next I
  260 close # 7
  270 print "TEST COMPLETED"
  999 end


