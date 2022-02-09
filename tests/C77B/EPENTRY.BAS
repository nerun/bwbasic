1     rem This is the Employee Payroll Record Entry Program

%INCLUDE ALL.BAS
500     dim n(2,20),k$(4,20)
     z5$="ep"
     z6$=z5$+"back":z7$=z5$+"size"
     repeat$="-----------------------------"
     fill$="                              "
     l$="$$##,###.##":u$="##########":v$="####"
     for z=1 to 70:delim$=delim$+"=":next z
 false=0:true=-1
     RESTORE
540 K$(4,1)="1 - NEW ENTRY":K$(4,2)="2 - EXAMINE EXISTING ENTRY"
550 K$(4,3)="3 - MODIFY EXISTING ENTRY"
560 K$(4,4)="4 - DELETE EXISTING ENTRY"
570 K$(4,5)="5 - CREATE NEW FILE":K$(4,6)="6 - CLEAR EXISTING FILE"
580 K$(4,7)="7 - BACK-UP AND SORT FILE":K$(4,8)="8 - LIST FILE"
590 K$(4,9)="9 - FINISHED"
1100 K$(1,1)="EMPLOYEE #":K$(1,2)="ACCT NUMBER":K$(1,3)="NAME"
1105 K$(1,4)="COMPANY":K$(1,5)="ADDRESS":K$(1,6)="CITY":K$(1,7)="STATE"
1110 K$(1,8)="ZIP CODE":K$(1,9)="PHONE (AAA) NNN NNNN"
1130 K$(1,10)="SOC SEC #":K$(1,11)="EMPLOYMENT DATE"
1132 K$(1,12)="TERMINATION DATE"
1135 K$(1,13)="MARITAL STATUS":K$(1,14)="HOURLY OR SALARIED (H OR S)"
1140 K$(1,15)="PAY PERIODS - W/B/S/M":K$(1,16)="NUMBER OF EXEMPTIONS"
1145 K$(1,17)="HOURLY RATE":K$(1,18)="HOURS THIS PERIOD"
1146 K$(1,19)="Q-T-D HOURS":K$(1,20)="Y-T-D HOURS"
1200 K$(2,1)="1 - EMPLOYEE #":K$(2,2)="2 - ACCT NUMBER":K$(2,3)="3 - NAME"
1205 K$(2,4)="4 - COMPANY":K$(2,5)="5 - ADDRESS":K$(2,6)="6 - CITY"
1206 K$(2,7)="7 - STATE"
1210 K$(2,8)="8 - ZIP CODE":K$(2,9)="9 - PHONE (AAA) NNN NNNN"
1230 K$(2,10)="10- SOC SEC #":K$(2,11)="11- EMPLOYMENT DATE"
1232 K$(2,12)="12- TERMINATION DATE"
1235 K$(2,13)="13- MARITAL STATUS":K$(2,14)="14- HMUR OR SALARY (H OR S)"
1240 K$(2,15)="15- PAY PERIODS - W/B/S/M":K$(2,16)="16- NUMBER OF EXEMPTIONS"
1245 K$(2,17)="17- HOURLY RATE"
     for z=1 to 17:read n(1,z):next z
     data 8,10,24,24,24,17,2,5,14,11,8,8,1,1,1,4,6
 print clear$:print

1500 if end #1 then 6000
 if end #2 then 15000
  open z5$ recl 512 as 1
  close 1
  open z7$ as 2
  read #2;z2,z3,flag
  close 2

1600 REM
     console
1620 PRINT CLEAR$
 if z2>z3 then print "*** OUT OF RECORD SPACE ***"
 PRINT "EMPLOYEE INFORMATION ENTRY PROGRAM"
  PRINT      "----------------------------------"
 PRINT:PRINT "THERE ARE ";Z3;" AVAILABLE RECORDS"
 PRINT "OF THESE THERE ARE:";TAB(30);(Z3-Z2)+1;" RECORDS OPEN"
 PRINT                      ;TAB(30);Z2-1;" RECORDS USED"
 PRINT

