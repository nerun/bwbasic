1     rem This is the General Journal Posting Program

%INCLUDE ALL.BAS
500     dim n(2,20),k$(3,20)
     first=8:last=18

fa$="DATE    /...5../    /...5...10...15...20.../                    "
fa$=fa$+"      PAGE  ####"
fb$="                    *** GENERAL JOURNAL ***"
fb1$="                    -----------------------"
fc$=" REF   #    DATE    DESCRIPTION                 ACCOUNT #"
fc$=fc$+"    AMOUNT      NET"
fd$="########  /...5../  /...5...10...15...20.../ ##########"
fd$=fd$+"  ###,###.##-"
fd1$="          /...5../  /...5...10...15...20.../ ##########"
fd1$=fd1$+"  ###,###.##-"
fd2$="########  /...5../  /...5...10...15...20.../           "
fd2$=fd2$+"  ###,###.##-"
fe$="                                             ##########"
fe$=fe$+"  ###,###.##-"
fe1$="                                             ##########"
fe1$=fe1$+"  ###,###.##- ###,###.##-"
ff$="                              /...5...10...1/"
ff$=ff$+"        ##,###,###.##-*"

fg$="DATE    /...5../    /...5...10...15...20.../                    "
fg$=fg$+"      PAGE  ####"
fh$="                    *** GENERAL LEDGER ***"
fh1$="                    ----------------------"
fi$=" ACCOUNT #  DESCRIPTION                       REF #    "
fi$=fi$+"BAL.FWRD.      CURRENT        BALANCE"
fj$="##########  /...5...10...15...20...25...3/         ##,###,###.##-"
fk$="           /.../  /...5...10...15...20.../ ########            "
fk$=fk$+" ##,###,###.##-"
fl$="                                             "
fl$=fl$+"                   ##,###,###.##-* ##,###,###.##-"

1000 PRINT clear$:PRINT
1002 PRINT "BEFORE PROCEEDING, IT IS A GOOD PRACTICE TO BACK UP"
1005 PRINT "BOTH THE GENERAL LEDGER FILE AND THE GENERAL JOURNAL FILE"
1010 PRINT "BY MAKING COPIES USING THE DISKCOPY FACILITY OF YOUR DOS."
1015 PRINT:PRINT"IF YOU ARE UNFAMILIAR WITH THE PROCESS, ASK YOUR"
1020 PRINT "SYSTEM SUPERVISOR FOR HELP.":PRINT
1025 PRINT "IF THIS HAS BEEN DONE; TYPE Y TO CONTINUE!"
1030 INPUT line temp$
1035 IF left$(temp$,1)<>"y" and left$(temp$,1)<>"Y" THEN CHAIN "master11"

1540 PRINT clear$:PRINT
1545 PRINT "PUT THE GENERAL LEDGER DISK IN DRIVE B":PRINT
1550 PRINT "PUT THE GENERAL JOURNAL DISK IN DRIVE A":PRINT
1555 PRINT "TYPE RETURN WHEN READY":INPUT line temp$
     initialize
2000 PRINT clear$:PRINT
2005 PRINT "LOADING ACCOUNT NUMBER TABLES":PRINT
2010 PRINT "*** PLEASE WAIT ***"
2020 open "glref" as 1
     read #1;z2
 zn%=z2+2
     dim h(zn%),ref%(zn%)
     for z=1 to z2
     read #1;h(z)
     next z
     close 1
3070 N(2,1)=8:N(2,2)=24:N(2,3)=24:N(2,4)=1
4000 REM THIS IS THE ACTUAL POSTING PROGRAM
4010 open "gl" recl 138 as 1
     open "gljo" recl 250 as 2
4015 P9=100:PAGE=0
4030 GOSUB 20000
4100 LPRINTER
4500 I=1
4505 read #2,i;N(0,1),N(0,2),Q$,N(0,7),N(0,8),N(0,9),     N(0,10),N(0,11),N(0,12),N(0,13),N(0,14),N(0,15),N(0,16),     N(0,17),N(0,18),N(0,19),FLAG$
4510 IF N(0,2)=0 THEN GOTO 10000 rem Get out when finished
4515 IF FLAG$="P" THEN 4540
4525 IF P9>60 THEN GOSUB 9000
4530 GOSUB 5000
4532 IF P9>60 THEN GOSUB 9000
4534 GOSUB 5100
4536 IF P9>60 THEN GOSUB 9000
4540 I=I+1:GOTO 4505
5000 REM SUBROUTINE TO PAST AND PRINT TOTALS
5002 IF N(0,2)=0 THEN 5050
5003 FOR Z4=1 TO Z2
5004 IF N(0,2)=H(Z4) THEN read #1,z4;n(1,1),n(1,2),r$,n(1,4),n(1,5),     n(1,6),n(1,7),n(1,8),n(1,9),n(1,10):goto 5006
5005 NEXT Z4
5006 FOR X=4 TO 6:N(1,X)=N(1,X)+N(0,7):NEXT X
5050 RETURN

