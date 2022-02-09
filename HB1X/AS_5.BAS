  100 rem Purpose: Verify AS range
  110 rem  Author: Howard Wulf, AF5NE
  120 rem    Date: 2015-06-07
  130 rem
  140 on error goto 300
  200 def abc( e as integer ) as integer
  210 let abc = e * 10
  220 fnend
  230 print "abc(6000)"
  240 print abc(6000)
  250 print "abc(60000)"
  260 print abc(60000)
  270 goto 999
  300 rem
  310 print "ERL=";ERL;"ERR=";ERR
  320 resume next
  999 end
