1     rem This is the Check Disbursements Journal Entry Program

%INCLUDE ALL.BAS
500     dim k$(6,20),n(2,20)
     repeat$="-----------------------------"
     fill$="                              "
 z5$="glcd":z6$="glcdback":z7$="glcdsize"
     L$="$#,###,###,###.##"
535 FOR Z=1 TO 72:delim$=delim$+"=":NEXT Z
540 K$(6,1)="1 - NEW ENTRY":K$(6,2)="2 - EXAMINE EXISTING ENTRY"
550 K$(6,3)="3 - MODIFY EXISTING ENTRY"
560 K$(6,4)="4 - DELETE EXISTING ENTRY"
570 K$(6,5)="5 - CREATE NEW FILE":K$(6,6)="6 - CLEAR EXISTING FILE"
580 K$(6,7)="7 - BACK-UP AND SORT FILE":K$(6,8)="8 - LIST FILE"
590 K$(6,9)="9 - FINISHED"
1200 K$(2,1)="1 - CHECK #":K$(2,2)="2 - BANK ACCT #":K$(2,3)="3 - DATE"
1205 K$(2,4)="4 - IN FAVOR OF":K$(2,5)="5 - REMARKS"
1210 K$(2,6)="6 - PRINT CHECKS(Y/N)":K$(2,7)="7 - AMOUNT OF CHECK"
 k$(2,8)="1st Entry Acct #"
 k$(2,9)="1st Entry Amount"
 k$(2,10)="2nd Entry Acct #"
 k$(2,11)="2nd Entry Amount"
 k$(2,12)="3rd Entry Acct #"
 k$(2,13)="3rd Entry Amount"
 k$(2,14)="4th Entry Acct #"
 k$(2,15)="4th Entry Amount"
 k$(2,16)="5th Entry Acct #"
 k$(2,17)="5th Entry Amount"
 k$(2,18)="6th Entry Acct #"
 k$(2,19)="6th Entry Amount"
1280 K$(2,20)="20- NONE"
1285 N(1,1)=6:N(1,2)=10:N(1,3)=8:N(1,4)=24:N(1,5)=24:N(1,6)=1
1290 N(1,7)=12:N(1,8)=10:N(1,9)=12:N(1,10)=10:N(1,11)=12:N(1,12)=10
1292 N(1,13)=12:N(1,14)=10:N(1,15)=12:N(1,16)=10:N(1,17)=12
1295 N(1,18)=10:N(1,19)=12
 print clear$:print
1300 PRINT "This program does a number of things to speed data entry."
1302 PRINT
1310 PRINT "1. It assigns CHECK NUMBERS automatically. To set the CHECK #"
1312 PRINT "   run the program CHKENT. If a '0' is entered, check numbers can"
1314 PRINT "   be entered manually (out of sequence). Any other number will be"
1316 PRINT "   considered the first number in a sequence and the number will"
1318 PRINT "   be entered automatically, then incremented to the next number."
1340 PRINT
1342 PRINT "2. You will be asked to enter the BANK ACCOUNT # to be credited."
1344 PRINT "   This number will be entered automatically in all records"
1346 PRINT "   prepared during the rest of this set of entry operations."
1348 PRINT
1350 PRINT "3. The date will be entered as the CURRENT DATE."
1360 PRINT
1362 PRINT "4. The PRINT CHECKS (Y/N) will automatically be set to 'Y'."
1364 PRINT "   Should you wish to override this entry, select MENU option"
1366 PRINT "   #6 when the program asks for any changes."
1370 PRINT
1372 PRINT "5. The amount of check should be entered as figures without a"
1374 PRINT "   minus sign. The program will force it to a negative (credit)"
1376 PRINT "   amount. Distributions will be set positive (debit)."
1378 PRINT:INPUT "Type <cr> to continue. ";LINE TEMP$
1400 PRINT CLEAR$:PRINT
1410 PRINT "Enter GENERAL LEDGER account number of the Bank  Account for"
1415 PRINT "credit. Although account number validity will be checked before"
1420 PRINT "posting, time will be saved if the account number is entered"
1425 PRINT "correctly here."
1430 PRINT
1450 INPUT LINE BANKNUM$:BANKNUM=VAL(BANKNUM$)
1455 PRINT "Is ";BANKNUM;" the correct account number? (Y or N)."
1460 PRINT:INPUT LINE TEMP$
1465 IF UCASE$(LEFT$(TEMP$,1)) <> "Y" THEN 1400