1650 PRINT "THIS IS A LIST OF OPERATIONS."
1655 PRINT
1660 FOR Z=1 TO 9:PRINT K$(4,Z):NEXT Z:PRINT
1665 PRINT "INDICATE WHAT YOU WOULD LIKE TO DO BY TYPING"
1670 PRINT "THE CORRESPONDING NUMBER."
1675 PRINT
1680 INPUT Z
 IF Z<1 OR Z>9 then 1620
1682 IF Z=1 THEN new$="N"
1685 ON Z GOSUB 2000,3000,4000,5000,6000,7000,8000,9000,10000
1690 FOR Z=1 TO 20:N(2,Z)=0:N(0,Z)=0:NEXT Z
1695 N$="":new$=""
1700 FOR Z=1 TO 20:K$(3,Z)=" ":NEXT Z
1705 GOTO 1600
2000 PRINT clear$:PRINT
2005 PRINT "RECORD NUMBER";z2:PRINT
2015 FOR Z=1 TO 2
 PRINT CUR$
2020 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2025 PRINT K$(1,Z);TAB(30);
2030 INPUT N(2,Z):PRINT chr$(13)
 PRINT CLEAR$
2035 NEXT Z
2040 FOR Z=3 TO 15
 PRINT CUR$
2045 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2050 PRINT K$(1,Z);TAB(30);
2055 INPUT line K$(3,Z):PRINT chr$(13)
 if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2060 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,i)
 PRINT CLEAR$
2065 NEXT Z
2066 N$=""
2070 FOR Z=3 TO 15:N$=N$+K$(3,Z):NEXT Z
2075 FOR Z=16 TO 17
 PRINT CUR$
2080 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2085 PRINT K$(1,Z);TAB(30);
2090 INPUT N(2,Z):PRINT chr$(13)
 PRINT CLEAR$
2095 NEXT Z
2100 Z1=Z2
2105 PRINT clear$;
2110 PRINT "RECORD NUMBER";z1;TAB(31);
2112 PRINT "1 - EMPLOYEE #";TAB(46);:print using u$;n(2,1)
2114 PRINT "2 - ACCT NUMBER";TAB(18);:print using u$;n(2,2);
2116 PRINT TAB(31);"3 - NAME";TAB(46);K$(3,3)
2118 PRINT TAB(31);"4 - COMPANY";TAB(46);K$(3,4)
2120 PRINT TAB(31);"5 - ADDRESS";TAB(46);K$(3,5)
2122 PRINT TAB(31);"6 - CITY";TAB(46);K$(3,6)
2124 PRINT TAB(31);"7 - STATE";TAB(46);K$(3,7);TAB(51);"8 - ZIP";
2126 PRINT TAB(61);K$(3,8)
2128 PRINT "9 - PHONE";TAB(16);K$(3,9);TAB(31);"10- SOC SEC #";
2130 PRINT TAB(46);K$(3,10)
2132 PRINT "11- EMPLOYED";TAB(16);K$(3,11);TAB(31);"12- TERMINATED";
2134 PRINT TAB(46);K$(3,12)
2136 PRINT "13- MARITAL";TAB(16);K$(3,13);TAB(31);"14- HOUR OR SA";
2138 PRINT TAB(46);K$(3,14)
2140 PRINT "15- PAY PERIOD";TAB(16);K$(3,15);TAB(31);"16- # OF EXEM";
2142 PRINT TAB(46);:print using v$;n(2,16);:PRINT TAB(51);"17- RATE";
     print using l$;n(2,17)
     print delim$
2146 PRINT TAB(16);"CURRENT";TAB(36);"QUARTER TO DATE";TAB(56);"YEAR TO DATE"
     print "Hours";tab(16);
     print using u$;n(2,18);tab(36);n(2,19);tab(56);n(2,20)
     print "Gross Pay";tab(16);
     print using l$;n(0,1);tab(36);n(0,2);tab(56);n(0,3)
     print "FICA";tab(16);
     print using l$;n(0,4);tab(36);n(0,5);tab(56);n(0,6)
     print "Fed Tax";tab(16);
     print using l$;n(0,7);tab(36);n(0,8);tab(56);n(0,9)
     print "State Tax";tab(16);
     print using l$;n(0,10);tab(36);n(0,11);tab(56);n(0,12)
     print "SDI";tab(16);
     print using l$;n(0,13);tab(36);n(0,14);tab(56);n(0,15)
     print "Misc Ded";tab(16);
     print using l$;n(0,16);tab(36);n(0,17);tab(56);n(0,18)
