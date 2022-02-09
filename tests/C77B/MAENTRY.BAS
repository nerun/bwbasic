1	rem This is the Mailing List Name Entry Program

%INCLUDE ALL.BAS
500	dim k$(6,20),n(2,20)
	repeat$="------------------------------"
	fill$ = "                               "
fmt1$="####  ####  ##########  /...5...10...15...20.../  "
fmt2$="                        "
fmt2$=fmt2$+"/...5...10...15...20.../  /...5...10...15...20.../  "
fmt2$=fmt2$+"/...5...10...15./  //  /.../  /...5...10.../"

	RESTORE
	PRINT CLEAR$:PRINT
300	print "Enter mailing list file name as up to 4 alphanumeric characters"
	print:print "i.e. MA0."
	print:input line z5$
	if len(z5$)>4 then 300
	z5$=""+z5$
	z6$=z5$+"back":z7$=z5$+"size"

	   print "Is reference no. to be zip code?";:input line temp$
		    if  ucase$(left$(temp$,1))="Y" then zip.flag=1

540 K$(6,1)="1 - NEW ENTRY":K$(6,2)=" 2 - EXAMINE EXISTING ENTRY"
550 K$(6,3)="3 - MODIFY EXISTING ENTRY"
560 K$(6,4)="4 - DELETE EXISTING ENTRY"
570 K$(6,5)="5 - CREATE NEW FILE":K$(6,6)="6 - CLEAR EXISTING FILE"
580 K$(6,7)="7 - BACK-UP AND SORT FILE":K$(6,8)="8 - LIST FILE"
590 K$(6,9)="9 - FINISHED"
1075 K$(1,1)="TAG":K$(1,2)="REFERENCE NUMBER":K$(1,3)="NAME"
1080 K$(1,4)="COMPANY":K$(1,5)="ADDRESS":K$(1,6)="CITY":K$(1,7)="STATE"
1085 K$(1,8)="ZIP CODE":K$(1,9)="PHONE (AAA) NNN NNNN"
     for z=1 to 9:read n(1,z):next z
     data 4,10,24,24,24,17,2,5,14
1100 K$(2,1)="1 - TAG":K$(2,2)="2 - REFERENCE NUMBER":K$(2,3)="3 - NAME"
1105 K$(2,4)="4 - COMPANY":K$(2,5)="5 - ADDRESS":K$(2,6)="6 - CITY"
1110 K$(2,7)="7 - STATE":K$(2,8)="8 - ZIP CODE"
1115 K$(2,9)="9 - PHONE (AAA) NNN NNNN":K$(2,10)="10- NONE"
	print clear$:print

1500	if end #1 then 6000
		open z5$ recl 150 as 1
		close 1
		open z7$ as 2
		read #2;z2,z3
		close 2

1600 REM
     console
1620 	print clear$
	if z2>z3 then print "*** OUT OF RECORD SPACE ***"
	PRINT "MAILING LIST ENTRY PROGRAM"
	PRINT "--------------------------"
	PRINT:PRINT "THERE ARE ";Z3;" AVAILABLE RECORDS"
	PRINT "OF THESE THERE ARE:";TAB(30);(Z3-Z2)+1;" RECORDS OPEN"
	PRINT			   ;TAB(30);Z2-1;" RECORDS USED"
	PRINT
1650 PRINT "THIS IS A LIST OF OPERATIONS."
1655 PRINT
1660 FOR Z=1 TO 9:PRINT K$(6,Z):NEXT Z:PRINT
1665 PRINT "INDICATE WHAT YOU WOULD LIKE TO DO BY TYPING"
1670 PRINT "THE CORRESPONDING NUMBER."
1675 PRINT
1680 INPUT Z
	IF Z<1 OR Z>9 THEN 1620
1682 IF Z=1 THEN new$="N"
1685 ON Z GOSUB 2000,3000,4000,5000,6000,7000,8000,9000,10000
1690 N(2,1)=0:N(2,2)=0:N$="":new$=""
1700 FOR Z=1 TO 10:K$(3,Z)="":NEXT Z
1705 GOTO 1600
2000 IF Z2>Z3 THEN RETURN
2001 PRINT CLEAR$:PRINT
2005 PRINT "RECORD NUMBER";Z2:PRINT
2015 FOR Z=1 TO 2
	PRINT CUR$
