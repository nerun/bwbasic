  100 rem Test ERROR( X )
  105 rem NOTE: ERROR 0 should NOT cause any error whatsoever (it clears the error status)
  110 on error goto 200
  120 for i = 0 to 255
  125   rem print "i="; i
  130   error i
  134   rem print "RESUME NEXT should have cleared ERR"; ERR
  135   if ERR <> 0 goto 999
  140 next i
  150 goto 999
  200 print "ERL="; ERL; ", ERR="; ERR; ", ERR$="; ERR$
  202 if ERL <> 130 goto 999
  205 if ERR = 0 goto 999
  210 resume next
  999 end

