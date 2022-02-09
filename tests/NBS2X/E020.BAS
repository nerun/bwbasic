1000 rem ------------------------------------------
1010 rem Filename: cvd1.bas
1020 rem  Purpose: Verify CVD() and MKD$()
1100 rem ------------------------------------------
1120 A = 0.000000001
1130 B$ = MKD$( A )
1140 C = CVD( B$ )
1150 if A <> C then goto TestFailed
1170 print "*** TEST #1 PASSED ***"
1200 rem ------------------------------------------
1220 A = -0.000000001
1230 B$ = MKD$( A )
1240 C = CVD( B$ )
1250 if A <> C then goto TestFailed
1270 print "*** TEST #2 PASSED ***"
1300 rem ------------------------------------------
1320 A = 123456789
1330 B$ = MKD$( A )
1340 C = CVD( B$ )
1350 if A <> C then goto TestFailed
1370 print "*** TEST #3 PASSED ***"
1400 rem ------------------------------------------
1420 A = -123456789
1430 B$ = MKD$( A )
1440 C = CVD( B$ )
1450 if A <> C then goto TestFailed
1470 print "*** TEST #4 PASSED ***"
1500 rem ------------------------------------------
1520 A = 0
1530 B$ = MKD$( A )
1540 C = CVD( B$ )
1550 if A <> C then goto TestFailed
1570 print "*** TEST #5 PASSED ***"
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