2175 INPUT "IF AN ITEM IS TO BE CHANGED, TYPE THE NUMBER - ELSE 18 ";Z
2176 IF Z>17 THEN 2216
2177 IF Z<1 THEN 2105
2178 IF Z>2 AND Z<16 THEN 2200
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
2217 FOR I=3 TO 15:N$=N$+K$(3,I):NEXT I
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
2280 open z5$ recl 512 as 1
     print #1,z1;N(2,1),N(2,2),N$,N(2,16),N(2,17),N(2,18),N(2,19),N(2,20),          N(0,1),N(0,2),N(0,3),N(0,4),N(0,5),N(0,6),N(0,7),          N(0,8),N(0,9),N(0,10),N(0,11),N(0,12),N(0,13),          N(0,14),N(0,15),N(0,16),N(0,17),N(0,18)
     close 1
2295 IF new$="N" THEN Z2=Z2+1
 IF new$="N" THEN flag=true:open z7$ as 1:print #1;z2,z3,flag:close 1
2300 RETURN
3000 PRINT clear$:PRINT:INPUT "RECORD NUMBER";Z1:PRINT clear$;
3003 IF Z1>=Z2 THEN 3000
3004 IF Z1<1 THEN RETURN
3005 open z5$ recl 512 as 1
     read #1,z1;N(2,1),N(2,2),N$,N(2,16),N(2,17),N(2,18),N(2,19),N(2,20),          N(0,1),N(0,2),N(0,3),N(0,4),N(0,5),N(0,6),N(0,7),          N(0,8),N(0,9),N(0,10),N(0,11),N(0,12),N(0,13),          N(0,14),N(0,15),N(0,16),N(0,17),N(0,18)
3008 Z9=1:FOR Z=3 TO 15:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
3009 close 1
3010 PRINT "RECORD NUMBER";z1;TAB(31);
3012 PRINT "EMPLOYEE #";TAB(46);:print using u$;n(2,1)
3014 PRINT "ACCT NUMBER";TAB(18);:print using u$;n(2,2);
3016 PRINT TAB(31);"NAME";TAB(46);K$(3,3)
3018 PRINT TAB(31);"COMPANY";TAB(46);K$(3,4)
3020 PRINT TAB(31);"ADDRESS";TAB(46);K$(3,5)
3022 PRINT TAB(31);"CITY";TAB(46);K$(3,6)
3024 PRINT TAB(31);"STATE";TAB(46);K$(3,7);TAB(51);"ZIP";
3026 PRINT TAB(61);K$(3,8)
3028 PRINT "PHONE";TAB(16);K$(3,9);TAB(31);"SOC SEC #";
3030 PRINT TAB(46);K$(3,10)
3032 PRINT "EMPLOYED";TAB(16);K$(3,11);TAB(31);"TERMINATED";
3034 PRINT TAB(46);K$(3,12)
3036 PRINT "MARITAL";TAB(16);K$(3,13);TAB(31);"HOUR OR SA";
3038 PRINT TAB(46);K$(3,14)
3040 PRINT "PAY PERIOD";TAB(16);K$(3,15);TAB(31);"# OF EXEM";
3042 PRINT TAB(46);:print using v$;n(2,16);:PRINT TAB(51);"RATE";
     print using l$;n(2,17)
     print delim$
3046 PRINT TAB(16);"CURRENT";TAB(36);"QUARTER TO DATE";TAB(56);"YEAR TO DATE"
     print "Hours";tab(16);
     print using u$;n(2,18);tab(36);n(2,19);tab(56);n(2,20)
     print "Gross Pay";tab(16);
     print using l$;n(0,1);tab(36);n(0,2);tab(56);n(0,3)
     print "FICA";tab(16);
     print using l$;n(0,4);tab(36);n(0,5);tab(56);n(0,6)
     print "Fed Tax";tab(16);
     print using l$;n(0,7);tab(36);n(0,8);tab(56);n(0,9)
     print "State Tax";tab(16);
     print using l$;n(0,10);tab(36);n(0,11);tab(56);n(0,12)
     print "SDI";tab(16);
     print using l$;n(0,13);tab(36);n(0,14);tab(56);n(0,15)
     print "Misc Ded";tab(16);
     print using l$;n(0,16);tab(36);n(0,17);tab(56);n(0,18)
