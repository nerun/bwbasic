1     rem This is the General Ledger Subheading Entry Program

%INCLUDE ALL.BAS
500     RESTORE
505 z5$="gls"
506 z6$="glsback"
 z7$="glssize"
515 repeat$="-------------------------------"
516 fill$="                               "
520 dim z$(5,5),n(2,20),k$(6,20)
    for z=1 to 80:delim$=delim$+"*":next z
    for z=1 to 80:delim1$=delim1$+"=":next z
540 K$(6,1)="1 - NEW ENTRY":K$(6,2)="2 - EXAMINE EXISTING ENTRY"
550 K$(6,3)="3 - MODIFY EXISTING ENTRY"
560 K$(6,4)="4 - DELETE EXISTING ENTRY"
570 K$(6,5)="5 - CREATE NEW FILE":K$(6,6)="6 - CLEAR EXISTING FILE"
580 K$(6,7)="7 - BACK-UP AND SORT FILE":K$(6,8)="8 - LIST FILE"
590 K$(6,9)="9 - FINISHED"
1000 rem THIS IS THE GENERAL LEDGER SUBHEADINGS ENTRY PROGRAM
1005 PRINT clear$
1010 PRINT
1075 K$(1,1)="TAG":K$(1,2)="LEDGER ACCOUNT NUMBER":K$(1,3)="DESCRIPTION"
1080 K$(1,4)="FIRST ACCOUNT NUMBER":K$(1,5)="LAST ACCOUNT NUMBER"
1085 K$(1,6)="M-T-D TOTAL":K$(1,7)="Q-T-D TOTAL"
1090 K$(1,8)="Y-T-D TOTAL":K$(1,9)="M-T-D PERCENTAGE"
1095 K$(1,10)="Q-T-D PERCENTAGE":K$(1,11)="Y-T-D PERCENTAGE"
1105 K$(2,1)="1 - TAG":K$(2,2)="2 - LEDGER ACCOUNT NUMBER"
1106 K$(2,3)="3 - DESCRIPTION"
1110 K$(2,4)="4 - FIRST ACCOUNT NUMBER":K$(2,5)="5 - LAST ACCOUNT NUMBER"
1115 K$(2,6)="6 - M-T-D TOTAL":K$(2,7)="7 - Q-T-D TOTAL"
1120 K$(2,8)="8 - Y-T-D TOTAL":K$(2,9)="9 - M-T-D PERCENTAGE"
1125 K$(2,10)="10- Q-T-D PERCENTAGE":K$(2,11)="11- Y-T-D PERCENTAGE"
1130 K$(2,12)="12- NONE"
1135 N(1,1)=4:N(1,2)=10:N(1,3)=30:N(1,4)=10:N(1,5)=10:N(1,6)=12
1140 N(1,7)=12:N(1,8)=12:N(1,9)=12:N(1,10)=12:N(1,11)=12
 print clear$:print

1500 if end #1 then 6000
 if end #2 then 15000
  open z5$ recl 150 as 1
  close 1
  open z7$ as 2
  read #2;z2,z3
  close 2

1600 REM
     console
1620 PRINT CLEAR$
 if z2>z3 then print "*** OUT OF RECORD SPACE ***"
 PRINT "GENERAL LEDGER SUBHEADING ENTRY PROGRAM"
  PRINT      "---------------------------------------"
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
1682 IF Z=1 THEN Z$(1,1)="N"
1685 ON Z GOSUB 2000,3000,4000,5000,6000,7000,8000,9000,10000
1690 FOR Z=1 TO 20:N(2,Z)=0:NEXT Z:n$=""
1700 FOR Z=1 TO 20:K$(3,Z)="":NEXT Z
1702 Z$(1,1)="":H$=""
1705 GOTO 1600
2000 IF Z2>Z3 THEN RETURN
2001 PRINT CLEAR$:PRINT
2005 PRINT "RECORD NUMBER";Z2:PRINT
2010 PRINT "TYPE OF SUBHEADING - A=ASSETS, L=LIABILITIES,"
2012 PRINT "E=EQUITY, S=SALES, OR X=EXPENSES."
2013 input line S$:S$="S"+S$
     if s$<>"SA" and s$<>"SL" and s$<>"SE" and s$<>"SS" and     s$<>"SX" then 2010
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
2055 input line k$(3,Z):PRINT chr$(13)
 if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2060 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=s$+K$(3,Z)+left$(fill$,I)
 PRINT CLEAR$
