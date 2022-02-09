REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:24
REM  FILENAME: F_INSTR_X_A_B_N.BAS
REM    SYNTAX: N  = INSTR( X, A$, B$ )
REM  UNIQUEID: F_INSTR_X_A_B_N
REM  FUNCTION: INSTR
REM 
REM	VARIABLE	DESCRIPTION
REM	I	CURRENT  TESTCASE NUMBER
REM	E	EXPECTED ERROR FLAG
REM	F	ACTUAL   ERROR FLAG
REM	R	EXPECTED FUNCTION RESULT
REM	N	ACTUAL   FUNCTION RESULT
REM	X	ACTUAL   FUNCTION PARAMETER
REM	A$	ACTUAL   FUNCTION PARAMETER
REM	B$	ACTUAL   FUNCTION PARAMETER
REM 
RESTORE 2000
REM GET TESTCASE #
READ  I
WHILE I > 0
REM GET TESTCASE DATA
READ  E, R, X, A$, B$
REM DISPLAY TESTCASE DATA
PRINT "TESTCASE #";I,"E=";E,"R=";R, "X=";X, "A$=[";A$;"]", "B$=[";B$;"]"
REM EXECUTE TESTCASE
N =  0 
S$ = ""
ON ERROR GOTO 9000
F = 0
999 N  = INSTR( X, A$, B$ )
ERROR 0
REM DISPLAY TESTCASE RESULTS
PRINT ,"F=";F,"N=";N
REM VERIFY EXPECTED ERROR
IF E <> F THEN
  PRINT "*** TEST FAILED, EXCEPTION MISMATCH ***"
  STOP
END IF
REM VERIFY EXPECTED RESULT
IF R > 0 THEN
  LET RMIN = 0.99 * R
  IF R > MAXNUM / 1.012 THEN
    LET RMAX = MAXNUM
  ELSE
    LET RMAX = 1.01 * R
  END IF
END IF
IF R < 0 THEN
  IF R < - MAXNUM / 1.012 THEN
    LET RMIN = -MAXNUM
  ELSE
    LET RMIN = 1.01 * R
  END IF
  LET RMAX = 0.99 * R
END IF
IF R = 0 THEN
  LET RMIN = -.01
  LET RMAX = +.01
END IF
IF N < RMIN THEN
  PRINT "*** TEST FAILED, RESULT MISMATCH ***"
  STOP
