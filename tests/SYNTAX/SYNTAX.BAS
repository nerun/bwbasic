OPTION VERSION BYWATER
OPTION BUGS OFF
REM PARSE SYNTAX DIAGRAMS AND GENERATE PARAMETER RANGE CHECK TESTS
REM EXAMPLE "     SYNTAX: N  = ASC( A$ )"
REM 
REM THIS PROGRAM ASSUMES A PROPERLY FORMATTED SYNTAX DIAGRAM;
REM ONLY MINIMAL ERROR CHECKING IS PERFORMED.
REM 
REM ------------------------------------------------------------
REM VARIABLE  USAGE
REM    F$     input file name, such as 'SYNTAX.INP'
REM    I$     inpuut line, such as 'SYNTAX: N  = ASC( A$ )'
REM    L      location of '('
REM    R      location of ')'
REM    C      location of ':' or '=' or or ','
REM    P$     current function/parameter
REM    N$()   function/parameter name, P$(0) = 'ASC', P$(1) = 'A$'
REM    T()    function/parameter type,  T(0) = NUMBER, T(1) = STRING
REM             0 == NUMBER, 1 == STRING
REM    N      number of paameters, such as 1
REM    I      generic loop variable
REM    C$     current character
REM    Z$     cleaned syntax diagram
REM    F$     cleaned function name, 'ASC'
REM    X$     user selevted function name, 'COS'
REM    U$     UniqueID, 'F_COS_X'
REM    VN     TESTCASE ACTUAL RESULT, NUMBER
REM    VS$    TESTCASE ACTUAL RESULT, STRING
REM    Q$     CHR$(34)
REM    RN()   RANDOM NUMBERS
REM    RS$()  RANDOM STRINGS
REM    #1     INPUT FILE, 'SYNTAX.TXT'
REM    #2     MERGE FILE, 'SYNTAX.MRG'
REM    #3     OUTPUT FILE, 'F_COS_X.BAS'
REM ------------------------------------------------------------
REM DECLARE ARRAYS
REM ------------------------------------------------------------
REM RANDOMIZE ' this must be off for automated regression
DIM N$(9)
DIM T(9)
DIM RN(100)
DIM RS$(100)
N = 0
GOSUB 4500
REM ------------------------------------------------------------
REM GET THE INPUT FILE NAME
REM ------------------------------------------------------------
F$ = "SYNTAX.INP"
X$ = "ASC"
REM PRINT "F$=[";F$;"]"
REM ------------------------------------------------------------
REM PROCESS THE INPUT FILE
REM ------------------------------------------------------------
OPEN F$ FOR INPUT AS #1
WHILE NOT EOF( 1 )
  LINE INPUT #1, I$
  REM PRINT "I$=[";I$;"]"
  REM Process Line in I$
  GOSUB 1000 ' ProcessLine
  REM IF I$ = X$ THEN
    GOSUB 4000 ' DetermineUniqueID
    GOSUB 5000 ' DumpResults
  REM END IF
WEND
CLOSE #1
PRINT "OK"
GOTO 9999 ' TheEnd


1000 REM ProcessLine
REM ------------------------------------------------------------
REM PROCESS THE SYNTAX DIAGRAM IN I$
REM ------------------------------------------------------------
N = 0
REM PRINT "I$=[";I$;"]"
I$ = TRIM$( I$ )
IF LEN( I$ ) = 0 THEN RETURN
REM PRINT "I$=[";I$;"]"
REM CHECK MISSING COLON
C = INSTR( I$, ":" )
REM PRINT "C=[";C;"]"
IF C = 0 THEN RETURN
REM REMOVE "SYNTAX:"
I$ = MID$( I$, C + 1 )
I$ = TRIM$( I$ )
REM PRINT "I$=[";I$;"]"
IF LEN( I$ ) = 0 THEN RETURN
REM SAVE CLEANED SYNTAX DIAGRAM
Z$ = I$
REM DETERMINE RETURN TYPE
C = INSTR( I$, "=" )
REM PRINT "C=[";C;"]"
IF C = 0 THEN RETURN
P$ = LEFT$( I$, C - 1 )
P$ = TRIM$( P$ )
REM PRINT "P$=[";P$;"]"
IF LEN( P$ ) = 0 THEN RETURN
I$ = MID$( I$, C + 1 )
I$ = TRIM$( I$ )
REM PRINT "I$=[";I$;"]"
IF LEN( I$ ) = 0 THEN RETURN
GOSUB 3000 ' DetermineTypeInP$
REM I$ = "ASC( A$ )"
REM PRINT "I$=[";I$;"]"
L = INSTR( I$, "(" )
R = INSTR( I$, ")" )
REM PRINT "L=[";L;"]"
REM PRINT "R=[";R;"]"
IF R < L THEN RETURN ' ERROR ...)...(
IF R = 0 AND L = 0 THEN
  REM "A$ = TIME$"
  REM N$(0) = I$
  REM PRINT "NO PARAMETERS"
  REM SAVE CLEANED FUNCTION NAME
  F$ = I$
  RETURN ' DONE
