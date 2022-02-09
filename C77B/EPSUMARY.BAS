1     rem This is the Payroll Register Printer

%INCLUDE ALL.BAS
500 DIM K$(3,20)
     dim h(9),s(9),t(4,9),N(2,20)
     t$="#,###.##":u$="##,###.##":v$="###,###.##"
     t5$="##########":t6$="####"
     fmt$="/.../     ###,###.##  ##,###.##  #,###.##  #,###.##  ##,###.##"
     fmt$=fmt$+"  ##,###.##  ###,###.##  ###,###.##  ##,###.##  ##,###.##"
     RESTORE
1050 Q$="E M P L O Y E E  P A Y R O L L  R E G I S T E R"
1075 N(1,3)=24:N(1,4)=24:N(1,5)=24:N(1,6)=17:N(1,7)=2:N(1,8)=5
1080 N(1,9)=14:N(1,10)=11:N(1,11)=8:N(1,12)=8:N(1,13)=1:N(1,14)=1:N(1,15)=1
1235 T%(1)=132
1240 FOR Z=1 TO 131:A$=A$+"=":B$=B$+"-":c5$=c5$+"*":NEXT Z
1400 rem this part gets cutoffs for futa and suta
     open "epc" as 1
     read #1;c1,c2,c3,c4,c5,c6,c7,c8
     close 1
3000 REM READ NAME FILE SUBROUTINE
3005 a5=T%(12)
 Z=24:Z9=1
 FOR Z0=1 TO 4
 K$(0,Z0)=MID$(N$,Z9,Z)
 Z9=Z9+Z
 NEXT Z0
3125 REM ROUTINE TO ELIMINATE TRAILING BLANKS
     l8=1
3130 FOR I=1 TO 4
     l9=len(k$(0,i))
3140 FOR Z=l9 TO l8 STEP -1
3150 IF MID$(K$(0,I),Z,1)<>" " THEN 3170
3160 NEXT Z
3170 Z$=""
3180 FOR Z0=1 TO Z
3190 Z$=Z$+MID$(K$(0,I),Z0,1)
3200 NEXT Z0
3210 K$(0,I)="":K$(0,I)=Z$
3220 NEXT I
6000 REM PRINT PAYROLL REGISTER
6005 PRINT clear$:PRINT:PRINT "PRINTING PAYROLL REGISTER":P0=1
6010 lprinter
6015 open "ep" recl 512 as 1
     z3=(size("ep")*block.size)/512
     for z2=1 to z3
     read #1,z2;n(2,1),n(2,2)
     if n(2,2)=0 then 6045
     next z2
6045 close 1
     z2=z2-1
6090 P1=1:P2=8
6095 open "ep" recl 512 as 1
6100 FOR I=1 TO (Z2/8)+1
6105 Z=0
6110 GOSUB 9600
6120 FOR Z1=P1 TO P2
6130 GOSUB 9000
6200 NEXT Z1
6210 GOSUB 9400
6220 P1=P1+8:P2=P2+8
6250 NEXT I
6300 GOSUB 9800
6400 GOTO 9900
9000 REM GET RECORD AND PRINT
9010 read #1,z1;n(2,1),n(2,2),n$,n,r,h1,h2,h3,e0,e1,e2,f1,f2,f3,          e3,e4,e5,e6,e7,e8,s1,s2,s3,m1,m2,m3
9020 IF N(2,2)=0 THEN 9395
9050 H2=H2+H1:H3=H3+H1:S2=S2+S1:S3=S3+S1:M2=M2+M1:M3=M3+M1
9052 E1=E1+E0:E2=E2+E0:E4=E4+E3:E5=E5+E3:E7=E7+E6:E8=E8+E6
9054 F2=F2+F1:F3=F3+F1
9095 Z9=1:FOR Z=3 TO 15:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
9100 N$=""
9105 open "tm" recl 128 as 2
     read #2,z1;a,b,n$,y1,y2
     close 2
