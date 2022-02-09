1     rem This is the Order/Invoice Entry Program 

%INCLUDE ALL.BAS
500     dim n(2,20),k$(4,20),g(10),q(10)
     l$="$$##,###.##":u$="##########":v$="####"
     repeat$="--------------------------------"
     fill$="                                "
     z5$="ir":z6$=z5$+"back":z7$=z5$+"size"
     RESTORE
     for z=1 to 70:delim1$=delim1$+"*":delim2$=delim2$+"=":next z
fmt1$ ="Rec No. ####  1 - Tag  ########     2 - A/R Account No.  ##########"
fmt2$ ="3 - Invoice No.   ##########        4 - Salesman  /             /"
fmt3$ ="5 - Cust Ord No. /               /  6 - Ship Date        /         /"
fmt4$ ="7 - Carrier (Via) /       /         8 - FOB Point /         /"
fmt5$ ="9 - Terms  /         /              10- Tax Code  !  Flags  /   /"
fmt6$ ="12- Customer Rec No. ####           13- Shipping Chgs $##########.##"
fmt7$ ="     Item Record No     Quantity         Item Record No     Quantity"
fmt8$ ="No 1          ####    ##########    No 2          ####    ##########"
fmt9$ ="No 3          ####    ##########    No 4          ####    ##########"
fmt10$="No 5          ####    ##########    No 6          ####    ##########"
fmt11$="No 7          ####    ##########    No 8          ####    ##########"
fmt12$="No 9          ####    ##########    No 10         ####    ##########"
540 K$(4,1)="1 - NEW ENTRY":K$(4,2)="2 - EXAMINE EXISTING ENTRY"
550 K$(4,3)="3 - MODIFY EXISTING ENTRY"
560 K$(4,4)="4 - DELETE EXISTING ENTRY"
570 K$(4,5)="5 - CREATE NEW FILE":K$(4,6)="6 - CLEAR EXISTING FILE"
580 K$(4,7)="7 - BACK-UP AND SORT FILE":K$(4,8)="8 - LIST FILE"
590 K$(4,9)="9 - FINISHED"
     print clear$:print
1100 K$(1,1)="TAG":K$(1,2)="A/R ACCT #":K$(1,3)="INVOICE #"
1105 K$(1,4)="SALESMAN":K$(1,5)="CUST. ORDER #":K$(1,6)="SHIPPING DATE"
1110 K$(1,7)="CARRIER (VIA)":K$(1,8)="FOB POINT":K$(1,9)="TERMS (1,2, OR 3)"
1130 K$(1,10)="TAX CODE":K$(1,11)="FLAGS"
1132 K$(1,12)="CUSTOMER REC #"
1135 K$(1,13)="SHIPPING CHARGES"
1200 K$(2,1)="1 - TAG":K$(2,2)="2 - A/R ACCT #":K$(2,3)="3 - INVOICE #"
1205 K$(2,4)="4 - SALESMAN":K$(2,5)="5 - CUST. ORDER #"
1206 K$(2,6)="6 - SHIPPING DATE":K$(2,7)="7 - CARRIER(VIA)"
1210 K$(2,8)="8 - FOB POINT":K$(2,9)="9 - TERMS (1,2, OR 3)"
1230 K$(2,10)="10- TAX CODE":K$(2,11)="11- FLAGS"
1232 K$(2,12)="12- CUSTOMER REC #"
1235 K$(2,13)="13- SHIPPING CHARGES"
1300 N(1,1)=8:N(1,2)=10:N(1,3)=10:N(1,4)=14:N(1,5)=10:N(1,6)=8
1305 N(1,7)=8:N(1,8)=10:N(1,9)=10:N(1,10)=1:N(1,11)=4:N(1,12)=4
1310 N(1,13)=12:G(0)=4:Q(0)=10
 print clear$:print

1500 if end #1 then 6000
 if end #2 then 15000
  open z5$ recl 384 as 1
  close 1
  open z7$ as 2
  read #2;z2,z3
  close 2

1600 REM
     console
