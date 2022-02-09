10 REM Purpose:  Verify 'SCRATCH #'
20 REM  Author:  Howard Wulf, AF5NE
30 REM    Date:  2015-10-19
40 REM
50 OPEN "A" FOR INPUT AS #2
60 REM The following statement should change the file from INPUT to OUTPUT
70 SCRATCH #2
80 PRINT #2, "100 REM HELLO WORLD"
90 CLOSE #2
99 END
