00100 REM ----------------------------------------------------
00110 REM FILENAME:  option1.bas
00120 REM  PURPOSE:  Verify OPTION ANGLE DEGREE/RADIANS
00130 REM   AUTHOR:  Howard Wulf AF5NE
00140 REM     DATE:  October 9th, 2014
00150 REM     NOTE:  We convert the results to a string 
00160 REM            so we only compare significant digits.
00170 REM ----------------------------------------------------
00180 REM let pi = 3.14159265
01000 REM ----------------------------------------------------
01010 REM Verify COS()
01020 REM ----------------------------------------------------
01030 PRINT "TESTING COS()"
01040 option angle radians
01050 let r = COS( pi / 4 )
01060 let r$ = str$( r )
01070 print "RADIAN COS()="; r$
01080 option angle degrees
01090 let d = COS( 45 )
01110 let d$ = str$( d )
01120 print "DEGREE COS()="; d$
01130 if r$ <> d$ then ERROR 1
02000 REM ----------------------------------------------------
02010 REM Verify SIN()
02020 REM ----------------------------------------------------
02030 PRINT "TESTING COS()"
02040 option angle radians
02050 let r = SIN( pi / 4 )
02060 let r$ = str$( r )
02070 print "RADIAN SIN()="; r$
02080 option angle degrees
02090 let d = SIN( 45 )
02110 let d$ = str$( d )
02120 print "DEGREE SIN()="; d$
02130 if r$ <> d$ then ERROR 1
03000 REM ----------------------------------------------------
03010 REM Verify TAN()
03020 REM ----------------------------------------------------
03030 PRINT "TESTING TAN()"
03040 option angle radians
03050 let r = TAN( pi / 4 )
03060 let r$ = str$( r )
03070 print "RADIAN TAN()="; r$
03080 option angle degrees
03090 let d = TAN( 45 )
03110 let d$ = str$( d )
03120 print "DEGREE TAN()="; d$
03130 if r$ <> d$ then ERROR 1
04000 REM ----------------------------------------------------
04010 REM Verify SEC() = 1/COS()
04020 REM ----------------------------------------------------
04030 PRINT "TESTING SEC()"
04040 option angle radians
04050 let r = SEC( pi / 4 )
04060 let r$ = str$( r )
04070 print "RADIAN SEC()="; r$
04080 option angle degrees
04090 let d = SEC( 45 )
04110 let d$ = str$( d )
04120 print "DEGREE SEC()="; d$
04130 if r$ <> d$ then ERROR 1
05000 REM ----------------------------------------------------
05010 REM Verify CSC() = 1/SIN()
05020 REM ----------------------------------------------------
05030 PRINT "TESTING CSC()"
05040 option angle radians
05050 let r = CSC( pi / 4 )
05060 let r$ = str$( r )
05070 print "RADIAN CSC()="; r$
05080 option angle degrees
05090 let d = CSC( 45 )
05110 let d$ = str$( d )
05120 print "DEGREE CSC()="; d$
05130 if r$ <> d$ then ERROR 1
06000 REM ----------------------------------------------------
06010 REM Verify COT() = 1/TAN()
06020 REM ----------------------------------------------------
06030 PRINT "TESTING COT()"
06040 option angle radians
06050 let r = COT( pi / 4 )
06060 let r$ = str$( r )
06070 print "RADIAN COT()="; r$
06080 option angle degrees
06090 let d = COT( 45 )
06110 let d$ = str$( d )
06120 print "DEGREE COT()="; d$
06130 if r$ <> d$ then ERROR 1
07000 REM ----------------------------------------------------
07010 REM Verify ACOS()
07020 REM ----------------------------------------------------
07030 PRINT "TESTING COS()"
07040 option angle radians
07050 let r = ACOS( 0.7071068 )
07060 let r$ = str$( r )
07070 print "RADIAN ACOS()="; r$
07080 option angle degrees
07090 let d = INT( ACOS( 0.7071068 ) + 0.5 )
07110 let d$ = str$( d )
07120 print "DEGREE ACOS()="; d$
07130 if left$(r$,9) <> " .785398" or d$ <> " 45" then ERROR 1
08000 REM ----------------------------------------------------
08010 REM Verify ASIN()
08020 REM ----------------------------------------------------
08030 PRINT "TESTING ASIN()"
08040 option angle radians
08050 let r = ASIN( 0.7071068 )
08060 let r$ = str$( r )
08070 print "RADIAN ASIN()="; r$
08080 option angle degrees
08090 let d = INT( ASIN( 0.7071068 ) + 0.5 )
08110 let d$ = str$( d )
08120 print "DEGREE ASIN()="; d$
08130 if left$(r$,9) <> " .785398" or d$ <> " 45" then ERROR 1
09000 REM ----------------------------------------------------
09010 REM Verify ATN()
09020 REM ----------------------------------------------------
09030 PRINT "TESTING ATN()"
09040 option angle radians
09050 let r = ATN( 1.0 )
09060 let r$ = str$( r )
09070 print "RADIAN ATN()="; r$
09080 option angle degrees
09090 let d = INT( ATN( 1.0 ) + 0.5 )
09110 let d$ = str$( d )
09120 print "DEGREE ATN()="; d$
09130 if left$(r$,9) <> " .785398" or d$ <> " 45" then ERROR 1
19900 REM ----------------------------------------------------
19910 REM *** ALL TESTS PASSED ***
19920 REM ----------------------------------------------------
19930 PRINT "*** ALL TESTS PASSED ***"
19999 QUIT


