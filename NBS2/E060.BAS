100 REM Purpose: Verify FREEFILE
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-02-19
130 REM
200 for i = 1 to maxdev
202 rem next value should be 1,2,3,...
205 print freefile
210 open "A" for output as #i
220 next i
225 REM next value should be zero
230 print freefile
