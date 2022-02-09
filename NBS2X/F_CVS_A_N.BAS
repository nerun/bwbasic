REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:20
REM  FILENAME: F_CVS_A_N.BAS
REM    SYNTAX: N  = CVS( A$ )
REM  UNIQUEID: F_CVS_A_N
REM  FUNCTION: CVS
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
999 N  = CVS( A$ )
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
DATA  12 ,  0 ,  5.35553E+7 , " LLLL"
DATA  13 ,  0 ,  1.35632E-19 , "    ''    "
DATA  14 ,  0 ,  4.95564E+28 , "   oo     "
DATA  15 ,  0 ,  1.49237E-19 , " 00  "
DATA  16 ,  0 ,  1.80285E+17 , "   \\\\  "
DATA  17 ,  0 ,  1.14327E+27 , " lllll  "
DATA  18 ,  0 ,  1.35632E-19 , "    uuuu  "
DATA  19 ,  0 ,  1.90689E-19 , "  a  "
DATA  20 ,  0 ,  1.35632E-19 , "       "
DATA  21 ,  0 ,  1.96856E-19 , " hh     "
DATA  22 ,  0 ,  1.96618E-19 , "  h   "
DATA  23 ,  1 ,  0 , " )"
DATA  24 ,  0 ,  1.35632E-19 , "    5   "
DATA  25 ,  0 ,  1.50878E-19 , "  2 "
DATA  26 ,  0 ,  1.35675E-19 , " -   "
DATA  27 ,  1 ,  0 , "dd "
DATA  28 ,  0 ,  7.73665E+31 , "  tttt   "
DATA  29 ,  0 ,  7.10012E-4 , "  ::::   "
DATA  30 ,  0 ,  1.15383E+19 , "   ___   "
DATA  31 ,  0 ,  1.35632E-19 , "     "
DATA  32 ,  0 ,  1.35632E-19 , "       "
DATA  33 ,  0 ,  1.60956E+19 , " _____  "
DATA  34 ,  1 ,  0 , " S "
DATA  35 ,  0 ,  12680. , "  FFFF "
DATA  36 ,  0 ,  1.35632E-19 , "    kkk"
DATA  37 ,  0 ,  1.35632E-19 , "    NNNN  "
DATA  38 ,  0 ,  3.09728E+27 , "   mmmm  "
DATA  39 ,  0 ,  1.35632E-19 , "    ,,,,, "
DATA  40 ,  0 ,  1.35788E-19 , "OO   "
DATA  41 ,  0 ,  163968. , "   HHHH  "
DATA  42 ,  1 ,  0 , " < "
DATA  43 ,  0 ,  4.8395E+25 , "   j    "
DATA  44 ,  0 ,  1.35632E-19 , "    yyyy    "
DATA  45 ,  0 ,  6.0796E-13 , "  +++ "
DATA  46 ,  1 ,  0 , "o  "
DATA  47 ,  0 ,  1.35632E-19 , "    (("
DATA  48 ,  0 ,  1.35632E-19 , "     uu     "
DATA  49 ,  0 ,  1.35632E-19 , "    00     "
DATA  50 ,  0 ,  1.35632E-19 , "     HH  "
DATA  51 ,  0 ,  1.35632E-19 , "     "
DATA  52 ,  0 ,  1.68666E-19 , "  G   "
DATA  53 ,  0 ,  1.35632E-19 , "    {{{{ "
DATA  54 ,  0 ,  1.35632E-19 , "    ]]]     "
DATA  55 ,  1 ,  0 , "CCC"
DATA  56 ,  0 ,  1.35632E-19 , "     44  "
DATA  57 ,  0 ,  1.35838E-19 , "^^  "
DATA  58 ,  0 ,  1.35632E-19 , "    QQ    "
DATA  59 ,  0 ,  1.35632E-19 , "     FFFF  "
DATA  60 ,  0 ,  1.3589E-19 , " n   "
DATA  61 ,  0 ,  1.35632E-19 , "    hh  "
DATA  62 ,  0 ,  1.53489E-19 , "555    "
DATA  63 ,  0 ,  1.35632E-19 , "    YYYY "
DATA  64 ,  0 ,  1.35632E-19 , "    w     "
DATA  65 ,  0 ,  2.15275E+8 , "MMMM "
DATA  66 ,  0 ,  3.31431E+6 , " JJJJ    "
DATA  67 ,  0 ,  1.35632E-19 , "    %%    "
DATA  68 ,  0 ,  1.35632E-19 , "     *  "
DATA  69 ,  0 ,  1.35632E-19 , "    @@@   "
DATA  70 ,  1 ,  0 , "I "
DATA  71 ,  0 ,  1.35632E-19 , "    ss  "
DATA  72 ,  0 ,  1.94924E-19 , "  f "
DATA  73 ,  0 ,  640.502 , "   D  "
DATA  74 ,  0 ,  1.38183E-19 , "###    "
DATA  75 ,  0 ,  1.35632E-19 , "    '     "
DATA  76 ,  0 ,  1.68327E+22 , "  dd    "
DATA  77 ,  0 ,  1.79849E-19 , " TT    "
DATA  78 ,  0 ,  1.248E+33 , "  vvv "
DATA  79 ,  0 ,  1.35632E-19 , "    X "
DATA  80 ,  0 ,  1.35632E-19 , "    %   "
DATA  81 ,  1 ,  0 , "  "
DATA  82 ,  0 ,  6.75051E-7 , " 5555   "
DATA  83 ,  0 ,  12689.6 , "FFFF   "
DATA  84 ,  0 ,  1.35632E-19 , "     %%  "
DATA  85 ,  0 ,  1.35632E-19 , "     "
DATA  86 ,  0 ,  8.44477E+37 , "  ~~~~    "
DATA  87 ,  0 ,  3.32568E+36 , "   ||    "
DATA  88 ,  0 ,  1.35632E-19 , "     SSS     "
DATA  89 ,  0 ,  2.38606E-6 , "   6666 "
DATA  90 ,  0 ,  1.35632E-19 , "    ww   "
DATA  91 ,  0 ,  2.57752E-9 , "  1111   "
DATA  92 ,  1 ,  0 , " K "
DATA  93 ,  0 ,  7.73665E+31 , "  ttttt "
DATA  94 ,  0 ,  3.47807E+9 , " OOOOO   "
DATA  95 ,  0 ,  1.35754E-19 , " E  "
DATA  96 ,  0 ,  1.39796E+10 , " PPPPP   "
DATA  97 ,  0 ,  1.35632E-19 , "    == "
DATA  98 ,  0 ,  5.89163E+13 , " VVVV    "
DATA  99 ,  0 ,  1.35853E-19 , " c   "
DATA  100 ,  0 ,  1.35632E-19 , "    yyyy     "
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
