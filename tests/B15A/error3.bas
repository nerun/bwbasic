  100 rem Test ERROR( X, A$ )
  105 rem NOTE: ERROR 0 should NOT cause any error whatsoever (it clears the error status)
  110 on error goto 200
  120 for i = 0 to 255
  130   error i, "#" & str$(i)
  134   rem RESUME NEXT on line 210 should have cleared ERR
  135   if ERR <> 0 goto 999
  140 next i
  150 goto 999
  200 print "ERL="; ERL; ", ERR="; ERR; ", ERR$="; ERR$
  202 if ERL <> 130 goto 999
  205 if ERR = 0 goto 999
  210 resume next
  999 end

