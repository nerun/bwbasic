REM verify structured FUNCTION - END FUNCTION
REM HOWARD WULF, AF5NE
REM 
PRINT "BEFORE FUNCTION DEFINITIONS"
FUNCTION TOM( X )
  PRINT "ENTER TOM"
  PRINT "X=";X
  REM verify the function name is a variable in the explicit LET command
  LET TOM = 1
  PRINT "EXIT TOM"
END FUNCTION
FUNCTION JERRY( X )
  PRINT "ENTER JERRY"
  PRINT "X=";X
  REM verify the function name is a variable in the implied LET command
  JERRY = 2
  PRINT "EXIT JERRY"
END FUNCTION
PRINT "AFTER FUNCTION DEFINITIONS"
REM verify use as a function
LET X = TOM( 11 )
PRINT "X=";X
IF X <> 1 THEN
  PRINT "*** TEST FAILED ***"
  STOP
END IF
REM verify use as a subroutine
TOM 12
REM verify use as a function
LET X = JERRY( 13 )
PRINT "X=";X
IF X <> 2 THEN
  PRINT "*** TEST FAILED ***"
  STOP
END IF
REM verify use as a subroutine
JERRY 14
REM verify use as a function
LET X = TOM( 15 ) + JERRY( 16 ) 
PRINT "X=";X
IF X <> 3 THEN
  PRINT "*** TEST FAILED ***"
  STOP
END IF
REM verify use as a subroutine with function
TOM JERRY( 17 )
REM verify use as a subroutine with function
JERRY TOM( 18 )
REM verify use as a function
LET X = TOM( JERRY( 19 )  ) 
PRINT "X=";X
IF X <> 1 THEN
  PRINT "*** TEST FAILED ***"
  STOP
END IF
REM verify use as a function
LET X = JERRY( TOM( 20 )  ) 
PRINT "X=";X
IF X <> 2 THEN
  PRINT "*** TEST FAILED ***"
  STOP
END IF
REM verify use as a subroutine calling itself as a parameter
TOM TOM( 21 )
REM verify use as a subroutine calling itself as a parameter
JERRY JERRY( 22 )
REM verify use as a function calling itself as a parameter
LET X = TOM( TOM( 23 ) )
IF X <> 1 THEN
  PRINT "*** TEST FAILED ***"
  STOP
END IF
REM verify use as a function calling itself as a parameter
LET X = JERRY( JERRY( 24 ) )
IF X <> 2 THEN
  PRINT "*** TEST FAILED ***"
  STOP
END IF
REM 
PRINT "*** TEST PASSED ***"
END
