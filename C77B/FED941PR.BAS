1     rem This is the Federal Form 941 Quarterly Tax Return Printer

%INCLUDE ALL.BAS
500     dim n(2,20),k$(3,20),h(9),s(9),t(4,9)
     print clear$:print
     RESTORE
1055 T$="#,###.##"
1060 U$="##,###.##"
1065 V$="###,###.##"
1075 N(1,3)=24:N(1,4)=24:N(1,5)=24:N(1,6)=17:N(1,7)=2:N(1,8)=5
1080 N(1,9)=14:N(1,10)=11:N(1,11)=8:N(1,12)=8:N(1,13)=1:N(1,14)=1:N(1,15)=1
1120 Z9=1
1145 Z9=0
1400 REM THIS PART GETS CUTOFFS FOR FICA,SDI,FUTA AND SUTA
     open "epc" as 1
     read #1;c1,c2,c3,c4,c5,c6,c7,c8
     close 1
3000 REM READ NAME FILE SUBROUTINE
3005 a5=T%(12)
3080 N(1,10)=20:N(1,11)=20
3085 Z9=1:FOR Z=3 TO 11:K$(0,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
3090 N(1,10)=11:N(1,11)=8
3125 REM ROUTINE TO ELIMINATE TRAILING BLANKS
     l8=1
3130 FOR I=3 TO 11
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
6000 REM PRINT FEDERAL FORM 941
6005 PRINT clear$:PRINT:PRINT "PRINTING FEDERAL FORM 941":P0=1
6015 open "ep" recl 512 as 1
6020 z3=(size("ep")*block.size)/512
6025 FOR Z2=1 TO Z3
6030 read #1,z2;N(2,1),N(2,2),N$,N,R,H1,H2,H3
6035 IF N(2,2)=0 THEN 6045
6040 NEXT Z2
6045 close 1
6050 Z2=Z2-1
6090 P1=1:P2=8
6095 open "ep" recl 512 as 1
6100 FOR Z1=1 TO Z2
9000 REM GET RECORD AND PRINT
9010 read #1,z1;N(2,1),N(2,2),N$,N,R,H1,H2,H3,          E0,E1,E2,F1,F2,F3,E3,E4,E5,E6,E7,E8,S1,S2,S3,M1,M2,M3
9050 T1=T1+E2:T2=T2+E5
9060 IF E2>C1 THEN Z6=Z6+C1:GOTO 9080
9070 Z6=Z6+E2
9080 NEXT Z1
9085 close 1
9100 PRINT clear$:PRINT
9105 PRINT "ENTER ADJUSTMENT OF WITHHELD INCOME TAX FOR PRECEDING"
9110 PRINT "QUARTER - LINE 4. ENTER INCREASE AS +, DECREASE AS -"
9115 PRINT:INPUT T3
9150 PRINT clear$:PRINT
9155 PRINT "ENTER TAXABLE TIPS REPORTED - LINE 7."
9160 PRINT:INPUT Z7
9200 PRINT clear$:PRINT
9205 PRINT "ENTER ADJUSTMENT OF FICA TAXES - LINE 9. ENTER INCREASE"
9210 PRINT "AS +, DECREASE AS -"
9215 PRINT:INPUT T7
9250 PRINT clear$:PRINT
9255 PRINT "SET FORM 941 TO FIRST PRINT LINE.":PRINT
9260 INPUT "ENTER QUARTER NUMBER (1,2,3 OR 4). ";line temp$
     lprinter
9270 IF VAL(temp$)=1 THEN print TAB(78);Z2:GOTO 9280
9280 print TAB(74);:print using v$;t1
9285 T4=Z6*C2*2:T5=Z7*C2:T6=T4+T5
9290 print TAB(74);:print using v$;t2
9295 print TAB(74);:print using v$;t3
9300 print TAB(74);:print using v$;t2+t3
9310 print using v$;tab(41);z6;tab(74);t4
9315 print using v$;tab(41);z7;tab(74);t5
9320 print using v$;tab(74);t6
9325 print using v$;tab(74);t7
9330 print using v$;tab(74);t6+t7
9335 print using v$;tab(74);t2+t3+t6+t7
9340 print
9350 T9=T2+T3+T6+T7
     console
9400 PRINT clear$:PRINT
9410 PRINT "ENTER OVERPAYMENT FROM PREVIOUS QUARTER."
9415 PRINT:INPUT Z
     lprinter
9420 T4=Z
9500 T1=0:T2=0:T3=0:T5=0:T6=0
9510 print TAB(62);:print using v$;z
9520 open "edep" recl 64 as 1
9525 I=1
9530 FOR Z1=1 TO 48
9540 read #1,z1;N(2,1),N(2,2),N(2,3),N(2,4),N(2,5),N$
9545 IF N(2,1)<>VAL(temp$) THEN 9600
9550 IF N(2,3)<>4 THEN GOSUB 20000
9555 IF N(2,3)=4 THEN GOSUB 20500
9600 NEXT Z1
9605 close 1
9610 print using v$;tab(39);t3;tab(62);t4
     console
9630 PRINT clear$:PRINT
9640 PRINT "ENTER FINAL DEPOSIT MADE IF NOT INCLUDED ABOVE."
9650 PRINT:INPUT Z
9660 PRINT "ENTER DATE OF FINAL DEPOSIT IF ANY."
9670 PRINT:INPUT Z$
     lprinter
9680 IF Z>0 THEN print TAB(51);Z$;TAB(62);:print using v$;z:T4=T4+Z:GOTO 9700
9690 print TAB(62);:print using v$;z
9700 print:print
9710 print TAB(74);:print using v$;t4
9720 print:print:print
9730 print TAB(74);:print using v$;t9-t4
     print chr$(12)
9915 console
9920 CHAIN "master5"
20000 T1=T1+N(2,4):T2=T2+N(2,5)
20005 print using v$;tab(39);n(2,4);tab(51);
      PRINT RIGHT$(N$,8);:PRINT USING V$;TAB(62);N(2,5)
20030 RETURN
20500 T1=T1+N(2,4):T2=T2+N(2,5)
20505 print using v$;tab(39);n(2,4);tab(51);
      PRINT RIGHT$(N$,8);:PRINT USING V$;TAB(62);N(2,5)
     print using v$;tab(39);t1;tab(62);t2
20530 T3=T3+T1:T4=T4+T2:T1=0:T2=0
20550 RETURN
