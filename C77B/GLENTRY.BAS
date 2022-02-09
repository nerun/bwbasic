1     rem This is the General Ledger Entry Program

%INCLUDE ALL.BAS
500 RESTORE
505 z5$="gl"
506 z6$="glback"
 z7$="glsize"
515 repeat$="------------------------------"
516 fill$="                               "
520 dim n(2,20),k$(6,20)
    for z=1 to 80:delim$=delim$+"*":next z
    for z=1 to 80:delim1$=delim1$+"=":next z
540 K$(6,1)="1 - NEW ENTRY":K$(6,2)="2 - EXAMINE EXISTING ENTRY"
550 K$(6,3)="3 - MODIFY EXISTING ENTRY"
560 K$(6,4)="4 - DELETE EXISTING ENTRY"
570 K$(6,5)="5 - CREATE NEW FILE":K$(6,6)="6 - CLEAR EXISTING FILE"
580 K$(6,7)="7 - BACK-UP AND SORT FILE":K$(6,8)="8 - LIST FILE"
590 K$(6,9)="9 - FINISHED"
1000 rem THIS IS THE GENERAL LEDGER ENTRY PROGRAM
1005 PRINT clear$:PRINT
1075 K$(1,1)="TAG":K$(1,2)="LEDGER ACCOUNT NUMBER":K$(1,3)="DESCRIPTION"
1080 K$(1,4)="MONTH TO DATE AMOUNT":K$(1,5)="QUARTER TO DATE AMOUNT"
1085 K$(1,6)="YEAR TO DATE AMOUNT":K$(1,7)="PREV MONTH TO DATE AMOUNT"
1090 K$(1,8)="PREV QUARTER TO DATE AMOUNT"
1095 K$(1,9)="PREV YEAR TO DATE AMOUNT"
1100 K$(1,10)="BUDGETED AMOUNT"
1105 K$(2,1)="1 - TAG":K$(2,2)="2 - LEDGER ACCOUNT NUMBER"
1106 K$(2,3)="3 - DESCRIPTION"
1110 K$(2,4)="4 - MONTH TO DATE AMOUNT":K$(2,5)="5 - QUARTER TO DATE AMOUNT"
1115 K$(2,6)="6 - YEAR TO DATE AMOUNT":K$(2,7)="7 - PREV MONTH TO DATE AMT"
1120 K$(2,8)="8 - PREV QUARTER TO DATE AMT"
1125 K$(2,9)="9 - PREV YEAR TO DATE AMT"
1130 K$(2,10)="10 - BUDGETED AMOUNT":K$(2,11)="11 - NONE"
1135 N(1,1)=4:N(1,2)=10:N(1,3)=30:N(1,4)=12:N(1,5)=12:N(1,6)=12
1140 N(1,7)=12:N(1,8)=12:N(1,9)=12:N(1,10)=12
 print clear$:print

1500 if end #1 then 6000
 if end #2 then 30000
  open z5$ recl 138 as 1
  close 1
  open z7$ as 2
  read #2;z2,z3
  close 2

1600 REM
     console
1620 PRINT CLEAR$
 if z2>z3 then print "*** OUT OF RECORD SPACE ***"
 PRINT "GENERAL LEDGER ENTRY PROGRAM"
  PRINT      "----------------------------"
 PRINT:PRINT "THERE ARE ";Z3;" AVAILABLE RECORDS"
 PRINT "OF THESE THERE ARE:";TAB(30);(Z3-Z2)+1;" RECORDS OPEN"
 PRINT                      ;TAB(30);Z2-1;" RECORDS USED"
 PRINT

1650 PRINT "THIS IS A LIST OF OPERATIONS."
1655 PRINT
1660 FOR Z=1 TO 9:PRINT K$(6,Z):NEXT Z:PRINT
1665 PRINT "INDICATE WHAT YOU WOULD LIKE TO DO BY TYPING"
1670 PRINT "THE CORRESPONDING NUMBER."
1675 PRINT
1680 INPUT Z
 IF Z<1 OR Z>9 then 1620
1682 IF Z=1 THEN new$="N"
1685 ON Z GOSUB 2000,3000,4000,5000,6000,7000,8000,9000,10000
1690 FOR Z=1 TO 20:N(2,Z)=0:NEXT Z:n$=""
1700 FOR Z=1 TO 20:k$(3,z)="":NEXT Z
1702 new$=""
1705 GOTO 1600
2000 IF Z2>Z3 THEN RETURN
2001 PRINT CLEAR$:PRINT
2005 PRINT "RECORD NUMBER";Z2:PRINT
2015 FOR Z=1 TO 2
 PRINT CUR$
2020 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2025 PRINT K$(1,Z);TAB(30);
2030 INPUT N(2,Z):PRINT chr$(13)
 PRINT CLEAR$
2035 NEXT Z
2040 Z=3
 PRINT CUR$
2045 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2050 PRINT K$(1,Z);TAB(30);
2055 INPUT line K$(3,Z):PRINT chr$(13)
 if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2060 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,i)
