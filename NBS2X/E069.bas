rem Purpose: verify comment character is honored for all commands
rem  Author: Howard Wulf, AF5NE
rem    Date: 2015-05-01
rem   Usage: implementation defined
rem Example: ~/bwbasic E069.bas
rem ---------------------------------------------
rem
rem cmds
clear ' comment
DEFDBL Z ' comment
DEFINT I ' comment
DEFSNG Y ' comment
DEFSTR A ' comment
COMMON   z ' comment
LET TRUE = -1 ' comment
LET FALSE = 0 ' comment
DATA  a , b , c ' comment
? 1 ' COMMENT
RESTORE ' comment
read a$,b$,c$ ' comment
print a$,b$,c$ ' comment
print len(a$), len(b$), len(c$)
call chr$(0) ' comment
SELECT CASE    5 ' comment
CASE IF = 1 ' comment
CASE IS = 2 ' comment
CASE 3 ' comment
CASE ELSE ' comment
END SELECT ' comment
dim x(10) ' comment
a$ = "1234567890" ' comment
change a$ to x  ' comment
change x to a$  ' comment
' CMDS         ' comment   

I = 0 ' comment
DO ' comment
I = I + 1 ' comment
IF I > 2 THEN ' comment
EXIT DO ' comment
ELSE ' comment
PRINT "I=";I ' comment
END IF ' comment
LOOP ' comment

I = 0 ' comment
DO WHILE TRUE ' comment
I = I + 1 ' comment
IF I > 2 THEN ' comment
EXIT DO ' comment
ELSE ' comment
PRINT "I=";I ' comment
END IF ' comment
LOOP WHILE TRUE ' comment

I = 0 ' comment
DO UNTIL FALSE ' comment
I = I + 1 ' comment
IF I > 2 THEN ' comment
EXIT DO ' comment
ELSE ' comment
PRINT "I=";I ' comment
END IF ' comment
LOOP UNTIL FALSE ' comment

I = 0 ' comment
UNTIL FALSE ' comment
I = I + 1 ' comment
IF I > 2 THEN ' comment
EXIT UNTIL ' comment
ELSE ' comment
PRINT "I=";I ' comment
END IF ' comment
UEND ' comment

I = 0 ' comment
WHILE TRUE ' comment
I = I + 1 ' comment
IF I > 2 THEN ' comment
EXIT WHILE ' comment
ELSE ' comment
PRINT "I=";I ' comment
END IF ' comment
WEND ' comment

FOR I = 1 TO 3 ' comment
IF I > 2 THEN ' comment
EXIT FOR ' comment
END IF ' comment
NEXT I ' comment

I = 3 + 4 ' comment
IF I = 2 THEN ' comment
PRINT "ERROR" ' comment
ELSEIF I = 7 ' comment
PRINT "Correct" ' comment
ELSE ' comment
PRINT "ERROR" ' comment
END IF ' comment

I = 5 * 6 ' comment
IF I > 10 THEN 1000 ELSE 9000 ' comment
ERROR 1 ' comment
1000 IF I < 99 THEN 2000 ELSE 9000 ' comment
ERROR 1 ' comment
2000 ' comment
GOTO 3000 ' comment
2100 IF I  = 1 THEN PRINT "Good 1":RETURN ELSE PRINT "ERROR ON 1" : GOTO 9000 ' comment
2200 IF I  = 2 THEN PRINT "Good 2":RETURN ELSE PRINT "ERROR ON 2" : GOTO 9000 ' comment
2300 IF I  = 3 THEN PRINT "Good 3":RETURN ELSE PRINT "ERROR ON 3" : GOTO 9000 ' comment
3000 ' fncs ' comment
3010 GOTO 3400 ' comment
3100 IF I  <> 1 THEN PRINT "ERROR ON 1" : GOTO 9000 ELSE PRINT "Good 1":GOTO 3500 ' comment
3200 IF I  <> 2 THEN PRINT "ERROR ON 2" : GOTO 9000 ELSE PRINT "Good 2":GOTO 3500 ' comment
3300 IF I  <> 3 THEN PRINT "ERROR ON 3" : GOTO 9000 ELSE PRINT "Good 3":GOTO 3500 ' comment
3310 PRINT "ERROR 4": STOP ' comment
3350 print "good 4"
return ' comment
3400 FOR I = 1 TO 3 ' comment
PRINT "I=";I ' comment
ON I GOSUB 2100,2200,2300 ' comment
ON I GOTO  3100,3200,3300 ' comment
3500 REM ' comment 
NEXT I ' comment 
gosub 3350 ' comment
LPRINT A$; ' comment
LPRINT B$, ' comment
LPRINT C$ ' comment