END IF
IF N > RMAX THEN
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
DATA  1 ,  1 ,  0 ,  0 , "", ""
DATA  2 ,  0 ,  1 ,  1 , " ", " "
DATA  3 ,  1 ,  0 , -1 , "3", "3"
DATA  4 ,  1 ,  0 ,  .4 , "z", "z"
DATA  5 ,  1 ,  0 , -.4 , "  ", "  "
DATA  6 ,  0 ,  1 ,  .6 , " 3", " 3"
DATA  7 ,  1 ,  0 , -.6 , " z", " z"
DATA  8 ,  0 ,  0 ,  254 , "3 ", "3 "
DATA  9 ,  0 ,  0 ,  255 , "z ", "z "
DATA  10 ,  0 ,  0 ,  256 , " 3 ", " 3 "
DATA  11 ,  1 ,  0 ,  32000 , " z ", " z "
DATA  12 ,  1 ,  0 , -.227229 , " LLLL", " LLLL"
DATA  13 ,  1 ,  0 , -.239157 , "    ''    ", "    ''    "
DATA  14 ,  1 ,  0 , -.213586 , "   oo     ", "   oo     "
DATA  15 ,  1 ,  0 ,  .274675 , " 00  ", " 00  "
DATA  16 ,  1 ,  0 ,  .169766 , "   \\\\  ", "   \\\\  "
DATA  17 ,  1 ,  0 ,  .318766 , " lllll  ", " lllll  "
DATA  18 ,  1 ,  0 ,  2.06658E-2 , "    uuuu  ", "    uuuu  "
DATA  19 ,  1 ,  0 ,  .188257 , "  a  ", "  a  "
DATA  20 ,  1 ,  0 , -.277537 , "       ", "       "
DATA  21 ,  1 ,  0 , -.481303 , " hh     ", " hh     "
DATA  22 ,  1 ,  0 ,  .43556 , "  h   ", "  h   "
DATA  23 ,  1 ,  0 , -.210971 , " )", " )"
DATA  24 ,  1 ,  0 , -.455216 , "    5   ", "    5   "
DATA  25 ,  1 ,  0 , -110.907 , "  2 ", "  2 "
DATA  26 ,  0 ,  0 ,  105.632 , " -   ", " -   "
DATA  27 ,  1 ,  0 , -114.341 , "dd ", "dd "
DATA  28 ,  0 ,  0 ,  109.706 , "  tttt   ", "  tttt   "
DATA  29 ,  0 ,  0 ,  109.198 , "  ::::   ", "  ::::   "
DATA  30 ,  0 ,  0 ,  85.8773 , "   ___   ", "   ___   "
DATA  31 ,  1 ,  0 , -58.3548 , "     ", "     "
DATA  32 ,  0 ,  0 ,  29.6397 , "       ", "       "
DATA  33 ,  1 ,  0 , -108.36 , " _____  ", " _____  "
DATA  34 ,  1 ,  0 , -46.5199 , " S ", " S "
DATA  35 ,  0 ,  0 ,  112.521 , "  FFFF ", "  FFFF "
DATA  36 ,  0 ,  0 ,  45.4151 , "    kkk", "    kkk"
DATA  37 ,  0 ,  0 ,  101.129 , "    NNNN  ", "    NNNN  "
DATA  38 ,  0 ,  0 ,  65.0473 , "   mmmm  ", "   mmmm  "
DATA  39 ,  0 ,  0 ,  48.4012 , "    ,,,,, ", "    ,,,,, "
DATA  40 ,  0 ,  0 ,  122.294 , "OO   ", "OO   "
DATA  41 ,  1 ,  0 , -56.1094 , "   HHHH  ", "   HHHH  "
DATA  42 ,  1 ,  0 , -121.087 , " < ", " < "
DATA  43 ,  1 ,  0 , -123.517 , "   j    ", "   j    "
DATA  44 ,  1 ,  0 , -97.6301 , "    yyyy    ", "    yyyy    "
DATA  45 ,  1 ,  0 , -64.0001 , "  +++ ", "  +++ "
DATA  46 ,  1 ,  0 , -25.6888 , "o  ", "o  "
DATA  47 ,  0 ,  0 ,  14.8006 , "    ((", "    (("
DATA  48 ,  1 ,  0 , -61.0877 , "     uu     ", "     uu     "
DATA  49 ,  1 ,  0 , -66.6381 , "    00     ", "    00     "
DATA  50 ,  1 ,  0 , -5476.5 , "     HH  ", "     HH  "
DATA  51 ,  1 ,  0 , -21210.3 , "     ", "     "
DATA  52 ,  1 ,  0 , -28757.2 , "  G   ", "  G   "
DATA  53 ,  1 ,  0 , -28732.2 , "    {{{{ ", "    {{{{ "
DATA  54 ,  1 ,  0 ,  12730.3 , "    ]]]     ", "    ]]]     "
DATA  55 ,  1 ,  0 , -1830.69 , "CCC", "CCC"
DATA  56 ,  1 ,  0 ,  5486.39 , "     44  ", "     44  "
DATA  57 ,  1 ,  0 ,  22405.9 , "^^  ", "^^  "
DATA  58 ,  1 ,  0 ,  30809.8 , "    QQ    ", "    QQ    "
DATA  59 ,  1 ,  0 , -28930.6 , "     FFFF  ", "     FFFF  "
DATA  60 ,  1 ,  0 , -6014.53 , " n   ", " n   "
DATA  61 ,  1 ,  0 ,  17414.6 , "    hh  ", "    hh  "
DATA  62 ,  1 ,  0 , -10538.3 , "555    ", "555    "
DATA  63 ,  1 ,  0 , -17536.3 , "    YYYY ", "    YYYY "
DATA  64 ,  1 ,  0 , -8856.62 , "    w     ", "    w     "
DATA  65 ,  1 ,  0 , -5140.12 , "MMMM ", "MMMM "
DATA  66 ,  1 ,  0 , -8595.31 , " JJJJ    ", " JJJJ    "
DATA  67 ,  1 ,  0 , -3382.87 , "    %%    ", "    %%    "
DATA  68 ,  1 ,  0 , -5002.07 , "     *  ", "     *  "
DATA  69 ,  1 ,  0 , -5800.72 , "    @@@   ", "    @@@   "
DATA  70 ,  1 ,  0 ,  29686.4 , "I ", "I "
DATA  71 ,  0 ,  0 ,  2571. , "    ss  ", "    ss  "
DATA  72 ,  1 ,  0 ,  31091. , "  f ", "  f "
DATA  73 ,  1 ,  0 ,  14819. , "   D  ", "   D  "
DATA  74 ,  0 ,  0 ,  3790.9 , "###    ", "###    "
DATA  75 ,  1 ,  0 , -4.2981E+9 , "    '     ", "    '     "
DATA  76 ,  1 ,  0 , -7.05453E+8 , "  dd    ", "  dd    "
DATA  77 ,  1 ,  0 , -3.10179E+9 , " TT    ", " TT    "
DATA  78 ,  1 ,  0 , -4.86159E+9 , "  vvv ", "  vvv "
DATA  79 ,  1 ,  0 , -8.62908E+8 , "    X ", "    X "
DATA  80 ,  1 ,  0 , -3.4101E+9 , "    %   ", "    %   "
DATA  81 ,  1 ,  0 ,  3.3889E+9 , "  ", "  "
DATA  82 ,  1 ,  0 ,  2.49826E+9 , " 5555   ", " 5555   "
DATA  83 ,  1 ,  0 ,  2.3491E+9 , "FFFF   ", "FFFF   "
DATA  84 ,  1 ,  0 ,  1.94745E+8 , "     %%  ", "     %%  "
DATA  85 ,  1 ,  0 ,  2.02475E+9 , "     ", "     "
DATA  86 ,  1 ,  0 , -2.26748E+9 , "  ~~~~    ", "  ~~~~    "
DATA  87 ,  1 ,  0 , -2.10587E+9 , "   ||    ", "   ||    "
DATA  88 ,  1 ,  0 ,  4.23585E+9 , "     SSS     ", "     SSS     "
DATA  89 ,  1 ,  0 , -2.66524E+9 , "   6666 ", "   6666 "
DATA  90 ,  1 ,  0 ,  4.01052E+9 , "    ww   ", "    ww   "
DATA  91 ,  1 ,  0 , -4.87912E+9 , "  1111   ", "  1111   "
DATA  92 ,  1 ,  0 ,  1.77556E+9 , " K ", " K "
DATA  93 ,  1 ,  0 ,  2.87705E+8 , "  ttttt ", "  ttttt "
DATA  94 ,  1 ,  0 ,  1.99977E+8 , " OOOOO   ", " OOOOO   "
DATA  95 ,  1 ,  0 , -3.39941E+9 , " E  ", " E  "
DATA  96 ,  1 ,  0 , -3.49119E+9 , " PPPPP   ", " PPPPP   "
DATA  97 ,  1 ,  0 , -2.86324E+9 , "    == ", "    == "
DATA  98 ,  1 ,  0 ,  4.20418E+9 , " VVVV    ", " VVVV    "
DATA  99 ,  1 ,  0 , -3.1518E+9 , " c   ", " c   "
DATA  100 ,  1 ,  0 ,  4.60615E+9 , "    yyyy     ", "    yyyy     "
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