2090 N$=""
2095 N$=K$(3,3)
2100 Z1=Z2
2105 PRINT clear$;
2110 PRINT "RECORD NUMBER";Z1:PRINT
2115 FOR Z=1 TO 2
2120 PRINT K$(2,Z);TAB(30);N(2,Z)
2125 NEXT Z
2135 PRINT K$(2,3);TAB(30);K$(3,3)
2140 FOR Z=4 TO 10
2141 PRINT K$(2,Z);TAB(30);:print using "##,###,###.##";n(2,z)
2142 NEXT Z
     print k$(2,11)
2145 PRINT
2150 PRINT "IF AN ITEM IS TO BE CHANGED, TYPE THE APPROPRIATE NUMBER."
2155 PRINT:INPUT Z
2160 PRINT
2165 IF Z>10 THEN 2216
2170 IF Z<1 THEN 2105
2175 IF Z=3 THEN 2200
2180 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2185 PRINT K$(1,Z);TAB(30);
2190 INPUT N(2,Z)
2195 GOTO 2225
2200 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2205 PRINT K$(1,Z);TAB(30);
2210 INPUT line K$(3,Z)
 if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2215 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,i)
2216 N$=""
2220 N$=K$(3,3)
2225 INPUT "ANY MORE CHANGES";line temp$
2230 if left$(temp$,1)="y" or left$(temp$,1)="Y" then 2105
2235 PRINT
2240 INPUT "IS RECORD TO BE ENTERED";line temp$
2245 PRINT
2250 if left$(temp$,1)="y" or left$(temp$,1)="Y" then 2280
2255 if left$(temp$,1)<>"n" and left$(temp$,1)<>"N" then 2235
2260 PRINT clear$:PRINT
2270 PRINT "*** RECORD NOT ENTERED ***":PRINT:PRINT
2275 FOR Z=1 TO 200:NEXT Z:RETURN
2280 open z5$ recl 138 as 1
     print #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),n(2,7),           n(2,8),n(2,9),n(2,10)
     close 1
2295 IF new$="N" THEN Z2=Z2+1:open z7$ as 1:print #1;z2,z3:close 1
2300 RETURN
3000 PRINT clear$:PRINT
3005 INPUT "RECORD NUMBER";Z1
3010 IF Z1>=Z2 THEN 3000
3012 IF Z1<1 THEN RETURN
open z5$ recl 138 as 1
     read #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),n(2,7),           n(2,8),n(2,9),n(2,10)
     close 1
3030 PRINT
3035 FOR Z=1 TO 2
3040 PRINT K$(1,Z);TAB(30);N(2,Z)
3045 NEXT Z
3055 PRINT K$(1,3);TAB(30);N$
3060 FOR Z=4 TO 10
3062 PRINT K$(1,Z);TAB(30);:print using "##,###,###.##";n(2,z)
3065 NEXT Z
3070 PRINT
3075 PRINT "FOR A NEW RECORD NUMBER, TYPE N - RETURN.":PRINT
3080 PRINT "IF FINISHED, TYPE F - RETURN.":PRINT
3085 input line temp$
3090 if left$(temp$,1)="n" or left$(temp$,1)="N" then 3000
3095 RETURN
4000 PRINT clear$:PRINT
4005 N$=""
4010 INPUT "RECORD NUMBER";Z1
4015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
4020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
4025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOTO 1620
open z5$ recl 138 as 1
     read #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),n(2,7),           n(2,8),n(2,9),n(2,10)
     close 1
4045 IF N(2,1)=0 THEN PRINT "*** DELETED RECORD ***":GOTO 1620
4060 K$(3,3)=N$
4075 GOTO 2105
5000 PRINT clear$:PRINT
5010 INPUT "RECORD NUMBER";Z1
5015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
5020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
5025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOTO 1620
open z5$ recl 138 as 1
     read #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),n(2,7),           n(2,8),n(2,9),n(2,10)
     close 1
5045 PRINT "RECORD NUMBER";Z1
5050 PRINT
5055 FOR Z=1 TO 2
5060 PRINT K$(1,Z);TAB(30);N(2,Z)
5065 NEXT Z
5070 Z=3
5075 PRINT K$(1,Z);TAB(30);N$
5080 FOR Z=4 TO 10
5085 PRINT K$(1,Z);TAB(30);:print using "##,###,###.##";n(2,z)
5090 NEXT Z
5095 PRINT
5100 INPUT "IS RECORD TO BE DELETED (MUST BE YES TO DELETE)";line temp$
5105 if left$(temp$,1)="n" or left$(temp$,1)="N" then return
5110 if left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" then 5095
5115 N(2,1)=0
     open z5$ recl 138 as 1
     print #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),n(2,7),           n(2,8),n(2,9),n(2,10)
     close 1
5135 RETURN
6000 PRINT clear$:PRINT
6005 PRINT "IF YOU HAVE ARRIVED HERE, AND HAVE A GENERAL LEDGER FILE"
6010 PRINT "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6015 PRINT "TYPE THE letter c FOLLOWED BY A RETURN TO CONTINUE."
6020 PRINT
6025 PRINT "IF YOU WISH TO CREATE A NEW FILE, TYPE THE letter n"
6030 PRINT "FOLLOWED BY RETURN.":PRINT
6035 INPUT line temp$
6040 IF temp$="c" OR temp$="C" THEN INITIALIZE:GOTO 1500
6045 PRINT clear$:PRINT
6050 INPUT "NUMBER OF RECORDS DESIRED";Z3
6055 PRINT
6060 for z=1 to 20:n(2,z)=0: next z
 N$=""
     for z=1 to 30:n$=n$+" ":next z
     create z5$ recl 138 as 1
