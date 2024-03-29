REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:34
REM  FILENAME: F_STRING_X_Y_S.BAS
REM    SYNTAX: S$ = STRING$( X, Y )
REM  UNIQUEID: F_STRING_X_Y_S
REM  FUNCTION: STRING$
REM 
REM	VARIABLE	DESCRIPTION
REM	I	CURRENT  TESTCASE NUMBER
REM	E	EXPECTED ERROR FLAG
REM	F	ACTUAL   ERROR FLAG
REM	R$	EXPECTED FUNCTION RESULT
REM	S$	ACTUAL   FUNCTION RESULT
REM	X	ACTUAL   FUNCTION PARAMETER
REM	Y	ACTUAL   FUNCTION PARAMETER
REM 
RESTORE 2000
REM GET TESTCASE #
READ  I
WHILE I > 0
REM GET TESTCASE DATA
READ  E, R$, X, Y
REM DISPLAY TESTCASE DATA
PRINT "TESTCASE #";I,"E=";E,"R$=[";R$;"]", "X=";X, "Y=";Y
REM EXECUTE TESTCASE
N =  0 
S$ = ""
ON ERROR GOTO 9000
F = 0
999 S$ = STRING$( X, Y )
ERROR 0
REM DISPLAY TESTCASE RESULTS
PRINT ,"F=";F,"S$=[";S$;"]"
REM VERIFY EXPECTED ERROR
IF E <> F THEN
  PRINT "*** TEST FAILED, EXCEPTION MISMATCH ***"
  STOP
END IF
REM VERIFY EXPECTED RESULT
IF R$ <> S$ THEN
  PRINT "*** TEST FAILED, RESULT MISMATCH ***"
  STOP
