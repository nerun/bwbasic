REM Purpose: Verify example program
REM   Refer:
REM    Page: 11
REM


PRINT 12 AND 3
PRINT &HC AND &H5
PRINT NOT -1
PRINT NOT &H3
PRINT 12 OR 3
PRINT &H0C OR &H5

LET I% = 12.11
LET J% = 3.2
PRINT I%  XOR J%

LET I% = 12.11
LET J% = 3.7
PRINT I%  XOR J%


END
