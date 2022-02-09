1     rem This is the Federal Tax Deposit Entry Program

%INCLUDE ALL.BAS
500     dim n(2,20),k$(6,20)
     z5$="edep":z6$=z5$+"back":z7$=z5$+"size"
     repeat$="-------------------------------"
     fill$="                               "
     l$="$#,###,###.##"
     RESTORE
540 K$(6,1)="1 - NEW ENTRY":K$(6,2)="2 - EXAMINE EXISTING ENTRY"
550 K$(6,3)="3 - MODIFY EXISTING ENTRY"
560 K$(6,4)="4 - NOT USED"
570 K$(6,5)="5 - CREATE NEW FILE":K$(6,6)="6 - CLEAR EXISTING FILE"
580 K$(6,7)="7 - NOT USED":K$(6,8)="8 - LIST FILE"
590 K$(6,9)="9 - FINISHED"
1075 K$(1,1)="QUARTER (1,2,3 OR 4)":K$(1,2)="MONTH (1,2 OR 3)"
1080 K$(1,3)="PERIOD (1,2,3 OR 4)":K$(1,4)="TAX LIABILITY"
1085 K$(1,5)="AMOUNT OF ANY DEPOSIT":K$(1,6)="CHECK NO."
1090 K$(1,7)="DATE OF DEPOSIT"
1095 K$(2,1)="1 - QUARTER (1,2,3 OR 4)":K$(2,2)="2 - MONTH (1,2 OR 3)"
1100 K$(2,3)="3 - PERIOD (1,2,3 OR 4)":K$(2,4)="4 - TAX LIABILITY"
1110 K$(2,5)="5 - AMOUNT OF ANY DEPOSIT":K$(2,6)="6 - CHECK NO."
1115 K$(2,7)="7 - DATE OF DEPOSIT":K$(2,8)="8 - NONE"
1135 N(1,1)=1:N(1,2)=1:N(1,3)=1:N(1,4)=12:N(1,5)=12:N(1,6)=8:N(1,7)=8
	print clear$:print

1500	if end #1 then 6000
	if end #2 then 15000
		open z5$ recl 64 as 1
		close 1
		open z7$ as 2
		read #2;z2,z3
		close 2

1600	REM
     console
1620	PRINT CLEAR$
	if z2>z3 then print "*** OUT OF RECORD SPACE ***"
	PRINT "FEDERAL PAYROLL DEPOSIT ENTRY PROGRAM"
		PRINT      "-------------------------------------"
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
1690 FOR Z=1 TO 20:N(2,Z)=0:NEXT Z:N$=" "
1700 FOR Z=1 TO 20:K$(3,Z)=" ":NEXT Z
1702 new$=""
1705 GOTO 1600
2000 PRINT clear$:PRINT
2005 PRINT "RECORD NUMBER";Z2:PRINT
2015 FOR Z=1 TO 5
	PRINT CUR$
2020 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$;
2025 PRINT K$(1,Z);TAB(30);
2030 INPUT N(2,Z):PRINT chr$(13)
	PRINT CLEAR$
2035 NEXT Z
2040 FOR Z=6 TO 7
	PRINT CUR$
2045 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$;
2050 PRINT K$(1,Z);TAB(30);
2055 INPUT line K$(3,Z):PRINT chr$(13)
	if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2060 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,i)
	PRINT CLEAR$
2065 NEXT Z
2090 N$=""
2095 FOR Z=6 TO 7:N$=N$+K$(3,Z):NEXT Z
2100 Z1=Z2
2105 PRINT clear$:PRINT
2110 PRINT "RECORD NUMBER";Z1:PRINT
2115 FOR Z=1 TO 5
2120 PRINT K$(2,Z);TAB(30);N(2,Z)
2125 NEXT Z
2130 FOR Z=6 TO 7
2135 PRINT K$(2,Z);TAB(30);K$(3,Z)
2140 NEXT Z
2142 PRINT K$(2,8)
2145 PRINT
2150 PRINT "IF AN ITEM IS TO BE CHANGED, TYPE THE APPROPRIATE NUMBER."
2155 PRINT:INPUT Z
2160 PRINT
2165 IF Z>7 THEN 2216
2170 IF Z<1 THEN 2105
2175 IF Z>5 THEN 2200
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
2220 FOR Z=6 TO 7:N$=N$+K$(3,Z):NEXT Z
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
2280 open z5$ recl 64 as 1
     print #1,z1;n(2,1),n(2,2),n(2,3),n(2,4),n(2,5),n$
     close 1
2295 IF new$="N" THEN Z2=Z2+1:open z7$ as 1:print #1;z2,z3:close 1
2300 RETURN
3000 PRINT clear$:PRINT
3005 INPUT "RECORD NUMBER";Z1
3010 IF Z1>=Z2 THEN 3000
3012 IF Z1<1 THEN RETURN
3020 open z5$ recl 64 as 1
     read #1,z1;n(2,1),n(2,2),n(2,3),n(2,4),n(2,5),n$
     close 1
