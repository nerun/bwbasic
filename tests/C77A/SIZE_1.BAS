100 REM Purpose: Verify SIZE function
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-12-23
130 REM

200 REM -----------------------------------------------------
210 LET File.Name$ = "SIZE_1.TXT"
220 REM -----------------------------------------------------

1100 REM -----------------------------------------------------
1110 PRINT "SIZE() of non-existing file is zero"
1120 LET N = SIZE( File.Name$ )
1130 PRINT "N=";N
1140 IF N <> 0 THEN 2000
1150 PRINT "  TEST PASSED"

1200 REM -----------------------------------------------------
1210 PRINT "SIZE() of zero length file is zero"
1220 CREATE File.Name$ AS 1200
1230 CLOSE 1200
1240 LET N = SIZE( File.Name$ )
1250 PRINT "N=";N
1260 IF N <> 0 THEN 2000
1270 PRINT " TEST PASSED"

1300 REM -----------------------------------------------------
1310 PRINT "SIZE() of < 1024 length file is 1"
1320 CREATE File.Name$ AS 1300
1330 PRINT # 1300; "anything"
1340 CLOSE 1300
1350 LET N = SIZE( File.Name$ )
1360 PRINT "N=";N
1370 IF N <> 1 THEN 2000
1380 PRINT " TEST PASSED"

1400 REM -----------------------------------------------------
1410 PRINT "SIZE() of 1024 length file is 1"
1420 CREATE File.Name$ RECL 1024 AS 1400
1430 PRINT # 1400; "anything"
1440 CLOSE 1400
1450 LET N = SIZE( File.Name$ )
1460 PRINT "N=";N
1470 IF N <> 1 THEN 2000
1480 PRINT " TEST PASSED"

1900 REM -----------------------------------------------------
1910 REM Cleanup
1920 CREATE File.Name$ AS 1900
1930 DELETE 1900
1999 GOTO 9999

2000 REM -----------------------------------------------------
2010 PRINT "TEST FAILED"

9999 END