1620 PRINT CLEAR$
 if z2>z3 then print "*** OUT OF RECORD SPACE ***"
 PRINT "INVOICE REGISTER ENTRY PROGRAM"
  PRINT      "------------------------------"
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
1700 FOR Z=1 TO 20:K$(3,Z)="":NEXT Z
     for z=1 to 10:g(z)=0:q(z)=0:next z
1720 GOTO 1600
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
2036 Z=0:GOSUB 24000
2040 FOR Z=4 TO 10
 PRINT CUR$
2042 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2044 PRINT K$(1,Z);TAB(30);
2046 INPUT line K$(3,Z):PRINT chr$(13)
 if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2048 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,i)
 PRINT CLEAR$
2050 NEXT Z
2052 N$=""
2054 FOR Z=4 TO 10:N$=N$+K$(3,Z):NEXT Z
2056 FOR Z=12 TO 13
 PRINT CUR$
2058 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2060 PRINT K$(1,Z);TAB(30);
2062 INPUT N(2,Z):PRINT chr$(13)
 PRINT CLEAR$
2064 NEXT Z
2066 FOR Z=1 TO 10
 PRINT CUR$
2068 PRINT TAB(30);left$(repeat$,g(0))
 PRINT UP$;
2070 PRINT "FINISHED GOODS REC # ";TAB(30);
2072 INPUT G(Z):PRINT chr$(13)
 PRINT CLEAR$
 PRINT CUR$
2074 PRINT TAB(30);left$(repeat$,q(0))
 PRINT UP$;
2076 PRINT "QUANTITY ";TAB(30);
2078 INPUT Q(Z):PRINT chr$(13)
2080 PRINT:INPUT "IS THIS THE LAST ITEM? ";line temp$
2082 IF left$(temp$,1)="y" or left$(temp$,1)="Y" THEN 2100
 PRINT CLEAR$
2084 NEXT Z
2100 Z1=Z2
2105 PRINT clear$;
2110 GOSUB 20000
2115 PRINT "IF A FINISHED GOODS REC # OR QUANTITY IS TO BE CHANGED"
2116 PRINT "TYPE # - RETURN. OTHERWISE TYPE THE NUMBER PRECEEDING AN ENTRY"
2117 PRINT "FOLLOWED BY RETURN.":PRINT
2118 PRINT "IF YOU ARE FINISHED, TYPE 14 - RETURN";
2119 INPUT line temp$
2120 IF temp$="#" THEN GOSUB 25000
2125 Z=VAL(temp$)
2150 IF Z>13 THEN 2216
2160 IF Z=3 THEN GOSUB 24000
2177 IF Z<1 THEN 2105
2178 IF Z>3 AND Z<12 THEN 2200
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
2217 FOR I=4 TO 11:N$=N$+K$(3,I):NEXT I
2225 INPUT "ANY MORE CHANGES";line temp$
2230 IF left$(temp$,1)="y" or left$(temp$,1)="Y" THEN 2105
2235 PRINT
2240 INPUT "IS RECORD TO BE ENTERED";line temp$
2245 PRINT
2250 IF left$(temp$,1)="y" OR left$(temp$,1)="Y" THEN 2280
2255 IF left$(temp$,1)<>"n" AND left$(temp$,1)<>"N" THEN 2235
2260 PRINT clear$:PRINT
2270 PRINT "*** RECORD NOT ENTERED ***":PRINT:PRINT
2275 FOR Z=1 TO 200:NEXT Z:RETURN
2280 open z5$ recl 384 as 1
     print #1,z1;N(2,1),N(2,2),N(2,3),N$,N(2,12),N(2,13),          G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),          G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
     close 1
2295 IF new$="N" THEN Z2=Z2+1:open z7$ as 1:print #1;z2,z3:close 1
2300 RETURN
3000 PRINT clear$:PRINT:INPUT "RECORD NUMBER";Z1:PRINT clear$:PRINT
3003 IF Z1>=Z2 THEN 3000
3004 IF Z1<1 THEN RETURN
3005 open z5$ recl 384 as 1
     read #1,z1;N(2,1),N(2,2),N(2,3),N$,N(2,12),N(2,13),          G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),          G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