3075 PRINT "FOR NEW RECORD, TYPE N-RETURN , OTHERWISE F-RETURN"
3085 INPUT temp$
3090 IF left$(temp$,1)="n" or left$(temp$,1)="N" THEN 3000
3095 RETURN
4000 PRINT clear$:PRINT
4005 N$=""
4010 INPUT "RECORD NUMBER";Z1
4015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
4020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
4025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";z1:GOTO 1620
4030 open z5$ recl 512 as 1
     read #1,z1;N(2,1),N(2,2),N$,N(2,16),N(2,17),N(2,18),N(2,19),N(2,20),          N(0,1),N(0,2),N(0,3),N(0,4),N(0,5),N(0,6),N(0,7),          N(0,8),N(0,9),N(0,10),N(0,11),N(0,12),N(0,13),          N(0,14),N(0,15),N(0,16),N(0,17),N(0,18)
4040 close 1
4045 IF N(2,1)=0 THEN PRINT "*** DELETED RECORD ***":GOTO 1620
4050 Z9=1
4055 FOR Z=3 TO 15
4060 K$(3,Z)=MID$(N$,Z9,N(1,Z))
4065 Z9=Z9+N(1,Z)
4070 NEXT Z
4075 GOTO 2105
5000 PRINT clear$:PRINT:INPUT "RECORD NUMBER";Z1:PRINT clear$:PRINT
5002 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
5004 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
5005 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";z1:GOTO 1620
5006 open z5$ recl 512 as 1
     read #1,z1;N(2,1),N(2,2),N$,N(2,16),N(2,17),N(2,18),N(2,19),N(2,20),          N(0,1),N(0,2),N(0,3),N(0,4),N(0,5),N(0,6),N(0,7),          N(0,8),N(0,9),N(0,10),N(0,11),N(0,12),N(0,13),          N(0,14),N(0,15),N(0,16),N(0,17),N(0,18)
5009 close 1
5010 Z9=1:FOR Z=3 TO 15:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
5011 PRINT "RECORD NUMBER";z1;TAB(31);
5012 PRINT "EMPLOYEE #";TAB(46);:print using u$;n(2,1)
5014 PRINT "ACCT NUMBER";TAB(18);:print using u$;n(2,2);
5016 PRINT TAB(31);"NAME";TAB(46);K$(3,3)
5018 PRINT TAB(31);"COMPANY";TAB(46);K$(3,4)
5020 PRINT TAB(31);"ADDRESS";TAB(46);K$(3,5)
5022 PRINT TAB(31);"CITY";TAB(46);K$(3,6)
5024 PRINT TAB(31);"STATE";TAB(46);K$(3,7);TAB(51);"ZIP";
5026 PRINT TAB(61);K$(3,8)
5028 PRINT "PHONE";TAB(16);K$(3,9);TAB(31);"SOC SEC #";
5030 PRINT TAB(46);K$(3,10)
5032 PRINT "EMPLOYED";TAB(16);K$(3,11);TAB(31);"TERMINATED";
5034 PRINT TAB(46);K$(3,12)
5036 PRINT "MARITAL";TAB(16);K$(3,13);TAB(31);"HOUR OR SA";
5038 PRINT TAB(46);K$(3,14)
5040 PRINT "PAY PERIOD";TAB(16);K$(3,15);TAB(31);"# OF EXEM";
5042 PRINT TAB(46);:print using v$;n(2,16);:PRINT TAB(51);"RATE";
     print using l$;n(2,17)
