REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:20
REM  FILENAME: F_CVI_A_N.BAS
REM    SYNTAX: N  = CVI( A$ )
REM  UNIQUEID: F_CVI_A_N
REM  FUNCTION: CVI
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
999 N  = CVI( A$ )
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
DATA  5 ,  0 ,  8224 , "  "
DATA  6 ,  0 ,  13088 , " 3"
DATA  7 ,  0 ,  31264 , " z"
DATA  8 ,  0 ,  8243 , "3 "
DATA  9 ,  0 ,  8314 , "z "
DATA  10 ,  0 ,  13088 , " 3 "
DATA  11 ,  0 ,  31264 , " z "
DATA  12 ,  0 ,  19488 , " LLLL"
DATA  13 ,  0 ,  8224 , "    ''    "
DATA  14 ,  0 ,  8224 , "   oo     "
DATA  15 ,  0 ,  12320 , " 00  "
DATA  16 ,  0 ,  8224 , "   \\\\  "
DATA  17 ,  0 ,  27680 , " lllll  "
DATA  18 ,  0 ,  8224 , "    uuuu  "
DATA  19 ,  0 ,  8224 , "  a  "
DATA  20 ,  0 ,  8224 , "       "
DATA  21 ,  0 ,  26656 , " hh     "
DATA  22 ,  0 ,  8224 , "  h   "
DATA  23 ,  0 ,  10528 , " )"
DATA  24 ,  0 ,  8224 , "    5   "
DATA  25 ,  0 ,  8224 , "  2 "
DATA  26 ,  0 ,  11552 , " -   "
DATA  27 ,  0 ,  25700 , "dd "
DATA  28 ,  0 ,  8224 , "  tttt   "
DATA  29 ,  0 ,  8224 , "  ::::   "
DATA  30 ,  0 ,  8224 , "   ___   "
DATA  31 ,  0 ,  8224 , "     "
DATA  32 ,  0 ,  8224 , "       "
DATA  33 ,  0 ,  24352 , " _____  "
DATA  34 ,  0 ,  21280 , " S "
DATA  35 ,  0 ,  8224 , "  FFFF "
DATA  36 ,  0 ,  8224 , "    kkk"
DATA  37 ,  0 ,  8224 , "    NNNN  "
DATA  38 ,  0 ,  8224 , "   mmmm  "
DATA  39 ,  0 ,  8224 , "    ,,,,, "
DATA  40 ,  0 ,  20303 , "OO   "
DATA  41 ,  0 ,  8224 , "   HHHH  "
DATA  42 ,  0 ,  15392 , " < "
DATA  43 ,  0 ,  8224 , "   j    "
DATA  44 ,  0 ,  8224 , "    yyyy    "
DATA  45 ,  0 ,  8224 , "  +++ "
DATA  46 ,  0 ,  8303 , "o  "
DATA  47 ,  0 ,  8224 , "    (("
DATA  48 ,  0 ,  8224 , "     uu     "
DATA  49 ,  0 ,  8224 , "    00     "
DATA  50 ,  0 ,  8224 , "     HH  "
DATA  51 ,  0 ,  8224 , "     "
DATA  52 ,  0 ,  8224 , "  G   "
DATA  53 ,  0 ,  8224 , "    {{{{ "
DATA  54 ,  0 ,  8224 , "    ]]]     "
DATA  55 ,  0 ,  17219 , "CCC"
DATA  56 ,  0 ,  8224 , "     44  "
DATA  57 ,  0 ,  24158 , "^^  "
DATA  58 ,  0 ,  8224 , "    QQ    "
DATA  59 ,  0 ,  8224 , "     FFFF  "
DATA  60 ,  0 ,  28192 , " n   "
DATA  61 ,  0 ,  8224 , "    hh  "
DATA  62 ,  0 ,  13621 , "555    "
DATA  63 ,  0 ,  8224 , "    YYYY "
DATA  64 ,  0 ,  8224 , "    w     "
DATA  65 ,  0 ,  19789 , "MMMM "
DATA  66 ,  0 ,  18976 , " JJJJ    "
DATA  67 ,  0 ,  8224 , "    %%    "
DATA  68 ,  0 ,  8224 , "     *  "
DATA  69 ,  0 ,  8224 , "    @@@   "
DATA  70 ,  0 ,  8265 , "I "
DATA  71 ,  0 ,  8224 , "    ss  "
DATA  72 ,  0 ,  8224 , "  f "
DATA  73 ,  0 ,  8224 , "   D  "
DATA  74 ,  0 ,  8995 , "###    "
DATA  75 ,  0 ,  8224 , "    '     "
DATA  76 ,  0 ,  8224 , "  dd    "
DATA  77 ,  0 ,  21536 , " TT    "
DATA  78 ,  0 ,  8224 , "  vvv "
DATA  79 ,  0 ,  8224 , "    X "
DATA  80 ,  0 ,  8224 , "    %   "
DATA  81 ,  0 ,  8224 , "  "
DATA  82 ,  0 ,  13600 , " 5555   "
DATA  83 ,  0 ,  17990 , "FFFF   "
DATA  84 ,  0 ,  8224 , "     %%  "
DATA  85 ,  0 ,  8224 , "     "
DATA  86 ,  0 ,  8224 , "  ~~~~    "
DATA  87 ,  0 ,  8224 , "   ||    "
DATA  88 ,  0 ,  8224 , "     SSS     "
DATA  89 ,  0 ,  8224 , "   6666 "
DATA  90 ,  0 ,  8224 , "    ww   "
DATA  91 ,  0 ,  8224 , "  1111   "
DATA  92 ,  0 ,  19232 , " K "
DATA  93 ,  0 ,  8224 , "  ttttt "
DATA  94 ,  0 ,  20256 , " OOOOO   "
DATA  95 ,  0 ,  17696 , " E  "
DATA  96 ,  0 ,  20512 , " PPPPP   "
DATA  97 ,  0 ,  8224 , "    == "
DATA  98 ,  0 ,  22048 , " VVVV    "
DATA  99 ,  0 ,  25376 , " c   "
DATA  100 ,  0 ,  8224 , "    yyyy     "
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
