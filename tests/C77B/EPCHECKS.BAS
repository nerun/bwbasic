1     rem This is the Payroll Check Printer


%INCLUDE ALL.BAS

500     dim TC%(28),n(2,20),k$(3,20),h(9),s(9),t(4,9)
     print clear$:print

 z5$="ep":z6$="tm":z7$="epsize"


 fmt6$="                                                            "
 fmt6$=fmt6$+"                /...5../"

 fmt9$="  ####  ####   ##.##  ####.##  ####.##     -      -   ####.##"
 fmt9$=fmt9$+"    ####.##     ####.##"

 fmt12$="                                  SDI       MISC"

 fmt13$="                                                           "
 fmt13$=fmt13$+"                 ########"

 fmt16$="  ####.##   ####.##   ####.##   ####.##   ####.##          "
 fmt16$=fmt16$+"                  ####.##"

 fmt19$=" /...5...10...15...20.../"

 fmt20$=" /...5...10/                                                "
 fmt20$=fmt20$+"                 ####.##"

 fmt32$="                                           "
 fmt32$=fmt32$+"/...5../   ########      $$#####.##"

 fmt33$="        /...5...10...15...20.../"
 fmt34$="        /...5...10...15...20.../"
 fmt35$="        /...5...10...15./, //  /.../"

     RESTORE
1075 N(1,3)=24:N(1,4)=24:N(1,5)=24:N(1,6)=17:N(1,7)=2:N(1,8)=5
1080 N(1,9)=14:N(1,10)=11:N(1,11)=8:N(1,12)=8:N(1,13)=1:N(1,14)=1:N(1,15)=1

 N$=""

8000 PRINT clear$:PRINT
8050 PRINT "THIS IS THE PAYROLL CHECK PRINTING PROGRAM."
8060 PRINT
8070 PRINT "SET PRINTER TO TOP OF FORM ****"
8080 PRINT
8100 print "Would you like a ***VOID*** check printed to"
 print "establish print positions ? (Y/N)"

 input line temp$

 if ucase$(left$(temp$,1))="Y" then GOSUB 10000
 if ucase$(left$(temp$,1))<>"N" then 8000


 open z7$ as 1
  read #1;z2,z3
  close 1

8240 Z2=z2-1
8270 PRINT clear$:PRINT
8290 PRINT "CHECKS TO BE PRINTED = ",Z2
8300 PRINT
8310 INPUT "TO PRINT CHECKS, DO CARRIAGE RETURN. ";line temp$

 open z5$ recl 512 as 1
 open z6$ recl 128 as 2

8325 lprinter
8330 FOR Z5=1 TO Z2
8340 read #1,z5;N(2,1),N(2,2),N$,N,R,H1,H2,H3,          E0,E1,E2,F1,F2,F3,E3,E4,E5,E6,E7,E8,S1,S2,S3,M1,M2,M3

 read #2,z5;temp1,temp2,o$,temp3,tips,temp4,temp5,  ot,dt,st,ht,vt

8390 P4=E0-S1-M1-E3-E6-F1

8395 Z9=1:FOR Z=3 TO 15:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z

 temp5=temp5+st+ht+vt

 if ucase$(k$(3,14))="S" then ot=0:dt=0

 if ucase$(k$(3,14))="S" and ucase$(k$(3,15))="W" then temp5=40
 if ucase$(k$(3,14))="S" and ucase$(k$(3,15))="B" then temp5=80
 if ucase$(k$(3,14))="S" and ucase$(k$(3,15))="S" then temp5=86.66667
 if ucase$(k$(3,14))="S" and ucase$(k$(3,15))="M" then temp5=173.33333

 if E0=0 then 8720   rem skip checks with zero gross pay

8400 REM *** AND PRINT THE CHECK ***

 for z=1 to 5:print:next z

 print using fmt6$;d$(2)

 print:print

 print using fmt9$;temp5,ot+dt,r,temp5*r,(ot*r*1.5)+(dt*r*2),  tips,e0-tips,e0

 print:print

 print fmt12$

 print using fmt13$;temp1

 print:print

 print using fmt16$;f1,e3,e6,s1,m1,s1+f1+e3+e6+m1

 print:print

 print using fmt19$;o$
 print using fmt20$;k$(3,10),p4

 for z=1 to 11:print:next z

 print using fmt32$;d$(1),temp1,p4
 print using fmt33$;k$(3,3)
 print using fmt34$;k$(3,5)
 print using fmt35$;k$(3,6),k$(3,7),k$(3,8)

 for z=1 to 7:print:next z


8720 NEXT Z5
8730 close 1:close 2
8740 console
8750 CHAIN "master5"


10000 REM *** AND PRINT A ***VOID*** CHECK ***

 lprinter

 k$(3,3)="*** VOID ***"

 for z=1 to 5:print:next z

 print using fmt6$;d$(2)

 print:print

 print using fmt9$;temp5,ot+dt,r,temp5*r,(ot*r*1.5)+(dt*r*2),  tips,e0-tips,e0

 print:print

 print fmt12$

 print using fmt13$;temp1

 print:print

 print using fmt16$;f1,e3,e6,s1,m1,s1+f1+e3+e6+m1

 print:print

 print using fmt19$;o$
 print using fmt20$;k$(3,10),p4

 for z=1 to 11:print:next z

 print using fmt32$;d$(1),temp1,p4
 print using fmt33$;k$(3,3)
 print using fmt34$;k$(3,5)
 print using fmt35$;k$(3,6),k$(3,7),k$(3,8)

 for z=1 to 7:print:next z

 console
 RETURN
