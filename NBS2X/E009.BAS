0100 rem ------------------------------------------
0110 rem Filename: case3.bas
0120 rem  Purpose: Verify CASE TO
0130 rem ------------------------------------------

Sub Main
   Print "SelCase.bas -- test SELECT CASE statement"
   rem Input "Enter a number"; d
   for d = -11 to 11 step 1

      print "d="; d

      Select Case d

      Case 3 to 5
          print "In Case 3 to 5"
         if d < 3 then goto TestFailed
         if d > 5 then goto TestFailed
         Print "The number is between 3 and 5."

      Case 6
          print "In Case 6"
         if d <> 6 then goto TestFailed
         Print "The number you entered is 6."

      Case 7 to 9
          print "In Case 7 to 9"
         if d < 7 then goto TestFailed
         if d > 9 then goto TestFailed
         Print "The number is between 7 and 9."

      Case If > 10
          print "In Case If > 10"
         if d <= 10 then goto TestFailed
         Print "The number is greater than 10"

      Case If < 0
          print "In Case If < 0"
         if d >= 0 then goto TestFailed
         Print "The number is less than 0"

      Case Else
          print "In Case Else"
         if d <> 0 and d <> 1 and d <> 2 and d <> 10 then goto TestFailed
         Print "The number is 0, 1, 2 or 10."

      End Select

   next d

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
9999 quit

End Sub
CALL MAIN
PRINT "*** SUB MAIN() was not executed ***"
STOP
END
