  100 REM PURPOSE: VERIFY 'LPT' COMMAND
  110 REM  AUTHOR: HOWARD WULF, AF5NE
  120 REM    DATE: 2017-03-22
  130 REM    NOTE: OPTION VERSION "PDP-8"
  140 REM
  200 REM ----------------------------------
  205 REM verified by PTR_3.BAS
  210 LPT
  220 PRINT "LPT LINE 1"
  230 PRINT "LPT LINE 2"
  240 PRINT "LPT LINE 3"
  300 REM ----------------------------------
  310 TTY OUT
  320 PRINT "TTY OUT LINE 1"
  400 REM ----------------------------------
  405 REM verified by PTR_4.BAS
  410 PTP "LPT.OUT"
  420 PRINT "LPT.OUT  LINE 1"
  430 PRINT "LPT.OUT  LINE 2"
  440 PRINT "LPT.OUT  LINE 3"
  500 REM ----------------------------------
  510 TTY
  520 PRINT "TTY  LINE 1"
  999 END
