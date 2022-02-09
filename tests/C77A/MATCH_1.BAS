100 REM Purpose: Verify MATCH command
110 REM  Author: Howard Wulf, AF5NE
120 REM    Date: 2015-12-25
130 REM


1000 REM --------------------------------------
1010 READ Test.Case.Number
1020 IF Test.Case.Number < 0 THEN 2000
1030 READ A$, B$, I%, N%
1040 PRINT "TEST CASE #" ; Test.Case.Number ; ": "; 
1050 PRINT "A$=[";A$;"], B$=[";B$;"], I%=";I%;", N%=";N%
1060 J% = MATCH( A$, B$, I% )
1070 IF J% = N% THEN 1000

1100 PRINT "TEST FAILED"
1110 GOTO 9999

2000 PRINT "TEST PASSED"
2010 GOTO 9999

8000 REM  Test #, A$, B$, I%, N
8010 DATA 1, "is", "Now is the", 1, 5
8020 DATA 2, "##", "October 8, 1876", 1, 12
8030 DATA 3, "a?", "character", 4, 5
8040 DATA 4, "\#", "123#45", 1, 4
8050 DATA 5, "ABCD", "ABC", 1, 0
8060 DATA 6, "\#1\\\?", "1#1\?2#", 1, 2
8070 DATA 7, " ", " ", 2, 0
8080 DATA 8, " ", "", 1, 0
8090 DATA 9, "", " ", 1, 1
8100 DATA 10, "", "", 1, 0
8110 DATA -1

9999 END

