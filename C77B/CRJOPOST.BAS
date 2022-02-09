1     rem This is the Accounts Receivable Posting Program

%INCLUDE ALL.BAS
500     RESTORE
     dim n(2,20),k$(2,20)
     l$="$$#,###,###.##"

 before1$="Before POSTING                "
 after1$="After POSTING                 "
 numbers1$="##,###,###.##   ##,###,###.##   ##,###,###.##"
 before$=before1$+numbers1$:after$=after1$+numbers1$

 temp$="Totals for Acct ########## /...5...10...15...20...25../"
 zero$=temp$+"               #.##"
 credit$=temp$+"   ##,###,###.## cr"
 debit$=temp$+"   ##,###,###.## db"
 temp$=""

     print clear$:print
1002 PRINT "BEFORE PROCEEDING, IT IS A GOOD PRACTICE TO BACK UP"
1005 PRINT "BOTH THE GENERAL LEDGER AND AR FILE"
1010 PRINT "BY MAKING COPIES USING THE DISKCOPY FACILITY OF YOUR DOS."
1015 PRINT:PRINT"IF YOU ARE UNFAMILIAR WITH THE PROCESS, ASK YOUR"
1020 PRINT "SYSTEM SUPERVISOR FOR HELP.":PRINT
1025 PRINT "IF THIS HAS BEEN DONE; TYPE Y TO CONTINUE!"
1030 INPUT line temp$
1035 IF left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" THEN CHAIN "master2"
     print clear$:print
1545 PRINT "INSTALL THE GENERAL LEDGER DISK IN DRIVE A":PRINT
1550 PRINT "INSTALL THE CUSTOMER DISK WITH AR FILE IN DRIVE B":PRINT
1555 PRINT "TYPE RETURN WHEN READY":INPUT line temp$
     initialize

 rem get  dates
 z9=0
     print clear$:print
2005 PRINT "LOADING ACCOUNT NUMBER TABLES":PRINT
2010 PRINT "*** PLEASE WAIT ***"
     open "glref" as 1
     read #1;z2
     dim h(z2)
     for z=1 to z2
     read #1;h(z)
     next z
     close 1
3070 N(2,1)=8:N(2,2)=24:N(2,3)=24:N(2,4)=1
4000 REM THIS IS THE ACTUAL POSTING PROGRAM
     open "ar" as 1
     read #1;s1,s2,t2,t4,t5
 close 1
     s3=0
     open "ar" as 1
     print #1;s3,s3,s3,s3,s3
     close 1
     open "gl" recl 138 as 1
4100 print clear$:print
4105 PRINT "THIS POSTING PROGRAM POSTS: ":PRINT
4110 PRINT "PREPAID SALES TO APPROPRIATE SALES ACCOUNT (BANK IS POSTED"
4115 PRINT "THROUGH CHECK RECEIPTS ROUTINE)."
4120 PRINT "SALES ON TERMS TO APPROPRIATE SALES ACCOUNT AND A/R"
4125 PRINT "COST OF GOODS TO INVENTORY"
4130 PRINT "SHIPPING CHARGES TO EXPENSES"
4135 PRINT "SALES TAX TO EXPENSES AND SALES TAX PAYABLE"
4140 PRINT:INPUT "TYPE RETURN TO CONTINUE ";line temp$
 lprinter
 print "ACCOUNTS RECEIVABLE AUDIT TRAIL RECORD FOR ";D$(1)
 print "=======================================================":print
 console
4150 print clear$:print
4200 PRINT "ENTER ACCOUNT NUMBER FOR PREPAID SALES ":INPUT N(0,1):PRINT
4205 GOSUB 20000
4210 IF A=1 THEN 4200
4220 N(0,2)=S1*(-1)
4225 GOSUB 9000
4300 PRINT "ENTER ACCOUNT NUMBER FOR ACCOUNTS RECEIVABLE ":INPUT N(0,1):PRINT
4305 GOSUB 20000
4310 IF A=1 THEN 4300
4320 N(0,2)=S2
4325 GOSUB 9000
4400 PRINT "ENTER ACCOUNT NUMBER FOR SALES ON TERMS ":INPUT N(0,1):PRINT
4405 GOSUB 20000
4410 IF A=1 THEN 4400
4420 N(0,2)=S2*(-1)
4425 GOSUB 9000
4600 PRINT "ENTER ACCOUNT NUMBER FOR INVENTORY ":INPUT N(0,1):PRINT
4605 GOSUB 20000
4610 IF A=1 THEN 4600
4620 N(0,2)=T2*(-1)
4625 GOSUB 9000
4700 PRINT "ENTER ACCOUNT NUMBER FOR SHIPPING EXPENSE ":INPUT N(0,1):PRINT
4705 GOSUB 20000
4710 IF A=1 THEN 4700
4720 N(0,2)=T4
4725 GOSUB 9000
4900 PRINT "ENTER ACCOUNT NUMBER FOR SALE TAX EXPENSE ":INPUT N(0,1):PRINT
4905 GOSUB 20000
4910 IF A=1 THEN 4900
4920 N(0,2)=T5
4925 GOSUB 9000
5000 PRINT "ENTER ACCOUNT NUMBER FOR SALES TAX PAYABLE ":INPUT N(0,1):PRINT
5005 GOSUB 20000
5010 IF A=1 THEN 5000
5015 A=0
5020 N(0,2)=T5*(-1)
5025 GOSUB 9000
6000 GOTO 10000
9000 REM SUBROUTINE TO POST AND PRINT TOTALS
9025 FOR Z4=1 TO Z2
9030 IF N(0,1)=H(Z4) THEN read #1,z4;N(1,1),N(1,2),R$,N(1,4),N(1,5),          N(1,6),N(1,7),N(1,8),N(1,9),N(1,10):GOTO 9040
9035 NEXT Z4
9040 lprinter
 print
 if n(0,2)=0 then print using zero$;n(1,2),r$,n(0,2)
 if n(0,2)<0 then print using credit$;n(1,2),r$,n(0,2)
 if n(0,2)>0 then print using debit$;n(1,2),r$,n(0,2)
 print using before$;n(1,4),n(1,5),n(1,6)
 FOR X=4 TO 6:N(1,X)=N(1,X)+N(0,2):NEXT X
 print #1,z4;N(1,1),N(1,2),R$,N(1,4),N(1,5),N(1,6),N(1,7), N(1,8),N(1,9),N(1,10)
 print using after$;n(1,4),n(1,5),n(1,6)
 console
9235 RETURN
10000 close 1
10005 PRINT "REPLACE DISK #3 IN DRIVE A - TYPE RETURN TO CONTINUE"
10010 INPUT line temp$
 lprinter
10015 print chr$(12):for z=1 to 100:next z:print chr$(12):console
 console
10020 chain "master2"
20000 print clear$:print
20005 A=0
20010 PRINT "*** VALIDATING ACCOUNT NUMBER ***"
20020 PRINT:PRINT "PLEASE WAIT"
20080 FOR Z4=1 TO Z2
20085 IF N(0,1)=0 THEN 20510
20090 IF N(0,1)=H(Z4) THEN print clear$:print:GOTO 21000
20100 NEXT Z4
20500 print N(0,1);" IS NOT A VALID ACCOUNT NUMBER"
20510 A=1
21000 RETURN
