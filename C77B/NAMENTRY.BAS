1     rem This is the Company Name Entry Program

%INCLUDE ALL.BAS
500     RESTORE
	DIM T%(20),A(16),D$(8),DATE1$(8)
 BIZMII.FLAG=0
505 Z5$="nm"
506 Z6$="nmback"
	Z7$="nmsize"
510 X$=" "
515 repeat$="------------------------"
516 fill$="                         "
520 DIM Z$(5,5),N(2,20)
530 DIM K$(6,20)
540 K$(6,1)="1 - NEW ENTRY":K$(6,2)="2 - EXAMINE EXISTING ENTRY"
550 K$(6,3)="3 - MODIFY EXISTING ENTRY"
560 K$(6,4)="4 - DELETE EXISTING ENTRY"
570 K$(6,5)="5 - CREATE NEW FILE":K$(6,6)="6 - CLEAR EXISTING FILE"
580 K$(6,7)="7 - BACK-UP AND SORT FILE":K$(6,8)="8 - LIST FILE"
590 K$(6,9)="9 - FINISHED"
1000 REM THIS IS THE NAME ENTRY PROGRAM
1005 PRINT clear$
1010 PRINT
1075 K$(1,1)="TAG":K$(1,2)="REFERENCE NUMBER":K$(1,3)="NAME"
1080 K$(1,4)="COMPANY":K$(1,5)="ADDRESS":K$(1,6)="CITY":K$(1,7)="STATE"
1085 K$(1,8)="ZIP CODE":K$(1,9)="PHONE (AAA) NNN NNNN"
1086 K$(1,10)="FEDERAL TAX ID #":K$(1,11)="STATE TAX ID #"
1090 N(1,1)=4:N(1,2)=10:N(1,3)=24:N(1,4)=24:N(1,5)=24:N(1,6)=17
1095 N(1,7)=2:N(1,8)=5:N(1,9)=14:N(1,10)=20:N(1,11)=20
1100 K$(2,1)="1 - TAG":K$(2,2)="2 - REFERENCE NUMBER":K$(2,3)="3 - NAME"
1105 K$(2,4)="4 - COMPANY":K$(2,5)="5 - ADDRESS":K$(2,6)="6 - CITY"
1110 K$(2,7)="7 - STATE":K$(2,8)="8 - ZIP CODE"
1115 K$(2,9)="9 - PHONE (AAA) NNN NNNN"
1116 K$(2,10)="10- FEDERAL TAX ID #":K$(2,11)="11- STATE TAX ID #"
1117 K$(2,12)="12- NONE"
	print clear$:print

1500	if end #1 then 6000
	if end #2 then 15000
		open z5$ recl 170 as 1
		close 1
		open z7$ as 2
		read #2;z2,z3
		close 2

1600	REM
     console
1620	PRINT CLEAR$
	if z2>z3 then print "*** OUT OF RECORD SPACE ***"
	PRINT "COMPANY NAME ENTRY PROGRAM"
	PRINT "--------------------------"
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
1685 ON Z go sub 2000,3000,4000,5000,6000,7000,8000,9000,10000
1690 N(2,1)=0:N(2,2)=0:N$="":new$=""
1700 FOR Z=1 TO 10:K$(3,Z)="":NEXT Z
1705 GOTO 1600
2000 IF Z2>Z3 THEN RETURN
2001 PRINT clear$:PRINT
2005 PRINT "RECORD NUMBER";Z2:PRINT
2015 FOR Z=1 TO 2
	PRINT CUR$
2020 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$;
2025 PRINT K$(1,Z);TAB(30);
2030 INPUT N(2,Z):PRINT chr$(13)
	PRINT CLEAR$
2035 NEXT Z
2040 FOR Z=3 TO 11 
	PRINT CUR$
2045 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$;
2050 PRINT K$(1,Z);TAB(30);
2055 input line K$(3,Z):PRINT chr$(13)
	if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2060 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,I)
	PRINT CLEAR$
