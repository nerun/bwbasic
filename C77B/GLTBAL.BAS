1     rem This is the General Ledger Trial Balance Printer

%INCLUDE ALL.BAS
500     dim n(2,12),k$(2,10),h(9),s(9),t(4,9)
     RESTORE
1025 PRINT clear$:PRINT
 A(9)=0
1050 Q$="G E N E R A L  L E D G E R  T R I A L  B A L A N C E"
3000 rem READ NAME FILE SUBROUTINE
3005 a5=t%(12)
3080 Z=24:Z9=1
3090 FOR Z0=1 TO 4
3100 K$(0,Z0)=MID$(N$,Z9,Z)
3110 Z9=Z9+Z
3120 NEXT Z0
3125 rem ROUTINE TO ELIMINATE TRAILING BLANKS
     l8=1
3130 FOR I=1 TO 4
     l9=len(K$(0,I))
3140 FOR Z=l9 to l8 STEP -1
3150 IF MID$(K$(0,I),Z,1)<>" " THEN 3170
3160 NEXT Z
3170 Z$=""
3180 FOR Z0=1 TO Z
3190 Z$=Z$+MID$(K$(0,I),Z0,1)
3200 NEXT Z0
3210 K$(0,I)="":K$(0,I)=Z$
3220 NEXT I
6000 rem PRINT TRIAL BALANCE
6005 PRINT clear$:PRINT
6010 PRINT "PRINTING TRIAL BALANCE"
6015 FOR Z=0 TO 9
6020 H(Z)=0:S(Z)=0:T(0,Z)=0:T(1,Z)=0:T(2,Z)=0:T(3,Z)=0:A(Z)=0
6025 NEXT Z
     lprinter
     open "gl" recl 138 as 1
     open "glh" recl 150 as 2 
     open "gls" recl 150 as 3
     p9=0
6200 GOSUB 9600
6250 GOSUB 10000
6300 GOSUB 11000
6350 GOSUB 15000
6400 rem DO PARTIAL TOTALS OR TOTALS AND TEST
6402 IF N(2,2)=0 THEN GOSUB 16000
 if n(2,2)=0 then gosub 30000
6403 IF N(2,2)=0 THEN FOR Z=1 TO 56-P9:print:NEXT Z:GOSUB 9800
6405 IF N(2,2)=0 THEN 20000
6410 GOSUB 13000
6415 IF P9>56 THEN GOSUB 9700
6420 IF S(1)< H(1) THEN 6300
6430 IF S(1)=H(1) THEN GOSUB 12000
6450 GOTO 6250
9600 rem PRINT PAGE HEADING SUBROUTINE
9605 P0=1
 GOSUB 9610
 RETURN
9610 print :P9=P9+1
9615 print:P9=P9+1
9616 print TAB((t%(1)-LEN(Q$))/2);Q$:P9=P9+1
9617 print:P9=P9+1
9620 FOR Z=2 TO 4
9625 print TAB((t%(1)-LEN(K$(0,Z)))/2);K$(0,Z):P9=P9+1
9630 NEXT Z
9635 print:P9=P9+1
9640 print TAB(t%(10));D$(1);TAB(t%(11));"PAGE #";P0:P9=P9+1
9645 print :P9=P9+1
9650 print:P9=P9+1
9652 print TAB(6);"ACCOUNT #";TAB(20);"DESCRIPTION"
9655 print TAB(6);"DEBIT";TAB(26);"CREDIT";tab(56);"ADJUSTMENT"
9660 P9=P9+2
9665 print :P9=P9+1
9670 RETURN
9700 rem REPORT CONTINUED SUBROUTINE
9705 print
9710 print "report continues on next page"
9715 print
9720 print chr$(12):P9=0
9725 P0=P0+1
 GOSUB 9610
9740 RETURN
9800 rem END OF REPORT ROUTINE
9810 print
9820 print "end of report"
9830 print
9840 print chr$(12)
9850 RETURN
10000 rem GET HEADING LINE AND PRINT
10005 H(9)=H(9)+1
10006 if p9>56 then gosub 9700
      read #2,h(9);n(2,1),n(2,2),h$,h(0),h(1),h(2),h(3),           h(4),h(5),h(6),h(7)
10015 print TAB(t%(4));mid$(h$,3,28):P9=P9+1
10995 RETURN
11000 rem GET SUBHEADING LINE AND PRINT
11005 S(9)=S(9)+1
11006 if p9>56 then gosub 9700
      read #3,s(9);n(2,1),n(2,2),s$,s(0),s(1),s(2),s(3),           s(4),s(5),s(6),s(7)
