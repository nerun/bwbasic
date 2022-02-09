100 rem Filename: date1.bas
110 rem  Purpose: verify DATE
120 rem this test must be executed manually
130 let X = DATE ' YYDDD
140 let Y = FIX( X / 1000 )
150 let D = X - Y * 1000
160 let Y = 1900 + Y
170 print "   The Year:"; Y
180 print "Day of year:"; D
190 input "Is this correct (Y/N)"; A$
200 if UCASE$( A$ ) <> "Y" then goto TestFailed
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
