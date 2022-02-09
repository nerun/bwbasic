1     rem This is the Payroll Time Card Entry Program

%INCLUDE ALL.BAS
500     dim n(2,20),k$(6,20)
     z5$="tm"
     z6$="ep"
 z7$="epsize"
     repeat$="-------------------------------"
     fill$="                               "
     l$="$$##,###.##":u$="##########":v$="###.#"
     for z=1 to 80:delim1$=delim1$+"*":delim2$=delim2$+"=":next z
     for z=1 to 80:delim3$=delim3$+"-":next z
 false=0:true=-1
     RESTORE
540 K$(6,1)="1 - NEW ENTRY":K$(6,2)="2 - EXAMINE EXISTING ENTRY"
550 K$(6,3)="3 - MODIFY EXISTING ENTRY"
560 K$(6,4)="4 - NOT USED"
570 K$(6,5)="5 - NOT USED":K$(6,6)="6 - CLEAR EXISTING FILE"
580 K$(6,7)="7 - NOT USED":K$(6,8)="8 - LIST FILE"
590 K$(6,9)="9 - FINISHED"
1075 K$(1,1)="EMPLOYEE #":K$(1,2)="ACCT NUMBER":K$(1,3)="NAME"
1080 K$(1,4)="PAY PERIOD #":K$(1,5)="REPORTABLE TIPS"
1085 K$(1,6)="MISC DEDUCTIONS":K$(1,7)="REGULAR HOURS"
1090 K$(1,8)="TIME & 1/2 HOURS"
1095 K$(1,9)="DOUBLE TIME HOURS"
1100 K$(1,10)="SICK TIME HOURS"
1105 K$(1,11)="HOLIDAY TIME HOURS":K$(1,12)="VACATION HOURS"
1205 K$(2,1)="1 - EMPLOYEE #":K$(2,2)="2 - ACCT NUMBER":K$(2,3)="3 - NAME"
1210 K$(2,4)="4 - PAY PERIOD #":K$(2,5)="5 - REPORTABLE TIPS"
1215 K$(2,6)="6 - MISC DEDUCTIONS":K$(2,7)="7 - REGULAR HOURS"
1220 K$(2,8)="8 - TIME & 1/2 HOURS"
1225 K$(2,9)="9 - DOUBLE TIME HOURS"
1230 K$(2,10)="10- SICK TIME HOURS":K$(2,11)="11- HOLIDAY HOURS"
1235 K$(2,12)="12- VACATION HOURS":K$(2,13)="13- NONE"
     for z=1 to 12:read n(1,z):next z
     data 8,10,24,4,12,12,4,4,4,4,4,4
 print clear$:print

1500 if end #1 then 20000
 if end #2 then 40000
 if end #3 then 6000

  open z6$ recl 512 as 1
  close 1
  open z7$ as 2
  read #2;z2,z3,flag
  close 2
  open z5$ recl 128 as 3
  close 3

1537 IF Z2=1 THEN PRINT "NO PAYROLL RECORDS":CHAIN "master5"
 IF flag=true then 1540
     PRINT:PRINT "Is this an update to an existing set of time cards (Y/N)"
     INPUT line temp$
          IF left$(temp$,1)="y" OR left$(temp$,1)="Y" THEN 1567
1540 INPUT "PAY PERIOD FOR THIS PAYROLL ";N(2,4)
1544 FOR Z=5 TO 12:N(2,Z)=0:NEXT Z
     if end #1 then 6000
1550 open z5$ recl 128 as 1
1552 open z6$ recl 512 as 2
1554 FOR Z=1 TO Z2-1
1556 read #2,z;n(2,1),n(2,2),n$
1558 N$=LEFT$(N$,24)
     print #1,z;N(2,1),N(2,2),N$,N(2,4),N(2,5),N(2,6),          N(2,7),N(2,8),N(2,9),N(2,10),N(2,11),N(2,12)
     next z
     close 1
     close 2

 flag=false:open z7$ as 2:print #2;z2,z3,flag:close 2

1567 Z4=Z2-1
     IF left$(temp$,1)<>"y" AND left$(temp$,1)<>"Y" THEN Z2=1

1600 REM
     console
1620 PRINT CLEAR$
 if z2>z3 then print "*** OUT OF RECORD SPACE ***"
 print "TIME CARD ENTRY PROGRAM"
  print   "-----------------------"
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
1690 FOR Z=5 TO 20:N(2,Z)=0:NEXT Z:N$=""
1700 FOR Z=1 TO 20:K$(3,Z)="":NEXT Z
1702 new$=""
1705 GOTO 1600
2000 PRINT clear$:PRINT
2002 IF Z2>Z4 THEN PRINT "NO MORE EMPLOYEES":FOR Z=1 TO 100:NEXT Z
2003 IF Z2>Z4 THEN GOTO 1600
2005 PRINT "RECORD NUMBER";Z2:PRINT
2008 open z5$ recl 128 as 1
     read #1,z2;n(2,1),n(2,2),n$