2065 FOR Z=4 TO 11
 PRINT CUR$
2070 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2075 PRINT K$(1,Z);TAB(30);
2080 INPUT N(2,Z):PRINT chr$(13)
 PRINT CLEAR$
2085 NEXT Z
2090 N$=""
2095 N$=K$(3,3)
2100 Z1=Z2
2105 PRINT clear$:PRINT
2110 PRINT "RECORD NUMBER";Z1:PRINT
2115 FOR Z=1 TO 2
2120 PRINT K$(2,Z);TAB(30);N(2,Z)
2125 NEXT Z
2135 PRINT K$(2,3);TAB(30);K$(3,3)
2140 FOR Z=4 TO 5:PRINT K$(2,Z);TAB(30);N(2,Z):NEXT Z
2141 FOR Z=6 TO 8:PRINT K$(2,Z);TAB(30);
:     print using "##,###,###.##";n(2,z)
:NEXT Z
2142 FOR Z=9 TO 11:PRINT K$(2,Z);TAB(30);"% ";N(2,Z):NEXT Z
2145 Z=12:PRINT K$(2,Z):PRINT
2150 PRINT "IF AN ITEM IS TO BE CHANGED, TYPE THE APPROPRIATE NUMBER."
2155 PRINT:INPUT Z
2160 PRINT
2165 IF Z>11 THEN 2216
2170 IF Z<1 THEN 2105
2175 IF Z=3 THEN 2200
2180 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2185 PRINT K$(1,Z);TAB(30);
2190 INPUT N(2,Z)
2195 GOTO 2225
2200 PRINT "TYPE OF SUBHEADING - A=ASSETS, L=LIABILITIES,"
2201 PRINT "E=EQUITY, S=SALES, OR X=EXPENSES."
2202 input line S$:S$="S"+S$
     if s$<>"SA" and s$<>"SL" and s$<>"SE" and s$<>"SS" and     s$<>"SX" then 2200
2204 PRINT TAB(30);left$(repeat$,n(1,z))
: PRINT UP$;
2205 PRINT K$(1,Z);TAB(30);
2210 input line K$(3,Z)
 if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2215 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=s$+K$(3,Z)+left$(fill$,I)
2216 N$=""
2220 N$=K$(3,3)
2225 INPUT "ANY MORE CHANGES";line temp$
2230 if left$(temp$,1)="y" or left$(temp$,1)="Y" then 2105
2235 PRINT
2240 INPUT "IS RECORD TO BE ENTERED";line temp$
2245 PRINT
     if left$(temp$,1)="y" or left$(temp$,1)="Y" then 2280
     if left$(temp$,1)<>"n" and left$(temp$,1)<>"N" then 2235
2260 PRINT clear$:PRINT
2270 PRINT "*** RECORD NOT ENTERED ***":PRINT:PRINT
2275 FOR Z=1 TO 200:NEXT Z:RETURN
2280 open z5$ recl 150 as 1
     print #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),          n(2,7),n(2,8),n(2,9),n(2,10),n(2,11)
     close 1
2295 IF Z$(1,1)="N" THEN Z2=Z2+1:open z7$ as 1:print #1;z2,z3:close 1
2300 RETURN
3000 PRINT clear$:PRINT
3005 INPUT "RECORD NUMBER";Z1
3010 IF Z1>=Z2 THEN 3000
3012 IF Z1<1 THEN RETURN
     open z5$ recl 150 as 1
      read #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),         n(2,7),n(2,8),n(2,9),n(2,10),n(2,11)
     close 1
