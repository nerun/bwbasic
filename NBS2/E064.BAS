REM Purpose: Verify integer divide, X \ Y
REM  Author: Howard Wulf, AF5NE
REM    Date: 2015-02-21
REM


READ X, Y, Z
DO
    LET N = X \ Y
    PRINT X,Y,Z,N
    IF Z <> N THEN
      PRINT "*** TEST FAILED ***"
      STOP
    END IF
    READ X, Y, Z
LOOP UNTIL X = 999
GOTO 999

REM TEST DATA
REM  X,Y,Z
DATA 1,1,1
DATA 2,1,2
DATA 3,1,3
DATA 9,3,3
DATA 1.0,1.5,0
DATA 1.5,1.5,1
DATA 1.5,1.0,2
DATA 10,3,3
DATA 1.4,1.3,1
DATA 1.3,1.4,1
DATA 999,999,999

999 END
