REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:20
REM  FILENAME: F_CVL_A_N.BAS
REM    SYNTAX: N  = CVL( A$ )
REM  UNIQUEID: F_CVL_A_N
REM  FUNCTION: CVL
REM 
REM	VARIABLE	DESCRIPTION
REM	I	CURRENT  TESTCASE NUMBER
REM	E	EXPECTED ERROR FLAG
REM	F	ACTUAL   ERROR FLAG
REM	R	EXPECTED FUNCTION RESULT
REM	N	ACTUAL   FUNCTION RESULT
REM	A$	ACTUAL   FUNCTION PARAMETER
REM 
RESTORE 2000
REM GET TESTCASE #
READ  I
WHILE I > 0
REM GET TESTCASE DATA
READ  E, R, A$
REM DISPLAY TESTCASE DATA
PRINT "TESTCASE #";I,"E=";E,"R=";R, "A$=[";A$;"]"
REM EXECUTE TESTCASE
N =  0 
S$ = ""
ON ERROR GOTO 9000
F = 0
999 N  = CVL( A$ )
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
DATA  1 ,  1 ,  0 , ""
DATA  2 ,  1 ,  0 , " "
DATA  3 ,  1 ,  0 , "3"
DATA  4 ,  1 ,  0 , "z"
DATA  5 ,  1 ,  0 , "  "
DATA  6 ,  1 ,  0 , " 3"
DATA  7 ,  1 ,  0 , " z"
DATA  8 ,  1 ,  0 , "3 "
DATA  9 ,  1 ,  0 , "z "
DATA  10 ,  1 ,  0 , " 3 "
DATA  11 ,  1 ,  0 , " z "
DATA  12 ,  0 ,  1.28007E+9 , " LLLL"
DATA  13 ,  0 ,  5.38976E+8 , "    ''    "
DATA  14 ,  0 ,  1.86438E+9 , "   oo     "
DATA  15 ,  0 ,  5.40029E+8 , " 00  "
DATA  16 ,  0 ,  1.54561E+9 , "   \\\\  "
DATA  17 ,  0 ,  1.81904E+9 , " lllll  "
DATA  18 ,  0 ,  5.38976E+8 , "    uuuu  "
DATA  19 ,  0 ,  5.43236E+8 , "  a  "
DATA  20 ,  0 ,  5.38976E+8 , "       "
DATA  21 ,  0 ,  5.43713E+8 , " hh     "
DATA  22 ,  0 ,  5.43695E+8 , "  h   "
DATA  23 ,  1 ,  0 , " )"
DATA  24 ,  0 ,  5.38976E+8 , "    5   "
DATA  25 ,  0 ,  5.40156E+8 , "  2 "
DATA  26 ,  0 ,  5.3898E+8 , " -   "
DATA  27 ,  1 ,  0 , "dd "
DATA  28 ,  0 ,  1.95377E+9 , "  tttt   "
DATA  29 ,  0 ,  9.76888E+8 , "  ::::   "
DATA  30 ,  0 ,  1.59594E+9 , "   ___   "
DATA  31 ,  0 ,  5.38976E+8 , "     "
DATA  32 ,  0 ,  5.38976E+8 , "       "
DATA  33 ,  0 ,  1.60009E+9 , " _____  "
DATA  34 ,  1 ,  0 , " S "
DATA  35 ,  0 ,  1.179E+9 , "  FFFF "
DATA  36 ,  0 ,  5.38976E+8 , "    kkk"
DATA  37 ,  0 ,  5.38976E+8 , "    NNNN  "
DATA  38 ,  0 ,  1.83082E+9 , "   mmmm  "
DATA  39 ,  0 ,  5.38976E+8 , "    ,,,,, "
DATA  40 ,  0 ,  5.38988E+8 , "OO   "
DATA  41 ,  0 ,  1.21006E+9 , "   HHHH  "
DATA  42 ,  1 ,  0 , " < "
DATA  43 ,  0 ,  1.78049E+9 , "   j    "
DATA  44 ,  0 ,  5.38976E+8 , "    yyyy    "
DATA  45 ,  0 ,  7.24247E+8 , "  +++ "
DATA  46 ,  1 ,  0 , "o  "
DATA  47 ,  0 ,  5.38976E+8 , "    (("
DATA  48 ,  0 ,  5.38976E+8 , "     uu     "
DATA  49 ,  0 ,  5.38976E+8 , "    00     "
DATA  50 ,  0 ,  5.38976E+8 , "     HH  "
DATA  51 ,  0 ,  5.38976E+8 , "     "
DATA  52 ,  0 ,  5.41532E+8 , "  G   "
DATA  53 ,  0 ,  5.38976E+8 , "    {{{{ "
DATA  54 ,  0 ,  5.38976E+8 , "    ]]]     "
DATA  55 ,  1 ,  0 , "CCC"
DATA  56 ,  0 ,  5.38976E+8 , "     44  "
DATA  57 ,  0 ,  5.38992E+8 , "^^  "
DATA  58 ,  0 ,  5.38976E+8 , "    QQ    "
DATA  59 ,  0 ,  5.38976E+8 , "     FFFF  "
DATA  60 ,  0 ,  5.38996E+8 , " n   "
DATA  61 ,  0 ,  5.38976E+8 , "    hh  "
DATA  62 ,  0 ,  5.40358E+8 , "555    "
DATA  63 ,  0 ,  5.38976E+8 , "    YYYY "
DATA  64 ,  0 ,  5.38976E+8 , "    w     "
DATA  65 ,  0 ,  1.29691E+9 , "MMMM "
DATA  66 ,  0 ,  1.24638E+9 , " JJJJ    "
DATA  67 ,  0 ,  5.38976E+8 , "    %%    "
DATA  68 ,  0 ,  5.38976E+8 , "     *  "
DATA  69 ,  0 ,  5.38976E+8 , "    @@@   "
DATA  70 ,  1 ,  0 , "I "
DATA  71 ,  0 ,  5.38976E+8 , "    ss  "
DATA  72 ,  0 ,  5.43564E+8 , "  f "
DATA  73 ,  0 ,  1.14296E+9 , "   D  "
DATA  74 ,  0 ,  5.39174E+8 , "###    "
DATA  75 ,  0 ,  5.38976E+8 , "    '     "
DATA  76 ,  0 ,  1.68428E+9 , "  dd    "
DATA  77 ,  0 ,  5.42397E+8 , " TT    "
DATA  78 ,  0 ,  1.98745E+9 , "  vvv "
DATA  79 ,  0 ,  5.38976E+8 , "    X "
DATA  80 ,  0 ,  5.38976E+8 , "    %   "
DATA  81 ,  1 ,  0 , "  "
DATA  82 ,  0 ,  8.92679E+8 , " 5555   "
DATA  83 ,  0 ,  1.17901E+9 , "FFFF   "
DATA  84 ,  0 ,  5.38976E+8 , "     %%  "
DATA  85 ,  0 ,  5.38976E+8 , "     "
DATA  86 ,  0 ,  2.12219E+9 , "  ~~~~    "
DATA  87 ,  0 ,  2.08248E+9 , "   ||    "
DATA  88 ,  0 ,  5.38976E+8 , "     SSS     "
DATA  89 ,  0 ,  9.08075E+8 , "   6666 "
DATA  90 ,  0 ,  5.38976E+8 , "    ww   "
DATA  91 ,  0 ,  8.25303E+8 , "  1111   "
DATA  92 ,  1 ,  0 , " K "
DATA  93 ,  0 ,  1.95377E+9 , "  ttttt "
DATA  94 ,  0 ,  1.3306E+9 , " OOOOO   "
DATA  95 ,  0 ,  5.38986E+8 , " E  "
DATA  96 ,  0 ,  1.34744E+9 , " PPPPP   "
DATA  97 ,  0 ,  5.38976E+8 , "    == "
DATA  98 ,  0 ,  1.4485E+9 , " VVVV    "
DATA  99 ,  0 ,  5.38993E+8 , " c   "
DATA  100 ,  0 ,  5.38976E+8 , "    yyyy     "
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