END IF
PRINT "*** TEST PASSED ***"
REM GET NEXT TESTCASE #
READ  I
WEND
PRINT "*** ALL TESTS PASSED ***"
GOTO 9999
1999 REM TESTCASE, EXPECTED ERROR, EXPECTED RESULT, PARAMETERS...
DATA  1 ,  0 , "",  0 ,  0 
DATA  2 ,  0 , "",  1 ,  1 
DATA  3 ,  1 , "", -1 , -1 
DATA  4 ,  0 , "",  .4 ,  .4 
DATA  5 ,  0 , "", -.4 , -.4 
DATA  6 ,  0 , "",  .6 ,  .6 
DATA  7 ,  1 , "", -.6 , -.6 
DATA  8 ,  0 , "��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������",  254 ,  254 
DATA  9 ,  0 , "���������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������",  255 ,  255 
DATA  10 ,  1 , "",  256 ,  256 
DATA  11 ,  1 , "",  32000 ,  32000 
DATA  12 ,  0 , "", -.227229 , -.227229 
DATA  13 ,  0 , "", -.239157 , -.239157 
DATA  14 ,  0 , "", -.213586 , -.213586 
DATA  15 ,  0 , "",  .274675 ,  .274675 
DATA  16 ,  0 , "",  .169766 ,  .169766 
DATA  17 ,  0 , "",  .318766 ,  .318766 
DATA  18 ,  0 , "",  2.06658E-2 ,  2.06658E-2 
DATA  19 ,  0 , "",  .188257 ,  .188257 
DATA  20 ,  0 , "", -.277537 , -.277537 
DATA  21 ,  0 , "", -.481303 , -.481303 
DATA  22 ,  0 , "",  .43556 ,  .43556 
DATA  23 ,  0 , "", -.210971 , -.210971 
DATA  24 ,  0 , "", -.455216 , -.455216 
DATA  25 ,  1 , "", -110.907 , -110.907 
DATA  26 ,  0 , "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj",  105.632 ,  105.632 
DATA  27 ,  1 , "", -114.341 , -114.341 
DATA  28 ,  0 , "nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn",  109.706 ,  109.706 
DATA  29 ,  0 , "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm",  109.198 ,  109.198 
DATA  30 ,  0 , "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV",  85.8773 ,  85.8773 
DATA  31 ,  1 , "", -58.3548 , -58.3548 
DATA  32 ,  0 , "",  29.6397 ,  29.6397 
DATA  33 ,  1 , "", -108.36 , -108.36 
DATA  34 ,  1 , "", -46.5199 , -46.5199 
DATA  35 ,  0 , "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq",  112.521 ,  112.521 
DATA  36 ,  0 , "---------------------------------------------",  45.4151 ,  45.4151 
DATA  37 ,  0 , "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",  101.129 ,  101.129 
DATA  38 ,  0 , "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",  65.0473 ,  65.0473 
DATA  39 ,  0 , "000000000000000000000000000000000000000000000000",  48.4012 ,  48.4012 
DATA  40 ,  0 , "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz",  122.294 ,  122.294 
DATA  41 ,  1 , "", -56.1094 , -56.1094 
DATA  42 ,  1 , "", -121.087 , -121.087 
DATA  43 ,  1 , "", -123.517 , -123.517 
DATA  44 ,  1 , "", -97.6301 , -97.6301 
DATA  45 ,  1 , "", -64.0001 , -64.0001 
DATA  46 ,  1 , "", -25.6888 , -25.6888 
DATA  47 ,  0 , "",  14.8006 ,  14.8006 
DATA  48 ,  1 , "", -61.0877 , -61.0877 
DATA  49 ,  1 , "", -66.6381 , -66.6381 
DATA  50 ,  1 , "", -5476.5 , -5476.5 
DATA  51 ,  1 , "", -21210.3 , -21210.3 
DATA  52 ,  1 , "", -28757.2 , -28757.2 
DATA  53 ,  1 , "", -28732.2 , -28732.2 
DATA  54 ,  1 , "",  12730.3 ,  12730.3 
DATA  55 ,  1 , "", -1830.69 , -1830.69 
DATA  56 ,  1 , "",  5486.39 ,  5486.39 
DATA  57 ,  1 , "",  22405.9 ,  22405.9 
DATA  58 ,  1 , "",  30809.8 ,  30809.8 
DATA  59 ,  1 , "", -28930.6 , -28930.6 
DATA  60 ,  1 , "", -6014.53 , -6014.53 
DATA  61 ,  1 , "",  17414.6 ,  17414.6 
DATA  62 ,  1 , "", -10538.3 , -10538.3 
DATA  63 ,  1 , "", -17536.3 , -17536.3 
DATA  64 ,  1 , "", -8856.62 , -8856.62 
DATA  65 ,  1 , "", -5140.12 , -5140.12 
DATA  66 ,  1 , "", -8595.31 , -8595.31 
DATA  67 ,  1 , "", -3382.87 , -3382.87 
DATA  68 ,  1 , "", -5002.07 , -5002.07 
DATA  69 ,  1 , "", -5800.72 , -5800.72 
DATA  70 ,  1 , "",  29686.4 ,  29686.4 
DATA  71 ,  1 , "",  2571. ,  2571. 
DATA  72 ,  1 , "",  31091. ,  31091. 
DATA  73 ,  1 , "",  14819. ,  14819. 
DATA  74 ,  1 , "",  3790.9 ,  3790.9 
DATA  75 ,  1 , "", -4.2981E+9 , -4.2981E+9 
DATA  76 ,  1 , "", -7.05453E+8 , -7.05453E+8 
DATA  77 ,  1 , "", -3.10179E+9 , -3.10179E+9 
DATA  78 ,  1 , "", -4.86159E+9 , -4.86159E+9 
DATA  79 ,  1 , "", -8.62908E+8 , -8.62908E+8 
DATA  80 ,  1 , "", -3.4101E+9 , -3.4101E+9 
DATA  81 ,  1 , "",  3.3889E+9 ,  3.3889E+9 
DATA  82 ,  1 , "",  2.49826E+9 ,  2.49826E+9 
DATA  83 ,  1 , "",  2.3491E+9 ,  2.3491E+9 
DATA  84 ,  1 , "",  1.94745E+8 ,  1.94745E+8 
DATA  85 ,  1 , "",  2.02475E+9 ,  2.02475E+9 
DATA  86 ,  1 , "", -2.26748E+9 , -2.26748E+9 
DATA  87 ,  1 , "", -2.10587E+9 , -2.10587E+9 
DATA  88 ,  1 , "",  4.23585E+9 ,  4.23585E+9 
DATA  89 ,  1 , "", -2.66524E+9 , -2.66524E+9 
DATA  90 ,  1 , "",  4.01052E+9 ,  4.01052E+9 
DATA  91 ,  1 , "", -4.87912E+9 , -4.87912E+9 
DATA  92 ,  1 , "",  1.77556E+9 ,  1.77556E+9 
DATA  93 ,  1 , "",  2.87705E+8 ,  2.87705E+8 
DATA  94 ,  1 , "",  1.99977E+8 ,  1.99977E+8 
DATA  95 ,  1 , "", -3.39941E+9 , -3.39941E+9 
DATA  96 ,  1 , "", -3.49119E+9 , -3.49119E+9 
DATA  97 ,  1 , "", -2.86324E+9 , -2.86324E+9 
DATA  98 ,  1 , "",  4.20418E+9 ,  4.20418E+9 
DATA  99 ,  1 , "", -3.1518E+9 , -3.1518E+9 
DATA  100 ,  1 , "",  4.60615E+9 ,  4.60615E+9 
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
