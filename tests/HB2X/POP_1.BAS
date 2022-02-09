  100 rem
  110 gosub 200
  120 print "OK"
  130 goto 999
  200 gosub 300
  210 print "ERROR"
  220 goto 999
  300 print "about"
  310 pop
  320 return
  330 print "FAILED"
  999 end

