1     rem This is the Aged Accounts Receivable Statement Printer

%INCLUDE ALL.BAS
500     RESTORE
     dim n(2,20),k$(3,20),h(9),s(9),t(4,9)
1009 PRINT clear$:PRINT
1020 N(1,3)=24:N(1,4)=24:N(1,5)=24:N(1,6)=17:N(1,7)=2:N(1,8)=5
1025 N(1,9)=14:N(1,10)=24:N(1,11)=24:N(1,12)=24:N(1,13)=17:N(1,14)=2
1030 N(1,15)=5:N(1,16)=14:N(1,17)=8
1080 Q$="A G E D  A C C O U N T S  R E C E I V A B L E  R E P O R T"
1090 U$="#,###,###.##":u1$="##########":u2$="###,###.##"
1100 REM GET DATES ROUTINE
     for z=1 to t%(1):del$=del$+"*":del1$=del1$+"=":del2$=del2$+"-":next z
3000 REM READ NAME FILE SUBROUTINE
3005 a5=T%(12)
3080 Z=24:Z9=1
3090 FOR Z0=1 TO 4
3100 K$(0,Z0)=MID$(N$,Z9,Z)
3110 Z9=Z9+Z
3120 NEXT Z0
3125 REM ROUTINE TO ELIMINATE TRAILING BLANKS
     l8=1
3130 FOR I=1 TO 4
     l9=len(k$(0,i))
3140 for z=l9 to l8 step -1
3150 IF MID$(K$(0,I),Z,1)<>" " THEN 3170
3160 NEXT Z
3170 Z$=""
3180 FOR Z0=1 TO Z
3190 Z$=Z$+MID$(K$(0,I),Z0,1)
3200 NEXT Z0
3210 K$(0,I)="":K$(0,I)=Z$
3220 NEXT I
6000 REM PRINT AGED ACCOUNTS RECEIVABLE REPORT
6005 PRINT clear$:PRINT
6010 PRINT "PRINTING AGED ACCOUNTS RECEIVABLE REPORT"
6015 FOR Z=0 TO 9
6020 H(Z)=0:S(Z)=0:T(0,Z)=0:T(1,Z)=0:T(2,Z)=0:T(3,Z)=0:A(Z)=0
6025 NEXT Z
6030 T1=0:T2=0:T3=0:T4=0:P0=1:ytd=0
6035 X=1
6100 P9=0
6200 open "cr" recl 384 as 1
6205 z3=(size("cr")*block.size)/384
6210 FOR Z2=1 TO Z3
6215 read #1,z2;n(2,1),n(2,2),n$
6220 IF N(2,2)=0 THEN 6230
6225 NEXT Z2
6230 close 1
6235 Z2=Z2-1
6240 PRINT:PRINT "RECORDS TO BE PRINTED ";Z2
6245 PRINT:INPUT "TO PRINT REPORT, TYPE RETURN.";line temp$
	lprinter
6300 open "cr" recl 384 as 1
6310 J=1
6320 FOR K=1 TO (Z2/7)+1
6330 GOSUB 9600
6340 FOR I=J TO J+6
6350 GOSUB 15000
6360 NEXT I
6370 GOSUB 9700:J=J+7
6380 NEXT K
6390 GOSUB 15600
6400 GOSUB 9800
6410 GOTO 20000
9600 REM PRINT PAGE HEADING SUBROUTINE
9610 print del$:P9=P9+1
9615 print:P9=P9+1
     IF J>1 then 9635
9616 print TAB((T%(1)-LEN(Q$))/2);Q$:P9=P9+1
9617 print:P9=P9+1
9620 FOR Z=2 TO 4
9625 print TAB((T%(1)-LEN(K$(0,Z)))/2);K$(0,Z):P9=P9+1
9630 NEXT Z
9635 print:P9=P9+1
9640 print TAB(T%(10));"DATE ";D$(X);TAB(T%(11));"PAGE #";P0
9642 P9=P9+1
9645 print del1$:P9=P9+1
9650 print " REC# - NAME";TAB(36);"DATE OF LAST PAYMENT - AMOUNT";
9655 print TAB(67);"YTD PAYMENTS"
9660 print "ACCT# - PHONE#";TAB(29);"CUR-CHGS";"    30-60/CHGS";
9665 print "    60-90/CHGS      90+/CHGS"
9670 print del1$:P9=P9+1
9695 RETURN
9700 REM REPORT CONTINUED SUBROUTINE
9705 print del1$
9710 print "Report continues on next page"
9715 print del1$
9720 print chr$(12):P9=0
9725 P0=P0+1
9740 RETURN
9800 REM End of report ROUTINE
9810 print del1$
9820 print "End of report"
9830 print del$
9840 print chr$(12)
9845 FOR Z=1 TO 100:NEXT Z:print chr$(12)
9850 RETURN
15000 IF I>Z2 THEN 15595
15005 read #1,i;n(2,1),n(2,2),n$,c1,c2,c3,c4,c5,c6
15020 Z9=1:FOR Z=3 TO 17:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
15030 print I;" - ";K$(3,4);TAB(43);K$(3,17);TAB(53);
      print using u$;c1;tab(68);c2
15050 print using u1$;n(2,2);:print "  ";k$(3,9);:print using u2$;c3;
      print using u$;tab(38);c4;tab(53);c5;tab(68);c6
15070 IF C3+C4+C5+C6>=0 THEN 15090
15080 print TAB(57);"*** CREDIT BALANCE ***"
15090 T1=T1+C3:T2=T2+C4:T3=T3+C5:T4=T4+C6
      ytd=ytd+c2
15550 print:print
15595 RETURN
15600 print :print 
15700 print del1$
15710 T5=T4+T3+T2+T1
15720 print "TOTALS:";TAB(27);:print using u2$;t1;tab(38);
      print using u$;t2;tab(53);t3;tab(68);t4
15730 print "REPORT TOTAL:";TAB(68);:print using u$;t5
      print:print "TOTAL Y-T-D RECEIPTS";TAB(68);:print using u$;ytd
15740 print del1$
15795 RETURN
20000 REM ROUTINE TO CLOSE FILES AND RETURN TO master2
20005 close 1
20040 console
20050 CHAIN "master2"