2012 K$(3,3)=N$
2014 close 1
2020 PRINT "PLEASE ENTER HOURS FOR ";K$(3,3);"EMPLOYEE # ";N(2,1)
2065 FOR Z=5 TO 12
 PRINT CUR$
2070 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2075 PRINT K$(1,Z);TAB(30);
2080 INPUT N(2,Z):PRINT chr$(13)
 PRINT CLEAR$
2085 NEXT Z
2100 Z1=Z2
2105 PRINT clear$:PRINT
2106 PRINT "RECORD NUMBER";Z1:PRINT
2115 FOR Z=1 TO 2
2120 PRINT K$(1,Z);TAB(30);N(2,Z)
2125 NEXT Z
2135 PRINT K$(1,3);TAB(30);K$(3,3)
2136 PRINT K$(1,4);TAB(30);:print using u$;n(2,4)
2140 FOR Z=5 TO 6
2141 PRINT K$(2,Z);TAB(30);:print using l$;n(2,z)
2142 NEXT Z
2143 FOR Z=7 TO 12
2144 PRINT K$(2,Z);TAB(30);:print using v$;n(2,z)
2145 NEXT Z
2146 PRINT K$(2,13)
2150 PRINT
2152 PRINT "IF AN ITEM IS TO BE CHANGED, TYPE THE APPROPRIATE NUMBER."
2155 PRINT:INPUT Z
2160 PRINT
2165 IF Z>12 THEN 2225
2170 IF Z<5 THEN 2105
2180 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2185 PRINT K$(1,Z);TAB(30);
2190 INPUT N(2,Z)
2225 INPUT "ANY MORE CHANGES";line temp$
2230 IF left$(temp$,1)="y" OR left$(temp$,1)="Y" THEN 2105
2235 PRINT
2240 INPUT "IS RECORD TO BE ENTERED";line temp$
2245 PRINT
2250 IF left$(temp$,1)="y" OR left$(temp$,1)="Y" THEN 2280
2255 IF left$(temp$,1)<>"n" AND left$(temp$,1)<>"N" THEN 2235
2260 PRINT clear$:PRINT
2270 PRINT "*** RECORD NOT ENTERED ***":PRINT:PRINT
2275 FOR Z=1 TO 200:NEXT Z:RETURN
2280 open z5$ recl 128 as 1
     print #1,z1;N(2,1),N(2,2),N$,N(2,4),N(2,5),N(2,6),          N(2,7),N(2,8),N(2,9),N(2,10),N(2,11),N(2,12)
2290 close 1
2295 IF new$="N" THEN Z2=Z2+1
2300 RETURN
3000 PRINT clear$:PRINT
3005 INPUT "RECORD NUMBER";Z1
3010 IF Z1>=Z2 THEN 3000
3012 IF Z1<1 THEN RETURN
3015 open z5$ recl 128 as 1
     read #1,z1;N(2,1),N(2,2),N$,N(2,4),N(2,5),N(2,6),          N(2,7),N(2,8),N(2,9),N(2,10),N(2,11),N(2,12)
3025 close 1
3030 PRINT
3035 FOR Z=1 TO 2
3040 PRINT K$(1,Z);TAB(30);N(2,Z)
3045 NEXT Z
3055 PRINT K$(1,3);TAB(30);N$
3057 PRINT K$(1,4);TAB(30);:print using u$;n(2,4)
3060 FOR Z=5 TO 6
3062 PRINT K$(1,Z);TAB(30);:print using l$;n(2,z)
3065 NEXT Z
3066 FOR Z=7 TO 12
3067 PRINT K$(1,Z);TAB(30);:print using v$;n(2,z)
3068 NEXT Z
3070 PRINT
3075 PRINT "FOR A NEW RECORD NUMBER, TYPE N - RETURN.":PRINT
3080 PRINT "IF FINISHED, TYPE F - RETURN.":PRINT
3085 INPUT line temp$
3090 IF ucase$(temp$)="N" THEN 3000
3095 RETURN
4000 PRINT clear$:PRINT
4005 N$=""
4010 INPUT "RECORD NUMBER";Z1
4015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
4020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
4025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOTO 1620
4030 open z5$ recl 128 as 1
     read #1,z1;N(2,1),N(2,2),N$,N(2,4),N(2,5),N(2,6),          N(2,7),N(2,8),N(2,9),N(2,10),N(2,11),N(2,12)
4040 close 1
4045 IF N(2,1)=0 THEN PRINT "*** DELETED RECORD ***":GOTO 1620
4060 K$(3,3)=N$
4065 PRINT clear$:PRINT
4070 PRINT "RECORD NUMBER";Z1:PRINT
4075 GOTO 2115
5000 print clear$:print
 print "Records are created in conjunction with employee file."
 print:print "Delete records from that file only."
 RETURN
6000 n$="":for z=1 to 30:n$=n$+" ":next z
     for z=1 to 20:n(2,z)=0:next z
