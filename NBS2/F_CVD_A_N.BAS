REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:19
REM  FILENAME: F_CVD_A_N.BAS
REM    SYNTAX: N  = CVD( A$ )
REM  UNIQUEID: F_CVD_A_N
REM  FUNCTION: CVD
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
999 N  = CVD( A$ )
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
DATA  12 ,  1 ,  0 , " LLLL"
DATA  13 ,  0 ,  6.02371E-154 , "    ''    "
DATA  14 ,  0 ,  6.01392E-154 , "   oo     "
DATA  15 ,  1 ,  0 , " 00  "
DATA  16 ,  0 ,  8.46098E-153 , "   \\\\  "
DATA  17 ,  0 ,  6.12461E-154 , " lllll  "
DATA  18 ,  0 ,  6.4441E+257 , "    uuuu  "
DATA  19 ,  1 ,  0 , "  a  "
DATA  20 ,  1 ,  0 , "       "
DATA  21 ,  0 ,  6.01347E-154 , " hh     "
DATA  22 ,  1 ,  0 , "  h   "
DATA  23 ,  1 ,  0 , " )"
DATA  24 ,  0 ,  6.01359E-154 , "    5   "
DATA  25 ,  1 ,  0 , "  2 "
DATA  26 ,  1 ,  0 , " -   "
DATA  27 ,  1 ,  0 , "dd "
DATA  28 ,  0 ,  6.13631E-154 , "  tttt   "
DATA  29 ,  0 ,  6.05149E-154 , "  ::::   "
DATA  30 ,  0 ,  6.1056E-154 , "   ___   "
DATA  31 ,  1 ,  0 , "     "
DATA  32 ,  1 ,  0 , "       "
DATA  33 ,  0 ,  6.1056E-154 , " _____  "
DATA  34 ,  1 ,  0 , " S "
DATA  35 ,  1 ,  0 , "  FFFF "
DATA  36 ,  1 ,  0 , "    kkk"
DATA  37 ,  0 ,  1.63409E+69 , "    NNNN  "
DATA  38 ,  0 ,  1.75584E-152 , "   mmmm  "
DATA  39 ,  0 ,  6.59474E-96 , "    ,,,,, "
DATA  40 ,  1 ,  0 , "OO   "
DATA  41 ,  0 ,  3.62212E-153 , "   HHHH  "
DATA  42 ,  1 ,  0 , " < "
DATA  43 ,  0 ,  6.01347E-154 , "   j    "
DATA  44 ,  0 ,  1.41118E+277 , "    yyyy    "
DATA  45 ,  1 ,  0 , "  +++ "
DATA  46 ,  1 ,  0 , "o  "
DATA  47 ,  1 ,  0 , "    (("
DATA  48 ,  0 ,  2.5606E-152 , "     uu     "
DATA  49 ,  0 ,  6.03687E-154 , "    00     "
DATA  50 ,  0 ,  3.62203E-153 , "     HH  "
DATA  51 ,  1 ,  0 , "     "
DATA  52 ,  1 ,  0 , "  G   "
DATA  53 ,  0 ,  6.53867E+286 , "    {{{{ "
DATA  54 ,  0 ,  8.76048E-153 , "    ]]]     "
DATA  55 ,  1 ,  0 , "CCC"
DATA  56 ,  0 ,  1.50685E-153 , "     44  "
DATA  57 ,  1 ,  0 , "^^  "
DATA  58 ,  0 ,  6.08513E-154 , "    QQ    "
DATA  59 ,  0 ,  3.52944E+30 , "     FFFF  "
DATA  60 ,  1 ,  0 , " n   "
DATA  61 ,  0 ,  6.11876E-154 , "    hh  "
DATA  62 ,  1 ,  0 , "555    "
DATA  63 ,  0 ,  2.6183E+122 , "    YYYY "
DATA  64 ,  0 ,  6.01397E-154 , "    w     "
DATA  65 ,  1 ,  0 , "MMMM "
DATA  66 ,  0 ,  6.01371E-154 , " JJJJ    "
DATA  67 ,  0 ,  6.02078E-154 , "    %%    "
DATA  68 ,  0 ,  6.02804E-154 , "     *  "
DATA  69 ,  0 ,  2.42411E-153 , "    @@@   "
DATA  70 ,  1 ,  0 , "I "
DATA  71 ,  0 ,  6.13485E-154 , "    ss  "
DATA  72 ,  1 ,  0 , "  f "
DATA  73 ,  1 ,  0 , "   D  "
DATA  74 ,  1 ,  0 , "###    "
DATA  75 ,  0 ,  6.01351E-154 , "    '     "
DATA  76 ,  0 ,  6.01347E-154 , "  dd    "
DATA  77 ,  1 ,  0 , " TT    "
DATA  78 ,  1 ,  0 , "  vvv "
DATA  79 ,  1 ,  0 , "    X "
DATA  80 ,  0 ,  6.0135E-154 , "    %   "
DATA  81 ,  1 ,  0 , "  "
DATA  82 ,  0 ,  6.01359E-154 , " 5555   "
DATA  83 ,  1 ,  0 , "FFFF   "
DATA  84 ,  0 ,  7.88534E-154 , "     %%  "
DATA  85 ,  1 ,  0 , "     "
DATA  86 ,  0 ,  6.15094E-154 , "  ~~~~    "
DATA  87 ,  0 ,  6.014E-154 , "   ||    "
DATA  88 ,  0 ,  2.51937E+93 , "     SSS     "
DATA  89 ,  0 ,  1.65663E-153 , "   6666 "
DATA  90 ,  0 ,  6.1407E-154 , "    ww   "
DATA  91 ,  0 ,  6.03833E-154 , "  1111   "
DATA  92 ,  1 ,  0 , " K "
DATA  93 ,  0 ,  2.44095E-152 , "  ttttt "
DATA  94 ,  0 ,  6.0822E-154 , " OOOOO   "
DATA  95 ,  1 ,  0 , " E  "
DATA  96 ,  0 ,  6.08367E-154 , " PPPPP   "
DATA  97 ,  1 ,  0 , "    == "
DATA  98 ,  0 ,  6.01378E-154 , " VVVV    "
DATA  99 ,  1 ,  0 , " c   "
DATA  100 ,  0 ,  1.41118E+277 , "    yyyy     "
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