1500 if end #1 then 6000
 if end #2 then 15000
  open z5$ recl 250 as 1
  close 1
  open z7$ as 2
  read #2;z2,z3
  close 2

1600 REM
     console
1620 PRINT CLEAR$
 if z2>z3 then print "*** OUT OF RECORD SPACE ***"
 PRINT "CHECK DISBURSEMENT ENTRY PROGRAM"
  PRINT      "--------------------------------"
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
1682 IF Z=1 THEN new$="N"
1685 ON Z GOSUB 2000,3000,4000,5000,6000,7000,8000,9000,10000
1690 FOR Z=1 TO 20:N(2,Z)=0:NEXT Z:N$=""
1700 FOR Z=1 TO 20:K$(3,Z)="":NEXT Z
1702 new$=""
1703 FLAG$=" "
1705 GOTO 1600
2000 IF Z2>Z3 THEN RETURN
2001 PRINT CLEAR$:PRINT
2005 PRINT "RECORD NUMBER";Z2:PRINT
2010 GOSUB 24000
2015 N(2,2)=BANKNUM
2020 K$(3,3)=D$(1)
2025 K$(3,6)="Y"
2030 FOR Z=4 TO 5
 PRINT CUR$
2034 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2038 PRINT k$(2,Z);TAB(28);
2040 INPUT LINE K$(3,Z):PRINT chr$(13)
 if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2044 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,i)
 PRINT CLEAR$
2048 NEXT Z
2050 Z=7
 PRINT CUR$
2054 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2058 PRINT k$(2,Z);TAB(28);
2060 INPUT N(2,Z):PRINT chr$(13)
 PRINT CLEAR$
2064 IF N(2,7)>0 THEN N(2,7)=N(2,7)*(-1)
2066 FOR Z=8.0 TO 19.0 STEP 2.0
 PRINT CUR$
2070 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2074 PRINT k$(2,Z);TAB(28);
2076 INPUT N(2,Z):PRINT chr$(13)
 PRINT CLEAR$
 PRINT CUR$
2082 PRINT TAB(30);left$(repeat$,n(1,z+1))
 PRINT UP$;
2086 PRINT k$(2,Z+1);TAB(28);
2088 INPUT N(2,Z+1):PRINT chr$(13)
 PRINT CLEAR$
       PRINT CUR$
2090 INPUT "Is this the last entry (Y or N)";LINE TEMP$
2092 IF UCASE$(LEFT$(TEMP$,1))="Y" THEN 2096
2094 NEXT Z
2096 N$=""
2098 FOR Z=3 TO 6:N$=N$+K$(3,Z):NEXT Z
2100 Z1=Z2
2105 gosub 11000
2163 PRINT"TO MAKE A CHANGE, TYPE THE APPROPRIATE #. IF NONE TYPE 20, RETURN"
2164 INPUT Z
2165 IF Z>19 THEN 2216
2170 IF Z<1 THEN 2105
2175 IF Z>2 AND Z<7 THEN 2200
2180 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2185 PRINT k$(2,Z);TAB(30);
2190 INPUT N(2,Z)
2195 GOTO 2225
2200 PRINT TAB(30);left$(repeat$,n(1,z))
 PRINT UP$;
2205 PRINT k$(2,Z);TAB(30);
2210 INPUT LINE K$(3,Z)
 if len(k$(3,z))>n(1,z) then k$(3,z)=left$(k$(3,z),n(1,z))
