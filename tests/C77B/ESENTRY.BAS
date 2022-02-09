1     rem This is the California State Tax Table Entry Program

%INCLUDE ALL.BAS
500     dim G(11,5),b(11,5),c(11,5),d(11,5),e(11,5),f(20)
     dim n(2,20),k$(3,20)
     l$="$##,###.##":u$=".###":r$="###.##  "
     z5$="es"
     RESTORE
540 K$(3,1)="1 - ENTER STATE TABLES":K$(3,2)="2 - EXAMINE STATE TABLES"
550 K$(3,3)="3 - MODIFY STATE TABLES"
570 K$(3,4)="4 - CREATE NEW FILE":K$(3,5)="5 - CLEAR EXISTING FILE"
590 K$(3,6)="6 - LIST STATE TABLES":K$(3,7)="7 - FINISHED"
     print clear$:print
1100 K$(1,1)="AT LEAST":K$(1,2)="BUT NOT OVER"
1110 K$(1,3)="BASE":K$(1,4)="%":K$(1,5)="OF EXCESS OVER"
1200 K$(2,1)="SINGLE TAXPAYER ANNUAL TABLES"
1210 K$(2,2)="MARRIED TAXPAYER ANNUAL TABLE"
1220 K$(2,3)="HEAD OF HOUSEHOLD ANNUAL TABLE"
1230 K$(2,4)="CALIFORNIA TAX CREDIT TABLE"
1250 N(1,1)=12:N(1,2)=12:N(1,3)=12:N(1,4)=12:N(1,5)=12
     if end #1 then 6000
1500 open z5$ as 1
     close 1
1510 Z1=1:Z2=1
1600 PRINT clear$
1605 PRINT
1650 PRINT "THIS IS A LIST OF OPERATIONS."
1655 PRINT
1660 FOR Z=1 TO 7:PRINT K$(3,Z):NEXT Z:PRINT
1665 PRINT "INDICATE WHAT YOU WOULD LIKE TO DO BY TYPING"
1670 PRINT "THE CORRESPONDING NUMBER."
1675 PRINT
1680 INPUT Z
1685 ON Z GOSUB 2000,3000,4000,6000,7000,9000,10000
1690 FOR X=1 TO 11:FOR Y=1 TO 5:G(X,Y)=0:B(X,Y)=0:NEXT Y:NEXT X
1695 FOR X=1 TO 11:FOR Y=1 TO 5:C(X,Y)=0:D(X,Y)=0:E(X,Y)=0:NEXT Y:NEXT X
1696 FOR Z=1 TO 20:F(Z)=0: NEXT Z
1705 GOTO 1600
2000 GOSUB 20000
2010 GOSUB 30000
2105 PRINT clear$:print
2110 GOSUB 40000
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
2280 open z5$ as 1
     for x=1 to 11
     print #1;c(x,1),c(x,2),c(x,3),c(x,4),c(x,5)
     print #1;d(x,1),d(x,2),d(x,3),d(x,4),d(x,5)
     print #1;e(x,1),e(x,2),e(x,3),e(x,4),e(x,5)
     next x
     for x=1 to 16
     print #1;f(x)
     next x
     close 1
2995 RETURN
3000 PRINT clear$:PRINT
3002 open z5$ as 1
     for x=1 to 11
     read #1;c(x,1),c(x,2),c(x,3),c(x,4),c(x,5)
     read #1;d(x,1),d(x,2),d(x,3),d(x,4),d(x,5)
     read #1;e(x,1),e(x,2),e(x,3),e(x,4),e(x,5)
     next x
     for x=1 to 16
     read #1;f(x)
     next x
     close 1
3400 PRINT clear$
3506 PRINT K$(2,1)
3508 PRINT "ROW";TAB(6);K$(1,1);TAB(21);K$(1,2);TAB(36);K$(1,3);TAB(51);
3510 PRINT K$(1,4);TAB(61);K$(1,5)
3512 FOR X=1 TO 11
3516 print x;tab(6);:print using l$;c(x,1);tab(21);c(x,2);tab(36);c(x,3);
     print using u$;tab(51);c(x,4);:print using l$;tab(61);c(x,5)
