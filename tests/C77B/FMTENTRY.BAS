1     rem This is the Dollar and Percentage Format Entry Program

%INCLUDE ALL.BAS
500     RESTORE
	DIM T%(20),A(16),D$(8),N(2,20),DATE1$(8)
 BIZMII.FLAG=0
505 z5$="glf"
506 z6$="glfback"
515 repeat$="-------------------------------"
516 fill$="                                "
520 dim z$(5,5),n(2,20),k$(6,20)
540 K$(6,1)="1 - ENTER $ 0R % FORMATS":K$(6,2)="2 - EXAMINE $ 0R % FORMATS"
550 K$(6,3)="3 - MODIFY $ OR % FORMATS"
570 K$(6,4)="4 - CREATE NEW FILE":K$(6,5)="5 - CLEAR EXISTING FILE"
590 K$(6,6)="6 - FINISHED"
1000 rem THIS IS THE $ OR % FORMAT ENTRY PROGRAM
1005 PRINT clear$
1010 PRINT
1075 K$(1,1)="DOLLAR FORMAT":K$(1,2)="PERCENT FORMAT"
1110 K$(2,1)="1 - DOLLAR FORMAT":K$(2,2)="2 - PERCENT FORMAT"
1145 K$(2,3)="3 - NONE"
1150 N(1,1)=20:N(1,2)=10
     if end #1 then 6000
1500 open z5$ as 1
     close 1
1580 Z1=1:Z2=1
1600 PRINT clear$
1605 PRINT
1650 PRINT "THIS IS A LIST OF OPERATIONS."
1655 PRINT
1660 FOR Z=1 TO 6:PRINT K$(6,Z):NEXT Z:PRINT
1665 PRINT "INDICATE WHAT YOU WOULD LIKE TO DO BY TYPING"
1670 PRINT "THE CORRESPONDING NUMBER."
1675 PRINT
1680 INPUT Z
	IF Z<1 OR Z>6 then 1600
1685 ON Z GOSUB 2000,3000,4000,6000,7000,10000
1705 GOTO 1600
2000 PRINT clear$:PRINT
2002 PRINT "FORMATTING NUMBERS IS RELATIVELY SIMPLE. FOR EACH"
2004 PRINT "DIGIT POSITION DESIRED, INSERT A #."
2012 PRINT "IF A DOLLAR SIGN IS REQUIRED, PLACE IT FIRST"
2014 PRINT "IN THE STRING. POSITIONS AFTER THE DECIMAL POINT ARE"
2016 PRINT "INDICATED BY '.##' FOR AS MANY DECIMAL POSITIONS AS"
2018 PRINT "DESIRED."
2041 PRINT
2044 FOR Z=1 TO 2
:	PRINT CUR$
2045 PRINT TAB(30);left$(repeat$,n(1,z))
:	PRINT UP$;
2050 PRINT K$(1,Z);TAB(30);
2055 input line K$(3,Z):PRINT chr$(13)
:	PRINT CLEAR$
2085 NEXT Z
2090 L$=K$(3,1):P$=K$(3,2)
2105 PRINT clear$:PRINT
2115 FOR Z=1 TO 3
2120 PRINT K$(2,Z);TAB(30);K$(3,Z)
2125 NEXT Z
2145 PRINT
2150 PRINT "IF AN ITEM IS TO BE CHANGED, TYPE THE APPROPRIATE NUMBER."
2155 PRINT:INPUT Z
2160 PRINT
2165 IF Z>2 THEN 2216
2170 IF Z<1 THEN 2105
2180 PRINT TAB(30);left$(repeat$,n(1,z))
	PRINT UP$;
2185 PRINT K$(1,Z);TAB(30);
2190 input line K$(3,Z)
2216 L$="":P$=""
2220 L$=K$(3,1):P$=K$(3,2)
2225 input "ANY MORE CHANGES";line temp$
2230 if left$(temp$,1)="y" or left$(temp$,1)="Y" then 2280
2235 PRINT
2240 input "IS RECORD TO BE ENTERED";line temp$
2245 PRINT
     if left$(temp$,1)="y" or left$(temp$,1)="Y" then 2280
     if left$(temp$,1)<>"n" and left$(temp$,1)<>"N" then 2235
2260 PRINT clear$:PRINT
2270 PRINT "*** RECORD NOT ENTERED ***":PRINT:PRINT
2275 FOR Z=1 TO 200:NEXT Z:RETURN
2280 open z5$ as 1
     print #1;l$,p$
     close 1
2300 RETURN
3000 PRINT clear$:PRINT
     open z5$ as 1
     read #1;l$,p$
     close 1
3030 PRINT
3032 K$(3,1)=L$:K$(3,2)=P$
3035 FOR Z=1 TO 2
3040 PRINT K$(1,Z);TAB(30);K$(3,Z)
3065 NEXT Z
3070 PRINT
3080 input "IF FINISHED, TYPE RETURN.";line temp$
3095 RETURN
4000 PRINT clear$:PRINT
4005 L$="":P$=""
4010 open z5$ as 1
     read #1;l$,p$
     close 1
4045 K$(3,1)=L$:K$(3,2)=P$
4070 GOTO 2105
6000 PRINT clear$:PRINT
6005 PRINT "IF YOU HAVE ARRIVED HERE, AND HAVE A FORMAT FILE"
6010 PRINT "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6015 PRINT "TYPE THE letter c FOLLOWED BY A RETURN TO CONTINUE."
6020 PRINT
6025 PRINT "IF YOU WISH TO CREATE A NEW FILE, TYPE THE letter n"
6030 PRINT "FOLLOWED BY RETURN.":PRINT
6035 input line temp$
     if left$(temp$,1)="c" or left$(temp$,1)="C" then initialize:GOTO 1500
6045 PRINT clear$:PRINT
     l$="":p$="":for z=1 to 20:l$=l$+" ":p$=p$+" ":next z
6055 create z5$ as 1
     print #1;l$,p$
     close 1
6095 PRINT clear$:PRINT
6100 PRINT "FORMAT FILE CREATED AND CLEARED.":PRINT
6110 input "TO CONTINUE, TYPE RETURN.";line temp$
6115 GOTO 1600
7000 INPUT "ARE YOU SURE !!! (YES OR NO)";line temp$
     if left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" then return
     l$="":p$="":for z=1 to 20:l$=l$+" ":p$=p$+" ":next z
7015 open z5$ as 1
     print #1;l$,p$
     close 1
7035 PRINT clear$:PRINT
7040 PRINT "FORMAT FILE CLEARED!":PRINT
7045 input "TO CONTINUE, TYPE RETURN.";line temp$
7060 RETURN
10000 print clear$:print:print "Replace disk #5 in drive A."
      print "Type return to continue."
      input line temp$
      initialize
      chain "bizmii"
