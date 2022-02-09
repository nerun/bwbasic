100 rem
110 on error goto 800
120 label1:
130 error 1
140 label2:
150 stop
800 rem
810 print "ERL=";ERL,"ERR$=";ERR$
820 print "label1=";label1
830 print "label2=";label2
840 if ERL > label1 AND ERL < label2 THEN
850 print "*** TEST PASSED ***"
860 goto 999
870 end if
880 stop
999 end
