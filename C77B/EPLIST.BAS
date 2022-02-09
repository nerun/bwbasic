1     rem This is the Payroll File Listing Program

%INCLUDE ALL.BAS
500     dim k$(4,20),n(2,20)
     l$="$$##,###.##":u$="##########":v$="####"
     z5$="ep":z6$=z5$+"back"
     RESTORE
     FOR Z=1 TO 17:READ N(1,Z):NEXT Z
     DATA 8,10,24,24,24,17,2,5,14,11,8,8,1,1,1,4,6
1010 PRINT clear$:PRINT
1020 PRINT "*** LISTING ***"
9001 lprinter
9002 open z5$ recl 512 as 1
9003 FOR Z1=1 TO Z2-1
     read #1,z1;N(2,1),N(2,2),N$,N(2,16),N(2,17),N(2,18),N(2,19),N(2,20),          N(0,1),N(0,2),N(0,3),N(0,4),N(0,5),N(0,6),N(0,7),          N(0,8),N(0,9),N(0,10),N(0,11),N(0,12),N(0,13),          N(0,14),N(0,15),N(0,16),N(0,17),N(0,18)
9006 Z9=1
9007 FOR Z=3 TO 15:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
     print delim$:print:print
9010 PRINT "RECORD NUMBER";z1;TAB(31);
9012 PRINT "EMPLOYEE #";TAB(46);:print using u$;n(2,1)
9014 PRINT "ACCT NUMBER";TAB(18);:print using u$;n(2,2);
9016 PRINT TAB(31);"NAME";TAB(46);K$(3,3)
9018 PRINT TAB(31);"COMPANY";TAB(46);K$(3,4)
9020 PRINT TAB(31);"ADDRESS";TAB(46);K$(3,5)
9022 PRINT TAB(31);"CITY";TAB(46);K$(3,6)
9024 PRINT TAB(31);"STATE";TAB(46);K$(3,7);TAB(51);"ZIP";
9026 PRINT TAB(61);K$(3,8)
9028 PRINT "PHONE";TAB(16);K$(3,9);TAB(31);"SOC SEC #";
9030 PRINT TAB(46);K$(3,10)
9032 PRINT "EMPLOYED";TAB(16);K$(3,11);TAB(31);"TERMINATED";
9034 PRINT TAB(46);K$(3,12)
9036 PRINT "MARITAL";TAB(16);K$(3,13);TAB(31);"HOUR OR SA";
9038 PRINT TAB(46);K$(3,14)
9040 PRINT "PAY PERIOD";TAB(16);K$(3,15);TAB(31);"# OF EXEM";
9042 PRINT TAB(46);:print using v$;n(2,16);:PRINT TAB(51);"RATE";
     print using l$;n(2,17)
     print delim$
9046 PRINT TAB(16);"CURRENT";TAB(36);"QUARTER TO DATE";TAB(56);"YEAR TO DATE"
     print "Hours";tab(16);
     print using u$;n(2,18);tab(36);n(2,19);tab(56);n(2,20)
     print "Gross Pay";tab(16);
     print using l$;n(0,1);tab(36);n(0,2);tab(56);n(0,3)
     print "FICA";tab(16);
     print using l$;n(0,4);tab(36);n(0,5);tab(56);n(0,6)
     print "Fed Tax";tab(16);
     print using l$;n(0,7);tab(36);n(0,8);tab(56);n(0,9)
     print "State Tax";tab(16);
     print using l$;n(0,10);tab(36);n(0,11);tab(56);n(0,12)
     print "SDI";tab(16);
     print using l$;n(0,13);tab(36);n(0,14);tab(56);n(0,15)
     print "Misc Ded";tab(16);
     print using l$;n(0,16);tab(36);n(0,17);tab(56);n(0,18)
9100 print chr$(12)
9105 NEXT Z1
9110 close 1
9115 console
9120 CHAIN "epentry"