END IF
REM  "...(...)"
REM R > L AND L > 0
REM SAVE CLEANED FUNCTION NAME
F$ = LEFT$( I$, L-1 )
F$ = TRIM$( F$ )
REM PRINT "F$=[";F$;"]"
I$ = MID$( I$, L+1, R-L-1 )
REM PRINT "I$=[";I$;"]"
I$ = TRIM$( I$ )
REM PRINT "I$=[";I$;"]"
IF LEN( I$ ) = 0 THEN RETURN ' N = POS()
P$ = ""
FOR I = 1 TO LEN( I$ )
  C$  = MID$( I$, I, 1 )
REM PRINT "C$=[";C$;"]"
  IF C$ = "," THEN 
    GOSUB 3000 ' DetermineTypeInP$
    P$ = ""
  ELSE
    P$ = P$ + C$
  END IF
NEXT I
GOSUB 3000 ' DetermineTypeInP$
RETURN


3000 REM DetermineTypeInP$
REM ------------------------------------------------------------
REM DETERMINE THE TYPE IN P$
REM ------------------------------------------------------------
P$ = TRIM$( P$ )
REM PRINT "P$=[";P$;"]"
IF LEN( P$ ) = 0 THEN RETURN
N$(N) = P$
T(N) = SGN( INSTR( P$, "$" ) )
N = N + 1
RETURN

4000 REM DetermineUniqueID
REM ------------------------------------------------------------
REM DETERMINE THE UNIQUEID USING NS() AND T()
REM ------------------------------------------------------------
IF N <= 0 THEN RETURN
U$ = "F_" + F$
FOR I = 1 TO N - 1
    U$ = U$ + "_" + N$(I)
NEXT I
U$ = U$ + "_" + N$(0)
REM PRINT "U$=[";U$;"]"
REM REMOVE "$"
J = LEN(U$)
I$ = ""
FOR I = 1 TO J
  C$ = MID$(U$,I,1)
  IF C$ = "$" THEN
      REM REMOVE
  ELSE
      I$ = I$ + C$
  END IF
NEXT I
U$ = I$
REM PRINT "U$=[";U$;"]"
RETURN

4500 REM LoadRandomValues
REM ------------------------------------------------------------
REM LOAD RANDOM VALUES
REM ------------------------------------------------------------
FOR I = 1 TO 100
    IF I < 25 THEN
       RN(I) = RND * 1.0 - 0.5
    ELSEIF I < 50 THEN
       RN(I) = RND * 255 - 128
    ELSEIF I < 75 THEN
       RN(I) = RND * 64535 - 32768
    ELSE
        RN(I) = RND * 10000000000 - 5000000000
    END IF
    K = 0
    WHILE K < 35 OR K > 126
        K = RND * 128
    WEND            
    RS$(I) = SPACE$( RND * 5 ) + STRING$( RND * 5, K ) + SPACE$( RND * 5 )
NEXT I
REM CERTAIN VALUES THAT SHOULD ALWAYS BE CHECKED
RN(1) = 0
RN(2) = 1
RN(3) = -1
RN(4) = 0.4
RN(5) = -0.4
RN(6) = 0.6
RN(7) = -0.6
RN(8) = 254
RN(9) = 255
RN(10) = 256
RN(11) = 32000
RS$(1) = ""
RS$(2) = " "
RS$(3) = "3"
RS$(4) = "z"
RS$(5) = "  "
RS$(6) = " 3"
RS$(7) = " z"
RS$(8) = "3 "
RS$(9) = "z "
RS$(10) = " 3 "
RS$(11) = " z "
RETURN