2215 I=(N(1,Z)-LEN(K$(3,Z))):K$(3,Z)=K$(3,Z)+left$(fill$,i)
2216 N$=""
2220 FOR Z=3 TO 6:N$=N$+K$(3,Z):NEXT Z
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
2280 open z5$ recl 250 as 1
     print #1,z1; N(2,1),N(2,2),N$,N(2,7),N(2,8),N(2,9),N(2,10),     N(2,11),N(2,12),N(2,13),N(2,14),     N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
     close 1
2295 IF new$="N" THEN Z2=Z2+1:open z7$ as 1:print #1;z2,z3:close 1
2300 RETURN
3000 PRINT clear$:PRINT
3005 INPUT "RECORD NUMBER";Z1
3010 IF Z1>=Z2 THEN 3000
3012 IF Z1<1 THEN RETURN
3015 open z5$ recl 250 as 1
     read #1,z1; N(2,1),N(2,2),N$,N(2,7),N(2,8),N(2,9),N(2,10),     N(2,11),N(2,12),N(2,13),N(2,14),     N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
3022 Z9=1:FOR Z=3 TO 6:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):next z
3025 CLOSE 1
3105 GOSUB 11000
3170 PRINT"FOR A NEW RECORD, TYPE N - TYPE F IF FINISHED.":INPUT line temp$
3190 IF temp$="n" or temp$="N" THEN 3000
3195 RETURN
4000 PRINT clear$:PRINT
4005 N$=""
4010 INPUT "RECORD NUMBER";Z1
4015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
4020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
4025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOTO 1620
4030 open z5$ recl 250 as 1
     read #1,z1; N(2,1),N(2,2),N$,N(2,7),N(2,8),N(2,9),N(2,10),     N(2,11),N(2,12),N(2,13),N(2,14),     N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
4036 Z9=1:FOR Z=3 TO 6:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
4040 CLOSE 1
4045 IF N(2,1)=0 THEN PRINT "*** DELETED RECORD ***":GOTO 1620
4075 GOTO 2105
5000 PRINT clear$:PRINT
5010 INPUT "RECORD NUMBER";Z1
5015 IF Z1<1 THEN PRINT "*** NO SUCH RECORD ***":GOTO 1620
5020 IF Z1>Z3 THEN PRINT "*** OUT OF RANGE ***":GOTO 1620
5025 IF Z1>=Z2 THEN PRINT "NO RECORD NUMBER";Z1:GOTO 1620
5030 open z5$ recl 250 as 1
     read #1,z1; N(2,1),N(2,2),N$,N(2,7),N(2,8),N(2,9),N(2,10),     N(2,11),N(2,12),N(2,13),N(2,14),     N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
5036 Z9=1:FOR Z=3 TO 6:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
5040 CLOSE 1
5105 GOSUB 11000
5200 INPUT "IS RECORD TO BE DELETED (MUST BE YES TO DELETE)";line temp$
5205 IF left$(temp$,1)="n" OR left$(temp$,1)="N" THEN RETURN
5210 IF temp$<>"YES" THEN 5200
5215 N(2,1)=0
5220 open z5$ recl 250 as 1
     print #1,z1; N(2,1),N(2,2),N$,N(2,7),N(2,8),N(2,9),N(2,10),     N(2,11),N(2,12),N(2,13),N(2,14),     N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
5230 close 1
5235 RETURN
6000 PRINT clear$:PRINT
6005 PRINT "IF YOU HAVE ARRIVED HERE, AND HAVE A CHECK DISBURSEMENTS FILE"
6010 PRINT "ALREADY ON A DISK, YOU SHOULD INSTALL THAT DISK THEN"
6015 PRINT "TYPE THE LETTER C FOLLOWED BY A RETURN TO CONTINUE."
6020 PRINT
6025 PRINT "IF YOU WISH TO CREATE A NEW FILE, TYPE THE LETTER N"
6030 PRINT "FOLLOWED BY RETURN.":PRINT
6035 INPUT line temp$
6040 IF temp$="c" OR temp$="C" THEN INITIALIZE:GOTO 1500
6045 PRINT clear$:PRINT
6050 INPUT "NUMBER OF RECORDS DESIRED";Z3
6055 PRINT
6060 n$="":for z=1 to 70:n$=n$+" ":next z
     for z=1 to 20:n(2,z)=0:next z
     create z5$ recl 250 as 1
