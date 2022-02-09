REM ------------------------------------------------------------
REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS
REM    AUTHOR: HOWARD WULF, AF5NE
REM GENERATED: 02/21/2015 14:08:36
REM  FILENAME: F_UCASE_A_S.BAS
REM    SYNTAX: S$ = UCASE$( A$ )
REM  UNIQUEID: F_UCASE_A_S
REM  FUNCTION: UCASE$
REM 
REM	VARIABLE	DESCRIPTION
REM	I	CURRENT  TESTCASE NUMBER
REM	E	EXPECTED ERROR FLAG
REM	F	ACTUAL   ERROR FLAG
REM	R$	EXPECTED FUNCTION RESULT
REM	S$	ACTUAL   FUNCTION RESULT
REM	A$	ACTUAL   FUNCTION PARAMETER
REM 
RESTORE 2000
REM GET TESTCASE #
READ  I
WHILE I > 0
REM GET TESTCASE DATA
READ  E, R$, A$
REM DISPLAY TESTCASE DATA
PRINT "TESTCASE #";I,"E=";E,"R$=[";R$;"]", "A$=[";A$;"]"
REM EXECUTE TESTCASE
N =  0 
S$ = ""
ON ERROR GOTO 9000
F = 0
999 S$ = UCASE$( A$ )
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
DATA  1 ,  0 , "", ""
DATA  2 ,  0 , " ", " "
DATA  3 ,  0 , "3", "3"
DATA  4 ,  0 , "Z", "z"
DATA  5 ,  0 , "  ", "  "
DATA  6 ,  0 , " 3", " 3"
DATA  7 ,  0 , " Z", " z"
DATA  8 ,  0 , "3 ", "3 "
DATA  9 ,  0 , "Z ", "z "
DATA  10 ,  0 , " 3 ", " 3 "
DATA  11 ,  0 , " Z ", " z "
DATA  12 ,  0 , " LLLL", " LLLL"
DATA  13 ,  0 , "    ''    ", "    ''    "
DATA  14 ,  0 , "   OO     ", "   oo     "
DATA  15 ,  0 , " 00  ", " 00  "
DATA  16 ,  0 , "   \\\\  ", "   \\\\  "
DATA  17 ,  0 , " LLLLL  ", " lllll  "
DATA  18 ,  0 , "    UUUU  ", "    uuuu  "
DATA  19 ,  0 , "  A  ", "  a  "
DATA  20 ,  0 , "       ", "       "
DATA  21 ,  0 , " HH     ", " hh     "
DATA  22 ,  0 , "  H   ", "  h   "
DATA  23 ,  0 , " )", " )"
DATA  24 ,  0 , "    5   ", "    5   "
DATA  25 ,  0 , "  2 ", "  2 "
DATA  26 ,  0 , " -   ", " -   "
DATA  27 ,  0 , "DD ", "dd "
DATA  28 ,  0 , "  TTTT   ", "  tttt   "
DATA  29 ,  0 , "  ::::   ", "  ::::   "
DATA  30 ,  0 , "   ___   ", "   ___   "
DATA  31 ,  0 , "     ", "     "
DATA  32 ,  0 , "       ", "       "
DATA  33 ,  0 , " _____  ", " _____  "
DATA  34 ,  0 , " S ", " S "
DATA  35 ,  0 , "  FFFF ", "  FFFF "
DATA  36 ,  0 , "    KKK", "    kkk"
DATA  37 ,  0 , "    NNNN  ", "    NNNN  "
DATA  38 ,  0 , "   MMMM  ", "   mmmm  "
DATA  39 ,  0 , "    ,,,,, ", "    ,,,,, "
DATA  40 ,  0 , "OO   ", "OO   "
DATA  41 ,  0 , "   HHHH  ", "   HHHH  "
DATA  42 ,  0 , " < ", " < "
DATA  43 ,  0 , "   J    ", "   j    "
DATA  44 ,  0 , "    YYYY    ", "    yyyy    "
DATA  45 ,  0 , "  +++ ", "  +++ "
DATA  46 ,  0 , "O  ", "o  "
DATA  47 ,  0 , "    ((", "    (("
DATA  48 ,  0 , "     UU     ", "     uu     "
DATA  49 ,  0 , "    00     ", "    00     "
DATA  50 ,  0 , "     HH  ", "     HH  "
DATA  51 ,  0 , "     ", "     "
DATA  52 ,  0 , "  G   ", "  G   "
DATA  53 ,  0 , "    {{{{ ", "    {{{{ "
DATA  54 ,  0 , "    ]]]     ", "    ]]]     "
DATA  55 ,  0 , "CCC", "CCC"
DATA  56 ,  0 , "     44  ", "     44  "
DATA  57 ,  0 , "^^  ", "^^  "
DATA  58 ,  0 , "    QQ    ", "    QQ    "
DATA  59 ,  0 , "     FFFF  ", "     FFFF  "
DATA  60 ,  0 , " N   ", " n   "
DATA  61 ,  0 , "    HH  ", "    hh  "
DATA  62 ,  0 , "555    ", "555    "
DATA  63 ,  0 , "    YYYY ", "    YYYY "
DATA  64 ,  0 , "    W     ", "    w     "
DATA  65 ,  0 , "MMMM ", "MMMM "
DATA  66 ,  0 , " JJJJ    ", " JJJJ    "
DATA  67 ,  0 , "    %%    ", "    %%    "
DATA  68 ,  0 , "     *  ", "     *  "
DATA  69 ,  0 , "    @@@   ", "    @@@   "
DATA  70 ,  0 , "I ", "I "
DATA  71 ,  0 , "    SS  ", "    ss  "
DATA  72 ,  0 , "  F ", "  f "
DATA  73 ,  0 , "   D  ", "   D  "
DATA  74 ,  0 , "###    ", "###    "
DATA  75 ,  0 , "    '     ", "    '     "
DATA  76 ,  0 , "  DD    ", "  dd    "
DATA  77 ,  0 , " TT    ", " TT    "
DATA  78 ,  0 , "  VVV ", "  vvv "
DATA  79 ,  0 , "    X ", "    X "
DATA  80 ,  0 , "    %   ", "    %   "
DATA  81 ,  0 , "  ", "  "
DATA  82 ,  0 , " 5555   ", " 5555   "
DATA  83 ,  0 , "FFFF   ", "FFFF   "
DATA  84 ,  0 , "     %%  ", "     %%  "
DATA  85 ,  0 , "     ", "     "
DATA  86 ,  0 , "  ~~~~    ", "  ~~~~    "
DATA  87 ,  0 , "   ||    ", "   ||    "
DATA  88 ,  0 , "     SSS     ", "     SSS     "
DATA  89 ,  0 , "   6666 ", "   6666 "
DATA  90 ,  0 , "    WW   ", "    ww   "
DATA  91 ,  0 , "  1111   ", "  1111   "
DATA  92 ,  0 , " K ", " K "
DATA  93 ,  0 , "  TTTTT ", "  ttttt "
DATA  94 ,  0 , " OOOOO   ", " OOOOO   "
DATA  95 ,  0 , " E  ", " E  "
DATA  96 ,  0 , " PPPPP   ", " PPPPP   "
DATA  97 ,  0 , "    == ", "    == "
DATA  98 ,  0 , " VVVV    ", " VVVV    "
DATA  99 ,  0 , " C   ", " c   "
DATA  100 ,  0 , "    YYYY     ", "    yyyy     "
DATA 0
9000 REM ERROR HANDLER
IF ERL = 999 THEN
  F = ABS(SGN(ERR))
  RESUME NEXT
END IF
PRINT "*** UNEXPECTED ERROR ON LINE ";ERL;" ***"
9999 END