3030 PRINT
3035 FOR Z=1 TO 2
3040 PRINT K$(1,Z);TAB(30);N(2,Z)
3045 NEXT Z
3055 PRINT K$(1,3);TAB(30);N$
3060 FOR Z=4 TO 5:PRINT K$(1,Z);TAB(30);N(2,Z):NEXT Z
3065 FOR Z=6 TO 8:PRINT K$(2,Z);TAB(30);
      print using "##,###,###.##";N(2,Z):NEXT Z
3070 FOR Z=9 TO 11:PRINT K$(2,Z);TAB(30);"% ";N(2,Z):NEXT Z:PRINT
3075 PRINT "FOR A NEW RECORD NUMBER, TYPE N - RETURN.":PRINT
3080 PRINT "IF FINISHED, TYPE F - RETURN.":PRINT
      input line temp$
      if left$(temp$,1)="n" or left$(temp$,1)="N" then 3000
3095 RETURN
4000 PRINT clear$:PRINT
4005 N$=""
4010 INPUT "RECORD NUMBER";Z1
4015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
4020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
4025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOTO 1620
     open z5$ recl 150 as 1
      read #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),         n(2,7),n(2,8),n(2,9),n(2,10),n(2,11)
     close 1
4045 IF N(2,1)=0 THEN PRINT "*** DELETED RECORD ***":GOTO 1620
4060 K$(3,3)=N$
4075 GOTO 2105
5000 PRINT clear$:PRINT
5010 INPUT "RECORD NUMBER";Z1
5015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
5020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
5025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOTO 1620
     open z5$ recl 150 as 1
      read #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),         n(2,7),n(2,8),n(2,9),n(2,10),n(2,11)
     close 1
5045 PRINT "RECORD NUMBER";Z1
5050 PRINT
5055 FOR Z=1 TO 2
5060 PRINT K$(1,Z);TAB(30);N(2,Z)
5065 NEXT Z
5070 Z=3
5075 PRINT K$(1,Z);TAB(30);N$
5080 FOR Z=4 TO 5:PRINT K$(1,Z);TAB(30);N(2,Z):NEXT Z
5085 FOR Z=6 TO 8:PRINT K$(1,Z);TAB(30);
     print using "##,###,###.##";n(2,Z):NEXT Z
5090 FOR Z=9 TO 11:PRINT K$(1,Z);TAB(30);"%";N(2,Z):NEXT Z
5095 PRINT
5100 INPUT "IS RECORD TO BE DELETED (MUST BE YES TO DELETE)";line temp$
     if left$(temp$,1)="n" or left$(temp$,1)="N" then return
     if temp$<>"YES" then 5095
5115 N(2,1)=0
     open z5$ recl 150 as 1
      print #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),         n(2,7),n(2,8),n(2,9),n(2,10),n(2,11)
     close 1
5135 RETURN
6000 PRINT clear$:PRINT
6005 PRINT "IF YOU HAVE ARRIVED HERE, AND HAVE A GENERAL LEDGER SUBHEADING FILE"
6010 PRINT "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6015 PRINT "TYPE THE letter c FOLLOWED BY A RETURN TO CONTINUE."
6020 PRINT
6025 PRINT "IF YOU WISH TO CREATE A NEW FILE, TYPE THE letter n"
6030 PRINT "FOLLOWED BY RETURN.":PRINT
     input line temp$
     if left$(temp$,1)="c" or left$(temp$,1)="C" then initialize:GOTO 1500
6045 PRINT clear$:PRINT
6050 INPUT "NUMBER OF RECORDS DESIRED";Z3
6055 PRINT
 N$=""
     for z=1 to 30:n$=n$+" ":next z
     for z=1 to 20:n(2,z)=0:next z
     create z5$ recl 150 as 1
6070 FOR Z1=1 TO Z3+2
     print #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),          n(2,7),n(2,8),n(2,9),n(2,10),n(2,11)