5000 REM DumpResults
REM ------------------------------------------------------------
REM DUMP RESULTS
REM ------------------------------------------------------------
REM 
REM THE FOLLOWING FUNCTIONS SHOULD NOT BE AUTOMATED
REM FOR VARIOUS REASONS
REM
IF N <= 0 THEN 
  REM ERROR
  RETURN
END IF
IF INSTR( F$, " " ) THEN
  REM TESTCSE NOT POSSIBLE, INTERNAL FUNCTIONS
  REM such as "DEF FN"
  RETURN
END IF
IF N = 1  THEN
  REM MANUAL TESTCASE REQUIRED, NO PARAMETERS
  REM such as DATE$, TIME$, TIMER
  RETURN
END IF
IF INSTR(".CLOSE.EOF.GET.INPUT$.LOC.LOF.OPEN.PUT.RESET.", "." + F$ + ".") > 0 THEN
  REM MANUAL TESTCASE REQUIRED, FILE RELATIED
  RETURN
END IF
IF INSTR(".CHDIR.FILES.KILL.MKDIR.NAME.RMDIR.", "." + F$ + ".") > 0 THEN
  REM MANUAL TESTCASE REQUIRED, DIRECTORY RELATED 
  RETURN
END IF
IF INSTR(".COMMAND$.ENVIRON.ENVIRON$.SHELL.", "." + F$ + ".") > 0 THEN
  REM MANUAL TESTCASE REQUIRED, SYSTEM RELATED 
  RETURN
END IF
IF INSTR(".INP.OUT.PEEK.POKE.WAIT.", "." + F$ + ".") > 0 THEN
  REM MANUAL TESTCASE REQUIRED, HARDWARE RELAATED
  RETURN
END IF
IF INSTR(".COLOR.INKEY$.LOCATE.LPOS.LWIDTH.POS.WIDTH.", "." + F$ + ".") > 0 THEN
  REM MANUAL TESTCASE REQUIRED, SCREEN or PRINTER
  RETURN
END IF
IF INSTR(".MKC$.MKD$.MKI$.MKL$.MKS$.", "." + F$ + ".") > 0 THEN
  REM MANUAL TESTCASE REQUIRED, MK*
  RETURN
END IF
IF INSTR(".MOD.OPTION.", "." + F$ + ".") > 0 THEN
  REM MANUAL TESTCASE REQUIRED, MISCELLANEOUS
  RETURN
END IF
IF INSTR(".RANDOMIZE.RND.SPC.TAB.", "." + F$ + ".") > 0 THEN
  REM NO TESTCASE REQUIRED, TESTED BY P###.BAS
  RETURN
END IF
REM 
REM GENERATE TESTCASE
REM 
LET X$ = U$ + ".BAS"
PRINT X$
OPEN X$ FOR OUTPUT AS #3
PRINT #3, "REM ------------------------------------------------------------"
PRINT #3, "REM   PURPOSE: TEST THE PARAMETER RANGE CHECKS"
PRINT #3, "REM    AUTHOR: HOWARD WULF, AF5NE"
PRINT #3, "REM GENERATED: ";DATE$;" ";TIME$
PRINT #3, "REM  FILENAME: ";X$
PRINT #3, "REM    SYNTAX: ";Z$
PRINT #3, "REM  UNIQUEID: ";U$
PRINT #3, "REM  FUNCTION: ";F$
REM ------------------------------------------------------------
REM HAS PARAMETERS
REM N > 1
PRINT #3, "REM "
PRINT #3, "REM","VARIABLE","DESCRIPTION"
PRINT #3, "REM","I"  ,"CURRENT  TESTCASE NUMBER"
PRINT #3, "REM","E"  ,"EXPECTED ERROR FLAG"
PRINT #3, "REM","F"  ,"ACTUAL   ERROR FLAG"
IF T(0) = 0 THEN
PRINT #3, "REM","R"  ,"EXPECTED FUNCTION RESULT"
ELSE
PRINT #3, "REM","R$" ,"EXPECTED FUNCTION RESULT"
END IF
FOR I = 0 TO N - 1
  IF I = 0 THEN
