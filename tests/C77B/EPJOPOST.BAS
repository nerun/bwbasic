1     rem This is the Payroll Journal Posting Program

%INCLUDE ALL.BAS
500     dim n(2,20)
     L$="$#,###,###.##"
     RESTORE
1040 PRINT clear$
1050 PRINT 
1060 PRINT  "THIS IS THE PAYROLL REGISTER POSTING PROGRAM."
1070 PRINT 
     print "Be sure the employee payroll disk is installed in drive B"
     PRINT:INPUT "Then type return to continue.";LINE temp$
     INITIALIZE
1145 Z9=0
1500 open "epsize" as 1
 read #1;z2,z3,flag
 close 1
     z2=z2-1
1600 PRINT clear$:PRINT
2000 PRINT  "RECORDS TO BE POSTED = ",Z2
2010 PRINT 
2015 INPUT "TO DO POSTING, TYPE CARRIAGE RETURN. ";line temp$
2220 open "ep" recl 512 as 1
2230 FOR Z=1 TO Z2
2240 read #1,z;N(2,1),N(2,2),N$,N,R,H1,H2,H3,     E0,E1,E2,F1,F2,F3,E3,E4,E5,E6,E7,E8,S1,S2,S3,M1,M2,M3
2260 H2=H2+H1:H3=H3+H1:S2=S2+S1:S3=S3+S1:M2=M2+M1:M3=M3+M1
2270 E1=E1+E0:E2=E2+E0:E4=E4+E3:E5=E5+E3:E7=E7+E6:E8=E8+E6
2280 F2=F2+F1:F3=F3+F1
2290 S4=S4+S1:M4=M4+M1:T0=T0+E0:T3=T3+E3:T6=T6+E6:F4=F4+F1
2300 H1=0:S1=0:M1=0:E0=0:E3=0:E6=0:F1=0
2310 print #1,z;N(2,1),N(2,2),N$,N,R,H1,H2,H3,     E0,E1,E2,F1,F2,F3,E3,E4,E5,E6,E7,E8,S1,S2,S3,M1,M2,M3
2330 NEXT Z
2340 close 1
2350 P4=T0-S4-M4-T3-T6-F4
2360 PRINT clear$
2370 PRINT "INSTALL GENERAL LEDGER (GL) DISK IN DRIVE B."
2380 PRINT 
2390 INPUT "THEN TYPE CARRIAGE RETURN TO CONTINUE. ";line temp$
     initialize
3010 PRINT clear$:print
3030 open "gl" recl 138 as 1
3040 INPUT "ENTER NET SALARIES ACCOUNT RECORD # - ";Z
3050 read #1,z; N(2,1),N(2,2),G$,G1,G2,G3,G4,G5,G6,G7
3060 PRINT 
3070 PRINT "IS ";N(2,2);" ";G$;" THE CORRECT ACCOUNT?"
3080 PRINT :INPUT "(Y OR N) ";line temp$
3090 if left$(temp$,1)="n" or left$(temp$,1)="N" then 3040
3500 if left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" then 3060
3510 G1=G1+P4:G2=G2+P4:G3=G3+P4
3520 print #1,z; n(2,1),n(2,2),g$,g1,g2,g3,g4,g5,g6,g7
     LPRINTER

 for z=1 to 72:delim$=delim$+"*":delim1$=delim1$+"=":next z
  print delim$:print
 print "Payroll posting to General Ledger for period ending ";d$(2)
  print:print delim1$:print

3530 print 
3540 print "(X)";tab(5);"TOTAL NET SALARIES POSTED";tab(62);
     print using l$;P4
     CONSOLE
3550 PRINT 
3560 INPUT "ENTER STATE DISABILITY INS PAYABLE ACCOUNT RECORD # - ";Z
3570 read #1,z; N(2,1),N(2,2),G$,G1,G2,G3,G4,G5,G6,G7
3580 PRINT 
3590 PRINT "IS ";N(2,2);" ";G$;" THE CORRECT ACCOUNT?"
3600 PRINT :INPUT "(Y OR N) ";line temp$
3610 if left$(temp$,1)="n" or left$(temp$,1)="N" then 3560
3620 if left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" then 3580
3630 G1=G1+S4:G2=G2+S4:G3=G3+S4
3640 print #1,z; n(2,1),n(2,2),g$,g1,g2,g3,g4,g5,g6,g7
     LPRINTER
3650 print 
3660 print "(X)";tab(5);"TOTAL STATE DIS INS PAYABLE POSTED";tab(62);
     print using l$;s4
     CONSOLE