5100 NU.LEFT%=0:NU.TRAN%=0:NET.AMT=0
 FOR B=FIRST TO LAST STEP 2
 IF N(0,B)<>0 THEN NU.TRAN%=NU.TRAN%+1: NET.AMT=NET.AMT+N(0,B+1)
 NEXT B
5110 IF N(0,7)=0 THEN PRINT USING FD2$;N(0,1),LEFT$(Q$,8), MID$(Q$,9,24),N(0,7):P9=P9+1:GOTO 5235
5115 FOR J=first to last STEP 2
5120 IF N(0,J)=0 THEN 5220
5125 FOR Z4=1 TO Z2
5130 IF N(0,j)=H(Z4) THEN read #1,z4;n(1,1),n(1,2),r$,n(1,4),n(1,5),     n(1,6),n(1,7),n(1,8),n(1,9),n(1,10):goto 5140
5135 NEXT Z4
5140 IF NU.TRAN%>1 THEN GOSUB 5400 ELSE GOTO 5142
 GOTO 5145
5142 PRINT USING FD$;N(0,1),LEFT$(Q$,8),MID$(Q$,9,24),N(0,J),N(0,J+1)
5145 P9=P9+1
5170 FOR X=4 TO 6:N(1,X)=N(1,X)+N(0,J+1):NEXT X
5220 NEXT J
5225 IF N(0,7)=0 THEN PRINT USING FD2$;N(0,1),LEFT$(Q$,8),      MID$(Q$,9,24),N(0,7):P9=P9+1
5235 RETURN
5400 IF NU.LEFT%=1 THEN PRINT USING FE1$;N(0,J),N(0,J+1),NET.AMT: NU.LEFT%=NU.LEFT%-1:RETURN
5420 IF NU.LEFT%>1 THEN PRINT USING FE$;N(0,J),N(0,J+1): NU.LEFT%=NU.LEFT%-1:RETURN
5430 PRINT USING FD$;N(0,1),LEFT$(Q$,8),MID$(Q$,9,24), N(0,J),N(0,J+1):NU.LEFT%=NU.TRAN%-1
5440 RETURN

9000 P9=0:PAGE=PAGE+1
9005 PRINT CHR$(12)
9010 PRINT USING FA$;D$(4),MID$(CO.NAME$,25,24),PAGE
9015 PRINT FB$
9020 PRINT FB1$
9025 PRINT FC$
9030 PRINT:P9=P9+6
9095 RETURN
10000 dr$="TOTAL DEBITS":cr$="TOTAL CREDITS"
10010 PRINT USING FF$;dr$,DEBIT
10020 PRINT USING FF$;cr$,CREDIT
10030 GOSUB 30000
10040 console
10050 CLOSE 1:CLOSE 2
10100 PRINT "PUT DISK #1 BACK IN DRIVE A - TYPE <cr> TO CONTINUE"
10105 INPUT line temp$
 if size("master11.int")<1 then 10100
10110 lprinter
10115 print chr$(12):for z=1 to 100:next z:print chr$(12) 
10120 console
10195 chain"master11"
20000 console:PRINT clear$:PRINT
20010 PRINT "*** VALIDATING ACCOUNT NUMBERS ***"
20020 PRINT:PRINT "PLEASE WAIT"
20030 I=1
20040 read #2,i;N(0,1),N(0,2),Q$,N(0,7),N(0,8),N(0,9),     N(0,10),N(0,11),N(0,12),N(0,13),N(0,14),N(0,15),N(0,16),     N(0,17),N(0,18),N(0,19),FLAG$
20050 IF N(0,2)=0 THEN 29975
20055 IF FLAG$="P" THEN 21010
20062 FOR Z4=1 TO Z2
     IF N(0,2)=H(Z4) THEN 20070
      NEXT Z4
20064 J=2:GOTO 20500
20070 CASH.OUT=CASH.OUT+N(0,7):ACCT.NUM=N(0,2)
20072 IF N(0,7)<0 THEN CREDIT=CREDIT+N(0,7):REF%(Z4)=REF%(Z4)+1
20074 IF N(0,7)>0 THEN DEBIT=DEBIT+N(0,7):REF%(Z4)=REF%(Z4)+1
20076 FOR J=first to last STEP 2
20080 FOR Z4=1 TO Z2
20085 IF N(0,J)=0 THEN 21000
20090 IF N(0,J)=H(Z4) THEN 21000
20100 NEXT Z4
20500 lprinter
 print:print n(0,j);" in record ";i;" is not a VALID account number!"
 abort = 1
 console
 IF J=2 THEN 20076
