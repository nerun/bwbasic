1     rem This is the Formatted Invoice Printing Program

%INCLUDE ALL.BAS
500     RESTORE
     dim n(5,20),k$(5,20),h(9),s(9),t(4,9),g(10),q(10)
     for z=4 to 11:read n(0,z):next z
     for z=3 to 17:read n(1,z):next z
     for z=3 to 9:read n(2,z):next z
     data 14,10,8,8,10,10,1,4
     data 24,24,24,17,2,5,14,24,24,24,17,2,5,14,8
     data 7,12,24,24,8,8,10
     z5$="ir":z6$="cr":z7$="fg"
     for z=1 to 80:del$=del$+"*":del1$=del1$+"=":del2$=del2$+"-":next z
fmt1$ ="           /...5...10...15...20.../"
fmt2$ ="           /...5...10...15...20.../"
fmt3$ ="           /...5...10...15...20.../           Invoice No.  ####"
fmt4$ ="                                              Invoice Date Salesman"
fmt5$ ="                                              /....../     /........./"

fmta$ ="      SOLD TO                           SHIP TO"

fmt6$ ="      /...5...10...15...20.../          /...5...10...15...20.../"
fmt7$ ="      /....................../          /....................../"
fmt8$ ="      /....................../          /....................../"
fmt9$ ="      /.............../, //  /.../      /.............../, //  /.../"
fmt10$="   Your Ord No. Shipped   Via           FOB Point      Terms"
fmt11$="   /........../ /....../  /........../  /......./      /.........../"
fmt12$="   Quan   Quan B.O.   Description            Unit                    "
fmt12$=fmt12$+"Total"
fmt13$="   ###### ######      /...5...10...15...2/   $$#,###.##              "
fmt13$=fmt13$+"$$##,###.##"
     u$="$$##,###.##"
     print clear$:print
1080 Q$="I N V O I C E   R E G I S T E R"
1145 Z9=0
1200 REM READ TAB VALUES ROUTINE
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
6000 REM PRINT INVOICES
6005 PRINT clear$:PRINT
6010 PRINT "PRINTING INVOICES"
6015 FOR Z=0 TO 9
6020 H(Z)=0:S(Z)=0:T(0,Z)=0:T(1,Z)=0:T(2,Z)=0:T(3,Z)=0:A(Z)=0
6025 NEXT Z
6030 T1=0:T2=0:T3=0:T4=0:T5=0:T6=0:P0=1
6035 X=1
6040 PRINT:PRINT "INSTALL INVOICE REGISTER DISK IN DRIVE A"
6045 PRINT:PRINT "INSTALL A CUSTOMER DISK IN DRIVE B":PRINT
6046 PRINT chr$(07)
6050 INPUT "TYPE RETURN TO CONTINUE ";line temp$
     initialize
6100 p9=0
6200 open z5$ recl 384 as 1
6205 z3=(size(z5$)*block.size)/384
6210 FOR Z2=1 TO Z3
     read #1,z2;z5,z6,n$
6220 IF Z6=0 THEN 6230
6225 NEXT Z2
6230 close 1
6235 Z2=Z2-1
6240 PRINT clear$:PRINT
6245 PRINT:PRINT "INVOICES TO BE PRINTED ";Z2
6246 PRINT chr$(07)
6250 PRINT:INPUT "TO PRINT INVOICES, TYPE RETURN.";line temp$
 lprinter
6300 open z5$ recl 384 as 1
6310 open z6$ recl 384 as 2
6320 FOR I=1 TO Z2
6350 GOSUB 15000
6380 NEXT I
6410 GOTO 20000
15000 IF I>Z2 THEN 15595
      read #1,i;N(3,1),N(3,2),N(3,3),N$,A1,D4,           G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),           G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