3520 NEXT X
3525 INPUT "FOR NEXT CHART TYPE RETURN.";line temp$
3600 PRINT clear$:PRINT:PRINT K$(2,2)
3608 PRINT "ROW";TAB(6);K$(1,1);TAB(21);K$(1,2);TAB(36);K$(1,3);TAB(51);
3610 PRINT K$(1,4);TAB(61);K$(1,5)
3612 FOR X=1 TO 11
3616 print x;tab(6);:print using l$;d(x,1);tab(21);d(x,2);tab(36);d(x,3);
     print using u$;tab(51);d(x,4);:print using l$;tab(61);d(x,5)
3620 NEXT X
3625 INPUT "FOR NEXT CHART TYPE RETURN.";line temp$
3700 PRINT clear$:PRINT:PRINT K$(2,3)
3708 PRINT "ROW";TAB(6);K$(1,1);TAB(21);K$(1,2);TAB(36);K$(1,3);TAB(51);
3710 PRINT K$(1,4);TAB(61);K$(1,5)
3712 FOR X=1 TO 11
3716 print x;tab(6);:print using l$;e(x,1);tab(21);e(x,2);tab(36);e(x,3);
     print using u$;tab(51);e(x,4);:print using l$;tab(61);e(x,5)
3720 NEXT X
3725 INPUT "FOR NEXT CHART TYPE RETURN.";line temp$
3800 PRINT clear$:PRINT:PRINT K$(2,4):PRINT
3820 FOR Z=1 TO 7
3830 print using r$;f(z);
3840 NEXT Z
3850 print using r$;f(8)
3860 FOR Z=9 TO 15
3870 print using r$;f(z);
3880 NEXT Z
3890 print using r$;f(16)
3895 PRINT
3950 INPUT "WHEN FINISHED TYPE RETURN.";line temp$
3995 RETURN
4000 PRINT clear$:PRINT
4002 open z5$ as 1
     for x=1 to 11
     read #1;c(x,1),c(x,2),c(x,3),c(x,4),c(x,5)
     read #1;d(x,1),d(x,2),d(x,3),d(x,4),d(x,5)
     read #1;e(x,1),e(x,2),e(x,3),e(x,4),e(x,5)
     next x
     for x=1 to 16
     read #1;f(x)
     next x
     close 1
4995 GOTO 2105
6000 PRINT clear$:PRINT
6005 PRINT "IF YOU HAVE ARRIVED HERE, AND HAVE A STATE TABLES FILE"
6010 PRINT "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6015 PRINT "TYPE THE LETTER C FOLLOWED BY A RETURN TO CONTINUE."
6020 PRINT
6025 PRINT "IF YOU WISH TO CREATE A NEW FILE, TYPE THE LETTER N"
6030 PRINT "FOLLOWED BY RETURN.":PRINT
6035 INPUT line temp$
6040 IF temp$="c" OR temp$="C" THEN INITIALIZE:GOTO 1500
6045 PRINT clear$:PRINT
6065 create z5$ as 1
     for x=1 to 11
     print #1;c(x,1),c(x,2),c(x,3),c(x,4),c(x,5)
     print #1;d(x,1),d(x,2),d(x,3),d(x,4),d(x,5)
     print #1;e(x,1),e(x,2),e(x,3),e(x,4),e(x,5)
     next x
     for x=1 to 16
     print #1;f(x)
     next x
     close 1
6495 PRINT clear$:PRINT
6500 PRINT "STATE TABLES FILE CREATED AND CLEARED.":PRINT
6510 PRINT "TO CONTINUE, TYPE RETURN.":INPUT line temp$
6515 GOTO 1600
7000 INPUT "ARE YOU SURE !!! (YES OR NO)";line temp$
7002 IF temp$<>"YES" THEN RETURN
     for x=1 to 11:for y=1 to 5:c(x,y)=0:d(x,y)=0:e(x,y)=0:next y:next x
     for z=1 to 16:f(z)=0:next z
7004 open z5$ as 1
     for x=1 to 11
     print #1;c(x,1),c(x,2),c(x,3),c(x,4),c(x,5)
     print #1;d(x,1),d(x,2),d(x,3),d(x,4),d(x,5)
     print #1;e(x,1),e(x,2),e(x,3),e(x,4),e(x,5)
     next x
     for x=1 to 16
     print #1;f(x)
     next x
     close 1