21000 IF N(0,J+1)<0 THEN CREDIT=CREDIT+N(0,J+1):REF%(Z4)=REF%(Z4)+1
21002 IF N(0,J+1)>0 THEN DEBIT=DEBIT+N(0,J+1):REF%(Z4)=REF%(Z4)+1
21004 NEXT J
21010 I=I+1
21020 GOTO 20040
29975 PRINT clear$:PRINT
 if abort = 1 then print "*** POSTING ABORTED !!! ***":GOTO 10050
29980 PRINT "*** POSTING ***"
29985 PRINT:PRINT "PLEASE WAIT"
29995 RETURN
30000 P9=100:PAGE=0
30005 IF P9>60 THEN GOSUB 31000
30010 OPEN "GLJOSIZE" AS 20:READ #20;LIMIT,DUMMY:CLOSE 20
 LIMIT=LIMIT-1
30100 FOR I=1 TO Z2
30200 IF REF%(I)=0 THEN 30795
30300 READ #1,I;N(1,1),N(1,2),R$,N(1,4),N(1,5),N(1,6),N(1,7), N(1,8),N(1,9),N(1,10)
30305 PRINT USING FJ$;N(1,2),R$,N(1,6):P9=P9+1
30310 CURRENT=0
30500 FOR K=1 TO LIMIT
30510 read #2,K;N(0,1),N(0,2),Q$,N(0,7),N(0,8),N(0,9),      N(0,10),N(0,11),N(0,12),N(0,13),N(0,14),N(0,15),N(0,16),      N(0,17),N(0,18),N(0,19),FLAG$
30512 IF FLAG$="P" THEN 30580
30515 IF N(0,2)=N(1,2) AND N(0,7)<>0 THEN PRINT USING FK$;LEFT$(Q$,5), MID$(Q$,9,24),N(0,1),N(0,7):FOR X=4 TO 6:N(1,X)=N(1,X)+N(0,7):NEXT X: CURRENT=CURRENT+N(0,7):P9=P9+1:REF%(I)=REF%(I)-1
30520 IF P9>60 THEN GOSUB 31000
30530 IF REF%(I)=0 THEN K=LIMIT:GOTO 30580
30550 FOR J1=FIRST TO LAST STEP 2
30555 IF N(0,J1)=N(1,2) AND N(0,J1+1)<>0 THEN PRINT USING FK$;LEFT$(Q$,5), MID$(Q$,9,24),N(0,1),N(0,J1+1):FOR X=4 TO 6:N(1,X)=N(1,X)+N(0,J1+1): NEXT X:CURRENT=CURRENT+N(0,J1+1):P9=P9+1:REF%(I)=REF%(I)-1
30560 IF P9>60 THEN GOSUB 31000
30566 IF REF%(I)=0 THEN K=LIMIT:GOTO 30580
30570 NEXT J1
30580 NEXT K
30600 PRINT USING FL$;CURRENT,N(1,6):P9=P9+1
30650 PRINT #1,I;N(1,1),N(1,2),R$,N(1,4),N(1,5),N(1,6),N(1,7), N(1,8),N(1,9),N(1,10)
30655 PRINT:PRINT:P9=P9+2
30660 IF P9>60 THEN GOSUB 31000
30795 NEXT I
30800 FOR I=1 TO LIMIT
30810 read #2,I;N(0,1),N(0,2),Q$,N(0,7),N(0,8),N(0,9),      N(0,10),N(0,11),N(0,12),N(0,13),N(0,14),N(0,15),N(0,16),      N(0,17),N(0,18),N(0,19),FLAG$
30815 FLAG$="P"
30820 print #2,I;N(0,1),N(0,2),Q$,N(0,7),N(0,8),N(0,9),      N(0,10),N(0,11),N(0,12),N(0,13),N(0,14),N(0,15),N(0,16),      N(0,17),N(0,18),N(0,19),FLAG$
30825 NEXT I
30995 RETURN
31000 PRINT CHR$(12):P9=0:PAGE=PAGE+1
31005 PRINT USING FG$;D$(4),MID$(CO.NAME$,25,24),PAGE
31010 PRINT FH$
31015 PRINT FH1$
31020 PRINT FI$
31025 PRINT:P9=P9+6
31095 RETURN