2065 NEXT Z
2066 N$=""
2070 FOR Z=3 TO 11:N$=N$+K$(3,Z):NEXT Z
2100 Z1=Z2
2105 PRINT clear$:PRINT
2110 PRINT "RECORD NUMBER";Z1:PRINT
2115 FOR Z=1 TO 2
2120 PRINT K$(2,Z);TAB(30);N(2,Z)
2125 NEXT Z
2130 FOR Z=3 TO 12
2135 PRINT K$(2,Z);TAB(30);K$(3,Z)
2140 NEXT Z
2145 PRINT
2150 PRINT "IF AN ITEM IS TO BE CHANGED, TYPE THE APPROPRIATE NUMBER."
2155 PRINT:INPUT Z
2160 PRINT
2165 IF Z>11 THEN 2216
2170 IF Z<1 THEN 2105
2175 IF Z>2 THEN 2200
2180 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$;
2185 PRINT K$(1,Z);TAB(30);
2190 INPUT N(2,Z)
2195 GOTO 2225
2200 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$;
2205 PRINT K$(1,Z);TAB(30);
2210 input line K$(3,Z)
	if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2215 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,I)
2216 N$=""
2220 FOR I=3 TO 11:N$=N$+K$(3,I):NEXT I
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
2280 open z5$ recl 170 as 1
     print #1,z1;N(2,1),N(2,2),N$
     close 1
2295 IF new$="N" THEN Z2=Z2+1:open z7$ as 1:print #1;z2,z3:close 1
2300 RETURN
3000 PRINT clear$:PRINT
3005 INPUT "RECORD NUMBER";Z1
3010 IF Z1>=Z2 THEN 3000
3012 IF Z1<1 THEN RETURN
     open z5$ recl 170 as 1
     read #1,z1;N(2,1),N(2,2),N$
     close 1
3030 PRINT
3035 FOR Z=1 TO 2
3040 PRINT K$(1,Z);TAB(30);N(2,Z)
3045 NEXT Z
3046 Z9=1
3050 FOR Z=3 TO 11
3055 PRINT K$(1,Z);TAB(30);MID$(N$,Z9,N(1,Z))
3060 Z9=Z9+N(1,Z)
3065 NEXT Z
3070 PRINT
3075 PRINT "FOR A NEW RECORD NUMBER, TYPE N - RETURN.":PRINT
3080 PRINT "IF FINISHED, TYPE F - RETURN.":PRINT
3085 input line temp$
3090 IF left$(temp$,1)="n" OR left$(temp$,1)="N" THEN 3000
3095 RETURN
4000 PRINT clear$:PRINT
4005 N$=""
4010 INPUT "RECORD NUMBER";Z1
4015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
4020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
4025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOTO 1620
     open z5$ recl 170 as 1
     read #1,z1;N(2,1),N(2,2),N$
     close 1
4045 IF N(2,1)=0 THEN PRINT "*** DELETED RECORD ***":GOTO 1620
4050 Z9=1
4055 FOR Z=3 TO 11
4060 K$(3,Z)=MID$(N$,Z9,N(1,Z))
4065 Z9=Z9+N(1,Z)
4070 NEXT Z
4075 GOTO 2105
5000 PRINT clear$:PRINT
5010 INPUT "RECORD NUMBER";Z1
5015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
5020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
5025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOTO 1620
     open z5$ recl 170 as 1
     read #1,z1;N(2,1),N(2,2),N$
     close 1
5045 PRINT "RECORD NUMBER";Z1
5050 PRINT
5055 FOR Z=1 TO 2
5060 PRINT K$(1,Z);TAB(30);N(2,Z)
5065 NEXT Z
5070 Z9=1
5075 FOR Z=3 TO 11
5080 PRINT K$(1,Z);TAB(30);MID$(N$,Z9,N(1,Z))
5085 Z9=Z9+N(1,Z)
5090 NEXT Z
5095 PRINT
5100 INPUT "IS RECORD TO BE DELETED (MUST BE yes TO DELETE)";line temp$
5105 IF left$(temp$,1)="n" OR left$(temp$,1)="N" THEN RETURN
5110 IF temp$<>"yes" AND temp$<>"YES" THEN 5095
5115 N(2,1)=0
     open z5$ recl 170 as 1
     print #1,z1;N(2,1),N(2,2),N$
     close 1
