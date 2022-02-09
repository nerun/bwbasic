100 REM Purpose: Verify CONSOLE command
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-12-22
130 REM
200 REM The following string is 70 characters long
210 LET A$ =  "....+....|....+....|....+....|....+....|....+....|....+....|....+....|"
300 CONSOLE
310 PRINT A$
320 CONSOLE WIDTH 70
330 PRINT A$
340 CONSOLE WIDTH 40
350 PRINT A$
360 CONSOLE WIDTH 80
370 PRINT A$
999 END