PRINT #3, "REM",N$(I),"ACTUAL   FUNCTION RESULT"
  ELSE
PRINT #3, "REM",N$(I),"ACTUAL   FUNCTION PARAMETER"
  END IF
NEXT I
PRINT #3, "REM "

REM GENERATE TESTS

Q$ = CHR$(34)

PRINT #3, "RESTORE 2000"
PRINT #3, "REM GET TESTCASE #"
PRINT #3, "READ  I"
PRINT #3, "WHILE I > 0"
PRINT #3, "REM GET TESTCASE DATA"
PRINT #3, "READ  E, ";
IF T(0) = 0 THEN
    PRINT #3, "R";
ELSE
    PRINT #3, "R$";
END IF
FOR J = 1 TO N - 1
    PRINT #3, ", ";
    PRINT #3, N$(J);
NEXT J
PRINT #3, ""
PRINT #3, "REM DISPLAY TESTCASE DATA"
PRINT #3, "PRINT ";Q$;"TESTCASE #";Q$;";I,";
PRINT #3, Q$;"E=";Q$;";E,";
IF T(0) = 0 THEN
    PRINT #3, Q$;"R=";Q$;";R";
ELSE
    PRINT #3, Q$;"R$=[";Q$;";R$;";Q$;"]";Q$;
END IF
FOR J = 1 TO N - 1
    PRINT #3, ", ";
    IF T(J) = 0 THEN
        PRINT #3, Q$;N$(J);"=";Q$;";";N$(J);
    ELSE
        PRINT #3, Q$;N$(J);"=[";Q$;";";N$(J);";";Q$;"]";Q$;
    END IF
NEXT J
PRINT #3, ""
PRINT #3, "REM EXECUTE TESTCASE"
PRINT #3, "N = ";0
PRINT #3, "S$ = ";Q$;Q$
PRINT #3, "ON ERROR GOTO 9000"
PRINT #3, "F = 0"
PRINT #3, "999 ";Z$
PRINT #3, "ERROR 0"
PRINT #3, "REM DISPLAY TESTCASE RESULTS"
PRINT #3, "PRINT ,";Q$;"F=";Q$;";F,";
IF T(0) = 0 THEN
    PRINT #3, Q$;N$(0);"=";Q$;";";N$(0)
ELSE
    PRINT #3, Q$;N$(0);"=[";Q$;";";N$(0);";";Q$;"]";Q$
END IF
PRINT #3, "REM VERIFY EXPECTED ERROR"
PRINT #3, "IF E <> F THEN"
PRINT #3, "  PRINT ";Q$;"*** TEST FAILED, EXCEPTION MISMATCH ***";Q$
PRINT #3, "  STOP"
PRINT #3, "END IF"
PRINT #3, "REM VERIFY EXPECTED RESULT"
IF T(0) = 0 THEN
PRINT #3, "IF R > 0 THEN"
PRINT #3, "  LET RMIN = 0.99 * R"
PRINT #3, "  IF R > MAXNUM / 1.012 THEN"
PRINT #3, "    LET RMAX = MAXNUM"
PRINT #3, "  ELSE"
PRINT #3, "    LET RMAX = 1.01 * R"
PRINT #3, "  END IF"
PRINT #3, "END IF"
PRINT #3, "IF R < 0 THEN"
PRINT #3, "  IF R < - MAXNUM / 1.012 THEN"
PRINT #3, "    LET RMIN = -MAXNUM"
PRINT #3, "  ELSE"
PRINT #3, "    LET RMIN = 1.01 * R"
PRINT #3, "  END IF"
PRINT #3, "  LET RMAX = 0.99 * R"
PRINT #3, "END IF"
PRINT #3, "IF R = 0 THEN"
PRINT #3, "  LET RMIN = -.01"
PRINT #3, "  LET RMAX = +.01"
PRINT #3, "END IF"
PRINT #3, "IF ";N$(0);" < RMIN THEN"
PRINT #3, "  PRINT ";Q$;"*** TEST FAILED, RESULT MISMATCH ***";Q$
PRINT #3, "  STOP"
PRINT #3, "END IF"
PRINT #3, "IF ";N$(0);" > RMAX THEN"
PRINT #3, "  PRINT ";Q$;"*** TEST FAILED, RESULT MISMATCH ***";Q$
PRINT #3, "  STOP"
PRINT #3, "END IF"
ELSE
PRINT #3, "IF R$ <> ";N$(0);" THEN"
PRINT #3, "  PRINT ";Q$;"*** TEST FAILED, RESULT MISMATCH ***";Q$
PRINT #3, "  STOP"
PRINT #3, "END IF"
END IF
PRINT #3, "PRINT ";Q$;"*** TEST PASSED ***";Q$
PRINT #3, "REM GET NEXT TESTCASE #"
PRINT #3, "READ  I"
PRINT #3, "WEND"
PRINT #3, "PRINT ";Q$;"*** ALL TESTS PASSED ***";Q$
PRINT #3, "GOTO 9999"
PRINT #3, "1999 REM TESTCASE, EXPECTED ERROR, EXPECTED RESULT, PARAMETERS..."
FOR I = 1 TO 100
    REM ------------------------------------------------------------
    REM CREATE MERGE FILE
    REM ------------------------------------------------------------
    OPEN "SYNTAX.MRG" FOR OUTPUT AS #2
    PRINT #2, "6000 REM MERGE START";I