15060 Z9=1:FOR Z=4 TO 11:K$(3,Z)=MID$(N$,Z9,N(0,Z)):Z9=Z9+N(0,Z):NEXT Z
15065 if mid$(k$(3,11),2,1)="I" then 15595
15070 z$="":z$=left$(k$(3,11),1)+"I"+right$(k$(3,11),2):k$(3,11)=z$
15105 read #2,a1;n(4,1),n(4,2),o$,c1,c2,c3,c4,c5,c6
15115 close 2
15120 Z9=1:FOR Z=3 TO 17:K$(4,Z)=MID$(O$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
15200 print using fmt1$;k$(0,2)
      print using fmt2$;k$(0,3)
      print using fmt3$;k$(0,4),n(3,3)
      print fmt4$
      print using fmt5$;d$(1),k$(3,4)
 print:print fmta$:print
      print using fmt6$;k$(4,3),k$(4,10)
      print using fmt7$;k$(4,4),k$(4,11)
      print using fmt8$;k$(4,5),k$(4,12)
      print using fmt9$;k$(4,6),k$(4,7),k$(4,8),k$(4,13),k$(4,14),k$(4,15)
15240 FOR Z=1 TO 2:print:NEXT Z:print del1$
15270 IF VAL(K$(3,9))=1 THEN terms$= "PREPAID"
15275 IF VAL(K$(3,9))=2 THEN terms$= "COD"
15280 IF VAL(K$(3,9))=3 THEN terms$= "1% 10, NET 30"
15285 Z$=K$(3,9)
15286 IF VAL(Z$)<>1 AND VAL(Z$)<>2 AND VAL(Z$)<>3 THEN terms$= K$(3,9)
      print fmt10$
      print using fmt11$;k$(3,5),k$(3,6),k$(3,7),k$(3,8),terms$
15290 print del1$
      print fmt12$
15310 print del2$
15315 print
 console
15350 PRINT clear$:PRINT
15352 PRINT "INSTALL THE APPROPRIATE FINISHED GOODS DISK IN DRIVE B"
15353 PRINT chr$(07)
15354 PRINT:INPUT "TYPE RETURN TO CONTINUE ";line temp$
: lprinter
:      initialize
:      open z7$ recl 256 as 3
:      if end #3 then 15570
15358 FOR J=1 TO 10
15360 IF G(J)=0 THEN 15500
15365 read #3,g(j);Z,Z,F$,F0,F1,F2,F3,F4,F5,F6,F7,F8,F9
15370 IF Q(J)<=F3 THEN quan=q(j):F3=F3-q(j)
      if q(j)>f3 then quan=f3:b.o.=q(j)-f3:F3=0
15380 Z9=1:FOR Z=3 TO 9:K$(5,Z)=MID$(F$,Z9,N(2,Z)):Z9=Z9+N(2,Z):NEXT Z
      print using fmt13$;quan,b.o.,k$(5,5),f2,f2*q(j)
 print #3,g(j);Z,Z,F$,F0,F1,F2,F3,F4,F5,F6,F7,F8,F9
     quan=0:b.o.=0
15395 D3=D3+(Q(J)*F2):T2=T2+(Q(J)*F1)
15400 IF ucase$(k$(3,10))<>"T" AND ucase$(k$(3,10))<>"Y" then 15410
15405 D5=D5+(D3*.06)
15410 IF VAL(K$(3,9))=1 THEN S1=S1+D3:GOTO 15500
15415 S2=S2+D3
15500 NEXT J
15505 T4=T4+D4:T5=T5+D5
15510 print TAB(70);"-----------"
15512 print:print TAB(58);"TOTAL";TAB(70);:print using u$;d3
15514 print:print TAB(58);"SALES TAX";TAB(70);:print using u$;d5
15516 print:print TAB(58);"SHIPPING";TAB(70);:print using u$;d4
15518 print TAB(70);"==========="
15520 print TAB(70);:print using u$;d3+d4+d5
15540 N$="":FOR Z=4 TO 11:N$=N$+K$(3,Z):NEXT Z
15545 print #1,i;N(3,1),N(3,2),N(3,3),n$,A1,D4,           G(1),Q(1),G(2),Q(2),G(3),Q(3),G(4),Q(4),G(5),Q(5),           G(6),Q(6),G(7),Q(7),G(8),Q(8),G(9),Q(9),G(10),Q(10)
15550 IF VAL(K$(3,9))=1 THEN K$(4,17)=D$(1):C1=D3+D4+D5:C2=C2+D3+D4+D5
      if val(k$(3,9))=1 then 15560
15555 C3=C3+D3+D4+D5
15560 D3=0:D4=0:D5=0
15565 O$="":FOR Z=3 TO 17:O$=O$+K$(4,Z):NEXT Z
15570 close 3
 console
15575 PRINT clear$:PRINT:PRINT "INSTALL CUSTOMER DISK IN DRIVE B"
15576 PRINT chr$(07)
15580 PRINT:INPUT "TYPE RETURN TO CONTINUE ";line temp$
: lprinter
: print chr$(12)
:      initialize
15582 open z6$ recl 384 as 2
      print #2,a1;n(4,1),n(4,2),o$,c1,c2,c3,c4,c5,c6
15595 RETURN
20000 REM ROUTINE TO CLOSE FILES AND RETURN TO master2
20005 close 1
20010 close 2
 print chr$(12):for z=1 to 100:next z:print chr$(12)
20015 console
20020 PRINT clear$:PRINT
20025 PRINT "INSTALL PROGRAM DISK IN DRIVE A":PRINT
20026 PRINT chr$(07)
20030 INPUT "TYPE RETURN TO CONTINUE ";line temp$
      initialize
20050 CHAIN "master2"
