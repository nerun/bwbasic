  100 rem test
  110 on error goto 200
  120 for x == 1 to 10
  130   print x
  140 next x
  150 goto 999
  200 print "ERL="; ERL; ",ERR="; ERR; ", ERR$="; ERR$
  999 end