OPEN "test.dat" FOR RANDOM AS #1 LEN 12 ' comment
FIELD #1, 5 AS D$, 7 AS E$ ' comment
FIELD #1, 12 AS F$

LSET D$ =  "d" ' comment
RSET E$ =  "e" ' comment

PRINT "[";D$;"]" ' comment
PRINT "[";E$;"]" ' comment

PUT #1 ' comment
SEEK #1, 1 ' comment
GET #1 ' comment
PRINT "["; F$; "]" ' comment
CLOSE #1

OPEN "test.dat" for output as #1
PRINT #1, A$ ' comment
CLOSE #1 ' comment
OPEN "test.dat" for INPUT as #1 ' comment
INPUT #1, B$ ' comment
CLOSE #1 ' comment
PRINT "["; B$; "]" ' comment

PRINT "Before SUB definition" ' comment
' MAINTAINER STACK
4000 SUB TestSub( X1, Y1 ) ' comment
' MAINTAINER STACK
IF X1 > Y1 THEN ' comment
PRINT "X1 > Y1",X1;">";Y1 ' comment
' LET TestSub = X1 - Y1
EXIT SUB ' comment
END IF ' comment
PRINT "X1 <= Y1",X1;"<=";Y1 ' comment
' LET TestSub = X1 + Y1
END SUB ' comment
PRINT "After SUB definition" ' comment

' MAINTAINER STACK

TestSub 1, 2 ' comment
' MAINTAINER STACK
CALL TestSub(2,1) ' comment
CALL ABS(2) ' comment


5000 FUNCTION TestFun( X2, Y2 ) ' comment
5010 IF X2 < Y2 THEN ' comment
5020 LET TestFun = X2 + Y2 ' comment
5025 PRINT "X2 < Y2",X2;"<";Y2 ' comment
5030 EXIT FUNCTION ' comment
5040 END IF ' comment
5050 LET TestFun = X2 - Y2 ' comment
5055 PRINT "X2 >= Y2",X2;">=";Y2 ' comment
5060 END FUNCTION ' comment

TestFun 1, 2 ' comment
CALL TestFun( 5, 1 ) ' comment
LET Z = TestFun( 10, 3 ) ' comment
PRINT "Z=";Z ' comment

OPEN "test.dat" FOR INPUT AS #1 ' comment
LINE INPUT #1, A$ ' comment
CLOSE #1 ' comment
PRINT "A$=";A$ ' comment
SWAP A$,B$ ' comment
PRINT "B$=";B$ ' comment
REM vars ' comment
OPEN "test.dat" FOR OUTPUT AS #1 ' comment
WRITE #1, B$ ' comment
CLOSE #1 ' comment
OPEN "test.dat" FOR INPUT AS #1 ' comment
LINE INPUT #1, A$ ' comment
CLOSE #1 ' comment
PRINT "A$=";A$ ' comment
GOTO AnotherPlace ' comment
PRINT "ERROR 7" ' comment
STOP ' comment

' comments are NOT allowed on LABEL lines
AnotherPlace:
' comments are NOT allowed on LABEL lines
5100 REM STOP ' comment
ON ERROR GOTO HELL ' comment
PRINT "firing..." ' comment
ERROR 51 ' comment
IF I = 17 THEN 6000 ' OK
STOP ' ERROR
' comments are NOT allowed on LABEL lines
Hell: 
' comments are NOT allowed on LABEL lines
PRINT "caught..."
IF ERR <> 51 THEN STOP ' ERROR
LET I = 17:RESUME NEXT ' OK
STOP ' ERROR
6000 ' OK
ERROR 0 ' clear
PRINT "Throw" ' comment
ON ERROR RESUME NEXT ' comment
ERROR 77 ' comment
IF ERR = 77 THEN ' comment
  PRINT "Caught" ' comment
  ERROR 0 ' clear
ELSE ' comment
  PRINT "Missed" ' comment
  STOP ' comment
END IF ' comment

REM HELP ' COMMENT
help help ' comment
erase x ' comment
dim x(23) ' comment
DEF FNA(X4)=X4+X(22) ' comment
LET X(22) = 2 ' comment
PRINT "FNA=";FNA(7) ' comment
CSAVE* X ' comment
erase x ' comment
dim X(23) ' comment
CLOAD* X ' comment
PRINT "X(22)=";X(22) ' comment

A$ = "abcdefghijklmnop" ' comment
B$ = "123456789" ' comment
MID$( A$, 5, 2 ) = B$ ' comment
PRINT "A$=";A$ ' comment

A$ = "abcdefghijklmnop" ' comment
B$ = "123456789" ' comment
MID$( A$, 5 ) = B$ ' comment
PRINT "A$=";A$ ' comment

REM VARS ' COMMENT
MAINTAINER STACK ' comment
print "Ok" ' comment
9000 end ' comment 