5135 RETURN
6000 PRINT clear$:PRINT
6005 PRINT "IF YOU HAVE ARRIVED HERE, AND HAVE A NAME-ENTRY FILE"
6010 PRINT "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6015 PRINT "TYPE THE Letter c FOLLOWED BY A RETURN TO CONTINUE."
6020 PRINT
6025 PRINT "IF YOU WISH TO CREATE A NEW FILE, TYPE THE Letter n"
6030 PRINT "FOLLOWED BY RETURN.":PRINT
6035 input line temp$
6040 IF left$(temp$,1)="c" OR left$(temp$,1)="C" THEN INITIALIZE:GOTO 1500
6045 PRINT clear$:PRINT
6050 INPUT "NUMBER OF RECORDS DESIRED";Z3
6055 PRINT
6060 N(2,1)=0:N(2,2)=0
     for z=1 to 150:n$=n$+" ":next z
     create z5$ recl 170 as 1
6070 FOR Z1=1 TO Z3+2
     print #1,z1;N(2,1),N(2,2),N$
     next z1
     close 1
6095 PRINT clear$:PRINT
6100 PRINT "NAME ENTRY FILE CREATED AND CLEARED.":PRINT
6105 PRINT Z3;"RECORDS CREATED.":PRINT
6110 input "TO CONTINUE, TYPE RETURN.";line temp$
	Z2=1
	create z7$ as 1:print #1;Z2,Z3:close 1
6115 GOTO 1500
7000 INPUT "ARE YOU SURE !!! (YES OR NO)";line temp$
7007 IF temp$<>"yes" AND temp$<>"YES" THEN RETURN
     N(2,1)=0:N(2,2)=0
     for z=1 to 150:n$=n$+" ":next z
     open z5$ recl 170 as 1
     FOR Z1=1 TO Z3+2
     print #1,z1;N(2,1),N(2,2),N$
     next z1
     close 1
7035 PRINT clear$:PRINT
7040 PRINT "NAME ENTRY FILE CLEARED!":PRINT
7045 PRINT "TO CONTINUE, TYPE RETURN."
7050 input line temp$
	z2=1
	open z7$ as 1:print #1;z2,z3:close 1
7060 RETURN
8000 chain "nmsort"
9000 PRINT clear$:PRINT
     lprinter
     open z5$ recl 170 as 1
9015 FOR Z1=1 TO Z2-1
     read #1,z1;N(2,1),N(2,2),N$
9025 FOR Z=1 TO 2:PRINT K$(1,Z);TAB(30);N(2,Z):NEXT Z
9030 Z9=1
9035 FOR Z=3 TO 11:PRINT K$(1,Z);TAB(30);MID$(N$,Z9,N(1,Z))
9040 Z9=Z9+N(1,Z):NEXT Z:PRINT
9045 NEXT Z1
9050 PRINT chr$(12)
9055 close 1
     console
9065 RETURN
10000 print clear$:print:print "Replace disk #5 in drive A."
      print "Type return to continue."
      input line temp$
      initialize
      chain "bizmii"
15000 PRINT clear$:PRINT:PRINT "CHECKING FILE LENGTH"
	PRINT:PRINT "*** PLEASE WAIT ***"
	open z5$ recl 170 as 1
	z3=(size(z5$)*block.size)/170
	for z2=1 to z3
	read #1,z2;n(2,1),n(2,2)
	if n(2,2)=0 then 15300
	next z2
15300	z3=int(z3)-2
	close 1
	create z7$ as 1
	print #1;z2,z3
	close 1
	GOTO 1500