6065 create z5$ recl 128 as 1
6070 FOR Z1=1 TO z3+2
     print #1,z1;N(2,1),N(2,2),N$,N(2,4),N(2,5),N(2,6),          N(2,7),N(2,8),N(2,9),N(2,10),N(2,11),N(2,12)
6085 NEXT Z1
6090 close 1
6095 PRINT clear$:PRINT
6100 PRINT "TIME ENTRY FILE CREATED AND CLEARED.":PRINT
6105 PRINT z3;"RECORDS CREATED.":PRINT
6110 PRINT "TO CONTINUE, TYPE RETURN.":INPUT line temp$
6115 GOTO 1500
7000 n$="":for z=1 to 30:n$=n$+" ":next z
     for z=1 to 20:n(2,z)=0:next z
7006 INPUT "ARE YOU SURE !!! (YES OR NO)";line temp$
7007 IF ucase$(temp$)<>"YES" THEN RETURN
     open z5$ recl 128 as 1
7010 FOR Z1=1 TO z3+2
     print #1,z1;N(2,1),N(2,2),N$,N(2,4),N(2,5),N(2,6),          N(2,7),N(2,8),N(2,9),N(2,10),N(2,11),N(2,12)
7025 NEXT Z1
7030 close 1
7035 PRINT clear$:PRINT
7040 PRINT "TIME ENTRY FILE CLEARED!":PRINT
7045 PRINT "TO CONTINUE, TYPE RETURN."
7050 INPUT line temp$
7055 Z2=1
7060 RETURN
8000 print clear$:print
 print "Records are sorted under employee information entry only!!"
 for z=1 to 100:next z
 RETURN
9000 PRINT clear$:PRINT:PRINT "LISTING"
9010 P1=1:P2=1:P3=4
 REM GET DATES ROUTINE
9025 lprinter
9030 open z5$ recl 128 as 1
9040 FOR Z0=1 TO (Z2/4)+1
9050 GOSUB 9500
9060 GOSUB 9100
9065 GOSUB 9300
9080 NEXT Z0
9085 GOSUB 9800
9090 RETURN
9100 FOR Z1=P2 TO P3
     read #1,z1;N(2,1),N(2,2),N$,N(2,4),N(2,5),N(2,6),          N(2,7),N(2,8),N(2,9),N(2,10),N(2,11),N(2,12)
9115 IF N(2,2)=0 THEN 9160
9120 print:print
9122 print delim2$
9125 print "EMPLOYEE #:";TAB(21);:print using u$;n(2,1);
     PRINT TAB(36);"ACCOUNT #:";
9128 print TAB(51);:print using u$;n(2,2)
9130 print "NAME:";TAB(7);N$;TAB(36);"PAY PERIOD:";TAB(51);
     print using v$;n(2,4)
9132 print "REPORTABLE TIPS:";TAB(21);:print using l$;n(2,5);TAB(36);
9134 print "MISC DED.:";TAB(51);:print using l$;n(2,6)
9136 print delim2$
9138 print TAB(11);"REGULAR";TAB(21);"TIME-1/2";TAB(31);"DOUBLE";
9140 print TAB(41);"SICK";TAB(51);"HOLIDAY";TAB(61);"VACATION"
9141 print delim3$
9142 print "Hours:";tab(11);
     print using v$;n(2,7);tab(21);n(2,8);tab(31);n(2,9);tab(41);     n(2,10);tab(51);n(2,11);tab(61);n(2,12)
9148 print delim1$
9160 NEXT Z1
9165 P2=P2+4:P3=P3+4
9170 RETURN
9300 print
     print delim2$
     print "report continues on next page"
     print delim2$
9340 print chr$(12)
9350 RETURN
9500 print delim1$
9510 print
9520 print TAB(18);"PAYROLL HOURS FOR PERIOD ENDING ";d$(2)
9540 print delim1$
9550 print:print
9590 print
9600 RETURN
9800 print
     print delim2$
     print "End of Report"
     print delim1$
9840 print chr$(12):FOR Z=1 TO 100:NEXT Z:print chr$(12)
9850 close 1
9860 console
9870 RETURN
10000 PRINT clear$:PRINT
10010 PRINT "A LISTING WILL AUTOMATICALLY BE GENERATED UNLESS YOU"
10020 PRINT "ENTER THE PROPER PASSWORD - ":INPUT line temp$
10030 IF ucase$(temp$)="NOLIST" THEN 10050
10040 GOSUB 9000
10050 CHAIN "master5"
20000 PRINT clear$:PRINT
20010 PRINT "*** PLEASE CREATE AN EMPLOYEE FILE BEFORE CONTINUING ***"
20020 FOR Z=1 TO 100:NEXT Z
20030 CHAIN "master5"
40000 print clear$:print
 print "There is no 'epsize' file on the disk. Entering the"
 print "Employee Information file (Item #1 on menu) will"
 print "automatically create this file."
 for z=1 to 500:next z
 chain "master5"