2020 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$
2025 PRINT K$(1,Z);TAB(30);
2030 INPUT line temp$:PRINT chr$(13)
	PRINT CLEAR$
	n(2,z)=val(temp$)
		if n(2,1)=0 then n(2,1)=z2
		if n(2,2)=0 then n(2,2)=z2
2035 NEXT Z
2040 FOR Z=3 TO 9
	PRINT CUR$
2045 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$
2050 PRINT K$(1,Z);TAB(30);
2055 INPUT line K$(3,Z):PRINT chr$(13)
	if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2060 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,i)
	PRINT CLEAR$
		if zip.flag=1 then n(2,2)=val(k$(3,8))
2065 NEXT Z
2066 N$=""
2070 FOR Z=3 TO 9:N$=N$+K$(3,Z):NEXT Z
2100 Z1=Z2
2105 PRINT clear$:PRINT
2110 PRINT "RECORD NUMBER";Z1:PRINT
2115 FOR Z=1 TO 2
2120 PRINT K$(2,Z);TAB(30);N(2,Z)
2125 NEXT Z
2130 FOR Z=3 TO 10
2135 PRINT K$(2,Z);TAB(30);K$(3,Z)
2140 NEXT Z
2145 PRINT
2150 PRINT "IF AN ITEM IS TO BE CHANGED, TYPE THE APPROPRIATE NUMBER."
2155 PRINT:INPUT Z
2160 PRINT
2165 IF Z>9 THEN 2216
2170 IF Z<1 THEN 2105
2175 IF Z>2 THEN 2200
2180 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$
2185 PRINT K$(1,Z);TAB(30);
2190 INPUT N(2,Z)
2195 GOTO 2225
2200 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$
2205 PRINT K$(1,Z);TAB(30);
2210 INPUT line K$(3,Z)
	if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2215 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,i)
2216 N$=""
2220 FOR I=3 TO 9:N$=N$+K$(3,I):NEXT I
2225 INPUT "ANY MORE CHANGES";line temp$
2230 IF ucase$(left$(temp$,1))="Y" then 2105
2235 print
2240 INPUT "IS RECORD TO BE ENTERED";line temp$
2245 print
2250 if ucase$(left$(temp$,1))="Y" then 2280
2255 if ucase$(left$(temp$,1))<>"N" then 2235
2260 print clear$:print
2270 print "*** RECORD NOT ENTERED ***":print:print
2275 for z=1 to 200:next z:return
2280 open z5$ recl 150 as 1
     print #1,z1;n(2,1),n(2,2),n$
     close 1
2295 if new$="N" then z2=z2+1:open z7$ as 1:print #1;z2,z3:close 1
2300 return
3000 print clear$:print
3005 input "RECORD NUMBER";z1
3010 if z1>=z2 then 3000
3012 if z1<1 then return
3015 open z5$ recl 150 as 1
     read #1,z1;n(2,1),n(2,2),n$
     close 1
3030 print
3035 for z=1 to 2
3040 print k$(1,z);tab(30);n(2,z)
3045 next z
3046 z9=1
3050 for z=3 to 9
3055 print k$(1,z);tab(30);mid$(n$,z9,n(1,z))
3060 z9=z9+n(1,z)
3065 next z
3070 print
3075 print "FOR A NEW RECORD NUMBER, TYPE N - RETURN.":PRINT
3080 PRINT "IF FINISHED, TYPE F - RETURN.":PRINT
3085 input line temp$
3090 if ucase$(temp$)="N" then 3000
3095 return
4000 print clear$:print
4005 N$=""
4010 input "RECORD NUMBER";z1
4015 if z1<1 then print "*** NO SUCH RECORD ***":goto 1620
4020 if z1>z3 then print "*** OUT OF RANGE ***":goto 1620
4025 if z1>=z2 then print "NO RECORD NUMBER";Z1:goto 1620
4030 open z5$ recl 150 as 1
     read #1,z1;n(2,1),n(2,2),n$
     close 1
