REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:27
REM  FILENAME: F_MAXLEN_A_N.BAS
REM    SYNTAX: N  = MAXLEN( A$ )
REM  UNIQUEID: F_MAXLEN_A_N
REM  FUNCTION: MAXLEN
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
999 N  = MAXLEN( A$ )
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
DATA  1 ,  0 ,  4096 , ""
DATA  2 ,  0 ,  4096 , " "
DATA  3 ,  0 ,  4096 , "3"
DATA  4 ,  0 ,  4096 , "z"
DATA  5 ,  0 ,  4096 , "  "
DATA  6 ,  0 ,  4096 , " 3"
DATA  7 ,  0 ,  4096 , " z"
DATA  8 ,  0 ,  4096 , "3 "
DATA  9 ,  0 ,  4096 , "z "
DATA  10 ,  0 ,  4096 , " 3 "
DATA  11 ,  0 ,  4096 , " z "
DATA  12 ,  0 ,  4096 , " LLLL"
DATA  13 ,  0 ,  4096 , "    ''    "
DATA  14 ,  0 ,  4096 , "   oo     "
DATA  15 ,  0 ,  4096 , " 00  "
DATA  16 ,  0 ,  4096 , "   \\\\  "
DATA  17 ,  0 ,  4096 , " lllll  "
DATA  18 ,  0 ,  4096 , "    uuuu  "
DATA  19 ,  0 ,  4096 , "  a  "
DATA  20 ,  0 ,  4096 , "       "
DATA  21 ,  0 ,  4096 , " hh     "
DATA  22 ,  0 ,  4096 , "  h   "
DATA  23 ,  0 ,  4096 , " )"
DATA  24 ,  0 ,  4096 , "    5   "
DATA  25 ,  0 ,  4096 , "  2 "
DATA  26 ,  0 ,  4096 , " -   "
DATA  27 ,  0 ,  4096 , "dd "
DATA  28 ,  0 ,  4096 , "  tttt   "
DATA  29 ,  0 ,  4096 , "  ::::   "
DATA  30 ,  0 ,  4096 , "   ___   "
DATA  31 ,  0 ,  4096 , "     "
DATA  32 ,  0 ,  4096 , "       "
DATA  33 ,  0 ,  4096 , " _____  "
DATA  34 ,  0 ,  4096 , " S "
DATA  35 ,  0 ,  4096 , "  FFFF "
DATA  36 ,  0 ,  4096 , "    kkk"
DATA  37 ,  0 ,  4096 , "    NNNN  "
DATA  38 ,  0 ,  4096 , "   mmmm  "
DATA  39 ,  0 ,  4096 , "    ,,,,, "
DATA  40 ,  0 ,  4096 , "OO   "
DATA  41 ,  0 ,  4096 , "   HHHH  "
DATA  42 ,  0 ,  4096 , " < "
DATA  43 ,  0 ,  4096 , "   j    "
DATA  44 ,  0 ,  4096 , "    yyyy    "
DATA  45 ,  0 ,  4096 , "  +++ "
DATA  46 ,  0 ,  4096 , "o  "
DATA  47 ,  0 ,  4096 , "    (("
DATA  48 ,  0 ,  4096 , "     uu     "
DATA  49 ,  0 ,  4096 , "    00     "
DATA  50 ,  0 ,  4096 , "     HH  "
DATA  51 ,  0 ,  4096 , "     "
DATA  52 ,  0 ,  4096 , "  G   "
DATA  53 ,  0 ,  4096 , "    {{{{ "
DATA  54 ,  0 ,  4096 , "    ]]]     "
DATA  55 ,  0 ,  4096 , "CCC"
DATA  56 ,  0 ,  4096 , "     44  "
DATA  57 ,  0 ,  4096 , "^^  "
DATA  58 ,  0 ,  4096 , "    QQ    "
DATA  59 ,  0 ,  4096 , "     FFFF  "
DATA  60 ,  0 ,  4096 , " n   "
DATA  61 ,  0 ,  4096 , "    hh  "
DATA  62 ,  0 ,  4096 , "555    "
DATA  63 ,  0 ,  4096 , "    YYYY "
DATA  64 ,  0 ,  4096 , "    w     "
DATA  65 ,  0 ,  4096 , "MMMM "
DATA  66 ,  0 ,  4096 , " JJJJ    "
DATA  67 ,  0 ,  4096 , "    %%    "
DATA  68 ,  0 ,  4096 , "     *  "
DATA  69 ,  0 ,  4096 , "    @@@   "
DATA  70 ,  0 ,  4096 , "I "
DATA  71 ,  0 ,  4096 , "    ss  "
DATA  72 ,  0 ,  4096 , "  f "
DATA  73 ,  0 ,  4096 , "   D  "
DATA  74 ,  0 ,  4096 , "###    "
DATA  75 ,  0 ,  4096 , "    '     "
DATA  76 ,  0 ,  4096 , "  dd    "
DATA  77 ,  0 ,  4096 , " TT    "
DATA  78 ,  0 ,  4096 , "  vvv "
DATA  79 ,  0 ,  4096 , "    X "
DATA  80 ,  0 ,  4096 , "    %   "
DATA  81 ,  0 ,  4096 , "  "
DATA  82 ,  0 ,  4096 , " 5555   "
DATA  83 ,  0 ,  4096 , "FFFF   "
DATA  84 ,  0 ,  4096 , "     %%  "
DATA  85 ,  0 ,  4096 , "     "
DATA  86 ,  0 ,  4096 , "  ~~~~    "
DATA  87 ,  0 ,  4096 , "   ||    "
DATA  88 ,  0 ,  4096 , "     SSS     "
DATA  89 ,  0 ,  4096 , "   6666 "
DATA  90 ,  0 ,  4096 , "    ww   "
DATA  91 ,  0 ,  4096 , "  1111   "
DATA  92 ,  0 ,  4096 , " K "
DATA  93 ,  0 ,  4096 , "  ttttt "
DATA  94 ,  0 ,  4096 , " OOOOO   "
DATA  95 ,  0 ,  4096 , " E  "
DATA  96 ,  0 ,  4096 , " PPPPP   "
DATA  97 ,  0 ,  4096 , "    == "
DATA  98 ,  0 ,  4096 , " VVVV    "
DATA  99 ,  0 ,  4096 , " c   "
DATA  100 ,  0 ,  4096 , "    yyyy     "
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
