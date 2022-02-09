REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:23
REM  FILENAME: F_FRE_A_N.BAS
REM    SYNTAX: N  = FRE( A$ )
REM  UNIQUEID: F_FRE_A_N
REM  FUNCTION: FRE
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
999 N  = FRE( A$ )
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
DATA  1 ,  0 ,  32000 , ""
DATA  2 ,  0 ,  32000 , " "
DATA  3 ,  0 ,  32000 , "3"
DATA  4 ,  0 ,  32000 , "z"
DATA  5 ,  0 ,  32000 , "  "
DATA  6 ,  0 ,  32000 , " 3"
DATA  7 ,  0 ,  32000 , " z"
DATA  8 ,  0 ,  32000 , "3 "
DATA  9 ,  0 ,  32000 , "z "
DATA  10 ,  0 ,  32000 , " 3 "
DATA  11 ,  0 ,  32000 , " z "
DATA  12 ,  0 ,  32000 , " LLLL"
DATA  13 ,  0 ,  32000 , "    ''    "
DATA  14 ,  0 ,  32000 , "   oo     "
DATA  15 ,  0 ,  32000 , " 00  "
DATA  16 ,  0 ,  32000 , "   \\\\  "
DATA  17 ,  0 ,  32000 , " lllll  "
DATA  18 ,  0 ,  32000 , "    uuuu  "
DATA  19 ,  0 ,  32000 , "  a  "
DATA  20 ,  0 ,  32000 , "       "
DATA  21 ,  0 ,  32000 , " hh     "
DATA  22 ,  0 ,  32000 , "  h   "
DATA  23 ,  0 ,  32000 , " )"
DATA  24 ,  0 ,  32000 , "    5   "
DATA  25 ,  0 ,  32000 , "  2 "
DATA  26 ,  0 ,  32000 , " -   "
DATA  27 ,  0 ,  32000 , "dd "
DATA  28 ,  0 ,  32000 , "  tttt   "
DATA  29 ,  0 ,  32000 , "  ::::   "
DATA  30 ,  0 ,  32000 , "   ___   "
DATA  31 ,  0 ,  32000 , "     "
DATA  32 ,  0 ,  32000 , "       "
DATA  33 ,  0 ,  32000 , " _____  "
DATA  34 ,  0 ,  32000 , " S "
DATA  35 ,  0 ,  32000 , "  FFFF "
DATA  36 ,  0 ,  32000 , "    kkk"
DATA  37 ,  0 ,  32000 , "    NNNN  "
DATA  38 ,  0 ,  32000 , "   mmmm  "
DATA  39 ,  0 ,  32000 , "    ,,,,, "
DATA  40 ,  0 ,  32000 , "OO   "
DATA  41 ,  0 ,  32000 , "   HHHH  "
DATA  42 ,  0 ,  32000 , " < "
DATA  43 ,  0 ,  32000 , "   j    "
DATA  44 ,  0 ,  32000 , "    yyyy    "
DATA  45 ,  0 ,  32000 , "  +++ "
DATA  46 ,  0 ,  32000 , "o  "
DATA  47 ,  0 ,  32000 , "    (("
DATA  48 ,  0 ,  32000 , "     uu     "
DATA  49 ,  0 ,  32000 , "    00     "
DATA  50 ,  0 ,  32000 , "     HH  "
DATA  51 ,  0 ,  32000 , "     "
DATA  52 ,  0 ,  32000 , "  G   "
DATA  53 ,  0 ,  32000 , "    {{{{ "
DATA  54 ,  0 ,  32000 , "    ]]]     "
DATA  55 ,  0 ,  32000 , "CCC"
DATA  56 ,  0 ,  32000 , "     44  "
DATA  57 ,  0 ,  32000 , "^^  "
DATA  58 ,  0 ,  32000 , "    QQ    "
DATA  59 ,  0 ,  32000 , "     FFFF  "
DATA  60 ,  0 ,  32000 , " n   "
DATA  61 ,  0 ,  32000 , "    hh  "
DATA  62 ,  0 ,  32000 , "555    "
DATA  63 ,  0 ,  32000 , "    YYYY "
DATA  64 ,  0 ,  32000 , "    w     "
DATA  65 ,  0 ,  32000 , "MMMM "
DATA  66 ,  0 ,  32000 , " JJJJ    "
DATA  67 ,  0 ,  32000 , "    %%    "
DATA  68 ,  0 ,  32000 , "     *  "
DATA  69 ,  0 ,  32000 , "    @@@   "
DATA  70 ,  0 ,  32000 , "I "
DATA  71 ,  0 ,  32000 , "    ss  "
DATA  72 ,  0 ,  32000 , "  f "
DATA  73 ,  0 ,  32000 , "   D  "
DATA  74 ,  0 ,  32000 , "###    "
DATA  75 ,  0 ,  32000 , "    '     "
DATA  76 ,  0 ,  32000 , "  dd    "
DATA  77 ,  0 ,  32000 , " TT    "
DATA  78 ,  0 ,  32000 , "  vvv "
DATA  79 ,  0 ,  32000 , "    X "
DATA  80 ,  0 ,  32000 , "    %   "
DATA  81 ,  0 ,  32000 , "  "
DATA  82 ,  0 ,  32000 , " 5555   "
DATA  83 ,  0 ,  32000 , "FFFF   "
DATA  84 ,  0 ,  32000 , "     %%  "
DATA  85 ,  0 ,  32000 , "     "
DATA  86 ,  0 ,  32000 , "  ~~~~    "
DATA  87 ,  0 ,  32000 , "   ||    "
DATA  88 ,  0 ,  32000 , "     SSS     "
DATA  89 ,  0 ,  32000 , "   6666 "
DATA  90 ,  0 ,  32000 , "    ww   "
DATA  91 ,  0 ,  32000 , "  1111   "
DATA  92 ,  0 ,  32000 , " K "
DATA  93 ,  0 ,  32000 , "  ttttt "
DATA  94 ,  0 ,  32000 , " OOOOO   "
DATA  95 ,  0 ,  32000 , " E  "
DATA  96 ,  0 ,  32000 , " PPPPP   "
DATA  97 ,  0 ,  32000 , "    == "
DATA  98 ,  0 ,  32000 , " VVVV    "
DATA  99 ,  0 ,  32000 , " c   "
DATA  100 ,  0 ,  32000 , "    yyyy     "
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