3008 Z9=1:FOR Z=4 TO 11:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
3009 CLOSE 1
3015 PRINT clear$:PRINT
3020 GOSUB 20000
3075 PRINT "FOR NEW RECORD, TYPE N-RETURN , OTHERWISE F-RETURN"
3085 INPUT line temp$
3090 IF ucase$(temp$)="N" THEN 3000
3095 RETURN
4000 PRINT clear$:PRINT
4005 N$=""
4010 INPUT "RECORD NUMBER";Z1
4015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOSUB 4100:GOTO 1620
4020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOSUB 4100:GOTO 1620
4025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOSUB 4100:GOTO 1620
4030 open z5$ recl 384 as 1
     read #1,z1;N(2,1),N(2,2),N(2,3),N$,N(2,12),N(2,13),          G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),          G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
     close 1
4045 IF N(2,1)=0 THEN PRINT "*** DELETED RECORD ***":GOTO 1620
4050 Z9=1
4055 FOR Z=4 TO 11
4060 K$(3,Z)=MID$(N$,Z9,N(1,Z))
4065 Z9=Z9+N(1,Z)
4070 NEXT Z
4075 GOTO 2105
4100 FOR AAA=1 TO 200 :NEXT AAA :RETURN
5000 PRINT clear$:PRINT:INPUT "RECORD NUMBER";Z1:PRINT clear$:PRINT
5002 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOSUB 4100:GOTO 1620
5004 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOSUB 4100:GOTO 1620
5005 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOSUB 4100:GOTO 1620
5006 open z5$ recl 384 as 1
     read #1,z1;N(2,1),N(2,2),N(2,3),N$,N(2,12),N(2,13),          G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),          G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
     close 1
5010 Z9=1:FOR Z=4 TO 11:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
5015 PRINT clear$:PRINT
5020 GOSUB 20000
5100 INPUT "IS RECORD TO BE DELETED (MUST BE YES TO DELETE)";line temp$
5105 IF left$(temp$,1)="n" OR left$(temp$,1)="N" THEN RETURN
5110 IF ucase$(temp$)<>"YES" THEN 5100
5115 N(2,1)=0
5120 open z5$ recl 384 as 1
     print #1,z1;N(2,1),N(2,2),N(2,3),N$,N(2,12),N(2,13),          G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),          G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
5130 close 1
5135 RETURN
6000 PRINT clear$:PRINT
6005 PRINT "IF YOU HAVE ARRIVED HERE, AND HAVE A INVOICE REG FILE"
6010 PRINT "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6015 PRINT "TYPE THE LETTER C FOLLOWED BY A RETURN TO CONTINUE."
6020 PRINT
6025 PRINT "IF YOU WISH TO CREATE A NEW FILE, TYPE THE LETTER N"
6030 PRINT "FOLLOWED BY RETURN.":PRINT
6035 INPUT line temp$
6040 IF ucase$(temp$)="C" then initialize:goto 1500
6045 PRINT clear$:PRINT
6050 INPUT "NUMBER OF RECORDS DESIRED";z3
6055 PRINT
6060 n$="":for z=1 to 150:n$=n$+" ":next z
     for z=1 to 20:n(2,z)=0:n(0,z)=0:next z
6062 FOR Z=1 TO 10:G(Z)=0:Q(Z)=0:NEXT Z
6065 create z5$ recl 384 as 1
6070 FOR Z1=1 TO z3+2
     print #1,z1;N(2,1),N(2,2),N(2,3),N$,N(2,12),N(2,13),          G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),          G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
6085 NEXT Z1
6090 close 1
6095 PRINT clear$:PRINT
6100 PRINT "INVOICE REG FILE CREATED AND CLEARED.":PRINT
6105 PRINT z3;"RECORDS CREATED.":PRINT
6110 PRINT "TO CONTINUE, TYPE RETURN.":INPUT line temp$
 Z2=1
 create z7$ as 1:print #1;Z2,Z3:close 1