5074 PRINT
5100 INPUT "IS RECORD TO BE DELETED (MUST BE YES TO DELETE)";line temp$
5105 IF left$(temp$,1)="n" OR left$(temp$,1)="N" THEN RETURN
5110 IF ucase$(temp$)<>"YES" THEN 5100
5115 N(2,1)=0
5120 open z5$ recl 512 as 1
     print #1,z1;N(2,1),N(2,2),N$,N(2,16),N(2,17),N(2,18),N(2,19),N(2,20),          N(0,1),N(0,2),N(0,3),N(0,4),N(0,5),N(0,6),N(0,7),          N(0,8),N(0,9),N(0,10),N(0,11),N(0,12),N(0,13),          N(0,14),N(0,15),N(0,16),N(0,17),N(0,18)
5130 close 1
5135 RETURN
6000 PRINT clear$:PRINT
6005 PRINT "IF YOU HAVE ARRIVED HERE, AND HAVE AN EMPLOYEE FILE"
6010 PRINT "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6015 PRINT "TYPE THE LETTER C FOLLOWED BY A RETURN TO CONTINUE."
6020 PRINT
6025 PRINT "IF YOU WISH TO CREATE A NEW FILE, TYPE THE LETTER N"
6030 PRINT "FOLLOWED BY RETURN.":PRINT
6035 INPUT line temp$
6040 IF ucase$(temp$)="C" then initialize:GOTO 1500
6045 PRINT clear$:PRINT
6050 INPUT "NUMBER OF RECORDS DESIRED";z3
6055 PRINT
     n$="":for z=1 to 150:n$=n$+" ":next z
6060 FOR I=1 TO 20:N(2,I)=0:N(0,I)=0:NEXT I
6065 create z5$ recl 512 as 1
6070 FOR Z1=1 TO z3+2
     print #1,z1;N(2,1),N(2,2),N$,N(2,16),N(2,17),N(2,18),N(2,19),N(2,20),          N(0,1),N(0,2),N(0,3),N(0,4),N(0,5),N(0,6),N(0,7),          N(0,8),N(0,9),N(0,10),N(0,11),N(0,12),N(0,13),          N(0,14),N(0,15),N(0,16),N(0,17),N(0,18)
6085 NEXT Z1
6090 close 1
6095 PRINT clear$:PRINT
6100 PRINT "EMPLOYEE FILE CREATED AND CLEARED.":PRINT
6105 PRINT z3;"RECORDS CREATED.":PRINT
6110 PRINT "TO CONTINUE, TYPE RETURN.":INPUT line temp$
 Z2=1
 create z7$ as 1:print #1;Z2,Z3:close 1
6115 GOTO 1500
7000 n$="":for z=1 to 150:n$=n$+" ":next z
     for z=1 to 20:n(2,z)=0:n(0,z)=0:next z
7006 INPUT "ARE YOU SURE !!! (YES OR NO)";line temp$
7007 IF ucase$(temp$)<>"YES" THEN RETURN
     open z5$ recl 512 as 1
7010 FOR Z1=1 TO z3+2
     print #1,z1;N(2,1),N(2,2),N$,N(2,16),N(2,17),N(2,18),N(2,19),N(2,20),          N(0,1),N(0,2),N(0,3),N(0,4),N(0,5),N(0,6),N(0,7),          N(0,8),N(0,9),N(0,10),N(0,11),N(0,12),N(0,13),          N(0,14),N(0,15),N(0,16),N(0,17),N(0,18)
7025 NEXT Z1
7030 close 1
7035 PRINT clear$:PRINT
7040 PRINT "EMPLOYEE FILE CLEARED!":PRINT
7045 PRINT "TO CONTINUE, TYPE RETURN."
7050 INPUT line temp$
 Z2=1
 open z7$ as 1:print #1;Z2,Z3:close 1
7060 RETURN
8000 CHAIN "EPSORT"
9000 CHAIN "EPLIST"
10000 CHAIN "master5"
15000 print clear$:print:print "CHECKING FILE LENGTH"
 PRINT:PRINT "*** PLEASE WAIT ***"
 CLOSE 2
 open z5$ recl 512 as 1
 z3=(size(z5$)*block.size)/512
 for z2=1 to z3
 read #1,z2;n(2,1),n(2,2)
 if n(2,2)=0 then 15300
 next z2
15300 z3=int(z3)-2
 close 1
 flag=false
 create z7$ as 1
 print #1;z2,z3,flag
 close 1
 GOTO 1500