11015 print TAB(t%(5));mid$(s$,3,28):P9=P9+1
11995 RETURN
12000 rem PRINT FINAL TOTAL
 print
12200 IF T4>=0 THEN 12305
12205 Z=26
12210 print TAB(6);"TOTAL "+MID$(H$,3,20)
12212 print TAB(Z);:print using l$;t4;
12216 print tab(66);"**************"
12217 print:print
12220 P9=P9+5
12225 GOTO 12990
12305 Z=6
12310 print TAB(6);"TOTAL "+MID$(H$,3,20)
12312 print TAB(Z);:print using l$;t4;
12316 print tab(66);"**************"
12317 print:print
12320 P9=P9+5
12990 h(2)=T4
      print #2,h(9);n(2,1),n(2,2),h$,h(0),h(1),h(2),h(3),           h(4),h(5),h(6),h(7)
 T4=0
12995 RETURN
13000 rem PRINT SUBTOTALS
 print
13200 IF T3>=0 THEN 13305
13205 Z=26
13210 print TAB(6);"TOTAL "+MID$(S$,3,20)
13213 print TAB(Z);:print using l$;t3;
13216 print tab(61);"=============="
13217 print
13220 P9=P9+4
13225 GOTO 13990
13305 Z=6
13310 print TAB(6);"TOTAL "+MID$(S$,3,20)
13313 print TAB(Z);:print using l$;t3;
13316 print tab(61);"=============="
13317 print
13320 P9=P9+4
13990 s(2)=T3
      print #3,s(9);n(2,1),n(2,2),s$,s(0),s(1),s(2),s(3),           s(4),s(5),s(6),s(7)
 T3=0
13995 RETURN
15000 rem GET ACCOUNTS IN SUBHEAD RANGE AND PRINT
15005 A(9)=A(9)+1
15006 IF P9>56 THEN GOSUB 9700
      read #1,a(9);n(2,1),n(2,2),k$(1,3),n(2,4),n(2,5),n(2,6),           n(2,7),n(2,8),n(2,9),n(2,10)
15105 IF N(2,2)=0 THEN 15995
 if n(2,4)=0 then 15990
15200 IF N(2,4)>=0 THEN 15305
15205 Z=26
15210 print TAB(6);N(2,2);TAB(26);K$(1,3)
15215 print TAB(Z);:print using l$;n(2,4);
15216 print tab(56);"--------------"
15220 T2=T2+N(2,4):T3=T3+N(2,4):T4=T4+N(2,4):P9=P9+3
15225 GOTO 15990
15305 Z=6
15310 print TAB(6);N(2,2);TAB(26);K$(1,3)
15315 print TAB(Z);:print using l$;n(2,4);
15316 print tab(56);"--------------"
15320 T1=T1+N(2,4):T3=T3+N(2,4):T4=T4+N(2,4):P9=P9+3
15990 IF N(2,2)< S(1) THEN 15005
15995 RETURN
16000 print "TOTALS AND DIFFERENCE"
16055 print using l$;tab(6);t1;tab(26);t2;tab(46);t1+t2
16995 RETURN
20000 rem ROUTINE TO CLOSE FILES AND RETURN TO BIZMII
20010 close 1
20020 close 2
20030 close 3
       console
20050 chain "master1"



30000 print chr$(12)
 s(9)=1:h(9)=1
 print "Summary of Trial Balance Headings and Subheadings"
 print
 p9=2
30010      read #2,h(9);n(2,1),n(2,2),h$,h(0),h(1),h(2),h(3),           h(4),h(5),h(6),h(7)
 if n(2,2)=0 then return
 print mid$(h$,3,28):print:p9=p9+2
30020      read #3,s(9);n(2,1),n(2,2),s$,s(0),s(1),s(2),s(3),           s(4),s(5),s(6),s(7)
 print tab(5);"Total ";mid$(s$,3,28);tab(56);
  print using l$;s(2)
  s(2)=0:p9=p9+1
      print #3,s(9);n(2,1),n(2,2),s$,s(0),s(1),s(2),s(3),           s(4),s(5),s(6),s(7)
  s(9)=s(9)+1
  if s(1)<h(1) then 30020
  print
 print tab(10);"Total ";mid$(h$,3,28);tab(66);
  print using l$;h(2)
 print:print
  h(2)=0:p9=p9+4
      print #2,h(9);n(2,1),n(2,2),h$,h(0),h(1),h(2),h(3),           h(4),h(5),h(6),h(7)
  h(9)=h(9)+1
  goto 30010
