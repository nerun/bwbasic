REM Purpose: Verify boolean operators
REM  Author: Howard Wulf, AF5NE
REM    Date: 2015-02-20
REM
FUNCTION TF$( N )
  IF N = 0 THEN
    LET TF$ = "F   "
  ELSE
    LET TF$ = "T   "
  END IF
END FUNCTION
REM
PRINT "                X   X   X   X   X   X   X   X   X   X   X   "
PRINT "       NOT NOT AND  OR XOR EQV IMP  <   <=  =   >=  >   <>  "
PRINT "X   Y   X   Y   Y   Y   Y   Y   Y   Y   Y   Y   Y   Y   Y   "
PRINT "=   =   =   =   =   =   =   =   =   =   =   =   =   =   =   "
for X = -1 to 0
for Y = -1 to 0
    PRINT TF$(      X);
    PRINT TF$(      Y);
    PRINT TF$(  NOT X);
    PRINT TF$(  NOT Y);
    PRINT TF$(X AND Y);
    PRINT TF$(X  OR Y);
    PRINT TF$(X XOR Y);
    PRINT TF$(X EQV Y);
    PRINT TF$(X IMP Y);
    PRINT TF$(X <   Y);
    PRINT TF$(X <=  Y);
    PRINT TF$(X  =  Y);
    PRINT TF$(X >=  Y);
    PRINT TF$(X   > Y);
    PRINT TF$(X  <> Y);
    PRINT
next Y
next X

END