6115 GOTO 1500
7000 INPUT "ARE YOU SURE !!! (YES OR NO)";line temp$
7007 IF ucase$(temp$)<>"YES" THEN RETURN
7008 n$="":for z=1 to 150:n$=n$+" ":next z
     for z=1 to 20:n(2,z)=0:n(0,z)=0:next z
     for z=1 to 10:g(z)=0:q(z)=0:next z
     open z5$ recl 384 as 1
7010 FOR Z1=1 TO z3+2
     print #1,z1;N(2,1),N(2,2),N(2,3),N$,N(2,12),N(2,13),          G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),          G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
7025 NEXT Z1
7030 close 1
7035 PRINT clear$:PRINT
7040 PRINT "INVOICE REG FILE CLEARED!":PRINT
7045 PRINT "TO CONTINUE, TYPE RETURN."
7050 INPUT line temp$
 Z2=1
 open z7$ as 1:print #1;Z2,Z3:close 1
7060 RETURN
8000 chain "crjosort"
9000 PRINT clear$:PRINT
9100 lprinter
9105 open z5$ recl 384 as 1
9110 FOR I=1 TO Z2-1
     read #1,i;N(2,1),N(2,2),N(2,3),N$,N(2,12),N(2,13),          G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),          G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
9140 Z9=1:FOR Z=4 TO 11:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
     print using fmt1$;i,n(2,1),n(2,2)
     print using fmt2$;n(2,3),k$(3,4)
     print using fmt3$;k$(3,5),k$(3,6)
     print using fmt4$;k$(3,7),k$(3,8)
     print using fmt5$;k$(3,9),k$(3,10),k$(3,11)
     print using fmt6$;n(2,12),n(2,13)
     print delim2$
     print fmt7$
     print using fmt8$;g(1),q(1),g(2),q(2)
     print using fmt9$;g(3),q(3),g(4),q(4)
     print using fmt10$;g(5),q(5),g(6),q(6)
     print using fmt11$;g(7),q(7),g(8),q(8)
     print using fmt12$;g(9),q(9),g(10),q(10)
     print
9800 NEXT I
 CLOSE 1
9995 RETURN
10000 CHAIN "master2"
15000 print clear$:print:print "CHECKING FILE LENGTH"
 PRINT:PRINT "*** PLEASE WAIT ***"
 open z5$ recl 384 as 1
 z3=(size(z5$)*block.size)/384
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
20000 print using fmt1$;z1,n(2,1),n(2,2)
     print using fmt2$;n(2,3),k$(3,4)
     print using fmt3$;k$(3,5),k$(3,6)
     print using fmt4$;k$(3,7),k$(3,8)
     print using fmt5$;k$(3,9),k$(3,10),k$(3,11)
     print using fmt6$;n(2,12),n(2,13)
     print delim2$
     print fmt7$
     print using fmt8$;g(1),q(1),g(2),q(2)
     print using fmt9$;g(3),q(3),g(4),q(4)
     print using fmt10$;g(5),q(5),g(6),q(6)
     print using fmt11$;g(7),q(7),g(8),q(8)
     print using fmt12$;g(9),q(9),g(10),q(10)
     print
20250 RETURN
24000 open "inv" as 1
      read #1;n(2,3)
      close 1
24030 IF N(2,3)<>0 THEN 24800
 PRINT CUR$
24040 PRINT TAB(30);left$(repeat$,n(1,3))
 PRINT UP$;
24050 PRINT K$(1,3);TAB(30);
24060 INPUT N(2,3):PRINT chr$(13)
 PRINT CLEAR$
24070 GOTO 24995
24800 IF Z=3 THEN 24995
24900 A=N(2,3)+1
24910 open "inv" as 1
      print #1;a
      close 1
24995 RETURN
25000 INPUT "ENTER INVOICE ITEM # ";Z
25010 PRINT TAB(30);left$(repeat$,g(0))
 PRINT UP$;
25020 PRINT "FINISHED GOODS REC # ";TAB(30);
25030 INPUT G(Z):PRINT chr$(13)
25040 PRINT TAB(30);left$(repeat$,q(0))
 PRINT UP$;
25050 PRINT "QUANTITY ";TAB(30);
25060 INPUT Q(Z):PRINT chr$(13)
25070 temp$="14"
25995 RETURN