REM    PRINT #2, "6010 PRINT ";Q$;"BEFORE ERROR";Q$
REM    PRINT #2, "6020 ERROR 12"
REM    PRINT #2, "6030 PRINT ";Q$;"AFTER ERROR";Q$
REM    PRINT #2, "6040 RETURN"
REM    PRINT #2, "6050 PRINT ";Q$;"AFTER RETURN";Q$
    FOR J = 1 TO N - 1
      IF T(J) = 0 THEN       
        K = RN(I)
        PRINT #2, "LET ";N$(J);" = ";K
        V(J) = K
      ELSE
        K$ = RS$(I)
        PRINT #2, "LET ";N$(J);" = ";Q$;K$;Q$
        V$(J) = K$
      END IF
    NEXT J
    PRINT #2, "6010 LET V";Z$
REM    PRINT #2, "6098 PRINT ";Q$;"BEFORE STOP";Q$
    PRINT #2, "6099 REM MERGE STOP";I
    CLOSE #2
    REM EXECUTE MERGED FILE
    DELETE 6000-6099
    MERGE "SYNTAX.MRG"
REM    LIST 5999-6999
    LET E = 0
    LET VN = 0
    LET VS$ = ""
REM    TRON
    ON ERROR RESUME NEXT
    GOSUB 6000
REM    TROFF
    REM DTERMINE ERROR FLAG
    LET E = ABS(SGN(ERR))
    REM CLEAR ERR, ERL, ERROR$
    ERROR 0
    ON ERROR GOTO 0
    REM REPORT RESULTS
    REM TESTCASE NUMBER, EXPECTED ERROR
    PRINT #3, "DATA ";I;", ";E;", ";
    REM EXPECTED RESULT
    IF T(0) = 0 THEN
        PRINT #3, VN;
    ELSE
        PRINT #3, Q$;VS$;Q$;
    END IF
    REM ACTUAL PARAMETERS
    FOR J = 1 TO N - 1
        PRINT #3, ", ";
        IF T(J) = 0 THEN
            PRINT #3, V(J);
        ELSE
            PRINT #3, Q$;V$(J);Q$;
        END IF
    NEXT J
    PRINT #3, ""
NEXT I
PRINT #3, "DATA 0"
PRINT #3, "9000 REM ERROR HANDLER"
PRINT #3, "IF ERL = 999 THEN" 
PRINT #3, "  F = ABS(SGN(ERR))"
PRINT #3, "  RESUME NEXT"
PRINT #3, "END IF"
PRINT #3, "PRINT ";Q$;"*** UNEXPECTED ERROR ON LINE ";Q$;";ERL;";Q$;" ***";Q$
PRINT #3, "9999 END"
CLOSE #3
RETURN

5999 REM BOUNDARY A
6000 REM MERGE START 0
6099 REM MERGE STOP 0
6100 RETURN
6999 REM BOUNDARY B

REM ------------------------------------------------------------
REM EOF
REM ------------------------------------------------------------
9999 END
