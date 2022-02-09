1 rem This is the Check Disbursements Entry Sort Program


 rem This program creates a backup file and copies all undeleted
 rem records to that file. Simultaneously it creates a RAM resident
 rem list of either tag numbers, account numbers, or reference numbers
 rem -  S(xxxx) - and Relative Record KEYS - T(xxxx).

 rem The tag, account, or reference numbers are then sorted via a Shell
 rem Metzler algorithm and the Relative Record KEYS carried along.

 rem When the sort is complete, the Relative Record KEYS are used to
 rem restore the backed up records to the original file in sorted order.


%INCLUDE ALL.BAS

500 dim s(z2),t(z2),n(2,20)

 z5$="glcd":z6$="glcdback":z7$="glcdsize":RL=250

8000 INPUT "SORT ON TAG OR REFERENCE # (T OR A)";LINE SORT.KEY$
 IF SORT.KEY$="t" OR SORT.KEY$="T" THEN A9=1:GOTO 8020
 IF SORT.KEY$="a" OR SORT.KEY$="A" THEN A9=2:GOTO 8020
 GOTO 8000

8020 PRINT clear$:PRINT
 PRINT "*** BACKING UP FILE ***"
 PRINT:PRINT "*** NOTE - BACKUP IGNORES DELETED RECORDS ***"

 open z5$ recl RL as 1
 create z6$ recl RL as 2


 Z1=0

 FOR Z=1 TO Z2-1
 read #1,z;N(2,1),N(2,2),N$,N(2,7),N(2,8),   N(2,9),N(2,10),N(2,11),N(2,12),N(2,13),N(2,14),   N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
 IF N(2,1)=0 THEN 8065
 IF N(2,2)=0 THEN 8070
 Z1=Z1+1
 print #2,z1;N(2,1),N(2,2),N$,N(2,7),N(2,8),   N(2,9),N(2,10),N(2,11),N(2,12),N(2,13),N(2,14),   N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
 T(Z1)=Z1:S(Z1)=N(2,A9)
8065  NEXT Z
8070   close 1
   close 2

 PRINT clear$:PRINT
 PRINT "*** SORTING ON";
 IF A9=1 THEN PRINT " TAG ***"
 IF A9=2 THEN PRINT " REFERENCE NUMBER ***":PRINT
 PRINT "THIS SORT TAKES FROM 30 SECONDS FOR 100 RECORDS"
 PRINT "TO 10 MINUTES FOR 1000 RECORDS":PRINT
 PRINT "*** PLEASE WAIT ***":PRINT


 rem This is the actual sort routine

 N9=Z1
 M9=N9
8515 M9=INT(M9/2)
 IF M9=0 THEN 8580
  J9=1:K9=N9-M9

8530 I9=J9
8535 L9=I9+M9
 IF S(I9)<S(L9) THEN 8565

  Z=S(I9):S(I9)=S(L9):S(L9)=Z
  Z=T(I9):T(I9)=T(L9):T(L9)=Z
  I9=I9-M9

 IF I9<1 THEN 8565

 GOTO 8535

8565 J9=J9+1
 IF J9>K9 THEN 8515
 GOTO 8530


8580 PRINT clear$:PRINT
 PRINT "SORT COMPLETED - YOU MAY STILL ABORT":PRINT
 PRINT "IF YOU WISH TO ABORT - JUST TYPE ABORT":PRINT
 PRINT "OTHERWISE TYPE A CARRIAGE RETURN TO CONTINUE !"
 PRINT
  INPUT LINE TEMP$
  IF TEMP$="abort" OR TEMP$="ABORT" THEN 10000

   if T(1)=0 then 10000

 open z5$ recl RL as 1
 open z6$ recl RL as 2
  open z7$ as 3:read #3;z2,z3:close 3


 Z2=Z1+1

 FOR Z=1 TO Z2-1
 READ #2,T(Z);N(2,1),N(2,2),N$,N(2,7),N(2,8),   N(2,9),N(2,10),N(2,11),N(2,12),N(2,13),N(2,14),   N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
 PRINT #1,Z;N(2,1),N(2,2),N$,N(2,7),N(2,8),   N(2,9),N(2,10),N(2,11),N(2,12),N(2,13),N(2,14),   N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
 NEXT Z

   for z=1 to 20:n(2,z)=0:next z:n$=""

 FOR Z=Z2 TO Z3+2
 print #1,z;N(2,1),N(2,2),N$,N(2,7),N(2,8),   N(2,9),N(2,10),N(2,11),N(2,12),N(2,13),N(2,14),   N(2,15),N(2,16),N(2,17),N(2,18),N(2,19),FLAG$
 NEXT Z
  close 1
  delete 2
  open z7$ as 3:print #3;z2,z3:close 3

chain "glcdtran"


10000 open z6$ recl RL as 1
 delete 1
 chain "glcdtran"
