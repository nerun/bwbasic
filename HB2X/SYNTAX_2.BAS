100 REM Purpose: Verify 'Syntax error' with ignore error handler
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2016-01-20
130 REM    Note: this is to verify the WARN_xxx macros
140 REM
200 ON ERROR RESUME NEXT
210 LET A
300 PRINT "ERR=";ERR,"ERL=";ERL
999 END