4045 if n(2,1) = 0 then print "*** DELETED RECORD ***":GOTO 1620
4050 Z9=1
4055 FOR Z=3 TO 9
4060 K$(3,Z)=MID$(N$,Z9,N(1,Z))
4065 Z9=Z9+N(1,Z)
4070 NEXT Z
4075 GOTO 2105
5000 PRINT clear$:print
5010 input "RECORD NUMBER";z1
5015 if z1<1 then print "*** NO SUCH RECORD ***":goto 1620
5020 if z1>z3 then print "*** OUT OF RANGE ***":goto 1620
5025 if z1>=z2 then print "NO RECORD NUMBER";Z1:goto 1620
5030 open z5$ recl 150 as 1
     read #1,z1;n(2,1),n(2,2),n$
     close 1
5045 if n(2,1) = 0 then print "*** DELETED RECORD ***":GOTO 1620
5050 print
5055 for z=1 to 2
5060 print k$(1,z);tab(30);n(2,z)
5065 next z
5070 Z9=1
5055 FOR Z=3 TO 9
5080 print k$(1,z);tab(30);mid$(n$,z9,n(1,z))
5085 Z9=Z9+N(1,Z)
5090 NEXT Z
5095 print
5100 input "IS RECORD TO BE DELETED (MUST BE YES TO DELETE)";line temp$
5105 if ucase$(left$(temp$,1))="N" then return
5110 if ucase$(temp$)<> "YES" then 5095
5115 n(2,1)=0
5120 open z5$ recl 150 as 1
     print #1,z1;n(2,1),n(2,2),n$
     close 1
5135 return
6000 print clear$:print
6005 print "IF YOU HAVE ARRIVED HERE AND HAVE THIS MAILING LIST FILE"
6010 print "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6016 print "TYPE THE LETTER C FOLLOWED BY A RETURN TO CONTINUE."
6020 print
6025 print "IF YOU WISH TO CREATE A NEW FILE, TYPE THE LETTER N"
6030 print "FOLLOWED BY RETURN.":print
6035 input line temp$
6040 if ucase$(temp$)="C" then initialize:goto 1500
6045 print clear$:print
6050 input "NUMBER OF RECORDS DESIRED";z3
6055 print
6060 n$="":for z=1 to 100:n$=n$+" ":next z:n(2,1)=0:n(2,2)=0
6065 create z5$ recl 150 as 1
6070 for z1=1 to z3+2
     print #1,z1;n(2,1),n(2,2),n$
6085 next z1
6090 close 1
6095 print clear$:print
6100 print "MAILING LIST FILE ";MID$(Z5$,3,4);" CREATED AND CLEARED.":PRINT
6105 PRINT z3;"RECORDS CREATED.":PRINT
6110 PRINT "TO CONTINUE, TYPE RETURN.":INPUT line temp$
	z2=1
	create z7$ as 1:print #1;z2,z3:close 1
6115 goto 1500
7000 n$="":for z=1 to 100:n$=n$+" ":next z:n(2,1)=0:n(2,2)=0
7006 input "ARE YOU SURE !!! (YES OR NO)";line temp$
7007 if ucase$(temp$)<> "YES" then return
     open z5$ recl 150 as 1
7010 for z1=1 to z3+2
     print #1,z1;n(2,1),n(2,2),n$
7025 next z1
7030 close 1
7035 print clear$:print
7040 print "MAILING LIST FILE ";mid$(z5$,3,4);" CLEARED!":PRINT
7045 print "TO CONTINUE, TYPE RETURN."
7050 INPUT line temp$
	z2=1
	open z7$ as 1:print #1;z2,z3:close 1
7060 return
8000 chain "MASORT"
9000 print clear$:print
9010 input "Enter number of lines per page.";page
9015 page1=1
9020 print clear$:print
9030 lprinter
9035 open z5$ recl 150 as 1
9040 for z1=1 to z2-1
9045 read #1,z1;n(2,1),n(2,2),n$
9050 z9=1
9055 for z=3 to 9
9060 k$(3,z)=mid$(n$,z9,n(1,z))
9065 z9=z9+n(1,z)
9070 next z
9100 print using fmt1$;z1,n(2,1),n(2,2),k$(3,3)
9101 print using fmt2$;k$(3,4),k$(3,5),k$(3,6),k$(3,7),k$(3,8),k$(3,9)
9105 page1=page1+2
9110 if page1>page then print chr$(12):page1=1
9120 next z1
9150 print chr$(12):for z=1 to 100:next z:print chr$(12)
     console
9155 return
10000 chain "master6"