3026 Z9=1:FOR Z=6 TO 7:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
3030 PRINT
3035 FOR Z=1 TO 5
3040 PRINT K$(1,Z);TAB(30);N(2,Z)
3045 NEXT Z
3050 FOR Z=6 TO 7
3055 PRINT K$(1,Z);TAB(30);K$(3,Z)
3060 NEXT Z
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
4030 open z5$ recl 64 as 1
     read #1,z1;n(2,1),n(2,2),n(2,3),n(2,4),n(2,5),n$
     close 1
4045 IF N(2,1)=0 THEN PRINT "*** DELETED RECORD ***":GOTO 1620
4050 Z9=1:FOR Z=6 TO 7:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
4075 GOTO 2105
5000 RETURN
6000 PRINT clear$:PRINT
6005 PRINT "IF YOU HAVE ARRIVED HERE, AND HAVE A FED DEPOSIT FILE"
6010 PRINT "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6015 PRINT "TYPE THE LETTER C FOLLOWED BY A RETURN TO CONTINUE."
6020 PRINT
6025 PRINT "IF YOU WISH TO CREATE A NEW FILE, TYPE THE LETTER N"
6030 PRINT "FOLLOWED BY RETURN.":PRINT
6035 INPUT line temp$
6040 IF ucase$(temp$)="C" THEN initialize:GOTO 1500
6045 PRINT clear$:PRINT
	z3=48
6055 PRINT
     n$="":for z=1 to 30:n$=n$+" ":next z
     for z=1 to 10:n(2,z)=0:next z
6065 create z5$ recl 64 as 1
6070 FOR Z1=1 TO z3+2
     print #1,z1;n(2,1),n(2,2),n(2,3),n(2,4),n(2,5),n$
6085 NEXT Z1
6090 close 1
6095 PRINT clear$:PRINT
6100 PRINT "FED DEPOSIT FILE CREATED AND CLEARED.":PRINT
6105 PRINT z3;"RECORDS CREATED.":PRINT
6110 PRINT "TO CONTINUE, TYPE RETURN.":INPUT line temp$
	Z2=1
	create z7$ as 1:print #1;Z2,Z3:close 1
6115 GOTO 1500
7000 n$="":for z=1 to 30:n$=n$+" ":next z
     for z=1 to 10:n(2,z)=0:next z
7006 INPUT "ARE YOU SURE !!! (YES OR NO)";line temp$
7007 IF ucase$(temp$)<>"YES" THEN RETURN
     open z5$ recl 64 as 1
7010 FOR Z1=1 TO z3+2
     print #1,z1;n(2,1),n(2,2),n(2,3),n(2,4),n(2,5),n$
7025 NEXT Z1
7030 close 1
7035 PRINT clear$:PRINT
7040 PRINT "FED DEPOSIT FILE CLEARED!":PRINT
7045 PRINT "TO CONTINUE, TYPE RETURN."
7050 INPUT line temp$
	Z2=1
	open z7$ as 1:print #1;Z2,Z3:close 1
7060 RETURN
8000 RETURN
9000 PRINT clear$:PRINT:PRINT "LISTING"
9010 P1=1:P2=1:P3=12
9020 lprinter
9030 open z5$ recl 64 as 1
9040 print chr$(12)
9045 print "FEDERAL DEPOSIT RECORD LISTING"
9050 print
9055 print "QUARTER";TAB(10);"MONTH";TAB(20);"PERIOD";TAB(30);
9060 print "TAX LIABILITY";TAB(50);"AMOUNT OF DEPOSIT";TAB(70);
9065 print "CHECK #";TAB(80);"DATE OF DEPOSIT"
9070 print
9075 FOR Z1=1 TO Z2-1
     read #1,z1;n(2,1),n(2,2),n(2,3),n(2,4),n(2,5),n$
9085 Z9=1:FOR Z=6 TO 7:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
9090 print N(2,1);TAB(10);N(2,2);TAB(20);N(2,3);TAB(30);
     print using l$;n(2,4);tab(50);n(2,5);
9095 print TAB(70);K$(3,6);TAB(80);K$(3,7)
9100 NEXT Z1
9110 print chr$(12):FOR Z=1 TO 100:NEXT Z:print chr$(12)
9115 close 1
	console
	return
10000 chain "master5"
15000	print clear$:print:print "CHECKING FILE LENGTH"
	PRINT:PRINT "*** PLEASE WAIT ***"
	open z5$ recl 64 as 1
	z3=(size(z5$)*block.size)/64
	for z2=1 to z3
	read #1,z2;n(2,1),n(2,2)
	if n(2,2)=0 then 15300
	next z2
15300	z3=48
	close 1
	create z7$ as 1
	print #1;z2,z3
	close 1
	GOTO 1500
