0100 rem ------------------------------------------
0110 rem Filename: case2.bas
0120 rem  Purpose: Verify CASE IS
0130 rem ------------------------------------------
1000 let correct = 0
1100 let x = 1
1110 select case x
1120 case is < 0
1130   print x; "is negative"
1140 case is > 0
1150   print x; "is positive"
let correct = 1
1160 case else
1170   print x; "is zero"
1180 end select
1190 if correct <> 1 then goto TestFailed
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
