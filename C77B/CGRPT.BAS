1     rem This is the Depreciation Report Printer

%INCLUDE ALL.BAS
500     dim n(2,20),k$(3,20)
     z5$="cg"
     RESTORE
1008 DIM H(9),S(9),T(4,9)
1009 PRINT clear$:PRINT
1090 U$="##,###,###"
1095 FOR Z=1 TO 104:A$=A$+"-":NEXT Z
1100 REM GET DATES ROUTINE
1145 Z9=0
1200 REM READ TAB VALUES ROUTINE
3000 REM READ NAME FILE SUBROUTINE
3005 a5=T%(12)
     for z=3 to 11:read n(1,z):next z
     data 24,24,24,17,2,5,14,20,20
3085 Z9=1:FOR Z=3 TO 11:K$(0,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
3090 N$="":FOR Z=1 TO 20:N(1,Z)=0:NEXT Z
3125 REM ROUTINE TO ELIMINATE TRAILING BLANKS
     l8=1
3130 FOR I=3 TO 6
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
6000 REM PROGRAM TO PRINT DEPRECIATION REPORT
6005 PRINT clear$:PRINT
6010 PRINT "PRINTING DEPRECIATION REPORT"
6030 T1=0:T2=0:T3=0:T4=0:P0=1
6035 X=1
6040 for z=4 to 8:read n(1,z):next z
     data 24,24,1,8,2
6200 open z5$ recl 256 as 1
6205 z3=(size(z5$)*block.size)/256
6210 FOR Z2=1 TO Z3
6215 read #1,z2;n(2,1),n(2,2),n(2,3),n$
6220 IF N(2,2)=0 THEN 6230
6225 NEXT Z2
6230 close 1
6235 Z2=Z2-1
6240 PRINT:PRINT "RECORDS TO BE PRINTED ";Z2
6245 PRINT:INPUT "TO PRINT REPORT, TYPE RETURN.";line temp$
     lprinter:P9=0
6300 open z5$ recl 256 as 1
6310 FOR Z1=1 TO Z2
6320 read #1,z1;N(2,1),N(2,2),N(2,3),N$,D1,D2,D3,D4,D5,D6,D7,D8,D9
6330 Z9=1:FOR Z=4 TO 8:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
6340 IF RIGHT$(K$(3,7),2)=RIGHT$(D$(6),2) AND D3>D8 THEN T3=T3+D3-D8
6350 NEXT Z1
6400 print K$(0,4);TAB(46);K$(0,10);TAB(65);" ";TAB(88);"PAGE ";
6405 print P0;" OF ":P9=P9+1
6410 print "STATEMENT OF DEPRECIATION FOR YEAR ENDING";TAB(46);D$(6)
6415 print A$:P9=P9+1
6420 print "DESCRIPTION";TAB(17);"N-NEW";TAB(26);"N";TAB(29);"DATE";
6425 print TAB(42);"COST OR";TAB(52);"DEPRECIABLE";TAB(64);
6430 print "DEPRECIATION";TAB(77);"METHOD";TAB(85);"USEFUL";
6435 print TAB(96);"CURRENT":P9=P9+1
6450 print "OF PROPERTY";TAB(17);"U-USED";TAB(26);"U";TAB(29);"ACQUIRED";
6455 print TAB(39);"OTHER";TAB(46);"BASIS";TAB(55);"BASIS";TAB(64);
6460 print "PRIOR  YEARS";TAB(84);"LIFE YRS DEPRECIATION":P9=P9+1
6465 print A$:P9=P9+1
6470 T9=T3:T3=0
6500 FOR Z1=1 TO Z2
6510 read #1,z1;N(2,1),N(2,2),N(2,3),N$,D1,D2,D3,D4,D5,D6,D7,D8,D9
6520 Z9=1:FOR Z=4 TO 8:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
6525 print K$(3,4);TAB(26);K$(3,6);TAB(29);K$(3,7);TAB(39);:print using u$;d1;
6530 print TAB(52);:print using u$;d2;TAB(64);:print using u$;d3;
     print  TAB(79);K$(3,8);
6535 print TAB(86);:print using "####";d4;TAB(93);:print using u$;d8:P9=P9+1
6560 T1=T1+D1:T2=T2+D2:T3=T3+D3:T4=T4+D8
6565 IF P9>38 THEN GOSUB 30000
6570 IF P9>38 THEN console
     IF P9>38 THEN PRINT "INSERT ANOTHER SHEET AND ADVANCE TO FIRST DESC LINE"
6575 IF P9>38 THEN INPUT "TYPE RETURN TO CONTINUE ";line temp$
6576 IF P9>38 THEN lprinter: GOSUB 30500:P9=0
6580 NEXT Z1
6585 print:print TAB(10);"ADDITIONAL FIRST YEAR DEPRECIATION";TAB(93);
6586 print using u$;t9
6590 print:print:print tab(10);"Totals";tab(39);
     print using u$;t1;tab(52);t2;tab(64);t3;tab(93);t4+t9
20000 REM ROUTINE TO CLOSE FILES AND RETURN TO master16
      IF P9<=38 THEN PRINT CHR$(12)
20005 close 1
20040 console
20050 CHAIN "master16"
30000 print:print tab(10);"Subtotals";tab(39);
      print using u$;t1;tab(52);t2;tab(64);t3;tab(93);t4
30010 print chr$(12)
30015 RETURN
30500 print:print tab(10);"Subtotals";tab(39);
      print using u$;t1;tab(52);t2;tab(64);t3;tab(93);t4
30515 RETURN