6070 for z1=1 to z3+2
     print #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),           n(2,7),n(2,8),n(2,9),n(2,10)
     next z1
     close 1
6095 PRINT clear$:PRINT
6100 PRINT "GENERAL LEDGER FILE CREATED AND CLEARED.":PRINT
6105 print z3;" RECORDS CREATED":print
6110 input "TO CONTINUE, TYPE RETURN ";line temp$
 Z2=1
 create z7$ as 1:print #1;Z2,Z3:close 1
6115 GOTO 1500
7000 input "ARE YOU SURE !!! (yes or no) ";line temp$
7007 if temp$<>"yes" AND temp$<>"YES" then return
     for z=1 to 20:n(2,z)=0:next z
 N$=""
     for z=1 to 30:n$=n$+" ":next z
     open z5$ recl 138 as 1
     for z1=1 to z3+2
     print #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),           n(2,7),n(2,8),n(2,9),n(2,10)
     next z1
     close 1
7035 PRINT clear$:PRINT
7040 PRINT "GENERAL LEDGER FILE CLEARED!":PRINT
7045 input "TO CONTINUE, TYPE RETURN ";line temp$
 Z2=1
 open z7$ as 1:print #1;Z2,Z3:close 1
7060 RETURN
8000 CHAIN "GLSORT"
9000 PRINT clear$:PRINT:PRINT "LISTING"
9010 P1=1:P2=1:P3=16
9020 lprinter
9030 open z5$ recl 138 as 1
9040 FOR Z0=1 TO (Z2/16)+1
9050 GOSUB 9500
9060 GOSUB 9100
9065 IF N(2,2)=0 THEN GOSUB 9800:RETURN
9070 GOSUB 9300
9080 NEXT Z0
9085 print chr$(12)
9090 RETURN
9100 FOR Z1=P2 TO P3
9110 read #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),         n(2,7),n(2,8),n(2,9),n(2,10)
9115 IF N(2,2)=0 THEN RETURN
9120 print
9130 print Z1;TAB(8);N(2,1);TAB(14);N(2,2);
9140 print using "##,###,###.##";tab(35);n(2,4);tab(50);n(2,5);tab(65);n(2,6)
 print n$;
9150 print using "##,###,###.##";tab(35);n(2,7);tab(50);n(2,8);tab(65);n(2,9)
9160 NEXT Z1
9165 P2=P2+16:P3=P3+16
9170 RETURN
9300 print
9310 print
9320 print "report continues on next page"
9330 print
9340 print chr$(12)
9350 RETURN
9500 print delim$
9510 print
 print "Rec #";tab(8);"Tag";tab(14);"Acct No.";tab(35);
 print "Month to Date";tab(50);"Quart to Date";tab(65);"Year to Date"
 print "Description";tab(35);"Previous MTD";tab(50);"Previous QTD";
 print tab(65);"Previous YTD"
9570 print TAB(66);"PAGE NUMBER ";P1:P1=P1+1
9585 print delim1$
9590 print
9600 RETURN
9800 print
9810 print
9820 print "END OF REPORT"
9830 print
9840 print chr$(12)
9845 print chr$(12)
9850 CLOSE 1
     console
9870 RETURN
10000 GOSUB 20000
10040 chain "master1"
20000 PRINT clear$:PRINT
20005 PRINT "A NEW GENERAL LEDGER REFERENCE FILE IS BEING"
20010 PRINT "CREATED.":PRINT
20015 PRINT "*** PLEASE WAIT ***"
20025 open z5$ recl 138 as 1
20035 FOR Z2=1 TO Z3+2
20040 read #1,z2;i,j
20045 IF j=0 THEN 20055
20050 NEXT Z2
20055 close 1
20060 Z2=Z2-1
20065 DIM U(Z2)
20070 open z5$ recl 138 as 1
20075 FOR Z=1 TO Z2
20080 read #1,z;i,u(z)
20085 print i,u(z)
20090 NEXT Z
20095 close 1
     if end #2 then 25000
20100 open "glref" as 2
      print #2;z2
20105 for z=1 to z2
20110 print #2;u(z)
20115 next z
20120 close 2
20505 return
25000 create "glref" as 2
      goto 20105
30000 print clear$:print:print "CHECKING FILE LENGTH"
 PRINT:PRINT "*** PLEASE WAIT ***"
 open z5$ recl 138 as 1
 z3=(size(z5$)*block.size)/138
 for z2=1 to z3
 read #1,z2;n(2,1),n(2,2)
 if n(2,2)=0 then 30300
 next z2
30300 z3=int(z3)-2
 close 1
 create z7$ as 1
 print #1;z2,z3
 close 1
 GOTO 1500