3670 PRINT 
3680 INPUT "ENTER MISC DEDUCTIONS PAYABLE ACCOUNT RECORD # - ";Z
3690 read #1,z; N(2,1),N(2,2),G$,G1,G2,G3,G4,G5,G6,G7
3700 PRINT 
3710 PRINT "IS ";N(2,2);" ";G$;" THE CORRECT ACCOUNT?"
3720 PRINT :INPUT "(Y OR N) ";line temp$
3730 if left$(temp$,1)="n" or left$(temp$,1)="N" then 3680
3740 if left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" then 3700
3750 G1=G1+M4:G2=G2+M4:G3=G3+M4
3760 print #1,z; n(2,1),n(2,2),g$,g1,g2,g3,g4,g5,g6,g7
     LPRINTER
3770 print 
3780 print "(X)";tab(5);"TOTAL MISC DED PAYABLE POSTED";tab(62);
     print using l$;m4
     CONSOLE
3790 PRINT 
3800 INPUT "ENTER FICA PAYABLE ACCOUNT RECORD # - ";Z
3810 read #1,z; N(2,1),N(2,2),G$,G1,G2,G3,G4,G5,G6,G7
3820 PRINT 
3830 PRINT "IS ";N(2,2);" ";G$;" THE CORRECT ACCOUNT?"
3840 PRINT :INPUT "(Y OR N) ";line temp$
3850 if left$(temp$,1)="n" or left$(temp$,1)="N" then 3800
3860 if left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" then 3820
3870 G1=G1+(F4*2):G2=G2+(F4*2):G3=G3+(F4*2)
3880 print #1,z; n(2,1),n(2,2),g$,g1,g2,g3,g4,g5,g6,g7
     LPRINTER
3890 print 
3900 print "(X)";tab(5);"TOTAL FICA PAYABLE POSTED";tab(62);
     print using l$;f4*2
     CONSOLE
3910 PRINT 
3920 INPUT "ENTER FEDERAL TAX PAYABLE ACCOUNT RECORD # - ";Z
3930 read #1,z; N(2,1),N(2,2),G$,G1,G2,G3,G4,G5,G6,G7
3940 PRINT 
3950 PRINT "IS ";N(2,2);" ";G$;" THE CORRECT ACCOUNT?"
3960 PRINT :INPUT "(Y OR N) ";line temp$
3970 if left$(temp$,1)="n" or left$(temp$,1)="N" then 3920
3980 if left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" then 3940
3990 G1=G1+T3:G2=G2+T3:G3=G3+T3
4000 print #1,z; n(2,1),n(2,2),g$,g1,g2,g3,g4,g5,g6,g7
     LPRINTER
4010 print 
4020 print "(X)";tab(5);"TOTAL FED TAX PAYABLE POSTED";tab(62);
     print using l$;t3
     CONSOLE
4030 PRINT 
4040 INPUT "ENTER STATE TAX PAYABLE ACCOUNT RECORD # - ";Z
4050 read #1,z; N(2,1),N(2,2),G$,G1,G2,G3,G4,G5,G6,G7
4060 PRINT 
4070 PRINT "IS ";N(2,2);" ";G$;" THE CORRECT ACCOUNT?"
4080 PRINT :INPUT "(Y OR N) ";line temp$
4090 if left$(temp$,1)="n" or left$(temp$,1)="N" then 4040
4100 if left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" then 4060
4110 G1=G1+T6:G2=G2+T6:G3=G3+T6
4120 print #1,z; n(2,1),n(2,2),g$,g1,g2,g3,g4,g5,g6,g7
     LPRINTER
4130 print 
4140 print "(X)";tab(5);"TOTAL STATE TAX PAYABLE POSTED";tab(62);
     print using l$;t6
4150 print 
4160 print TAB(62);"----------------"
4170 print "TOTAL NET SALARIES AND DEDUCTIONS POSTED";
4180 print TAB(62);:print using l$;P4+S4+M4+F4+T3+T6
4190 print 
4200 print TAB(62);"================"
4210 print 
4220 print "GROSS PAY TOTAL";tab(62);:print using l$;t0
4230 print TAB(62);"================"
4240 print chr$(12):for z=1 to 100:next z:print chr$(12)
4250 close 1
4260 console
 print "Replace payroll file disk in drive B."
 Input "Type RETURN to continue - ";line temp$
 initialize
4290 PRINT clear$
4300 chain "master5"
