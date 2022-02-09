  100 REM Purpose: Verify PRINT USING command, using inline IMAGE string
  110 REM  Author: Howard Wulf, AF5NE
  120 REM    Date: 2015-11-24
  130 REM    Note: OPTION VERSION SYSTEM-360
  140 REM
  200 PRINT USING "###", 123
  210 PRINT USING "###", 12
  220 PRINT USING "###", 1.23
  230 PRINT USING "##.##", 123
  240 PRINT USING "##.##", 1.23
  250 PRINT USING "##.##", 1.23456
  260 PRINT USING "##.##", .123
  270 PRINT USING "##.##", 12.345
  280 PRINT USING "###!!!!", 123
  290 PRINT USING "###!!!!", 12.3
  300 PRINT USING "###!!!!", .1234
  310 PRINT USING "##.#!!!!", 123
  320 PRINT USING "##.#!!!!", 12.3
  330 PRINT USING "##.#!!!!", .1234
  340 PRINT USING "##.#!!!!", 1234
  999 END

