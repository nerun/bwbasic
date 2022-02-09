1     rem This is the General Ledger Balance Sheet Printer

%INCLUDE ALL.BAS
500     dim n(2,12),k$(2,10),h(9),s(9),t(4,9)
     RESTORE
1009 PRINT clear$:PRINT
: A(9)=0
1010 PRINT "IS THIS BALANCE SHEET TO BE FOR THE MONTH TO DATE,"
1011 PRINT "QUARTER TO DATE, OR YEAR TO DATE (M, Q, OR Y)?"
1015 INPUT Z$
1017 IF ucase$(Z$)="M" THEN X=4:GOTO 1025
1020 IF ucase$(Z$)="Q" THEN X=5:GOTO 1025
1021 IF ucase$(Z$)="Y" THEN X=6:GOTO 1025
1023 GOTO 1010
1025 PRINT:PRINT"DO YOU WANT AN ORDINARY OR A COMPARATIVE"
1028 PRINT"BALANCE SHEET (O OR C) ?"
1030 INPUT U$
1050 PRINT clear$:PRINT
1075 IF ucase$(U$)="C" THEN Q$="COMPARATIVE BALANCE SHEET"
1076 IF ucase$(U$)="C" THEN GOTO 1100
1080 Q$="B A L A N C E  S H E E T"
1100 REM
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
6000 rem PRINT BALANCE SHEET
6005 PRINT clear$:PRINT
6010 PRINT "PRINTING BALANCE SHEET"
6015 FOR Z=0 TO 9
6020 H(Z)=0:S(Z)=0:T(0,Z)=0:T(1,Z)=0:T(2,Z)=0:T(3,Z)=0:A(Z)=0
6025 NEXT Z
6030 T1=0:T2=0:T3=0:T4=0
     lprinter
     open "gl" recl 138 as 1
     open "glh" recl 150 as 2
     open "gls" recl 150 as 3
6200 GOSUB 9600
6250 GOSUB 10000
6255 IF N(2,2)=0 THEN 6403
6300 GOSUB 11000
6350 GOSUB 15000
6400 rem DO PARTIAL TOTALS OR TOTALS AND TEST
6403 IF N(2,2)=0 THEN FOR Z=1 TO 51-P9:print:NEXT Z:GOSUB 9800
6405 IF N(2,2)=0 THEN 20000
6410 print:print TAB(t%(6));"TOTAL "+MID$(S$,3,20);TAB(t%(7));
          print using l$;abs(t1):T1=0:P9=P9+1
  print tab(t%(7));"-------------":p9=p9+1
6415 IF ucase$(U$)<>"C" THEN 6980
6420 print TAB(t%(6));"PREVIOUS";TAB(t%(7));
          print using l$;abs(t3):T3=0:P9=P9+1
  print tab(t%(7));"-------------":p9=p9+1
6980 IF P9>51 THEN GOSUB 9700
6985 IF S(1)< H(1) THEN 6300
6990 IF S(1)=H(1) THEN GOSUB 12000
6995 GOTO 6250
9600 rem PRINT PAGE HEADING SUBROUTINE
9605 P0=1
 GOSUB 9610
 RETURN
9610 print:P9=P9+1
9615 print:P9=P9+1
9616 print TAB((t%(1)-LEN(Q$))/2);Q$:P9=P9+1
9617 print:P9=P9+1
9620 FOR Z=2 TO 4
9625 print TAB((t%(1)-LEN(K$(0,Z)))/2);K$(0,Z):P9=P9+1
9630 NEXT Z
9635 print:P9=P9+1
9640 print TAB(t%(10));"AS OF ";D$(X);TAB(t%(11));"PAGE #";P0:P9=P9+1
9645 print:P9=P9+1
9670 RETURN
9700 rem REPORT CONTINUED SUBROUTINE
 print:print
9710 print "report continues on next page"
9720 print chr$(12):P9=0
9725 P0=P0+1
 GOSUB 9610
9740 RETURN
9800 rem end of report ROUTINE
9820 print "end of report"
9840 print chr$(12)
9850 RETURN
10000 rem GET HEADING LINE AND PRINT
10005 H(9)=H(9)+1
10006 if p9>51 then gosub 9700
      read #2,h(9);n(2,1),n(2,2),h$,h(0),h(1),h(2),h(3),           h(4),h(5),h(6),h(7)
10015 IF LEFT$(H$,2)="HS" OR LEFT$(H$,2)="HX" THEN N(2,2)=0:GOTO 10995
10020 print TAB(t%(4));mid$(h$,3,28):print:P9=P9+2
10995 RETURN
11000 rem GET SUBHEADING LINE AND PRINT
11005 S(9)=S(9)+1
11006 if p9>51 then gosub 9700
      read #3,s(9);n(2,1),n(2,2),s$,s(0),s(1),s(2),s(3),           s(4),s(5),s(6),s(7)
11015 print:print TAB(t%(5));mid$(s$,3,28):print:P9=P9+3
11995 RETURN
12000 rem PRINT FINAL TOTAL LINE
12010 print:P9=P9+1
12020 print TAB(t%(8));"TOTAL "+MID$(H$,3,20);TAB(t%(9));
          print using l$;abs(t2):P9=P9+1
  print tab(t%(9));"=============":p9=p9+1
12025 IF ucase$(U$)<>"C" THEN 12990
12030 print TAB(t%(8));"PREVIOUS";TAB(t%(9));
          print using l$;abs(t4):P9=P9+1
  print tab(t%(9));"=============":p9=p9+1
12990 print:print:p9=p9+2:T1=0:T2=0:T3=0:T4=0
12995 RETURN
15000 rem GET ACCOUNTS IN SUBHEAD RANGE AND PRINT
15005 A(9)=A(9)+1
15006 IF P9>51 THEN GOSUB 9700
      read #1,a(9);n(2,1),n(2,2),k$(1,3),n(2,4),n(2,5),n(2,6),           n(2,7),n(2,8),n(2,9),n(2,10)
      if n(2,2)=0 then 15995
 if n(2,x)=0 AND n(2,(x+3))=0 then 15990
15200 print TAB(t%(5)+2);K$(1,3);TAB(t%(5)+34);
          print using l$;abs(n(2,x)):P9=P9+1
15205 T1=T1+N(2,X):T2=T2+N(2,X):T3=T3+N(2,(X+3)):T4=T4+N(2,(X+3))
15210 IF ucase$(U$)<>"C" THEN 15990
15215 print TAB(t%(5)+2);"PREVIOUS";TAB(t%(5)+34);
          print using l$;abs(n(2,(x+3))):P9=P9+1
15990 IF N(2,2)< S(1) THEN 15005
15995 RETURN
20000 rem ROUTINE TO CLOSE FILES AND RETURN TO MASTER1
        close 1
        close 2
        close 3
        console
20050 CHAIN "master1"