7935 PRINT clear$:PRINT
7940 PRINT "STATE TABLES FILE CLEARED!":PRINT
7945 PRINT "TO CONTINUE, TYPE RETURN."
7950 INPUT line temp$
7960 RETURN
9000 PRINT clear$:PRINT
     print "Listing":print
     lprinter
9002 open z5$ as 1
     for x=1 to 11
     read #1;c(x,1),c(x,2),c(x,3),c(x,4),c(x,5)
     read #1;d(x,1),d(x,2),d(x,3),d(x,4),d(x,5)
     read #1;e(x,1),e(x,2),e(x,3),e(x,4),e(x,5)
     next x
     for x=1 to 16
     read #1;f(x)
     next x
     close 1
9506 PRINT K$(2,1)
9508 PRINT "ROW";TAB(6);K$(1,1);TAB(21);K$(1,2);TAB(36);K$(1,3);TAB(5:GOTO 40000
41000 PRINT clear$:PRINT:PRINT K$(2,2)
41008 PRINT "ROW";TAB(6);K$(1,1);TAB(21);K$(1,2);TAB(36);K$(1,3);TAB(51);
41010 PRINT K$(1,4);TAB(61);K$(1,5)
41012 FOR X=1 TO 11
41016 print x;tab(6);:print using l$;d(x,1);tab(21);d(x,2);tab(36);d(x,3);
      print using u$;tab(51);d(x,4);:print using l$;tab(61);d(x,5)
41020 NEXT X
41030 PRINT "TO CHANGE AN ITEM TYPE M FOLLOWED BY ROW AND COLUMN"
41040 PRINT "TYPE F FOLLOWED BY RETURN IF NO CHANGES"
41050 INPUT line temp$
41055 IF temp$="F" THE
9620 NEXT X
9700 PRINT:PRINT K$(2,3)
9708 PRINT "ROW";TAB(6);K$(1,1);TAB(21);K$(1,2);TAB(36);K$(1,3);TAB(51);
9710 PRINT K$(1,4);TAB(61);K$(1,5)
9712 FOR X=1 TO 11
9716 print x;tab(6);:print using l$;e(x,1);tab(21);e(x,2);tab(36);e(x,3);
     print using u$;tab(51);e(x,4);:print using l$;tab(61);e(x,5)
9720 NEXT X
9800 PRINT:PRINT K$(2,4):PRINT
9820 FOR Z=1 TO 7
9830 print using r$;f(z);
9840 NEXT Z
9850 print using r$;f(8)
9860 FOR Z=9 TO 15
9870 print using r$;f(z);
9880 NEXT Z
9890 print using r$;f(16)
9895 PRINT
     print chr$(12):for z=1 to 100:next z:print chr$(12)
     console
9995 RETURN
10000 CHAIN "bizmii"
20000 PRINT clear$:PRINT
20010 Z1=1
20020 PRINT K$(2,Z1)
20030 FOR X=1 TO 11
20040 PRINT "VALUES FOR ROW ";X
20050 FOR Y=1 TO 5
20060 PRINT TAB(30);"------------"
	PRINT UP$;
20070 PRINT K$(1,Y);TAB(30);
20080 IF Z1=1 THEN INPUT C(X,Y):PRINT chr$(13)
20090 IF Z1=2 THEN INPUT D(X,Y):PRINT chr$(13)
20100 IF Z1=3 THEN INPUT E(X,Y):PRINT chr$(13)
20110 NEXT Y
20120 PRINT clear$:PRINT:PRINT K$(2,Z1)
20130 NEXT X
20140 IF Z1<3 THEN Z1=Z1+1:PRINT clear$:PRINT:GOTO 20020
20150 RETURN
30000 PRINT clear$:PRINT
30010 PRINT K$(2,4)
30020 FOR X=1 TO 16
30030 PRINT TAB(30);"------------"
	PRINT UP$;
      print TAB(30);
30040 INPUT F(X):PRINT chr$(13)
30050 NEXT X
30060 RETURN
40000 PRINT K$(2,1)
40008 PRINT "ROW";TAB(6);K$(1,1);TAB(21);K$(1,2);TAB(36);K$(1,3);TAB(51);
40010 PRINT K$(1,4);TAB(61);K$(1,5)
40012 FOR X=1 TO 11
40016 print x;tab(6);:print using l$;c(x,1);tab(21);c(x,2);tab(36);c(x,3);
      print using u$;tab(51);c(x,4);:print using l$;tab(61);c(x,5)
40020 NEXT X
40030 PRINT "TO CHANGE AN ITEM TYPE S FOLLOWED BY ROW AND COLUMN"
40040 PRINT "TYPE F FOLLOWED BY RETURN IF NO CHANGES"
40050 INPUT line temp$
40055 IF temp$="F" THEN 41000
40060 INPUT "ROW ";X:INPUT "COLUMN ";Y
40065 IF temp$<>"S" THEN PRINT clear$:PRINT:GOTO 40000
40070 PRINT TAB(30);"------------"
	PRINT UP$;
40075 PRINT K$(1,Y);TAB(30);
40080 INPUT C(X,Y):PRINT clear$:PRINT:GOTO 40000
41000 PRINT clear$:PRINT:PRINT K$(2,2)
41008 PRINT "ROW";TAB(6);K$(1,1);TAB(21);K$(1,2);TAB(36);K$(1,3);TAB(51);
41010 PRINT K$(1,4);TAB(61);K$(1,5)
41012 FOR X=1 TO 11
41016 print x;tab(6);:print using l$;d(x,1);tab(21);d(x,2);tab(36);d(x,3);
      print using u$;tab(51);d(x,4);:print using l$;tab(61);d(x,5)
41020 NEXT X
41030 PRINT "TO CHANGE AN ITEM TYPE M FOLLOWED BY ROW AND COLUMN"
41040 PRINT "TYPE F FOLLOWED BY RETURN IF NO CHANGES"
41050 INPUT line temp$
41055 IF temp$="F" THEN 42000
41060 INPUT "ROW ";X:INPUT "COLUMN ";Y
41065 IF temp$<>"M" THEN PRINT clear$:PRINT:GOTO 41000
41070 PRINT TAB(30);"------------"
	PRINT UP$;
41075 PRINT K$(1,Y);TAB(30);
41080 INPUT D(X,Y):PRINT clear$:PRINT:GOTO 41000
42000 PRINT clear$:PRINT:PRINT K$(2,3)
42008 PRINT "ROW";TAB(6);K$(1,1);TAB(21);K$(1,2);TAB(36);K$(1,3);TAB(51);
42010 PRINT K$(1,4);TAB(61);K$(1,5)
42012 FOR X=1 TO 11
42016 print x;tab(6);:print using l$;e(x,1);tab(21);e(x,2);tab(36);e(x,3);
      print using u$;tab(51);e(x,4);:print using l$;tab(61);e(x,5)
42020 NEXT X
42030 PRINT "TO CHANGE AN ITEM TYPE H FOLLOWED BY ROW AND COLUMN"
42040 PRINT "TYPE F FOLLOWED BY RETURN IF NO CHANGES"
42050 INPUT line temp$
42055 IF temp$="F" THEN 43000
42060 INPUT "ROW ";X:INPUT "COLUMN ";Y
42065 IF temp$<>"H" THEN PRINT clear$:PRINT:GOTO 42000
42070 PRINT TAB(30);"------------"
	PRINT UP$;
42075 PRINT K$(1,Y);TAB(30);
42080 INPUT E(X,Y):PRINT clear$:PRINT:GOTO 42000
43000 PRINT clear$:PRINT:PRINT K$(2,4):PRINT
43020 FOR Z=1 TO 7
43030 print using r$;f(z);
43040 NEXT Z
43050 print using r$;f(8)
43060 FOR Z=9 TO 15
43070 print using r$;f(z);
43080 NEXT Z
43090 print using r$;f(16)
43100 PRINT:PRINT"TO CHANGE AN ITEM TYPE C FOLLOWED BY THE NUMBER (1 TO 16)"
43110 PRINT "TYPE F FOLLOWED BY RETURN IF NO CHANGES"
43115 INPUT line temp$
43120 IF temp$="F" THEN RETURN
43125 INPUT Z
43130 IF temp$<>"C" THEN 43000
43135 PRINT TAB(30);"------------"
	PRINT UP$;
43140 PRINT TAB(30);
43145 INPUT F(Z)
43150 GOTO 43000
