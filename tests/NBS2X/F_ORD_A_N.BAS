REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:29
REM  FILENAME: F_ORD_A_N.BAS
REM    SYNTAX: N  = ORD( A$ )
REM  UNIQUEID: F_ORD_A_N
REM  FUNCTION: ORD
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
999 N  = ORD( A$ )
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
DATA  2 ,  0 ,  32 , " "
DATA  3 ,  0 ,  51 , "3"
DATA  4 ,  0 ,  122 , "z"
DATA  5 ,  1 ,  0 , "  "
DATA  6 ,  1 ,  0 , " 3"
DATA  7 ,  1 ,  0 , " z"
DATA  8 ,  1 ,  0 , "3 "
DATA  9 ,  1 ,  0 , "z "
DATA  10 ,  1 ,  0 , " 3 "
DATA  11 ,  1 ,  0 , " z "
DATA  12 ,  1 ,  0 , " LLLL"
DATA  13 ,  1 ,  0 , "    ''    "
DATA  14 ,  1 ,  0 , "   oo     "
DATA  15 ,  1 ,  0 , " 00  "
DATA  16 ,  1 ,  0 , "   \\\\  "
DATA  17 ,  1 ,  0 , " lllll  "
DATA  18 ,  1 ,  0 , "    uuuu  "
DATA  19 ,  1 ,  0 , "  a  "
DATA  20 ,  1 ,  0 , "       "
DATA  21 ,  1 ,  0 , " hh     "
DATA  22 ,  1 ,  0 , "  h   "
DATA  23 ,  1 ,  0 , " )"
DATA  24 ,  1 ,  0 , "    5   "
DATA  25 ,  1 ,  0 , "  2 "
DATA  26 ,  1 ,  0 , " -   "
DATA  27 ,  1 ,  0 , "dd "
DATA  28 ,  1 ,  0 , "  tttt   "
DATA  29 ,  1 ,  0 , "  ::::   "
DATA  30 ,  1 ,  0 , "   ___   "
DATA  31 ,  1 ,  0 , "     "
DATA  32 ,  1 ,  0 , "       "
DATA  33 ,  1 ,  0 , " _____  "
DATA  34 ,  1 ,  0 , " S "
DATA  35 ,  1 ,  0 , "  FFFF "
DATA  36 ,  1 ,  0 , "    kkk"
DATA  37 ,  1 ,  0 , "    NNNN  "
DATA  38 ,  1 ,  0 , "   mmmm  "
DATA  39 ,  1 ,  0 , "    ,,,,, "
DATA  40 ,  1 ,  0 , "OO   "
DATA  41 ,  1 ,  0 , "   HHHH  "
DATA  42 ,  1 ,  0 , " < "
DATA  43 ,  1 ,  0 , "   j    "
DATA  44 ,  1 ,  0 , "    yyyy    "
DATA  45 ,  1 ,  0 , "  +++ "
DATA  46 ,  1 ,  0 , "o  "
DATA  47 ,  1 ,  0 , "    (("
DATA  48 ,  1 ,  0 , "     uu     "
DATA  49 ,  1 ,  0 , "    00     "
DATA  50 ,  1 ,  0 , "     HH  "
DATA  51 ,  1 ,  0 , "     "
DATA  52 ,  1 ,  0 , "  G   "
DATA  53 ,  1 ,  0 , "    {{{{ "
DATA  54 ,  1 ,  0 , "    ]]]     "
DATA  55 ,  1 ,  0 , "CCC"
DATA  56 ,  1 ,  0 , "     44  "
DATA  57 ,  1 ,  0 , "^^  "
DATA  58 ,  1 ,  0 , "    QQ    "
DATA  59 ,  1 ,  0 , "     FFFF  "
DATA  60 ,  1 ,  0 , " n   "
DATA  61 ,  1 ,  0 , "    hh  "
DATA  62 ,  1 ,  0 , "555    "
DATA  63 ,  1 ,  0 , "    YYYY "
DATA  64 ,  1 ,  0 , "    w     "
DATA  65 ,  1 ,  0 , "MMMM "
DATA  66 ,  1 ,  0 , " JJJJ    "
DATA  67 ,  1 ,  0 , "    %%    "
DATA  68 ,  1 ,  0 , "     *  "
DATA  69 ,  1 ,  0 , "    @@@   "
DATA  70 ,  1 ,  0 , "I "
DATA  71 ,  1 ,  0 , "    ss  "
DATA  72 ,  1 ,  0 , "  f "
DATA  73 ,  1 ,  0 , "   D  "
DATA  74 ,  1 ,  0 , "###    "
DATA  75 ,  1 ,  0 , "    '     "
DATA  76 ,  1 ,  0 , "  dd    "
DATA  77 ,  1 ,  0 , " TT    "
DATA  78 ,  1 ,  0 , "  vvv "
DATA  79 ,  1 ,  0 , "    X "
DATA  80 ,  1 ,  0 , "    %   "
DATA  81 ,  1 ,  0 , "  "
DATA  82 ,  1 ,  0 , " 5555   "
DATA  83 ,  1 ,  0 , "FFFF   "
DATA  84 ,  1 ,  0 , "     %%  "
DATA  85 ,  1 ,  0 , "     "
DATA  86 ,  1 ,  0 , "  ~~~~    "
DATA  87 ,  1 ,  0 , "   ||    "
DATA  88 ,  1 ,  0 , "     SSS     "
DATA  89 ,  1 ,  0 , "   6666 "
DATA  90 ,  1 ,  0 , "    ww   "
DATA  91 ,  1 ,  0 , "  1111   "
DATA  92 ,  1 ,  0 , " K "
DATA  93 ,  1 ,  0 , "  ttttt "
DATA  94 ,  1 ,  0 , " OOOOO   "
DATA  95 ,  1 ,  0 , " E  "
DATA  96 ,  1 ,  0 , " PPPPP   "
DATA  97 ,  1 ,  0 , "    == "
DATA  98 ,  1 ,  0 , " VVVV    "
DATA  99 ,  1 ,  0 , " c   "
DATA  100 ,  1 ,  0 , "    yyyy     "
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