6070 for z1=1 to z3+2
     print #1,z1; N(2,1),N(2,2),N$,N(2,7),N(2,8),N(2,9),N(2,10),     N(2,11),N(2,12),N(2,13),N(2,14),     N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
     next z1
     close 1
6095 PRINT clear$:PRINT
6100 PRINT "CHECK DISBURSEMENTS FILE CREATED AND CLEARED.":PRINT
6105 PRINT z3;"RECORDS CREATED.":PRINT
6110 PRINT "TO CONTINUE, TYPE RETURN.":INPUT line temp$
 Z2=1
 create z7$ as 1:print #1;Z2,Z3:close 1
6115 GOTO 1500
7000 open z5$ recl 250 as 1
     n$="":for z=1 to 70:n$=n$+" ":next z
     for z=1 to 20:n(2,z)=0:next z
7006 INPUT "ARE YOU SURE !!! (YES OR NO)";temp$
7007 IF temp$<>"YES" THEN close 1:return
7010 FOR Z1=1 TO z3+2
     print #1,z1; N(2,1),N(2,2),N$,N(2,7),N(2,8),N(2,9),N(2,10),     N(2,11),N(2,12),N(2,13),N(2,14),     N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
7025 next z1
7030 close 1
7035 PRINT clear$:PRINT
7040 PRINT "CHECK DISBURSEMENTS FILE CLEARED!":PRINT
7045 PRINT "TO CONTINUE, TYPE RETURN."
7050 INPUT line temp$
 Z2=1
 open z7$ as 1:print #1;Z2,Z3:close 1
7060 RETURN
8000 CHAIN "GLCDSORT"
9000 print clear$:print:print "listing"
     p1=1:p2=1:p3=2
     t3=0:t4=0:t5=0
     lprinter
     open z5$ recl 250 as 1
     for z0=1 to (z2/2)+1
     gosub 9500
     gosub 9100
     if n(2,2)=0 then gosub 9800:goto 9090
     gosub 9300
     next z0
     print chr$(12)
     console
9090 return
9100 for z1=p2 to p3
     read #1,z1;n(2,1),n(2,2),n$,n(2,7),n(2,8),n(2,9),n(2,10),          n(2,11),n(2,12),n(2,13),n(2,14),n(2,15),n(2,16),n(2,17),          n(2,18),n(2,19),FLAG$
     z9=1:for z=3 to 10:k$(3,z)=mid$(n$,z9,n(1,z)):z9=z9+n(1,z):next z
     if n(2,2)=0 then 9170
     t5=t5+n(2,7)
     t1=0:t2=0
     print:print:print
     gosub 11000
     next z1
     p2=p2+2:p3=p3+2
9170 return
9300 print
     print "report continues on next page"
     print
     print chr$(12)
     return
9500 print
     print
     print tab(66);"Page Number ";p1:p1=p1+1
     print:print
     return
9800 print
     print "*** Summary ***":print
     print "Total Debits";tab(35);:print using l$;t3
     print "Total Credits";tab(55);:print using l$;t4
     print "Difference";tab(55);:print using l$;t3+t4:print
     print
     print "end of report"
     print chr$(12):for z=1 to 100:next z: print chr$(12)
     close 1
     return