9120 Y4=Y4+Y2
9176 H4=H4+H1:H5=H5+H2:H6=H6+H3:S4=S4+S1:S5=S5+S2:S6=S6+S3:M4=M4+M1
9178 M5=M5+M2:M6=M6+M3:T0=T0+E0:T1=T1+E1:T2=T2+E2:T3=T3+E3:T4=T4+E4
9180 T5=T5+E5:T6=T6+E6:T7=T7+E7:T8=T8+E8:F4=F4+F1:F5=F5+F2:F6=F6+F3
9200 IF E2-E0>C5 THEN A4=0
9205 IF E2-E0>C5 THEN GOTO 9225
9210 IF E2>C5 THEN A4=((E2-C5)*C6)
9215 IF E2>C5 THEN GOTO 9225
9220 IF E2<=C5 THEN A4=E0*C6
9225 A6=A6+A4
9230 IF E2-E0>C7 THEN A5=0
9235 IF E2-E0>C7 THEN GOTO 9255
9240 IF E2>C7 THEN A5=((E2-C7)*C8)
9245 IF E2>C7 THEN GOTO 9255
9250 IF E2<=C7 THEN A5=E0*C8
9255 A7=A7+A5
9260 print tab(11);k$(3,3);tab(41);     k$(3,10);tab(66);h1;tab(76);h2;tab(88);h3;tab(101);r
9280 print using fmt$;left$(d$(1),5),e0,e3,f1,s1,e6,m1,e0-e3-f1-s1-e6-m1,     y2,a4,a5
     print using fmt$;"Q-T-D",e1,e4,f2,s2,e7,m2,e1-e4-f2-s2-e7-m2
     print using fmt$;"Y-T-D",e2,e5,f3,s3,e8,m3,e2-e5-f3-s3-e8-m3
9335 print:print
9340 H2=0:H3=0:S2=0:S3=0:M2=0:M3=0:E1=0:E2=0:E4=0:E5=0:E7=0:E8=0
9345 F2=0:F3=0:Y1=0:Y2=0:A4=0:A5=0
9395 RETURN
9400 print
9410 print A$
9420 print "Report continues on next page"
9430 print A$
9440 print chr$(12)
9450 RETURN
9600 REM PRINT PAGE HEADING SUBROUTINE
9610 print c5$
9615 print:P9=P9+1
9616 print TAB((T%(1)-LEN(Q$))/2);Q$:P9=P9+1
9617 print:P9=P9+1
9625 print K$(0,2);TAB(101);"FOR PERIOD ENDING ";D$(2)
9630 print B$
9632 IF Z=1 THEN 9645
9635 print "EMP #";TAB(11);"NAME";TAB(41);"SOC. SEC #";TAB(65);"HOURS";
9640 print TAB(76);"HOURS QTD";TAB(88);"HOURS YTD";TAB(101);"RATE"
9645 print "DATE";TAB(11);"GROSS";TAB(23);"F/W";TAB(34);"FICA";TAB(44);
9650 print "SDI";TAB(54);"S/W";TAB(65);"MISC DED";TAB(76);"NET PAY";
9655 print TAB(88);"OTHER";TAB(100);"FUTA";TAB(112);"SUTA"
9665 print B$
9670 RETURN
9800 q$="":Q$="P A Y R O L L   R E G I S T E R   S U M M A R Y"
9802 Z=1
9805 GOSUB 9600
9810 print using fmt$;left$(d$(1),5),t0,t3,f4,s4,t6,m4,t0-t3-f4-s4-t6-m4,     y4,a6,a7
     print using fmt$;"Q-T-D",t1,t4,f5,s5,t7,m5,t1-t4-f5-s5-t7-m5
     print using fmt$;"Y-T-D",t2,t5,f6,s6,t8,m6,t2-t4-f6-s6-t8-m6
9862 print
9864 IF T4+(F5*2)>200 THEN print "Q-T-D FED TAX LIABILITY IS ";
     if t4+(f5*2)>200 then print using v$;t4+(f5*2)
9866 IF T4+(F5*2)>200 THEN print "YOU SHOULD MAKE A DEPOSIT WITHIN 15 DAYS."
9868 IF T4+(F5*2)>2000 THEN print "*** ALERT *** Q-T-D FED TAX LIABILITY IS ";
     if t4+(f5*2)>2000 then print using v$;t4+(f5*2)
9872 if t4+(f5*2)>2000 then print     "You should make a deposit within 3 banking days!!"
9873 print
9874 print "CURRENT FEDERAL TAX LIABILITY IS ";:print using v$;t3+(f4*2)
9876 print "PLEASE ENTER THIS AMOUNT IN FEDERAL DEPOSIT FILE."
9882 print 
9884 print :print :print 
9886 print A$
9888 print "End of Report"
9890 print A$
9892 print chr$(12)
9895 RETURN
9900 print chr$(12)
9910 close 1
9915 console
10000 chain "master5"
