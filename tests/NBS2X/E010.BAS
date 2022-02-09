1000 rem ------------------------------------------
1010 rem Filename: cdbl1.bas
1020 rem  Purpose: Verify CDBL()
1100 rem ------------------------------------------
1110 I = 0
1120 for X = -100.74 to +100.92 step 1.2345
1130 I = I + 1
1140 RESULT = CDBL( X )
1150 print "The CDBL value of "; X; " is"; RESULT
1160 if RESULT <> X then goto TestFailed
1170 print "*** TEST #"; I; " PASSED ***"
1180 next X
8000 rem ------------------------------------------
8010 TestPassed:
8020 print "*** ALL TESTS PASSED ***"
8030 goto TheEnd
8500 rem ------------------------------------------
8510 TestFailed:
8520 print "*** TEST FAILED ***"
8530 goto TheEnd
9000 rem ------------------------------------------
9010 TheEnd:
9999 end