6085 NEXT Z1
6090 close 1
6095 PRINT clear$:PRINT
6100 PRINT "GENERAL LEDGER SUBHEADING FILE CREATED AND CLEARED.":PRINT
6105 PRINT Z3;"RECORDS CREATED.":PRINT
6110 PRINT "TO CONTINUE, TYPE RETURN.":INPUT line temp$
 Z2=1
 create z7$ as 1:print #1;Z2,Z3:close 1
6115 GOTO 1500
7000 input "ARE YOU SURE !!! (YES or no) ";line temp$
7007 if temp$<>"YES" then return
 N$=""
     for z=1 to 20:n(2,z)=0:next z
     for z=1 to 30:n$=n$+" ":next z
     open z5$ recl 150 as 1
     for z1=1 to z3+2
     print #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),           n(2,7),n(2,8),n(2,9),n(2,10),n(2,11)
     next z1
     close 1
7035 PRINT clear$:PRINT
7040 PRINT "GENERAL LEDGER SUBHEADING FILE CLEARED!":PRINT
7045 input "TO CONTINUE, TYPE RETURN ";line temp$
 Z2=1
 open z7$ as 1:print #1;Z2,Z3:close 1
7060 RETURN
8000 chain "glssort"
9000 PRINT clear$:PRINT:PRINT "LISTING"
9010 P1=1:P2=1:P3=12
9020 lprinter
9030 open z5$ recl 150 as 1
9040 FOR Z0=1 TO Z2/12
9050 GOSUB 9500
9060 GOSUB 9100
9065 IF P3>Z2 THEN GOSUB 9800:RETURN
9070 GOSUB 9300
9080 NEXT Z0
9085 print chr$(12)
9090 RETURN
9100 FOR Z1=P2 TO P3
     read #1,z1;n(2,1),n(2,2),n$,n(2,4),n(2,5),n(2,6),          n(2,7),n(2,8),n(2,9),n(2,10),n(2,11)
9115 if n(2,2)=0 then return
9120 print
9130 print N(2,1);TAB(7);N(2,2);TAB(21);mid$(n$,3,30);
9135 print TAB(53);N(2,4);TAB(65);N(2,5)
9140 print using "##,###,###.##";n(2,6);tab(21);n(2,7);tab(41);n(2,8)
9145 print N(2,9);TAB(21);N(2,10);TAB(41);N(2,11)
9160 NEXT Z1
9165 P2=P2+12:P3=P3+12
9170 RETURN
9300 print
     print
     print "report continues on next page"
     print
9340 print chr$(12)
9350 RETURN
9500 print delim$
9510 print
9520 print "TAG";TAB(7);"ACCOUNT NUMBER";TAB(21);"DESCRIPTION";
9525 print TAB(53);"1ST ACCT #";TAB(65);"2ND ACCT #"
9530 print "M-T-D TOTAL";TAB(21);"Q-T-D TOTAL";TAB(41);"Y-T-D TOTAL"
9540 print "M-T-D PERCENT";TAB(21);"Q-T-D PERCENT";TAB(41);"Y-T-D PERCENT"
9570 print TAB(66);"PAGE NUMBER ";P1:P1=P1+1
9580 print delim1$
9590 print
9600 RETURN
9800 print
     print
     print "end of report"
     print
9840 print chr$(12)
9845 print chr$(12)
9850 CLOSE 1
     console
9870 RETURN
10000 print clear$:print:print "Replace disk #5 in drive A."
      print "Type return to continue."
      input line temp$
      initialize
      chain "bizmii"
15000 print clear$:print:print "CHECKING FILE LENGTH"
 PRINT:PRINT "*** PLEASE WAIT ***"
 open z5$ recl 150 as 1
 z3=(size(z5$)*block.size)/150
 for z2=1 to z3
 read #1,z2;n(2,1),n(2,2)
 if n(2,2)=0 then 15300
 next z2
15300 z3=int(z3)-2
 close 1
 create z7$ as 1
 print #1;z2,z3
 close 1
 GOTO 1500