10000 CHAIN "master11"
11000 PRINT clear$;
      T1=0:T2=0
      PRINT "RECORD NUMBER";Z1;TAB(31);"CHECK DISBURSEMENT ENTRY"
      PRINT K$(2,1);TAB(16);N(2,1);TAB(24);K$(2,2);N(2,2);
      PRINT TAB(51);K$(2,3);TAB(62);K$(3,3)
      PRINT K$(2,4);TAB(21);K$(3,4)
      PRINT K$(2,5);TAB(21);K$(3,5);TAB(51);K$(2,6);K$(3,6)
      PRINT K$(2,7);TAB(21);:print using l$;n(2,7):T2=T2+N(2,7)
      PRINT delim$
      PRINT "POSTING #";TAB(11);"ACCT #";TAB(35);"DEBIT";TAB(55);"CREDIT"
      PRINT delim$
      PRINT "#1";TAB(11);"8 -";N(2,8);TAB(31);"9 -";
      IF N(2,9)<=0 THEN PRINT TAB(55);:print using l$;n(2,9):T2=T2+N(2,9)
      IF N(2,9)>0 THEN PRINT TAB(35);:print using l$;n(2,9):T1=T1+N(2,9)
      PRINT "#2";TAB(11);"10-";N(2,10);TAB(31);"11-";
      IF N(2,11)<=0 THEN PRINT TAB(55);:print using l$;n(2,11):T2=T2+N(2,11)
      IF N(2,11)>0 THEN PRINT TAB(35);:print using l$;n(2,11):T1=T1+N(2,11)
      PRINT "#3";TAB(11);"12-";N(2,12);TAB(31);"13-";
      IF N(2,13)<=0 THEN PRINT TAB(55);:print using l$;n(2,13):T2=T2+N(2,13)
      IF N(2,13)>0 THEN PRINT TAB(35);:print using l$;n(2,13):T1=T1+N(2,13)
      PRINT "#4";TAB(11);"14-";N(2,14);TAB(31);"15-";
      IF N(2,15)<=0 THEN PRINT TAB(55);:print using l$;n(2,15):T2=T2+N(2,15)
      IF N(2,15)>0 THEN PRINT TAB(35);:print using l$;n(2,15):T1=T1+N(2,15)
      PRINT "#5";TAB(11);"16-";N(2,16);TAB(31);"17-";
      IF N(2,17)<=0 THEN PRINT TAB(55);:print using l$;n(2,17):T2=T2+N(2,17)
      IF N(2,17)>0 THEN PRINT TAB(35);:print using l$;n(2,17):T1=T1+N(2,17)
      PRINT "#6";TAB(11);"18-";N(2,18);TAB(31);"19-";
      IF N(2,19)<=0 THEN PRINT TAB(55);:print using l$;n(2,19):T2=T2+N(2,19)
      IF N(2,19)>0 THEN PRINT TAB(35);:print using l$;n(2,19):T1=T1+N(2,19)
      PRINT TAB(35);"=================";TAB(55);"================="
      PRINT "TOTALS";TAB(35);:print using l$;t1;TAB(55);:print using l$;t2
      PRINT:PRINT "DIFFERENCE";TAB(55);:print using l$;t1+t2:PRINT
 t3=t3+t1:t4=t4+t2
      return
15000 print clear$:print:print "CHECKING FILE LENGTH"
 PRINT:PRINT "*** PLEASE WAIT ***"
 open z5$ recl 250 as 1
 z3=(size(z5$)*block.size)/250
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
24000 OPEN "CHK" AS 1
24005 READ #1;N(2,1)
24010 CLOSE 1
24030 IF N(2,1)<>0 THEN 24800
24035 PRINT CUR$
24040 PRINT TAB(30);LEFT$(REPEAT$,N(1,1))
24045 PRINT UP$;
24050 PRINT K$(2,1);TAB(28);
24060 INPUT N(2,1):PRINT CHR$(13)
24065 PRINT CLEAR$
24070 GOTO 24995
24800 IF Z=3 THEN 24995
24900 A=N(2,1)+1
24910 OPEN "CHK" AS 1
24915 PRINT #1;A
24920 CLOSE 1
24995 RETURN
