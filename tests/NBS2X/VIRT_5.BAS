  100 rem Purpose: Verify VIRTUAL variables
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2016-04-01
  130 rem    Note: values are verified by VIRT_6.BAS
  140 rem ------------------------------------------
  150 rem
  160 rem
  170 rem
  180 rem
  190 kill "VIRT_5.DAT"
  200 option base 1
  210 dim # 7 , X( 10 ), A$( 15 )
  220 open "VIRT_5.DAT" for VIRTUAL as # 7
  230 for I = 1 to 10 : let X( I ) = I / 10 : let A$( I ) = STR$( I  ) : next I
  240 rem 
  250 for I = 11 to 15 : let A$( I ) = STR$( I  ) : next I
  260 close # 7
  270 print "TEST COMPLETED"
  999 end

