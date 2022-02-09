100 REM Purpose: Verify FREEFILE
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-02-19
130 REM
190 print "maxdev=";maxdev
200 for i = 1 to maxdev
210 print "These should match:";i;freefile
220 open "A" for output as #i
230 next i
240 print "This should be zero:";freefile
999 end
