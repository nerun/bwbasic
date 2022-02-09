0010 REM -------------------------
0020 REM Filename: bug3.bas
0030 REM  Purpose: Verify bug #3
0040 REM -------------------------
0100 REM #3 Nested DO LOOP 
0105 REM ERROR in line 1280: in bwb_loop(): exec stack code != EXEC_DO
0110 REM -------------------------
0120 REM Status: open
0130 REM Owner: nobody 
0140 REM Labels: None 
0150 REM Priority: 5 
0160 REM Updated: 2009-12-28 
0170 REM Created: 2009-12-28 
0180 REM Creator:Ahmad Hawwash
0190 REM Private: No 
0200 REM -------------------------
1000 DO
1010   T = 1
1020   PRINT "Guess a Number 1-100"
1030   N = INT(RND(1) * 100) + 1
N = 40
1040   PRINT N
1050   DO
1060     PRINT "Trial "; T
1070     INPUT G
1080     IF G = N THEN
1090       PRINT "Correct"
1100       PRINT "You Won!"
1110       EXIT DO
1120     ELSE
1130       PRINT "Wrong"
1140       IF T = 10 THEN
1150         PRINT "You Lose!"
1160         EXIT DO
1170       ELSE
1180         IF N < G THEN
1190           PRINT "Try Smaller"
1200         ELSE
1210           PRINT "TRY Bigger"
1220         END IF
1230       END IF
1240     END IF
1250     T = T + 1
1260   LOOP
1270   INPUT "Do you want to play again";A$
1280 LOOP UNTIL A$<>"Y" AND A$<>"y"
8000 rem ------------------------------------------
8010 TestPassed:
8020 print "*** ALL TESTS PASSED ***"
8030 goto TheEnd
8500 rem ------------------------------------------
8510 TestFailed:
8520 print "*** TEST FAILED ***"
8530 goto TheEnd
9000 rem ------------------------------------------
9010 TheEnd:
9999 end
