010 REM OPTION VERSION BYWATER
020 REM
100 REM PAD80.BAS
101 REM Pad the test output to 80 characters.
102 REM   bwbasic P001.BAS > P001.OUT
103 REM   echo "500 DATA P001.OUT, P001.80" > PAD80.INP
104 REM   bwbasic PAD80.BAS
105 REM   diff P001.run P001.80
109 REM ----------------------------------------------------------
120 REM GET FILE NAME INTO A$
122 DELETE 500
124 MERGE "PAD80.INP"
126 REM LIST
128 RESTORE 500
130 READ A$
135 REM PRINT "SOURCE:"; A$
140 READ B$
145 REM PRINT "TARGET:"; B$
300 REM ----------------------------------------------------------
301 REM PROCESS FILENAME IN A$
302 REM ----------------------------------------------------------
310 OPEN A$ FOR INPUT  AS #2
315 OPEN B$ FOR OUTPUT AS #3
320 REM PRINT "PROCESS TEXT LINE"
330 IF EOF( 2 ) THEN 390
335 REM PRINT "NOT EOF"
340 LINE INPUT #2, C$
345 REM PRINT "C$=";C$
350 C$ = LEFT$( C$ + SPACE$( 80 ), 80 )
355 REM PRINT "yyy"
360 REM WRITE OUTPUT LINE
370 PRINT #3, C$
375 REM PRINT "zzz"
380 GOTO 320
390 REM CLOSE FILES
400 CLOSE #3
410 CLOSE #2
499 REM ----------------------------------------------------------
500 REM REPLACED BY CONTENTS OF "PAD80.INP"
501 REM ----------------------------------------------------------
900 REM ----------------------------------------------------------
910 REM THE END
920 REM ----------------------------------------------------------
999 END
