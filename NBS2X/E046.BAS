1000 rem ------------------------------------------
1010 rem Filename: cvs1.bas
1020 rem  Purpose: Verify CVS() and MKS$()
1100 rem ------------------------------------------
1120 A = 0.0001
1130 B$ = MKS$( A )
1140 C = CVS( B$ )
1150 R = A / C
1160 if R < 0.99 then goto TestFailed
1170 if R > 1.01 then goto TestFailed
1180 print "*** TEST #1 PASSED ***"
1200 rem ------------------------------------------
1220 A = -0.0001
1230 B$ = MKS$( A )
1240 C = CVS( B$ )
1250 R = A / C
1260 if R < 0.99 then goto TestFailed
1270 if R > 1.01 then goto TestFailed
1280 print "*** TEST #2 PASSED ***"
1300 rem ------------------------------------------
1320 A = 123456789
1330 B$ = MKS$( A )
1340 C = CVS( B$ )
1350 R = A / C
1360 if R < 0.99 then goto TestFailed
1370 if R > 1.01 then goto TestFailed
1380 print "*** TEST #3 PASSED ***"
1400 rem ------------------------------------------
1420 A = -123456789
1430 B$ = MKS$( A )
1440 C = CVS( B$ )
1450 R = A / C
1460 if R < 0.99 then goto TestFailed
1470 if R > 1.01 then goto TestFailed
1480 print "*** TEST #4 PASSED ***"
1500 rem ------------------------------------------
1520 A = 0
1530 B$ = MKS$( A )
1540 C = CVS( B$ )
1570 if C <> 0 then goto TestFailed
1580 print "*** TEST #5 PASSED ***"
8000 rem ------------------------------------------
8010 TestPassed:
8020 print "*** ALL TESTS PASSED ***"
8030 goto TheEnd
8500 rem ------------------------------------------
8510 TestFailed:
8520 print "*** TEST FAILED ***"
8522 print "A="; A
8524 print "C="; C
8530 goto TheEnd
9000 rem ------------------------------------------
9010 TheEnd:
9999 end
