100 REM Purpose: Verify READ LINE # command
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-12-23
130 REM
180 LET Record.Length% = 64
190 LET File.Name$ = "READ_1.TXT"
200 CREATE File.Name$ RECL Record.Length% AS 777
210 FOR I = 1 TO 10
220   LET A$ = "THIS IS LINE"
230   PRINT # 777; A$, I,
235   PRINT # 777; "extra stuff", I * 10 + I
240 NEXT I
250 CLOSE 777
300 OPEN File.Name$ RECL Record.Length% AS 333
310 FOR I = 1 TO 10
320   READ # 333; LINE B$
330   PRINT B$
340 NEXT I
350 CLOSE 333
999 END
