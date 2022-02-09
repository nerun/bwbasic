1     rem This is the Raw Goods Inventory Alert Printer

%INCLUDE ALL.BAS
500     RESTORE
1005 DIM N(2,20)
     dim k$(3,20)
     z5$="rg"
     fmt1$="$$#,###,###.##"
     fmt2$="#,###,###"
     for z=1 to 130:repeat1$=repeat1$+"-":repeat2$=repeat2$+"="
     repeat3$=repeat3$+"*":next z
1020 DIM H(9),S(9),T(4,9)
1025 PRINT clear$:PRINT
1050 Q$="R A W  G O O D S  (I N V E N T O R Y)  A L E R T"
1065 L$="###,###.##"
1070 U$="#,###.#"
     for z=3 to 9:read n(1,z):next z
     data 7,12,24,24,8,8,10
1100 REM GET DATES ROUTINE
1145 Z9=0
1200 REM READ TAB VALUES ROUTINE
	JRF%=T%(1)
1235 T%(1)=130
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
6000 REM PRINT RAW GOODS ALERT
6005 PRINT clear$:PRINT:PRINT "PRINTING RAW GOODS ALERT":P0=1
	P9=0
6010 lprinter
	IF END#1 THEN 9915
     open z5$ recl 256 as 1
	IF END#1 THEN 6060
	N(2,2)=10
6020 GOSUB 9600
6030 GOSUB 9100
6040 IF P9>56 THEN GOSUB 9300
6050 IF N(2,2)<>0 THEN 6030
6060 GOSUB 9800
6070 GOTO 9900
9100 N$=""
9105 A9=A9+1
     read #1,a9;n(2,1),n(2,2),n$,f0,f1,f2,f3,f4,f5,f6,f7,f8,f9
9115 Z9=1:FOR Z=3 TO 9:K$(3,Z)=MID$(N$,Z9,N(1,Z)):Z9=Z9+N(1,Z):NEXT Z
9120 IF N(2,2)=0 THEN 9295
9125 IF F3>F9 THEN 9150
9130 IF F3<F8 THEN 9150
9140 GOTO 9295
9150 print A9;TAB(11);K$(3,9);TAB(26);F0
9152 print N(2,1);TAB(6);N(2,2);TAB(18);K$(3,3);TAB(27);
9155 print K$(3,4);TAB(41);K$(3,5);TAB(67);K$(3,6);TAB(93);K$(3,7);
9160 print TAB(105);K$(3,8)
9165 print:P9=P9+3
9170 IF F3>F9 THEN print "QUANTITY OVER MAXIMUM BY ";F3-F9;
9175 IF F3>F9 THEN print TAB(61);"QUANTITY ON ORDER ";F4
9180 IF F3<F8 THEN print "QUANTITY UNDER MINIMUM BY ";F8-F3;
9185 IF F3<F8 THEN print TAB(61);"QUANTITY ON ORDER ";F4
9200 print repeat1$
9205 print:P9=P9+3
9295 RETURN
9300 P9=0
	IF N(2,2)=0 THEN 9350
9305 print
     print repeat2$
     print "alert continues on next page"
     print repeat2$
9340 print chr$(12)
	GOSUB 9600
9350 RETURN
9600 REM PRINT PAGE HEADING SUBROUTINE
9605 IF N(2,2)=0 THEN 9795
9610 print repeat3$:P9=P9+1
9615 print:P9=P9+1
9616 print TAB((T%(1)-LEN(Q$))/2);Q$:P9=P9+1
9617 print:P9=P9+1
9620 FOR Z=2 TO 4
9625 print TAB((T%(1)-LEN(K$(0,Z)))/2);K$(0,Z):P9=P9+1
9630 NEXT Z
9635 print:P9=P9+1
9640 print TAB(T%(10));"FOR PERIOD ENDING ";D$(2);TAB(T%(11));"PAGE #";P0
9642 P9=P9+1
9645 print repeat2$:P9=P9+1
9650 print "RECORD #";TAB(11);"VEND CODE";TAB(26);"VEND TAG #"
9652 print "TAG";TAB(6);"ACCT #";TAB(18);
9655 print "ITEM #";TAB(27);"PART #";TAB(41);"DESCRIPTION";TAB(67);
9660 print "LOCATION";TAB(93);"LAST ISSUE";TAB(105);"LAST RECEIPT"
9670 print:P9=P9+3
9710 print repeat2$:P9=P9+2
	P0=P0+1
9795 RETURN
9800 print repeat2$
     print "end of alert"
     print repeat3$
9895 RETURN
9900	PRINT CHR$(12)
9910 close 1
9915 console
9920 	T%(1)=JRF%
	CHAIN "master4"
