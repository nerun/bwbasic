1     rem This is the General Check Printer


%INCLUDE ALL.BAS
500 fmt2$="    /...5../       /...5....0....5....0.../"
fmt3$="                                          "
fmt4$=fmt3$+"                  /...5....0....5....0./"
fmt6$=fmt3$+"                        ##########          ###,###.##-"
fmt15$="         ##########"+fmt3$
fmt15$=fmt15$+"                      $$###,###.##"
fmt30$="         /...5....0....5....0.../                  /...5../"
fmt30$=fmt30$+"  ##########       $$######.##"


     dim TC%(28),n(2,20),k$(3,20),h(9),s(9),t(4,9)
     print clear$:print
 uline$="":for h%=1 to 22:uline$=uline$+"-":next h%
 z5$="glcd":z7$="glcdsize"

     RESTORE
1075 N(1,3)=8:N(1,4)=24:N(1,5)=24:N(1,6)=1

 N$=""

8000 PRINT clear$:PRINT
8050 PRINT "THIS IS THE GENERAL CHECK PRINTING PROGRAM."
8060 PRINT:PRINT:PRINT:PRINT
 print"Before you begin--set the FORM LENGTH SELECTION"
 print"DIAL on the printer to the 7 inch form length"
 print"position ***"
 print
 print"Now set the checks to the top-of-form position"
 print"for your printer/check combination."
 print
 print"Be sure that you have posted the CHECK DISBURSEMENTS"
 print"FILE--the checks will NOT be printed if this has not"
 print"been done first ***"
 print
 input "Type <cr> to continue";line temp$
8080 PRINT clear$:print
8100 print "Would you like a ***VOID*** check printed to"
 print "establish print positions ? (Y/N)"

 input line temp$

 if ucase$(left$(temp$,1))="Y" then 10000
 if ucase$(left$(temp$,1))<>"N" then 8000


 open z7$ as 1
  read #1;z2,z3
  close 1

8240 Z2=z2-1
8270 PRINT clear$:PRINT
8300 PRINT
8310 INPUT "TO PRINT CHECKS, DO CARRIAGE RETURN. ";line temp$

 open z5$ recl 250 as 1

8325 lprinter
8330 FOR Z5=1 TO Z2
8340 read #1,z5;N(2,1),N(2,2),N$,N(2,7),N(2,8),N(2,9), N(2,10),N(2,11),N(2,12),N(2,13),N(2,14),N(2,15),N(2,16), N(2,17),N(2,18),N(2,19),FLAG$

8395 Z9=1:FOR Z=3 TO 6:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z

 if FLAG$<>"P" then 12000 rem file not posted
 if K$(3,6)="N" then 8720   rem skip checks
 if N(2,7)=0 then 8720 rem skip checks with zero amount

8400 REM *** AND PRINT THE CHECK ***
8410 PRINT
 print using fmt2$;k$(3,3),k$(3,5)
 print
 print using fmt4$;"Account Control Number"
 print using fmt4$;uline$
8450 for i%=8 to 18 step 2
 if N(2,i%)=0 then print:goto 8470
 print using fmt6$;N(2,i%),N(2,i%+1)
8470 next i%
 print:print:print
 print using fmt15$;N(2,2),ABS(N(2,7))
8480 for j%=1 to 14:print:next j%
 print using fmt30$;k$(3,4),d$(1),N(2,2),ABS(N(2,7))
 print chr$(12)


 K$(3,6)="N"
 N$=""
 FOR Z=3 TO 6:N$=N$+K$(3,Z):NEXT Z

8600 print #1,z5;N(2,1),N(2,2),N$,N(2,7),N(2,8),N(2,9), N(2,10),N(2,11),N(2,12),N(2,13),N(2,14),N(2,15),N(2,16), N(2,17),N(2,18),N(2,19),FLAG$

8720 NEXT Z5
8730 close 1
8740 console
8745 print clear$:print
 print "All checks have been printed"
 print:print
 print "It is a good idea to review the check"
 print "numbers in the CHECK DISBURSEMENTS FILE,"
 print "especially if you have printed any ***VOID***"
 print "checks that you did not anticipate"
 print
 input "Type <cr> to continue";line temp$
8747 print clear$:print
 print "*** Return the printer FORM LENGTH CONTROL "
 print "to its normal setting (11 inches) ***"
 print
 input "Type <cr> to continue ";line temp$
8750 CHAIN "MASTER11"


10000 REM *** AND PRINT A ***VOID*** CHECK ***

 lprinter

 k$(3,5)="*** VOID ***"
10010 print
 print using fmt2$;D$(1),k$(3,5)

10020 for i%=1 to 12:print:next i%

 print using fmt15$;N(2,2),N(2,7)
10030 for j%=1 to 14:print:next j%
 print using fmt30$;k$(3,5),d$(1),N(2,2),N(2,7)
 print chr$(12)

 console
 goto 8100

12000 print clear$:print
 print "The CHECK DISBURSEMENTS have not been posted!!"
 print "You must do so before the checks can be printed!!"
 print 
 input "Type <cr> to continue ";line temp$
12040 console
12050 goto 8747
