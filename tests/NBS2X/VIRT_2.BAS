  100 rem Purpose: Verify VIRTUAL variables
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2016-04-01
  130 rem    Note: verify values written by VIRT_1.BAS
  140 rem --------------------------------------------
  150 rem    Note: should print 1..100
  160 rem
  170 rem
  180 rem
  190 rem
  200 option base 1
  210 dim # 11 , Y( 100 )
  220 open "VIRT_1.DAT" for VIRTUAL as # 11
  230 for I = 1 to 100
  240   print Y( I ) * 10
  250 next I
  260 close # 11
  270 print "TEST COMPLETED"
  999 end


