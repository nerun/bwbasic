100 REM Purpose: Verify COMMAND$(0-9)
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-10-08
130 REM   Usage: ~/bwbasic F_COMMAND5_X_S.BAS a b c "d e f"
200 FOR I = 0 TO 9
210  PRINT I, COMMAND$(I)
220 NEXT I
999 END
