010 let correct = 0
100 on error goto 200
110 print 1/0
120 print "*** TEST FAILED #1 ***"
130 goto 299
135 ErrorHandler:
140 print "again"
150 PRINT "*** TEST PASSED ***"
199 goto 299
200 print "ERL="; ERL
210 print "ERR="; ERR
220 if ERL = 110 then 
resume ErrorHandler
end if
230 PRINT "*** TEST FAILED #2 ***"
299 END
